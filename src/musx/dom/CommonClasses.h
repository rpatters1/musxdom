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

#include "BaseClasses.h"

namespace musx {
namespace dom {

// This file contains common classes that are shared among Options, Others, and Details.

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

    static const xml::XmlElementArray<FontInfo>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class KeySignature
 * @brief Shared key signature class that is contained in other classes. (See @ref Measure)
 */
class KeySignature : public Base
{
public:
    /**
     * @brief Default constructor
     * @param document A weak pointer to the document object.
     */
    explicit KeySignature(const DocumentWeakPtr& document)
        : Base(document, SCORE_PARTID, ShareMode::All) {}

    /** @brief 16-bit value intepreted as follows:
     * - <b> Linear Keys </b>: top bit is 0, the next 7 bits define the key mode (see #getKeyMode),
     * bottom 8 bits is a signed byte (+/-7) giving the number of sharps or flats.
     * - <b> Nonlinear Keys </b>: Always have values from 0x4000-0x4fff. These are Cmper values to the records
     * that define the nonlinear key.
     */
    uint16_t key{};
    bool keyless{};                 ///< Indicates the absence of a key signature
    bool hideKeySigShowAccis{};     ///< Instead of a key signature, show accidentals for the key on the notes where they occur.

    /** @brief Returns the key mode.
     *
     * For linear keys, this is a value from 0..127 where 0 is reserved for built-in major keys and 1 is reserved for
     * built-in minor keys. Values from 2..127 are user-defined linear modes and are a @ref Cmper value for finding
     * the custom key signature information related to the key. User-defined linear keys might, for example, represent diatonic
     * jazz modes or church modes. They can also be used to define microtonal scales with key signatures.
     *
     * For non-linear keys, this is a value from 0x4000..0x4fff. This is a Cmper that is used to find all the custom key
     * signature information that defines the key. These are sometimes used to define modes like Klezmer Freyish mode, or
     * other modes whose accidentals do not follow the circle of fifths. They can also be used to define
     * microtonal scales without key signatures.
     *
     * Values from 0x8000 and higher are invalid.
     */
    Cmper getKeyMode() const { return isLinear() ? key >> 8 : key;  }

    /// @brief For linear keys, returns the number of sharps or flats from -7..7.
    /// @return Number of sharps/flats for linear keys or std::nullopt for non-linear or invalid keys
    std::optional<int> getAlteration() const { return isLinear() ? std::optional<int>(int8_t(key & 0xff)) : std::nullopt; }

    bool isLinear() const { return (key & 0xC000) == 0; }                   ///< returns if this is a linear key
    bool isNonLinear() const { return (key & 0xC000) != 0; }                ///< returns if this is a non-linear key
    bool isBuiltIn() const { return isLinear() && getKeyMode() <= 1; }      ///< returns if this is a built-in key
    bool isMajor() const { return getKeyMode() == 0; }                      ///< returns if this is a built-in major key
    bool isMinor() const { return getKeyMode() == 0; }                      ///< returns if this is a built-in minor key

    void integrityCheck() override
    {
        if (key >= 0x8000) {
            MUSX_INTEGRITY_ERROR("Key signature has invalid value: " + std::to_string(key));
        }
    }

    //static const xml::XmlElementArray<KeySignature>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
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

    static const xml::XmlElementArray<Enclosure>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
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
     * @param inci incident value, for subclasses that have them.
     */
    explicit MusicRange(const DocumentWeakPtr& document, Cmper partId = SCORE_PARTID, ShareMode shareMode = ShareMode::All,
            Cmper cmper = 0, std::optional<Inci> inci = std::nullopt)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    MeasCmper startMeas{};      ///< Starting measure in the range.
    Edu startEdu{};             ///< Starting EDU (Elapsed Durational Unit) in the range.
    MeasCmper endMeas{};        ///< Ending measure in the range.
    Edu endEdu{};               ///< Ending EDU (Elapsed Durational Unit) in the range.

    /// @brief Returns true of the given metric location is contained in this MusicRange instance.
    /// @param measId The measure ID to search for.
    /// @param eduPosition The Edu position within the measure to search for.
    bool contains(MeasCmper measId, Edu eduPosition) const
    {
        return (startMeas < measId || (startMeas == measId && startEdu <= eduPosition)) &&
               (endMeas > measId || (endMeas == measId && endEdu >= eduPosition));
    }

    static const xml::XmlElementArray<MusicRange>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class NamePositioning
 * @brief Contains horizontal and vertical offsets, alignment, and expansion settings for name positioning.
 * 
 * This class is used both for default names as well as name positioning @ref Staff, @ref StaffStyle,
 * and @ref details::StaffGroup.
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

    static const xml::XmlElementArray<NamePositioning>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace others
} // namespace dom
} // namespace mux
