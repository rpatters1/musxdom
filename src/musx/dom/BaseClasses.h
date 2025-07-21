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

#include <cassert>
#include <filesystem>
#include <set>
#include <string_view>
#include <unordered_set>
#include <memory>

#include "musx/dom/Fundamentals.h"
#include "musx/xml/XmlInterface.h"
#include "musx/util/Logger.h"

namespace musx {

/**
 * @namespace musx::dom
 * @brief The DOM (document object model) for musx files.
 */
namespace dom {

/**
 * @brief Exception for integrity errors. (Used when `MUSX_THROW_ON_INTEGRITY_CHECK_FAIL` is defined.)
 */
class integrity_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

class Document;
/** @brief Shared `Document` pointer */
using DocumentPtr = std::shared_ptr<Document>;
/** @brief Shared weak `Document` pointer */
using DocumentWeakPtr = std::weak_ptr<Document>;

/**
 * @class Base
 * @brief Base class to enforce polymorphism across all DOM classes.
 */
class Base
{
public:
    /// @brief The container type for shared nodes
    using SharedNodes = std::set<std::string>;

    /// @enum ShareMode
    /// @brief Describes how this instance is shared between part and score.
    enum class ShareMode
    {
        // this enum was giving Doxygen fits until I switched to using the slash-splat-bang-lt comment commands.
        All,            /*!< All parts and score always share (no "share" attribute). Default. */
        Partial,        /*!< Part and score share some attributes and have their own unlinked versions of others. (attribute "share"="true") */
        None            /*!< Each part and score has its own version of the DOM class. (attribute "share"="false") */
    };

    /**
     * @brief Virtual destructor for polymorphic behavior.
     */
    virtual ~Base() noexcept(false) = default;

    /**
     * @brief Gets a reference to the Document.
     * 
     * @return A pointer to the Document instance.
     */
    DocumentPtr getDocument() const
    {
        auto document = m_document.lock();
        assert(document); // program bug if this pointer goes out of scope.
        if (!document) {
            throw std::logic_error("Document pointer is no longer valid.");
        }
        return document;
    }

    /**
     * @brief Gets the partId for this instance (or SCORE_PARTID for score)
     * @note When you use `get` or `getArray` to retrieve an instance for a part, the value
     * returned is either for that part or it is for the score. Therefore, this value is
     * only reliable for determining the partId you are working on if the entity's share mode
     * is #ShareMode::None. Otherwise, it returns SCORE_PARTID for instances shared with the score.
     *
     * Runtime-only classes (e.g., StaffComposite) override this function to return the requested part id.
     */
    virtual Cmper getPartId() const { return m_partId; }

    /**
     * @brief Gets the sharing mode for this instance.
     */
    ShareMode getShareMode() const { return m_shareMode; }

    /**
     * @brief Gets the unlinked nodes for this instance. (Only populated for `ShareMode::Partial`)
     */
    const SharedNodes& getUnlinkedNodes() const { return m_unlinkedNodes; }

    /**
     * @brief Adds a shared node for this instance
     */
    void addUnlinkedNode(const std::string& nodeName)
    {
        m_unlinkedNodes.insert(nodeName);
    }

    /**
     * @brief Allows a class to determine if it has been properly contructed by the factory and fix issues that it can,
     * such as creating default instances of contained classes.
     *
     * The default implementation should always be called inside an overridden implementation.
     *
     * @throws #musx::dom::integrity_error if there is a problem.
     */
    virtual void integrityCheck() { }

    /**
     * @brief Specifies if the parser should alert (print or throw) when an unknown xml tag is found for this class.
     *
     * Some classes make it difficult to discover all the possible xml tags that might be used for all its options.
     * An example is @ref others::TextBlock. By overriding this function, a class can allow its members to be discovered
     * as needed without causing error messages or throwing exceptions.
     *
     * @remark This value only escapes errors on fields. Enum values must still have all values provided to avoid
     * error messages or exceptions.
     */
    virtual bool requireAllFields() const { return true; }

protected:
    /**
     * @brief Constructs the base class.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part ID for this instance, or zero if for score.
     * @param shareMode The @ref ShareMode for this instance.
     */
    Base(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode)
        : m_document(document), m_partId(partId), m_shareMode(shareMode) {}

