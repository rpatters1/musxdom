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
#include <functional>
#include <vector>
#include <unordered_set>

#include "BaseClasses.h"

 // do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {

class EntryFrame;

namespace options {
class TupletOptions;
} // namespace options

namespace others {
class Measure;
} // namespace others

/**
 * @namespace musx::dom::details
 * @brief Classes in the @ref DetailsPool.
 */
namespace details {

/**
 * @class GFrameHold
 * @brief Represents the attributes of a Finale frame holder.
 *
 * Cmper1 is the staff (inst) @ref Cmper and Cmper2 is the measur @ref Cmper
 * This class is identified by the XML node name "gfhold".
 */
class GFrameHold : public DetailsBase
{
public:
    /**
     * @brief Constructor function
     * @param document A weak pointer to the associated document.
     * @param partId The part that this is for (probably always 0).
     * @param shareMode The sharing mode for this #GFrameHold (probably always #ShareMode::All)
     * @param inst The staff ID for this #GFrameHold.
     * @param meas The measure ID for this #GFrameHold.
     */
    explicit GFrameHold(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper inst, Cmper meas)
        : DetailsBase(document, partId, shareMode, inst, meas), frames(MAX_LAYERS) {}

    /**
     * @brief Enum representing the clef mode for the frame.
     */
    enum class ShowClefMode
    {
        WhenNeeded, ///< Clef is displayed only when needed (the default).
        Never,      ///< Clef is never displayed. (xml value is "hidden")
        Always      ///< Clef is always displayed. (xml value is "forced")
    };

    // Public properties corresponding to the XML structure
    std::optional<ClefIndex> clefId;        ///< clef index when there are no mid-measure clef changes. (xml tag is `<clefID>`).
    Cmper clefListId{};                     ///< The clef list ID when there are mid-measure clef changes, if non-zero. (xml tag is `<clefListID>`).
    ShowClefMode showClefMode{};            ///< "Show Clef" mode. (xml tag is `<clefMode>`)
    bool mirrorFrame{};                     ///< Indicates this is a mirror frame. (Not used after Finale 14.5.)
    int clefPercent{};                      ///< Clef percent where 100 means 100%.
    std::vector<Cmper> frames;              ///< @ref others::Frame values for layers 1..4 (layer indices 0..3) if non-zero

    /// @brief returns the inst (staff) number for this #GFrameHold
    InstCmper getStaff() const { return InstCmper(getCmper1()); }

    /// @brief returns the measure number for this #GFrameHold
    MeasCmper getMeasure() const { return MeasCmper(getCmper2()); }

    /** @brief Returns the @ref EntryFrame for all entries in the given layer.
     *
     * @param layerIndex The layer index (0..3) to iterate.
     * @return EntryFrame for layer or nullptr if none.
     */
    std::shared_ptr<const EntryFrame> createEntryFrame(LayerIndex layerIndex) const;
    
    /**
     * @brief iterates the entries for the specified layer in this #GFrameHold from left to right
     * @param layerIndex The layer index (0..3) to iterate.
     * @param iterator The callback function for each iteration.
     * @return true if higher-level iteration should continue. false if it should halt.
     * @throws std::invalid_argument if the layer index is out of range
     */
    bool iterateEntries(LayerIndex layerIndex, std::function<bool(const std::shared_ptr<const EntryInfo>&)> iterator);

    /**
     * @brief iterates the entries for this #GFrameHold from left to right for each layer in order
     * @param iterator The callback function for each iteration.
     * @return true if higher-level iteration should continue. false if it should halt.
     */
    bool iterateEntries(std::function<bool(const std::shared_ptr<const EntryInfo>&)> iterator);

    void integrityCheck() override
    {
        this->DetailsBase::integrityCheck();
        if (clefListId && clefId.has_value()) {
            MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getCmper1()) + " and measure " + std::to_string(getCmper2()) + " has both clef and clef list.");
        }
        if (!clefListId && !clefId.has_value()) {
            MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getCmper1()) + " and measure " + std::to_string(getCmper2()) + " has neither clef nor clef list.");
        }
    }

    constexpr static std::string_view XmlNodeName = "gfhold"; ///< The XML node name for this type.
    static const xml::XmlElementArray<GFrameHold> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffGroup
 * @brief Represents the attributes of a Finale staff group that brackets staves
 *
 * This class is identified by the XML node name "staffGroup".
 */
