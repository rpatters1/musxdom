/*
 * Copyright (C) 2025, Robert Patterson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#pragma once

#include <string>
#include <stdexcept>
#include <vector>

#if defined(_WIN32)
#include <windows.h>
#else
#include <iconv.h>
#include <cerrno>
#include <cstring>
#endif

namespace text_encoding {

namespace detail {

/**
 * @brief Maps a Windows code page integer to an iconv "from" encoding name.
 *
 * @param codepage A Windows code page number (e.g., 1252, 65001).
 * @return A null-terminated iconv encoding name string, or nullptr if unsupported.
 */
inline const char* codepageToIconvFrom(int codepage)
{
    switch (codepage) {

    // Unicode
    case 65001: return "UTF-8";        // CP_UTF8
    case 1200:  return "UTF-16LE";     // Windows "Unicode"
    case 1201:  return "UTF-16BE";

    // Windows ANSI code pages
    case 1250:  return "CP1250";       // Central Europe
    case 1251:  return "CP1251";       // Cyrillic
    case 1252:  return "CP1252";       // Western European
    case 1253:  return "CP1253";       // Greek
    case 1254:  return "CP1254";       // Turkish
    case 1255:  return "CP1255";       // Hebrew
    case 1256:  return "CP1256";       // Arabic
    case 1257:  return "CP1257";       // Baltic
    case 1258:  return "CP1258";       // Vietnamese

    // ISO encodings
    case 28591: return "ISO-8859-1";   // Latin-1

    // East Asian
    case 932:   return "SHIFT-JIS";    // Japanese
    case 936:   return "GB2312";       // Simplified Chinese
    case 949:   return "EUC-KR";       // Korean
    case 950:   return "BIG5";         // Traditional Chinese

    // Classic Mac encodings
    case 10000: return "MACINTOSH";    // MacRoman

    // OEM (rare but possible)
    case 437:   return "CP437";        // DOS Latin US

    default:
        return nullptr;
    }
}

#if !defined(_WIN32)

/**
 * @brief Converts a byte string from a given encoding to UTF-8 using iconv.
 *
 * @param fromEncoding iconv encoding name describing the input bytes.
 * @param source       Input bytes.
 * @return UTF-8 bytes on success, std::nullopt on failure.
 */
inline std::optional<std::string>
convertToUtf8WithIconv(const char* fromEncoding, const std::string& source)
{
    if (source.empty()) {
        return std::string{};
    }

    iconv_t converter = iconv_open("UTF-8", fromEncoding);
    if (converter == (iconv_t)-1) {
        return std::nullopt;
    }

    std::vector<char> output(source.size() * 4 + 8);

    const char* inputPtrConst = source.data();
    char* inputPtr = const_cast<char*>(inputPtrConst);
    size_t inputRemaining = source.size();

    char* outputPtr = output.data();
    size_t outputRemaining = output.size();

    while (true) {
        size_t result = iconv(converter, &inputPtr, &inputRemaining,
                              &outputPtr, &outputRemaining);

        if (result != static_cast<size_t>(-1)) {
            break;
        }

        if (errno == E2BIG) {
            const size_t used = static_cast<size_t>(outputPtr - output.data());
            output.resize(output.size() * 2);
            outputPtr = output.data() + used;
            outputRemaining = output.size() - used;
            continue;
        }

        iconv_close(converter);
        return std::nullopt;
    }

    iconv_close(converter);

    const size_t bytesProduced =
        static_cast<size_t>(outputPtr - output.data());

    return std::string(output.data(), bytesProduced);
}

#endif // !defined(_WIN32)

} // namespace detail

/**
 * @brief Converts a byte string encoded in the specified Windows code page into UTF-8.
 *
 * This is the single cross-platform API:
 * - Windows: MultiByteToWideChar → WideCharToMultiByte(CP_UTF8)
 * - macOS/Linux: iconv with an explicit code page → encoding-name mapping
 *
 * @param codepage Windows code page number describing the encoding of @p source.
 * @param source   Input bytes in the specified code page.
 * @return UTF-8 encoded bytes on success, std::nullopt on failure.
 *
 * @note The returned string contains UTF-8 bytes and may contain embedded '\0'.
 */
inline std::optional<std::string>
toUtf8FromCodepage(int codepage, const std::string& source)
{
    if (source.empty()) {
        return std::string{};
    }

#if defined(_WIN32)

    int wideLength = MultiByteToWideChar(
        static_cast<UINT>(codepage),
        MB_ERR_INVALID_CHARS,
        source.data(),
        static_cast<int>(source.size()),
        nullptr,
        0
        );
    if (wideLength <= 0) {
        return std::nullopt;
    }

    std::wstring wideString(static_cast<size_t>(wideLength), L'\0');
    if (MultiByteToWideChar(
            static_cast<UINT>(codepage),
            MB_ERR_INVALID_CHARS,
            source.data(),
            static_cast<int>(source.size()),
            wideString.data(),
            wideLength) != wideLength) {
        return std::nullopt;
    }

    int utf8Length = WideCharToMultiByte(
        CP_UTF8,
        0,
        wideString.data(),
        wideLength,
        nullptr,
        0,
        nullptr,
        nullptr
        );
    if (utf8Length <= 0) {
        return std::nullopt;
    }

    std::string utf8(static_cast<size_t>(utf8Length), '\0');
    if (WideCharToMultiByte(
            CP_UTF8,
            0,
            wideString.data(),
            wideLength,
            utf8.data(),
            utf8Length,
            nullptr,
            nullptr) != utf8Length) {
        return std::nullopt;
    }

    return utf8;

#else

    if (codepage == 0) {
        // Try utf8 and hope it works...
        if (auto u8out = detail::convertToUtf8WithIconv("UTF-8", source)) {
            return u8out;
        }
        // ... or use a common ANSI value. We have no way to know the correct one.
        return detail::convertToUtf8WithIconv("CP1252", source);
    }


    const char* fromEncoding = detail::codepageToIconvFrom(codepage);
    if (!fromEncoding) {
        return std::nullopt;
    }

    return detail::convertToUtf8WithIconv(fromEncoding, source);

#endif
}

} // namespace text_encoding
