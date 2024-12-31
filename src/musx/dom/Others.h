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
#include <map>

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {
namespace others {

/**
 * @class FontDefinition
 * @brief The name and font characteristics of fonts contained.
 *
 * The cmper is the font ID used in classes throughout the document.
 *
 * This class is identified by the XML node name "fontName".
 */
class FontDefinition : public OthersBase {
public:
    /** @brief Constructor function */
    explicit FontDefinition(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    std::string charsetBank;    ///< probably only "Mac" or "Win"
    int charsetVal{};           ///< A value specifying the character set, usually 4095 or 0
    int pitch{};                ///< Represents the `<pitch>` element, e.g., 0. (use unknown)
    int family{};               ///< Represents the `<family>` element, e.g., 0. (use unknown)
    std::string name;           ///< The font name e.g., "Broadway Copyist Text".

    constexpr static std::string_view XmlNodeName = "fontName"; ///< The XML node name for this type.
    static const xml::XmlElementArray<FontDefinition> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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

    static std::shared_ptr<Staff> getStaffAtIndex(const std::vector<std::shared_ptr<InstrumentUsed>>& iuArray, Cmper index);

    constexpr static std::string_view XmlNodeName = "instUsed"; ///< The XML node name for this type.
    static const xml::XmlElementArray<InstrumentUsed> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    static const xml::XmlElementArray<LayerAttributes> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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

        static const xml::XmlElementArray<ScorePartData> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    // Public properties
    std::shared_ptr<ScorePartData> scoreData; ///< Score-wide measure number data.
    std::shared_ptr<ScorePartData> partData;  ///< Part-specific measure number data.

    MeasCmper startMeas{};      ///< Starting measure number for the region.
    MeasCmper endMeas{};        ///< Ending measure number for the region.
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

    constexpr static std::string_view XmlNodeName = "measNumbRegion"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MeasureNumberRegion> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    ChannelPressure,        ///< Playback affects MIDI channel pressure. (xml vlaue is "midiPressure")
    RestrikeKeys,           ///< Playback retrikes keys. (xml value is "rekey")
    Dump,                   ///< Playback is an arbitrary data dump. (Data is in <playDumpText> with the same Cmper value.)
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
 * @enum HorizontalExprJustification
 * @brief Specifies the horizontal alignment for text expressions and marking categories.
 */
enum class HorizontalExprJustification
{
    Left,    ///< Justified left.
    Center,  ///< Justified center.
    Right    ///< Justified right.
};

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
    HorizontalExprJustification justification{}; ///< Justification for the text within the marking

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

    /** @brief A list of text expressions in this category.
     *
     * (This in not in the xml but is created by the factory.)
     */
    std::map<Cmper, std::weak_ptr<TextExpressionDef>> textExpressions;

    /** @brief gets the name of the marking category */
    std::string getName() const;

    constexpr static std::string_view XmlNodeName = "markingsCategory"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MarkingCategory> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    static const xml::XmlElementArray<MarkingCategoryName> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 *
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
    int smartMusicInst{};               ///< SmartMusic instrument ID (-1 if not used).

    /** @brief Get the part name if any */
    std::string getName() const;

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "partDef"; ///< The XML node name for this type.
    static const xml::XmlElementArray<PartDefinition> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    static const xml::XmlElementArray<PartGlobals> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class Staff
 * @brief Represents the definition of a Finale staff.
 *
 * The cmper is the staff ID. This class is identified by the XML node name "staffSpec".
 */
class Staff : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit Staff(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    // Public properties corresponding to the XML structure
    ClefIndex defaultClef{};        ///< Index of default clef for the staff.
    ClefIndex transposedClef{};     ///< Index of transposed clef for the staff.
    Evpu lineSpace{};               ///< Distance between staff lines.
    Evpu topBarlineOffset{};        ///< Offset for the top barline.
    Evpu botBarlineOffset{};        ///< Offset for the bottom barline.
    Evpu dwRestOffset{};            ///< Offset for downstem rests.
    Evpu wRestOffset{};             ///< Offset for whole rests.
    Evpu hRestOffset{};             ///< Offset for half rests.
    Evpu otherRestOffset{};         ///< Offset for other rests.
    Evpu topRepeatDotOff{};         ///< Offset for top repeat dots.
    Evpu botRepeatDotOff{};         ///< Offset for bottom repeat dots.
    int staffLines{};               ///< Number of lines in the staff.
    int stemReversal{};             ///< Stem reversal value.
    Cmper fullNameTextId{};         ///< Full name @ref TextBlock ID. (xml node is `<fullName>`)
    Cmper abbrvNameTextId{};        ///< Abbreviated name @ref TextBlock ID. (xml node is `<abbrvName>`)
    Evpu vertTabNumOff{};           ///< Vertical offset for tab number.

    /// @brief Get the full staff name without Enigma tags
    std::string getFullName() const;

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "staffSpec"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Staff> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    std::string getText(bool trimTags = false) const;

    /** @brief return display text with Enigma tags removed */
    static std::string getText(const DocumentPtr& document, const Cmper textId, bool trimTags = false);

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "textBlock"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextBlock> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    HorizontalExprJustification horzExprJustification{}; ///< Horizontal justification of the text expression.
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
    static const xml::XmlElementArray<TextExpressionDef> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    static const xml::XmlElementArray<TextExpressionEnclosure> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TextRepeatEnclosure
 * @brief The enclosure for a text expression (if it exists)
 *
 * The cmper is the same as for @ref TextRepeateDef.
 *
 * This class is identified by the XML node name "textRepeatEnclosure".
 */
class TextRepeatEnclosure : public Enclosure {
public:
    using Enclosure::Enclosure;

    constexpr static std::string_view XmlNodeName = "textRepeatEnclosure"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TextRepeatEnclosure> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace musx