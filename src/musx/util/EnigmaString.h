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
#include <string_view>
#include <vector>
#include <functional>
#include <memory>
#include <unordered_set>

#include "musx/dom/Fundamentals.h"

namespace musx {

namespace dom {
class FontInfo;
class Document;
} // namespace dom

namespace util {

/// @class EnigmaStyles
/// @brief Text styles for enigma strings
struct EnigmaStyles
{
    /// @enum CategoryTracking
    /// @brief Specifies is the current enigma style tracks an expressions marking category font.
    /// When the marking category font changes, the Finale U.I. searchs all enigma strings for the category
    /// and modifies any tracked fonts to match the new settings in the marking category. See @ref musx::dom::others::MarkingCategory.
    /// @note Finale never implemented number fonts in the U.I., and it is possible they do not occur in real-world Finale files.
    /// However, the enigma data structures support number fonts.
    enum class CategoryTracking
    {
        None,           ///< no category tracking
        MusicFont,      ///< tracks the category's music font
        TextFont,       ///< tracks the category's text font
        NumberFont      ///< tracks the category's number font (not implemented in Finale U.I.)
    };

    /// @brief constructor
    EnigmaStyles(const std::weak_ptr<dom::Document>& document)
        : font(std::make_shared<dom::FontInfo>(document))
    {
    }

    std::shared_ptr<dom::FontInfo> font;    ///< the font to use
    CategoryTracking categoryFont{};        ///< how this font is tracked against a marking category
    dom::Evpu baseline{};                   ///< baseline setting (positive means up)
    dom::Evpu superscript{};                ///< superscript setting added to #baseline (positive means up)
    int tracking{};                         ///< inter-character tracking in EMs (1/1000 font size)
};

/**
 * @brief Static class that provides utilities to extract information from enigma strings. Enigma strings
 * use text inserts delineated by a preceding caret (^) and parenthesis for parameters. Here is a list of
 * known inserts.
 *
 * <b>Text formatting</b>
 * - `^font(name[, encoding])`: sets the font face. The optional encoding usually specifies mac (4095) or win (2) symbol encoding.
 * Font IDs (especially zero) can be specified as name in the format "FontXX" where XX is the font Id. Typically this occurs for the default
 * music font ("Font0").
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

    /// @brief Concerts a 32-bit codepoint to a utf8-encoded std::string.
    static std::string toU8(char32_t cp);

    /**
     * @brief Enumeration to specify the default handling for accidental insert commands. Like all Enigma commands,
     * accidental insert commands are passed to the TextInsertCallback function if there is one. These options determine
     * how the insert is handled if the callback opts not to handle it.
     */
    enum class AccidentalInsertHandling
    {
        /**
         * @brief Parse accidental insert commands into glyph font changes and character strings.
         *
         * Suitable when the parser should produce fully parsed output ready for rendering.
         */
        ParseToGlyphs,

        /**
         * @brief Substitute accidental insert commands with textual representations.
         *
         * The substitution style is determined by the `substitutionStyle` field in EnigmaParsingOptions.
         * Suitable for environments where accidentals should be represented as text rather than as font changes.
         */
        Substitute
    };

    /**
     * @brief Enumeration to specify the type of accidental substitution representation.
     *
     * Defines how accidentals are represented in text when substitution is selected.
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
         * Suitable for environments without Unicode or SMuFL support.
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

    /**
     * @brief Options for configuring how Enigma strings are parsed.
     *
     * This struct encapsulates all parsing configuration for Enigma string processing,
     * including accidental insert handling behavior and substitution style.
     */
    struct EnigmaParsingOptions
    {
        /// @brief constructor
        EnigmaParsingOptions() :
            insertHandling(AccidentalInsertHandling::ParseToGlyphs),
            substitutionStyle(AccidentalStyle::Unicode) {}

        /// @brief constructor for accidental substitution
        EnigmaParsingOptions(AccidentalStyle accidentalStyle) :
            insertHandling(AccidentalInsertHandling::Substitute),
            substitutionStyle(accidentalStyle) {}

        /**
         * @brief Specifies how accidental insert commands are handled during parsing.
         *
         * Defaults to ParseToGlyphs, which parses insert commands into font changes
         * and glyph strings.
         */
        AccidentalInsertHandling insertHandling;

        /**
         * @brief Specifies the accidental substitution style used when insertHandling is Substitute.
         *
         * Ignored unless insertHandling is set to Substitute.
         * Defaults to Unicode substitution.
         */
        AccidentalStyle substitutionStyle;

        /**
         * @brief Specifies whether to strip unknown tags or dump them into the output string.
         */
        bool stripUnknownTags = true;

        /**
         * @brief If value is true, font & style tags are ignored. Note that you may still get
         * get font and style changes for accidental inserts.
         */
        bool ignoreStyleTags = false;

