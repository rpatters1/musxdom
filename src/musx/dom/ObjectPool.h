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

#include "MusxInstance.h"

#include "BaseClasses.h"
#include "Others.h"
#include "Details.h"
#include "Entries.h"
#include "ShapeDesigner.h"
#include "SmartShape.h"
#include "Staff.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
namespace bench {
/// @brief Gives access to the raw pools for benchmarking. This class is
/// only defined in the benchmarks.
template<class> class PoolAccessor;
}; // namespace bench
#endif

namespace musx {
namespace dom {

/// @brief Type trait to determine if a class in a given pool
/// @tparam Pool The pool type
/// @tparam T The class to query
template <typename Pool, typename T>
struct is_pool_type : std::false_type {};  // default: not valid

/// @brief Value shortcut for @ref is_pool_type
template <typename Pool, typename T>
inline constexpr bool is_pool_type_v = is_pool_type<Pool, T>::value;

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
        /** @brief shared pointer to `ObjectBaseType` */
    using ObjectPtr = std::shared_ptr<ObjectBaseType>;
    /** @brief key type for storing in pool */
    struct ObjectKey {
        std::string_view nodeId;        ///< the identifier for this node. usually the XML node name.
        Cmper partId;                   ///< the part this item is associated with (or 0 for score).
        std::optional<Cmper> cmper1;    ///< optional cmper1 for Others, Texts, Details.
        std::optional<Cmper> cmper2;    ///< optional cmper2 for Details.
        // Use `int` instead of `Inci` to work around GCC spurious -Wmaybe-uninitialized warning
        std::optional<int> inci;        ///< optional inci for multi-inci classes

        /** @brief explicit constructor for optional parameters */
        ObjectKey(std::string_view n,
            Cmper p,
            std::optional<Cmper> c1 = std::nullopt,
            std::optional<Cmper> c2 = std::nullopt,
            std::optional<int> i = std::nullopt) : nodeId(n), partId(p), cmper1(c1), cmper2(c2), inci(i)
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

        /** @brief provides a description of the key for diagnostic purposes */
        std::string description() const
        {
            std::string result = std::string(nodeId) + " part " + std::to_string(partId);
            if (cmper1) {
                if (cmper2) {
                    result += " cmpers [" + std::to_string(cmper1.value()) + ", " + std::to_string(cmper2.value()) + "]";
                } else {
                    result += " cmper " + std::to_string(cmper1.value());
                }
            }
            if (inci) {
                result += " inci " + std::to_string(inci.value());
            }
            return result;
        }
    };

private:
    template <typename T>
    std::shared_ptr<const T> bindWithPartId(std::shared_ptr<const T> obj, Cmper requestedPartId) const
    {
        if constexpr (std::is_base_of_v<OthersBase, T> || std::is_base_of_v<DetailsBase, T>) {
            if (obj && obj->getRequestedPartId() != requestedPartId) {
                return PartContextCloner::copyWithPartId(obj, requestedPartId);
            }
        }
        return obj;
    }

    inline static ObjectKey makeEndKey(const ObjectKey& key)
    {
        return ObjectKey{
            key.nodeId,
            key.partId,
            key.cmper1.value_or((std::numeric_limits<Cmper>::max)()),
            key.cmper2.value_or((std::numeric_limits<Cmper>::max)()),
            key.inci.value_or((std::numeric_limits<Inci>::max)())
        };
    }

    template <typename T>
    inline static MusxInstance<T> checkedStaticCast(const ObjectKey& key, const ObjectPtr& p)
    {
        static_assert(std::is_base_of_v<ObjectBaseType, T>, "T must derive from ObjectBaseType");
        if constexpr (!std::is_same_v<T, ObjectBaseType>) {
            assert(T::XmlNodeName == key.nodeId && "nodeId/type mismatch: pool invariant violated");
        }
    #ifndef NDEBUG
        // Optional paranoia while refactoring:
        //assert(std::dynamic_pointer_cast<T>(p));
    #endif
        return std::static_pointer_cast<T>(p);
    }

