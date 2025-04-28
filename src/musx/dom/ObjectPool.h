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

#include <string>
#include <map>
#include <unordered_map>
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

        /** @brief Returns a string version of the nodeId for inclusion in messages. */
        std::string nodeString() const
        {
            if constexpr (std::is_same_v<TopKeyElementType, std::string>) {
                return nodeId;
            } else {
                return std::string("entry ") + std::to_string(nodeId);
            }
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
        if (key.inci.has_value()) {
            ObjectKey noInciKey = key;
            noInciKey.inci = std::nullopt;
            auto currentIncis = getArray<ObjectBaseType>(noInciKey);
            if (key.inci.value() != int(currentIncis.size())) {
                MUSX_INTEGRITY_ERROR("Node " + key.nodeString() + " has inci " + std::to_string(key.inci.value()) + " that is out of sequence.");
            }
        }
        m_pool.emplace(key, object);
        auto it = m_shareMode.find(key.nodeId);
        if (it == m_shareMode.end()) {
            m_shareMode.emplace(key.nodeId, object->getShareMode());
        } else if (object->getShareMode() != it->second && object->getShareMode() != Base::ShareMode::All) {
            if (it->second == Base::ShareMode::All) {
                m_shareMode[key.nodeId] = object->getShareMode();
            } else {
                MUSX_INTEGRITY_ERROR("Share mode for added " + key.nodeString() + " object [" + std::to_string(int(object->getShareMode()))
                    + "] does not match previous [" + std::to_string(int(it->second)) + "]");                
            }
        }
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
    std::vector<std::shared_ptr<T>> getArrayForPart(const ObjectKey& key) const
    {
        Base::ShareMode forShareMode = Base::ShareMode::All;
        if (key.partId != SCORE_PARTID) {
            auto it = m_shareMode.find(key.nodeId);
            // If the nodeId is not found in m_shareMode, it means the document contains no instances,
            // and every path will return an empty vector. We can safely ignore this case.
            if (it != m_shareMode.end()) {
                forShareMode = it->second;
            }
            if (forShareMode == Base::ShareMode::Partial) {
                if constexpr (std::is_base_of_v<OthersBase, T>) {
                    if (!key.cmper1.has_value()) {
                        throw std::invalid_argument("Array searches on partially shared Others must supply a cmper.");
                    }
                } else if constexpr (std::is_base_of_v<DetailsBase, T>) {
                    if (!key.cmper1.has_value() || !key.cmper2.has_value()) {
                        throw std::invalid_argument("Array searches on partially shared Details must supply both cmpers.");
                    }
                }
            }
        }
        auto keyResult = getArray<T>(key);
        if (!keyResult.empty() || key.partId == SCORE_PARTID || forShareMode == Base::ShareMode::None) {
            return keyResult;
        }
        ObjectKey scoreKey(key);
        scoreKey.partId = SCORE_PARTID;
        return getArray<T>(scoreKey);
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
        if (key.partId == SCORE_PARTID) {
            // if this is already the score version, there is nothing to return.
            return nullptr;
        }
        ObjectKey scoreKey(key);
        scoreKey.partId = SCORE_PARTID;
        return get<T>(scoreKey);
    }

protected:
    // prevent standalone construction
    ObjectPool() = default;

private:
    std::map<ObjectKey, ObjectPtr> m_pool;
    std::unordered_map<TopKeyElementType, dom::Base::ShareMode> m_shareMode;
};

/**
 * @class OptionsPool
 * @brief A pool that manages collections of `OptionsBase` objects that have no Cmper value.
 */
class OptionsPool : protected ObjectPool<OptionsBase>
{
public:
    /** @brief Scalar version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<OptionsBase>& instance)
    {
        if (instance->getPartId()) {
            MUSX_INTEGRITY_ERROR("Options node " + nodeName + " hase non-zero part id [" + std::to_string(instance->getPartId()) + "]");
        }
        ObjectPool::add({ nodeName, instance->getPartId() }, instance);
    }

    /** @brief Scalar version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray() const
    {
        return ObjectPool::getArray<T>({ std::string(T::XmlNodeName), SCORE_PARTID });
    }

    /** @brief Scalar version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get() const
    {
        return ObjectPool::get<T>({ std::string(T::XmlNodeName), SCORE_PARTID });
    }
};
/** @brief Shared `OptionsPool` pointer */
using OptionsPoolPtr = std::shared_ptr<OptionsPool>;

/**
 * @class OthersPool
 * @brief A pool that manages collections of `OthersBase` objects.
 */
class OthersPool : public ObjectPool<OthersBase>
{
public:
    /** @brief OthersPool version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<OthersBase>& instance)
    { ObjectPool::add({nodeName, instance->getPartId(), instance->getCmper(), std::nullopt, instance->getInci()}, instance); }
    
    /** @brief OthersPool version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray(Cmper partId, std::optional<Cmper> cmper = std::nullopt) const
    { return ObjectPool::getArrayForPart<T>({ std::string(T::XmlNodeName), partId, cmper }); }

    /** @brief OthersPool version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(Cmper partId, Cmper cmper, std::optional<Inci> inci = std::nullopt) const
    { return ObjectPool::getEffectiveForPart<T>({std::string(T::XmlNodeName), partId, cmper, std::nullopt, inci}); }
};
/** @brief Shared `OthersPool` pointer */
using OthersPoolPtr = std::shared_ptr<OthersPool>;

/**
 * @class DetailsPool
 * @brief A pool that manages collections of `DetailsBase` objects, organized by XML node names and `Cmper` values.
 * 
 * Examples of `OneCmperBase` classes are @ref OthersBase and @ref TextsBase.
 */
class DetailsPool : protected ObjectPool<DetailsBase>
{
public:
    /** @brief DetailsPool version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<DetailsBase>& instance)
    { ObjectPool::add({nodeName, instance->getPartId(), instance->getCmper1(), instance->getCmper2(), instance->getInci()}, instance); }

    /** @brief DetailsPool version of #ObjectPool::getArray */
    template <typename T, typename std::enable_if_t<!std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    std::vector<std::shared_ptr<T>> getArray(Cmper partId, Cmper cmper1, std::optional<Cmper> cmper2 = std::nullopt) const
    { return ObjectPool::template getArrayForPart<T>({ std::string(T::XmlNodeName), partId, cmper1, cmper2 }); }

