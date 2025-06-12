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

#include <numeric>
#include <filesystem>
#include <array>

#include "musx/util/Fraction.h"
#include "BaseClasses.h"

namespace music_theory {
class Transposer;
enum class DiatonicMode : int;
} // namespace music_theory

namespace musx {
namespace dom {

namespace details {
class IndependentStaffDetails; // forward delcaration
} // namespace details

namespace others {
    class Measure;  // forward declaration
    class Staff;    // forward declaration
} // namespace others

// This file contains common classes that are shared among Options, Others, and Details.

/**
 * @brief Enum class representing note types based on EDU values.
 *
 * The values are expressed in hexadecimal.
 */
enum class NoteType : Edu
{
    Maxima = 0x8000,
    Longa = 0x4000,
    Breve = 0x2000,
    Whole = 0x1000,
    Half = 0x0800,
    Quarter = 0x0400,
    Eighth = 0x0200,
    Note16th = 0x0100,
    Note32nd = 0x0080,
    Note64th = 0x0040,
    Note128th = 0x0020,
    Note256th = 0x0010,
    Note512th = 0x0008,
    Note1024th = 0x0004,
    Note2048th = 0x0002,
    Note4096th = 0x0001
};

/// @brief Clef types used by default in Finale documents. The values correspond to indices into
/// @ref musx::dom::options::ClefOptions::clefDefs.
/// Note that the user may change the clef list to any clefs the user wishes. These are only defaults, but
/// they are frequently left unchanged.
enum class DefaultClefType : ClefIndex
{
    Treble          = 0,  ///< G clef, standard treble.
    Alto            = 1,  ///< C clef, centered on third line (Alto clef).
    Tenor           = 2,  ///< C clef, centered on fourth line (Tenor clef).
    Bass            = 3,  ///< F clef, standard bass.
    Percussion      = 4,  ///< Percussion clef, open rectangle (no pitch).
    Treble8vb       = 5,  ///< G clef, sounds one octave lower (8vb).
    Bass8vb         = 6,  ///< F clef, sounds one octave lower (8vb).
    Baritone        = 7,  ///< F clef on third line (Baritone clef).
    FrenchViolin    = 8,  ///< G clef placed on first line (French violin clef).
    BaritoneC       = 9,  ///< C clef on fifth line (Baritone clef).
    MezzoSoprano    = 10, ///< C clef on second line (Mezzo-soprano clef).
    Soprano         = 11, ///< C clef on first line (Soprano clef).
    AltPercussion   = 12, ///< Alternate percussion clef, heavy vertical hash marks (no pitch).
    Treble8va       = 13, ///< G clef, sounds one octave higher (8va).
    Bass8va         = 14, ///< F clef, sounds one octave higher (8va).
    Blank           = 15, ///< Blank clef (invisible, no symbol).
    Tab1            = 16, ///< Tablature clef (5 lines).
    Tab2            = 17  ///< Tablature clef (5 lines, alternative style).
};

/**
 * @brief Enum representing the clef display mode for a frame.
 */
enum class ShowClefMode
{
    WhenNeeded, ///< Clef is displayed only when needed (the default).
    Never,      ///< Clef is never displayed. (xml value is "hidden")
    Always      ///< Clef is always displayed. (xml value is "forced")
};

/**
 * @struct FontInfo
 * @brief Represents the default font settings for a particular element type.
 *
 * The FontInfo struct holds information about font properties, such as the font ID, size, and styles like
 * bold, italic, underline, strikeout, fixed size, and visibility.
 */
class FontInfo : public CommonClassBase
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

    using CommonClassBase::CommonClassBase;

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

    /// @brief Calculates if this is the default music font.
    bool calcIsDefaultMusic() const
    { return fontId == 0; }

    /// @brief Calculates if this is a symbol font. (See #others::FontDefinition::calcIsSymbolFont.)
    bool calcIsSymbolFont() const;

    /// @brief Returns the filepath of the SMuFL font's metadata json file, if any
    std::optional<std::filesystem::path> calcSMuFLMetaDataPath() const;

    /**
     * @brief Calculates whether this is a SMuFL font.
     */
    bool calcIsSMuFL() const  { return calcSMuFLMetaDataPath().has_value(); }

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
 * @brief Shared key signature class that is contained in other classes. (See @ref others::Measure)
 */
class KeySignature : public CommonClassBase
{
private:
    std::vector<unsigned> calcTonalCenterArrayForSharps() const;
    std::vector<unsigned> calcTonalCenterArrayForFlats() const;
    std::vector<unsigned> calcTonalCenterArray() const;
    std::vector<int> calcAcciAmountsArray() const;
    std::vector<unsigned> calcAcciOrderArray() const;

    int m_octaveDisplacement{};         ///< Displace notes by this many octaves (for transposed keys)
    int m_alterationOffset{};           ///< Offset of tonal center (for transposed keys)

public:
    using CommonClassBase::CommonClassBase;

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

    /// @brief For linear keys, returns the number of sharps or flats from -7..7 in key-transposed pitch (if any).
    /// @return Number of sharps/flats for linear keys or 0 for non-linear or invalid keys
    int getAlteration() const { return getConcertAlteration() + m_alterationOffset; }

