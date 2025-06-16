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

#include <tuple>
#include <map>

#include "musx/util/Fraction.h"
#include "BaseClasses.h"
#include "CommonClasses.h"
 // do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

 namespace music_theory {
    class Transposer;
} // namespace music_theory
    
namespace musx {
namespace dom {

namespace others {
class PercussionNoteInfo;
class Staff;
class StaffComposite;
} // namespace others

namespace details {
class TupletDef;
class GFrameHold;

/**
 * @class GFrameHoldContext
 * @brief A context wrapper for @ref GFrameHold associated with a specific part and location.
 *
 * This class retrieves the appropriate @ref GFrameHold from a Document using part, instrument, and measure IDs,
 * and enables part-aware operations like iterating over EntryFrame objects.
 */
class GFrameHoldContext {
public:
    /**
     * @brief Constructs a context-aware @ref GFrameHold wrapper.
     * 
     * @param document Weak pointer to the owning Document.
     * @param partId The requested part ID.
     * @param inst The instrument ID for.
     * @param meas The measure ID for.
     */
    GFrameHoldContext(const DocumentPtr& document, Cmper partId, Cmper inst, Cmper meas);

    /**
     * @brief Returns the requested part ID associated with this context.
     * 
     * @return The requested part ID.
     */
    Cmper getRequestedPartId() const { return m_requestedPartId; }

    /**
     * @brief Provides const pointer-style access to the underlying @ref GFrameHold.
     * 
     * @return A const pointer to @ref GFrameHold.
     */
    const GFrameHold* operator->() const { return m_hold.get(); }

    /**
     * @brief Returns true if the internal @ref GFrameHold is valid.
     * 
     * @return True if the @ref GFrameHold was successfully retrieved; false otherwise.
     */
    explicit operator bool() const { return static_cast<bool>(m_hold); }

    /// @brief Returns the clef index in effect for at the specified @ref Edu position.
    /// This function does not take into account transposing clefs. Those are addressed in #createEntryFrame.
    /// @param position The Edu position of the clef *in staff-level Edus*. (The staff-level matters for Independent Key Signature staves.)
    ClefIndex calcClefIndexAt(Edu position) const;

    /// @brief Returns the clef index in effect for at the specified @ref util::Fraction position (as a fraction of whole notes).
    /// This function does not take into account transposing clefs. Those are addressed in #createEntryFrame.
    /// @param position The *staff-level* position of the clef. (The staff-level matters for Independent Key Signature staves.)
    ClefIndex calcClefIndexAt(util::Fraction position) const
    { return calcClefIndexAt(position.calcEduDuration()); }

    /** @brief Returns the @ref EntryFrame for all entries in the given layer.
     *
     * @param layerIndex The layer index (0..3) to iterate.
     * @param forWrittenPitch If true, the key and clef for each entry are calculated for written pitch rather than concert pitch.
     * @return EntryFrame for layer or nullptr if none.
     */
    std::shared_ptr<const EntryFrame> createEntryFrame(LayerIndex layerIndex, bool forWrittenPitch = false) const;
    
    /**
     * @brief iterates the entries for the specified layer in this @ref GFrameHold from left to right
     * @param layerIndex The layer index (0..3) to iterate.
     * @param iterator The callback function for each iteration.
     * @return true if higher-level iteration should continue. false if it should halt.
     * @throws std::invalid_argument if the layer index is out of range
     */
    bool iterateEntries(LayerIndex layerIndex, std::function<bool(const EntryInfoPtr&)> iterator);

    /**
     * @brief iterates the entries for this @ref GFrameHold from left to right for each layer in order
     * @param iterator The callback function for each iteration.
     * @return true if higher-level iteration should continue. false if it should halt.
     */
    bool iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator);

    /// @brief Calculates the number of voices used by the GFrameHold instance.
    /// @return A list of each layer that contains entries and whether that layer uses voice2.
    std::map<LayerIndex, bool> calcVoices() const;

private:
    std::shared_ptr<GFrameHold> m_hold;      ///< The resolved GFrameHold object, or null if not found.
    Cmper m_requestedPartId;                 ///< The requested part context.
};

} // namespace details

/**
 * @brief Calculates the @ref NoteType and number of dots in an @ref Edu value.
 *
 * @param duration The Edu duration to check.
 * @return A std::pair containing:
 *         - NoteType: The note type (Whole, Quarter, Eighth, etc.)
 *         - unsigned: The number of augmentation dots
 * @throws std::invalid_argument if the duration is out of valid range (> 1 and < 0x10000).
 */
std::pair<NoteType, unsigned> calcNoteInfoFromEdu(Edu duration);

/// @brief Calculates the number of beams or flags in the @ref Edu value.
unsigned calcNumberOfBeamsInEdu(Edu duration);

/**
 * @class Note
 * @brief Represents a single note element in an entry.
 *
 * This class is identified by the XML node name "note".
 */
class Note : public Base
{
public:
    /** @brief Constructor function */
    explicit Note(const DocumentWeakPtr& document, NoteNumber noteId)
        : Base(document, 0, ShareMode::All), m_noteId(noteId)
    {
    }

