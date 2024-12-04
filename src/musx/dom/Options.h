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

class FontInfo;

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
    DefaultFonts(const DocumentWeakPtr& document) : OptionsBase(document) {}

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
    std::shared_ptr<FontInfo> getFontInfo(FontType type) const
    {
        auto it = defaultFonts.find(type);
        if (it == defaultFonts.end()) {
            throw std::invalid_argument("Font type " + std::to_string(int(type)) + " not found in document");
        }
        return it->second;
    }

    /**
     * @brief get the `FontInfo` for a particular type from the document pool
     * @param document the `Document` to search
     * @param type the `FontType` to retrieve
     * @return a shared pointer to the font info for that type
     * @throws std::invalid_paremter if the type is not found in the document
     */
    static std::shared_ptr<FontInfo> getFontInfo(const DocumentPtr& document, FontType type)
    {
        auto options = document->getOptions();
        if (!options) {
            throw std::invalid_argument("No options found in document");
        }
        auto defaultFonts = options->get<DefaultFonts>();
        if (!defaultFonts) {
            throw std::invalid_argument("Default fonts not found in document");
        }
        return defaultFonts->getFontInfo(type);
    }

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "fontOptions";
};


} // namespace options
} // namespace dom
} // namespace musx