    /// @brief For linear keys, returns the number of sharps or flats from -7..7 in concert pitch.
    /// @return Number of sharps/flats for linear keys or 0 for non-linear or invalid keys
    int getConcertAlteration() const { return isLinear() ? int(int8_t(key & 0xff)) : 0; }

    bool isLinear() const { return (key & 0xC000) == 0; }                   ///< whether this is a linear key
    bool isNonLinear() const { return (key & 0xC000) != 0; }                ///< whether this is a non-linear key
    bool isBuiltIn() const { return isLinear() && getKeyMode() <= 1; }      ///< whether this is a built-in key
    bool isMajor() const { return getKeyMode() == 0; }                      ///< whether this is a built-in major key
    bool isMinor() const { return getKeyMode() == 1; }                      ///< whether this is a built-in minor key

    /// @brief If this key specifies a diatonic mode, returns the mode. This value is independent of EDO divisions.
    /// Any correctly configured linear key signature should return a value.
    /// @return The diatonic mode or std::nullopt if it is not one.
    std::optional<music_theory::DiatonicMode> calcDiatonicMode() const;

    /// @brief returns whether the two key signatures represent the same key signature, taking into account transposition.
    bool isSame(const KeySignature& src)
    {
        return isSameConcert(src) && m_alterationOffset == src.m_alterationOffset && m_octaveDisplacement == src.m_octaveDisplacement;
    }

    /// @brief returns whether the two key signatures represent the same concert key signature, ignoring transposition.
    bool isSameConcert(const KeySignature& src)
    {
        return key == src.key && keyless == src.keyless && hideKeySigShowAccis == src.hideKeySigShowAccis;
    }

    /**
     * @brief Transposes the key by the specified amounts. Set them to zero to remove transposition.
     *
     * This is used to adjust the key signature for staves that use Key Signature transposition.
     * Staves that use Chromatic transposition do not transpose the key. They transpose the pitches directly.
     *
     * Finale works pretty well with key signature transposition in microtone scales. However, it does not
     * simplify key signatures usefully. This function simplifies them correctly, provided that the step
     * values for accidentals are set to cycle through sharps and flats by chromatic half-steps rather than
     * the default of 1.
     * See @ref others::AcciAmountSharps and @ref others::AcciAmountFlats.
     *
     * @param interval The interval by which to transpose.
     * @param keyAdjustment The key adjustment (positive for sharps, negative for flats)
     * @param simplify If true, enharmonically adjust the key to have 6 or fewer accidentals
     */
    void setTransposition(int interval, int keyAdjustment, bool simplify);

    /// @brief Sets the key's transposition based on the input staff's transposition settings.
    void setTransposition(const std::shared_ptr<const others::Staff>& staff);

    /// @brief Calculates the tonal center index for the key, where C=0, D=1, E=2, ...
    /// 
    /// This is the modal tonal center, so a minor key with no sharps or flats returns 5 (=A).
    int calcTonalCenterIndex() const;

    /// @brief The octave displacement if this key is a transposed key.
    /// @return 0 for non-transposing keys or the octave displacement for transposed keys.
    int getOctaveDisplacement() const
    { return m_octaveDisplacement; }

    /// @brief Calculates the amount of alteration on a note int the key.
    /// @param noteIndex note index, where C=0, D=1, E=3, F=3, G=4, A=5, B=6
    int calcAlterationOnNote(unsigned noteIndex) const;

    /// @brief Calculates the number of EDO division for the key. (The standard value is 12.)
    int calcEDODivisions() const;

    /// @brief Calculates the key's diatonic key map
    std::optional<std::vector<int>> calcKeyMap() const;

    /// @brief Creates a transposer for this KeySignature instance.
    /// @param displacement Displacement value (e.g., from @ref Note)
    /// @param alteration Alteration value (e.g., from @ref Note)
    /// @return A unique pointer to a transposer for this key.
    std::unique_ptr<music_theory::Transposer> createTransposer(int displacement, int alteration) const;

    void integrityCheck() override
    {
        Base::integrityCheck();
        if (key >= 0x8000) {
            MUSX_INTEGRITY_ERROR("Key signature has invalid key value: " + std::to_string(key));
        }
    }

    static const xml::XmlElementArray<KeySignature>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

namespace texts {
class LyricsTextBase; // forward delcaration
} // namespace texts

/**
 * @class LyricsSyllableInfo
 * @brief Contains the syllable information for a single syllable. (See @ref texts::LyricsTextBase)
 */
class LyricsSyllableInfo : CommonClassBase
{
public:

    std::string syllable;       ///< the syllable text with no hyphenation or font information.
    bool hasHyphenBefore;       ///< indicates the syllable is preceded by a hyphen.
    bool hasHyphenAfter;        ///< indicates the syllable if followed by a hyphen.

private:
    /// @brief Constructor function
    /// @param document Shared pointer to the document.
    /// @param text The syllable text.
    /// @param before Whether there is a hyphen before the syllable.
    /// @param after Whether there is a hyphen after the syllable.
    LyricsSyllableInfo(const DocumentWeakPtr& document, const std::string text, bool before, bool after)
        : CommonClassBase(document), syllable(text), hasHyphenBefore(before), hasHyphenAfter(after)
    {
    }

