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
#include <unordered_map>
#include <vector>

#include "Header.h"
#include "ObjectPool.h"

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

/// @class InstrumentInfo
/// @brief Represents information about each instrument in the document. This is calculated from the staves,
/// staff groups, and multistaff instrument groups.
struct InstrumentInfo
{
    std::unordered_map<StaffCmper, size_t> staves;   ///< List of each staffId with its sequence index from top to bottom.
    Cmper staffGroupId{};                           ///< The @ref details::StaffGroup that visually represents the instrument. (May be zero.)
    Cmper multistaffGroupId{};                      ///< The @ref others::MultiStaffInstrumentGroup that defines the instrument. (May be zero.)

    /// @brief Returns the staffIds in sequence as they appear in Scroll View in the score.
    std::vector<StaffCmper> getSequentialStaves() const;
};
using InstrumentMap = std::unordered_map<StaffCmper, InstrumentInfo>; ///< A list of instruments, which may be single- or multi-staff

/**
 * @brief Represents a document object that encapsulates the entire EnigmaXML structure.
 */
class Document
{
public:
    /**  @brief Retrieves the header */
    HeaderPtr& getHeader() { return m_header; }
    /**  @brief Retrieves the const header */
    const HeaderPtr& getHeader() const { return m_header; }
    
    /** @brief Retrieves the options pool */
    OptionsPoolPtr& getOptions() { return m_options; }
    /** @brief Retrieves the const options pool */
    const OptionsPoolPtr& getOptions() const { return m_options; }
    
    /** @brief Retrieves the others pool */
    OthersPoolPtr& getOthers() { return m_others; }
    /** @brief Retrieves the const others pool */
    const OthersPoolPtr& getOthers() const { return m_others; }
    
    /** @brief Retrieves the details pool */
    DetailsPoolPtr& getDetails() { return m_details; }
    /** @brief Retrieves the const others pool */
    const DetailsPoolPtr& getDetails() const { return m_details; }
    
    /** @brief Retrieves the entry pool */
    EntryPoolPtr& getEntries() { return m_entries; }
    /** @brief Retrieves the entry others pool */
    const EntryPoolPtr& getEntries() const { return m_entries; }

    /** @brief Retrieves the texts pool */
    TextsPoolPtr& getTexts() { return m_texts; }
    /** @brief Retrieves the const texts pool */
    const TextsPoolPtr& getTexts() const { return m_texts; }

    /// @brief Returns the Scroll View Cmper for the given @p partId.
    /// @param partId The linked part to check.
    [[nodiscard]]
    constexpr Cmper calcScrollViewCmper([[maybe_unused]]Cmper partId) const noexcept
    {
        // as of now, there is no calculation, but keep that assumption encapsulated.
        return BASE_SYSTEM_ID;        
    }

    /// @brief Returns the scroll view staves for the given @p partId.
    /// @param partId The linked part to check.
    MusxInstanceList<others::StaffUsed> getScrollViewStaves(Cmper partId) const;

    /// @brief Searches pages to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    MusxInstance<others::Page> calculatePageFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Searches systems to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    MusxInstance<others::StaffSystem> calculateSystemFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Returns the maximum number of blank pages in any part. This is calculated by #factory::DocumentFactory::create.
    int getMaxBlankPages() const { return m_maxBlankPages; }

    /// @brief Returns the instrument map for this document. It is computed by the factory.
    const InstrumentMap& getInstruments() const { return m_instruments; }

    /// @brief Get the instrument info for the given staffId
    /// @param staffId The staffId to find.
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
    InstrumentMap createInstrumentMap(Cmper forPartId) const;

    /// @brief Calculate if the current score/part has staves that differ from system to system.
    /// @param forPartId The linked score or part ID to check.
    bool calcHasVaryingSystemStaves(Cmper forPartId) const;

private:
    /// @brief Constructs a `Document`
    explicit Document() = default;
    
    HeaderPtr m_header;         ///< The <header>
    OptionsPoolPtr m_options;   ///< The <options> pool
    OthersPoolPtr m_others;     ///< The <others> pool
    DetailsPoolPtr m_details;   ///< The <details> pool
    EntryPoolPtr m_entries;     ///< The <entries> pool
    TextsPoolPtr m_texts;       ///< The <texts> pool

    int m_maxBlankPages{};      ///< The maximum number of leading blank pages in any part.

    InstrumentMap m_instruments; ///< List of instruments in the document, indexed by the top staff in each instrument in Scroll View of the score.
                                ///< This computed by the factory.

    // Grant the factory class access to the private constructor
    friend class musx::factory::DocumentFactory;
};

} // namespace dom
} // namespace musx