class StaffGroup : public DetailsBase {
public:
    /** @brief Constructor
     *
     * @param document The document owner of this instance
     * @param partId The part that owns this staff group
     * @param shareMode The sharing mode deduced from xml attributes
     * @param cmper1 In modern Finale files, all groups have 0 for cmper1. In legacy files this was a cmper for an @ref others::InstrumentUsed list.
     * @param cmper2 The identifier for the StaffGroup.
     */
    StaffGroup(const DocumentWeakPtr& document, Cmper partId, Base::ShareMode shareMode, Cmper cmper1, Cmper cmper2)
        : DetailsBase(document, partId, shareMode, cmper1, cmper2) {}

    /** @brief Enum for barline justification */
    using BarlineType = others::Measure::BarlineType;

    /** @brief Enum for horizontal alignment */
    using AlignJustify = others::NamePositioning::AlignJustify;

    /** @brief Enum for how to draw group barlines */
    enum class DrawBarlineStyle
    {
        OnlyOnStaves,       ///< Default value (may not appear in xml)
        ThroughStaves,      ///< Draw barlines through staves (xml value is "group")
        Mensurstriche       ///< Draw barlines between staves (xml value is "Mensurstriche" with capitalization)
    };

    /** @brief Enum for optimization options */
    enum class HideStaves
    {
        Normally,       ///< Hide staves as if there were no StaffGroup (this is the default and may not appear in the xml)
        AsGroup,        ///< Hide staves only if all staves are empty
        None            ///< Never hide the staves in this StaffGroup
    };

    /** @brief Bracket style enum for StaffGroup */
    enum class BracketStyle : int
    {
        None = 0,                   ///< No bracket (the default)
        ThickLine = 1,              ///< Thick line, no hooks
        BracketStraightHooks = 2,   ///< Thick bracket with straight hooks
        PianoBrace = 3,             ///< Piano brace
        Unknown4,                   ///< Possibly never used
        Unknown5,                   ///< Possibly never used
        BracketCurvedHooks = 6,     ///< Thick bracket with curved hooks
        Unknown7,                   ///< Possibly never used
        DeskBracket = 8             ///< Thin bracket with horizontal hook lines
    };

    /** @brief Embedded class to represent the "bracket" node */
    class Bracket
    {
    public:
        BracketStyle style{};       ///< Bracket style (xml node is `<id>`)
        Evpu horzAdjLeft{};         ///< "Distance from Left Edge of Staff" (xml node is `<bracPos>`)
        Evpu vertAdjTop{};          ///< "Vertical Adjust (Top of Bracket)" (xml node is `<bracTop>`)
        Evpu vertAdjBot{};          ///< "Vertical Adjust (Bottom of Bracket)" (xml node is `<bracBot>`)
        bool showOnSingleStaff{};   ///< "Show Bracket If Group Contains Only One Staff" (xml node is `<onSingle>`)

        /**
         * @brief Default constructor for Bracket.
         */
        Bracket() = default;

