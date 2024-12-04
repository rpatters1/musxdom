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
namespace others {

/**
 * @class Enclosure
 * @brief Represents the enclosure settings for text expressions.
 */
class Enclosure : public OthersBase
{
public:
    /**
     * @enum Shape
     * @brief Enumeration for enclosure shapes.
     */
    enum class Shape : uint8_t
    {
        NoEnclosure = 0,    ///< No enclosure
        Rectangle = 1,      ///< Rectangle
        Ellipse = 2,        ///< Ellipse
        Triangle = 3,       ///< Triangle
        Diamond = 4,        ///< Diamond
        Pentagon = 5,       ///< Pentagon
        Hexagon = 6,        ///< Hexagon
        Heptagon = 7,       ///< Heptagon
        Octogon = 8         ///< Octogon
    };

    /**
     * @brief Constructs an Enclosure object.
     * @param document Shared pointer to the document.
     * @param cmper Comparison parameter.
     */
    Enclosure(const DocumentWeakPtr& document, Cmper cmper)
        : OthersBase(document, cmper)
    {
    }

    Evpu xAdd{};              ///< Center X offset - offsets text from center (in EVPU).
    Evpu yAdd{};              ///< Center Y offset - offsets text from center (in EVPU).
    Evpu xMargin{};           ///< Half width - extra space on left/right sides (in EVPU).
    Evpu yMargin{};           ///< Half height - extra space on top/bottom sides (in EVPU).
    Efix lineWidth{};         ///< Line thickness in 64ths of an EVPU (EFIX).
    Shape shape{Shape::NoEnclosure}; ///< Enclosure shape (default: NoEnclosure).
    Efix cornerRadius{};      ///< Corner radius (in EFIX).
    bool fixedSize{};         ///< Whether the enclosure is fixed size (ignore text bounding box)
    bool notTall{};           ///< "Enforce Minimum Width": don't let shape get taller than it is wide
    bool opaque{};            ///< Whether the enclosure is opaque.
    bool roundCorners{};      ///< Whether the enclosure has rounded corners.
};

/**
 * @class FontDefinition
 * @brief The name and font characteristics of fonts contained in the musx file.
 *
 * The cmper is the font ID used in classes throughout the document.
 *
 * This class is identified by the XML node name "fontName".
 */
class FontDefinition : public OthersBase
{
public:
    /** @brief Constructor function */
    FontDefinition(const DocumentWeakPtr& document, int cmper)
        : OthersBase(document, cmper)
    {
    }

    // Public properties corresponding to the XML structure
    std::string charsetBank; ///< probably only "Mac" or "Win"
    int charsetVal = 0;      ///< A value specifying the character set, usually 4095 or 0
    int pitch = 0;           ///< Represents the `<pitch>` element, e.g., 0. (use unknown)
    int family = 0;          ///< Represents the `<family>` element, e.g., 0. (use unknown)
    std::string name;        ///< The font name e.g., "Broadway Copyist Text".

    constexpr static std::string_view XmlNodeName = "fontName"; ///< The XML node name for this type.
};
#include <string>

/**
 * @class TextExpressionDef
 * @brief Stores the properties and behaviors of text expressions.
 *
 * This class is identified by the XML node name "textExprDef".
 */
class TextExpressionDef : public OthersBase
{
public:
    /**
     * @enum PlaybackType
     * @brief Specifies the playback behavior for the text expression.
     */
    enum class PlaybackType
    {
        None,                   ///< Default value, no playback (the default).
        Tempo,                  ///< Playback affects tempo.  (xml value is "time")
        MidiController,         ///< Playback type for MIDI controller commands.
        KeyVelocity,            ///< Playback affects key velocity. (xml value is "amplitude")
        Transpose,              ///< Playback causes transposition.
        Channel,                ///< Playback set the MIDI channel.
        MidiPatchChange,        ///< Playback changes the MIDI patch.
        PercussionMidiMap,      ///< Playback uses percussion MIDI map. (xml value is "percMidiMap")
        MidiPitchWheel,         ///< Playback affects the MIDI pitch wheel. (xml value is "midiPitchwheel")
        Dump,                   ///< Playback is an arbitrary data dump. (Data is in <playDumpText> with the same Cmper value.)
        PlayTempoToolChanges,   ///< Play changes from Tempo Tool. (xml value is "startTempo")
        IgnoreTempoToolChanges, ///< Ignore changes from Tempo Tool. (xml value is "stopTempo")
        Swing,                  ///< Playback in swing style
        SmartPlaybackOn,        ///< Turn on smart playback. (xml value is "hpOn")
        SmartPlaybackOff,       ///< Turn off smart playback. (xml value is "hpOff")
    };

