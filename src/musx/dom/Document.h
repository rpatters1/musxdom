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

#include <memory>
#include <optional>
#include <unordered_map>
#include <vector>

#include "Header.h"
#include "ObjectPool.h"
#include "MusxInstance.h"

/**
 * @namespace musx
 * @brief object model for musx file (enigmaxml)
 */
namespace musx {

namespace factory {
class DocumentFactory;
} // namespace factory

/**
 * @namespace musx::dom
 * @brief classes representing the musx document
 */
namespace dom {

using namespace header;

struct InstrumentInfo;
using InstrumentMap = std::unordered_map<StaffCmper, InstrumentInfo>; ///< A list of instruments, which may be single- or multi-staff
/// @class InstrumentInfo
/// @brief Represents information about each instrument in the document. This is calculated from the staves,
/// staff groups, and multistaff instrument groups.
struct InstrumentInfo
{
    std::unordered_map<StaffCmper, size_t> staves;  ///< List of each staffId with its sequence index from top to bottom.
    Cmper staffGroupId{};                           ///< The @ref details::StaffGroup that visually represents the instrument. (May be zero.)
    Cmper multistaffGroupId{};                      ///< The @ref others::MultiStaffInstrumentGroup that defines the instrument. (May be zero.)

    /// @brief Returns the staffIds in sequence as they appear in Scroll View in the score.
    std::vector<StaffCmper> getSequentialStaves() const;

    /// @brief Get the instrument info for the given staffId in the given map
    /// @param map The map to search.
    /// @param staffId The staffId to find.
    /// @return The InstrumentInfo for the @p staffId or null if not found.
    static const InstrumentInfo* getInstrumentForStaff(const InstrumentMap& map, StaffCmper staffId);
};

/// @enum PartVoicingPolicy
/// @brief Controls whether Finale-style part voicing is applied when iterating entries via
///        @ref musx::dom::details::GFrameHoldContext.
enum class PartVoicingPolicy
{
    Ignore,     ///< Default. Part voicing is ignored entirely. All layers in a staff are visible to all parts,
                ///< and entries behave as if no part-voicing definitions exist.

    Apply       ///< Apply Finale’s voiced-part interpretation.
                ///< When enabled, entry iteration filters layers according to the active part voicing,
                ///< and individual notes may be included or excluded based on voicing rules.
                ///< Callers that operate at the note level should use
                ///< #NoteInfoPtr::calcIsIncludedInVoicing to test note visibility.
                ///< If all notes in an entry are excluded by voicing, then
                ///< #EntryInfoPtr::calcDisplaysAsRest returns true for that entry.
};

/**
 * @brief Represents a document object that encapsulates the entire EnigmaXML structure.
 */
class Document
{
public:

    /**  @brief Retrieves the header */
    [[nodiscard]]
    HeaderPtr& getHeader() { return m_header; }
    /**  @brief Retrieves the const header */
    [[nodiscard]]
    const HeaderPtr& getHeader() const { return m_header; }

    /** @brief Retrieves the options pool */
    [[nodiscard]]
    OptionsPoolPtr& getOptions() { return m_options; }
    /** @brief Retrieves the const options pool */
    [[nodiscard]]
    const OptionsPoolPtr& getOptions() const { return m_options; }

    /** @brief Retrieves the others pool */
    [[nodiscard]]
    OthersPoolPtr& getOthers() { return m_others; }
    /** @brief Retrieves the const others pool */
    [[nodiscard]]
    const OthersPoolPtr& getOthers() const { return m_others; }

    /** @brief Retrieves the details pool */
    [[nodiscard]]
    DetailsPoolPtr& getDetails() { return m_details; }
    /** @brief Retrieves the const others pool */
    [[nodiscard]]
    const DetailsPoolPtr& getDetails() const { return m_details; }

    /** @brief Retrieves the entry pool */
    [[nodiscard]]
    EntryPoolPtr& getEntries() { return m_entries; }
    /** @brief Retrieves the entry others pool */
    [[nodiscard]]
    const EntryPoolPtr& getEntries() const { return m_entries; }

    /** @brief Retrieves the texts pool */
    [[nodiscard]]
    TextsPoolPtr& getTexts() { return m_texts; }
    /** @brief Retrieves the const texts pool */
    [[nodiscard]]
    const TextsPoolPtr& getTexts() const { return m_texts; }

    /** @brief Retrieves the document's part voicing policy */
    [[nodiscard]]
    PartVoicingPolicy getPartVoicingPolicy() const { return m_partVoicingPolicy; }

