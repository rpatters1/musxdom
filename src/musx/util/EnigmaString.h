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

namespace musx {

namespace dom {
class FontInfo;
} // namespace dom

namespace util {

/**
 * @brief Static class that provides utilties to extract information from enigma strings.
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
         *
         * Suitable for musical notation systems or specialized fonts that support SMuFL.
         */
        Smufl
    };

    /** @brief Returns true if the enigma string starts with a font command. */

    static bool startsWithFontCommand(const std::string& text);
    
    /**
     * @brief Parses an enigma text insert into its constituent components.
     *
     * The function takes an enigma text insert starting with `^` and extracts the command
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
     * @return A vector of strings representing the command and its parameters, or an empty vector if invalid.
     */
    static std::vector<std::string> parseComponents(const std::string& input, size_t* parsedLength = nullptr);

    /// @brief Iteration function type that the parser calls back when the font has changed
    /// - text: the chunk of text styled with the specified font information
    /// - font: the font information.
    using TextChunkCallback = std::function<bool(
        const std::string& text,
        const std::shared_ptr<dom::FontInfo>& font
    )>;

    /// @brief Iteration function type that the parser calls back when it encounters an Enigma command
    /// that requires text subtitution. If the function returns std::nullopt, the command is dumped as-is
    /// into the processed text. If the function returns an empty string, the command is stripped from the
    /// processed text.
    /// - parsedCommand: a vector containing the command (without the leading '^') and all its parameters.
    using CommandCallback = std::function<std::optional<std::string>(
        const std::vector<std::string>& parsedCommand
    )>;

    /**
     * @brief Parses an Enigma-formatted string, handling font commands and escaped carets.
     *
     * This function scans an Enigma-formatted string (typically Finale-style encoded text),
     * extracts text chunks and control commands, and invokes a callback for each contiguous span
     * of text using the current font state. It automatically interprets font-related commands 
     * like ^fontTxt, ^fontMus, ^fontid, ^size, and ^nfx, updating the font info accordingly.
     *
     * Escaped carets ("^^") are converted to literal '^' characters. All other unrecognized
     * commands (e.g., ^value(...) or ^page(...)) are passed through to the callback for higher-level
     * processing or markup.
     *
     * @param rawText The full input Enigma string to parse.
     * @param onText The handler for when font styling changes.
     * @param onCommand The handler to substitute text for a command.
     */
    static void parseEnigmaText(const DocumentPtr& document, const std::string& rawText, const TextChunkCallback& onText, const CommandCallback& onCommand);

    /// @brief Simplified version of #parseEnigmaText that strips unhandled commands.
    /// Useful in particular when the caller only cares about font information.
    /// @param rawText The full input Enigma string to parse.
    /// @param onText The handler for when font styling changes.
    static void parseEnigmaText(const std::string& rawText, const TextChunkCallback& onText)
    {
        parseEnigmaText(rawText, onText, [](const std::vector<std::string>&) -> std::optional<std::string> {
            return ""; // strip all unhandled commands
        });
    }

    /**
     * @brief Incorporates an enigma font command into the supplied @ref dom::FontInfo instance.
     *
     * Enigma font commands are
     * - one of the font identifying commands, such as `^font`, `fontid`, etc. (See @ref startsWithFontCommand.)
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