        /**
         * @brief Skip tags in this set. Primarily used by the "partname" insert to avoid a situation where
         * the partname itself contains a `^partname` tag. (Though the Finale UI prevents this, a
         * plugin might possibly do it in theory.)
         */
        std::unordered_set<std::string_view> ignoreTags;
    };

    /** @brief Returns true if the enigma string starts with a font insert. */
    static bool startsWithFontCommand(const std::string& text);

    /** @brief Returns true if the enigma string starts with a style insert. */
    static bool startsWithStyleCommand(const std::string& text);
    
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
     * parseComponents("^unbalanced(abc");        // Returns {}
     * @endcode
     *
     * @param input The enigma text insert to parse.
     * @param parsedLength If supplied, returns the number of characters parsed
     * @return A vector of strings representing the insert and its parameters, or an empty vector if invalid.
     */
    static std::vector<std::string> parseComponents(const std::string& input, size_t* parsedLength = nullptr);

    /// @brief Iteration function type that the parser calls back when the font has changed or when recursively parsing
    /// an insert that is itself an enigma string, such as (in particular) the part name.
    /// - text: the chunk of text styled with the specified font information
    /// - font: the font information.
    using TextChunkCallback = std::function<bool(
        const std::string& text,
        const EnigmaStyles& styles
    )>;

    /// @brief Iteration function type that the parser calls back when it encounters an Enigma text insert
    /// that requires text subtitution. If the function returns an empty string, the insert is stripped from the
    /// processed text.
    ///
    /// If the function returns std::nullopt, the Enigma parsing function inserts an appropriate
    /// value. Therefore, TextInsertCallback functions should only process known commands and return std::nullopt
    /// for any others.
    ///
    /// - parsedCommand: a vector containing the insert (without the leading '^') and each of its parameters.
    using TextInsertCallback = std::function<std::optional<std::string>(
        const std::vector<std::string>& parsedCommand
    )>;

    /// @brief Inserts callback to take all default insert subsitutions determined by #parseEnigmaText.
    static inline TextInsertCallback defaultInsertsCallback = [](const std::vector<std::string>&) { return std::nullopt; };

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
     * The ^date, ^time, and ^fdate inserts default to POSIX functions for formatting date and time on macOS and Linux.
     * On Windows they default to WinAPI. If you need finer control over date or time formatting, you should handle
     * these yourself.
     *
     * @param document The document from which the enigma string is taken.
     * @param rawText The full input Enigma string to parse.
     * @param onText The handler for when font styling changes.
     * @param onInsert The handler to substitute text for an insert.
     * @param options Parsing options.
     * @return true if parsing completed, false if aborted by the @p onText function.
     */
    static bool parseEnigmaText(const std::shared_ptr<dom::Document>& document, dom::Cmper partId, const std::string& rawText,
        const TextChunkCallback& onText, const TextInsertCallback& onInsert,
        const EnigmaParsingOptions& options = {})
    {
        return parseEnigmaTextImpl(document, partId, rawText, onText, onInsert, options, EnigmaStyles(document));
    }

    /// @brief Simplified version of #parseEnigmaText that strips unhandled inserts.
    /// Useful in particular when the caller only cares about the raw text or the font information.
    /// @param document The document from which the enigma string is taken.
    /// @param rawText The full input Enigma string to parse.
    /// @param onText The handler for when font styling changes.
    /// @param options Parsing options.
    /// @return true if parsing completed, false if aborted by the @p onText function.
    static bool parseEnigmaText(const std::shared_ptr<dom::Document>& document, dom::Cmper partId, const std::string& rawText, const TextChunkCallback& onText,
        const EnigmaParsingOptions& options = {})
    {
        return parseEnigmaTextImpl(document, partId, rawText, onText, defaultInsertsCallback, options, EnigmaStyles(document));
    }

    /**
     * @brief Incorporates an enigma font insert into the supplied @ref dom::FontInfo instance.
     *
     * Enigma font inserts are
     * - one of the font identifying inserts, such as `^font`, `fontid`, etc. (See @ref startsWithFontCommand.)
     * - `^size` specifies the font size in points.
     * - `^nfx` specifies a bit mask of style properties. These are resolved with @ref dom::FontInfo::setEnigmaStyles.
     */
    static bool parseStyleCommand(std::vector<std::string> components, EnigmaStyles& styles);

    /** @brief Trims all enigma tags from an enigma string, leaving just the plain text. */
    static std::string trimTags(const std::string& input);

private:
    static bool parseEnigmaTextImpl(const std::shared_ptr<dom::Document>& document, dom::Cmper partId, const std::string& rawText,
    const TextChunkCallback& onText, const TextInsertCallback& onInsert,
    const EnigmaParsingOptions& options, const EnigmaStyles& startingStyles);
};

} // namespace util
} // namespace musx