    /// @brief Non floating rests have a note with this noteId that defines their staff positions.
    static constexpr NoteNumber RESTID = 31;

    /// @brief The available note names, in array order.
    enum class NoteName : int
    {
        C = 0,
        D = 1,
        E = 2,
        F = 3,
        G = 4,
        A = 5,
        B = 6
    };
    
    int harmLev{};      ///< Diatonic displacement relative to middle C or to the tonic in the middle C octave (if the key signature tonic is not C).
    int harmAlt{};      ///< Chromatic alteration relative to the key signature. Never has a magnitude greater than +/-7.
    bool isValid{};     ///< Should always be true but otherwise appears to be used internally by Finale.
    bool crossStaff{};  ///< Signifies that the note has a @ref details::CrossStaff note detail.
    bool tieStart{};    ///< Indicates a tie starts on this note.
    bool tieEnd{};      ///< Indicates a tie ends on this note.
    bool upStemSecond{}; ///< Indicates that this note is the upper note of a second.
                        ///< When the entry is upstem, it is drawn on the "wrong" side of the stem.
    bool downStemSecond{}; ///< Indicates that this note is the lower note of a second.
                        ///< When the entry is downstem, it is drawn on the "wrong" side of the stem.
    bool upSplitStem{}; ///< True if the stem splits on this note. To split a chord in the normal way, every note from this higher
                        ///< should have #upSplitStem set to `true`. Only takes effect if #Entry::splitStem is `true`.
    bool showAcci{};    ///< True if the note has an accidental. (Dynamically changed by Finale unless `freezeAcci` is set.)
    bool parenAcci{};   ///< True if the accidental has parentheses.
    bool freezeAcci{};  ///< True if the accidental should be forced on or off (based on `showAcci`.)

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

    /// @brief Calculates the default enharmonic equivalent of this note. This is the value that Finale uses when
    /// #details::NoteAlterations::enharmonic is true.
    ///
    /// Normally you do not have to call this function directly. It is called inside #calcNoteProperties. But the function
    /// is available if you need it.
    ///
    /// @param key The key signature to use for transposition.
    /// @return A std::pair containing
    ///         - int: the enharmonic equivalent's displacement value relative to the tonic.
    ///         - int: the enharmonic equivalent's alteration value relative to the key signature.
    std::pair<int, int> calcDefaultEnharmonic(const std::shared_ptr<KeySignature>& key) const;

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position. This function does
     * not take into account percussion notes and their staff position override.
     *
     * Finale does not transpose correctly with microtonal key signatures. This function transposes
     * mostly correctly for them, which means that microtonal key signatures may have different
     * results than in Finale. Of particular note is Chromatic Transposition, which produces nonsense
     * results in Finale but here produces correct results, provided that the alteration value
     * is understood to be a chromatic half-step alteration rather than a step alteration in EDO divisions.
     *
     * See #KeySignature::setTransposition for information about differences in key signature transposition.
     *
     * @param key The key signature in effect.
     * @param clefIndex The index of the clef in effect.
     * @param staff If provided, the notes are transposed by any Chromatic Transposition specified for the staff. If
     * calling #calcNoteProperties for Concert Pitch (sounding pitch) values, omit this parameter.
     * @param respellEnharmonic If true, the notes are enharmonically respelled using the default enharmonic spelling.
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<NoteName, int, int, int> calcNoteProperties(const std::shared_ptr<KeySignature>& key, ClefIndex clefIndex,
        const std::shared_ptr<const others::Staff>& staff = nullptr, bool respellEnharmonic = false) const;

    bool requireAllFields() const override { return false; }

    static const xml::XmlElementArray<Note>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.

private:
    NoteNumber m_noteId{}; ///< Unique identifier for the note.
};

/**
 * @class Entry
 * @brief Represents an entry containing metadata and notes.
 *
 * This class is identified by the XML node name "entry".
 */
class Entry : public Base
{
public:
    /** @brief Constructor function
     *
     * The partId and shareMode values should always be 0 and ShareMode::All, but they are required by the factory function.
    */
    explicit Entry(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, EntryNumber prev, EntryNumber next)
        : Base(document, partId, shareMode), m_entnum(entnum), m_prev(prev), m_next(next)
    {
    }

