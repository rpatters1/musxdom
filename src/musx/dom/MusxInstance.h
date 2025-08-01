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

/**
 * @brief A wrapper for shared instance pointers.
 * 
 * @tparam T The type of the musx object.
 */
template <typename T>
class MusxInstance {
public:
    using element_type = T;                         ///< The base type that this pointer points to.
    using pointer_type = std::shared_ptr<const T>;  ///< The full pointer type (e.g. shared_ptr<const T>).

    /// @brief Default constructor.
    MusxInstance() = default;

    /// @brief Null constructor.
    explicit MusxInstance(std::nullptr_t)
        : m_ptr(nullptr) {}

    /// @brief Copy constructor accepts T or any subclass of T
    template <typename U,
              typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    MusxInstance(const MusxInstance<U>& other)
        : m_ptr(other.ptr()) {}

    /// @brief Move constructor from pointer.
    explicit MusxInstance(pointer_type ptr)
        : m_ptr(std::move(ptr)) {}

    /// @brief Constructor from raw pointer (shared_ptr only)
    template <typename U = pointer_type,
            typename = std::enable_if_t<std::is_same_v<U, std::shared_ptr<const T>>>>
    MusxInstance(const element_type* rawPtr)
        : m_ptr(rawPtr) {}

    /// @brief Returns true if the pointer is valid (non-null or not expired).
    explicit operator bool() const {
        return static_cast<bool>(m_ptr);
    }

    /// @brief Gets the shared_ptr.
    pointer_type ptr() const {
        return m_ptr;
    }

    /// @brief Gets the raw pointer.
    const T* get() const {
        return m_ptr.get();
    }

    /// @brief Dereference operator.
    const T* operator->() const {
        return get();
    }

    /// @brief Dereference operator.
    const T& operator*() const {
        return *get();
    }

    /// @brief Assignment operator accepts T or any subclass of T
    template <typename U,
              typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    MusxInstance& operator=(const MusxInstance<U>& other) {
        m_ptr = other.ptr();  // use public accessor
        return *this;
    }

private:
    pointer_type m_ptr;
};

/// @brief Weak musx instance pointer (non-owning reference)
/// @tparam T The musx object type
template <typename T>
using MusxInstanceWeak = std::weak_ptr<const T>;

class Document;

namespace others {
class StaffUsed;
class Staff;
}

/**
 * @class MusxInstanceListBase
 * @brief A container of pooled shared object instances from an ObjectPool.
 *
 * This class wraps a std::vector of std::shared_ptr<T> and optionally adds
 * type-specific helper methods via @ref MusxInstanceListExtension.
 *
 * @tparam T The object type stored in the list (e.g., StaffUsed, SmartShape, etc.).
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
 * This struct is specialized for individual types (e.g., StaffUsed) to add
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
 * @class MusxInstanceList<others::StaffUsed>
 * @brief Specialization for @ref others::StaffUsed that adds methods for processing the array as a whole.
 */
template<>
class MusxInstanceList<others::StaffUsed> : public MusxInstanceListBase<others::StaffUsed>
{
public:
    using MusxInstanceListBase<others::StaffUsed>::MusxInstanceListBase;

    /// @brief Returns the @ref Staff instance (without any staff styles applied) at a specified index of iuArray or nullptr if not found
    /// @param index The 0-based index to find.
    MusxInstance<others::Staff> getStaffInstanceAtIndex(Cmper index) const;

    /// @brief Returns the 0-based index of the StaffCmper or std::nullopt if not found.
    /// @param staffId The @ref Staff cmper value to find.
    std::optional<size_t> getIndexForStaff(StaffCmper staffId) const;
};

} // namespace dom
} // namespace musx