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

namespace musx {
namespace dom {

using Cmper = uint16_t;     ///< Enigma "comperator" key type
using Inci = int16_t;       ///< Enigma "incidend" key type
using Evpu = int32_t;       ///< EVPU value (288 per inch)
using EvpuFloat = double;   ///< EVPU fractional value (288.0 per inch)
using Efix = int32_t;       ///< EFIX value (64 per EVPU, 64*288=18432 per inch)
using Edu = int32_t;        ///< EDU value (1024 per quarter note)

constexpr Cmper MUSX_GLOBALS_CMPER = 65534; ///< The prefs cmper for global variables (used sparingly since Finale 26.2)

class Document;
/** @brief Shared `Document` pointer */
using DocumentPtr = std::shared_ptr<Document>;
/** @brief Shared weak `Document` pointer */
using DocumentWeakPtr = std::weak_ptr<Document>;

/**
 * @brief Base class to enforce polymorphism across all DOM classes.
 * 
 * This class uses CRTP (Curiously Recurring Template Pattern) to ensure that all
 * derived classes define a static constexpr `XmlNodeName` of type `constexpr char[]`.
 * 
 * @tparam Derived The derived class inheriting from this base.
 */
class Base
{
public:
    /// @brief The container type for shared nodes
    using SharedNodes = std::set<std::string>;

    ///> @enmum ShareMode
    enum class ShareMode
    {
        All,            ///> All parts and score always share (no "share" attribute). Default.
        Partial,        ///> Part and score share some attributes and share others. (attribute "share"="true")
        None            ///> Each part and score has its own version of the DOM class. (attribute "share"="false")
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
        return document;
    }

    /**
     * @brief Gets the partId for this instance (or 0 for score)
     */
    Cmper getPartId() const { return m_partId; }

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

protected:
    /**
     * @brief Constructs the base class and enforces the static constexpr XmlNodeName.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part Id for this instance, or zero if for score.
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
class OptionsBase : public Base
{
protected:
    /**
     * @brief Constructs the OptionsBase and validates XmlNodeName in the derived class.
     *
     * @param document A weak pointer to the parent document
     * @param partId Usually 0. This parameter is needed for the generic factory routine.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
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
private:
    Cmper m_cmper;                  ///< Common attribute: cmper (key value).
    std::optional<Inci> m_inci;     ///< Optional array index: inci (starting from 0).

protected:
    /**
     * @brief Constructs an OthersBase object.
     * 
     * @param document A weak pointer to the parent document
     * @param partId The part Id for this Other, or zero if for score.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
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
     * @brief Sets the `cmper` key value.
     * 
     * @param cmper The new `cmper` value.
     */
    void setCmper(Cmper cmper) { m_cmper = cmper; }

    /**
     * @brief Gets the optional array index (`inci`).
     * 
     * @return The `inci` value.
     */
    std::optional<Inci> getInci() const { return m_inci; }

    /**
     * @brief Sets the array index (`inci`).
     * 
     * @param inci The new `inci` value.
     */
    void setInci(std::optional<Inci> inci) { m_inci = inci; }
};

class FontInfo;
/**
 * @brief Base class for all text blocks.
 * 
 * Options types derive from this base class so they can reside in the text pool.
 */
class TextsBase : public Base
{
private:
    Cmper m_textNumber;             ///< Common attribute: cmper (key value).

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
};

/**
 * @struct FontInfo
 * @brief Represents the default font settings for a particular element type.
 *
 * The FontInfo struct holds information about font properties, such as the font ID, size, and styles like
 * bold, italic, underline, strikeout, fixed size, and visibility.
 */
class FontInfo : public Base
{
public:
    Cmper fontId{};     ///< Font identifier. This is a Cmper for @ref others::FontDefinition.
    int fontSize{};     ///< Font size.
    bool bold{};        ///< Bold effect.
    bool italic{};      ///< Italic effect.
    bool underline{};   ///< Underline effect.
    bool strikeout{};   ///< Strikeout effect.
    bool absolute{};    ///< Fixed size effect.
    bool hidden{};      ///< Hidden effect.

    /**
     * @brief Default constructor
     * @param document A weak pointer to the document object.
     */
    explicit FontInfo(const DocumentWeakPtr& document)
        : Base(document, 0, ShareMode::All) {}

    /**
     * @brief Get the name of the font.
     * @return The name of the font as a string.
     */
    std::string getFontName() const;

    /**
     * @brief Sets the id of the font from a string name.
     * @param name The font name to find.
     * @throws std::invalid_parameter if the name is not found.
     */
    void setFontIdByName(const std::string& name);

    /**
     * @brief Set style effects based on a bitmask. This is mainly useful for capturing text styles
     * from enigma strings. (See @ref musx::util::EnigmaString::parseFontCommand.)
     *
     * @param efx A 16-bit integer representing style effects with specific bit masks.
     */
    void setEnigmaStyles(uint16_t efx)
    {
        bold = efx & 0x01;         // FONT_EFX_BOLD
        italic = efx & 0x02;       // FONT_EFX_ITALIC
        underline = efx & 0x04;    // FONT_EFX_UNDERLINE
        strikeout = efx & 0x20;    // FONT_EFX_STRIKEOUT
        absolute = efx & 0x40;     // FONT_EFX_ABSOLUTE
        hidden = efx & 0x80;       // FONT_EFX_HIDDEN
    }

    /**
     * @brief Calculates whether this is a SMuFL font.
     */
    bool calcIsSMuFL() const;

    /**
     * @brief Returns the standard SMuFL font folder.
     *
     * @return a std::vector<std::filesystem::path> where element 0 is the user path and element 1 is the system path
     */
    static std::vector<std::filesystem::path> calcSMuFLPaths();
};

} // namespace dom
} // namespace musx