    /**
     * @brief Duration of the entry, not taking into account tuplets.
     *
     * This is effectively the type of note (quarter, eight, whole etc.) plus the augmentation dots, if any.
     * The most significant bit determines the note type and each set bit to the right is an augmentation dot.
     *
     * (xml node is `<dura>`)
    */
    Edu duration{};
    int numNotes{};          ///< Number of notes in the entry. There is an error if this is not the same as notes.size().
    Evpu hOffset{};          ///< Manual offset created with the Note Position Tool. (xml node is `<posi>`.)
    bool isValid{};          ///< Should always be true but otherwise appears to be used internally by Finale.
    bool isNote{};           ///< If this value is false, the entry is a rest.
    bool graceNote{};        ///< Indicate the entry is a grace note.
    bool floatRest{};        ///< Is floating rest. If false, the first note element gives the staff position of the rest.
    bool isHidden{};         ///< Indicates the entry is hidden, (xml node is `<ignore>`)
    bool voice2{};           ///< This is a V2 note. (xml node `<v2>`)
    bool articDetail{};      ///< Indicates there is an articulation on the entry
    bool noteDetail{};       ///< Indicates there is a note detail or EntrySize record for the entry.
    bool dotTieAlt{};        ///< Indicates dot or tie alterations are present.
    bool tupletStart{};      ///< Indicates that a tuplet start on the entry.
    bool beamExt{};          ///< Indicates that there is a beam extension on the entry.
    bool beam{};             ///< Signifies the start of a beam or singleton entry. (That is, any beam breaks at this entry.)
    bool secBeam{};          ///< Signifies a secondary beam break occurs on the entry.
    bool doubleStem{};       ///< Creates a double stem on the entry. (Appears to be exclusive with #splitStem.)
    bool splitStem{};        ///< Indicates the presence of a note with #Note::upSplitStem set.
                             ///< If no note has a split stem, it shows as a double stem. (Appears to be exclusive with #doubleStem.)
    bool crossStaff{};       ///< Signifies that at least one note in the entry has been cross staffed.
    bool freezeStem{};       ///< Freeze stem flag (#upStem gives the direction.)
    bool upStem{};           ///< Whether a stem is up or down. (Only reliable when #freezeStem is true.)
    bool noLeger{};          ///< Hide ledger lines.
    bool stemDetail{};       ///< Indicates there are stem modifications.
    bool reverseUpStem{};    ///< Indicates that a stem normally up is reversed.
    bool reverseDownStem{};  ///< Indicates that a stem normally down is reversed.
    bool smartShapeDetail{}; ///< Indicates this entry has a smart shape assignment.
    bool sorted{};           ///< Sorted flag.
    bool noPlayback{};       ///< Indicates that the entry should not be played back.
    bool lyricDetail{};      ///< Indicates there is a lyric assignment on the entry.
    bool performanceData{};  ///< Indicates there is performance data on the entry.
    bool freezeBeam{};       ///< Freeze beam flag (Derived from the presence of `<freezeBeam>` node.)

    /** @brief Collection of notes that comprise the entry. These are in order from lowest to highest. */
    std::vector<std::shared_ptr<Note>> notes;

    /// @brief Gets the entry number for this entry
    EntryNumber getEntryNumber() const { return m_entnum; }

    /// @brief Gets the next entry in this list or nullptr if none.
    ///
    /// Note that the entry list may contain entries that aren't in any frame. These should be ignored.
    std::shared_ptr<Entry> getNext() const;

    /// @brief Gets the previous entry in this list or nullptr if none
    ///
    /// Note that the entry list may contain entries that aren't in any frame. These should be ignored.
    std::shared_ptr<Entry> getPrevious() const;

    /**
     * @brief Calculates the NoteType and number of augmentation dots. (See #calcNoteInfoFromEdu.)
     */
    std::pair<NoteType, unsigned> calcNoteInfo() const { return calcNoteInfoFromEdu(duration); }

    /**
     * @brief Calculates the duration as a @ref util::Fraction of a whole note
     */
    util::Fraction calcFraction() const { return util::Fraction::fromEdu(duration); }

    /// @brief Returns true if the entry's duration has a stem.
    /// @return True if the entry's duration is less than a whole note, irrespective of whether it is a rest or a note.
    bool hasStem() const { return duration < Edu(NoteType::Whole); }

    void integrityCheck() override
    {
        this->Base::integrityCheck();
        if (size_t(numNotes) != notes.size()) {
            MUSX_INTEGRITY_ERROR("Entry " + std::to_string(m_entnum) + " has an incorrect number of notes.");
        }
    }

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "entry"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Entry>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.

private:    
    EntryNumber m_entnum;   ///< Entry number.
    EntryNumber m_prev;     ///< Previous entry number in the list. (0 if none)
    EntryNumber m_next;     ///< Next entry number in the list. (0 if none)
    
};

class EntryInfo;
class EntryFrame;
class NoteInfoPtr;

/// @brief Wraps a frame of shared_ptr<const EntryInfo> and an index for per entry access.
/// This class manages ownership of the frame so that any instance of it keeps the frame alive
/// without the need for circular references.
class EntryInfoPtr
{
public:
    /** @brief Default constructor */
    EntryInfoPtr() : m_entryFrame(nullptr), m_indexInFrame(0) {}
    
    /** @brief Constructor function
     *
     * @param entryFrame The entry frame.
     * @param index The index of this instance within the frame. 
    */
    explicit EntryInfoPtr(const std::shared_ptr<const EntryFrame>& entryFrame, size_t index = 0)
        : m_entryFrame(entryFrame), m_indexInFrame(index) {}

    /// @brief Allows `->` access to the underlying @ref EntryInfo instance.
    const std::shared_ptr<const EntryInfo> operator->() const;

    /// @brief Provides a boolean conversion based on whether the frame is valid and contains entries.
    operator bool() const;