    Base(const Base&) = default;        ///< explicit default copy constructor
    Base(Base&&) noexcept = default;    ///< explicit default move constructor

    /// @brief no-op copy assignment operator allows subclasses to copy their values.
    Base& operator=(const Base&) { return *this; }
    /// @brief no-op move assignment operator allows subclasses to move their values.
    Base& operator=(Base&&) noexcept { return *this; }

private:
    const DocumentWeakPtr m_document;
    const Cmper m_partId;
    const ShareMode m_shareMode;
    SharedNodes m_unlinkedNodes;
};

/**
 * @class CommonClassBase
 * @brief Base class for classes that are commonly used among others, details, entries, and/or texts.
 * They can be constructed in atypical contexts, so their Part and Sharing info is not meaningful.
 */
class CommonClassBase : public Base
{
public:
    /**
     * @brief Constructs a CommonClassBase object.
     * 
     * @param document A weak pointer to the parent document.
     */
    CommonClassBase(const DocumentWeakPtr& document)
        : Base(document, SCORE_PARTID, ShareMode::All) {}

protected:
    // Because the part ID and share mode are hard-coded for this category of classes,
    // these Base functions do not return useful results. However, we allow our subclasses
    // to use their own versions of these functions (esp. `getPartId`) to avoid promiscuous
    // hard-coding of SCORE_PARTID. The rules may change in the future.
    using Base::getPartId;
    using Base::getShareMode;
    using Base::getUnlinkedNodes;
};

/**
 * @class ContainedClassBase
 * @brief Base class for classes that are contained by other classes.
 * @tparam ContainerClass The class that contains this class.
 */
template <typename ContainerClass>
class ContainedClassBase : public Base
{
public:
    /**
     * @brief Constructs a ContainedClassBase object.
     * 
     * @param parent A shared pointer to the parent document.
     */
    ContainedClassBase(const std::shared_ptr<ContainerClass>& parent)
        : Base(parent->getDocument(), SCORE_PARTID, ShareMode::All), m_parent(parent)
    {
        // std::is_base_of_v can't work until the entire class is known, which is true when creating the constructor
        static_assert(std::is_base_of_v<Base, ContainerClass>, "ContainerClass must inherit from Base.");
    }

    /// @brief Override of getPartId returns parent's part ID.
    Cmper getPartId() const override { return getParent()->getPartId(); }

    /// @brief Get the parent.
    std::shared_ptr<ContainerClass> getParent() const
    {
        auto result = m_parent.lock();
        MUSX_ASSERT_IF (!result) {
            throw std::logic_error("Attempt to get parent of contained class, but the parent is no longer allocated.");
        }
        return result;
    }
    
private:
    const std::weak_ptr<ContainerClass> m_parent;
};

/**
 * @brief Base class for all "options" types.
 * 
 * Options types derive from this base class so they can reside in the options pool.
 */
class OptionsBase : public Base {
protected:
    /**
     * @brief Constructs the OptionsBase and validates XmlNodeName in the derived class.
     *
     * @param document A weak pointer to the parent document
     * @param partId Always 0. This parameter is needed for the generic factory routine.
     * @param shareMode Always `ShareMode::All`. This parameter is needed for the generic factory routine.
     */
    OptionsBase(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode)
        : Base(document, partId, shareMode) {}
};

/**
 * @brief Base class for all "others" types.
 * 
 * This class provides common attributes and methods for handling
 * "others" types in the DOM, including `cmper` and `inci`.
 */
class OthersBase : public Base
{
protected:
    /**
     * @brief Constructs an OthersBase object.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part Id for this Other, or zero if for score.
     * @param shareMode Usually `ShareMode::All`. This parameter is used with linked parts data.
     * @param cmper The `Cmper` key value.
     * @param inci The array index (`Inci`).
     */
    OthersBase(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, std::optional<Inci> inci = std::nullopt)
        : Base(document, partId, shareMode), m_cmper(cmper), m_inci(inci) {}

public:
    /**
     * @brief Gets the `cmper` key value.
     * 
     * @return The `cmper` value.
     */
    Cmper getCmper() const { return m_cmper; }

