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

#include <vector>
#include <memory>

#include "Fundamentals.h"

namespace musx {
namespace dom {

/// @brief Defines the type of a musx instance stored in a pool
/// @tparam T The musx type
template <typename T>
using MusxInstance = std::shared_ptr<T>;

/// @brief Defines a weak ptr to the type of a musx instance stored in a pool
/// @tparam T The musx type
template <typename T>
using MusxInstanceWeak = std::weak_ptr<T>;

class Document;

namespace others {
class InstrumentUsed;
class Staff;
}

/**
 * @class MusxInstanceListBase
 * @brief A container of pooled shared object instances from an ObjectPool.
 *
 * This class wraps a std::vector of std::shared_ptr<T> and optionally adds
 * type-specific helper methods via @ref MusxInstanceListExtension.
 *
 * @tparam T The object type stored in the list (e.g., InstrumentUsed, SmartShape, etc.).
 */
template <typename T>
class MusxInstanceListBase : public std::vector<MusxInstance<T>>
{
    using VectorType = std::vector<MusxInstance<T>>;

public:
    /// @brief Default constructor.
    explicit MusxInstanceListBase(const std::weak_ptr<Document>& document, Cmper partId)
        : m_document(document), m_partId(partId) {}

    /// @brief Gets the part id that was used to create this list
    Cmper getRequestedPartId() const { return m_partId; }

    /// @brief Gets the document that was used to create this list
    std::shared_ptr<Document> getDocument() const
    {
        auto document = m_document.lock();
        MUSX_ASSERT_IF(!document) {
            throw std::logic_error("Document pointer is no longer valid.");
        }
        return document;
    }

private:
    std::weak_ptr<Document> m_document;
    Cmper m_partId;
};

/**
 * @class MusxInstanceList
 * @brief Provides optional per-type extension methods for MusxInstanceList.
 *
 * This struct is specialized for individual types (e.g., InstrumentUsed) to add
 * type-specific methods. The default template is empty.
 *
 * @tparam T The object type.
 */
template <typename T>
class MusxInstanceList : public MusxInstanceListBase<T>
{
public:
    using MusxInstanceListBase<T>::MusxInstanceListBase;

    // No helpers by default
};

/**
 * @class MusxInstanceList<others::InstrumentUsed>
 * @brief Specialization for @ref others::InstrumentUsed that adds methods for processing the array as a whole.
 */
template<>
class MusxInstanceList<others::InstrumentUsed> : public MusxInstanceListBase<others::InstrumentUsed>
{
public:
    using MusxInstanceListBase<others::InstrumentUsed>::MusxInstanceListBase;

    /// @brief Returns the @ref Staff instance (without any staff styles applied) at a specified index of iuArray or nullptr if not found
    /// @param index The 0-based index to find.
    std::shared_ptr<others::Staff> getStaffInstanceAtIndex(Cmper index) const;

    /// @brief Returns the 0-based index of the InstCmper or std::nullopt if not found.
    /// @param staffId The @ref Staff cmper value to find.
    std::optional<size_t> getIndexForStaff(InstCmper staffId) const;
};

} // namespace dom
} // namespace musx