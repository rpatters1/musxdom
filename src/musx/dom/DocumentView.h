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
 * @class DocumentViewBase
 * @brief Base class for calculated document views.
 *
 * This is not a Finale data class and is not populated directly from EnigmaXML. It provides document
 * and requested-part context for normalized, interchange-oriented views calculated from the DOM.
 */
class DocumentViewBase
{
public:
    /// @brief Constructs an empty document view for the score.
    DocumentViewBase() = default;

    /// @brief Constructs a document view.
    /// @param document The document this view describes.
    /// @param requestedPartId The score or linked part ID represented by this view.
    DocumentViewBase(const DocumentWeakPtr& document, Cmper requestedPartId = SCORE_PARTID)
        : m_document(document), m_requestedPartId(requestedPartId) {}

    /// @brief Gets a pointer to the document this view describes.
    /// @return A pointer to the document instance.
    DocumentPtr getDocument() const
    {
        auto document = m_document.lock();
        MUSX_ASSERT_IF(!document) {
            throw std::logic_error("Document pointer is no longer valid.");
        }
        return document;
    }

    /// @brief Returns the score or linked part ID represented by this view.
    Cmper getRequestedPartId() const { return m_requestedPartId; }

private:
    DocumentWeakPtr m_document;
    Cmper m_requestedPartId{SCORE_PARTID};
};

} // namespace dom
} // namespace musx