    /** @brief EntryDetailsPool version of #ObjectPool::getArray */
    template <typename T, typename std::enable_if_t<std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    std::vector<std::shared_ptr<T>> getArray(Cmper partId, EntryNumber entnum) const
    { return ObjectPool::template getArrayForPart<T>({ std::string(T::XmlNodeName), partId, Cmper(entnum >> 16), Cmper(entnum & 0xffff) }); }

    /** @brief DetailsPool version of #ObjectPool::get */
    template <typename T, typename std::enable_if_t<!std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    std::shared_ptr<T> get(Cmper partId, Cmper cmper1, Cmper cmper2, std::optional<Inci> inci = std::nullopt) const
    { return ObjectPool::getEffectiveForPart<T>({std::string(T::XmlNodeName), partId, cmper1, cmper2, inci}); }

    /** @brief EntryDetailsPool version of #ObjectPool::get */
    template <typename T, typename std::enable_if_t<std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    std::shared_ptr<T> get(Cmper partId, EntryNumber entnum, std::optional<Inci> inci = std::nullopt) const
    { return ObjectPool::getEffectiveForPart<T>({std::string(T::XmlNodeName), partId, Cmper(entnum >> 16), Cmper(entnum & 0xffff), inci}); }

    /// @brief Returns the detail for a particular note
    template <typename T, typename std::enable_if_t<std::is_base_of_v<NoteDetailsBase, T>, int> = 0>
    std::shared_ptr<T> getForNote(const NoteInfoPtr noteInfo)
    {
        auto entry = noteInfo.getEntryInfo()->getEntry();
        auto details = getArray<T>(entry->getPartId(), entry->getEntryNumber());
        for (const auto& detail : details) {
            if (detail->getNoteId() == noteInfo->getNoteId()) {
                return detail;
            }
        }
        return nullptr;
    }
};

/** @brief Shared `DetailsPool` pointer */
using DetailsPoolPtr = std::shared_ptr<DetailsPool>;

/** @brief Entry pool */
class EntryPool : protected ObjectPool<Entry, EntryNumber>
{
public:
    /** @brief EntryPool version of #ObjectPool::add */
    void add(EntryNumber entryNumber, const std::shared_ptr<Entry>& instance)
    { ObjectPool::add({ entryNumber, SCORE_PARTID }, instance); }

    /** @brief EntryPool version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(EntryNumber entryNumber) const
    { return ObjectPool::get<T>({ entryNumber, SCORE_PARTID }); }
};
/** @brief Shared `EntryPool` pointer */
using EntryPoolPtr = std::shared_ptr<EntryPool>;

/** @brief Text pool */
class TextsPool : protected ObjectPool<TextsBase>
{
public:
    /** @brief Texts version of #ObjectPool::add */
    void add(const std::string& nodeName, const std::shared_ptr<TextsBase>& instance)
    {
        if (instance->getPartId()) {
            MUSX_INTEGRITY_ERROR("Texts node " + nodeName + " hase non-zero part id [" + std::to_string(instance->getPartId()) + "]");
        }
        ObjectPool::add({ nodeName, instance->getPartId(), instance->getTextNumber() }, instance);
    }
    
    /** @brief Texts version of #ObjectPool::getArray */
    template <typename T>
    std::vector<std::shared_ptr<T>> getArray(std::optional<Cmper> cmper = std::nullopt) const
    { return ObjectPool::getArray<T>({ std::string(T::XmlNodeName), SCORE_PARTID, cmper }); }

    /** @brief Texts version of #ObjectPool::get */
    template <typename T>
    std::shared_ptr<T> get(Cmper cmper) const
    { return ObjectPool::get<T>({ std::string(T::XmlNodeName), SCORE_PARTID, cmper, std::nullopt, std::nullopt }); }
};
/** @brief Shared `OthersPool` pointer */
using TextsPoolPtr = std::shared_ptr<TextsPool>;

} // namespace dom
} // namespace musx