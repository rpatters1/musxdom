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
#include <vector>
#include <functional>
#include <memory>

namespace musx {

namespace dom {
class FontInfo;
class Document;
} // namespace dom

namespace util {

/**
 * @brief Static class that provides utilities to extract information from enigma strings. Enigma strings
 * use text inserts delineated by a preceding caret (^) and parenthesis for parameters. Here is a list of
 * known inserts.
 *
 * <b>Text formatting</b>
 * - `^font(name[, encoding])`: sets the font face. The optional encoding usually specifies mac (4095) or win (2) symbol encoding.
 * - `^fontid(fontId[, encoding])`: sets the font face using the font id within the document.
 * - `^Font(name[, encoding])`: variant of `^font`.
 * - `^fontMus(name[, encoding])`: sets the font face but indicates that it tracks the marking category's Music Font setting.
 * - `^fontTxt(name[, encoding])`: sets the font face but indicates that it tracks the marking category's Text Font setting.
 * - `^fontNum(name[, encoding])`: sets the font face but indicates that it tracks the marking category's Number Font setting.
 * (The number font was never implemented in Finale's UI.)
 * - `^size(points)`: sets the font size in points. Only integer values are allowed.
 * - `^nfx(mask)`: a bitmask of style bits
 * - `^baseline(evpu)`: adjusts the baseline up (positive) or down (negative) by the given Evpu value.
 * - `^superscript(evpu)`: adjusts the baseline up (positive) or down (negative) by the given Evpu value. (Added to baseline value.)
 * - `^tracking(em)`: adjusts the inter-letter spacing by the given EM value. (1/1000 of font size.)
 *
 * <b>Accidental inserts</b>
 * - `^flat()`: inserts a flat using settings in @ref musx::dom::options::TextOptions.
 * - `^natural()`: inserts a natural using settings in @ref musx::dom::options::TextOptions.
 * - `^sharp()`: inserts a sharp using settings in @ref musx::dom::options::TextOptions.
 * - `^dbflat()`: inserts a double flat using settings in @ref musx::dom::options::TextOptions.
 * - `^dbsharp()`: inserts a double sharp using settings in @ref musx::dom::options::TextOptions.
 *
 * <b>Expression playback inserts</b>
 * - `^value()`: inserts the playback value from the associated text or shape expression. (See @ref musx::dom::others::TextExpressionDef::value.)
 * - `^control()`: inserts the playback controller value from the associated text or shape expression. (See @ref musx::dom::others::TextExpressionDef::auxData1.)
 * - `^pass()`: inserts the repeat pass value from the associated text or shape expression. (See @ref musx::dom::others::TextExpressionDef::playPass.)
 *
 * <b>Data/time inserts</b>
 * - `^date(format)`: inserts the current date where format 0=short, 1=long, 2=abbreviated. (See @ref musx::dom::options::TextOptions::DateFormat.)
 * - `^fdate(format)`: inserts the file modified date where the format is the same as for `^date`.
 * - `^time(seconds)`: inserts the current time where seconds 0=omit seconds, nonzero (normally 1)=include seconds.
 * The OS locale settings determine if it is rendered with AM/PM or 24-hour times. (See @ref musx::dom::options::TextOptions::showTimeSeconds.)
 * - `^perftime(format)`: inserts the total performance time. For a list of format values, see the following note.
 *
 * @note The Finale U.I. does not seem to have a mechanism to modify the `^perftime` format from its default value of 4. However, plugins
 * can easily create an Enigma string with any of the format values. They are as follows.
 * - 0: M:SS (e.g., `1:02`)
 * - 1: HH:MM:SS (e.g., `00:01:02`)
 * - 2: HH:MM:SS.mmm (e.g., `00:01:02.000`)
 * - 3: MM:SS (e.g., `01:02`)
 * - 4: M&apos;SS&quot; (e.g., `1'02"`)
 * - 5: M (e.g., `1`)
 *
 * <b>Other text subsitution inserts</b>
 * - `^^`: inserts a caret (^).
 * - `^arranger()`: inserts the arranger name from File Info (ScoreManager window).
 * - `^composer()`: inserts the composer name from File Info (ScoreManager window).
 * - `^copyright()`: inserts the copyright text from File Info (ScoreManager window).
 * - `^cprsym()`: inserts a copyright (@) symbol.
 * - `^description()`: inserts the description text from File Info (ScoreManager window).
 * - `^filename()`: inserts the file name (no path).
 * - `^lyricist()`: inserts the lyricist name from File Info (ScoreManager window).
 * - `^page(offset)`: inserts the current page number, offsetting from the offset parameter. (The Finale UI prevents more than one `^page` insert per Enigma string.)
 * - `^subtitle()`: inserts the subtitle from File Info (ScoreManager window).
 * - `^partname()`: score or part name. (Score uses the score text from File Info.)
 * - `^title()`: inserts the title from File Info (ScoreManager window).
 * - `^totpages()`: inserts the total number of pages in the document.
*/
class EnigmaString
{
public:
    /// @brief Convert u8"" literals to std::string in a way that works both in C++17 and C++20+
    template <typename CharT>
    static std::string fromU8(const CharT* s)
    {
        static_assert(std::is_same<CharT, char>::value || std::is_same<CharT, decltype(u8'a')>::value,
            "fromU8() only accepts char or char8_t pointers");
        return std::string(reinterpret_cast<const char*>(s));
    }

