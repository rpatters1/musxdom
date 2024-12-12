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
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {
namespace options {

/**
 * @class AlternateNotationOptions
 * @brief Options controlling the appearance of alternate notation.
 *
 * This class is identified by the XML node name "alternateNotationOptions".
 */
class AlternateNotationOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit AlternateNotationOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    Evpu halfSlashLift{};       ///< Baseline adjustment for half diamond in @ref Evpu.
    Evpu wholeSlashLift{};      ///< Baseline adjustment for whole diamond in @ref Evpu.
    Evpu dWholeSlashLift{};     ///< LBaseline adjustment for double whole diamond in @ref Evpu.
    Evpu halfSlashStemLift{};   ///< Stem connection value for half diamond in @ref Evpu.
    Evpu quartSlashStemLift{};  ///< Stem connection value for quarter slashes in @ref Evpu.
    Evpu quartSlashLift{};      ///< Baseline adjustment for quarter slashes in @ref Evpu.
    Evpu twoMeasNumLift{};      ///< Vertical offset for two-bar repeat numbers in @ref Evpu.

    constexpr static std::string_view XmlNodeName = "alternateNotationOptions"; ///< The XML node name for this type.
};

/**
 * @class BarlineOptions
 * @brief Options controlling the appearance of barlines.
 *
 * This class is identified by the XML node name "barlineOptions".
 */
class BarlineOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit BarlineOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    bool drawBarlines{};                    ///< "Display All Barlines"
    bool drawCloseSystemBarline{};          ///< "Close Barline at End of Each System"
    bool drawCloseFinalBarline{};           ///< "Close Barline at End of Piece"
    bool drawFinalBarlineOnLastMeas{};      ///< "Final Barline at End of Piece"
    bool drawDoubleBarlineBeforeKeyChanges{}; ///< "Double Barline Before Key Changes"
    bool drawLeftBarlineSingleStaff{};      ///< Left Barlines: "Display on Single Staves"
    bool drawLeftBarlineMultipleStaves{};   ///< Left Barlines: "Display on Multiple Staves"
    bool leftBarlineUsePrevStyle{};         ///< Left Barlines: "Default Style is Previous Measure's Right Barline"
    Efix thickBarlineWidth{};               ///< Width of thick barlines in @ref Efix.
    Efix barlineWidth{};                    ///< Width of thin barlines in @ref Efix.
    Efix doubleBarlineSpace{};              ///< Space between double barlines in @ref Efix.
    Efix finalBarlineSpace{};               ///< Space between final barlines in @ref Efix.
    Evpu barlineDashOn{};                   ///< Dash length for dashed barlines in @ref Evpu.
    Evpu barlineDashOff{};                  ///< Dash space for dashed barlines in @ref Evpu.

    constexpr static std::string_view XmlNodeName = "barlineOptions"; ///< The XML node name for this type.
};

/**
 * @class ClefOptions
 * @brief Options for configuring clefs in a musical context.
 */
class ClefOptions : public OptionsBase
{
public:
    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     *
     * Constructs a FontOptions object that is associated with the provided document.
     */
    explicit ClefOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    /**
     * @brief Represents a single clef definition.
     */
    struct ClefDef
    {
        int middleCPos{};               ///< Staff position of middle-C for this clef (from top staffline). (xml node is `<adjust>`.)
        char32_t clefChar{};            ///< UTF-32 character code for the clef symbol.
        int staffPositon{};             ///< Staff position of the clef symbol's baseline (from top staffline). (xml node is `<clefYDisp>`)
        Efix baselineAdjust{};          ///< Additional baseline adjustment in Efix. (xml node is `<baseAdjust>`)
        int shapeId{};                  ///< Shape ID if the clef is represented as a shape. (xml node is `<shapeID>`)
        bool isShape{};                 ///< Indicates if the clef is a shape.
        bool scaleToStaffHeight{};      ///< Indicates if the shape should scale to staff height.
        bool useOwnFont{};              ///< Indicates if the clef has its own font.
        std::shared_ptr<FontInfo> font; ///< When `useOwnFont` is true, this is the clef's font. Otherwise `nullptr`.
    };

