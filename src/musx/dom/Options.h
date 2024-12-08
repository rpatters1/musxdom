/*
 * Copyright (C) 2024, Robert Patterson
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
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {
namespace options {

/**
 * @class DefaultFonts
 * @brief An unordered map of default font settings for the document.
 *
 * The DefaultFonts class stores default font settings used throughout the document. It provides functionality
 * to manage various font attributes, such as size, style (bold, italic), and whether the font is hidden or fixed size.
 */
class DefaultFonts : public OptionsBase
{
public:
    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     *
     * Constructs a DefaultFonts object that is associated with the provided document.
     */
    explicit DefaultFonts(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    /**
     * @enum FontType
     * @brief Types of fonts available for the document.
     *
     * The FontType enum represents different types of fonts used for musical and textual elements within the document.
     */
    enum class FontType
    {
        Music,             ///< Default music font.
        Key,               ///< Key signature font.
        Clef,              ///< Clef symbol font.
        Time,              ///< Time signature font.
        Chord,             ///< Chord name font.
        ChordAcci,         ///< Chord accidental font.
        Ending,            ///< Ending font.
        Tuplet,            ///< Tuplet font.
        TextBlock,         ///< Text block font.
        LyricVerse,        ///< Lyric verse font.
        LyricChorus,       ///< Lyric chorus font.
        LyricSection,      ///< Lyric section font.
        MultiMeasRest,     ///< Multi-measure rest font.
        Tablature,         ///< Tablature font.
        ChordSuffix,       ///< Chord suffix font.
        Expression,        ///< Expression font.
        Repeat,            ///< Repeat symbol font.
        Fretboard,         ///< Fretboard font.
        Flags,             ///< Flags font.
        Accis,             ///< Accidental symbols font.
        AltNotSlash,       ///< Alternate notation slash font.
        AltNotNum,         ///< Alternate notation number font.
        Rests,             ///< Rests notation font.
        ReptDots,          ///< Repeat dots font.
        Noteheads,         ///< Notehead symbols font.
        AugDots,           ///< Augmentation dots font.
        TimePlus,          ///< Additional time signature symbols font.
        Articulation,      ///< Articulation symbols font.
        Percussion,        ///< Percussion font.
        SmartShape8va,     ///< 8va smart shape font.
        MeasNumb,          ///< Measure number font.
        StaffNames,        ///< Staff names font.
        AbbrvStaffNames,   ///< Abbreviated staff names font.
        GroupNames,        ///< Group names font.
        SmartShape8vb,     ///< 8vb smart shape font.
        SmartShape15ma,    ///< 15ma smart shape font.
        SmartShape15mb,    ///< 15mb smart shape font.
        SmartShapeTrill,   ///< Trill smart shape font.
        SmartShapeWiggle,  ///< Wiggle smart shape font.
        AbbrvGroupNames,   ///< Abbreviated group names font.
        BendCurveFull,     ///< Full bend curve font.
        BendCurveWhole,    ///< Whole bend curve font.
        BendCurveFrac,     ///< Fractional bend curve font.
        TimeParts,         ///< Time signature parts font.
        TimePlusParts      ///< Additional time signature parts font.
    };

    /**
     * @brief Stores the default fonts for different elements.
     *
     * An unordered map that associates each `FontType` with its corresponding `FontInfo` settings.
     */
    std::unordered_map<FontType, std::shared_ptr<FontInfo>> defaultFonts;

    /**
     * @brief get the `FontInfo` for a particular type
     * @param type the `FontType` to retrieve
     * @return a shared pointer to the font info for that type
     * @throws std::invalid_paremter if the type is not found in the document
     */
    std::shared_ptr<FontInfo> getFontInfo(FontType type) const;

    /**
     * @brief get the `FontInfo` for a particular type from the document pool
     * @param document the `Document` to search
     * @param type the `FontType` to retrieve
     * @return a shared pointer to the font info for that type
     * @throws std::invalid_paremter if the type is not found in the document
     */
    static std::shared_ptr<FontInfo> getFontInfo(const DocumentPtr& document, FontType type);

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "fontOptions";
};

/**
 * @class PageFormatOptions
 * @brief Options for page formatting in the document.
 * 
 * This class provides configuration options for page formatting, including 
 * scope adjustment, page format score, and page format parts.
 */
class PageFormatOptions : public OptionsBase {
public:
    /**
     * @enum AdjustPageScope
     * @brief Enum for the scope of page adjustments.
     */
    enum class AdjustPageScope {
        Current,        ///< Adjust the current page only.
        All,            ///< Adjust all pages.
        LeftOrRight,    ///< Adjust left or right pages.
        PageRange       ///< Adjust page range. (xml value is "range")
    };

