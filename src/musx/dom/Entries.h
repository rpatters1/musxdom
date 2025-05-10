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
class Staff;
class StaffComposite;
} // namespace others

namespace details {
class TupletDef;
class GFrameHold;
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
    bool showAcci{};    ///< True if the note has an accidental. (Dynamically changed by Finale unless `freezeAcci` is set.)
    bool freezeAcci{};  ///< True if the accidental should be forced on or off (based on `showAcci`.)

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position.
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
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<NoteName, int, int, int> calcNoteProperties(const std::shared_ptr<KeySignature>& key, ClefIndex clefIndex,
        const std::shared_ptr<const others::Staff>& staff = nullptr) const;

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
    bool isValid{};          ///< Should always be true but otherwise appears to be used internally by Finale.
    bool isNote{};           ///< If this value is false, the entry is a rest.
    bool graceNote{};        ///< Indicate the entry is a grace note.
    bool floatRest{};        ///< Is floating rest. If false, the first note element gives the staff position of the rest.
    bool isHidden{};         ///< Indicates the entry is hidden, (xml node is `<ignore>`)
    bool voice2{};           ///< This is a V2 note. (xml node `<v2>`)
    bool articDetail{};      ///< Indicates there is an articulation on the entry
    bool noteDetail{};       ///< Indicates there is a note detail for the entry.
    bool beam{};             ///< Signifies the start of a beam or singleton entry. (That is, any beam breaks at this entry.)
    bool secBeam{};          ///< Signifies a secondary beam break occurs on the entry.
    bool crossStaff{};       ///< Signifies that at least one note in the entry has been cross staffed.
    bool freezeStem{};       ///< Freeze stem flag (#upStem gives the direction.)
    bool upStem{};           ///< Whether a stem is up or down. (Only reliable when #freezeStem is true.)
    bool stemDetail{};       ///< Indicates there are stem modification.
    bool smartShapeDetail{}; ///< Indicates this entry has a smart shape assignment.
    bool sorted{};           ///< Sorted flag.
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
    std::pair<NoteType, int> calcNoteInfo() const { return calcNoteInfoFromEdu(duration); }

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
    EntryInfoPtr(const std::shared_ptr<const EntryFrame>& entryFrame, size_t index = 0)
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

    /// @brief Get the measure cmper
    MeasCmper getMeasure() const;

    /// @brief Creates the current StaffComposite for the entry
    /// @param forStaffId Specifies optional staff ID. If supplied, it overrides the entry's staff ID. (Usefule when notes are cross-staffed.)
    std::shared_ptr<others::StaffComposite> createCurrentStaff(const std::optional<InstCmper>& forStaffId = std::nullopt) const;

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

private:
    bool canBeBeamed() const;

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
class EntryFrame : public Base, public std::enable_shared_from_this<EntryFrame>
{
public:
    /** @brief Constructor function
     *
     * @param gfhold The @ref details::GFrameHold instance creating this EntryFrame
     * @param staff The Cmper for the @ref others::Staff of the entry
     * @param measure The Cmper for the @ref others::Measure of the entry
     * @param layerIndex The @ref LayerIndex (0..3) of the entry
     * @param forWrittenPitch If true, the key and clef for each entry are calculated for written pitch rather than concert pitch.
    */
    explicit EntryFrame(const details::GFrameHold& gfhold, InstCmper staff, MeasCmper measure, LayerIndex layerIndex, bool forWrittenPitch);

    /// @brief class to track tuplets in the frame
    struct TupletInfo
    {
        std::shared_ptr<const details::TupletDef> tuplet;   ///< the tuplet
        size_t startIndex;                                  ///< the index of the first entry in the tuplet
        size_t endIndex;                                    ///< the index of the last entry in the tuplet
        util::Fraction startDura;                           ///< the actual duration where the tuplet starts
        util::Fraction endDura;                             ///< the actual duration where the tuplet ends

        /// @brief Constructor
        TupletInfo(const std::shared_ptr<const details::TupletDef>& tup, size_t index, util::Fraction start)
            : tuplet(tup), startIndex(index), endIndex(std::numeric_limits<size_t>::max()),
                startDura(start), endDura(-1)
        {}
    };

    /** @brief A list of the tuplets in the frame and their calculated starting and ending information.
     *
     * @note Tuplets that start on grace notes are omitted from this list. Finale does not display them, and
     * it is not possible to calculate their endpoints correctly in the general case. (Which is probably why
     * Finale does not display them.)
    */
    std::vector<TupletInfo> tupletInfo;
    std::shared_ptr<KeySignature> keySignature; ///< this can be different than the measure key sig if the staff has independent key signatures

    /// @brief Get the staff for the entry
    InstCmper getStaff() const { return m_staff; }

    /// @brief Get the measure for the entry
    MeasCmper getMeasure() const { return m_measure; }

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const { return m_layerIndex; }

    /// @brief Returns if this entry frame was created for written pitch.
    /// @return True if for written pitch, false if for sounding pitch (i.e., concert pitch)
    bool isForWrittenPitch() const { return m_forWrittenPitch; }

    /// @brief Get the entry list.
    const std::vector<std::shared_ptr<const EntryInfo>>& getEntries() const
    { return m_entries; }

    /// @brief Returns the first entry in the specified v1/v2 or null if none.
    ///
    /// @param voice 1 or 2
    EntryInfoPtr getFirstInVoice(int voice) const;

    /// @brief Add an entry to the list.
    void addEntry(const std::shared_ptr<const EntryInfo>& entry)
    { m_entries.emplace_back(entry); }

    /// @brief Gets the entry frame for the next measure with the same staff and layer.
    /// @return Frame or nullpter if the next measure has no matching frame.
    std::shared_ptr<const EntryFrame> getNext() const;

    /// @brief Gets the entry frame for the previous measure with the same staff and layer.
    /// @return Frame or nullpter if the previous measure has no matching frame,
    std::shared_ptr<const EntryFrame> getPrevious() const;

private:
    InstCmper m_staff;
    MeasCmper m_measure;
    LayerIndex m_layerIndex;
    bool m_forWrittenPitch;

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
    friend details::GFrameHold;
#endif

public:
    util::Fraction elapsedDuration{};   ///< the elapsed duration within the measure where this entry occurs (in fractions of a whole note)
    util::Fraction actualDuration{};    ///< the actual duration of entry (in fractions of a whole note), taking into account tuplets and grace notes
    bool v2Launch{};                    ///< indicates if this entry (which is voice1) launches a voice2 sequence
    unsigned graceIndex{};              ///< the Finale grace note index, counting from 1 starting from the leftmost grace note counting rightward.
                                        ///< the main note has a grace index of zero.
    ClefIndex clefIndex{};              ///< the clef index in effect for the entry.

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
    /// @param entryInfo 
    /// @param noteIndex 
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
     * @brief Calculates the note name, octave number, actual alteration, and staff position.
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration in EDO divisions (normally semitones), relative to natural
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<Note::NoteName, int, int, int> calcNoteProperties() const;

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