    int defaultClef{};                  ///< Default clef identifier. An index into `clefDefs`.
    int clefChangePercent{};            ///< Percentage adjustment for end-of-measure clef changes. 100 means no adjustment. (xml node is `<endMeasClefPercent>`)
    Evpu clefChangeOffset{};            ///< Offset from next barline for end-of-measure clef changes. (xml node is `<endMeasClefPosAdd>`).
    Evpu clefFrontSepar{};              ///< Spacing before clef. (xml node is `<clefFront>`)
    Evpu clefBackSepar{};               ///< Spacing after clef. (xml node is `<clefBack>`)
    bool showClefFirstSystemOnly{};     ///< "Display Clef Only on First System"
    Evpu clefKeySepar{};                ///< "Extra Space Between Clef and Key Signature" (xml node is `<clefKey>`)
    Evpu clefTimeSepar{};               ///< "Extra Space Between Clef and Time Signature" (xml node is `<clefTime>`)
    bool cautionaryClefChanges{};       ///< "Display Courtesy Clef a End of Staff System"

    /**
     * @brief Vector of clef definitions (@ref ClefDef).
     */
    std::vector<ClefDef> clefDefs{18};  // reserve space for 18 clef definitions, 0..17, which is the norm for Finale

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "clefOptions";
};

/**
 * @class FlagOptions
 * @brief Options controlling the appearance and adjustments of flags.
 *
 * This class is identified by the XML node name "flagOptions".
 */
class FlagOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit FlagOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    bool straightFlags{};                   ///< Whether to use straight flags.
    Efix upHAdj{};                          ///< Horizontal adjustment for upward flags in @ref Efix.
    Efix downHAdj{};                        ///< Horizontal adjustment for downward flags in @ref Efix.
    Efix upHAdj2{};                         ///< Horizontal adjustment for secondary upward flags in @ref Efix.
    Efix downHAdj2{};                       ///< Horizontal adjustment for secondary downward flags in @ref Efix.
    Efix upHAdj16{};                        ///< Horizontal adjustment for sixteenth upward flags in @ref Efix.
    Efix downHAdj16{};                      ///< Horizontal adjustment for sixteenth downward flags in @ref Efix.
    Efix eighthFlagHoist{};                 ///< Hoist value for eighth note flags in @ref Efix. (This value is not on Finale's UI page for Flag Options.)
    Efix stUpHAdj{};                        ///< Horizontal adjustment for straight upward flags in @ref Efix.
    Efix stDownHAdj{};                      ///< Horizontal adjustment for straight downward flags in @ref Efix.
    Efix upVAdj{};                          ///< Vertical adjustment for upward flags in @ref Efix.
    Efix downVAdj{};                        ///< Vertical adjustment for downward flags in @ref Efix.
    Efix upVAdj2{};                         ///< Vertical adjustment for secondary upward flags in @ref Efix. (xml node is `<upVadj2>`: this is probably a type somewhere in Finale)
    Efix downVAdj2{};                       ///< Vertical adjustment for secondary downward flags in @ref Efix.
    Efix upVAdj16{};                        ///< Vertical adjustment for sixteenth upward flags in @ref Efix.
    Efix downVAdj16{};                      ///< Vertical adjustment for sixteenth downward flags in @ref Efix.
    Efix stUpVAdj{};                        ///< Vertical adjustment for straight upward flags in @ref Efix.
    Efix stDownVAdj{};                      ///< Vertical adjustment for straight downward flags in @ref Efix.
    Evpu flagSpacing{};                     ///< "Flag Spacing" in @ref Evpu. (xml node is `<separ>`)
    Evpu secondaryGroupAdj{};               ///< "Secondary Group Adjust" in @ref Evpu. (xml node is `<extra>`)

    constexpr static std::string_view XmlNodeName = "flagOptions"; ///< The XML node name for this type.
};