    /**
     * @brief Gets the optional array index (`inci`).
     * 
     * @return The `inci` value.
     */
    std::optional<Inci> getInci() const { return m_inci; }

private:
    Cmper m_cmper;                  ///< Common attribute: cmper (key value).
    std::optional<Inci> m_inci;     ///< Optional array index: inci (starting from 0).
};

/// @brief Template pattern for OthersBase items consisting of an array of a single item.
/// @tparam ElementType The type of the elements in the array
/// @tparam REQUIRED_SIZE If non-zero, the required size of the array.
template <typename ElementType, size_t REQUIRED_SIZE = 0>
class OthersArray : public OthersBase
{
private:
    virtual std::string_view xmlTag() const = 0;

public:
    /** @brief Constructor function */
    explicit OthersArray(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper)
    {
    }

    std::vector<ElementType> values;    ///< Values in the array
                                        ///< Guaranteed to have REQUIRED_SIZE elements.

    /// @brief Override of #Base::integrityCheck
    void integrityCheck() override
    {
        OthersBase::integrityCheck();
        if constexpr (REQUIRED_SIZE > 0) {
            const size_t originalSize = values.size();
            values.resize(REQUIRED_SIZE); // resize first, in case MUSX_INTEGRITY_ERROR throws. (Avoid unreachable code warning.)
            if (originalSize < REQUIRED_SIZE) {
                MUSX_INTEGRITY_ERROR("Array with xml tag " + std::string(xmlTag()) + " and cmper " + std::to_string(getCmper())
                    + " has fewer than " + std::to_string(REQUIRED_SIZE) + " elements.");
            }
        }
    }
};

/**
 * @brief Base class for all "details" types.
 * 
 * This class provides common attributes and methods for handling
 * "details" types in the DOM, including `cmper1`, `cmper2`, and `inci`.
 */
class DetailsBase : public Base
{
protected:
    /**
     * @brief Constructs a DetailsBase object.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part Id for this Detail, or zero if for score.
     * @param shareMode Usually `ShareMode::All`. This parameter is used with linked parts data.
     * @param cmper1 The first `Cmper` key value.
     * @param cmper2 The second `Cmper` key value.
     * @param inci The array index (`Inci`).
     */
    DetailsBase(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper1, Cmper cmper2, std::optional<Inci> inci = std::nullopt)
        : Base(document, partId, shareMode), m_cmper1(cmper1), m_cmper2(cmper2), m_inci(inci) {}

public:
    /**
     * @brief Gets the `cmper1` key value.
     */
    Cmper getCmper1() const { return m_cmper1; }

    /**
     * @brief Gets the `cmper2` key value.
     */
    Cmper getCmper2() const { return m_cmper2; }

    /**
     * @brief Gets the optional array index (`inci`).
     */
    std::optional<Inci> getInci() const { return m_inci; }

private:
    Cmper m_cmper1;                  ///< Common attribute: cmper1 (key value).
    Cmper m_cmper2;                  ///< Common attribute: cmper2 (key value).
    std::optional<Inci> m_inci;     ///< Optional array index: inci (starting from 0).
};

/**
 * @brief Base class for all "details" types that use `entnum` rather than `cmper` and `cmper`.
 */
class EntryDetailsBase : public DetailsBase
{
protected:
    /**
     * @brief Constructs a EntryDetailsBase object.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part Id for this Detail, or zero if for score.
     * @param shareMode Usually `ShareMode::All`. This parameter is used with linked parts data.
     * @param entnum The `EntryNumber` key value.
     * @param inci The array index (`Inci`).
     */
    EntryDetailsBase(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, std::optional<Inci> inci = std::nullopt)
        : DetailsBase(document, partId, shareMode, Cmper(entnum >> 16), Cmper(entnum & 0xffff), inci) {}

public:
    /**
     * @brief Gets the `entnum` key value.
     */
    EntryNumber getEntryNumber() const { return EntryNumber(getCmper1()) << 16 | EntryNumber(getCmper2()); }

private:
    using DetailsBase::getCmper1;
    using DetailsBase::getCmper2;
};

/// @brief Template pattern for DetailsBase items consisting of an array of a single item.
/// @tparam ElementType The type of the elements in the array
/// @tparam REQUIRED_SIZE If non-zero, the required size of the array.
template <typename ElementType, size_t REQUIRED_SIZE = 0>
class DetailsArray : public DetailsBase
{
private:
    virtual std::string_view xmlTag() const = 0;

public:
    /** @brief Constructor function */
    explicit DetailsArray(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper1, Cmper cmper2)
        : DetailsBase(document, partId, shareMode, cmper1, cmper2)
    {
    }

