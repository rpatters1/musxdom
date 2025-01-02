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

#include "Header.h"
#include "ObjectPool.h"
#
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

/**
 * @brief Represents a document object that encapsulates the entire EnigmaXML structure.
 * 
 * @todo Add other members besides the header.
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

private:
    /**
     * @brief Constructs a `Document`
     */
     explicit Document() = default;

     HeaderPtr m_header;        ///< The <header>
     OptionsPoolPtr m_options;  ///< The <options> pool
     OthersPoolPtr m_others;    ///< The <others> pool
     DetailsPoolPtr m_details;  ///< The <details> pool
     EntryPoolPtr m_entries;    ///< The <entries> pool
     TextsPoolPtr m_texts;      ///< The <texts> pool

    // Grant the factory function access to the private constructor
    friend class musx::factory::DocumentFactory;
};

} // namespace dom
} // namespace musx