    inline static bool logicalLess(const ObjectKey& a, const ObjectKey& b)
    {
        return std::tie(a.cmper1, a.cmper2, a.inci) < std::tie(b.cmper1, b.cmper2, b.inci);
    }

    inline static bool logicalEq(const ObjectKey& a, const ObjectKey& b)
    {
        return a.cmper1 == b.cmper1 && a.cmper2 == b.cmper2 && a.inci == b.inci;
    }

public:
    /** @brief virtual destructor */
    virtual ~ObjectPool() = default;

    /**
     * @brief Adds an `ObjectBaseType` object to the pool.
     *
     * @param key The key with which to store the object
     * @param object A shared pointer to the `ObjectBaseType` object to add.
     */
    void add(ObjectKey key, ObjectPtr object)
    {
        if (key.inci.has_value()) {
            ObjectKey noInciKey = key;
            noInciKey.inci = std::nullopt;
            auto currentIncis = getArray<ObjectBaseType>(noInciKey, key.partId);
            if (key.inci.value() != int(currentIncis.size())) {
                MUSX_INTEGRITY_ERROR("Node " + std::string(key.nodeId) + " has inci " + std::to_string(key.inci.value()) + " that is out of sequence.");
            }
        }
        auto shareModeIt = m_shareMode.find(key.nodeId);
        auto [poolIt, emplaced] = m_pool.emplace(std::move(key), object);
        if (!emplaced) {
            MUSX_INTEGRITY_ERROR("Attempted to add same key more than once: " + poolIt->first.description());
        }
        if (shareModeIt == m_shareMode.end()) {
            m_shareMode.emplace(poolIt->first.nodeId, object->getShareMode());
        } else if (object->getShareMode() != shareModeIt->second && object->getShareMode() != Base::ShareMode::All) {
            if (shareModeIt->second == Base::ShareMode::All) {
                m_shareMode[poolIt->first.nodeId] = object->getShareMode();
            } else {
                MUSX_INTEGRITY_ERROR("Share mode for added " + std::string(poolIt->first.nodeId) + " object [" + std::to_string(int(object->getShareMode()))
                    + "] does not match previous [" + std::to_string(int(shareModeIt->second)) + "]");
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
     * @tparam T The derived type of `ObjectBaseType` to retrieve.
     * @param key The key value used to filter the objects.
     * @param requestedPartId The part for which this array was requested.
     * @return An MusxInstanceList of shared pointers to objects of type `T`.
     */
    template <typename T>
    MusxInstanceList<T> getArray(const ObjectKey& key, Cmper requestedPartId) const
    {
        MusxInstanceList<T> result(m_document, requestedPartId);

        auto rangeStart = m_pool.lower_bound(key);
        auto rangeEnd = m_pool.upper_bound(makeEndKey(key));

        for (auto it = rangeStart; it != rangeEnd; ++it) {
            auto typedPtr = bindWithPartId<T>(checkedStaticCast<T>(key, it->second), requestedPartId);
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
     * @tparam T The derived type of `ObjectBaseType` to retrieve.
     * @param key The key value used to filter the objects.
     * @return An MusxInstanceList of shared pointers to objects of type `T`.
     */
    template <typename T>
    MusxInstanceList<T> getArrayForPart(const ObjectKey& key) const
    {
        Base::ShareMode forShareMode = Base::ShareMode::All;
        if (key.partId != SCORE_PARTID) {
            auto it = m_shareMode.find(key.nodeId);
            // If the nodeId is not found in m_shareMode, it means the document contains no instances,
            // and every path will return an empty vector. We can safely ignore this case.
            if (it != m_shareMode.end()) {
                forShareMode = it->second;
            }
        }

        if (key.partId == SCORE_PARTID || forShareMode != Base::ShareMode::Partial) {
            return getArray<T>(key, key.partId);
        }

        const auto partStart = m_pool.lower_bound(key);
        const auto partEnd   = m_pool.upper_bound(makeEndKey(key));

        ObjectKey scoreKey(key);
        scoreKey.partId = SCORE_PARTID;
        const auto scoreStart = m_pool.lower_bound(scoreKey);
        const auto scoreEnd   = m_pool.upper_bound(makeEndKey(scoreKey));

        auto pit = partStart;
        auto sit = scoreStart;

        MusxInstanceList<T> result(m_document, key.partId);
        while (pit != partEnd || sit != scoreEnd) {
            auto emit = [&](const auto& it) {
                auto typed = bindWithPartId<T>(checkedStaticCast<T>(key, it->second), key.partId);
                result.push_back(typed);
            };

            if (sit == scoreEnd) {
                emit(pit++);
                continue;
            }
            if (pit == partEnd) {
                emit(sit++); // bind score record to requested part
                continue;
            }

            const ObjectKey& pk = pit->first;
            const ObjectKey& sk = sit->first;

            if (logicalEq(pk, sk)) {
                emit(pit++); // prefer part instance
                sit++;
            } else if (logicalLess(pk, sk)) {
                emit(pit++);
            } else {
                emit(sit++); // score fallback
            }
        }

        return result;
    }

    /**
     * @brief Retrieves the first (and usually only) object of a specific type from the pool.
     * @warning The returned value is the source item from the pool and is guaranteed not to be a copy.
     * Because of this, it does not reflect the requested part id. External callers should not
     * directly call this function.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A shared_ptr to the type or nullptr if none exists
     */
    template <typename T>
    MusxInstance<T> getSource(const ObjectKey& key) const
    {
        auto it = m_pool.find(key);
        if (it == m_pool.end()) {
            return nullptr;
        }
        return checkedStaticCast<T>(key, it->second);
    }

    /**
     * @brief Retrieves the first (and usually only) object of a specific type from the pool for a part
     * @warning The returned value is the source item from the pool and is guaranteed not to be a copy.
     * Because of this, it does not reflect the requested part id. External callers should not
     * directly call this function.
     *
     * If no part item exists, returns the score item.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A shared_ptr to the type or nullptr if none exists
     */
    template <typename T>
    MusxInstance<T> getEffectiveSourceForPart(const ObjectKey& key) const
    {
        if (auto partVersion = getSource<T>(key)) {
            return partVersion;
        }
        if (key.partId == SCORE_PARTID) {
            // if this is already the score version, there is nothing to return.
            return nullptr;
        } else {
            // if this is for a part, do not search score for unshared types.
            auto it = m_shareMode.find(key.nodeId);
            if (it != m_shareMode.end() && it->second == Base::ShareMode::None) {
                return nullptr;
            }
        }
        ObjectKey scoreKey(key);
        scoreKey.partId = SCORE_PARTID;
        return getSource<T>(scoreKey);
    }

    /**
     * @brief Retrieves the first (and usually only) object of a specific type from the pool for a part.
     *
     * If no part item exists, returns the score item.
     *
     * If the requestedPartId differs from the sourcePartId, a copy of the source is returned with the
     * requestedPartId set correctly.
     *
     * @tparam T The derived type of `OthersBase` to retrieve.
     *           Must have a `constexpr static std::string_view XmlNodeName` member.
     * @param key The key value used to filter the objects.
     * @return A shared_ptr to the type or nullptr if none exists
     */
    template <typename T>
    MusxInstance<T> getEffectiveForPart(const ObjectKey& key) const
    {
        return bindWithPartId<T>(getEffectiveSourceForPart<T>(key), key.partId);
    }

    /// @brief Constructs the object pool
    /// @param document THe document for this pool.
    /// @param knownShareModes Optional parameter that specifies known share modes for certain elements.
    /// These can be particurly important for Base::ShareMode::None because there may be no parts containing them.
    ObjectPool(const DocumentWeakPtr& document, const std::unordered_map<std::string_view, dom::Base::ShareMode>& knownShareModes = {})
        : m_document(document), m_shareMode(knownShareModes) {}

private:
    DocumentWeakPtr m_document;
    std::unordered_map<std::string_view, dom::Base::ShareMode> m_shareMode;

    std::map<ObjectKey, ObjectPtr> m_pool;
};

/**
 * @class OptionsPool
 * @brief A pool that manages collections of `OptionsBase` objects that have no Cmper value.
 */
class OptionsPool
{
    ObjectPool<OptionsBase> m_pool;
    friend class bench::PoolAccessor<OptionsPool>;

public:
    /// @brief Constructor function
    OptionsPool(const DocumentWeakPtr& document) : m_pool(document) {}

    /** @brief Scalar version of #ObjectPool::add */
    void add(std::string_view nodeName, const std::shared_ptr<OptionsBase>& instance)
    {
        const Base* basePtr = instance.get();
        if (basePtr->getSourcePartId() != SCORE_PARTID) {
            MUSX_INTEGRITY_ERROR("Options node " + std::string(nodeName) + " hase non-score part id [" + std::to_string(basePtr->getSourcePartId()) + "]");
        }
        m_pool.add({ nodeName, basePtr->getSourcePartId() }, instance);
    }

    /** @brief Scalar version of #ObjectPool::getArray */
    template <typename T>
    MusxInstanceList<T> getArray() const
    {
        static_assert(is_pool_type_v<OptionsPool, T>, "Type T is not registered in OptionsPool");
        return m_pool.getArray<T>({ T::XmlNodeName, SCORE_PARTID }, SCORE_PARTID);
    }

    /** @brief Get a single item out of the pool */
    template <typename T>
    MusxInstance<T> get() const
    {
        static_assert(is_pool_type_v<OptionsPool, T>, "Type T is not registered in OptionsPool");
        return m_pool.getSource<T>({ T::XmlNodeName, SCORE_PARTID });
    }
};
/** @brief Shared `OptionsPool` pointer */
using OptionsPoolPtr = std::shared_ptr<OptionsPool>;

/**
 * @class OthersPool
 * @brief A pool that manages collections of `OthersBase` objects.
 */
class OthersPool
{
    ObjectPool<OthersBase> m_pool;
    friend class bench::PoolAccessor<OthersPool>;
    friend MusxInstance<others::StaffComposite> others::StaffComposite::createCurrent(const DocumentPtr& document, Cmper partId, StaffCmper staffId, MeasCmper measId, Edu eduPosition);

    MusxInstance<others::Staff> getRawStaff(Cmper partId, StaffCmper staffId)
    {
        return m_pool.getEffectiveSourceForPart<others::Staff>({ std::string(others::Staff::XmlNodeName), partId, staffId, std::nullopt, std::nullopt });
    }

public:
    /// @brief Constructor
    OthersPool(const DocumentWeakPtr& document) : m_pool(document, {
        { others::BeatChartElement::XmlNodeName, Base::ShareMode::None },
        { others::StaffUsed::XmlNodeName, Base::ShareMode::None },
        { others::SystemLock::XmlNodeName, Base::ShareMode::None },
        { others::MultimeasureRest::XmlNodeName, Base::ShareMode::None },
        { others::MultiStaffGroupId::XmlNodeName, Base::ShareMode::None },
        { others::Page::XmlNodeName, Base::ShareMode::None },
        { others::PartGlobals::XmlNodeName, Base::ShareMode::None },
        { others::PartVoicing::XmlNodeName, Base::ShareMode::None },
        { others::StaffSystem::XmlNodeName, Base::ShareMode::None },
        { others::StaffStyleAssign::XmlNodeName, Base::ShareMode::None },
        // add other known sharemode none items as they are identified.
    }) {}

    /** @brief OthersPool version of #ObjectPool::add */
    void add(std::string_view nodeName, const std::shared_ptr<OthersBase>& instance)
    { m_pool.add({nodeName, instance->getSourcePartId(), instance->getCmper(), std::nullopt, instance->getInci()}, instance); }

    /** @brief OthersPool version of #ObjectPool::getArray */
    template <typename T>
    MusxInstanceList<T> getArray(Cmper partId, std::optional<Cmper> cmper = std::nullopt) const
    {
        static_assert(is_pool_type_v<OthersPool, T>, "Type T is not registered in OthersPool");
        return m_pool.getArrayForPart<T>({ T::XmlNodeName, partId, cmper });
    }

    /** @brief Get a single item out of the pool */
    template <typename T>
    MusxInstance<T> get(Cmper partId, Cmper cmper, std::optional<Inci> inci = std::nullopt) const
    {
        static_assert(is_pool_type_v<OthersPool, T>, "Type T is not registered in OthersPool");
        return m_pool.getEffectiveForPart<T>({ T::XmlNodeName, partId, cmper, std::nullopt, inci });
    }
};
/** @brief Shared `OthersPool` pointer */
using OthersPoolPtr = std::shared_ptr<OthersPool>;

/**
 * @class DetailsPool
 * @brief A pool that manages collections of `DetailsBase` objects, organized by XML node names and `Cmper` values.
 *
 * Examples of `OneCmperBase` classes are @ref OthersBase and @ref TextsBase.
 */
class DetailsPool
{
    ObjectPool<DetailsBase> m_pool;
    friend class bench::PoolAccessor<DetailsPool>;

public:
    /// @brief Constructor
    DetailsPool(const DocumentWeakPtr& document) : m_pool(document, {
        { details::CenterShape::XmlNodeName, Base::ShareMode::None },
        { details::StaffGroup::XmlNodeName, Base::ShareMode::None },
        { details::StaffSize::XmlNodeName, Base::ShareMode::None },
        // add other known sharemode none items as they are identified.
    }) {}

    /** @brief DetailsPool version of #ObjectPool::add */
    void add(std::string_view nodeName, const std::shared_ptr<DetailsBase>& instance)
    { m_pool.add({nodeName, instance->getSourcePartId(), instance->getCmper1(), instance->getCmper2(), instance->getInci()}, instance); }

    /** @brief version of #ObjectPool::getArray for getting all of them */
    template <typename T, typename = std::enable_if_t<is_pool_type_v<DetailsPool, T>>>
    MusxInstanceList<T> getArray(Cmper partId) const
    { return m_pool.template getArrayForPart<T>({ T::XmlNodeName, partId }); }

    /** @brief DetailsPool version of #ObjectPool::getArray */
    template <typename T, typename std::enable_if_t<!std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    MusxInstanceList<T> getArray(Cmper partId, Cmper cmper1, std::optional<Cmper> cmper2 = std::nullopt) const
    {
        static_assert(is_pool_type_v<DetailsPool, T>, "Type T is not registered in DetailsPool");
        return m_pool.template getArrayForPart<T>({ T::XmlNodeName, partId, cmper1, cmper2 });
    }

    /** @brief EntryDetailsPool version of #ObjectPool::getArray */
    template <typename T, typename std::enable_if_t<std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    MusxInstanceList<T> getArray(Cmper partId, EntryNumber entnum) const
    {
        static_assert(is_pool_type_v<DetailsPool, T>, "Type T is not registered in DetailsPool");
        return m_pool.template getArrayForPart<T>({ T::XmlNodeName, partId, Cmper(entnum >> 16), Cmper(entnum & 0xffff) });
    }

    /** @brief Get a single DetailsBase item out of the pool (not EntryDetailsBase) */
    template <typename T, typename std::enable_if_t<!std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    MusxInstance<T> get(Cmper partId, Cmper cmper1, Cmper cmper2, std::optional<Inci> inci = std::nullopt) const
    {
        static_assert(is_pool_type_v<DetailsPool, T>, "Type T is not registered in DetailsPool");
        return m_pool.getEffectiveForPart<T>({ T::XmlNodeName, partId, cmper1, cmper2, inci });
    }

    /** @brief Get a single EntryDetailsBase item out of the pool */
    template <typename T, typename std::enable_if_t<std::is_base_of_v<EntryDetailsBase, T>, int> = 0>
    MusxInstance<T> get(Cmper partId, EntryNumber entnum, std::optional<Inci> inci = std::nullopt) const
    {
        static_assert(is_pool_type_v<DetailsPool, T>, "Type T is not registered in DetailsPool");
        return m_pool.getEffectiveForPart<T>({ T::XmlNodeName, partId, Cmper(entnum >> 16), Cmper(entnum & 0xffff), inci });
    }

    /// @brief Returns the detail for a particular note
    /// @tparam T The type to retrieve (must be derived from @ref NoteDetailsBase)
    /// @tparam enable_if_t Enforces T as a base of @ref NoteDetailsBase
    /// @param noteInfo The note for which to get the note detail
    /// @param forPartId The part for which to get the note detail. If omitted, the @p noteInfo part is used.
    /// @return The instance associated with @p noteInfo or nullptr if none.
    template <typename T, typename std::enable_if_t<std::is_base_of_v<NoteDetailsBase, T>, int> = 0>
    MusxInstance<T> getForNote(const NoteInfoPtr& noteInfo, const std::optional<Cmper>& forPartId = std::nullopt)
    {
        static_assert(is_pool_type_v<DetailsPool, T>, "Type T is not registered in DetailsPool");
        auto details = getArray<T>(
            forPartId.value_or(noteInfo.getEntryInfo().getFrame()->getRequestedPartId()),
            noteInfo.getEntryInfo()->getEntry()->getEntryNumber()
        );
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
class EntryPool // uses different implementation than other pools for more efficient access
{
public:
    /// @brief Constructor function
    EntryPool(const DocumentWeakPtr& document) : m_document(document) {}

    /** @brief Add an entry to the EntryPool. (Used by the factory.) */
    void add(EntryNumber entryNumber, const std::shared_ptr<Entry>& instance)
    {
        auto [it, emplaced] = m_pool.emplace(entryNumber, instance);
        if (!emplaced) {
            MUSX_INTEGRITY_ERROR("Entry number " + std::to_string(entryNumber) + " added twice.");
        }
    }

    /** @brief Get an entry from the EntryPool. */
    MusxInstance<Entry> get(EntryNumber entryNumber) const
    {
        const auto it = m_pool.find(entryNumber);
        if (it == m_pool.end()) {
            return nullptr;
        }
        return it->second;
    }

private:
    DocumentWeakPtr m_document;
    std::unordered_map<EntryNumber, std::shared_ptr<Entry>> m_pool;

    friend class bench::PoolAccessor<EntryPool>;
};
/** @brief Shared `EntryPool` pointer */
using EntryPoolPtr = std::shared_ptr<EntryPool>;

/** @brief Text pool */
class TextsPool
{
    ObjectPool<TextsBase> m_pool;
    friend class bench::PoolAccessor<TextsPool>;

public:
    /// @brief Constructor fundtion
    TextsPool(const DocumentWeakPtr& document) : m_pool(document) {}

    /** @brief Texts version of #ObjectPool::add */
    void add(std::string_view nodeName, const std::shared_ptr<TextsBase>& instance)
    {
        const Base* basePtr = instance.get();
        if (basePtr->getSourcePartId() != SCORE_PARTID) {
            MUSX_INTEGRITY_ERROR("Texts node " + std::string(nodeName) + " hase non-score part id [" + std::to_string(basePtr->getSourcePartId()) + "]");
        }
        m_pool.add({ nodeName, basePtr->getSourcePartId(), instance->getTextNumber() }, instance);
    }

    /** @brief Texts version of #ObjectPool::getArray */
    template <typename T>
    MusxInstanceList<T> getArray(std::optional<Cmper> cmper = std::nullopt) const
    {
        static_assert(is_pool_type_v<TextsPool, T>, "Type T is not registered in TextsPool");
        return m_pool.getArray<T>({ T::XmlNodeName, SCORE_PARTID, cmper }, SCORE_PARTID);
    }

    /** @brief Get a single item out of the pool */
    template <typename T>
    MusxInstance<T> get(Cmper cmper) const
    {
        static_assert(is_pool_type_v<TextsPool, T>, "Type T is not registered in TextsPool");
        return m_pool.getSource<T>({ T::XmlNodeName, SCORE_PARTID, cmper, std::nullopt, std::nullopt });
    }
};
/** @brief Shared `OthersPool` pointer */
using TextsPoolPtr = std::shared_ptr<TextsPool>;

} // namespace dom
} // namespace musx