    /**
     * @enum HorizontalMeasExprAlign
     * @brief Specifies the horizontal alignment relative to musical elements.
     */
    enum class HorizontalMeasExprAlign
    {
        Manual,                 ///< "Horizontal Click Position" alignment.
        LeftOfAllNoteheads,     ///< Align left of all noteheads
        LeftOfPrimaryNotehead,  ///< Align to the left of the primary notehead.
        Stem,                   ///< Align to the stem.
        CenterPrimaryNotehead,  ///< Align to the center of the primary notehead.
        CenterAllNoteheads,     ///< Align to the center of all noteheads.
        RightOfAllNoteheads,    ///< Align to the right of all noteheads.
        LeftBarline,            ///< Align with left barline (the default). (xml value is "leftEdge", if encountered)
        StartTimeSig,           ///< Align at the start of the time signature.
        AfterClefKeyTime,       ///< Align after clef/key/time/repeat.
        StartOfMusic,           ///< Align at start of music.
        CenterOverBarlines,     ///< Align over the center of the barlines.
        CenterOverMusic,        ///< Align over the center of music.
        RightBarline            ///< Align with left barline. (xml value is "rightEdge")
    };

    /**
     * @enum VerticalMeasExprAlign
     * @brief Specifies the vertical alignment relative to musical elements.
     */
    enum class VerticalMeasExprAlign
    {
        Manual,              ///< "Vertical Click Position" alignment.
        RefLine,             ///< Align to staff reference line.
        AboveStaff,          ///< Align above ths staff (the default).
        BelowStaff,          ///< Align below the staff.
        TopNote,             ///< Align with top note.
        BottomNote,          ///< Align with bottom note.
        AboveEntry,          ///< Align above entry.
        BelowEntry,          ///< Align below entry.
        AboveStaffOrEntry,   ///< Align above the staff or entry.
        BelowStaffOrEntry    ///< Align below the staff or entry.
    };

    /**
     * @enum HorizontalExprAlign
     * @brief Specifies the horizontal alignment for text expressions.
     */
    enum class HorizontalExprJustification
    {
        Left,    ///< Justified left.
        Center,  ///< Justified center.
        Right    ///< Justified right.
    };

    /// Properties
    Cmper textIDKey{};                              ///< Identifier for the #TextBlock associated with this 
    int categoryID{};                               ///< Identifier for the category of the text expression.
    int value{};                                    ///< Value associated with the expression (e.g., velocity).
    int auxData1{};                                 ///< Auxiliary data for the expression.
    PlaybackType playbackType{ PlaybackType::None }; ///< Playback behavior of the text expression.
    HorizontalMeasExprAlign horzMeasExprAlign{ HorizontalMeasExprAlign::LeftBarline }; ///< Horizontal alignment of the expression.
    HorizontalExprJustification horzExprJustification{ HorizontalExprJustification::Left }; ///< Horizontal justification of the text expression.
    VerticalMeasExprAlign vertMeasExprAlign{ VerticalMeasExprAlign::AboveStaff }; ///< Vertical alignment of the expression.
    int measXAdjust{};                              ///< Horizontal adjustment for measurement alignment.
    int yAdjustEntry{};                             ///< Vertical adjustment for entry alignment.
    int yAdjustBaseline{};                          ///< Vertical adjustment for baseline alignment.
    bool useCategoryFonts{};                        ///< Whether to use category fonts.
    bool useCategoryPos{};                          ///< Whether to use category position.
    bool hasEnclosure{};                            ///< Whether the text expression has an enclosure.
    bool breakMmRest{};                             ///< Whether the text breaks multimeasure rests.
    bool useAuxData{};                              ///< Whether auxiliary data is used.
    std::string descStr;                            ///< Description of the text expression.

    /**
     * @brief Constructor.
     *
     * Initializes all fields to their default values.
     */
    TextExpressionDef(const DocumentWeakPtr& document, Cmper cmper)
        : OthersBase(document, cmper) {}

    std::shared_ptr<Enclosure> getEnclosure() const;

    constexpr static std::string_view XmlNodeName = "textExprDef"; ///< The XML node name for this type.
};

/**
 * @class TextExpressionEnclosure
 * @brief The enclosure for a text expression (if it exists)
 *
 * The cmper is the same as for the associated #TextExpressionDef.
 *
 * This class is identified by the XML node name "textExpressionEnclosure".
 */
class TextExpressionEnclosure : public Enclosure
{
public:
    using Enclosure::Enclosure;

    constexpr static std::string_view XmlNodeName = "textExpressionEnclosure"; ///< The XML node name for this type.
};

/**
 * @class TextRepeatEnclosure
 * @brief The enclosure for a text expression (if it exists)
 *
 * The cmper is the same as for #TextRepeateDef.
 *
 * This class is identified by the XML node name "textRepeatEnclosure".
 */
class TextRepeatEnclosure : public Enclosure
{
public:
    using Enclosure::Enclosure;

    constexpr static std::string_view XmlNodeName = "textRepeatEnclosure"; ///< The XML node name for this type.
};

} // namespace others
} // namespace dom
} // namespace musx