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
#include <tuple>
#include <variant>
#include <stdexcept>
#include <functional>

#include "BaseClasses.h"

namespace musx {
namespace dom {

/**
 * @class ObjectPool
 * @brief A pool that manages collections of `OthersBase` objects, organized by XML node names and `Cmper` values.
 * 
 * This class provides functionality to store and retrieve objects derived from a `Base` object type,
 * It supports dynamic retrieval of objects cast to their specific derived types.
 *
 * @tparam ObjectBaseType the base type for the objects in the pool
 */
template <typename ObjectBaseType>
class ObjectPool
{
public:
    using ObjectPtr = std::shared_ptr<ObjectBaseType>;
    using ObjectKey = std::variant<std::string, std::tuple<std::string, Cmper>, std::tuple<std::string, Cmper, Cmper>>;

    /**
     * @brief Adds an `OthersBase` object to the pool.
     * 
     * @param nodeName The XML node name associated with the object.
     * @param other A shared pointer to the `OthersBase` object to add.
     */
    void add(const ObjectKey& key, ObjectPtr object)
    {
        m_pool[key].push_back(object);
    }

    /**
     * @brief Retrieves a vector of objects of a specific type from the pool.
     * 
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A vector of shared pointers to objects of type `T`.
     */
    template <typename T>
    std::vector<std::shared_ptr<T>> get(const ObjectKey& key) const
    {
        std::vector<std::shared_ptr<T>> result;

        // Find the pool entry for the specific XML node name.
        auto it = m_pool.find(key);
        if (it == m_pool.end()) return result;

        // Cast and add each object to the result vector.
        for (const auto& object : it->second)
        {
            auto typedPtr = std::static_pointer_cast<T>(object);
            assert(typedPtr); // There is a program bug if the pointer cast fails.
            result.push_back(typedPtr);
        }
        return result;
    }

protected:
    // prevent standalone construction
    ObjectPool() = default;

private:
    // Hash function for std::tuple
    struct HashTuple
    {
        // Function to calculate the hash of a tuple recursively using an index sequence
        template <typename Tuple, std::size_t... Index>
        std::size_t recursiveHash(const Tuple& tuple, std::index_sequence<Index...>) const {
            std::size_t seed = 0;
            (..., (seed ^= std::hash<std::tuple_element_t<Index, Tuple>>{}(std::get<Index>(tuple)) + 0x9e3779b9 + (seed << 6) + (seed >> 2)));
            return seed;
        }

        // Operator() to calculate the hash of the entire tuple
        template <typename... Args>
        std::size_t operator()(const std::tuple<Args...>& tuple) const {
            return recursiveHash(tuple, std::make_index_sequence<sizeof...(Args)>());
        }
    };

    // Hash function for the Key type to support unordered_map
    struct KeyHash {
        std::size_t operator()(const ObjectKey& key) const {
            return std::visit([](auto&& arg) {
                using DecayedType = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<DecayedType, std::string>) {
                    return std::hash<std::string>{}(arg);
                } else {
                    return HashTuple{}(arg);
                }
             }, key);
        }
    };

    std::unordered_map<ObjectKey, std::vector<ObjectPtr>, KeyHash> m_pool;
};

/**
 * @class ScalarPool
 * @brief A pool that manages collections of `ScalarBase` objects that have no Cmper value.
 * Examples include header and options objects.
 */
template<typename ScalarBase>
class ScalarPool : private ObjectPool<ScalarBase>
{
public:
    /** @brief Scalar version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<ScalarBase>& other)
    { ObjectPool<ScalarBase>::add(nodeName, other); }

    /** @brief Scalar version of #ObjectPool::get */
    template <typename T>
    std::vector<std::shared_ptr<T>> get() const
    { return ObjectPool<ScalarBase>::template get<T>(std::string(T::XmlNodeName)); }
};

/**
 * @class OthersPool
 * @brief A pool that manages collections of `OthersBase` objects, organized by XML node names and `Cmper` values.
 * 
 * This class provides functionality to store and retrieve objects derived from `OthersBase`,
 * categorized by their node names and a `Cmper` attribute.
 */
class OthersPool : private ObjectPool<OthersBase>
{
public:
    /** @brief Others version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<OthersBase>& other)
    { ObjectPool::add(std::make_tuple(nodeName, other->getCmper()), other); }

    /** @brief Others version of #ObjectPool::get */
    template <typename T>
    std::vector<std::shared_ptr<T>> get(Cmper cmper) const
    { return ObjectPool::get<T>(std::make_tuple(std::string(T::XmlNodeName, cmper))); }
};
using OthersPoolPtr = std::shared_ptr<OthersPool>;

} // namespace dom
} // namespace musx