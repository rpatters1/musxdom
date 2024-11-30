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

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "BaseClasses.h"

namespace musx {
namespace dom {
namespace others {

/**
 * @class OthersPool
 * @brief A pool that manages collections of `OthersBase` objects, organized by XML node names and `Cmper` values.
 * 
 * This class provides functionality to store and retrieve objects derived from `OthersBase`,
 * categorized by their node names and a `Cmper` attribute. It supports dynamic retrieval
 * of objects cast to their specific derived types.
 */
class OthersPool
{
    using OtherPtr = std::shared_ptr<OthersBase>; ///< Alias for a shared pointer to `OthersBase`.
    using Array = std::vector<OtherPtr>;         ///< Alias for a vector of `OtherPtr`.

    /// The internal pool structure: a map of node names to maps of `Cmper` values and arrays of `OtherPtr`.
    std::unordered_map<std::string, std::unordered_map<Cmper, Array>> m_pool;

public:
    /**
     * @brief Adds an `OthersBase` object to the pool.
     * 
     * @param nodeName The XML node name associated with the object.
     * @param other A shared pointer to the `OthersBase` object to add.
     */
    void addOther(const std::string& nodeName, OtherPtr other)
    {
        m_pool[nodeName][other->getCmper()].push_back(other);
    }

    /**
     * @brief Retrieves a vector of objects of a specific type from the pool.
     * 
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param cmper The `Cmper` value used to filter the objects.
     * @return A vector of shared pointers to objects of type `T`.
     */
    template <typename T>
    std::vector<std::shared_ptr<T>> getOthers(Cmper cmper) const
    {
        std::vector<std::shared_ptr<T>> result;

        // Find the pool entry for the specific XML node name.
        auto it = m_pool.find(T::XmlNodeName);
        if (it == m_pool.end()) return result;

        // Find the array corresponding to the given `Cmper` value.
        auto cmperIt = it->second.find(cmper);
        if (cmperIt == it->second.end()) return result;

        // Cast and add each object to the result vector.
        for (const auto& other : cmperIt->second)
        {
            auto typedPtr = std::static_pointer_cast<T>(other);
            assert(typedPtr); // There is a program bug if the pointer cast fails.
            result.push_back(typedPtr);
        }
        return result;
    }
};

/**
 * @class FontDefinition
 * @brief The name and font characteristics of fonts contained in the musx file.
 *
 * The cmper is the font ID used in classes throughout the document.
 *
 * This class is identified by the XML node name "fontName".
 */
class FontDefinition : public OthersBase
{
public:
    using OthersBase::OthersBase; ///< Inherit the constructors from `OthersBase`.

    // Public properties corresponding to the XML structure
    std::string charsetBank; ///< probably only "Mac" or "Win"
    int charsetVal = 0;      ///< A value specifying the character set, usually 4095 or 0
    int pitch = 0;           ///< Represents the `<pitch>` element, e.g., 0. (use unknown)
    int family = 0;          ///< Represents the `<family>` element, e.g., 0. (use unknown)
    std::string name;        ///< The font name e.g., "Broadway Copyist Text".

    constexpr static std::string_view XmlNodeName = "fontName"; ///< The XML node name for this type.
};


} // namespace others
} // namespace dom
} // namespace musx