/**
 * @class FontOptions
 * @brief An unordered map of default font settings for the document.
 *
 * The FontOptions class stores default font settings used throughout the document. It provides functionality
 * to manage various font attributes, such as size, style (bold, italic), and whether the font is hidden or fixed size.
 */
class FontOptions : public OptionsBase
{
public:
    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     *
     * Constructs a FontOptions object that is associated with the provided document.
     */
    explicit FontOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    /**
     * @enum FontType
     * @brief Types of fonts available for the document.
     *
     * The FontType enum represents different types of fonts used for musical and textual elements within the document.
     */
    enum class FontType
    {
        Music,             ///< Default music font.
        Key,               ///< Key signature font.
        Clef,              ///< Clef symbol font.
        Time,              ///< Time signature font.
        Chord,             ///< Chord name font.
        ChordAcci,         ///< Chord accidental font.
        Ending,            ///< Ending font.
        Tuplet,            ///< Tuplet font.
        TextBlock,         ///< Text block font.
        LyricVerse,        ///< Lyric verse font.
        LyricChorus,       ///< Lyric chorus font.
        LyricSection,      ///< Lyric section font.
        MultiMeasRest,     ///< Multi-measure rest font.
        Tablature,         ///< Tablature font.
        ChordSuffix,       ///< Chord suffix font.
        Expression,        ///< Expression font.
        Repeat,            ///< Repeat symbol font.
        Fretboard,         ///< Fretboard font.
        Flags,             ///< Flags font.
        Accis,             ///< Accidental symbols font.
        AltNotSlash,       ///< Alternate notation slash font.
        AltNotNum,         ///< Alternate notation number font.
        Rests,             ///< Rests notation font.
        ReptDots,          ///< Repeat dots font.
        Noteheads,         ///< Notehead symbols font.
        AugDots,           ///< Augmentation dots font.
        TimePlus,          ///< Additional time signature symbols font.
        Articulation,      ///< Articulation symbols font.
        Percussion,        ///< Percussion font.
        SmartShape8va,     ///< 8va smart shape font.
        MeasNumb,          ///< Measure number font.
        StaffNames,        ///< Staff names font.
        AbbrvStaffNames,   ///< Abbreviated staff names font.
        GroupNames,        ///< Group names font.
        SmartShape8vb,     ///< 8vb smart shape font.
        SmartShape15ma,    ///< 15ma smart shape font.
        SmartShape15mb,    ///< 15mb smart shape font.
        SmartShapeTrill,   ///< Trill smart shape font.
        SmartShapeWiggle,  ///< Wiggle smart shape font.
        AbbrvGroupNames,   ///< Abbreviated group names font.
        BendCurveFull,     ///< Full bend curve font.
        BendCurveWhole,    ///< Whole bend curve font.
        BendCurveFrac,     ///< Fractional bend curve font.
        TimeParts,         ///< Time signature parts font.
        TimePlusParts      ///< Additional time signature parts font.
    };

    /**
     * @brief Stores the default fonts for different elements.
     *
     * An unordered map that associates each `FontType` with its corresponding `FontInfo` settings.
     */
    std::unordered_map<FontType, std::shared_ptr<FontInfo>> fontOptions;

    /**
     * @brief get the `FontInfo` for a particular type
     * @param type the `FontType` to retrieve
     * @return a shared pointer to the font info for that type
     * @throws std::invalid_paremter if the type is not found in the document
     */
    std::shared_ptr<FontInfo> getFontInfo(FontType type) const;

