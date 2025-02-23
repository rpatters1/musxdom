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
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <map>
#include <tuple>

#include "musx/util/EnigmaString.h"
#include "musx/util/Logger.h"
#include "musx/util/Fraction.h"

#include "BaseClasses.h"
#include "CommonClasses.h"
// do not add other dom class dependencies. Use Implementations.cpp for implementations that need total class access.

namespace musx {
namespace dom {

class Entry;

namespace details {
class GFrameHold;
class StaffGroup;
}

/**
 * @namespace musx::dom::others
 * @brief Classes in the @ref OthersPool.
 */
namespace others {

/**
 * @class ClefList
 * @brief Represents an element in multimeasure clef list with its positioning and percentage values.
 *
 * The cmper is obtained from the @ref details::GFrameHold instance for the staff and measure. Inci 0
 * is the initial clef at the barline.
 *
 * This class is identified by the XML node name "clefEnum".
 */
class ClefList : public OthersBase {
public:
    /** @brief Constructor function */
    explicit ClefList(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci) {}
        
    // Public properties corresponding to the XML structure, in the same order as in the XML.
    ClefIndex clefIndex{};  ///< The 0-based clef index from the `<clef>` element.
    Edu xEduPos{};          ///< The xEduPos value from the `<xEduPos>` element.
    Evpu yEvpuPos{};        ///< The yEvpuPos value from the `<yEvpuPos>` element.
    int percent{};          ///< The percentage value from the `<percent>` element.
    int xEvpuOffset{};      ///< The xEvpuOffset value from the `<xEvpuOffset>` element.
    ShowClefMode clefMode{}; ///< The clef mode from the `<clefMode>` element.
    bool unlockVert{};      ///< "Allow Vertical Drag"
    bool afterBarline{};    ///< "Place Clef After Barline"
    
