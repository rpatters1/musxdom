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

#include <cstdint>
#include <cassert>
#include <filesystem>
#include <set>

#if defined(_WIN32)
#define MUSX_RUNNING_ON_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define MUSX_RUNNING_ON_MACOS
#elif defined(__unix__) || defined(__linux__)
#define MUSX_RUNNING_ON_LINUX_UNIX
#else
#error "Unable to detect operating system platform."
#endif

#ifdef MUSX_THROW_ON_INTEGRITY_CHECK_FAIL
#define MUSX_INTEGRITY_ERROR(S) throw ::musx::dom::integrity_error(S)
#else
#define MUSX_INTEGRITY_ERROR(S) ::musx::util::Logger::log(::musx::util::Logger::LogLevel::Warning, (S))
#endif

#ifdef MUSX_THROW_ON_UNKNOWN_XML
#define MUSX_UNKNOWN_XML(S) throw ::musx::factory::unknown_xml_error(S)
#else
#define MUSX_UNKNOWN_XML(S) ::musx::util::Logger::log(::musx::util::Logger::LogLevel::Warning, (S))
#endif


#define MUSX_ASSERT_IF(TEST) \
assert(!(TEST)); \
if (TEST)


#include "musx/xml/XmlInterface.h"

namespace musx {

/**
 * @namespace musx::dom
 * @brief The DOM (document object model) for musx files.
 */
namespace dom {

namespace others {
class PartDefinition;
}

/**
 * @brief Exception for integrity errors. (Used when `MUSX_THROW_ON_INTEGRITY_CHECK_FAIL` is defined.)
 */
class integrity_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

using Cmper = uint16_t;             ///< Enigma "comperator" key type
using Inci = int16_t;               ///< Enigma "incident" key type
using Evpu = int32_t;               ///< EVPU value (288 per inch)
using EvpuFloat = double;           ///< EVPU fractional value (288.0 per inch)
using Evpu16ths = int32_t;          ///< 1/16 of an EVPU.
using Efix = int32_t;               ///< EFIX value (64 per EVPU, 64*288=18432 per inch)
using Edu = int32_t;                ///< "Enigma Durational Units" value (1024 per quarter note)
using EduFloat = double;            ///< "Enigma Durational Units" floating point value (1024.0 per quarter note)

using MeasCmper = int16_t;          ///< Enigma meas Cmper (may be negative when not applicable)
using InstCmper = int16_t;          ///< Enigma staff (inst) Cmper (may be negative when not applicable)
using SystemCmper = int16_t;        ///< Enigma systems Cmper (may be negative when not applicable)
using ClefIndex = uint16_t;         ///< Index into @ref options::ClefOptions::clefDefs.
using EntryNumber = int32_t;        ///< Entry identifier.
using NoteNumber = uint16_t;        ///< Note identifier.
using LayerIndex = unsigned int;    ///< Layer index (valid values are 0..3)
using BeamNumber = unsigned int;    ///< A number where 1 corresponds to the primary (8th note) beam, 2 the 16th beam, 3 the 32nd beam, etc.

constexpr Cmper MUSX_GLOBALS_CMPER = 65534; ///< The prefs cmper for global variables (used sparingly since Finale 26.2)
constexpr int MAX_LAYERS = 4;       ///< The maximum number of music layers in a Finale document.
constexpr Cmper SCORE_PARTID = 0;   ///< The part id of the score.
constexpr Cmper BASE_SYSTEM_ID = 0; ///< The base system cmper that gives a list of all available staves and their score order (@ref others::InstrumentUsed cmper)

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
     * @brief Gets the partId for this instance (or 0 for score)
     */
    Cmper getPartId() const { return m_partId; }

    /** @brief Gets the @ref others::PartDefinition corresponding to #getPartId */
    std::shared_ptr<others::PartDefinition> getPartDefinition() const;

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
     * @brief Constructs the base class and enforces the static constexpr XmlNodeName.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part ID for this instance, or zero if for score.
     * @param shareMode The @ref ShareMode for this instance.
     */
    Base(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode)
        : m_document(document), m_partId(partId), m_shareMode(shareMode) {}

    /// @brief assignment constructor: m_unlinkedNodes is intentionally omitted
    Base& operator=(const Base&) { return *this; }

private:
    const DocumentWeakPtr m_document;
    const Cmper m_partId;
    const ShareMode m_shareMode;
    SharedNodes m_unlinkedNodes;
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
        if constexpr (REQUIRED_SIZE) {
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

class NoteInfoPtr;

/// @brief Base class note details. Note details are entry details associated with a note ID.
class NoteDetailsBase : public EntryDetailsBase
{
public:
    /// @brief Required virtual function that returns the note id.
    virtual NoteNumber getNoteId() const = 0;

protected:
    using EntryDetailsBase::EntryDetailsBase;
};

class FontInfo;
/**
 * @brief Base class for all text blocks.
 * 
 * Options types derive from this base class so they can reside in the text pool.
 */
class TextsBase : public Base
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

    /**
     * @brief Returns a shared pointer to a FontInfo instance that reflects
     * the first font information in the text.
     */
    std::shared_ptr<FontInfo> parseFirstFontInfo() const;

private:
    Cmper m_textNumber;             ///< Common attribute: cmper (key value).
};

} // namespace dom
} // namespace musx