    /**
     * @class PageFormat
     * @brief Represents the format settings for a page.
     */
    class PageFormat
    {
    public:
        Evpu pageHeight{};               ///< Height of the page.
        Evpu pageWidth{};                ///< Width of the page.
        int pagePercent{};               ///< Page scaling percentage (a value of 100 means no scaling).
        int sysPercent{};                ///< System scaling percentage (a value of 100 means no scaling).
        int rawStaffHeight{};            ///< Raw staff height (in 1/16 Evpu units).
        Evpu leftPageMarginTop{};        ///< Top margin for the left page. (Sign reversed in Finale UI.)
        Evpu leftPageMarginLeft{};       ///< Left margin for the left page.
        Evpu leftPageMarginBottom{};     ///< Bottom margin for the left page.
        Evpu leftPageMarginRight{};      ///< Right margin for the left page. (Sign reversed in Finale UI.)
        Evpu rightPageMarginTop{};       ///< Top margin for the right page. (Sign reversed in Finale UI.)
        Evpu rightPageMarginLeft{};      ///< Left margin for the right page.
        Evpu rightPageMarginBottom{};    ///< Bottom margin for the right page.
        Evpu rightPageMarginRight{};     ///< Right margin for the right page. (Sign reversed in Finale UI.)
        Evpu sysMarginTop{};             ///< System top margin. (Sign reversed in Finale UI.)
        Evpu sysMarginLeft{};            ///< System left margin.
        Evpu sysMarginBottom{};          ///< System bottom margin.
        Evpu sysMarginRight{};           ///< System bottom margin. (Sign reversed in Finale UI.)
        Evpu sysDistanceBetween{};       ///< Distance between systems. (Sign reversed in Finale UI.)
        Evpu firstPageMarginTop{};       ///< Top margin for the first page. (Sign reversed in Finale UI.)
        Evpu firstSysMarginTop{};        ///< Top margin for the first system. (Sign reversed in Finale UI.)
        Evpu firstSysMarginLeft{};       ///< Left margin for the first system.
        Evpu firstSysMarginDistance{};   ///< Distance between the first systems. (Sign reversed in Finale UI.)
        bool facingPages{};              ///< Whether to use the right page margin values.
        bool differentFirstSysMargin{};  ///< Whether to use the first system values.
        bool differentFirstPageMargin{}; ///< Whether to use the `firstPageMarginTop` value.

        /**
         * @brief Default constructor for PageFormat.
         */
        PageFormat() = default;
    };

    // Properties
    AdjustPageScope adjustPageScope{AdjustPageScope::Current}; ///< Scope of page adjustments.
    std::shared_ptr<PageFormat> pageFormatScore; ///< Page format for score settings.
    std::shared_ptr<PageFormat> pageFormatParts; ///< Page format for parts settings.
    bool avoidSystemMarginCollisions{}; ///< Whether to avoid system margin collisions.

    /**
     * @brief Constructor for PageFormatOptions.
     * 
     * @param document A shared pointer to the document.
     */
    explicit PageFormatOptions(const DocumentPtr& document)
        : OptionsBase(document) {}

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "pageFormatOptions";
};

} // namespace options
} // namespace dom
} // namespace musx