    /**
     * @brief get the `FontInfo` for a particular type from the document pool
     * @param document the `Document` to search
     * @param type the `FontType` to retrieve
     * @return a shared pointer to the font info for that type
     * @throws std::invalid_paremter if the type is not found in the document
     */
    static std::shared_ptr<FontInfo> getFontInfo(const DocumentPtr& document, FontType type);

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "fontOptions";
};

/**
 * @class GraceNoteOptions
 * @brief Options controlling the appearance and behavior of grace notes.
 *
 * This class is identified by the XML node name "graceNoteOptions".
 */
class GraceNoteOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit GraceNoteOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    int tabGracePerc{};              ///< "Tablature Grace Note Size" (%)
    int gracePerc{};                 ///< "Grace Note Size" (%)
    Edu playbackDuration{};          ///< Playback duration for grace notes in @ref Edu.
    Evpu entryOffset{};              ///< "Grace Note Offset on Entry" in @ref Evpu. (xml node is `<graceBackup>`)
    bool slashFlaggedGraceNotes{};   ///< "Always Slash Flagged Grace Notes"
    Efix graceSlashWidth{};          ///< Width (thickness) of grace note slashes in @ref Efix.

    constexpr static std::string_view XmlNodeName = "graceNoteOptions"; ///< The XML node name for this type.
};

/**
 * @class KeySignatureOptions
 * @brief Options controlling the appearance and behavior of key signatures.
 *
 * This class is identified by the XML node name "keySignatureOptions".
 */
class KeySignatureOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit KeySignatureOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    bool doKeyCancel{};                        ///< "Cancel Outgoing Key Signature"
    bool doCStart{};                           ///< "In C, Display Naturals at Start of Staff System"
    bool redisplayOnModeChange{};              ///< "Redisplay Key Signature If Only Mode is Changing" (xml node is `<doBankDiff>`)
    Evpu keyFront{};                           ///< "Space Before Key Signature"
    Evpu keyMid{};                             ///< "Space After Cancelled Key"
    Evpu keyBack{};                            ///< "Space After Key Signature"
    Evpu acciAdd{};                            ///< "Space Between Key Signature Accidentals"
    bool showKeyFirstSystemOnly{};             ///< "Show Key Signature Only on First System"
    Evpu keyTimeSepar{};                       ///< "Extra Space Between Key & Time Signatures" (xml node is `<keyTime>`)
    bool simplifyKeyHoldOctave{};              ///< "Preserve Octave When Simplifying Keys"
    bool cautionaryKeyChanges{};               ///< "Display Courtesy Key Signature at End of Staff System"
    bool doKeyCancelBetweenSharpsFlats{};      ///< "When Switching Between Sharps and Flats"

    constexpr static std::string_view XmlNodeName = "keySignatureOptions"; ///< The XML node name for this type.
};

/**
 * @class LineCurveOptions
 * @brief Options controlling the appearance of line and curve elements.
 *
 * This class is identified by the XML node name "lineCurveOptions".
 */
class LineCurveOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit LineCurveOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    int bezierStep{};                    ///< Number of steps for Bézier curves.
    Efix enclosureWidth{};               ///< Default enclosure width in @ref Efix.
    bool enclosureRoundCorners{};        ///< Whether default enclosures have rounded corners.
    Efix enclosureCornerRadius{};        ///< Default rounded corner radius width for enclosures in @ref Efix.
    Efix staffLineWidth{};               ///< Width of staff lines in @ref Efix.
    Efix legerLineWidth{};               ///< Width of leger lines in @ref Efix.
    Evpu legerFrontLength{};             ///< Front length of leger lines in @ref Evpu.
    Evpu legerBackLength{};              ///< Back length of leger lines in @ref Evpu.
    Evpu restLegerFrontLength{};         ///< Front length of rest leger lines in @ref Evpu.
    Evpu restLegerBackLength{};          ///< Back length of rest leger lines in @ref Evpu.
    double psUlDepth{};                  ///< Depth of PostScript underline in 72 dpi points (fractional).
    double psUlWidth{};                  ///< Width of PostScript underline in 72 dpi points (fractional).
    EvpuFloat pathSlurTipWidth{};        ///< Width of slur tip path in fractional @ref EvpuFloat.

    constexpr static std::string_view XmlNodeName = "lineCurveOptions"; ///< The XML node name for this type.
};

