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

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {

/**
 * @class Note
 * @brief Represents a single note element in an entry.
 *
 * This class is identified by the XML node name "note".
 */
class Note : public Base 
{
private:
    NoteNumber m_noteId{}; ///< Unique identifier for the note.

public:
    /** @brief Constructor function */
    explicit Note(const DocumentWeakPtr& document, NoteNumber noteId)
        : Base(document, 0, ShareMode::All), m_noteId(noteId) {}

    int harmLev{};    ///< Diatonic displacement relative to tonic or to C (for keyless scores)
    int harmAlt{};    ///< Chromatic alterations relative to key signature.
    bool isValid{};   ///< Should always be true.
    bool showAcci{};  ///< Should the accidental be shown?

    /// @brief Gets the note id for this note. This value does not change, even if the notes
    /// in a chord are rearranged (which affects the order of #Entry::notes.)
    NoteNumber getNoteId() const { return m_noteId; }

    bool requireAllFields() const override { return false; }

    static const xml::XmlElementArray<Note> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class Entry
 * @brief Represents an entry containing metadata and notes.
 *
 * This class is identified by the XML node name "entry".
 */
class Entry : public Base {
private:
    EntryNumber m_entnum;   ///< Entry number.
    EntryNumber m_prev;     ///< Previous entry number in the list. (0 if none)
    EntryNumber m_next;     ///< Next entry number in the list. (0 if none)

public:
    /** @brief Constructor function
     *
     * The partId and shareMode values should always be 0 and ShareMode::All, but they are required by the factory function.
    */
    explicit Entry(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, EntryNumber prev, EntryNumber next)
        : Base(document, partId, shareMode), m_entnum(entnum), m_prev(prev), m_next(next) {}

    Edu duration{};          ///< Duration of the entry, not taking into account tuplets. (xml node is `<dura>`).
    int numNotes{};          ///< Number of notes in the entry. There is an error if this is not the same as notes.size().
    bool isValid{};          ///< Should always be true.
    bool isNote{};           ///< If this value is false, the entry is a rest.
    bool voice2{};           ///< This is a V2 note. (xml node `<v2>`)
    bool articDetail{};      ///< Indicates there is an articulation on the entry
    bool beam{};             ///< Signifies the start of a beam or singleton entry. (That is, any beam breaks at this entry.)
    bool stemDetail{};       ///< Indicates there are stem modification.
    bool sorted{};           ///< Sorted flag.
    bool lyricDetail{};      ///< Indicates there is a lyric assignment on the entry.
    bool performanceData{};  ///< Indicates there is performance data on the entry.
    bool smartShapeDetail{}; ///< Indicates there is a 
    bool freezeBeam{};       ///< Freeze beam flag (Derived from the presence of `<freezeBeam>` node.)

    /** @brief Collection of notes that comprise the entry. These are in order from lowest to highest. */
    std::vector<std::shared_ptr<Note>> notes;

    /// @brief Gets the entry number for this entry
    EntryNumber getEntryNumber() const { return m_entnum; }

    /// @brief Gets the next entry in this frame or nullptr if none
    std::shared_ptr<Entry> getNext() const;
    /// @brief Gets the previous entry in this frame or nullptr if none
    std::shared_ptr<Entry> getPrevious() const;

    bool requireAllFields() const override { return false; }

    constexpr static std::string_view XmlNodeName = "entry"; ///< The XML node name for this type.
    static const xml::XmlElementArray<Entry> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

} // namespace dom
} // namespace entries