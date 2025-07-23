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
#include <unordered_set>

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

struct InstrumentInfo
{
    std::unordered_set<InstCmper> staves;
    Cmper staffGroupId{};
};
using InstrumentMap = std::unordered_map<InstCmper, InstrumentInfo>; ///< A list of instruments, which may be single- or multi-staff

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

    /// @brief Searches pages to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    std::shared_ptr<others::Page> calculatePageFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Searches systems to find the page that contains the measure.
    /// @param partId the linked part to search
    /// @param measureId the measure to find
    std::shared_ptr<others::StaffSystem> calculateSystemFromMeasure(Cmper partId, MeasCmper measureId) const;

    /// @brief Returns the maximum number of blank pages in any part. This is calculated by #factory::DocumentFactory::create.
    int getMaxBlankPages() const { return m_maxBlankPages; }

    /// @brief Returns the instrument map for this document. It is computed by the factory.
    const InstrumentMap& getInstruments() const { return m_instruments; }

private:
    /// @brief Constructs a `Document`
    explicit Document() = default;

    void createInstrumentMap();
    
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