        static const xml::XmlElementArray<Bracket> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    // Public properties corresponding to the XML structure, ordered as they appear in the XML
    InstCmper startInst{};                    ///< Starting staff ID
    InstCmper endInst{};                      ///< Ending staff ID
    MeasCmper startMeas{};                    ///< Starting measure number
    MeasCmper endMeas{};                      ///< Ending measure number
    Cmper fullNameId{};                       ///< Full name TextBlock cmper (xml node is `<fullID>`)
    int fullNameXadj{};                       ///< Horizontal adjustment for full name (xml node is `<fullXadj>`)
    int fullNameYadj{};                       ///< Vertical adjustment for full name (xml node is `<fullYadj>`)
    std::shared_ptr<Bracket> bracket{};       ///< Bracket Options. The factory guarantees this value to exist.
    BarlineType barlineType{};                ///< Group barline type (xml node is `<barline>`)
    AlignJustify fullNameJustify{};           ///< Full name justification (xml node is `<fullJustify>`)
    AlignJustify abbrvNameJustify{};          ///< Abbreviated name justification (xml node is `<abbrvJustify>`)
    DrawBarlineStyle drawBarlines;            ///< "Draw Barlines" option (xml node is `<groupBarlineStyle>`)
    bool ownBarline{};                        ///< "Use Alternate Group Barline"
    bool fullNameIndivPos{};                  ///< Indicates if full name has individual position (xml node is `<fullIndivPos>`)
    bool abbrvNameIndivPos{};                 ///< Indicates if abbreviated name has individual position (xml node is `<abbrvIndivPos>`)
    bool hideName{};                          ///< Inverse of "Show Group Name"
    Cmper abbrvNameId{};                      ///< Abbreviated name TextBlock cmper (xml node is `<abbrvID>`)
    int abbrvNameXadj{};                      ///< Horizontal adjustment for abbreviated name (xml node is `<abbrvXadj>`)
    int abbrvNameYadj{};                      ///< Vertical adjustment for abbreviated name (xml node is `<abbrvYadj>`)
    AlignJustify fullNameAlign{};             ///< Full name horizontal alignment (xml node is `<fullHAlign>`)
    AlignJustify abbrvNameAlign{};            ///< Abbreviated name horizontal alignment (xml node is `<abbrvHAlign>`)
    bool fullNameExpand{};                    ///< "Expand Single Word" for full name (xml node is `<fullExpand>`)
    bool abbrvNameExpand{};                   ///< "Expand Single Word" for abbreviated name (xml node is `<abbrvExpand>`)
    HideStaves hideStaves{};                  ///< "When Hiding Empty Staves" option (xml node is `<optimize>`)

    Cmper multiStaffGroupId{};      ///< Calculated cmper for @ref others::MultiStaffGroupId, if any. This value is not in the xml.
                                    ///< It is set by the factory with the Resolver function for @ref others::MultiStaffGroupId.
    std::unordered_set<InstCmper> staves; ///< Calculated list of staves in the group

    /// @brief Get the full staff name without Enigma tags
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    std::string getFullName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Get the abbreviated staff name without Enigma tags
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    std::string getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Returns the @ref others::MultiStaffInstrumentGroup for this group if it is part of one. Otherwise nullptr.
    std::shared_ptr<others::MultiStaffInstrumentGroup> getMultiStaffInstGroup() const;

    /// @brief Returns the full instrument name for this group without Enigma tags and with autonumbering (if any).
    /// @note Ordinal prefix numbering is currently supported only for English.
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    /// @return Full instrument name if this group is associated with a @ref others::MultiStaffInstrumentGroup. Otherwise #getFullName.
    std::string getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Returns the abbreviated instrument name for this group without Enigma tags and with autonumbering (if any)
    /// @note Ordinal prefix numbering is currently supported only for English.
    /// @param accidentalStyle The style for accidental subsitution in names like "Clarinet in Bb".
    /// @return Abbreviated instrument name if this group is associated with a @ref others::MultiStaffInstrumentGroup. Otherwise #getAbbreviatedName.
    std::string getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle = util::EnigmaString::AccidentalStyle::Ascii) const;

    /// @brief Returns true if this group spans all measures. 
    bool isAllMeasures() const
    { return (startMeas == 1 && endMeas == (std::numeric_limits<MeasCmper>::max)()); }

    void integrityCheck() override
    {
        this->DetailsBase::integrityCheck();
        if (endMeas <= startMeas || startMeas <= 0) {
            MUSX_INTEGRITY_ERROR("Staff group " + std::to_string(getCmper2()) + " for part " + std::to_string(getPartId())
                + " starts at measure " + std::to_string(startMeas) + " and ends at measure " + std::to_string(endMeas));
        }
        if (!bracket) {
            // this is not an error. Finale omits the bracket node for groups with entirely default bracket info.
            bracket = std::make_shared<Bracket>();
        }
    }

