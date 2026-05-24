/*
 * Copyright (C) 2026, Robert Patterson
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

#include <cassert>
#include <memory>
#include <stdexcept>

#include "musx/dom/Fundamentals.h"

namespace musx {
namespace dom {

class Document;
/** @brief Shared `Document` pointer */
using DocumentPtr = std::shared_ptr<Document>;
/** @brief Shared weak `Document` pointer */
using DocumentWeakPtr = std::weak_ptr<Document>;

/**
 * @class DocumentElement
 * @brief Base for DOM classes that belong to a Document.
 *
 * This base provides a document context with a part id.
 */
class DocumentElement
{
public:
    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~DocumentElement() noexcept(false) = default;

    /**
     * @brief Gets a reference to the Document.
     *
     * @return A pointer to the Document instance.
     */
    DocumentPtr getDocument() const
    {
        auto document = m_document.lock();
        MUSX_ASSERT_IF(!document) {
            throw std::logic_error("Document pointer is no longer valid.");
        }
        return document;
    }

    /**
     * @brief Gets the part id associated with this instance.
     */
    Cmper getPartId() const { return m_partId; }

protected:
    /**
     * @brief Constructs the document element.
     *
     * @param document A weak pointer to the parent document.
     * @param partId The part id associated with this instance.
     */
    DocumentElement(const DocumentWeakPtr& document, Cmper partId)
        : m_document(document), m_partId(partId) {}

    DocumentElement(const DocumentElement&) = default;        ///< explicit default copy constructor
    DocumentElement(DocumentElement&&) noexcept = default;    ///< explicit default move constructor

    /// @brief no-op copy assignment operator allows subclasses to copy their values.
    DocumentElement& operator=(const DocumentElement&) { return *this; }

    /// @brief no-op move assignment operator allows subclasses to move their values.
    DocumentElement& operator=(DocumentElement&&) noexcept { return *this; }

private:
    const DocumentWeakPtr m_document{};
    const Cmper m_partId{};
};

} // namespace dom
} // namespace musx