    /// @brief Returns whether the input and the current instance refer to the same entry.
    /// @return false if either this or src is null and true if they are both non null and refer to the same entry.
    bool isSameEntry(const EntryInfoPtr& src) const;

    /// @brief Returns the frame.
    std::shared_ptr<const EntryFrame> getFrame() const { return m_entryFrame; }

    /// @brief Returns the index within the frame.
    size_t getIndexInFrame() const { return m_indexInFrame; }

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const;

    /// @brief Get the staff cmper
    InstCmper getStaff() const;

    /// @brief Creates the current StaffComposite for the entry
    /// @param forStaffId Specifies optional staff ID. If supplied, it overrides the entry's staff ID. (Useful when notes are cross-staffed.)
    std::shared_ptr<others::StaffComposite> createCurrentStaff(const std::optional<InstCmper>& forStaffId = std::nullopt) const;

    /// @brief Get the measure cmper
    MeasCmper getMeasure() const;

    /// @brief Get the key signature of the entry
    std::shared_ptr<KeySignature> getKeySignature() const;

    /// @brief Caclulates the grace index counting leftward (used by other standards such as MNX)
    unsigned calcReverseGraceIndex() const;

    /// @brief Returns the next higher tuplet index that this entry starts, or std::nullopt if none
    std::optional<size_t> calcNextTupletIndex(std::optional<size_t> currentIndex) const;

    /// @brief Get the next entry in the frame
    EntryInfoPtr getNextInFrame() const;

    /// @brief Get the next entry in the same layer and staff. This can be in the next measure.
    /// @return  The next continguous entry. Returns nullptr if it encounters an empty frame or end of file.
    EntryInfoPtr getNextInLayer() const;

    /// @brief Get the next entry in the frame in the same voice
    ///
    /// For V2, it returns null after the current V2 launch sequence.
    EntryInfoPtr getNextSameV() const;

    /// @brief Get the previous entry in the same layer and staff. This can be in the previous measure.
    /// @return  The previous continguous entry. Returns nullptr if it encounters an empty frame or the beginning of the file.
    EntryInfoPtr getPreviousInLayer() const;

    /// @brief Get the previous entry in the frame
    EntryInfoPtr getPreviousInFrame() const;

    /// @brief Get the previous entry in the frame in the same voice
    ///
    /// For V2, it returns null when it hits the v2Launch note for the current V2 launch sequence.
    EntryInfoPtr getPreviousSameV() const;

    /// @brief Returns the next entry in the frame in the specified v1/v2 or null if none.
    ///
    /// Unlike #getNextSameV, this returns the next v2 entry in any v2 launch sequence.
    ///
    /// @param voice 1 or 2
    EntryInfoPtr getNextInVoice(int voice) const;

    /// @brief Returns the previous entry in the frame in the specified v1/v2 or null if none.
    ///
    /// Unlike #getPreviousSameV, this returns the next v2 entry in any v2 launch sequence.
    ///
    /// @param voice 1 or 2
    EntryInfoPtr getPreviousInVoice(int voice) const;

    /// @brief Gets the next entry in a beamed group or nullptr if the entry is not beamed or is the last in the group.
    EntryInfoPtr getNextInBeamGroup(bool includeHiddenEntries = false) const
    { return iterateBeamGroup<&EntryInfoPtr::nextPotentialInBeam, &EntryInfoPtr::previousPotentialInBeam>(includeHiddenEntries); }

    /// @brief Gets the previous entry in a beamed group or nullptr if the entry is not beamed or is the first in the group.
    EntryInfoPtr getPreviousInBeamGroup(bool includeHiddenEntries = false) const
    { return iterateBeamGroup<&EntryInfoPtr::previousPotentialInBeam, &EntryInfoPtr::nextPotentialInBeam>(includeHiddenEntries); }

    /// @brief Calculates if an entry displays as a rest.
    /// @todo Eventually calcDisplaysAsRest should take into account voiced parts.
    bool calcDisplaysAsRest() const;

    /// @brief Returns whether this is an unbeamed entry
    /// @return 
    bool calcUnbeamed() const;

    /// @brief Returns whether this is the start of a primary beam
    bool calcIsBeamStart() const;

    /// @brief Calculates if the entry starts a feathered beam and returns information about it if so.
    /// @param [out] outLeftY The height of the left side of the feathered beam
    /// @param [out] outRightY The height of the right side of the feathered beam
    /// @return true if this is a feathered beam. If the return value is false, outLeftY and outRightY are unchanged.
    bool calcIsFeatheredBeamStart(Evpu& outLeftY, Evpu& outRightY) const;

    /// @brief Finds the first entry of a beamed group or returns the current entry if it is not beams.
    /// @note This behavior is different than other beam functions in that it returns the current entry if there is no beam
    /// rather than returning NULL.
    /// @return The first entry of a beamed group or the current entry if no beam.
    EntryInfoPtr findBeamStartOrCurrent() const;

    /// @brief Finds the end entry of a beamed group.
    /// @return The entry if found, NULL if the entry cannot be beamed or if it is not part of a beamed group.
    EntryInfoPtr findBeamEnd() const;