/**
 * @class MusicSpacingOptions
 * @brief Options controlling music spacing.
 *
 * This class is identified by the XML node name "musicSpacingOptions".
 */
class MusicSpacingOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit MusicSpacingOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}
 
    /// @brief Options for how to handle unisons
    enum class ColUnisonsChoice
    {
        None,           ///< Do not avoid collisions with unisons. (The default.)
        DiffNoteheads,  ///< Avoid collisions with unisons that have different noteheads.
        All             ///< Avoid collisions with all unisons.
    };

    /// @brief Options for how to handle grace note spacing
    enum class GraceNoteSpacing
    {
        ResetToEntry,   ///< Reset to the entry position. (The default.)
        Automatic,      ///< Automatically recompute grace note spacing. This activates `minDistGrace`. (xml value is "recomp")
        KeepCurrent     ///< Keep current grace note spacing. (xml value is "keep")
    };

    /// @brief Options for how to handle manual positioning
    enum class ManualPositioning
    {
        Clear,          ///< Clear manual positioning.
        Ignore,         ///< Ignore manual positioning.
        Incorporate     ///< Incorporate manual positioning. (xml value is "incorp")
    };

    Evpu minWidth{};                        ///< Minimum measure width in @ref Evpu.
    Evpu maxWidth{};                        ///< Maximum measure width in @ref Evpu.
    Evpu minDistance{};                     ///< Minimum distance between items in @ref Evpu.
    Evpu minDistTiedNotes{};                ///< Minimum distance between tied notes in @ref Evpu.
    bool avoidColNotes{};                   ///< Avoid collisions with notes.
    bool avoidColLyrics{};                  ///< Avoid collisions with lyrics.
    bool avoidColChords{};                  ///< Avoid collisions with chords.
    bool avoidColArtics{};                  ///< Avoid collisions with articulations.
    bool avoidColClefs{};                   ///< Avoid collisions with clefs.
    bool avoidColSeconds{};                 ///< Avoid collisions with seconds.
    bool avoidColStems{};                   ///< Avoid collisions with stems.
    ColUnisonsChoice avoidColUnisons{};     ///< Choice for how to handle collisions with unisons.
    bool avoidColLedgers{};                 ///< Avoid collisions with ledger lines.
    ManualPositioning manualPositioning{};  ///< Manual positioning choice.
    bool ignoreHidden{};                    ///< Ignore hidden notes. (This value is reversed in the Finale UI.)
    bool useAllottmentTables{};             ///< Use allotment tables for spacing.
    Edu referenceDuration{};                ///< Reference duration for spacing in @ref Edu.
    Evpu referenceWidth{};                  ///< Reference width for spacing in @ref Evpu.
    double scalingFactor{};                 ///< Scaling factor for spacing.
    Evpu defaultAllotment{};                ///< Default allotment for spacing in @ref Evpu. (This value may not be settable in the Finale UI.)
    Evpu minDistGrace{};                    ///< Minimum distance for grace notes in @ref Evpu.
    GraceNoteSpacing graceNoteSpacing{};    ///< Choice for how to handle grace note spacing. (xml node is `<graceNoteManualPositioning>`)
    Evpu musFront{};                        ///< Additional spacing before the first note in @ref Evpu. (From Notes & Rests window.)
    Evpu musBack{};                         ///< Additional spacing after the last note in @ref Evpu. (From Notes & Rests window.)

    constexpr static std::string_view XmlNodeName = "musicSpacingOptions"; ///< The XML node name for this type.
};

