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
#include <map>
#include <vector>
#include <memory>
#include <tuple>
#include <variant>
#include <stdexcept>
#include <functional>
#include <limits>

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
template <typename ObjectBaseType, typename TopKeyElementType = std::string>
class ObjectPool
{
public:
    /** @brief shared pointer to `ObjectBaseType` */
    using ObjectPtr = std::shared_ptr<ObjectBaseType>;
    /** @brief key type for storing in pool */
    struct ObjectKey {
        TopKeyElementType nodeId;
        std::optional<Cmper> cmper1;
        std::optional<Cmper> cmper2;
        std::optional<Inci> inci;

        bool operator<(const ObjectKey& other) const {
            if (nodeId != other.nodeId) {
                return nodeId < other.nodeId;
            }
            if (cmper1 != other.cmper1) {
                return cmper1 < other.cmper1;
            }
            if (cmper2 != other.cmper2) {
                return cmper2 < other.cmper2;
            }
            return inci < other.inci;
        }
    };

    /** @brief virtual destructor */
    virtual ~ObjectPool() = default;

    /**
     * @brief Adds an `OthersBase` object to the pool.
     * 
     * @param key The key with which to store the object 
     * @param object A shared pointer to the `ObjectBaseType` object to add.
     */
    void add(const ObjectKey& key, ObjectPtr object)
    {
        m_pool.emplace(key, object);
    }

    /**
     * @brief Retrieves a vector of objects of a specific type from the pool.
     *
     * Some types are arrays (with a 0-based "inci" value). Use this function to
     * retrieve the entire array.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     * @param key The key value used to filter the objects.
     * @return A vector of shared pointers to objects of type `T`.
     */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray(const ObjectKey& key) const
    {
        std::vector<std::shared_ptr<T>> result;

        auto rangeStart = m_pool.lower_bound(key);
        auto rangeEnd = m_pool.upper_bound(
            ObjectKey{
                key.nodeId,
                key.cmper1.value_or(std::numeric_limits<Cmper>::max()), 
                key.cmper2.value_or(std::numeric_limits<Cmper>::max()), 
                key.inci.value_or(std::numeric_limits<Inci>::max())
            }
        );

        for (auto it = rangeStart; it != rangeEnd; ++it) {
            auto typedPtr = std::static_pointer_cast<T>(it->second);
            assert(typedPtr);
            result.push_back(typedPtr);
        }
        return result;
    }

    /**
     * @brief Retrieves the first (and usually only) object of a specific type from the pool.
     *
     * Many types are scalar values with only one instance per key. Use this function
     * to retrieve them.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A shared_ptr to the type or nullptr if none exists
     */
    template <typename T>
    std::shared_ptr<T> get(const ObjectKey& key) const
    {
        auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            return nullptr;
        }
        auto typedPtr = std::static_pointer_cast<T>(it->second);
        assert(typedPtr); // There is a program bug if the pointer cast fails.
        return typedPtr;
    }

protected:
    // prevent standalone construction
    ObjectPool() = default;

private:
    std::map<ObjectKey, ObjectPtr> m_pool;
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
    { ObjectPool<ScalarBase>::add({nodeName}, other); }

    /** @brief Scalar version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray() const
    { return ObjectPool<ScalarBase>::template getArray<T>({std::string(T::XmlNodeName)}); }

    /** @brief Scalar version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get() const
    { return ObjectPool<ScalarBase>::template get<T>({std::string(T::XmlNodeName)}); }
};

/** @brief Shared `OptionsPool` pointer */
using OptionsPoolPtr = std::shared_ptr<ScalarPool<OptionsBase>>;

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
    { ObjectPool::add({nodeName, other->getCmper(), std::nullopt, other->getInci()}, other); }

    /** @brief Others version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray(std::optional<Cmper> cmper = std::nullopt) const
    { return ObjectPool::getArray<T>({std::string(T::XmlNodeName), cmper}); }

    /** @brief Others version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(Cmper cmper, std::optional<Inci> inci = std::nullopt) const
    {
        return ObjectPool::get<T>({ std::string(T::XmlNodeName), cmper, std::nullopt, inci});
    }
};

/** @brief Shared `OthersPool` pointer */
using OthersPoolPtr = std::shared_ptr<OthersPool>;

} // namespace dom
} // namespace musx