    /// @brief Returns the Scroll View Cmper for the given @p partId.
    /// @param partId The linked part to check.
    [[nodiscard]]
    constexpr Cmper calcScrollViewCmper([[maybe_unused]]Cmper partId) const noexcept
    {
        // as of now, there is no calculation, but keep that assumption encapsulated.
        return BASE_SYSTEM_ID;
    }

    /// @brief Returns the Scroll View staves for the given @p partId.
    /// @param partId The linked part to check.
    [[nodiscard]]
    MusxInstanceList<others::StaffUsed> getScrollViewStaves(Cmper partId) const;

    /// @brief Returns the Studio View staves for the given @p partId. Normally this is
    /// only one staff and is invariant between parts.
    /// @param partId The linked part to check.
    [[nodiscard]]
    MusxInstanceList<others::StaffUsed> getStudioViewStaves(Cmper partId) const;

    /// @brief Searches pages to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    [[nodiscard]]
    MusxInstance<others::Page> calcPageFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Searches systems to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    [[nodiscard]]
    MusxInstance<others::StaffSystem> calcSystemFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Returns the maximum number of blank pages in any part. This is calculated by #factory::DocumentFactory::create.
    [[nodiscard]]
    int getMaxBlankPages() const { return m_maxBlankPages; }

    /// @brief Returns the instrument map for this document. It is computed by the factory.
    [[nodiscard]]
    const InstrumentMap& getInstruments() const { return m_instruments; }

    /// @brief Get the instrument info for the given staffId
    /// @param staffId The staffId to find.
    [[nodiscard]]
    const InstrumentInfo& getInstrumentForStaff(StaffCmper staffId) const;

    /**
     * @brief Builds an instrument map for the specified linked part ID.
     *
     * This routine detects instrument groupings in three stages:
     * 1. Defined multi-staff instruments (via multiStaffInstId).
     * 2. Visually bracketed staves with matching instrument UUIDs.
     * 3. Remaining single staves as individual instruments.
     *
     * This is especially important for supporting legacy .musx files
     * created before multi-staff instruments were defined explicitly.
     *
     * @param forPartId The linked part for which to create the map.
     */
    [[nodiscard]]
    InstrumentMap createInstrumentMap(Cmper forPartId) const;

    /// @brief Calculate if the current score/part has staves that differ from system to system.
    /// @param forPartId The linked score or part ID to check.
    [[nodiscard]]
    bool calcHasVaryingSystemStaves(Cmper forPartId) const;

    /// @brief Calcuate a @ref MusicRange instance for the entire document.
    [[nodiscard]]
    MusicRange calcEntireDocument() const;

    /// @brief Calculates the measures from which playback jumped into @p currentMeasure.
    /// @details This function inspects repeat endings and text repeats to detect jumps that
    /// land on the specified measure. The result is the measure immediately preceding the
    /// jump origin, when determinable. It is intended for jump-aware tie continuation logic.
    /// @param partId The linked part to inspect (use #SCORE_PARTID for score).
    /// @param currentMeasure The measure that playback has jumped into.
    /// @return A list of measures immediately preceding jump origins, ordered by preference
    /// (forward jump origins first in ascending order, then backward origins in descending order).
    [[nodiscard]]
    std::vector<MeasCmper> calcJumpFromMeasures(Cmper partId, MeasCmper currentMeasure) const;

    /// @brief Iterate all entries in the document by staff and then measure. This function wraps MusxInstanceList<others::StaffUsed>::iterateEntries.
    /// @param partId The linked part id to iterate. (Use #SCORE_PARTID to iterate the score.)
    /// @param iterator The callback function.
    /// @return True if iteration completed. False if the @p iterator returned false and exited early.
    bool iterateEntries(Cmper partId, std::function<bool(const EntryInfoPtr&)> iterator) const;

private:
    /// @brief Constructs a `Document`
    explicit Document() = default;

    DocumentWeakPtr m_self;     ///< A weak pointer to self, to allow constructing common classes inside this.

    HeaderPtr m_header;         ///< The <header>
    OptionsPoolPtr m_options;   ///< The <options> pool
    OthersPoolPtr m_others;     ///< The <others> pool
    DetailsPoolPtr m_details;   ///< The <details> pool
    EntryPoolPtr m_entries;     ///< The <entries> pool
    TextsPoolPtr m_texts;       ///< The <texts> pool

    int m_maxBlankPages{};      ///< The maximum number of leading blank pages in any part.

    InstrumentMap m_instruments; ///< List of instruments in the document, indexed by the top staff in each instrument in Scroll View of the score.
                                ///< This computed by the factory.

    PartVoicingPolicy m_partVoicingPolicy{};    ///< The part voicing policy in effect for this document.

    // Grant the factory class access to the private constructor
    friend class musx::factory::DocumentFactory;
};

} // namespace dom
} // namespace musx