    /// @brief Calculates the number of beams or flags on the entry.
    unsigned calcNumberOfBeams() const;

    /// @brief Returns the lowest beam number starting at this entry, where 1 = 8th note beam, 2 = 16th note beam, etc.
    /// @return 0 if not beamed or no beam starts this entry; otherwise, the beam number
    unsigned calcLowestBeamStart() const;

    /// @brief Returns the lowest beam number ending at this entry, where 1 = 8th note beam, 2 = 16th note beam, etc.
    /// @return 0 if not beamed or no beam ends this entry; otherwise, the beam number
    unsigned calcLowestBeamEnd() const;

    /// @brief Returns the lowest beam stub at this entry, where 2 = 16th note stub, 3 = 32nd note stub, etc.
    /// @return 0 if not beamed or no beam stub exists on this entry; otherwise, the lowest beam stub number
    unsigned calcLowestBeamStub() const;

    /// @brief Calculates if a beam stub on this entry would go left or right. It does not check that an entry actually has a beam stub.
    /// Use #calcLowestBeamStub to discover if the entry has a beam stub.
    /// @note This is a best approximation of Finale's behavior for default beam stub direction. No doubt there are edge cases
    /// where it does not match.
    /// @return True if a beam stub would go left; false if it would go right or if no calculation is possible.
    bool calcBeamStubIsLeft() const;

    /// @brief Calculates the elapsed duration in global edu, removing any time stretch due to independent time signature
    util::Fraction calcGlobalElapsedDuration() const;

    /// @brief Calculates the actual duration in global edu, removing any time stretch due to independent time signature
    util::Fraction calcGlobalActualDuration() const;

    /// @brief Determines if this entry can be beamed.
    bool canBeBeamed() const;

    /// @brief Calculates if this entry is part of a cue.
    /// @return true if
    ///         - the entry is reduced in size
    ///         - the entry is hidden by "Blank Notation" or "Blank Notation with Rests" alternate notation in the score but not in a part.
    bool calcIsCue() const;

    /// @brief Returns whether this is a full measure rest.
    /// @note Note that in Finale, only whole rests are used as full measure rests.
    bool calcIsFullMeasureRest() const;

    /// @brief A common workaround in Finale is to hide a rest in v1 and supply it in v2. Typicall it is used when a beam starts or ends with
    /// a 16th beam hook, has a 16th rest in the middle and an 8th note on the other end. This code detects that situation.
    /// @return True if this is either the replacement rest in v2 or the hidden rest in v1.
    bool calcIsBeamedRestWorkaroud() const;

private:
    unsigned calcVisibleBeams() const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    std::optional<unsigned> iterateFindRestsInSecondaryBeam(const EntryInfoPtr nextOrPrevInBeam) const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    EntryInfoPtr iteratePotentialEntryInBeam() const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
    bool iterateNotesExistLeftOrRight() const;

    EntryInfoPtr nextPotentialInBeam(bool includeHiddenEntries) const;

    EntryInfoPtr previousPotentialInBeam(bool includeHiddenEntries) const;

    template<EntryInfoPtr(EntryInfoPtr::* Iterator)(bool) const, EntryInfoPtr(EntryInfoPtr::* ReverseIterator)(bool) const>
    EntryInfoPtr iterateBeamGroup(bool includeHiddenEntries) const;

    std::shared_ptr<const EntryFrame> m_entryFrame;
    size_t m_indexInFrame{};              ///< the index of this item in the frame.
};

/**
 * @class EntryFrame
 * @brief Represents a vector of @ref EntryInfo instances for a given frame, along with computed information.
 *
 * Its pointers are owned by @ref EntryInfoPtr
 */
class EntryFrame : public std::enable_shared_from_this<EntryFrame>
{
public:
    /** @brief Constructor function
     *
     * @param gfhold The @ref details::GFrameHoldContext instance creating this EntryFrame
     * @param layerIndex The @ref LayerIndex (0..3) of the entry frame
     * @param forWrittenPitch If true, the key and clef for each entry are calculated for written pitch rather than concert pitch.
     * @param timeStretch The ratio of global Edu to staff edu.
    */
    explicit EntryFrame(const details::GFrameHoldContext& gfhold, LayerIndex layerIndex, bool forWrittenPitch, util::Fraction timeStretch) :
        m_context(gfhold),
        m_layerIndex(layerIndex),
        m_forWrittenPitch(forWrittenPitch),
        m_timeStretch(timeStretch)
    {
    }

    /// @brief class to track tuplets in the frame
    struct TupletInfo
    {
        std::shared_ptr<const details::TupletDef> tuplet;   ///< the tuplet
        size_t startIndex;                                  ///< the index of the first entry in the tuplet
        size_t endIndex;                                    ///< the index of the last entry in the tuplet
        util::Fraction startDura;                           ///< the actual duration where the tuplet starts
        util::Fraction endDura;                             ///< the actual duration where the tuplet ends
        bool voice2;                                        ///< whether this tuplet is for voice2