    /**
     * @brief Enumeration to specify the type of accidental replacement.
     *
     * This enum class is used to choose between different representations
     * of musical accidentals when processing text.
     */
    enum class AccidentalStyle
    {
        /**
         * @brief Use ASCII substitutions for accidentals.
         *
         * - Flat: 'b'
         * - Sharp: '#'
         * - Natural: (empty string)
         * - Double Flat: 'bb'
         * - Double Sharp: 'x'
         *
         * Suitable for environments where Unicode or SMuFL support is unavailable.
         */
        Ascii,

        /**
         * @brief Use Unicode text accidentals.
         *
         * - Flat: ♭ (U+266D)
         * - Sharp: ♯ (U+266F)
         * - Natural: ♮ (U+266E)
         * - Double Flat: 𝄫 (U+1D12B)
         * - Double Sharp: 𝄪 (U+1D12A)
         *
         * Suitable for inline text representations like "Clarinet in B♭."
         */
        Unicode,

        /**
         * @brief Use SMuFL notation-specific accidentals.
         *
         * - Flat: (U+E260)
         * - Sharp: (U+E262)
         * - Natural: (U+E261)
         * - Double Flat: (U+E264)
         * - Double Sharp: (U+E263)
         *
         * Suitable for musical notation systems or specialized fonts that support SMuFL.
         */
        Smufl
    };

    /** @brief Returns true if the enigma string starts with a font insert. */

    static bool startsWithFontCommand(const std::string& text);
    
    /**
     * @brief Parses an enigma text insert into its constituent components.
     *
     * The function takes an enigma text insert starting with `^` and extracts the insert
     * and its parameters. If the string is invalid or unbalanced, it returns an empty vector.
     *
     * Examples:
     * @code{.cpp}
     * parseComponents("^fontTxt(Times,4096)");   // Returns {"fontTxt", "Times", "4096"}
     * parseComponents("^size(10)");              // Returns {"size", "10"}
     * parseComponents("^nfx(130,(xyz))");        // Returns {"nfx", "130", "(xyz)"}
     * parseComponents("^some");                  // Returns {"some"}
     * parseComponents("^^");                     // Returns {"^"}
     * parseComponents("^^invalid");              // Returns {}
     * parseComponents("^unbalanced(abc");        // Returns {}
     * @endcode
     *
     * @param input The enigma text insert to parse.
     * @param parsedLength If supplied, returns the number of characters parsed
     * @return A vector of strings representing the insert and its parameters, or an empty vector if invalid.
     */
    static std::vector<std::string> parseComponents(const std::string& input, size_t* parsedLength = nullptr);

