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

namespace musx {
namespace dom {

namespace others {
class InstrumentUsed;
class Staff;
}

/**
 * @class ObjectListBase
 * @brief A container of pooled shared object instances from an ObjectPool.
 *
 * This class wraps a std::vector of std::shared_ptr<T> and optionally adds
 * type-specific helper methods via @ref ObjectListExtension.
 *
 * @tparam T The object type stored in the list (e.g., InstrumentUsed, SmartShape, etc.).
 */
template <typename T>
class ObjectListBase : public std::vector<std::shared_ptr<T>>
{
    using VectorType = std::vector<std::shared_ptr<T>>;

public:
    /// @brief Default constructor.
    ObjectListBase() = default;

    /// @brief Constructs from a vector of shared pointers.
    /// @param v The source vector to copy.
    ObjectListBase(const VectorType& v) : VectorType(v) {}

    /// @brief Constructs from a moved vector of shared pointers.
    /// @param v The source vector to move.
    ObjectListBase(VectorType&& v) : VectorType(std::move(v)) {}
};

/**
 * @class ObjectList
 * @brief Provides optional per-type extension methods for ObjectList.
 *
 * This struct is specialized for individual types (e.g., InstrumentUsed) to add
 * type-specific methods. The default template is empty.
 *
 * @tparam T The object type.
 */
template <typename T>
class ObjectList : public ObjectListBase<T>
{
public:
    using ObjectListBase<T>::ObjectListBase;

    // No helpers by default
};

/**
 * @class ObjectList<others::InstrumentUsed>
 * @brief Specialization for @ref others::InstrumentUsed that adds methods for processing the array as a whole.
 */
template<>
class ObjectList<others::InstrumentUsed> : public ObjectListBase<others::InstrumentUsed>
{
public:
    using ObjectListBase<others::InstrumentUsed>::ObjectListBase;

    /// @brief Returns the @ref Staff instance (without any staff styles applied) at a specified index of iuArray or nullptr if not found
    /// @param index The 0-based index to find.
    std::shared_ptr<others::Staff> getStaffInstanceAtIndex(Cmper index) const;

    /// @brief Returns the 0-based index of the InstCmper or std::nullopt if not found.
    /// @param staffId The @ref Staff cmper value to find.
    std::optional<size_t> getIndexForStaff(InstCmper staffId) const;
};

} // namespace dom
} // namespace musx