        /// @brief Constructor
        TupletInfo(const std::weak_ptr<const EntryFrame>& parent, const std::shared_ptr<const details::TupletDef>& tup, size_t index, util::Fraction start, bool forVoice2)
            : tuplet(tup), startIndex(index), endIndex(std::numeric_limits<size_t>::max()),
                startDura(start), endDura(-1), voice2(forVoice2), m_parent(parent)
        {}

        /// @brief Calculates if this tuplet represents a tremolo based on the following criteria.
        ///     - the tuplet ratio is a positive integral power of 2.
        ///     - the tuplet contains exactly 2 entries of equal duration and actual duration.
        ///     - the tuplet is invisible.
        ///
        /// @note The TGTools Tremolo plugin always creates beam extensions for both upstem and downstem cases. To detect the type of
        /// stem connections for the tremolo, it is recommended to look for either an upstem or a downstem extension. This
        /// covers the TGTools plugin as well as any that might have been created by hand. See @ref details::BeamExtension.
        ///
        /// @return true if the tuplet is a tremolo. If so, use `EntryInfoPtr::calcNumberOfBeams` on either entry to determine
        /// the number of beams. Use `details::TupletDef::calcReferenceDuration` to get the total length of the tremolo.
        bool calcIsTremolo() const;

        /// @brief Calculates if this tuplet is being used to create a singleton beam to the right.
        ///
        /// Finale has no built-in support for beams on singleton notes. As a workaround, users and (especially)
        /// plugins such as Beam Over Barline create singleton beams using a 0-length tuplet and hiding either the tuplet
        /// note or its next neighbor, depending on whether the beam goes to the left or the right. You should never
        /// encounter a 0-length tuplet encompassing more than one entry, but these functions guarantee this if they return `true`.
        ///
        /// @return True if this tuplet creates a singleton beam to the right. You may handle this as follows.
        ///     - The entry with the tuplet is the visible entry to use. You can mark this entry as having a singleton beam right, if your application allows it.
        ///     - Ignore the tuplet on the visible entry. If you need the entry's actual duration in context, its next neighbor in the same voice
        /// has the correct value.
        ///     - Ignore the entry's next neighbor in the same voice. It will have its leger lines suppressed and non-visible notehead(s) and stem.
        /// Its `hidden` flag, however, will still be false. (This function guarantees these conditions if it returns `true`.)
        bool calcCreatesSingletonRight() const { return calcCreatesSingleton(false); }

        /// @brief Calculates if this tuplet is being used to create a singleton beam to the left.
        ///
        /// See comments at #calcCreatesSingletonRight.
        ///
        /// @return True if this tuplet creates a singleton beam to the left. You may handle this as follows.
        ///     - Skip the entry and its tuplet.
        ///     - You can mark the next entry in the same voice as having a singleton beam left, if your application allows it.
        ///     - The current entry with the 0-length tuplet will have its leger lines suppressed and non-visible notehead(s) and stem.
        /// Its `hidden` flag, however, will still be false. (This function guarantees these conditions if it returns `true`.)
        bool calcCreatesSingletonLeft() const { return calcCreatesSingleton(true); }

        /// @brief Calculates if this tuplet creates a beam continuation over a barline to the right,
        /// as created by the Beam Over Barlines plugin.
        ///
        /// @note The Beam Over Barlines plugin has poor support for v1/v2. This function detects v1/v2 correctly on the chance
        /// that a user may have manually setup v1/v2 the way Beam Over Barlines should have.
        ///
        /// @return If the function returns true, you can treat the result similarly to the result from #calcCreatesSingletonRight.
        /// However, you simply extend a beam from the designated entry to the appropriate entries in the next measure.
        bool calcCreatesBeamContinuationRight() const;

        /// @brief Calculates if this tuplet creates a beam continuation over a barline to the left,
        /// as created by the Beam Over Barlines plugin.
        ///
        /// @note The Beam Over Barlines plugin has poor support for v1/v2. This function detects v1/v2 correctly on the chance
        /// that a user may have manually setup v1/v2 the way Beam Over Barlines should have.
        ///
        /// @return If the function returns true, you can treat the result similarly to the result from #calcCreatesSingletonLeft.
        /// However, you simply extend a beam from the designated entry to the appropriate entries in the previous measure.
        bool calcCreatesBeamContinuationLeft() const;

        /// @brief Detects tuplets being used to create time stretch in an independent time signature.
        ///
        /// Because the Finale UI is so buggy with smart shapes and clefs when there is an Independent Time Signature,
        /// a common workaround is to set the Independent Time Signature to the same as the global time signature but
        /// display a different time signature. Invisible tuplets then create the appearance of time stretch.
        ///
        /// @return This function returns true if
        ///     - the tuplet's total reference duration matches the length of the measure exactly 
        ///     - the tuplet is invisible
        ///     - the staff has an independent time signature
        //      - the independent display time signature matches the tuplet's total display duration
        bool calcCreatesTimeStretch() const;

    private:
        bool calcCreatesSingleton(bool left) const;

        const std::weak_ptr<const EntryFrame> m_parent;
    };

