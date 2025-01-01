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
#include "Entries.h"

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
        TopKeyElementType nodeId;       ///< the identifier for this node. usually the XML node name.
        Cmper partId;                   ///< the part this item is associated with (or 0 for score).
        std::optional<Cmper> cmper1;    ///< optional cmper1 for Others, Texts, Details.
        std::optional<Cmper> cmper2;    ///< optional cmper2 for Details.
        std::optional<Inci> inci;       ///< optional inci for multi-inci classes

        /** @brief explicit constructor for optional parameters */
        ObjectKey(const TopKeyElementType n,
            Cmper p,
            std::optional<Cmper> c1 = std::nullopt,
            std::optional<Cmper> c2 = std::nullopt,
            std::optional<Inci> i = std::nullopt) : nodeId(n), partId(p), cmper1(c1), cmper2(c2), inci(i)
        {
        }

        /** @brief comparison operator for std::map */
        bool operator<(const ObjectKey& other) const
        {
            if (nodeId != other.nodeId) {
                return nodeId < other.nodeId;
            }
            if (partId != other.partId) {
                return partId < other.partId;
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
     * This function may be used to
     * - retrieve a multi-inci array
     * - get all the objects of a specific type, regardless of @ref Cmper value(s), such as getting a vector
     * of all the @ref others::TextExpressionDef instances.
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
                key.partId,
                key.cmper1.value_or((std::numeric_limits<Cmper>::max)()),
                key.cmper2.value_or((std::numeric_limits<Cmper>::max)()), 
                key.inci.value_or((std::numeric_limits<Inci>::max)())
            }
        );

        for (auto it = rangeStart; it != rangeEnd; ++it) {
            auto typedPtr = std::dynamic_pointer_cast<T>(it->second);
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
        auto typedPtr = std::dynamic_pointer_cast<T>(it->second);
        assert(typedPtr); // There is a program bug if the pointer cast fails.
        return typedPtr;
    }

    /**
     * @brief Retrieves the first (and usually only) object of a specific type from the pool for a part
     *
     * If no part item exists, returns the score item.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A shared_ptr to the type or nullptr if none exists
     */
    template <typename T>
    std::shared_ptr<T> getEffectiveForPart(const ObjectKey& key) const
    {
        if (auto partVersion = get<T>(key)) {
            return partVersion;
        }
        if (key.partId == 0) {
            // if this is already the score version, there is nothing to return.
            return nullptr;
        }
        ObjectKey scoreKey(key);
        scoreKey.partId = 0;
        return get<T>(scoreKey);
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
class ScalarPool : protected ObjectPool<ScalarBase>
{
public:
    /** @brief Scalar version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<ScalarBase>& instance)
    { ObjectPool<ScalarBase>::add({nodeName, instance->getPartId()}, instance); }

    /** @brief Scalar version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray() const
    {
        return ObjectPool<ScalarBase>::template getArray<T>({ std::string(T::XmlNodeName), 0 });
    }

    /** @brief Scalar version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get() const
    {
        return ObjectPool<ScalarBase>::template get<T>({ std::string(T::XmlNodeName), 0 });
    }
};

/** @brief Shared `OptionsPool` pointer */
using OptionsPoolPtr = std::shared_ptr<ScalarPool<OptionsBase>>;

/**
 * @class OneCmperPool
 * @brief A pool that manages collections of `OneCmperBase` objects, organized by XML node names and `Cmper` values.
 * 
 * Examples of `OneCmperBase` classes are @ref OthersBase and @ref TextsBase.
 */
template<typename OneCmperBase>
class OneCmperPool : protected ObjectPool<OneCmperBase>
{
public:
    /** @brief OneCmperBase version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray(std::optional<Cmper> cmper = std::nullopt) const
    {
        return ObjectPool<OneCmperBase>::template getArray<T>({ std::string(T::XmlNodeName), 0, cmper });
    }

    /** @brief OneCmperBase version of #ObjectPool::getArray with call for part
     *
     * Use this with items that always exist for parts, such as @ref others::InstrumentUsed lists.
    */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArrayForPart(Cmper partId, std::optional<Cmper> cmper = std::nullopt) const
    {
        return ObjectPool<OneCmperBase>::template getArray<T>({ std::string(T::XmlNodeName), partId, cmper });
    }

    /** @brief OneCmperBase version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(Cmper cmper, std::optional<Inci> inci = std::nullopt) const
    {
        return ObjectPool<OneCmperBase>::template get<T>({std::string(T::XmlNodeName), 0, cmper, std::nullopt, inci});
    }

    /** @brief OneCmperBase version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> getEffectiveForPart(Cmper partId, Cmper cmper, std::optional<Inci> inci = std::nullopt) const
    {
        return ObjectPool<OneCmperBase>::template getEffectiveForPart<T>({std::string(T::XmlNodeName), partId, cmper, std::nullopt, inci});
    }
};

/** @brief Others pool */
class OthersPool : public OneCmperPool<OthersBase>
{
public:
    /** @brief Others version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<OthersBase>& instance)
    { ObjectPool::add({nodeName, instance->getPartId(), instance->getCmper(), std::nullopt, instance->getInci()}, instance); }
};
/** @brief Shared `OthersPool` pointer */
using OthersPoolPtr = std::shared_ptr<OthersPool>;

/** @brief Entry pool */
class EntryPool : public ObjectPool<Entry, EntryNumber>
{
public:
    /** @brief Others version of #ObjectPool::add */
    void add(EntryNumber entryNumber, const std::shared_ptr<Entry>& instance)
    { ObjectPool::add({entryNumber, 0, std::nullopt, std::nullopt, std::nullopt}, instance); }

    /** @brief EntryPool version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(EntryNumber entryNumber) const
    { return ObjectPool::get<T>({entryNumber, 0, std::nullopt, std::nullopt, std::nullopt}); }
};
/** @brief Shared `EntryPool` pointer */
using EntryPoolPtr = std::shared_ptr<EntryPool>;

/** @brief Text pool */
class TextsPool : public OneCmperPool<TextsBase>
{
public:
    /** @brief Texts version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<TextsBase>& instance)
    { ObjectPool::add({nodeName, instance->getPartId(), instance->getTextNumber(), std::nullopt, std::nullopt}, instance); }
};
/** @brief Shared `OthersPool` pointer */
using TextsPoolPtr = std::shared_ptr<TextsPool>;

} // namespace dom
} // namespace musx