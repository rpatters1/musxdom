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
#include <functional>
#include <optional>
#include <type_traits>
#include <utility>

#include "Fundamentals.h"

namespace musx {
namespace dom {

/// @brief Defines the type of a musx instance stored in a pool
/// @tparam T The musx type
template <typename T>
using MusxInstance = std::shared_ptr<const T>;

/// @brief Defines a weak ptr to the type of a musx instance stored in a pool
/// @tparam T The musx type
template <typename T>
using MusxInstanceWeak = std::weak_ptr<const T>;

class Document;
class MusicRange;
class EntryInfoPtr;

namespace others {
class StaffUsed;
class Staff;
class StaffComposite;
}

/**
 * @class MusxInstanceListBase
 * @brief A container of pooled shared object instances from an ObjectPool.
 *
 * This class wraps a std::vector of @ref MusxInstance<T>.
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

    /// @brief Returns the @ref others::Staff instance (without any staff styles applied) at a specified index of iuArray or nullptr if not found
    /// @param index The 0-based index to find.
    MusxInstance<others::Staff> getStaffInstanceAtIndex(size_t index) const;

    /// @brief Returns the current @ref others::StaffComposite instance at a specified index of iuArray or nullptr if not found
    /// @param index The 0-based index to find.
    /// @param measureId The measure of the location to get.
    /// @param eduPosition The edu position of the location to get.
    MusxInstance<others::StaffComposite> getStaffInstanceAtIndex(size_t index, MeasCmper measureId, Edu eduPosition = 0) const;

    /// @brief Returns the 0-based index of the StaffCmper or std::nullopt if not found.
    /// @param staffId The @ref others::Staff cmper value to find.
    std::optional<size_t> getIndexForStaff(StaffCmper staffId) const;

    /// @brief Returns the top staff id or zero if none
    StaffCmper getTopStaffId() const;

    /// @brief Returns the bottom staff id or zero if none
    StaffCmper getBottomStaffId() const;

    /// @brief Iterates all the entries that start within the input @ref MusicRange. It iterates by staff and then measure.
    /// If an entry starts before the range, it is excluded even if its duration takes it within the range. Conversely, an
    /// entry is included if it starts within the range, even if its duration takes it outside the range.
    /// If @p startIndex is less than @p endIndex, the staves are iterated upwards. Otherwise they are interated downwards.
    /// @param startIndex The first staff index to iterate.
    /// @param endIndex The last staff index to inerate.
    /// @param range The music range to iterate. It should be expressed in *global* position values.
    /// @param iterator The iterator function. Return `false` from this function to stop iterating.
    /// @return True if all items were iterated. False if the @p iterator returned false and exited early.
    bool iterateEntries(size_t startIndex, size_t endIndex, const MusicRange& range, std::function<bool(const EntryInfoPtr&)> iterator) const;
};

///
/// @class DeferredReference
/// @brief Wraps a reference to an existing object or owns a temporary value if needed.
///
/// This utility allows a function parameter to accept either a non-owning reference
/// (bound implicitly from a `const T&` or `const T*`) or an owned value created
/// later via `emplace()`. It avoids copies and enables lazy materialization.
///
/// Example usage:
/// @code
/// int func(DeferredReference<const Foo> foo = {})
/// {
///     if (!foo)
///         foo.emplace(createFoo()); // owns the result
///     return foo->bar();
/// }
///
/// Foo f;
/// func(f);                        // non-owning, no copy
/// func();                         // omitted (empty), function will emplace()
/// func(DeferredReference{&f});    // explicit pointer form
/// @endcode
///
/// @tparam T The public reference type. May be const-qualified (e.g., `const Foo`).
///            Ownership is always stored as a mutable `std::remove_const_t<T>` internally.
///
template<class T>
class DeferredReference {
    static_assert(!std::is_reference_v<T>, "T must not be a reference");
    using OwnedT = std::remove_const_t<T>; ///< Mutable type for owned storage

public:
    /// @brief Constructs an empty DeferredReference with no bound reference.
    DeferredReference() noexcept = default;

    /// @brief Constructs a non-owning DeferredReference bound to an existing object.
    /// @param ref The object to bind. No copy is made.
    DeferredReference(const T& ref) noexcept : m_ref(&ref) {}

    /// @brief Constructs a non-owning DeferredReference bound to a pointer.
    /// @param ptr Pointer to the object. No copy is made.
    explicit DeferredReference(const T* ptr) noexcept : m_ref(ptr) {}

    /// @brief Moves a value into owned storage and binds to it.
    ///
    /// This replaces any previous binding (owned or non-owned) and ensures
    /// that the DeferredReference has a valid internal reference thereafter.
    ///
    /// @param value The object to move into internal storage.
    /// @return A const reference to the stored object.
    const T& emplace(OwnedT&& value) noexcept(std::is_nothrow_move_constructible_v<OwnedT>) {
        m_owned.emplace(std::move(value));
        m_ref = std::addressof(*m_owned);
        return *m_ref;
    }

    /// @brief Checks whether this DeferredReference currently references a valid object.
    explicit operator bool() const noexcept { return m_ref != nullptr; }

    /// @brief Gets a const reference to the referenced or owned object.
    /// @warning Undefined behavior if called when `!(*this)`.
    const T& get() const noexcept { return *m_ref; }

    /// @brief Provides pointer-like access to the referenced or owned object.
    const T* operator->() const noexcept { return m_ref; }

    /// @brief Dereferences to the referenced or owned object.
    const T& operator*() const noexcept { return *m_ref; }

private:
    std::optional<OwnedT> m_owned{}; ///< Optional owned storage (constructed on demand)
    const T* m_ref = nullptr;        ///< Pointer to the referenced or owned object
};

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
///
/// @name CTAD guides
/// @brief Allow deduction of `T` when constructed from a reference or pointer.
/// @details
/// These guides enable `DeferredReference x{object};` or `DeferredReference x{&object};`
/// to automatically deduce the template parameter `T`.
/// @{
///
template<class T> DeferredReference(const T&) -> DeferredReference<T>;
template<class T> DeferredReference(const T*) -> DeferredReference<T>;
/// @}
#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace dom
} // namespace musx