/**
 * @class PageFormatOptions
 * @brief Options for page formatting in the document.
 * 
 * This class provides configuration options for page formatting, including 
 * scope adjustment, page format score, and page format parts.
 */
class PageFormatOptions : public OptionsBase {
public:
    /**
     * @enum AdjustPageScope
     * @brief Enum for the scope of page adjustments.
     */
    enum class AdjustPageScope {
        Current,        ///< Adjust the current page only.
        All,            ///< Adjust all pages.
        LeftOrRight,    ///< Adjust left or right pages.
        PageRange       ///< Adjust page range. (xml value is "range")
    };

    /**
     * @class PageFormat
     * @brief Represents the format settings for a page.
     */
    class PageFormat
    {
    public:
        Evpu pageHeight{};               ///< Height of the page.
        Evpu pageWidth{};                ///< Width of the page.
        int pagePercent{};               ///< Page scaling percentage (a value of 100 means no scaling).
        int sysPercent{};                ///< System scaling percentage (a value of 100 means no scaling).
        int rawStaffHeight{};            ///< Raw staff height (in 1/16 Evpu units).
        Evpu leftPageMarginTop{};        ///< Top margin for the left page. (Sign reversed in Finale UI.)
        Evpu leftPageMarginLeft{};       ///< Left margin for the left page.
        Evpu leftPageMarginBottom{};     ///< Bottom margin for the left page.
        Evpu leftPageMarginRight{};      ///< Right margin for the left page. (Sign reversed in Finale UI.)
        Evpu rightPageMarginTop{};       ///< Top margin for the right page. (Sign reversed in Finale UI.)
        Evpu rightPageMarginLeft{};      ///< Left margin for the right page.
        Evpu rightPageMarginBottom{};    ///< Bottom margin for the right page.
        Evpu rightPageMarginRight{};     ///< Right margin for the right page. (Sign reversed in Finale UI.)
        Evpu sysMarginTop{};             ///< System top margin. (Sign reversed in Finale UI.)
        Evpu sysMarginLeft{};            ///< System left margin.
        Evpu sysMarginBottom{};          ///< System bottom margin.
        Evpu sysMarginRight{};           ///< System bottom margin. (Sign reversed in Finale UI.)
        Evpu sysDistanceBetween{};       ///< Distance between systems. (Sign reversed in Finale UI.)
        Evpu firstPageMarginTop{};       ///< Top margin for the first page. (Sign reversed in Finale UI.)
        Evpu firstSysMarginTop{};        ///< Top margin for the first system. (Sign reversed in Finale UI.)
        Evpu firstSysMarginLeft{};       ///< Left margin for the first system.
        Evpu firstSysMarginDistance{};   ///< Distance between the first systems. (Sign reversed in Finale UI.)
        bool facingPages{};              ///< Whether to use the right page margin values.
        bool differentFirstSysMargin{};  ///< Whether to use the first system values.
        bool differentFirstPageMargin{}; ///< Whether to use the `firstPageMarginTop` value.

        /**
         * @brief Default constructor for PageFormat.
         */
        PageFormat() = default;
    };

    // Properties
    AdjustPageScope adjustPageScope{};    ///< Scope of page adjustments.
    std::shared_ptr<PageFormat> pageFormatScore; ///< Page format for score settings.
    std::shared_ptr<PageFormat> pageFormatParts; ///< Page format for parts settings.
    bool avoidSystemMarginCollisions{};   ///< Whether to avoid system margin collisions.

    /**
     * @brief Constructor for PageFormatOptions.
     * 
     * @param document A shared pointer to the document.
     */
    explicit PageFormatOptions(const DocumentPtr& document)
        : OptionsBase(document) {}

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "pageFormatOptions";
};

/**
 * @class PianoBraceBracketOptions
 * @brief Options controlling the appearance of piano braces and brackets.
 *
 * This class is identified by the XML node name "pianoBraceBracketOptions".
 */
class PianoBraceBracketOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit PianoBraceBracketOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    Evpu defBracketPos{};                   ///< "Group Brackets: Default Distance from Left Edge of Staff"
    EvpuFloat centerThickness{};            ///< Center thickness of braces in fractional @ref EvpuFloat.
    EvpuFloat tipThickness{};               ///< End thickness of braces in fractional @ref EvpuFloat. (xml node is `<endThickness>`)
    EvpuFloat outerBodyV{};                 ///< Vertical position of the outer wing in fractional @ref EvpuFloat. (xml node is `<outerWingY>`)
    EvpuFloat innerTipV{};                  ///< Vertical position of the inner tip in fractional @ref EvpuFloat. (xml node is `<innerTipY>`)
    EvpuFloat innerBodyV{};                 ///< Vertical position of the inner body in fractional @ref EvpuFloat. (xml node is `<innerWingY>`)
    EvpuFloat outerTipH{};                  ///< Horizontal position of the outer tip in fractional @ref EvpuFloat. (xml node is `<outerTipX>`)
    EvpuFloat outerTipV{};                  ///< Vertical position of the outer tip in fractional @ref EvpuFloat. (xml node is `<outerTipY>`)
    EvpuFloat outerBodyH{};                 ///< Horizontal positione of the outer body in fractional @ref EvpuFloat. (xml node is `<outerWingX>`)
    EvpuFloat width{};                      ///< Width of braces in fractional @ref EvpuFloat.
    EvpuFloat innerTipH{};                  ///< Horizontal position of the inner tip in fractional @ref EvpuFloat. (xml node is `<innerTipX>`)
    EvpuFloat innerBodyH{};                 ///< Horizontal position of the inner body in fractional @ref EvpuFloat. (xml node is `<innerWingX>`)

    constexpr static std::string_view XmlNodeName = "pianoBraceBracketOptions"; ///< The XML node name for this type.
};

/**
 * @class RepeatOptions
 * @brief Options controlling the appearance of repeats.
 *
 * This class is identified by the XML node name "repeatOptions".
 */
class RepeatOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit RepeatOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    /// @brief Wing Styles from Document Options - Repeats
    enum class WingStyle
    {
        None,
        Curved,
        SingleLine,
        DoubleLine
    };

    /// @brief Back-to-Back Styles from Document Options - Repeats
    enum class BackToBackStyle
    {
        Thin,
        Mixed,
        Thick
    };

    int maxPasses{};                       ///< Maximum number of passes for repeats.
    bool addPeriod{};                      ///< "Add Period After Number"
    Efix thickLineWidth{};                 ///< Heavy line thickness in @ref Efix.
    Efix thinLineWidth{};                  ///< Thin line thickness in @ref Efix.
    Efix lineSpace{};                      ///< Space between lines in @ref Efix.
    BackToBackStyle backToBackStyle{};     ///< Back-to-back style.
    Evpu forwardDotHPos{};                 ///< Horizontal separation of forward repeat dots in @ref Evpu.
    Evpu backwardDotHPos{};                ///< Horizontal separation of backward repeat dots in @ref Evpu.
    Evpu upperDotVPos{};                   ///< Vertical adjustment of the upper dot in @ref Evpu.
    Evpu lowerDotVPos{};                   ///< Vertical adjustment of the lower dot in @ref Evpu.
    WingStyle wingStyle{};                 ///< Wing style.
    Evpu afterClefSpace{};                 ///< Space after clef in @ref Evpu.
    Evpu afterKeySpace{};                  ///< Space after key signature in @ref Evpu.
    Evpu afterTimeSpace{};                 ///< Space after time signature in @ref Evpu.
    Cmper showOnStaffListNumber{};         ///< Staff list number (0 if none). @ref Cmper links to associated "repeatStaffList..." xml nodes.
    Evpu bracketHeight{};                  ///< Height of the bracket in @ref Evpu.
    Evpu bracketHookLen{};                 ///< Length of the bracket hook in @ref Evpu.
    Efix bracketLineWidth{};               ///< Width of the bracket line in @ref Efix.
    Evpu bracketStartInset{};              ///< Inset at the start of the bracket in @ref Evpu.
    Evpu bracketEndInset{};                ///< Inset at the end of the bracket in @ref Evpu.
    Evpu bracketTextHPos{};                ///< Horizontal position of bracket text in @ref Evpu.
    Evpu bracketTextVPos{};                ///< Vertical position of bracket text in @ref Evpu.
    Evpu bracketEndHookLen{};              ///< Length of the hook at the end of the bracket in @ref Evpu.
    bool bracketEndAnchorThinLine{};       ///< Whether the end of the bracket anchors to a thin line.

    constexpr static std::string_view XmlNodeName = "repeatOptions"; ///< The XML node name for this type.
};