    friend class texts::LyricsTextBase;
};
    
/**
 * @class TimeSignature
 * @brief Shared time signature class that is derived from other classes. (See @ref others::Measure)
 */
class TimeSignature : public CommonClassBase
{
public:

    /// @brief A single time signature component
    struct TimeSigComponent
    {
        std::vector<util::Fraction> counts; ///< The array of counts in this component.
                                            ///< For simple time signatures this is a single value, and it is an integer like 4/1.
        std::vector<Edu> units;             ///< The array of beat values in this component.
                                            ///< For simple time signatures this is a single value, and it is a note value like 1024 (quarter note)
                                            ///< For straightforward compound time it may be a single dotted value like 1536 (dotted quarter)

        /// @brief Test if two TimeSigComponent values are the same.
        bool operator==(const TimeSigComponent& src) const
        { return counts == src.counts && units == src.units; }

        /// @brief Compute the sum of all counts.
        util::Fraction sumCounts() const
        { return std::accumulate(counts.begin(), counts.end(), util::Fraction{}); }

        /// @brief Compute the sum of all units.
        Edu sumUnits() const
        { return std::accumulate(units.begin(), units.end(), Edu{}); }
    };

    std::vector<TimeSigComponent> components;     ///< the components in the time signature

    /// @brief Calculates the simplest form of of this time signature, expressed as a fractional count of @ref NoteType units.
    ///
    /// In typical cases, the returned @ref util::Fraction has a denominator of 1, but Finale supports other kinds of fractions.
    /// Use #util::Fraction::quotient to get the integer value and #util::Fraction::remainder to get the residual fractional component.
    std::pair<util::Fraction, NoteType> calcSimplified() const;

    /// @brief Calculates the total duration of the time signature as a fraction of a whole note.
    util::Fraction calcTotalDuration() const
    {
        util::Fraction result = std::accumulate(components.begin(), components.end(), util::Fraction{},
            [](const util::Fraction& acc, const TimeSigComponent& comp)
            { return acc + (comp.sumCounts() * comp.sumUnits()); }
        );
        return result / Edu(NoteType::Whole);
    }

    /// @brief returns whether the two time signatures represent the same time signature
    bool isSame(const TimeSignature& src)
    {
        return components == src.components && m_abbreviate == src.m_abbreviate;
    }

    /// @brief Creates a time signature corresponding to the component at @p index
    /// @param index The 0-based component index
    /// @throw std::invalid_argument if @p index is out of range
    std::shared_ptr<TimeSignature> createComponent(size_t index) const
    {
        checkIndex(index);
        return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), components[index], m_abbreviate));
    }

    /// @brief Returns the abbreviated symbol (code point) for this time signature, or std::nullopt if none.
    ///
    /// If the musx document lacks music symbol options but abbreviation was requested, the SMuFL values
    /// are returned as default substitute values.
    std::optional<char32_t> getAbbreviatedSymbol() const;

    /// @brief Returns if this time signature is common time
    bool isCommonTime() const;
    /// @brief Returns if this time signature is cut time
    bool isCutTime() const;

private:
    void checkIndex(size_t index) const
    {
        if (index > components.size()) {
            throw std::invalid_argument("Index out of range. The time signature has " + std::to_string(components.size())
                + " elements. The index requested was " + std::to_string(index) + ".");
        }
    }

    /**
     * @brief Constructor for measures and independent time signature records.
     * @param measure An instance of 
     */
    explicit TimeSignature(const DocumentWeakPtr& document, int beats, Edu unit, bool hasCompositeTop, bool hasCompositeBottom,
        std::optional<bool> abbreviate = std::nullopt);

    /**
     * @brief Constructor for components
     * @param measure An instance of 
     */
    explicit TimeSignature(const DocumentWeakPtr& document, const TimeSigComponent& timeSigUnit, std::optional<bool> abbreviate)
        : CommonClassBase(document), m_abbreviate(abbreviate)
    {
        components.push_back(timeSigUnit);
    }

    std::optional<bool> m_abbreviate;

    friend class others::Measure;
    friend class details::IndependentStaffDetails;
};

namespace others {

// The following classes are defined here because they are shared by multiple subclasses and container classes.

/**
 * @class Enclosure
 * @brief Represents the enclosure settings for text expressions.
 */
class Enclosure : public OthersBase
{
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
     * @param cmper Comperator parameter. This value is zero for enclosures taken from @ref others::MeasureNumberRegion.
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

    /// @brief Returns the next metric location following the music range.
    /// @param forStaff If provided, calculates the next metric location using staff-level Edus.
    /// @return An optional std::pair containing
    ///         - MeasCmper: the measure of the next location
    ///         - Edu: the location within the measure of the next location
    ///         Return std::nullopt if the next location is past the end of the document, or other error.
    std::optional<std::pair<MeasCmper, Edu>> nextLocation(const std::optional<InstCmper>& forStaff = std::nullopt) const;

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