    /** @brief A list of the tuplets in the frame and their calculated starting and ending information.
     *
     * @note Tuplets that start on grace notes are omitted from this list. Finale does not display them, and
     * it is not possible to calculate their endpoints correctly in the general case. (Which is probably why
     * Finale does not display them.)
    */
    std::vector<TupletInfo> tupletInfo;
    std::shared_ptr<KeySignature> keySignature; ///< this can be different than the measure key sig if the staff has independent key signatures

    /// @brief Get the document for the entry frame
    DocumentPtr getDocument() const;

    /// @brief Get the requested part ID for the entry frame
    Cmper getRequestedPartId() const { return m_context.getRequestedPartId(); }

    /// @brief Get the staff for the entry
    InstCmper getStaff() const;

    /// @brief Get the measure for the entry frame
    MeasCmper getMeasure() const;

    /// @brief Get the layer index (0..3) of the entry frame
    LayerIndex getLayerIndex() const { return m_layerIndex; }

    /// @brief Returns if this entry frame was created for written pitch.
    /// @return True if for written pitch, false if for sounding pitch (i.e., concert pitch)
    bool isForWrittenPitch() const { return m_forWrittenPitch; }

    /// @brief Get the time stretch in this frame. Rather than accessing this value directly,
    /// consider using #EntryInfoPtr::calcGlobalElapsedDuration or #EntryInfoPtr::calcGlobalActualDuration instead.
    util::Fraction getTimeStretch() const { return m_timeStretch; }

    /// @brief Get the entry list.
    const std::vector<std::shared_ptr<const EntryInfo>>& getEntries() const
    { return m_entries; }

    /// @brief Returns the first entry in the specified v1/v2 or null if none.
    ///
    /// @param voice 1 or 2
    EntryInfoPtr getFirstInVoice(int voice) const;

    /// @brief Returns the last entry in the specified v1/v2 or null if none.
    ///
    /// @param voice 1 or 2
    EntryInfoPtr getLastInVoice(int voice) const;

    /// @brief Add an entry to the list.
    void addEntry(const std::shared_ptr<const EntryInfo>& entry)
    { m_entries.emplace_back(entry); }

    /// @brief Gets the entry frame for the next measure with the same staff and layer.
    /// @return Frame or nullpter if the next measure has no matching frame.
    std::shared_ptr<const EntryFrame> getNext() const;

    /// @brief Gets the entry frame for the previous measure with the same staff and layer.
    /// @return Frame or nullpter if the previous measure has no matching frame,
    std::shared_ptr<const EntryFrame> getPrevious() const;
    
    /// @brief Creates a current StaffComposite for the entry frame.
    /// @param eduPosition The Edu position for which to create the staff.
    /// @param forStaffId Specifies optional staff ID. If supplied, it overrides the entry's staff ID. (Useful when notes are cross-staffed.)
    std::shared_ptr<others::StaffComposite> createCurrentStaff(Edu eduPosition, const std::optional<InstCmper>& forStaffId = std::nullopt) const;

    /// @brief Get the measure instance
    std::shared_ptr<others::Measure> getMeasureInstance() const;

private:
    details::GFrameHoldContext m_context;
    LayerIndex m_layerIndex;
    bool m_forWrittenPitch;
    util::Fraction m_timeStretch;

    std::vector<std::shared_ptr<const EntryInfo>> m_entries;
};

namespace details {
class GFrameHold;
} // namespace details

/**
 * @class EntryInfo
 * @brief Information an entry along with the entry.
 *
 * This class is used in iteration functions to supply information about the entry along with the entry itself.
 *
 * Its pointers are owned by @ref EntryFrame.
 */
class EntryInfo
{
    /** @brief Constructor function
     *
     * @param entry The entry.
    */
    explicit EntryInfo(const std::shared_ptr<const Entry>& entry)
        : m_entry(entry) {}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
    friend details::GFrameHoldContext;
#endif

public:
    util::Fraction elapsedDuration{};   ///< the elapsed duration within the measure where this entry occurs (in fractions of a whole note)
                                        ///< This is a staff-level position and must be scaled for the global value. (Use #EntryInfoPtr::calcGlobalElapsedDuration.)
    util::Fraction actualDuration{};    ///< the actual duration of entry (in fractions of a whole note), taking into account tuplets and grace notes
                                        ///< This is a staff-level value and must be scaled for the global value. (Use #EntryInfoPtr::calcGlobalActualDuration.)
    bool v2Launch{};                    ///< indicates if this entry (which is voice1) launches a voice2 sequence
    unsigned graceIndex{};              ///< the Finale grace note index, counting from 1 starting from the leftmost grace note counting rightward.
                                        ///< the main note has a grace index of zero.
    ClefIndex clefIndex{};              ///< the clef index in effect for the entry.
    ClefIndex clefIndexConcert{};       ///< the concert clef index in effect for the entry.

    /// @brief Get the entry
    /// @throws std::logic_error if the entry pointer is no longer valid 
    std::shared_ptr<const Entry> getEntry() const
    {
        auto retval = m_entry.lock();
        if (!retval) {
            throw std::logic_error("Entry pointer is no longer valid");
        }
        return retval;
    }

