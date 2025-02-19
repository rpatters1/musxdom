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

#include "musx/util/Fraction.h"
#include "BaseClasses.h"
#include "CommonClasses.h"
 // do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {

int calcAugmentationDotsFromEdu(Edu duration);      ///< Calculates the number of dots from an @ref Edu value.
NoteType calcNoteTypeFromEdu(Edu duration);         ///< Calculates the @ref NoteType from an @ref Edu value.

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

    int harmLev{};      ///< Diatonic displacement relative to middle C or to the tonic in the middle C octave (if the key signature tonic is not C).
    int harmAlt{};      ///< Chromatic alteration relative to the key signature. Never has a magnitude greater than +/-7.
    bool isValid{};     ///< Should always be true but otherwise appears to be used internally by Finale.
    bool showAcci{};    ///< True if the note has an accidental. (Dynamically changed by Finale unless `freezeAcci` is set.)
    bool freezeAcci{};  ///< True if the accidental should be forced on or off (based on `showAcci`.)

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

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
     * @brief Calculates the NoteType based on the most significant bit of the duration field.
     *
     * @return NoteType corresponding to the most significant bit of the duration.
     * @throws std::invalid_argument if the duration is out of valid range (> 1 and < 0x10000).
     */
    NoteType calcNoteType() const { return calcNoteTypeFromEdu(duration); }

    /**
     * @brief Calculates the duration as a @ref util::Fraction of a whole note
     */
    util::Fraction calcFraction() const { return util::Fraction::fromEdu(duration); }

    /**
     * @brief Calculates the number of augmentation dots in the duration.
     *
     * @return The number of augmentation dots.
     */
    int calcAugmentationDots() const { return calcAugmentationDotsFromEdu(duration); }

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
    explicit EntryInfo(const std::shared_ptr<const Entry>& entry, const std::shared_ptr<EntryFrame>& entryFrame, size_t indexInFrame)
        : m_entry(entry), m_entryFrame(entryFrame), m_indexInFrame(indexInFrame) {}

    friend details::GFrameHold;

public:
    util::Fraction elapsedDuration{};   ///< the elapsed duration within the measure where this entry occurs (in fractions of a whole note)
    util::Fraction actualDuration{};    ///< the actual duration of entry (in fractions of a whole note), taking into account tuplets and grace notes
    bool v2Launch{};                    ///< indicates if this entry (which is voice1) launches a voice2 sequence
    unsigned graceIndex{};              ///< the Finale grace note index, counting from 1 starting from the leftmost grace note counting rightward.
                                        ///< the main note has a grace index of zero.

    /// @brief Get the layer index (0..3) of the entry
    LayerIndex getLayerIndex() const { return m_entryFrame->getLayerIndex(); }


    /// @brief Caclulates the grace index counting leftward (used by other standards such as MNX)
    unsigned calcReverseGraceIndex() const;

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
        size_t nextIndex = m_indexInFrame + 1;
        if (nextIndex < m_entryFrame->getEntries().size()) {
            return m_entryFrame->getEntries()[nextIndex];
        }
        return nullptr;
    }

    /// @brief Get the previous entry in the frame
    std::shared_ptr<const EntryInfo> getPrevious() const
    {
        if (m_indexInFrame > 0) {
            return m_entryFrame->getEntries()[m_indexInFrame - 1];
        }
        return nullptr;
    }

private:
    std::weak_ptr<const Entry> m_entry;
    size_t m_indexInFrame;
    std::shared_ptr<EntryFrame> m_entryFrame;
};

} // namespace dom
} // namespace entries