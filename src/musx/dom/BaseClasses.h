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

#ifdef MUSX_THROW_ON_INTEGRITY_CHECK_FAIL
#define MUSX_INTEGRITY_ERROR(S) throw ::musx::dom::integrity_error(S)
#else
#define MUSX_INTEGRITY_ERROR(S) ::musx::util::Logger::log(::musx::util::Logger::LogLevel::Warning, (S))
#endif

#include "musx/xml/XmlInterface.h"

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

using Cmper = uint16_t;             ///< Enigma "comperator" key type
using Inci = int16_t;               ///< Enigma "incidend" key type
using Evpu = int32_t;               ///< EVPU value (288 per inch)
using EvpuFloat = double;           ///< EVPU fractional value (288.0 per inch)
using Evpu16ths = int32_t;          ///< 1/16 of an EVPU.
using Efix = int32_t;               ///< EFIX value (64 per EVPU, 64*288=18432 per inch)
using Edu = int32_t;                ///< "Elapsed Durational Units" value (1024 per quarter note)

using MeasCmper = int16_t;          ///< Enigma meas Cmper (may be negative when not applicable)
using InstCmper = int16_t;          ///< Enigma staff (inst) Cmper (may be negative when not applicable)
using SystemCmper = int16_t;        ///< Enigma systems Cmper (may be negative when not applicable)
using ClefIndex = int16_t;          ///< Index into @ref options::ClefOptions::clefDefs.
using EntryNumber = int32_t;        ///< Entry identifier.
using NoteNumber = int16_t;         ///< Note identifier.
using LayerIndex = unsigned int;    ///< Layer index (valid values are 0..3)

constexpr Cmper MUSX_GLOBALS_CMPER = 65534; ///< The prefs cmper for global variables (used sparingly since Finale 26.2)
constexpr int MAX_LAYERS = 4;       ///< The maximum number of music layers in a Finale document.
constexpr Cmper SCORE_PARTID = 0;   ///< The part id of the score.

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

    /**
     * @brief Allows a class to determine if it has been properly contructed by the factory.
     *
     * The defauly implementation does nothing.
     *
     * @throws #musx::dom::integrity_error if there is a problem.
     */
    virtual void integrityCheck() const { }

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
        : Base(document, SCORE_PARTID, ShareMode::All) {}

    /**
     * @brief Get the name of the font.
     * @return The name of the font as a string.
     */
    std::string getName() const;

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

    static const xml::XmlElementArray<FontInfo> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

namespace others {

// The following classes are defined here because they are shared by multiple subclasses and container classes.

/**
 * @class Enclosure
 * @brief Represents the enclosure settings for text expressions.
 */
class Enclosure : public OthersBase {
public:
    /**
     * @enum Shape
     * @brief Enumeration for enclosure shapes.
     */
    enum class Shape : uint8_t
    {
        NoEnclosure = 0,    ///< No enclosure
        Rectangle = 1,      ///< Rectangle
        Ellipse = 2,        ///< Ellipse
        Triangle = 3,       ///< Triangle
        Diamond = 4,        ///< Diamond
        Pentagon = 5,       ///< Pentagon
        Hexagon = 6,        ///< Hexagon
        Heptagon = 7,       ///< Heptagon
        Octogon = 8         ///< Octogon
    };

    /**
     * @brief Constructs an Enclosure object.
     * @param document Shared pointer to the document.
     * @param partId Usually 0. This parameter is needed for the generic factory routine.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
     * @param cmper Comperator parameter. This value is zero for enclosures taken from @ref MeasureNumberRegion.
     */
    explicit Enclosure(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All, Cmper cmper = 0)
        : OthersBase(document, partId, shareMode, cmper) {}

    Evpu xAdd{};              ///< Center X offset - offsets text from center (in EVPU).
    Evpu yAdd{};              ///< Center Y offset - offsets text from center (in EVPU).
    Evpu xMargin{};           ///< Half width - extra space on left/right sides (in EVPU).
    Evpu yMargin{};           ///< Half height - extra space on top/bottom sides (in EVPU).
    Efix lineWidth{};         ///< Line thickness in 64ths of an EVPU (EFIX).
    Shape shape{};            ///< Enclosure shape (default: NoEnclosure).
    Efix cornerRadius{};      ///< Corner radius (in EFIX).
    bool fixedSize{};         ///< Whether the enclosure is fixed size (ignore text bounding box)
    bool equalAspect{};       ///< "Match Height and Width"
    bool notTall{};           ///< "Enforce Minimum Width": don't let shape get taller than it is wide
    bool opaque{};            ///< Whether the enclosure is opaque.
    bool roundCorners{};      ///< Whether the enclosure has rounded corners.

    static const xml::XmlElementArray<Enclosure> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 *
 * @class MusicRange
 * @brief Represents a range of music using measure and EDUs.
 * 
 * This class is used to specify start and end points in a musical range using measures and EDUs.
 */
class MusicRange : public OthersBase
{
public:

    /**
     * @brief Constructs a MusicRange object.
     * @param document Shared pointer to the document.
     * @param partId The part ID if this range is unlinked, otherwise 0.
     * @param shareMode The share mode if this range is unlinked.
     * @param cmper Comperator parameter. This value is zero for ranges taken from @ref others::InstrumentUsed.
     */
    explicit MusicRange(const DocumentWeakPtr& document, Cmper partId = SCORE_PARTID, ShareMode shareMode = ShareMode::All, Cmper cmper = 0)
        : OthersBase(document, partId, shareMode, cmper) {}

    MeasCmper startMeas{};      ///< Starting measure in the range.
    Edu startEdu{};             ///< Starting EDU (Elapsed Durational Unit) in the range.
    MeasCmper endMeas{};        ///< Ending measure in the range.
    Edu endEdu{};               ///< Ending EDU (Elapsed Durational Unit) in the range.

    static const xml::XmlElementArray<MusicRange> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class NamePositioning
 * @brief Contains horizontal and vertical offsets, alignment, and expansion settings for name positioning.
 * 
 * This class is used both for default names as well as name positioning @ref Staff, @ref StaffStyle,
 * and @ref StaffGroup.
 */
class NamePositioning : public OthersBase
{
public:

    /**
     * @brief Constructs an NamePositioning object.
     * @param document Shared pointer to the document.
     * @param partId The part ID if this name positioning is unlinked.
     * @param shareMode The share mode if this name positioning is unlinked.
     * @param cmper Comperator parameter. This value is zero for name positioning taken from @ref options::StaffOptions.
     */
    explicit NamePositioning(const DocumentWeakPtr& document, Cmper partId = SCORE_PARTID, ShareMode shareMode = ShareMode::All, Cmper cmper = 0)
        : OthersBase(document, partId, shareMode, cmper) {}

    /// @brief Alignment and justification options for staff and group names.
    enum class AlignJustify
    {
        Left,   ///< Left alignment or justification (the default value.)
        Right,  ///< Right alignment.
        Center  ///< Center alignment.
    };
    
    Evpu horzOff{};             ///< Horizontal distance from staff in @ref Evpu.
    Evpu vertOff{};             ///< Vertical offset from staff in @ref Evpu.
    AlignJustify justify{};     ///< Justification for the name text.
    bool indivPos{};            ///< Indicates that this positioning overrides the default positioning. (Not used by @ref options::StaffOptions.)
    AlignJustify hAlign{};      ///< Horizontal alignment for the name text. (xml node is `<halign>`)
    bool expand{};              ///< "Expand Single Word"

    static const xml::XmlElementArray<NamePositioning> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace musx