    /// @brief Iteration function type that the parser calls back when the font has changed
    /// - text: the chunk of text styled with the specified font information
    /// - font: the font information.
    using TextChunkCallback = std::function<bool(
        const std::string& text,
        const std::shared_ptr<dom::FontInfo>& font
    )>;

    /// @brief Iteration function type that the parser calls back when it encounters an Enigma text insert
    /// that requires text subtitution. If the function returns std::nullopt, the insert is dumped as-is
    /// into the processed text. If the function returns an empty string, the insert is stripped from the
    /// processed text.
    /// - parsedCommand: a vector containing the insert (without the leading '^') and all its parameters.
    using CommandCallback = std::function<std::optional<std::string>(
        const std::vector<std::string>& parsedCommand
    )>;

    /**
     * @brief Parses an Enigma-formatted string, handling font inserts and escaped carets.
     *
     * This function scans an Enigma-formatted string (typically Finale-style encoded text),
     * extracts text chunks and control inserts, and invokes a callback for each contiguous span
     * of text using the current font state. It automatically interprets font-related inserts 
     * like ^fontTxt, ^fontMus, ^fontid, ^size, and ^nfx, updating the font info accordingly.
     *
     * Escaped carets ("^^") are converted to literal '^' characters. All other unrecognized
     * inserts (e.g., ^value(...) or ^page(...)) are passed through to the callback for higher-level
     * processing or markup.
     *
     * @param document The document from which the enigma string is taken.
     * @param rawText The full input Enigma string to parse.
     * @param onText The handler for when font styling changes.
     * @param onCommand The handler to substitute text for an insert.
     * @param accidentalStyle If supplied, accidentals are replaced with characters according to the accidental style
     */
    static void parseEnigmaText(const std::shared_ptr<dom::Document>& document, const std::string& rawText,
        const TextChunkCallback& onText, const CommandCallback& onCommand,
        const std::optional<AccidentalStyle>& accidentalStyle = std::nullopt);

    /// @brief Simplified version of #parseEnigmaText that strips unhandled inserts.
    /// Useful in particular when the caller only cares about font information.
    /// @param document The document from which the enigma string is taken.
    /// @param rawText The full input Enigma string to parse.
    /// @param onText The handler for when font styling changes.
    /// @param accidentalStyle If supplied, accidentals are replaced with characters according to the accidental style.
    static void parseEnigmaText(const std::shared_ptr<dom::Document>& document, const std::string& rawText, const TextChunkCallback& onText,
        const std::optional<AccidentalStyle>& accidentalStyle = std::nullopt)
    {
        parseEnigmaText(document, rawText, onText, [](const std::vector<std::string>&) -> std::optional<std::string> {
            return ""; // strip all unhandled inserts
        }, accidentalStyle);
    }

    /**
     * @brief Incorporates an enigma font insert into the supplied @ref dom::FontInfo instance.
     *
     * Enigma font inserts are
     * - one of the font identifying inserts, such as `^font`, `fontid`, etc. (See @ref startsWithFontCommand.)
     * - `^size` specifies the font size in points.
     * - `^nfx` specifies a bit mask of style properties. These are resolved with @ref dom::FontInfo::setEnigmaStyles.
     */
    static bool parseFontCommand(const std::string& fontTag, dom::FontInfo& fontInfo, size_t* parsedLength = nullptr);

    /** @brief Trims all font tags from an enigma string. */
    static std::string trimFontTags(const std::string& input);

    /** @brief Trims all enigma tags from an enigma string, leaving just the plain text. */
    static std::string trimTags(const std::string& input);

    /** @brief Replaces ^flat() and ^sharp() inserts with 'b' and '#'. */
    static std::string replaceAccidentalTags(const std::string& input, AccidentalStyle style = AccidentalStyle::Ascii);
};

} // namespace util
} // namespace musx