/**
 * @class StemOptions
 * @brief Options controlling the appearance of stems.
 *
 * This class is identified by the XML node name "stemOptions".
 */
class StemOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit StemOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    Evpu halfStemLength{};     ///< Half stem length in @ref Evpu.
    Evpu stemLength{};         ///< Stem length in @ref Evpu.
    Evpu shortStemLength{};    ///< Short stem length in @ref Evpu. (xml node is `<stem2>`)
    Evpu revStemAdj{};         ///< Reverse stem adjustment in @ref Evpu.
    bool noReverseStems{};     ///< Set if "Display Reverse Stemming" is unchecked. (This bit is reversed in the Finale UI.)
    Efix stemWidth{};          ///< Stem width in @ref Efix.
    Efix stemOffset{};         ///< Stem offset in @ref Efix. (xml node is `<stemLift>`)
    bool useStemConnections{}; ///< "Use Stem Connections"

    constexpr static std::string_view XmlNodeName = "stemOptions"; ///< The XML node name for this type.
};

/**
 * @class TimeSignatureOptions
 * @brief Options controlling the appearance and behavior of time signatures.
 *
 * This class is identified by the XML node name "timeSignatureOptions".
 */
class TimeSignatureOptions : public OptionsBase
{
public:
    /** @brief Constructor function */
    explicit TimeSignatureOptions(const DocumentWeakPtr& document)
        : OptionsBase(document) {}

    Evpu timeUpperLift{};           ///< "Vertical Adjustment: Top Symbol, Score"
    Evpu timeFront{};               ///< "Space Before Time Signature, Score"
    Evpu timeBack{};                ///< "Space After Time Signature, Score"
    Evpu timeFrontParts{};          ///< "Space Before Time Signature, Parts"
    Evpu timeBackParts{};           ///< "Space After Time Signature, Parts"
    Evpu timeUpperLiftParts{};      ///< "Vertical Adjustment: Top Symbol, Parts"
    Evpu timeLowerLiftParts{};      ///< "Vertical Adjustment: Bottom Symbol, Parts"
    Evpu timeAbrvLiftParts{};       ///< "Vertical Adjustment: Abbreviated Symbol, Parts"
    bool timeSigDoAbrvCommon{};     ///< Abbreviate common time.
    bool timeSigDoAbrvCut{};        ///< Abbreviate cut time.
    int numCompositeDecimalPlaces{};///< "Decimal Places for Composite Meters" (xml node is `<defFloat>`)
    bool cautionaryTimeChanges{};   ///< "Display Courtesy Time Signature at End of Staff System"
    Evpu timeLowerLift{};           ///< "Vertical Adjustment: Bottom Symbol, Score"
    Evpu timeAbrvLift{};            ///< "Vertical Adjustment: Abbreviated Symbol, Score"

    constexpr static std::string_view XmlNodeName = "timeSignatureOptions"; ///< The XML node name for this type.
};

} // namespace options
} // namespace dom
} // namespace musx