    /// @brief Calculates the next duration position after this entry
    util::Fraction calcNextElapsedDuration() const
    { return elapsedDuration + actualDuration; }

private:
    std::weak_ptr<const Entry> m_entry;
};

/// @brief Wraps an @ref EntryInfo instance and a note index.
class NoteInfoPtr
{
public:
    /// @brief Default constructor
    NoteInfoPtr() : m_entry(), m_noteIndex(0) {}

    /// @brief Constructor
    /// @param entryInfo The entry info containing the note.
    /// @param noteIndex The index of this note within #Entry::notes.
    NoteInfoPtr(const EntryInfoPtr& entryInfo, size_t noteIndex)
        : m_entry(entryInfo), m_noteIndex(noteIndex)
    {}

    /// @brief Provides a boolean conversion based on whether the EntryInfoPtr is valid and the note index is valid.
    operator bool() const
    { return m_entry && m_noteIndex < m_entry->getEntry()->notes.size(); }

    /// @brief Returns whether the input and the current instance refer to the same note.
    bool isSameNote(const NoteInfoPtr& src) const
    { return m_entry.isSameEntry(src.m_entry) && m_noteIndex == src.m_noteIndex; }

    /// @brief Finds a note with the same pitch in the supplied entry
    /// @param entry the entry to search
    /// @return The found note or an null instance of NoteInfoPtr.
    NoteInfoPtr findEqualPitch(const EntryInfoPtr& entry) const;

    /// @brief Allows `->` access to the underlying @ref Note instance.
    std::shared_ptr<const Note> operator->() const
    {
        MUSX_ASSERT_IF(m_noteIndex >= m_entry->getEntry()->notes.size()) {
            throw std::logic_error("Note index is too large for notes array.");
        }
        return m_entry->getEntry()->notes[m_noteIndex];
    }

    /// @brief Gets the entry info for this note
    EntryInfoPtr getEntryInfo() const
    { return m_entry; }

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position. This function does
     * not take into account percussion notes and their staff position override. To discover if a note is a percussion
     * note, call #calcPercussionNoteInfo. If it returns non-null, use that for staff position instead of this function.
     * @param enharmonicRespell If supplied, return the default enharmonic respelling based on this value. If omitted,
     * this value is calculated automatically based on the score or part settings. Normally you will omit it.
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<Note::NoteName, int, int, int> calcNoteProperties(const std::optional<bool>& enharmonicRespell = std::nullopt) const;

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position for the concert pitch of thenote. This function does
     * not take into account percussion notes and their staff position override. To discover if a note is a percussion
     * note, call #calcPercussionNoteInfo. If it returns non-null, use that for staff position instead of this function.
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<Note::NoteName, int, int, int> calcNotePropertiesConcert() const;

    /// @brief Calculates the percussion note info for this note, if any.
    /// @return If the note is on a percussion staff and has percussion note info assigned, returns it. Otherwise `nullptr`.
    std::shared_ptr<others::PercussionNoteInfo> calcPercussionNoteInfo() const;

    /// @brief Calculates the note that this note could tie to. Check the return value's #Note::tieEnd
    /// to see if there is actually a tie end.
    /// @return The candidate note or an empty NoteInfoPtr if no candidate was found.
    NoteInfoPtr calcTieTo() const;

    /// @brief Calculates the note that this note could tie from. Check the return value's #Note::tieStart
    /// to see if there is actually a tie.
    /// @return The candidate note or an empty NoteInfoPtr if no candidate was found.
    NoteInfoPtr calcTieFrom() const;

    /// @brief Calculates the staff number, taking into account cross staffing
    InstCmper calcStaff() const;

    /// @brief Creates a transposer for this Note instance.
    /// @return A unique pointer to a transposer for this Note.
    std::unique_ptr<music_theory::Transposer> createTransposer() const;

    /// @brief Gets the next note in a chord on the same entry.
    /// @return The next note or nullptr if none.
    NoteInfoPtr getNext() const
    {
        if (m_noteIndex >= m_entry->getEntry()->notes.size()) {
            return NoteInfoPtr();
        }
        return NoteInfoPtr(m_entry, m_noteIndex + 1);
    }

    /// @brief Gets the next note in a chord on the same entry.
    /// @return The next note or nullptr if none.
    NoteInfoPtr getPrevious() const
    {
        if (m_noteIndex <= 0) {
            return NoteInfoPtr();
        }
        return NoteInfoPtr(m_entry, m_noteIndex - 1);
    }

private:
    /// @brief Returns true if the @p src and this have the same level and alteration.
    /// It is only meaningful when this and src are in the same key.
    /// @param src the value to compare with.
    bool isSamePitchValues(const NoteInfoPtr& src) const
    {
        if (!*this || !src) {
            return false;
        }
        return (*this)->harmLev == src->harmLev
            && (*this)->harmAlt == src->harmAlt;
    }

    EntryInfoPtr m_entry;
    size_t m_noteIndex;
};

} // namespace dom
} // namespace entries