    constexpr static std::string_view XmlNodeName = "clefEnum"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ClefList>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FontDefinition
 * @brief The name and font characteristics of fonts contained.
 *
 * The cmper is the font ID used in classes throughout the document.
 *
 * This class is identified by the XML node name "fontName".
 */
class FontDefinition : public OthersBase
{
    static constexpr uint32_t SYMBOL_CHARSET_MAC = 0xfff; // (4095)
    static constexpr uint32_t SYMBOL_CHARSET_WIN = 2;

public:
    /** @brief Constructor function */
    explicit FontDefinition(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /// @enum CharacterSetBank
    /// @brief The character set for the bank
    enum class CharacterSetBank
    {
        MacOS,          /// xml value is "Mac"
        Windows         /// xml value is "Win"
    };

    // Public properties corresponding to the XML structure
    CharacterSetBank charsetBank{}; ///< The character set bank.
    int charsetVal{};               ///< A value specifying the character set. The meaning is dependent on #charsetBank.
                                    ///< A value of 0 indicates ANSI character set for Windows and MacRoman for macOS.
                                    ///< A value of 2 (`SYMBOL_CHARSET`) indicates a symbol font for Windows.
                                    ///< A value of 4095 (0xfff) indicates a symbol font for macOS.
                                    ///< Other values are possible but less likely.
    int pitch{};                    ///< Represents the `<pitch>` element, e.g., 0. (use unknown)
    int family{};                   ///< Represents the `<family>` element, e.g., 0. (use unknown)
    std::string name;               ///< The font name e.g., "Broadway Copyist Text".

    /**
     * @brief Calculates if this font is a symbol font.
     *
     * The code points in symbol fonts
     * are mapped directly to the glyph index of the font without any attempt at
     * linguistic mapping.
     *
     * Many legacy symbol fonts in Finale were music fonts following more-or-less the layout
     * of Adobe Sonata font, so this may be an appropriate proxy for telling if the font is a legacy
     * music font, at least in some contexts. The same font face (e.g., Engraver) may appear as
     * either a Windows symbol font or a macOS symbol font, depending on the origin of the font
     * and of the document.
    */
    bool calcIsSymbolFont() const
    {
        if (charsetBank == CharacterSetBank::MacOS && charsetVal == SYMBOL_CHARSET_MAC) {
            return true;
        }
        if (charsetBank == CharacterSetBank::Windows && charsetVal == SYMBOL_CHARSET_WIN) {
            return true;
        }
        return false;
    }

    constexpr static std::string_view XmlNodeName = "fontName"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FontDefinition>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class Frame
 * @brief Represents the attributes of a TGF entry frame.
 *
 * The inci is almost always zero. It appears there can be multiple incis when there is an old-style pickup.
 * In this case, the observed order of Frame incis is
 * - `inci 0`: startTime
 * - `inci 1`: startEntry & endEntry
 *
 * The class is identified by the XML node name "frameSpec".
 */
class Frame : public OthersBase
{
public:
    /** @brief Constructor function. */
    explicit Frame(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci = 0)
        : OthersBase(document, partId, shareMode, cmper, inci) {}

    // Public properties corresponding to the XML structure
    EntryNumber startEntry{}; ///< Start entry number for this frame. (Appears to be zero when #startTime is supplied.)
    EntryNumber endEntry{};   ///< End entry number for this frame. (Appears to be zero when #startTime is supplied.)
    Edu startTime{};          ///< The starting position within the measure. (Appears to be zero when #startEntry and #endEntry are supplied.)

    /** @brief Returns a vector of entries contained in the frame.
     *
     * These are raw entries. Use #details::GFrameHold::createEntryFrame for a vector of entries with computed values.
     */
    std::vector<std::shared_ptr<const Entry>> getEntries();

    void integrityCheck() override
    {
        this->OthersBase::integrityCheck();
        if (startTime && (startEntry || endEntry)) {
            MUSX_INTEGRITY_ERROR("Frame " + std::to_string(getCmper()) + " has non-zero startTime and non-zero startEntry or endEntry.");
        }
        if (!startTime && !startEntry) {
            MUSX_INTEGRITY_ERROR("Frame " + std::to_string(getCmper()) + " has no start entry.");
        }
        if (!startTime && !endEntry) {
            MUSX_INTEGRITY_ERROR("Frame " + std::to_string(getCmper()) + " has no end entry.");
        }
    }

    constexpr static std::string_view XmlNodeName = "frameSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Frame>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
};

class Staff;
/**
 * @class InstrumentUsed
 * @brief An array of InstrumentUsed defines a set of staves in a staff system or in Scroll View.
 *
 * This class is identified by the XML node name "instUsed".
 */
class InstrumentUsed : public OthersBase {
public:
    /** @brief Constructor function */
    explicit InstrumentUsed(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci) {}

    InstCmper staffId{};                    ///< Staff cmper (xml node is `<inst>`)
    Evpu distFromTop{};                     ///< Distance from the top of the system (negative is down)
    std::shared_ptr<MusicRange> range;      ///< The music range. (Late versions of Finale may always include the entire piece here.)

    /// @brief Returns the @ref Staff instance for this element
    std::shared_ptr<Staff> getStaff() const;

    /// @brief Returns the @ref Staff instance at a specified index of iuArray or nullptr if not found
    /// @param iuArray And array of @ref InstrumentUsed instances, representing a staff system or staff view (e.g., Scroll View)
    /// @param index The 0-based index to find.
    static std::shared_ptr<Staff> getStaffAtIndex(const std::vector<std::shared_ptr<InstrumentUsed>>& iuArray, Cmper index);

    /// @brief Returns the 0-based index of the InstCmper or std::nullopt if not found.
    /// @param iuArray And array of @ref InstrumentUsed instances, representing a staff system or staff view (e.g., Scroll View)
    /// @param staffId The @ref Staff cmper value to find.
    static std::optional<size_t> getIndexForStaff(const std::vector<std::shared_ptr<InstrumentUsed>>& iuArray, InstCmper staffId);

    constexpr static std::string_view XmlNodeName = "instUsed"; ///< The XML node name for this type.
    static const xml::XmlElementArray<InstrumentUsed>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class LayerAttributes
 * @brief Represents the attributes of a Finale "layer".
 *
 * The cmper is the layer ID (0-3). In the Finale UI these are represented as Layer 1 thru Layer 4.
 * The 0-based cmper value is used to identify layers where needed, rather than the 1-based UI value.
 *
 * This class is identified by the XML node name "layerAtts".
 */
class LayerAttributes : public OthersBase {
public:
    /** @brief Constructor function */
    explicit LayerAttributes(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    int restOffset{};                   ///< "Adjust Floating Rests by " number of staff steps.
    bool freezTiesToStems{};            ///< "Freeze Ties in the Same Direction as Stems" (xml node is `<flipTies>`)
    bool onlyIfOtherLayersHaveNotes{};  ///< "Apply Settings Only if Notes are in Other Layers" (xml node is `<floatLayer>`)
    bool useRestOffset{};               ///< "Adjust Floating Rests by ". Determines if `restOffset` is used. (xml node is `<useRestOff>`)
    bool freezeStemsUp{};               ///< If true, freeze stems up, otherwise freeze stems down. Only used if `freezeLayer` is true. (xml node is `<freezeLayUp>`)
    bool freezeLayer{};                 ///< "Freeze Stems and Ties"
    bool playback{};                    ///< If true, playback this layer. If false, the layer is muted.
    bool affectSpacing{};               ///< "Affect Music Spacing" (xml node is `<spacing>`)
    bool ignoreHiddenNotesOnly{};       ///< "Ignore Layers Containing Only Hidden Notes" (xml node is `<ignoreHidden>`)
    bool ignoreHiddenLayers{};          ///< "Ignore Hidden Layers"
    bool hideLayer{};                   ///< "Hide Layer when Inactive"

    constexpr static std::string_view XmlNodeName = "layerAtts"; ///< The XML node name for this type.
    static const xml::XmlElementArray<LayerAttributes>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @enum RehearsalMarkStyle
 * @brief Specifies the sequencing style for rehearsal marks
 */
enum class RehearsalMarkStyle
{
    None,                       ///< Default value, no rehearsal mark style
    Letters,                    ///< A, B, C...AA, BB, CC
    LetterNumbers,              ///< A, B, C...A1, B1, C1
    LettersLowerCase,           ///< a, b, c...aa, bb, cc
    LettersNumbersLowerCase,    ///< a, b, c...a1, b1, c1
    Numbers,                    ///< 1, 2, 3, 4
    MeasureNumber               ///< Displays the current measure number
};

/**
 * @enum PlaybackType
 * @brief Specifies the playback behavior for the an expression or marking category.
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
    ChannelPressure,        ///< Playback affects MIDI channel pressure. (xml value is "midiPressure")
    RestrikeKeys,           ///< Playback retrikes keys. (xml value is "rekey")
    Dump,                   ///< Playback is an arbitrary data dump. (Data is in node `<playDumpText>` with the same @ref Cmper value.)
    PlayTempoToolChanges,   ///< Play changes from Tempo Tool. (xml value is "startTempo")
    IgnoreTempoToolChanges, ///< Ignore changes from Tempo Tool. (xml value is "stopTempo")
    Swing,                  ///< Playback in swing style
    SmartPlaybackOn,        ///< Turn on smart playback. (xml value is "hpOn")
    SmartPlaybackOff,       ///< Turn off smart playback. (xml value is "hpOff")
};

/**
 * @enum HorizontalMeasExprAlign
 * @brief Specifies the horizontal alignment relative to musical elements for an expression or marking category.
 */
enum class HorizontalMeasExprAlign
{
    LeftBarline,            ///< Align with left barline (the default). (xml value is "leftEdge", if encountered)
    Manual,                 ///< "Horizontal Click Position" alignment.
    LeftOfAllNoteheads,     ///< Align left of all noteheads
    LeftOfPrimaryNotehead,  ///< Align to the left of the primary notehead.
    Stem,                   ///< Align to the stem.
    CenterPrimaryNotehead,  ///< Align to the center of the primary notehead.
    CenterAllNoteheads,     ///< Align to the center of all noteheads.
    RightOfAllNoteheads,    ///< Align to the right of all noteheads.
    StartTimeSig,           ///< Align at the start of the time signature.
    AfterClefKeyTime,       ///< Align after clef/key/time/repeat.
    StartOfMusic,           ///< Align at start of music.
    CenterOverBarlines,     ///< Align over the center of the barlines.
    CenterOverMusic,        ///< Align over the center of music.
    RightBarline            ///< Align with left barline. (xml value is "rightEdge")
};

/**
 * @enum VerticalMeasExprAlign
 * @brief Specifies the vertical alignment relative to musical elements for an expression or marking category.
 */
enum class VerticalMeasExprAlign
{
    AboveStaff,          ///< Align above ths staff (the default).
    Manual,              ///< "Vertical Click Position" alignment.
    RefLine,             ///< Align to staff reference line.
    BelowStaff,          ///< Align below the staff.
    TopNote,             ///< Align with top note.
    BottomNote,          ///< Align with bottom note.
    AboveEntry,          ///< Align above entry.
    BelowEntry,          ///< Align below entry.
    AboveStaffOrEntry,   ///< Align above the staff or entry.
    BelowStaffOrEntry    ///< Align below the staff or entry.
};

/**
 * @enum HorizontalTextJustification
 * @brief Specifies the horizontal alignment for text expressions and marking categories.
 */
enum class HorizontalTextJustification
{
    Left,    ///< Justified left.
    Center,  ///< Justified center.
    Right    ///< Justified right.
};

class ShapeExpressionDef;
class TextExpressionDef;

/**
 * @class MarkingCategory
 * @brief Represents a category of markings used.
 *
 * This class is identified by the XML node name "markingsCategory".
 */
class MarkingCategory : public OthersBase {
public:
    /** @brief Enumeration for the type of marking category */
    enum class CategoryType : Cmper
    {
        Invalid,            ///< There should always be a category type supplied
        Dynamics,           ///< Dynamics markings, such as forte, piano, etc.
        TempoMarks,         ///< Tempo indications such as Allegro (with or without metronome marking).
        TempoAlterations,   ///< Tempo alteration markings, such as accel. and rit. (xml value is "tempoAlts")
        ExpressiveText,     ///< Expressive text such as "espressivo."
        TechniqueText,      ///< Technique text such as "pizzicato" or "arco."
        RehearsalMarks,     ///< Rehearsal marks, often used for reference points in the score.
        Misc                ///< Represents miscellaneous markings that do not fit into other categories.
    };

    /** @brief Constructor function */
    explicit MarkingCategory(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    CategoryType categoryType{}; ///< Category type of the marking

    // Font information for the marking category
    std::shared_ptr<FontInfo> textFont;      ///< Text font
    std::shared_ptr<FontInfo> musicFont;     ///< Music font
    std::shared_ptr<FontInfo> numberFont;    ///< Number font

    HorizontalMeasExprAlign horzAlign{};     ///< Horizontal alignment for the marking
    VerticalMeasExprAlign vertAlign{};       ///< Vertical alignment for the marking
    HorizontalTextJustification justification{}; ///< Justification for the text within the marking

    // Vertical and horizontal offsets for positioning adjustments
    Evpu horzOffset{};         ///< Additional horizontal offset
    Evpu vertOffsetBaseline{}; ///< Additional vertical offset
    Evpu vertOffsetEntry{};    ///< Additional vertical entry offset

    // Usage flags representing certain behaviors and visual elements
    bool usesTextFont{};      ///< whether this category uses the text font
    bool usesMusicFont{};     ///< whether this category uses the music font
    bool usesNumberFont{};    ///< whether this category uses the number font
    bool usesPositioning{};   ///< whether this category uses the positioning elements (Finale UI only allows true)
    bool usesStaffList{};     ///< whether this category uses a staff list
    bool usesBreakMmRests{};  ///< whether this category uses the `breakMmRest` setting
    bool breakMmRest{};       ///< whether expressions in this category break multimeasure rests
    bool userCreated{};       ///< whether the user created this category

    // Staff list represented as an integer
    Cmper staffList{};        ///< The staff list if `useStaffList` is true

    /** @brief A list of shape expressions in this category.
     *
     * (This in not in the xml but is created by the factory.)
     */
    std::map<Cmper, std::weak_ptr<ShapeExpressionDef>> shapeExpressions;

    /** @brief A list of text expressions in this category.
     *
     * (This in not in the xml but is created by the factory.)
     */
    std::map<Cmper, std::weak_ptr<TextExpressionDef>> textExpressions;

    /** @brief gets the name of the marking category */
    std::string getName() const;

    constexpr static std::string_view XmlNodeName = "markingsCategory"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MarkingCategory>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MarkingCategoryName
 * @brief Represents the name associated with a @ref MarkingCategory.
 *
 * This class has the same #Cmper as its @ref MarkingCategory.
 *
 * This class is identified by the XML node name "markingsCategoryName".
 */
class MarkingCategoryName : public OthersBase {
public:
    /** @brief Constructor function */
    explicit MarkingCategoryName(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::string name; ///< The name of the marking category.

    constexpr static std::string_view XmlNodeName = "markingsCategoryName"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MarkingCategoryName>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class Measure
 * @brief Represents the attributes of a measure.
 *
 * This class is identified by the XML node name "measSpec".
 */
class Measure : public OthersBase {
public:
    /** @brief Constructor function */
    explicit Measure(const DocumentWeakPtr& document, Cmper measureId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, measureId, shareMode, cmper) {}

    /// @enum PositioningType
    /// @brief Positioning (music spacing) type for the measure
    enum class PositioningType
    {
        Manual,                     ///< "Manually (By Dragging)" the default.
        TimeSignature,              ///< Legacy option "Use the Time Signature" (xml node is `<timesig>`)
        BeatChart,                  ///< Legacy option "Use a Beat Positioning Chart" (xml node is `<beatchart>`)
        TimeSigPlusPositioning,     ///< "According to the Time Signature" (xml node is `<timesigPlusPos>`)
        BeatChartPlusPositioning    ///< "Using Beat-Chart Spacing" (xml node is `<beatchartPlusPos>`)
    };

    /// @enum BarlineType
    /// @brief Barline type for left and right barlines.
    enum class BarlineType
    {
        None,                   ///< No barline. (This is the default value for the enum.)
        OptionsDefault,         ///< Left barlines only: use value from @ref options::BarlineOptions (xml values is "default")
        Normal,                 ///< Normal barline
        Double,                 ///< Double barline
        Final,                  ///< Final barline
        Solid,                  ///< Solid (thick) barline
        Dashed,                 ///< Dashed barline (xml value is "dash")
        Tick,                   ///< Right barlines only: tick barline (xml value is "partial")
        Custom                  ///< Custom barline (created with Shape Designer)
    };

    /// @enum ShowKeySigMode
    /// @brief How and whether to show a Key Signature in this measure.
    enum class ShowKeySigMode
    {
        IfNeeded,               ///< Show if needed (the default)
        Always,                 ///< Always show (xml value is "deltaKey")
        Never                   ///< Never show (xml value is "ignoreKey")
    };

    /// @enum ShowTimeSigMode
    /// @brief How and whether to show a Time Signature in this measure.
    enum class ShowTimeSigMode
    {
        IfNeeded,               ///< Show if needed (the default)
        Always,                 ///< Always show (xml value is "deltaTime")
        Never                   ///< Never show (xml value is "ignoreTime")
    };

    Evpu width{};               ///< "Ideal" measure width in Evpu. Page layout determines actual width.
    std::shared_ptr<KeySignature> keySignature; ///< the key signature on this measure. Guaranteed to be non-null. (xml node is `<keySig>`)
    Cmper beats{};              ///< Number of beats in the measure or the Cmper to a `timesigUpper` composite numerator list.
    Cmper divBeat{};            ///< Divisions per beat (Edu) or the Cmper to a `timesigLower` composite denominator list.
    Cmper dispBeats{};          ///< Displayed beats in the measure or the Cmper to a `timesigUpper` composite numerator list.
    Cmper dispDivbeat{};        ///< Displayed divisions per beat (Edu) or the Cmper to a `timesigLower` composite denominator list.
    Evpu frontSpaceExtra{};     ///< Extra space at front of bar.
    Evpu backSpaceExtra{};      ///< Extra space at end of bar.
    bool breakWordExt{};        ///< Barline ends word extensions on lyrics.
    bool hideCaution{};         ///< "Hide Cautionary Clefs, Key, and Time Signature"
    bool showFullNames{};       ///< "Show Full Staff & Group Names"
    bool allowSplitPoints{};    ///< "Allow Horizontal Split Points" (xml node is `<posSplit>`)
    bool groupBarlineOverride{}; ///< Override the barline specified by a @ref details::StaffGroup (if any)
    Cmper customBarShape{};     ///< Cmper of Shape Designer shape for custom right barline
    Cmper customLeftBarShape{}; ///< Cmper of Shape Designer shape for custom left barline
    ShowKeySigMode showKey{};   ///< Show mode for key signatures
    ShowTimeSigMode showTime{}; ///< Show mode for time signatures
    PositioningType positioningMode{}; ///< Positioning type for the measure. (xml node is `<posMode>`)
    bool beginNewSystem{};      ///< "Begin a New Staff System" (xml node is `<lineBreak>`)
    bool breakMmRest{};         ///< "Break a Multimeasure Rests" (xml node is `<breakRest>`)
    bool noMeasNum{};           ///< Inverse of "Include in Measure Numbering"
    BarlineType barlineType{};  ///< Barline type. (xml node is `<barline>`)
    bool hasSmartShape{};       ///< Indicates if the measure has a smart shape.
    bool evenlyAcrossMeasure{}; ///< "Position Evenly Across Measure" (xml node is `<indivPosDef>`)
    bool hasExpression{};       ///< Indicates if the measure has an expression assigned. See @ref MeasureExprAssign. (xml node is `<hasExpr>`)
    bool forwardRepeatBar;      ///< Indicates a forward repeat bar on this measure. (xml node is `<forRepBar>`)
    bool backwardsRepeatBar;    ///< Indicates a forward repeat bar on this measure. (xml node is `<bacRepBar>`)
    bool hasEnding;             ///< Indicates the presence of a repeat ending. (xml node is `<barEnding>`)
    bool hasTextRepeat;         ///< Indicates the presence of one or more text repeat assigments. (xml node is `<txtRepeats>`)
    bool compositeNumerator{};  ///< Indicates a composite numerator for the time signature. (xml node is `<altNumTsig>`)
    bool compositeDenominator{}; ///< Indicates a composite denominator for the time signature. (xml node is `<altDenTsig>`)
    bool abbrvTime{};           ///< Indicates abbreviated time signature (e.g., Common or Cut time.) Applies to the display time signature only.
                                ///< The actual time signature's abbreviation is controlled by the values in @ref options::TimeSignatureOptions.
    bool useDisplayTimesig{};   ///< Indicates whether to use the display time signature.
    BarlineType leftBarlineType{}; ///< Left barline type. (xml node is `<leftBarline>`)
    bool compositeDispNumerator{};  ///< Indicates a composite numerator for the display time signature. (xml node is `<displayAltNumTsig>`)
    bool compositeDispDenominator{}; ///< Indicates a composite denominator for the display time signature. (xml node is `<displayAltDenTsig>`)

    /// @brief Calculates if a measure should show full names vs. abbreviated names
    bool calcShouldShowFullNames() const
    { return getCmper() == 1 || showFullNames; }

    /// @brief Calculates the visible number of the measure, based on the first MeasureNumberRegion that contains it.
    ///
    /// Depending on the display options of the found MeasureNumberRegion, the number may or may not appear in the score.
    /// And if it does, it may not appear as a number.
    int calcDisplayNumber() const;

    /// @brief Create a shared pointer to an instance of the @ref TimeSignature for this measure
    std::shared_ptr<TimeSignature> createTimeSignature() const
    {
        return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, compositeNumerator, compositeDenominator));
    }

    /// @brief Create a shared pointer to an instance of the display @ref TimeSignature for this measure
    /// @return The diplay time signature if there is one, otherwise `nullptr`.
    std::shared_ptr<TimeSignature> createDisplayTimeSignature() const
    {
        if (!useDisplayTimesig) {
            return nullptr;
        }
        return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivbeat, compositeDispNumerator, compositeDispDenominator, abbrvTime));
    }

    void integrityCheck() override
    {
        this->OthersBase::integrityCheck();
        if (!keySignature) {
            keySignature = std::make_shared<KeySignature>(getDocument());
        }
    }

    bool requireAllFields() const override { return false; } ///< @todo: remove this override after identifying all fields.

    constexpr static std::string_view XmlNodeName = "measSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Measure>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MeasureExprAssign
 * @brief Assigns a @ref TextExpressionDef or @ref ShapeExpressionDef to a measure
 *
 * Only one of #textExprId or #shapeExprId is non-zero.
 *
 * The Cmper for a MeasureExprAssign is the cmper of the Measure to which it is attached.
 */
class MeasureExprAssign : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit MeasureExprAssign(const DocumentWeakPtr& document, Cmper ID, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, ID, shareMode, cmper, inci) {}

    // Public properties corresponding to the XML structure
    Cmper textExprId{};         ///< The @ref Cmper of a text expression (xml node is `<textExprID>`)
    Cmper shapeExprId{};        ///< The @ref Cmper of a shape expression (xml node is `<shapeExprID>`)
    Evpu horzEvpuOff{};         ///< Horizontal Evpu offset from the default position.
    Edu eduPosition{};          ///< Horizontal Edu position (xml node is `<horzEduOff>`)
    Evpu vertEvpuOff{};         ///< Vertical Evpu offset from the default position (xml node is `<vertOff>`)
    InstCmper staffAssign{};    ///< The staff to which this expression is assigned, or -1 if it uses #staffList.
    int layer{};                ///< The 1-based layer number to which this expression is assigned. (0 means all)
    bool dontScaleWithEntry{};  ///< Inverse of "Scale Expression with Attached Note".
    Cmper staffGroup{};         ///< Not sure what this is used for, but it seems to be a @ref details::StaffGroup cmper.
    Cmper staffList{};          ///< The cmper of the staff list to use if #staffAssign is negative.

    /// @brief Gets the assigned text expression.
    /// @return The text expression or nullptr if this assignment is for a shape expression or #textExprId not found.
    std::shared_ptr<TextExpressionDef> getTextExpression() const;

    /// @brief Gets the assigned shape expression.
    /// @return The shape expression or nullptr if this assignment is for a text expression or #shapeExprId not found.
    std::shared_ptr<ShapeExpressionDef> getShapeExpression() const;

    void integrityCheck() override
    {
        this->OthersBase::integrityCheck();
        if (!textExprId && !shapeExprId) {
            MUSX_INTEGRITY_ERROR("Expression assignment at measure " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value_or(-1))
                + " has no expression definition ID.");
        } else if (textExprId && shapeExprId) {
            MUSX_INTEGRITY_ERROR("Expression assignment at measure " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value_or(-1))
                + " has both text expr ID " + std::to_string(textExprId) + " and shape expr ID " + std::to_string(shapeExprId));
        }
    }

    bool requireAllFields() const override { return false; } ///< @todo: remove this override after identifying all fields.

    constexpr static std::string_view XmlNodeName = "measExprAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MeasureExprAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MeasureNumberRegion
 * @brief Represents the Measure Number Region with detailed font and enclosure settings for score and part data.
 *
 * This class is identified by the XML node name "measNumbRegion".
 */
class MeasureNumberRegion : public OthersBase {
public:
    /** @brief Constructor function */
    explicit MeasureNumberRegion(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /// @brief Alignment and justification options for measure numbers.
    enum class AlignJustify
    {
        Left,   ///< Left alignment or justification (the default value.)
        Right,  ///< Right alignment.
        Center  ///< Center alignment.
    };

    /// @brief Precision for time display
    enum class TimePrecision
    {
        WholeSeconds,   ///< the default value
        Tenths,
        Hundredths,
        Thousandths,
    };

    /// @brief Measure number data that can differ in score or part.
    class ScorePartData : public Base
    {
    public:
        /** @brief Constructor */
        explicit ScorePartData(const DocumentWeakPtr& document) : Base(document, 0, ShareMode::All) {}

        std::shared_ptr<FontInfo> startFont;          ///< The font used for numbers at start of system.
        std::shared_ptr<FontInfo> multipleFont;       ///< The font used for mid-system numbers.
        std::shared_ptr<FontInfo> mmRestFont;         ///< The font used for multi-measure rest ranges.
        std::shared_ptr<Enclosure> startEnclosure;    ///< Enclosure settings for numbers at start of system.
        std::shared_ptr<Enclosure> multipleEnclosure; ///< Enclosure settings for mid-system numbers.

        Evpu startXdisp{};         ///< Horizontal offset for numbers at start of system.
        Evpu startYdisp{};         ///< Vertical offset for numbers at start of system.
        Evpu multipleXdisp{};      ///< Horizontal offset for mid-system numbers.
        Evpu multipleYdisp{};      ///< Vertical offset for mid-system numbers.
        Evpu mmRestXdisp{};        ///< Horizontal offset for multi-measure rest ranges.
        Evpu mmRestYdisp{};        ///< Vertical offset for multi-measure rest ranges.
        char32_t leftMmBracketChar{};  ///< UTF-32 code for the left bracket of multi-measure rest ranges.
        char32_t rightMmBracketChar{}; ///< UTF-32 code for the right bracket of multi-measure rest ranges.
        int startWith{};           ///< "Beginning with" value. (This value is 0-based. The Finale UI adds 1 for user display.)
        int incidence{};           ///< "Show on Every" value.
        AlignJustify startAlign{};  ///< Alignment of numbers at the start of system
        AlignJustify multipleAlign{}; ///< Alignment for mid-system numbers.
        AlignJustify mmRestAlign{}; ///< Alignment for multi-measure ranges.
        bool showOnStart{};        ///< "Show On Start of Staff System" (xml node is `<startOfLine>`)
        bool showOnEvery{};        ///< "Show on Every" activates mid-system numbers. (xml node is `<multipleOf>`)
        bool hideFirstMeasure{};   ///< "Hide First Measure Number in Region." (xml node is `<exceptFirstMeas>`)
        bool showMmRange{};        ///< "Show Measure Ranges on Multimeasure Rests" (xml node is `<mmRestRange>`)
        bool showOnMmRest{};       ///< "Show on Multimeasure Rests"  (xml node is `<mmRestRangeForce>`)
        bool useStartEncl{};       ///< Use enclosure for start-of-system settings.
        bool useMultipleEncl{};    ///< Use enclosure for mid-system settings.
        bool showOnTop{};          ///< Show measure numbers on the top staff.
        bool showOnBottom{};       ///< Show measure numbers on the bottom staff.
        bool excludeOthers{};      ///< Exclude other staves.
        bool breakMmRest{};        ///< Mid-system numbers break multimeasure rests.
        AlignJustify startJustify{}; ///< Justification for numbers at the start of system.
        AlignJustify multipleJustify{}; ///< Justification for mid-system numbers.
        AlignJustify mmRestJustify{}; ///< Justification for multi-measure rest ranges.

        static const xml::XmlElementArray<ScorePartData>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    // Public properties
    std::shared_ptr<ScorePartData> scoreData; ///< Score-wide measure number data.
    std::shared_ptr<ScorePartData> partData;  ///< Part-specific measure number data.

    MeasCmper startMeas{};      ///< Starting measure number for the region.
    MeasCmper endMeas{};        ///< Ending measure number for the region (non-inclusive).
    char32_t startChar{};       ///< UTF-32 code for the first character in the sequence. (Frequently '0', 'a', or 'A')
    int base{};                 ///< The base used for measure number calculations. (Frequently 10 for numeric or 26 for alpha)
    int numberOffset{};         ///< This value is 1 less than the "Starting Number" field in the Finale UI. (xml node is `<offset>`)
    std::string prefix;         ///< Text prefix for measure numbers (encoded UTF-8).
    std::string suffix;         ///< Text suffix for measure numbers (encoded UTF-8).

    bool countFromOne{};        ///< Start counting from 1 rather than 0, e.g., "1, 2, 3, 4" numbering style (in conjuction with base 10)
    bool noZero;                ///< Indicates the base has no zero value: true for alpha sequences and false for numeric sequences
    bool doubleUp{};            ///< Indicates "a, b, c...aa, bb, cc" number style: the symbols are repeated when they exceed the base.
    bool time{};                ///< Display real time sequences rather than numbers or letters.
    bool includeHours{};        ///< Display hours (when showing real time measure numbers)
    bool smpteFrames{};         ///< SMPTE frames (when showing real time measure numbers). This option supercedes `timePrecision`.
    bool useScoreInfoForPart{}; ///< Use score-wide settings for parts.
    int region{};               ///< The region ID. This 1-based value is set by Finale and never changes, whereas the @ref Cmper may change when Finale sorts the regions.
    TimePrecision timePrecision{}; ///< Precision for real-time sequences.
    bool hideScroll{};          ///< Indicates if numbers are hidden in Scroll View and Studio View.
    bool hidePage{};            ///< Indicates if numbers are hidden in Page View.

    /// @brief Calculates whether the input measure is covered by this measure number region
    /// @param measureId The measure id to check.
    bool calcIncludesMeasure(MeasCmper measureId) const
    {
        return measureId >= startMeas && measureId < endMeas; // endMeas is non-inclusive!
    }

    /// @brief Returns the starting measure number for this region.
    int getStartNumber() const { return int(numberOffset + 1); }

    /// @brief Returns the visible number for a measure id with respect to the region.
    /// @throw std::logic_error if measureId is not contained in the region
    int calcDisplayNumberFor(MeasCmper measureId) const;

    /// @brief Finds the measure number region containing a measure
    /// @param document The document to search
    /// @param measureId The measure Id to search for
    /// @return The first MeasureNumberRegion instance that contains the @p measureId, or nullptr if not found.
    static std::shared_ptr<MeasureNumberRegion> findMeasure(const DocumentPtr& document, MeasCmper measureId);

    constexpr static std::string_view XmlNodeName = "measNumbRegion"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MeasureNumberRegion>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MultimeasureRest
 * @brief Represents the attributes of a multimeasure rest in the page layout.
 *
 * The Cmper is the first measure of the multimeasure rest.
 *
 * This class is identified by the XML node name "mmRest".
 */
class MultimeasureRest : public OthersBase {
public:
    /** @brief Constructor function */
    explicit MultimeasureRest(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper startMeasure)
        : OthersBase(document, partId, shareMode, startMeasure) {}

    Evpu measWidth{};           ///< Width of the multemeasure rest "measure" in Evpu. (xml node is `<meaSpace>`)
    MeasCmper nextMeas{};       ///< Next measure after the multimeasure west.
    Evpu numVertAdj{};          ///< Vertical number adjustment, sign-revered from Finale UI. (xml node is `<numdec>`)
    Cmper shapeDef{};           ///< Cmper of Shape Designer shape that specifies the H-bar.
    int numStart{};             ///< Number start value. If the number of measures in the multimeasure rest is fewer than this
                                ///< number, no number appears on the multimeasure rest.
    int symbolThreshold{};      ///< If the number of rests is less than this value, symbols are used when #useSymbols is true. (xml node is `<threshold>`)
    Evpu symbolSpacing{};       ///< Spacing between symbols in Evpu. (xml node is `<spacing>`)
    Evpu numHorzAdj{};          ///< Horizontal number adjustment in Evpu. (xml node is `<numAdjX>`)
    Evpu shapeStartAdjust{};    ///< Start adjustment for the H-bar shape in Evpu. (xml node is `<startAdjust>`)
    Evpu shapeEndAdjust{};      ///< End adjustment for the shape in Evpu. (xml node is `<endAdjust>`)
    bool useSymbols{};          ///< Use symbols instead of an H-bar, based on #symbolThreshold. (xml node is `<useCharRestStyle>`)

    /// @brief Get the start measure of this multimeasure rest
    MeasCmper getStartMeasure() const { return getCmper(); }

    /// @brief Calculates the number of measures spanned by this multimeasure rest
    int calcNumberOfMeasures() const { return (std::max)(nextMeas - getStartMeasure(), 0); }
    
    /// @brief Calculates if the number on this multimeasure rest is visible.
    bool calcIsNumberVisible() const { return calcNumberOfMeasures() >= numStart; }

    void integrityCheck() override
    {
        this->OthersBase::integrityCheck();
        if (nextMeas <= getStartMeasure()) {
            MUSX_INTEGRITY_ERROR("Multimeasure rest at " + std::to_string(getCmper()) + " in part " + std::to_string(getPartId()) + " spans 0 or fewer measures.");
        }
    }

    constexpr static std::string_view XmlNodeName = "mmRest"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MultimeasureRest>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MultiStaffGroupId
 * @brief Represents a group ID for a multi-staff setup.
 *
 * This class is identified by the XML node name "multiStaffGroupID".
 */
class MultiStaffGroupId : public OthersBase {
public:
    /** @brief Constructor function */
    explicit MultiStaffGroupId(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    Cmper staffGroupId{}; ///< Cmper of @ref details::StaffGroup that has the instrument's full and abbreviated names.

    constexpr static std::string_view XmlNodeName = "multiStaffGroupID"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MultiStaffGroupId>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MultiStaffInstrumentGroup
 * @brief Represents a group of instruments spanning multiple staves.
 *
 * This class is identified by the XML node name "multiStaffInstGroup".
 */
class MultiStaffInstrumentGroup : public OthersBase {
public:
    /** @brief Constructor function */
    explicit MultiStaffInstrumentGroup(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::vector<InstCmper> staffNums; ///< Vector of Cmper values representing up to 3 staff numbers.

    /// @brief Returns the staff at the index position or null if out of range or not found.
    /// @param x the 0-based index to find
    std::shared_ptr<Staff> getStaffAtIndex(size_t x) const;

    /// @brief Returns the first staff (with integrity check)
    std::shared_ptr<Staff> getFirstStaff() const;

    /// @brief Returns the index of the input staffId or std::nullopt if not found
    std::optional<size_t> getIndexOf(InstCmper staffId) const
    {
        for (size_t x = 0; x < staffNums.size(); x++) {
            if (staffNums[x] == staffId) return x;        
        }
        return std::nullopt;
    }

    /// @brief Gets the group associated with this multistaff instrument, or nullptr if not found
    std::shared_ptr<details::StaffGroup> getStaffGroup() const;

    void integrityCheck() override
    {
        OthersBase::integrityCheck();
        if (staffNums.empty()) {
            MUSX_INTEGRITY_ERROR("MultiStaffInstrumentGroup " + std::to_string(getCmper()) + " contains no staves.");
        } else if (staffNums.size() > 3) {
            MUSX_INTEGRITY_ERROR("MultiStaffInstrumentGroup " + std::to_string(getCmper()) + " contains more than 3 staves.");
        }
    }

    constexpr static std::string_view XmlNodeName = "multiStaffInstGroup"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MultiStaffInstrumentGroup>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class Page
 * @brief Represents the attributes of a page in the page layout.
 *
 * This class is identified by the XML node name "pageSpec".
 */
class Page : public OthersBase {
public:
    /** @brief Constructor function */
    explicit Page(const DocumentWeakPtr& document, Cmper pageId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, pageId, shareMode, cmper) {}

    Evpu height{};              ///< Page height in Evpu.
    Evpu width{};               ///< Page width in Evpu.
    int percent{};              ///< Percent value (scaling factor).
    SystemCmper firstSystem{};  ///< First system on the page (-1 if page is blank). See @ref StaffSystem.
    bool holdMargins{};         ///< "Hold Margins" (xml node is `<scaleContentOnly>`)
    Evpu margTop{};             ///< Top margin in Evpu. (Sign reversed in Finale UI.)
    Evpu margLeft{};            ///< Left margin in Evpu.
    Evpu margBottom{};          ///< Bottom margin in Evpu.
    Evpu margRight{};           ///< Right margin in Evpu. (Sign reversed in Finale UI.)

    std::optional<SystemCmper> lastSystem; ///< Computed by the Resolver function. This value is not in the xml.

    /** @brief is this a blank page */
    bool isBlank() const { return firstSystem < 0; }

    constexpr static std::string_view XmlNodeName = "pageSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Page>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class PartDefinition
 * @brief Represents the attributes of a Finale "partDef".
 *
 * @todo After identifying all possible fields, remove the override of #PartDefinition::requireAllFields.
 *
 * The cmper is the part definition ID, representing unique part definitions in the Finale document.
 * This class is identified by the XML node name "partDef".
 */
class PartDefinition : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit PartDefinition(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    Cmper nameId{};                    ///< @ref Cmper of the part name @ref TextBlock. (xml tag is `<nameID>`)
    int partOrder{};                   ///< Value that determines the order of listed parts in Finale's UI.
    int copies{};                      ///< Number of copies to print.
    bool extractPart{};                ///< Indicates if the part should be extracted.
    bool needsRecalc{};                ///< Indicates if the part needs update layout.
    bool useAsSmpInst{};               ///< Indicates if the part is used as a SmartMusic instrument.
    int smartMusicInst{};              ///< SmartMusic instrument ID (-1 if not used).
    Cmper defaultNameStaff{};          ///< If non-zero, this points to the @ref Staff that has the default name (if unspecified by #nameId.) 
    Cmper defaultNameGroup{};          ///< If non-zero, this points to the @ref details::StaffGroup that has the default name (if unspecified by #nameId.) 

    /** @brief Get the part name if any */
    std::string getName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /** @brief Return true if this part corresponds to the score */
    bool isScore() const { return getCmper() == SCORE_PARTID; }

    /** @brief Return the @ref InstrumentUsed cmper by this part for the specified system.
     *
     * This function either returns the input @p systemId or the Special Part Extraction cmper.
     *
     * @param systemId The staff system to find.
    */
    Cmper calcSystemIuList(Cmper systemId) const;

    /** @brief Return the instance for the score */
    static std::shared_ptr<PartDefinition> getScore(const DocumentPtr& document);

    /** @brief Return the linked parts sorted in UI order by #partOrder */
    static std::vector<std::shared_ptr<PartDefinition>> getInUserOrder(const DocumentPtr& document);

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "partDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PartDefinition>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class PartGlobals
 * @brief Represents global values that can vary by part
 *
 * The cmper is always 65534.
 *
 * This class is identified by the XML node name "partGlobals".
 */
class PartGlobals : public OthersBase {
public:
    /** @brief Constructor function */
    explicit PartGlobals(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    bool showTransposed{};                  ///< If true, "Display Concert Pitch" is unchecked for the part.
    Cmper scrollViewIUlist{};               ///< If this value is non-zero, it is the iuList @ref Cmper of the current Staff List in Scroll View.
    Cmper studioViewIUlist{};               ///< The iuList @ref Cmper for Studio View.
    /** @brief If non-zero, Special Part Extraction is in effect and this is the @ref Cmper for its @ref InstrumentUsed array.
     *
     * When Special Part Extraction is in effect, staff systems no longer have their own instrument lists. Instead, they use this value.
     *
     * (xml node is `<pageViewIUlist>`)
    */
    Cmper specialPartExtractionIUList{};    ///< If non-zero, Special Part Extraction is in effect and this is the iuList @ref Cmper. 

    constexpr static std::string_view XmlNodeName = "partGlobals"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PartGlobals>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @enum RepeatActionType
 * @brief Enum for the possible values of the `<action>` element.
 */
enum class RepeatActionType
{
    JumpAuto,               ///< Automatically Jump. (the default). The targetValue is meaningless for this action.
    JumpAbsolute,           ///< Jump to the measure number specified in the targetValue field.
    JumpRelative,           ///< Jump relative to the current measure. The targetValue field specifies how many measures to jump.
                            ///< The targetValue may be positive or negative for forward or backward relative jumps.
    JumpToMark,             ///< Jump to a specified repeat number (used by text repeats).
    Stop,                   ///< Stops playback after a number of passes (e.g. "Fine")
    NoJump                  ///< Do not jump. The targetValue is meaningless for this action.
};

/**
 * @enum RepeatTriggerType
 * @brief Enum for the possible values of the `<trigger>` element.
 */
enum class RepeatTriggerType
{
    Always,                 ///< Always jump (the default)
    OnPass,                 ///< Jump on a sepecified pass number.
    UntilPass               ///< Jump until a specified pass number is reached.
};

/**
 * @class RepeatBack
 * @brief Represents a repeat-backward marker with positioning and behavior properties.
 *
 * The cmper is the cmper of the @ref Measure that has this item.
 *
 * This class is identified by the XML node name "repeatBack".
 */
class RepeatBack : public OthersBase {
public:
    /** @brief Constructor function */
    explicit RepeatBack(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    int passNumber{};               ///< Specifies the pass number for the trigger and depends on the value of #trigger. (xml tag is `<actuate>`)
                                    ///< If #trigger is `UntilPass`, this specifies the number of times to play the section.
                                    ///< If #trigger is `OnPass`, this specifies the pass on which to jump.
    int targetValue{};              ///< Absolute or relative measure number, depending on #jumpAction. (xml tag is `<target>`)
    Evpu leftHPos{};                ///< The horizontal position of the left bracket, relative to the default. (xml tag is `<pos1>`)
    Evpu leftVPos{};                ///< The vertical position of the lower left bracket, relative to the default. (xml tag is `<line1>`)
    bool individualPlacement{};     ///< "Allow Individual Edits Per Staff" (xml tag is `<indivPlac>`)
    bool topStaffOnly{};            ///< "Show On: Top Staff Only"
    bool resetOnAction{};           ///< "Reset on Repeat Action" (xml tag is `<clrOnChange>`)
    RepeatActionType jumpAction{};  ///< The jump action for this repeat ending. The automatic jump is to the next ending. (xml tag is `<action>`)
    RepeatTriggerType trigger{};    ///< The condition that triggers the #jumpAction.
    Cmper staffList{};              ///< If non-zero, specifies a staff list for which staves to show the ending.
    Evpu rightHPos{};               ///< The horizontal position of the upper right bracket, relative to the default. (xml tag is `<pos2>`)
    Evpu rightVPos{};               ///< The vertical position of the upper right bracket, relative to the default. (xml tag is `<line2>`)
    
    constexpr static std::string_view XmlNodeName = "repeatBack"; ///< The XML node name for this type.
    static const xml::XmlElementArray<RepeatBack>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
/**
 * @class RepeatEndingStart
 * @brief Represents a repeat ending start marker in the document.
 *
 * The cmper is the cmper of the @ref Measure that has this item.
 *
 * This class is identified by the XML node name "repeatEndingStart".
 */
class RepeatEndingStart : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit RepeatEndingStart(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    Cmper staffList{};              ///< If non-zero, specifies a staff list for which staves to show the ending.
    int targetValue{};              ///< Absolute or relative measure number, depending on #jumpAction. (xml tag is `<nextEnd>`)
    Evpu textHPos{};                ///< The horizontal position of the text relative to #leftHPos. (xml tag is `<textPos>`)
    Evpu leftHPos{};                ///< The horizontal position of the lower left bracket, relative to the default. (xml tag is `<pos1>`)
    Evpu leftVPos{};                ///< The vertical position of the lower left bracket, relative to the default. (xml tag is `<line1>`)
    bool individualPlacement{};     ///< "Allow Individual Edits Per Staff" (xml tag is `<indivPlac>`)
    bool topStaffOnly{};            ///< "Show On: Top Staff Only"
    RepeatActionType jumpAction{};  ///< The jump action for this repeat ending. The automatic jump is to the next ending. (xml tag is `<action>`)
    RepeatTriggerType trigger{};    ///< The condition that triggers the #jumpAction.
                                    ///< For `RepeatEndingStart` this value is always `OnPass`, and it seems to mean that
                                    ///< it jumps when the current pass exceeds the final ending number.
    bool jumpIfIgnoring{};          ///< "Skip Ending if Ignoring Repeats" (xml tag is `<jmpIgnore>`)
    Evpu endLineVPos{};             ///< The vertical offset of the final bracket, relative to #rightVPos or 0 if the ending is open. (xml tag is `<endLine>`)
    Evpu textVPos{};                ///< The vertical position of the text relative to #leftVPos. (xml tag is `<textLine>`)
    Evpu rightHPos{};               ///< The horizontal position of the upper right bracket, relative to the default. (xml tag is `<pos2>`)
    Evpu rightVPos{};               ///< The vertical position of the upper right bracket, relative to the default. (xml tag is `<line2>`)

    /// @brief Calculates the number of measures in the ending based on #jumpAction.
    int calcEndingLength() const;

    /// @brief Calculates if the ending is open or closed, based on a number of factors.
    ///
    /// Openness is a visual feature. If true, it means that the ending bracket has a downward stroke on the right.
    bool calcIsOpen() const;


    void integrityCheck()
    {
        OthersBase::integrityCheck();
        if (trigger != RepeatTriggerType::OnPass) {
            MUSX_INTEGRITY_ERROR("RepeatEndingStart at measure " + std::to_string(getCmper()) + " has an unexpected trigger value " +
                std::to_string(int(trigger)));
        }
    }

    constexpr static std::string_view XmlNodeName = "repeatEndingStart"; ///< The XML node name for this type.
    static const xml::XmlElementArray<RepeatEndingStart>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class RepeatEndingText
 * @brief Represents the text associated with a @ref RepeatEndingStart.
 *
 * This is an optional value. If it is omitted, the ending shows a standard number list.
 *
 * This class has the same #Cmper as its @ref RepeatEndingStart and is the @ref MeasCmper of the ending.
 *
 * This class is identified by the XML node name "repeatEndingText".
 */
class RepeatEndingText : public OthersBase {
public:
    /** @brief Constructor function */
    explicit RepeatEndingText(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::string text; ///< The text 

    constexpr static std::string_view XmlNodeName = "repeatEndingText"; ///< The XML node name for this type.
    static const xml::XmlElementArray<RepeatEndingText>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class RepeatPassList
 * @brief Represents a list of repeat ending numbers for a @ref RepeatEndingStart instance.
 *
 * The cmper is the cmper of the @ref Measure that has this item.
 *
 * This class is identified by the XML node name "repeatPassList".
 */
class RepeatPassList : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit RepeatPassList(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    std::vector<int> endingNumbers; ///< List of repeat ending numbers extracted from xml `<act>` elements.

    constexpr static std::string_view XmlNodeName = "repeatPassList"; ///< The XML node name for this type.
    static const xml::XmlElementArray<RepeatPassList>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class ShapeExpressionDef
 * @brief Stores the properties and behaviors of shape expressions.
 *
 * This class is identified by the XML node name "shapeExprDef".
 */
class ShapeExpressionDef : public OthersBase {
public:
    /**
     * @brief Constructor.
     *
     * Initializes all fields to their default values.
     */
    explicit ShapeExpressionDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    Cmper shapeDef{};                               ///< Identifier for the Shape Designer Shape associated with this expression.
    Cmper categoryId{};                             ///< Identifier for the category of the text expression. (xml node is "categoryID")
    RehearsalMarkStyle rehearsalMarkStyle{};        ///< Auto-sequencing style for rehearsal marks.
    int value{};                                    ///< Value associated with the expression (e.g., velocity).
    int auxData1{};                                 ///< Auxiliary data for the expression. (xml node is "auxdata1")
    int playPass{};                                 ///< "Play Only on Pass" value.
    bool breakMmRest{};                             ///< Whether the text breaks multimeasure rests.
    bool useAuxData{};                              ///< Whether auxiliary data is used.
    bool masterShape{};                             ///< Whether this expression references the master copy of the shape.
    bool noPrint{};                                 ///< Inverse of "Hidden" checkbox.
    bool noHorzStretch{};                           ///< Inverse of "Allow Horizontal Stretching" checkbox.
    PlaybackType playbackType{};                    ///< Playback behavior of the text expression.
    HorizontalMeasExprAlign horzMeasExprAlign{};    ///< Horizontal alignment of the expression.
    VerticalMeasExprAlign vertMeasExprAlign{};      ///< Vertical alignment of the expression.
    HorizontalTextJustification horzExprJustification{}; ///< Horizontal justification of the text expression.
    Evpu measXAdjust{};                             ///< Horizontal adjustment for measurement alignment.
    Evpu yAdjustEntry{};                            ///< Vertical adjustment for entry alignment.
    Evpu yAdjustBaseline{};                         ///< Vertical adjustment for baseline alignment.
    bool useCategoryPos{};                          ///< Whether to use category position.
    std::string description;                        ///< Description of the text expression. (xml node is "descStr")

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "shapeExprDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<ShapeExpressionDef>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
class StaffStyle;
/**
 * @class Staff
 * @brief Represents the definition of a Finale staff.
 *
 * The cmper is the staff ID. This class is identified by the XML node name "staffSpec".
 */
class Staff : public OthersBase
{
public:
    /** @brief Enum for auto-numbering style. Auto-numbering is based on #instUuid. */
    enum class AutoNumberingStyle
    {
        ArabicSuffix,       ///< Arabic numeral suffix (default). May not appear in xml.
        RomanSuffix,        ///< Roman numeral suffix.
        OrdinalPrefix,      ///< Ordinal number prefix: 1st 2nd 3rd, ...
        AlphaSuffix,        ///< Alphabetic suffix.
        ArabicPrefix        ///< Arabic numeral prefix (with dot): 1. 2. 3. ...
    };

    /** @brief Enum for staff-level stem direction override. */
    enum class StemDirection
    {
        Default,            ///< the default (may not occur in xml)
        AlwaysUp,           ///< stems are always up on this staff
        AlwaysDown          ///< stems are always down on this staff
    };

    /** @brief Constructor function */
    explicit Staff(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    ClefIndex defaultClef{};        ///< Index of default clef for the staff.
    ClefIndex transposedClef{};     ///< Index of transposed clef for the staff.
    std::optional<int> staffLines{}; ///< Number of lines in the staff (if no custom staff)
    std::optional<std::vector<int>> customStaff; ///< A list of stafflines from 0..26 where a standard 5-line staff is values 11, 12, 13, 14, 15.
    Evpu lineSpace{};               ///< Distance between staff lines.
    std::string instUuid;           ///< Unique identifier for the type of instrument.
    //noteFont
    bool hasStyles{};               ///< Indicates that this staff has staff style assignments
    bool showNameInParts{};         ///< "Display Staff Name in Parts" (xml node is `<showNameParts>`)
    //transposition
    bool hideNameInScore{};         ///< Inverse of "Display Staff Name in Score" (xml node is `<hideStfNameInScore>`)
    Evpu topBarlineOffset{};        ///< Offset for the top barline.
    Evpu botBarlineOffset{};        ///< Offset for the bottom barline.
    Evpu dwRestOffset{};            ///< Offset for downstem rests.
    Evpu wRestOffset{};             ///< Offset for whole rests.
    Evpu hRestOffset{};             ///< Offset for half rests.
    Evpu otherRestOffset{};         ///< Offset for other rests.
    int stemReversal{};             ///< Stem reversal value.
    Cmper fullNameTextId{};         ///< Full name @ref TextBlock ID. (xml node is `<fullName>`)
    Cmper abbrvNameTextId{};        ///< Abbreviated name @ref TextBlock ID. (xml node is `<abbrvName>`)
    Evpu botRepeatDotOff{};         ///< Offset for bottom repeat dots.
    Evpu topRepeatDotOff{};         ///< Offset for top repeat dots.
    Evpu vertTabNumOff{};           ///< Vertical offset for tab number.
    bool hideStems{};               ///< Inverse of "Display Stems"
    StemDirection stemDirection{};  ///< stem direction for staff (xml node is `<stemDir>`)
    AutoNumberingStyle autoNumbering{}; ///< Autonumbering style if #useAutoNumbering is true. (xml node is `<autoNum>`)
    bool useAutoNumbering{};        ///< Whether names should be auto-numbered. (xml node is `<useAutoNum>`)

    Cmper multiStaffInstId{};       ///< Calculated cmper for @ref MultiStaffInstrumentGroup, if any. This value is not in the xml.
                                    ///< It is set by the factory with the Resolver function for @ref MultiStaffInstrumentGroup.
    std::optional<int> autoNumberValue; ///< Calculated autonumbering value. It is computed by #calcAutoNumberValues.

    /// @brief Returns the full staff name without Enigma tags
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    std::string getFullName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Returns the abbreviated staff name without Enigma tags
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    std::string getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Returns the @ref MultiStaffInstrumentGroup for this staff if it is part of one. Otherwise nullptr.
    std::shared_ptr<MultiStaffInstrumentGroup> getMultiStaffInstGroup() const;

    /// @brief Returns the full instrument name for this staff without Enigma tags and with autonumbering (if any)
    /// @note Ordinal prefix numbering is currently supported only for English.
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    /// @param preferStaffName When true, use the staff name if there is one (rather than the multi-instrument group name)
    std::string getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii, bool preferStaffName = false) const;

    /// @brief Returns the abbreviated instrument name for this staff without Enigma tags and with autonumbering (if any)
    /// @note Ordinal prefix numbering is currently supported only for English.
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    /// @param preferStaffName When true, use the staff name if there is one (rather than the multi-instrument group name)
    std::string getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii, bool preferStaffName = false) const;

    /// @brief Returns if names should be shown for the specified part
    bool showNamesForPart(Cmper partId) const
    { return partId == SCORE_PARTID ? !hideNameInScore : showNameInParts; }

    /**
     * @brief Get the auto-numbering value for this staff, if applicable.
     * 
     * Calculates #autoNumberValue for every staff based on the occurrences of the instrument UUID
     * and in each staff instance. If numbering is not applicable (e.g., auto numbering
     * is disabled, the UUID is empty, or there is only one instance), #autoNumberValue has no value.
     *
     * This function is called by the DocumentFactory when the document is created.
     */
    static void calcAutoNumberValues(const DocumentPtr& document);

    /// @brief Add auto numbering as a prefix or suffix, if needed
    /// @param plainName The name (full or abbreviated) to which to add the auto numbering
    /// @return Auto numbered name.
    std::string addAutoNumbering(const std::string& plainName) const;

    /// @brief Returns the first clef in this staff
    ClefIndex calcFirstClefIndex() const;

    /// @brief Returns the first clef in the specified staff in the document
    /// @param document the document to search
    /// @param partId the linked part to search
    /// @param staffCmper the staff cmper to search
    static ClefIndex calcFirstClefIndex(const DocumentPtr& document, Cmper partId, InstCmper staffCmper);

    /// @brief Returns the middle staff position. For staves with even numbers of lines, it is the middle space.
    int calcMiddleStaffPosition() const;

    void integrityCheck() override
    {
        OthersBase::integrityCheck();
        if (!staffLines && !customStaff) {
            MUSX_INTEGRITY_ERROR("Staff " + std::to_string(getCmper()) + " has neither a standard nor a custom staff definition.");
        } else if (staffLines && customStaff) {
            MUSX_INTEGRITY_ERROR("Staff " + std::to_string(getCmper()) + " has both a standard and a custom staff definition.");            
        }
        if (customStaff) { // guarantee ascending order of staves.
            std::sort(customStaff.value().begin(), customStaff.value().end(),
                [](const auto& a, const auto& b) { return a < b; });
        }
    }

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "staffSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Staff>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffStyle
 * @brief Represents a Finale staff style.
 *
 * The cmper is a 1-based staff style ID (not necessarily sequential).
 * This class is identified by the XML node name "staffStyle".
 */
class StaffStyle : public Staff
{
protected:
    /** @brief protected constructor for @ref StaffComposite */
    explicit StaffStyle(const std::shared_ptr<Staff>& staff)
        : Staff(*staff), masks(std::make_shared<Masks>(staff->getDocument())) {}

public:
    /** @brief Constructor function */
    explicit StaffStyle(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : Staff(document, partId, shareMode, cmper) {}

    /// @brief lists the masks that deterimine if this staff style overrides the staff settings
    /// @todo add masks as needed. If we ever add them all, remove Base inheritance and #requireAllFields function.
    class Masks : public Base // Base inheritance needed for requireAllFields
    {
    public:
        /**
         * @brief Default constructor
         * @param document A weak pointer to the document object.
         */
        explicit Masks(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All) {}

        bool defaultClef{};         ///< overrides default clef
        bool staffType{};           ///< overrides staff properties (see #StaffComposite::applyStyle)
        bool transposition{};       ///< overrides transposition fields
        bool negNameScore{};        ///< overrides #hideNameInScore.
        bool fullName{};            ///< overrides #fullNameTextId.
        bool abrvName{};            ///< overrides #abbrvNameTextId.
        bool showStems{};           ///< overrides stem properties (see #StaffComposite::applyStyle)
        bool showNameParts{};       ///< overrides #showNameInParts

        bool requireAllFields() const override { return false; }

        static const xml::XmlElementArray<Masks>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
    };

    std::string styleName;              ///< name of staff style
    bool addToMenu;                     ///< add this staff style to the context menu for staff styles
    std::shared_ptr<Masks> masks;       ///< override masks: guaranteed to exist by #integrityCheck, which is called by the factory
                                        ///< (xml node is `<mask>`)

    /// @brief Finds a subset from all StaffStyle instances that overlap with the specified
    /// metric position on a given staff in a give linked part or score.
    /// @param document The document to search
    /// @param partId The linked part id to search
    /// @param staffId The staff to search
    /// @param measId The MeasCmper of the measure position
    /// @param eduPosition The Edu position within the measure specified by @p measId.
    static std::vector<std::shared_ptr<StaffStyle>> findAllOverlappingStyles(const DocumentPtr& document,
        Cmper partId, InstCmper staffId, MeasCmper measId, Edu eduPosition);

    bool requireAllFields() const override { return false; }

    void integrityCheck() override
    {
        Staff::integrityCheck();
        if (!masks) {
            // Finale allows creation of staff styles with no masks, so this is just a verbose comment
            util::Logger::log(util::Logger::LogLevel::Verbose, "StaffStyle " + styleName
                + " (" + std::to_string(getCmper()) + ") does not override anything.");
            masks = std::make_shared<Masks>(getDocument());
        }
    }

    constexpr static std::string_view XmlNodeName = "staffStyle"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StaffStyle>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffStyleAssign
 * @brief Represents an assignment
 *
 * The cmper is the staff ID. This class is identified by the XML node name "staffStyleAssign".
 */
class StaffStyleAssign : public MusicRange
{
public:
    /** @brief Constructor function */
    explicit StaffStyleAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : MusicRange(document, partId, shareMode, cmper, inci) {}

    Cmper styleId{};        ///< The cmper of the assigned @ref StaffStyle.

    /// @brief Returns the @ref StaffStyle instance for this assignment
    /// @return Can return nullptr only in the case when there is an itegrity error
    /// @throws musx::dom::integrity_error if compiled to throw integrity errors
    std::shared_ptr<StaffStyle> getStaffStyle() const;

    void integrityCheck() override
    {
        MusicRange::integrityCheck();
        if (!styleId) {
            MUSX_INTEGRITY_ERROR(std::string("Staff style assignment has no staff style id:")
                + " Part " + std::to_string(getPartId())
                + " Staff " + std::to_string(getCmper())
            );
        }
    }

    constexpr static std::string_view XmlNodeName = "staffStyleAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StaffStyleAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffComposite
 * @brief Represents a composite of an underlying @ref Staff instance with any applicable @ref StaffStyle instances applied.
 *
 * Instances of StaffComposite are not part of the document structure and cannot be created directly.
 * Use #createCurrent to create an instance.
 */
class StaffComposite : public StaffStyle
{
private:
    /** @brief private constructor */
    explicit StaffComposite(const std::shared_ptr<Staff>& staff)
        : StaffStyle(staff) {}

    /// @brief Modifies the current StaffComposite instance with all applicable values from the @ref StaffStyle.
    /// @param staffStyle The @ref StaffStyle to apply.
    void applyStyle(const std::shared_ptr<StaffStyle>& staffStyle);

public:
    /// @brief Calculates the current staff at the specified metric position by applying all relevant staff styles,
    ///
    /// Note that the Finale app has logic to assure that no two assigments modify the same staff properties at the same metric
    /// position. If this occurs despite Finale's code, the last one processed is the one that takes effect.
    ///
    /// @param document The document to search
    /// @param partId The ID of the linked part or score
    /// @param staffId The @ref Staff cmper of the base staff for which to apply staff styles
    /// @param measId The measure location to search
    /// @param eduPosition The Edu position within the measure to search
    /// @return The composite result or null if @p staffId is not valid.
    static std::shared_ptr<StaffComposite> createCurrent(const DocumentPtr& document, Cmper partId, InstCmper staffId, MeasCmper measId, Edu eduPosition);
};

/**
 * @class StaffSystem
 * @brief Represents the attributes of a staff system in the page layout.
 *
 * This class is identified by the XML node name "staffSystemSpec".
 */
class StaffSystem : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit StaffSystem(const DocumentWeakPtr& document, Cmper staffSystemId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, staffSystemId, shareMode, cmper)
    {
    }

    MeasCmper startMeas{};          ///< Starting measure of the staff system. See @ref Measure.
    MeasCmper endMeas{};            ///< Ending measure of the staff system. See @ref Measure.
    double horzPercent{};           ///< Horizontal scaling percentage (fractional, 100.0 means no scaling).
                                    ///< This value affects "stretchable" items such as word extensions on lyrics.
    int ssysPercent{};              ///< Staff system scaling percentage (100 means no scaling).
    Efix staffHeight{};             ///< Staff height in Efix units. Note that this is different units than #options::PageFormatOptions::PageFormat::rawStaffHeight.
    Evpu top{};                     ///< Top margin in Evpu. (Sign reversed in Finale UI.)
    Evpu left{};                    ///< Left margin in Evpu.
    Evpu right{};                   ///< Right margin in Evpu. (Sign reversed in Finale UI.)
    Evpu bottom{};                  ///< Bottom margin in Evpu. This value is 96 (i.e., 1 5-line staff thickness) less than the U.I. value.
                                    ///< That means if you enter 0 in the Finale UI, this value is -96.
    bool hasStaffScaling{};         ///< Indicates if any individual staff in the system has scaling applied.
    bool placeEndSpaceBeforeBarline{}; ///< Indicates that extra space is placed before the barline.
    bool scaleVert{};               ///< "Resize Vertical Space"
    bool holdMargins{};             ///< "Hold Margins" (xml node is `<scaleContentOnly>`)
    Evpu distanceToPrev{};          ///< Distance to the previous staff system in Evpu.
    Evpu extraStartSystemSpace{};   ///< Extra space at the start of the staff system in Evpu.
    Evpu extraEndSystemSpace{};     ///< Extra space at the end of the staff system in Evpu.

    constexpr static std::string_view XmlNodeName = "staffSystemSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StaffSystem>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TempoChange
 * @brief A single tempo change value entered with Finale's Tempo tool.
 *
 * Although the Finale UI dropped the Tempo tool around 2010, the notation engine
 * still supported them. It was always possible (even as late as Finale 27.4) to add tempo
 * changes using third-pary plugins.
 *
 * The most common tool for creating TempoChange instances was the plugin JW Tempo.
 * It only ever used absolute ratios, so the focus of this class is on them.
 *
 * The cmper is the measure number, and incis should be stored in order by #eduPosition. 
 *
 * This class is identified by the XML node name "tempoDef".
 */
class TempoChange : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit TempoChange(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    int ratio{};                    ///< Absolute or relative ratio.
                                    ///< Relative ratios are are a percent value * 10. 100% would have a value of 1000.
                                    ///< See #getAbsoluteTempo for how to use absolute ratios.
                                    ///< The xml provides one of two nodes for this field: `<relativeRatio>` or `<absoluteRatio>`.
    Edu eduPosition{};              ///< The position within the measure. (xml node is `<eldur>`)
    int unit{};                     ///< Hardware ticks/sec. For Macs this is 1000.
    bool isRelative;                ///< A computed value that determines if #ratio is relative or absolute.
                                    ///< This value is not represented in the xml but instead captures whether the #ratio
                                    ///< property was populated from `<relativeRatio>` or `<absoluteRatio>`.

    /// @brief Computes the absolute tempo represented by the TempoChange Instance.
    /// @param noteType optional note type for which to get the beats per minute. (The default is NoteType::Quarter.)
    /// @return The per minute value.
    /// @throws std::logic_error if this instance represents a relative tempo.
    int getAbsoluteTempo(NoteType noteType = NoteType::Quarter) const;

    constexpr static std::string_view XmlNodeName = "tempoDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TempoChange>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
/**
 * @class TextBlock
 * @brief Represents the attributes of a Finale "textBlock".
 *
 * @todo After identifying all possible fields, remove the override of #TextBlock::requireAllFields.
 *
 * The cmper is the textBlock ID, representing unique text blocks in the Finale document.
 * This class is identified by the XML node name "textBlock".
 */
class TextBlock : public OthersBase
{
public:
    /** @brief Enum for textTag values */
    enum class TextType
    {
        Block,      ///< #textId is a #Cmper for a @ref texts::BlockText
        Expression  ///< #textId is a #Cmper for a @ref texts::ExpressionText
    };

    /** @brief Constructor function */
    explicit TextBlock(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    Cmper textId{};                    ///< @ref Cmper of the text block. (xml tag is `<textID>`)
    int lineSpacingPercentage{};       ///< Line spacing percentage.
    bool newPos36{};                   ///< This is likely a compatibility setting. Best guess is that blocks created before Finale 3.6 do not have this set.
    bool showShape{};                  ///< Show shape
    bool noExpandSingleWord{};         ///< Do not expand single word
    bool wordWrap{};                   ///< Wrap words (in frames)
    Evpu width{};                      ///< Width of frame
    Evpu height{};                     ///< Height of frame
    bool roundCorners{};               ///< Use rounded corners on frame
    Efix cornerRadius{};               ///< Corner radius for rounded corners.
    TextType textType{};               ///< Text tag indicating the type of text block. (xml tag is `<textTag>`)

    /** @brief return display text with Enigma tags removed */
    std::string getText(bool trimTags = false, util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /** @brief return display text with Enigma tags removed */
    static std::string getText(const DocumentPtr& document, const Cmper textId, bool trimTags = false,
        util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii);

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "textBlock"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextBlock>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextExpressionDef
 * @brief Stores the properties and behaviors of text expressions.
 *
 * This class is identified by the XML node name "textExprDef".
 */
class TextExpressionDef : public OthersBase {
public:
    Cmper textIdKey{};                              ///< Identifier for the @ref TextBlock associated with this 
    Cmper categoryId{};                             ///< Identifier for the category of the text expression.
    RehearsalMarkStyle rehearsalMarkStyle{};        ///< Auto-sequencing style for rehearsal marks.
    int value{};                                    ///< Value associated with the expression (e.g., velocity).
    int auxData1{};                                 ///< Auxiliary data for the expression. (xml node is "auxdata1")
    int playPass{};                                 ///< "Play Only on Pass" value.
    bool hideMeasureNum;                            ///< "Hide Measure Numbers" (used on Rehearsal Marks)
    bool matchPlayback;                             ///< purpose needs investigation.
    bool useAuxData{};                              ///< Whether auxiliary data is used.
    bool hasEnclosure{};                            ///< Whether the text expression has an enclosure. (xml node is "newEnclosure")
    bool breakMmRest{};                             ///< Whether the text breaks multimeasure rests.
    bool createdByHp{};                             ///< Whether the text block was created by Finale's smart playback system
    PlaybackType playbackType{};                    ///< Playback behavior of the text expression.
    HorizontalMeasExprAlign horzMeasExprAlign{};    ///< Horizontal alignment of the expression.
    VerticalMeasExprAlign vertMeasExprAlign{};      ///< Vertical alignment of the expression.
    HorizontalTextJustification horzExprJustification{}; ///< Horizontal justification of the text expression.
    Evpu measXAdjust{};                             ///< Horizontal adjustment for measurement alignment.
    Evpu yAdjustEntry{};                            ///< Vertical adjustment for entry alignment.
    Evpu yAdjustBaseline{};                         ///< Vertical adjustment for baseline alignment.
    bool useCategoryFonts{};                        ///< Whether to use category fonts.
    bool useCategoryPos{};                          ///< Whether to use category position.
    std::string description;                        ///< Description of the text expression. (xml node is "descStr")

    /**
     * @brief Constructor.
     *
     * Initializes all fields to their default values.
     */
    explicit TextExpressionDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /** @brief Gets the enclosure for this expression, or nullptr if none. */
    std::shared_ptr<Enclosure> getEnclosure() const;

    constexpr static std::string_view XmlNodeName = "textExprDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextExpressionDef>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextExpressionEnclosure
 * @brief The enclosure for a text expression (if it exists)
 *
 * The cmper is the same as for the associated @ref TextExpressionDef`.
 *
 * This class is identified by the XML node name "textExpressionEnclosure".
 */
class TextExpressionEnclosure : public Enclosure {
public:
    using Enclosure::Enclosure;

    constexpr static std::string_view XmlNodeName = "textExpressionEnclosure"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextExpressionEnclosure>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextRepeatAssign
 * @brief Represents a text repeat assignment with positioning and behavior properties.
 *
 * The cmper is the cmper of the @ref Measure that has this item.
 *
 * This class is identified by the XML node name "textRepeatAssign".
 */
class TextRepeatAssign : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit TextRepeatAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    // Public properties corresponding to the XML structure
    Evpu horzPos{};                 ///< The horizontal offset from default of the text repeat marker.
    int passNumber{};               ///< Play Section N Times, Jump on Pass, Stop on Pass value, depending on #jumpAction and #trigger. (xml tag is `<actuate>`)
    int targetValue{};              ///< Measure number, TextRepeatDef ID, or offset, depending on #jumpAction. (xml tag is `<target>`)
    Cmper textRepeatId{};           ///< The Cmper of the assigned @ref TextRepeatDef. (xml tag is `<repnum>`)
    Evpu vertPos{};                 ///< The vertical offset from default of the text repeat marker.
    bool individualPlacement{};     ///< "Allow Individual Edits Per Staff" (xml tag is `<indivPlac>`)
    bool topStaffOnly{};            ///< "Show On: Top Staff Only"
    bool resetOnAction{};           ///< "Reset on Repeat Action" (xml tag is `<clrOnChange>`)
    bool jumpOnMultiplePasses{};    ///< If true, use #TextRepeatDef::passList to get the passes and ignore #passNumber. (xml tag is `<multiActuate>`)
    RepeatActionType jumpAction{};  ///< The jump action for this repeat assignment. (xml tag is `<action>`)
    bool autoUpdate{};              ///< "Auto-Update Target"
    RepeatTriggerType trigger{};    ///< The condition that triggers the #jumpAction.
    bool jumpIfIgnoring{};          ///< "Jump if Ignoring Repeats" (xml tag is `<jmpIgnore>`)
    Cmper staffList{};              ///< If non-zero, specifies a staff list for which staves to show the ending.

    constexpr static std::string_view XmlNodeName = "textRepeatAssign"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextRepeatAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
/**
 * @class TextRepeatDef
 * @brief Defines text repeat elements with font styling and justification.
 *
 * Text repeat elements are indications like "D.S. al Fine", "Segno", or "To Coda" that
 * have playback jumps associated with them.
 *
 * The cmper is the unique identifier used in the document.
 *
 * This class is identified by the XML node name "textRepeatDef".
 */
class TextRepeatDef : public OthersBase {
public:
    /** @brief Enum for poundReplace options */
    enum class PoundReplaceOption
    {
        Passes,             ///< "Number of Times Played" (the default: may never appear in xml)
        RepeatID,           ///< "Text Repeat ID in Target" (xml value is "repeatID")
        MeasureNumber       ///< "Measure Number in Target" (xml value is "measNum")
    };

    /** @brief Constructor function */
    explicit TextRepeatDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper), font(new FontInfo(document)) {}

    // Public properties corresponding to the XML structure
    std::shared_ptr<FontInfo> font;                 ///< The font for this text repeat. (xml nodes `<fontID>`, `<fontSize>`, and `<efx>`)
    bool hasEnclosure{};                            ///< Whether the text repeat has an enclosure. (xml node is `<newEnclosure>`)
    bool useThisFont{};                             ///< "Use This Font" (for the `#` substitution)
    PoundReplaceOption poundReplace{};              ///< "Replace # With" choice.
    HorizontalTextJustification justification{};    ///< Although called "justification" in Finale's U.I, this value is used
                                                    ///< for both the alignment of the text within the measure as well as its justification.
                                                    ///< (xml node is `<justify >`)
    std::vector<int> passList;                      ///< If this vector contains elements, they define the repeat passes that apply to this instance.

    constexpr static std::string_view XmlNodeName = "textRepeatDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextRepeatDef>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextRepeatEnclosure
 * @brief The enclosure for a text expression (if it exists)
 *
 * The cmper is the same as for @ref TextRepeatDef.
 *
 * This class is identified by the XML node name "textRepeatEnclosure".
 */
class TextRepeatEnclosure : public Enclosure {
public:
    using Enclosure::Enclosure;

    constexpr static std::string_view XmlNodeName = "textRepeatEnclosure"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextRepeatEnclosure>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextRepeatText
 * @brief Represents the text associated with a @ref TextRepeatDef
 *
 * The cmper is the same as for @ref TextRepeatDef.
 *
 * This class is identified by the XML node name "textRepeatText".
 */
class TextRepeatText : public OthersBase {
public:
    /** @brief Constructor function */
    explicit TextRepeatText(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    std::string text; ///< The text 

    constexpr static std::string_view XmlNodeName = "textRepeatText"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextRepeatText>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
/**
 * @class TimeCompositeLower
 * @brief Represents the lower composite time signature array.
 *
 * The cmper taken from the `unit` value when `hasCompositeBottom` is true. (See @ref TimeSignature.)
 * 
 * This class is identified by the XML node name "timeLower".
 */
class TimeCompositeLower : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit TimeCompositeLower(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    /** @class CompositeItem
     *  @brief Represents an individual lower composite item in the time signature.
     */
    class CompositeItem
    {
    public:
        Edu unit{};             ///< The size of the unit for this item. (xml node is `<integer>`)
        bool startGroup{};      ///< Indicates the start of a group.

        static const xml::XmlElementArray<CompositeItem>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    std::vector<std::shared_ptr<CompositeItem>> items;  ///< composite items collection

    constexpr static std::string_view XmlNodeName = "timeLower"; ///< XML node name.
    static const xml::XmlElementArray<TimeCompositeLower>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TimeCompositeUpper
 * @brief Represents the upper composite time signature structure.
 *
 * The cmper taken from the `beats` value when `hasCompositeTop` is true. (See @ref TimeSignature.)
 * 
 * This class is identified by the XML node name "timeUpper".
 */
class TimeCompositeUpper : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit TimeCompositeUpper(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    /** @class CompositeItem
     *  @brief Represents an individual composite item in the time signature.
     */
    class CompositeItem
    {
    public:
        Edu beats{};                ///< The number of beats in this item. (xml node is `<integer>`)
        util::Fraction fraction{};  ///< Fraction of beats (between 0 <= fraction < 1)
        bool startGroup{};          ///< Indicates the start of a group.

        /// @brief Return the beats as a complete fraction
        util::Fraction fullFraction() const { return fraction + beats; }

        static const xml::XmlElementArray<CompositeItem>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /// Collection of composite items
    std::vector<std::shared_ptr<CompositeItem>> items;  ///< composite items collection

    constexpr static std::string_view XmlNodeName = "timeUpper"; ///< XML node name.
    static const xml::XmlElementArray<TimeCompositeUpper>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};
    
} // namespace others
} // namespace dom
} // namespace musx