    std::vector<ElementType> values;    ///< Values in the array
                                        ///< Guaranteed to have REQUIRED_SIZE elements.

    /// @brief Override of #Base::integrityCheck
    void integrityCheck() override
    {
        DetailsBase::integrityCheck();
        if constexpr (REQUIRED_SIZE > 0) {
            const size_t originalSize = values.size();
            values.resize(REQUIRED_SIZE); // resize first, in case MUSX_INTEGRITY_ERROR throws. (Avoid unreachable code warning.)
            if (originalSize < REQUIRED_SIZE) {
                MUSX_INTEGRITY_ERROR("Array with xml tag " + std::string(xmlTag()) + " and cmpers [" + std::to_string(getCmper1()) + ", " + std::to_string(getCmper2())
                    + "] has fewer than " + std::to_string(REQUIRED_SIZE) + " elements.");
            }
        }
    }
};

class NoteInfoPtr;

/// @brief Base class note details. Note details are entry details associated with a note ID.
class NoteDetailsBase : public EntryDetailsBase
{
public:
    /// @brief Required virtual function that returns the note id.
    virtual NoteNumber getNoteId() const = 0;

protected:
    /// @brief Constructor for NoteDetailsBase uses @ref EntryDetailsBase constructor.
    using EntryDetailsBase::EntryDetailsBase;
};

class FontInfo;
/**
 * @brief Base class for all text blocks.
 * 
 * Options types derive from this base class so they can reside in the text pool.
 */
class TextsBase : public Base, public std::enable_shared_from_this<TextsBase>
{
public:
    /**
     * @brief Constructs a `TextsBase` object.
     * 
     * @param document A weak pointer to the parent document
     * @param partId Always 0, but this parameter is needed for the generic factory routine
     * @param shareMode Always `ShareMode::All`, but this parameter is needed for the generic factory routine.
     * @param textNumber The text number (`Cmper`).
     */
    TextsBase(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper textNumber)
        : Base(document, partId, shareMode), m_textNumber(textNumber) {}

    std::string text;    ///< Raw Enigma string (with Enigma string tags), encoded UTF-8.

    /**
     * @brief Returns the raw text number.
     */
    Cmper getTextNumber() const { return m_textNumber; }

    /**
     * @brief Sets the raw text number.
     */
    void setTextNumber(Cmper textNumber) { m_textNumber = textNumber; }

    /// @brief Gets the raw text block
    /// @param forPartId The linked part to use for ^partname and ^totpages inserts
    /// @param forPageId The default value to use for ^page inserts. If omitted, the default value is "#", which mimics Finale's behavior.
    /// @param defaultInsertFunc The default text insert replacement function for this context. This function is called if the function supplied
    /// to #util::EnigmaParsingContext::parseEnigmaText returns std::nullopt.
    util::EnigmaParsingContext getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId = std::nullopt,
        util::EnigmaString::TextInsertCallback defaultInsertFunc = util::EnigmaString::defaultInsertsCallback) const;

private:
    Cmper m_textNumber;             ///< Common attribute: cmper (key value).
};

} // namespace dom
} // namespace musx