    constexpr static std::string_view XmlNodeName = "staffGroup"; ///< XML node name for this type.
    static const xml::XmlElementArray<StaffGroup> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TupletDef
 * @brief Options controlling the appearance of tuplets.
 *
 * This class is identified by the XML node name "tupletDef".
 */
class TupletDef : public EntryDetailsBase
{
public:
    /** @brief Constructor function */
    explicit TupletDef(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, Inci inci)
        : EntryDetailsBase(document, partId, shareMode, entnum, inci)
    {
    }

    /// @brief see @ref options::TupletOptions::AutoBracketStyle
    using AutoBracketStyle = options::TupletOptions::AutoBracketStyle;
    /// @brief see @ref options::TupletOptions::NumberStyle
    using NumberStyle = options::TupletOptions::NumberStyle;
    /// @brief see @ref options::TupletOptions::PositioningStyle
    using PositioningStyle = options::TupletOptions::PositioningStyle;
    /// @brief see @ref options::TupletOptions::BracketStyle
    using BracketStyle = options::TupletOptions::BracketStyle;

    int displayNumber{};                    ///< The number of notes to display (xml node is `<symbolicNum>`)
    Edu displayDuration{};                  ///< The duration of each note to display (xml node is `<symbolicDur>`)
    int referenceNumber{};                  ///< The number of notes "in the time of" (xml node is `<refNum>`)
    Edu referenceDuration{};                ///< The duration of eacn note "in the time of" (xml node is `<refDur>`)
    bool alwaysFlat{};                      ///< "Always Flat" (xml node is `<flat>`)
    bool fullDura{};                        ///< "Bracket Full Duration"
    bool metricCenter{};                    ///< "Center Number Using Duration"
    bool avoidStaff{};                      ///< "Avoid Staff"
    AutoBracketStyle autoBracketStyle{};    ///< Autobracket style
    Evpu tupOffX{};                         ///< Horizontal offset.
    Evpu tupOffY{};                         ///< Vertical.
    Evpu brackOffX{};                       ///< Horizontal offset for brackets.
    Evpu brackOffY{};                       ///< Vertical offset for brackets.
    NumberStyle numStyle{};                 ///< Number style
    PositioningStyle posStyle{};            ///< Positioning style
    bool allowHorz{};                       ///< "Allow Horizontal Drag"
    bool ignoreHorzNumOffset{};             ///< "Ignore Horizontal Number Offset" (xml node is `<ignoreGlOffs>`)
    bool breakBracket{};                    ///< "Break Slur or Bracket"
    bool matchHooks{};                      ///< "Match Length of Hooks"
    bool useBottomNote{};                   ///< "Use Bottom Note" (xml node is `<noteBelow>`)
    BracketStyle brackStyle{};              ///< Bracket style.
    bool smartTuplet{};                     ///< "Engraver Tuplets"
    Evpu leftHookLen{};                     ///< Length of the left hook in the tuplet bracket. (This value is sign-reversed in the Finale UI.)
    Evpu leftHookExt{};                     ///< Extension of the left hook beyond the tuplet bracket.
    Evpu rightHookLen{};                    ///< Length of the right hook in the tuplet bracket. (This value is sign-reversed in the Finale UI.)
    Evpu rightHookExt{};                    ///< Extension of the right hook beyond the tuplet bracket.
    Evpu manualSlopeAdj{};                  ///< "Manual Slope Adjustment" in @ref Evpu. (xml node is `<slope>`)

    /** @brief return the reference duration as a @ref util::Fraction of a whole note */
    util::Fraction calcReferenceDuration() const { return util::Fraction(referenceNumber * referenceDuration, Edu(Entry::NoteType::Whole)); }

    /** @brief return the display duration as a @ref util::Fraction of a whole note */
    util::Fraction calcDisplayDuration() const { return util::Fraction(displayNumber * displayDuration, Edu(Entry::NoteType::Whole)); }

    /** @brief return the tuplet ratio (reference / display) */
    util::Fraction calcRatio() const { return util::Fraction(referenceNumber * referenceDuration, displayNumber * displayDuration); }
        
    constexpr static std::string_view XmlNodeName = "tupletDef";    ///< The XML node name for this type.
    static const xml::XmlElementArray<TupletDef> XmlMappingArray;   ///< Required for musx::factory::FieldPopulator.
};

} // namespace details
} // namespace dom
} // namespace entries