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

namespace musx {
namespace dom {

namespace details {
class TupletDef;
} // namespace details;

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
    bool showAcci{};    ///< True if the note has an accidental. (Dynamically changed by Finale unless `freezeAcci` is set.)
    bool freezeAcci{};  ///< True if the accidental should be forced on or off (based on `showAcci`.)

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

    /**
     * @brief Calculates the note name, octave number, actual alteration, and staff position.
     * @param key The key signature in effect.
     * @param clefIndex The index of the clef in effect.
     * @return A tuple containing:
     *         - NoteName: The note name (C, D, E, F, G, A, B)
     *         - int: The octave number (where 4 is the middle C octave)
     *         - int: The actual alteration (in semitones, relative to natural)
     *         - int: The staff position of the note relative to the staff reference line. (For 5-line staves this is the top line.)
     */
    std::tuple<NoteName, int, int, int> calcNoteProperties(const std::shared_ptr<KeySignature>& key, ClefIndex clefIndex) const;


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
    bool beam{};             ///< Signifies the start of a beam or singleton entry. (That is, any beam breaks at this entry.)
    bool freezeStem{};       ///< Freeze stem flag (#upStem gives the direction.)
    bool upStem{};           ///< Whether a stem is up or down. (Only reliable when #freezeStem is true.)
    bool stemDetail{};       ///< Indicates there are stem modification.
    bool sorted{};           ///< Sorted flag.
    bool lyricDetail{};      ///< Indicates there is a lyric assignment on the entry.
    bool performanceData{};  ///< Indicates there is performance data on the entry.
    bool smartShapeDetail{}; ///< Indicates this entry has a smart shape assignment.
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

    void integrityCheck() override
    {
        this->Base::integrityCheck();
        if (numNotes != notes.size()) {
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

/**
 * @class EntryFrame
 * @brief Returns a vector of @ref EntryInfo instances for a given frame, along with computed information.
 * @todo Possibly compute current key
 */
class EntryFrame
{
public:
    /** @brief Constructor function
     *
     * @param staff The Cmper for the @ref others::Staff of the entry
     * @param measure The Cmper for the @ref others::Measure of the entry
     * @param layerIndex The @ref LayerIndex (0..3) of the entry
    */
    explicit EntryFrame(InstCmper staff, MeasCmper measure, LayerIndex layerIndex)
        : m_staff(staff), m_measure(measure), m_layerIndex(layerIndex)
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
                                                ///< @todo This must be adjusted based on concert or transposed pitch.

    /// @brief Get the staff for the entry
    InstCmper getStaff() const { return m_staff; }

    /// @brief Get the measure for the entry
    MeasCmper getMeasure() const { return m_measure; }

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const { return m_layerIndex; }

    /// @brief Get the entry list.
    const std::vector<std::shared_ptr<const EntryInfo>>& getEntries() const
    { return m_entries; }

    /// @brief Add an entry to the list.
    void addEntry(const std::shared_ptr<const EntryInfo>& entry)
    { m_entries.emplace_back(entry); }

private:
    InstCmper m_staff;
    MeasCmper m_measure;
    LayerIndex m_layerIndex;

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
 * @todo compute current clef index.
 */
class EntryInfo
{
    /** @brief Constructor function
     *
     * @param layerIndex The @ref LayerIndex (0..3) of the entry
     * @param entry The entry.
    */
    explicit EntryInfo(const std::shared_ptr<const Entry>& entry, const std::weak_ptr<const EntryFrame>& entryFrame, size_t index)
        : m_entry(entry), m_entryFrame(entryFrame), indexInFrame(index) {}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
    friend details::GFrameHold;
#endif

public:
    size_t indexInFrame{};              ///< the index of this item in the frame.
    util::Fraction elapsedDuration{};   ///< the elapsed duration within the measure where this entry occurs (in fractions of a whole note)
    util::Fraction actualDuration{};    ///< the actual duration of entry (in fractions of a whole note), taking into account tuplets and grace notes
    bool v2Launch{};                    ///< indicates if this entry (which is voice1) launches a voice2 sequence
    unsigned graceIndex{};              ///< the Finale grace note index, counting from 1 starting from the leftmost grace note counting rightward.
                                        ///< the main note has a grace index of zero.
    ClefIndex clefIndex{};              ///< the clef index in effect for the entry.
                                        ///< @todo This must be adjusted based on concert or transposed pitch.

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const { return getFrame()->getLayerIndex(); }

    /// @brief Get the staff cmper
    InstCmper getStaff() const { return getFrame()->getStaff(); }

    /// @brief Get the measure cmper
    MeasCmper getMeasure() const { return getFrame()->getMeasure(); }

    /// @brief Get the key signature of the entry
    auto getKeySignature() const { return getFrame()->keySignature; }

    /// @brief Caclulates the grace index counting leftward (used by other standards such as MNX)
    unsigned calcReverseGraceIndex() const;

    /// @brief Returns the next higher tuplet index that this entry starts, or std::nullopt if none
    std::optional<size_t> calcNextTupletIndex(std::optional<size_t> currentIndex) const;

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

    /// @brief Get the next entry in the frame
    std::shared_ptr<const EntryInfo> getNext() const
    {
        size_t nextIndex = indexInFrame + 1;
        auto frame = getFrame();
        if (nextIndex < frame->getEntries().size()) {
            return frame->getEntries()[nextIndex];
        }
        return nullptr;
    }

    /// @brief Get the next entry in the frame in the same voice
    ///
    /// For V2, it stops at the current V2 launch sequence.
    std::shared_ptr<const EntryInfo> getNextSameV() const
    {
        auto next = getNext();
        if (getEntry()->voice2) {
            if (next && next->getEntry()->voice2) {
                return next;
            }
            return nullptr;
        }
        if (v2Launch) {
            while (next && next->getEntry()->voice2) {
                next = next->getNext();
            }
        }
        return next;
    }

    /// @brief Get the previous entry in the frame
    std::shared_ptr<const EntryInfo> getPrevious() const
    {
        if (indexInFrame > 0) {
            return getFrame()->getEntries()[indexInFrame - 1];
        }
        return nullptr;
    }

    /// @brief Get the previous entry in the frame in the same voice
    ///
    /// For V2, it stops at the current V2 launch sequence.
    std::shared_ptr<const EntryInfo> getPreviousSameV() const
    {
        auto prev = getPrevious();
        if (getEntry()->voice2) {
            if (prev && prev->getEntry()->voice2) {
                return prev;
            }
            return nullptr;
        }
        while (prev && prev->getEntry()->voice2) {
            prev = prev->getPrevious();
        }
        return prev;
    }

    /// @brief Get the EntryFrame for this EntryInfo
    std::shared_ptr<const EntryFrame> getFrame() const
    {
        auto locked = m_entryFrame.lock();
        assert(locked); // program bug if this pointer goes out of scope.
        if (!locked) {
            throw std::logic_error("Entry frame is no longer valid.");
        }
        return locked;
    }

private:
    std::weak_ptr<const Entry> m_entry;
    std::weak_ptr<const EntryFrame> m_entryFrame;
};

} // namespace dom
} // namespace entries