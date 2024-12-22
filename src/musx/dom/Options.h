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
 * @class AccidentalOptions
 * @brief Options controlling the appearance and positioning of accidentals.
 *
 * This class is identified by the XML node name "accidentalOptions".
 */
class AccidentalOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit AccidentalOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    int minOverlap{};                       ///< "Minimum Vertical Spacing Between Accidentals (Measured in Steps)" (xml node is `<overlap>`)
    Evpu multiCharSpace{};                  ///< "Space Within Multi-Character Accidentals" (xml node is `<bacciAdd>`)
    bool crossLayerPositioning{};           ///< "Use Cross-Layer Accidental Positioning" (xml node is `<useNewAcciPositioning>`)
    Evpu startMeasureSepar{};               ///< "Adjustment at Start of Measure"--The sign is reversed in the Finale UI. (xml node is `<frontAcciSepar>`)
    Evpu acciNoteSpace{};                   ///< Space between accidentals and notes.
    Evpu acciAcciSpace{};                   ///< Space between accidentals.

    constexpr static std::string_view XmlNodeName = "accidentalOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<AccidentalOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class AlternateNotationOptions
 * @brief Options controlling the appearance of alternate notation.
 *
 * This class is identified by the XML node name "alternateNotationOptions".
 */
class AlternateNotationOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit AlternateNotationOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    Evpu halfSlashLift{};       ///< Baseline adjustment for half diamond in @ref Evpu.
    Evpu wholeSlashLift{};      ///< Baseline adjustment for whole diamond in @ref Evpu.
    Evpu dWholeSlashLift{};     ///< LBaseline adjustment for double whole diamond in @ref Evpu.
    Evpu halfSlashStemLift{};   ///< Stem connection value for half diamond in @ref Evpu.
    Evpu quartSlashStemLift{};  ///< Stem connection value for quarter slashes in @ref Evpu.
    Evpu quartSlashLift{};      ///< Baseline adjustment for quarter slashes in @ref Evpu.
    Evpu twoMeasNumLift{};      ///< Vertical offset for two-bar repeat numbers in @ref Evpu.

    constexpr static std::string_view XmlNodeName = "alternateNotationOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<AlternateNotationOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class AugmentationDotOptions
 * @brief Options controlling the appearance of augmentation dots.
 *
 * This class is identified by the XML node name "augmentationDotOptions".
 */
class AugmentationDotOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit AugmentationDotOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    Evpu dotUpFlagOffset{};     ///< "Horizontal Adjustment for Upstem Flags"
    Evpu dotOffset{};           ///< "Space Between Dots"
    bool adjMultipleVoices{};   ///< "Adjust Dots for Multiple Voices" (xml node is `<doDotDownAdjust>`)
    Evpu dotNoteOffset{};      ///< "Space Between Dot and Note" (xml node is `<dotFirstOffset>`)
    Evpu dotLift{};             ///< "Vertical Adjustment of Dot"
    /**
     * @brief Use legacy dot positioning on flipped stems.
     *
     * Finale 27.4 fixed a long-standing bug in how dots on flipped stems are positioned. Finale added
     * this option to avoid messing up documents in which the user manually worked around the bug.
     * Enabling it suppresses the 27.4 behavior. It is not represented in the Finale UI, except as
     * a question when you first open a document that it detects might contain the workaround.
     *
     * xml node is `<skip27_4DotAdjust>`
     */
    bool useLegacyFlippedStemPositioning{};

    constexpr static std::string_view XmlNodeName = "augmentationDotOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<AugmentationDotOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class BarlineOptions
 * @brief Options controlling the appearance of barlines.
 *
 * This class is identified by the XML node name "barlineOptions".
 */
class BarlineOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit BarlineOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<BarlineOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class BeamOptions
 * @brief Options controlling the appearance of beams.
 *
 * This class is identified by the XML node name "beamOptions".
 */
class BeamOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit BeamOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    /// @brief Beaming style choices for when to flatten beams
    enum class FlattenStyle
    {
        OnEndNotes,         ///< Flatten based on end notes only (the legacy default)
        OnStandardNote,     ///< Flatten based on standard note
        OnExtremeNote,      ///< Flatten based on extreme note
        AlwaysFlat          ///< Flatten all beams
    };

    Evpu beamStubLength{};                   ///< "Broken Beam Length" in @ref Evpu.
    Evpu maxSlope{};                         ///< Maximum slope in @ref Evpu.
    Evpu beamSepar{};                        ///< Secondary beam separation in @ref Evpu. This is the distance between the top of one beam to the top of the next.
    Evpu maxFromMiddle{};                    ///< "Maximum Distance from Middle Staff Line" in @ref Evpu. (xml node is `<hmBeamTip>`)
    FlattenStyle beamingStyle{};             ///< Beaming style.
    bool extendBeamsOverRests{};             ///< "Extend Beams Over Rests" (xml node is `<incEdgeRestsInBeamGroups>`)
    bool incRestsInFourGroups{};             ///< "Include Rests when Beaming in Group of Four" (xml node is `<incRestsInClassicBeams>`)
    bool beamFourEighthsInCommonTime{};      ///< "Beam Four Eighth Notes Together in Common Time"
    bool beamThreeEighthsInCommonTime{};     ///< "Beam Three Eighth Notes Together Before/After An Eighth Rest"
    bool dispHalfStemsOnRests{};             ///< "Display Half-Stems for Beamed Rests" (xml node is `<doStemStubs>`)
    bool spanSpace{};                        ///< "Allow Primary Beam Within a Space"
    bool extendSecBeamsOverRests{};          ///< "Extend Secondary Beams Over Rests"
    Efix beamWidth{};                        ///< Thickness of beams in @ref Efix.

    constexpr static std::string_view XmlNodeName = "beamOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<BeamOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class ClefOptions
 * @brief Options for configuring clefs in a musical context.
 */
class ClefOptions : public OptionsBase {
public:
    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     * @param partId Usually 0. This parameter is needed for the generic factory routine.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
     *
     * Constructs a FontOptions object that is associated with the provided document.
     */
    explicit ClefOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    /**
     * @brief Represents a single clef definition.
     */
    class ClefDef : public Base
    {
    public:
        /** @brief the constructor */
        explicit ClefDef(const DocumentWeakPtr& document)
            : Base(document, 0, ShareMode::All) {}
            
        int middleCPos{};               ///< Staff position of middle-C for this clef (from top staffline). (xml node is `<adjust>`.)
        char32_t clefChar{};            ///< UTF-32 character code for the clef symbol.
        int staffPositon{};             ///< Staff position of the clef symbol's baseline (from top staffline). (xml node is `<clefYDisp>`)
        Efix baselineAdjust{};          ///< Additional baseline adjustment in Efix. (xml node is `<baseAdjust>`)
        int shapeId{};                  ///< Shape ID if the clef is represented as a shape. (xml node is `<shapeID>`)
        bool isShape{};                 ///< Indicates if the clef is a shape.
        bool scaleToStaffHeight{};      ///< Indicates if the shape should scale to staff height.
        bool useOwnFont{};              ///< Indicates if the clef has its own font.
        std::shared_ptr<FontInfo> font; ///< When `useOwnFont` is true, this is the clef's font. Otherwise `nullptr`.

        static const xml::XmlElementArray<ClefDef> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    std::vector<std::shared_ptr<ClefDef>> clefDefs;

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "clefOptions";
    static const xml::XmlElementArray<ClefOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FlagOptions
 * @brief Options controlling the appearance and adjustments of flags.
 *
 * This class is identified by the XML node name "flagOptions".
 */
class FlagOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit FlagOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<FlagOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class FontOptions
 * @brief An unordered map of default font settings for the document.
 *
 * The FontOptions class stores default font settings used throughout the document. It provides functionality
 * to manage various font attributes, such as size, style (bold, italic), and whether the font is hidden or fixed size.
 */
class FontOptions : public OptionsBase {
public:
    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     * @param partId Usually 0. This parameter is needed for the generic factory routine.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
     *
     * Constructs a FontOptions object that is associated with the provided document.
     */
    explicit FontOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
        Ending,            ///< Repeat ending number font.
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
    static const xml::XmlElementArray<FontOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class GraceNoteOptions
 * @brief Options controlling the appearance and behavior of grace notes.
 *
 * This class is identified by the XML node name "graceNoteOptions".
 */
class GraceNoteOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit GraceNoteOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    int tabGracePerc{};              ///< "Tablature Grace Note Size" (%)
    int gracePerc{};                 ///< "Grace Note Size" (%)
    Edu playbackDuration{};          ///< Playback duration for grace notes in @ref Edu.
    Evpu entryOffset{};              ///< "Grace Note Offset on Entry" in @ref Evpu. (xml node is `<graceBackup>`)
    bool slashFlaggedGraceNotes{};   ///< "Always Slash Flagged Grace Notes"
    Efix graceSlashWidth{};          ///< Width (thickness) of grace note slashes in @ref Efix.

    constexpr static std::string_view XmlNodeName = "graceNoteOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<GraceNoteOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class KeySignatureOptions
 * @brief Options controlling the appearance and behavior of key signatures.
 *
 * This class is identified by the XML node name "keySignatureOptions".
 */
class KeySignatureOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit KeySignatureOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<KeySignatureOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class LineCurveOptions
 * @brief Options controlling the appearance of line and curve elements.
 *
 * This class is identified by the XML node name "lineCurveOptions".
 */
class LineCurveOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit LineCurveOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<LineCurveOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MiscOptions
 * @brief Options controlling miscellaneous settings.
 *
 * This class is identified by the XML node name "miscOptions".
 */
class MiscOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit MiscOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    bool showRepeatsForParts{};             ///< "Show Repeats for Parts"
    bool keepOctaveTransInConcertPitch{};   ///< "Keep Octave Transposition in Concert Pitch" (xml node is `<retainOctaveTransInConcertPitch>`)
    bool showActiveLayerOnly{};             ///< "Show Active Layer Only" (xml node is `<showCurrentLayerOnly>`)
    bool consolidateRestsAcrossLayers{};    ///< "Consolidate Rests Across Layers" (xml node is `<combineRestsAcrossLayers>`)
    Evpu shapeDesignerDashLength{};         ///< Shape Designer dash length in @ref Evpu. (xml node is `<sdDashOn>`)
    Evpu shapeDesignerDashSpace{};          ///< Shape Designer dash space in @ref Evpu. (xml node is `<sdDashOff>`)
    bool alignMeasureNumbersWithBarlines{}; ///< "Align Measure Numbers With Barlines" (xml node is `<drawMeasureNumbersOverBarlines>`)

    constexpr static std::string_view XmlNodeName = "miscOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MiscOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MultimeasureRestOptions
 * @brief Options controlling the appearance and behavior of multimeasure rests.
 *
 * This class is identified by the XML node name "multimeasureRestOptions".
 */
class MultimeasureRestOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit MultimeasureRestOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    Evpu measWidth{};            ///< Space allocated per measure in multimeasure rest in @ref Evpu. (xml node is `<meaSpace>`)
    Evpu numAdjY{};              ///< Vertical adjustment for number in @ref Evpu. The Finale UI reverses the sign of this value. (xml node is `<numdec>`)
    Cmper shapeDef{};            ///< @ref Cmper of shape for multimeasure rest H-bar.
    int numStart{};              ///< Value for "Start Number At".
    int useSymsThreshold{};      ///< Numbers of bars rest below this number use symbols if `useSymbols` is checked. (xml node is `<threshold>`)
    Evpu symSpacing{};           ///< Spacing between symbols in @ref Evpu if `useSymbols` is checkd. (xml node is `<spacing>`)
    Evpu numAdjX{};              ///< Horizontal adjustment for number in @ref Evpu.
    Evpu startAdjust{};          ///< Start point adjustment for H-bar shape in @ref Evpu.
    Evpu endAdjust{};            ///< End point adjustment for H-bar shape in @ref Evpu.
    bool useSymbols{};           ///< Use character style for rests. (xml node is `<useCharRestStyle>`)
    bool autoUpdateMmRests{};    ///< Automatically update multimeasure rests when the user changes these options.

    constexpr static std::string_view XmlNodeName = "multimeasureRestOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<MultimeasureRestOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class MusicSpacingOptions
 * @brief Options controlling music spacing.
 *
 * This class is identified by the XML node name "musicSpacingOptions".
 */
class MusicSpacingOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit MusicSpacingOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}
 
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
        Clear,          ///< Clear manual positioning (the default).
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
    bool interpolateAllotments{};           ///< Interpolate allotments.
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
    static const xml::XmlElementArray<MusicSpacingOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    enum class AdjustPageScope
    {
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

        static const xml::XmlElementArray<PageFormat> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
     * @param partId Usually 0. This parameter is needed for the generic factory routine.
     * @param shareMode Usually `ShareMode::All`. This parameter is needed for the generic factory routine.
     */
    explicit PageFormatOptions(const DocumentPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode)
    {
    }

    /**
     * @brief The XML node name for this type.
     */
    constexpr static std::string_view XmlNodeName = "pageFormatOptions";
    static const xml::XmlElementArray<PageFormatOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class PianoBraceBracketOptions
 * @brief Options controlling the appearance of piano braces and brackets.
 *
 * This class is identified by the XML node name "pianoBraceBracketOptions".
 */
class PianoBraceBracketOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit PianoBraceBracketOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<PianoBraceBracketOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class RepeatOptions
 * @brief Options controlling the appearance of repeats.
 *
 * This class is identified by the XML node name "repeatOptions".
 */
class RepeatOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit RepeatOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    Evpu bracketHeight{};                  ///< Height of the bracket in @ref Evpu.
    Evpu bracketHookLen{};                 ///< Length of the bracket hook in @ref Evpu.
    Efix bracketLineWidth{};               ///< Width of the bracket line in @ref Efix.
    Evpu bracketStartInset{};              ///< Inset at the start of the bracket in @ref Evpu.
    Evpu bracketEndInset{};                ///< Inset at the end of the bracket in @ref Evpu.
    Evpu bracketTextHPos{};                ///< Horizontal position of bracket text in @ref Evpu.
    Evpu bracketTextVPos{};                ///< Vertical position of bracket text in @ref Evpu.
    Evpu bracketEndHookLen{};              ///< Length of the hook at the end of the bracket in @ref Evpu.
    bool bracketEndAnchorThinLine{};       ///< Whether the end of the bracket anchors to a thin line.
    bool showOnTopStaffOnly{};             ///< "Show On Top Staff Only"
    Cmper showOnStaffListNumber{};         ///< Staff list number (0 if none). @ref Cmper links to associated "repeatStaffList..." xml nodes.

    constexpr static std::string_view XmlNodeName = "repeatOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<RepeatOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class SmartShapeOptions
 * @brief Options controlling the appearance of smart shapes in the musx file.
 *
 * This class is identified by the XML node name "smartShapeOptions".
 */
class SmartShapeOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit SmartShapeOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    /// @brief Default slur direction
    enum class DefaultDirection
    {
        Automatic,
        Over,
        Under
    };

    /// @brief Connection index values
    enum class ConnectionIndex
    {
        HeadRightTop,
        HeadLeftTop,
        StemRightTop,
        StemLeftTop,
        HeadRightBottom,
        HeadLeftBottom,
        StemRightBottom,
        StemLeftBottom,
        NoteRightCenter,
        NoteLeftCenter,
        NoteRightBottom,
        NoteLeftBottom,
        NoteRightTop,
        NoteLeftTop
    };

    /// @brief Slur connection style types
    enum class SlurConnectStyleType
    {
        OverNoteStart,
        OverNoteEnd,
        OverStemStart,
        OverStemEnd,
        UnderNoteStart,
        UnderNoteEnd,
        UnderStemStart,
        UnderStemEnd,
        OverMixStemStart,
        OverMixStemEnd,
        OverStemGrace,
        OverStemPrincipal,
        UnderStemGrace,
        UnderStemPrincipal,
        UnderNoteGrace,
        UnderStemNotePrincipal,
        OverNoteGrace,
        OverStemNotePrincipal,
        OverBeamStart,
        OverBeamEnd,
        UnderBeamStart,
        UnderBeamEnd,
        OverMixFlagStart,
        OverFlagStart,
        UnderFlagStart,
        OverTabNumStart,
        OverTabNumEnd,
        UnderTabNumStart,
        UnderTabNumEnd
    };

    /// @brief Tab slide connection style types
    enum class TabSlideConnectStyleType
    {
        DiffLevelPitchUpLineStart,
        DiffLevelPitchUpLineEnd,
        DiffLevelPitchUpSpaceStart,
        DiffLevelPitchUpSpaceEnd,
        DiffLevelPitchDownLineStart,
        DiffLevelPitchDownLineEnd,
        DiffLevelPitchDownSpaceStart,
        DiffLevelPitchDownSpaceEnd,
        SameLevelPitchUpLineStart,
        SameLevelPitchUpLineEnd,
        SameLevelPitchUpSpaceStart,
        SameLevelPitchUpSpaceEnd,
        SameLevelPitchDownLineStart,
        SameLevelPitchDownLineEnd,
        SameLevelPitchDownSpaceStart,
        SameLevelPitchDownSpaceEnd,
        SameLevelPitchSameStart,
        SameLevelPitchSameEnd
    };

    /// @brief Glissando connection style types
    enum class GlissandoConnectStyleType
    {
        DefaultStart,
        DefaultEnd
    };

    /// @brief Bend curve connection style types
    enum class BendCurveConnectStyleType
    {
        NoteStart,
        StaffEnd,
        StaffStart,
        NoteEnd,
        StaffToTopLineStart,
        StaffFromTopLineEnd,
        StaffEndOffset,
        StaffFromTopEndOffset
    };

    /// @brief Slue control style types
    enum class SlurControlStyleType
    {
        ShortSpan,
        MediumSpan,
        LongSpan,
        ExtraLongSpan
    };

    Evpu shortHairpinOpeningWidth{};         ///< "Short Hairpin Opening Width"
    Evpu crescHeight{};                      ///< "Crescendo Height"
    Evpu maximumShortHairpinLength{};       ///< "Maximum Short Hairpin Length"
    Efix crescLineWidth{};                  ///< "Crescendo Line Width"
    Evpu hookLength{};                       ///< "Hook Length"
    Efix smartLineWidth{};                  ///< "Smart Line Width"
    bool showOctavaAsText{};                   ///< "Show Octava As Text"
    Evpu smartDashOn{};                      ///< "Smart Dash On Length"
    Evpu smartDashOff{};                     ///< "Smart Dash Off Length"
    bool crescHorizontal{};                    ///< "Horizontal Crescendo"
    DefaultDirection direction{};           ///< Default slur direction
    Evpu slurThicknessCp1X{};                ///< "Slur Thickness Control Point 1 X"
    Evpu slurThicknessCp1Y{};                ///< "Slur Thickness Control Point 1 Y"
    Evpu slurThicknessCp2X{};                ///< "Slur Thickness Control Point 2 X"
    Evpu slurThicknessCp2Y{};                ///< "Slur Thickness Control Point 2 Y"
    bool slurAvoidAccidentals{};               ///< "Slur Avoid Accidentals"
    Evpu slurAvoidStaffLinesAmt{};            ///< "Slur Avoid Staff Lines Amount"
    Efix maxSlurStretch{};                 ///< "Maximum Slur Stretch"
    Efix maxSlurLift{};                    ///< "Maximum Slur Lift"
    int slurSymmetry{};                   ///< "Slur Symmetry"
    bool useEngraverSlurs{};                   ///< "Use Engraver Slurs"
    Evpu slurLeftBreakHorzAdj{};              ///< "Slur Left Break Horizontal Adjustment"
    Evpu slurRightBreakHorzAdj{};            ///< "Slur Right Break Horizontal Adjustment"
    Evpu slurBreakVertAdj{};                 ///< "Slur Break Vertical Adjustment"
    bool slurAvoidStaffLines{};             ///< "Slur Avoid Staff Lines"
    Evpu slurPadding{};                      ///< "Slur Padding"
    Efix maxSlurAngle{};                   ///< "Maximum Slur Angle"
    Evpu slurAcciPadding{};                  ///< "Slur Accidental Padding"
    bool slurDoStretchFirst{};              ///< "Initial Adjustment: Stretch"
    bool slurStretchByPercent{};            ///< "As Percentage of Slur Length"
    int maxSlurStretchPercent{};          ///< "Maximum Slur Stretch Percentage"
    Evpu articAvoidSlurAmt{};                 ///< "Articulation Avoid Slur Amount"
    Cmper ssLineStyleCmpCustom{};              ///< "SS Line Style Custom"
    Cmper ssLineStyleCmpGlissando{};           ///< "SS Line Style Glissando"
    Cmper ssLineStyleCmpTabSlide{};            ///< "SS Line Style Tab Slide"
    Cmper ssLineStyleCmpTabBendCurve{};        ///< "SS Line Style Tab Bend Curve"
    EvpuFloat smartSlurTipWidth{};            ///< "Smart Slur Tip Width"
    bool guitarBendUseParens{};                ///< "Guitar Bend Use Parentheses"
    bool guitarBendHideBendTo{};               ///< "Guitar Bend Hide Bend To"
    bool guitarBendGenText{};                  ///< "Guitar Bend Generate Text"
    bool guitarBendUseFull{};                  ///< "Guitar Bend Use Full"

    /// @brief Defined the connection offsets for each @ref ConnectionIndex value.
    struct ConnectionStyle {
        ConnectionIndex connectIndex{}; ///< Index of connection point
        Evpu xOffset{};                ///< Horizontal offset
        Evpu yOffset{};                ///< Vertical offset

        static const xml::XmlElementArray<ConnectionStyle> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    /// @brief defines the slur contours for short, medium, long, and extra long slurs
    struct ControlStyle {
        Efix span{};                  ///< Length of the span
        Efix inset{};                 ///< Inset value
        Evpu height{};               ///< Height of the span

        static const xml::XmlElementArray<ControlStyle> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    std::unordered_map<SlurConnectStyleType, std::shared_ptr<ConnectionStyle>> slurConnectStyles; ///< Slur connections
    std::unordered_map<SlurControlStyleType, std::shared_ptr<ControlStyle>> slurControlStyles; ///< Slur contours
    std::unordered_map<TabSlideConnectStyleType, std::shared_ptr<ConnectionStyle>> tabSlideConnectStyles; ///< Tab slide connections
    std::unordered_map<GlissandoConnectStyleType, std::shared_ptr<ConnectionStyle>> glissandoConnectStyles; ///< Glissando connections
    std::unordered_map<BendCurveConnectStyleType, std::shared_ptr<ConnectionStyle>> bendCurveConnectStyles; ///< Benx curve connections

    constexpr static std::string_view XmlNodeName = "smartShapeOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<SmartShapeOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StaffOptions
 * @brief Options controlling the appearance and layout of staves.
 *
 * This class is identified by the XML node name "staffOptions".
 */
class StaffOptions : public OptionsBase {
using NamePositioning = others::NamePositioning;

public:
    /** @brief Constructor function */
    explicit StaffOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    Evpu staffSeparation{};         ///< Default topline-to-topline distance in @ref Evpu. (This value is sign-reversed in the Finale UI.)
    Evpu staffSeparIncr{};          ///< "Add Vertical Space" value for Setup Wizard.
    bool autoAdjustStaffSepar{};    ///< "The Default Value Topline-to-Topline Distance is a Preferred Value That Can Change if Necessary" (for Setup Wizard.)

    std::shared_ptr<NamePositioning> namePos;           ///< Default full name positioning for staves.
    std::shared_ptr<NamePositioning> namePosAbbrv;      ///< Default abbreviated name positioning for staves.
    std::shared_ptr<NamePositioning> groupNameFullPos;  ///< Default full name positioning for staff groups.
    std::shared_ptr<NamePositioning> groupNameAbbrvPos; ///< Default abbreviated name positioning for staff groups.

    constexpr static std::string_view XmlNodeName = "staffOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StaffOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class StemOptions
 * @brief Options controlling the appearance of stems.
 *
 * This class is identified by the XML node name "stemOptions".
 */
class StemOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit StemOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    Evpu halfStemLength{};     ///< Half stem length in @ref Evpu.
    Evpu stemLength{};         ///< Stem length in @ref Evpu.
    Evpu shortStemLength{};    ///< Short stem length in @ref Evpu. (xml node is `<stem2>`)
    Evpu revStemAdj{};         ///< Reverse stem adjustment in @ref Evpu.
    bool noReverseStems{};     ///< Set if "Display Reverse Stemming" is unchecked. (This bit is reversed in the Finale UI.)
    Efix stemWidth{};          ///< Stem width in @ref Efix.
    Efix stemOffset{};         ///< Stem offset in @ref Efix. (xml node is `<stemLift>`)
    bool useStemConnections{}; ///< "Use Stem Connections"

    constexpr static std::string_view XmlNodeName = "stemOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<StemOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TieOptions
 * @brief Options controlling the appearance of ties.
 *
 * This class is identified by the XML node name "tieOptions".
 */
class TieOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit TieOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    /** @brief Enumeration for seconds placement options */
    enum class SecondsPlacement
    {
        None,                   ///< the default
        ShiftForSeconds         ///< xml value is "both"
    };

    /** @brief Enumeration for chord tie direction type */
    enum class ChordTieDirType {
        OutsideInside,          ///< Legacy Finale 3.7 behavior (the default)
        StemReversal,           ///< Split at stem reversal point.
        SplitEvenly             ///< Split evenly
    };

    /** @brief Enumeration for mixed stem direction */
    enum class MixedStemDirection {
        Over,
        Under,
        OppositeFirst           ///< xml value is "start"
    };

    /** @brief Enumeration for special position mode */
    enum class SpecialPosMode {
        None, ///< the default
        Avoid
    };

    /** @brief Enumeration for inset styles */
    enum class InsetStyle {
        Fixed,                  ///< the default
        Percent
    };

    /** @brief Enumeration for tie connect style types */
    enum class ConnectStyleType {
        OverStartPosInner,
        OverEndPosInner,
        UnderStartPosInner,
        UnderEndPosInner,
        OverHighestNoteStartPosOver,
        OverHighestNoteEndPosOver,
        UnderLowestNoteStartPosUnder,
        UnderLowestNoteEndPosUnder,
        OverHighestNoteStemStartPosOver,
        OverHighestNoteStemEndPosOver,
        UnderLowestNoteStemStartPosUnder,
        UnderLowestNoteStemEndPosUnder
    };

    /** @brief Enumeration for tie control style types */
    enum class ControlStyleType {
        ShortSpan,
        MediumSpan,
        LongSpan,
        TieEnds
    };

    Evpu frontTieSepar{};                   ///< Separation for the front of ties
    Evpu thicknessRight{};                 ///< Right thickness of the tie
    Evpu thicknessLeft{};                  ///< Left thickness of the tie
    bool breakForTimeSigs{}; ///< Break ties for time signatures
    bool breakForKeySigs{};  ///< Break ties for key signatures
    Evpu breakTimeSigLeftHOffset{};        ///< Left horizontal offset for breaking time signature
    Evpu breakTimeSigRightHOffset{};       ///< Right horizontal offset for breaking time signature
    Evpu breakKeySigLeftHOffset{};         ///< Left horizontal offset for breaking key signature
    Evpu breakKeySigRightHOffset{};        ///< Right horizontal offset for breaking key signature
    Evpu sysBreakLeftHAdj{};               ///< Left adjustment for system breaks
    Evpu sysBreakRightHAdj{};              ///< Right adjustment for system breaks
    bool useOuterPlacement{}; ///< Use outer placement for ties
    SecondsPlacement secondsPlacement{}; ///< Placement of seconds
    ChordTieDirType chordTieDirType{}; ///< Chord tie direction type
    bool chordTieDirOpposingSeconds{}; ///< Chord tie opposing seconds
    MixedStemDirection mixedStemDirection{}; ///< Mixed stem direction
    bool afterSingleDot{}; ///< Special handling after a single dot
    bool afterMultipleDots{}; ///< Special handling after multiple dots
    bool beforeAcciSingleNote{}; ///< Special handling before accidental single notes
    SpecialPosMode specialPosMode{}; ///< Special positioning mode
    Evpu avoidStaffLinesDistance{};        ///< Distance to avoid staff lines
    InsetStyle insetStyle{}; ///< Inset style for ties
    bool useInterpolation{}; ///< "Interpolate Height Between Short and Long Span"
    bool useTieEndCtlStyle{}; ///< Use tie end control style
    bool avoidStaffLinesOnly{}; ///< Only avoid staff lines
    EvpuFloat tieTipWidth{};               ///< Width of the tie tip

    /** @brief Struct for tie connect style */
    struct ConnectStyle {
        Evpu offsetX{}; ///< Horizontal offset
        Evpu offsetY{}; ///< Vertical offset

        static const xml::XmlElementArray<ConnectStyle> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    /** @brief Tie connect styles */
    std::unordered_map<ConnectStyleType, std::shared_ptr<ConnectStyle>> tieConnectStyles;

    /** @brief Struct for tie control style control points */
    struct ControlPoint {
        Efix insetRatio{}; ///< Inset ratio
        Evpu height{}; ///< Height
        Evpu insetFixed{}; ///< Fixed inset

        static const xml::XmlElementArray<ControlPoint> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    /** @brief Struct for tie control style */
    struct ControlStyle {
        Evpu span{}; ///< Span length
        std::shared_ptr<ControlPoint> cp1; ///< Control point 1
        std::shared_ptr<ControlPoint> cp2; ///< Control point 2

        static const xml::XmlElementArray<ControlStyle> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
    };

    /** @brief Tie control styles */
    std::unordered_map<ControlStyleType, std::shared_ptr<ControlStyle>> tieControlStyles;

    constexpr static std::string_view XmlNodeName = "tieOptions"; ///< The XML node name for this type
    static const xml::XmlElementArray<TieOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
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
    explicit TimeSignatureOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

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
    static const xml::XmlElementArray<TimeSignatureOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class TupletOptions
 * @brief Options controlling the appearance of tuplets.
 *
 * This class is identified by the XML node name "tupletOptions".
 */
class TupletOptions : public OptionsBase {
public:
    /** @brief Constructor function */
    explicit TupletOptions(const DocumentWeakPtr& document, Cmper partId = 0, ShareMode shareMode = ShareMode::All)
        : OptionsBase(document, partId, shareMode) {}

    /** @brief Auto-bracket style options. */
    enum class AutoBracketStyle
    {
        Always,             ///< "Always Use Specified Shape"--the default value
        UnbeamedOnly,       ///< "Bracket Unbeamed Notes Only"
        NeverBeamSide,      ///< "Never Bracket Beamed Notes on Beam Side"
    };

    /** @brief Number style options.
     * 
     * @warning Finale appears to have implemented `RatioPlusBothNotes` and `RatioPlusDenominatorNote`
     * backwards from how they are named. It is probably a long-standing bug that was never worth fixing.
    */
    enum class NumberStyle
    {
        Nothing,                    ///< The default
        Number,                     ///< Number only
        UseRatio,                   ///< Ratio X:Y
        RatioPlusBothNotes,         ///< @warning displays a note value on the denominator
        RatioPlusDenominatorNote,   ///< @warning display notes on both values (xml value is "ratioPlusDenNote")
    };

    /** @brief Positioning style options. */
    enum class PositioningStyle
    {
        Manual,     ///< The default
        BeamSide,   ///< Display the tuplet on the beam side
        NoteSide,   ///< Display the tuplet on the note side
        Above,      ///< Display the tuplet above the notes
        Below       ///< Display the tuplet below the notes
    };

    /** @brief Bracket style options. */
    enum class BracketStyle
    {
        Nothing,    ///< No bracket (the default)
        Slur,       ///< Use a slur instead of a bracket.
        Bracket     ///< Use a bracket.
    };

    bool alwaysFlat{};                      ///< "Always Flat" (xml node is `<flat>`)
    bool fullDura{};                        ///< "Bracket Full Duration"
    bool metricCenter{};                    ///< "Center Number Using Duration"
    bool avoidStaff{};                      ///< "Avoid Staff"
    AutoBracketStyle autoBracketStyle{};    ///< Autobracket style
    Evpu tupOffX{};                         ///< Horizontal offset.
    Evpu tupOffY{};                         ///< Vertical.
    Evpu brackOffX{};                       ///< Horizontal offset for brackets.
    Evpu brackOffY{};                       ///< Vertical offset for brackets.
    NumberStyle numStyle{};                 ///< Number style
    PositioningStyle posStyle{};            ///< Positioning style
    bool allowHorz{};                       ///< "Allow Horizontal Drag"
    bool ignoreHorzNumOffset{};             ///< "Ignore Horizontal Number Offset" (xml node is `<ignoreGlOffs>`)
    bool breakBracket{};                    ///< "Break Slur or Bracket"
    bool matchHooks{};                      ///< "Match Length of Hooks"
    bool useBottomNote{};                   ///< "Use Bottom Note" (xml node is `<noteBelow>`)
    BracketStyle brackStyle{};              ///< Bracket style.
    bool smartTuplet{};                     ///< "Engraver Tuplets"
    Evpu leftHookLen{};                     ///< Length of the left hook in the tuplet bracket. (This value is sign-reversed in the Finale UI.)
    Evpu leftHookExt{};                     ///< Extension of the left hook beyond the tuplet bracket.
    Evpu rightHookLen{};                    ///< Length of the right hook in the tuplet bracket. (This value is sign-reversed in the Finale UI.)
    Evpu rightHookExt{};                    ///< Extension of the right hook beyond the tuplet bracket.
    Evpu manualSlopeAdj{};                  ///< "Manual Slope Adjustment" in @ref Evpu. (xml node is `<slope>`)
    int tupMaxSlope{};                      ///< Maximum slope for automatic tuplet brackets in 10ths of an angular degree.
    Efix tupLineWidth{};                    ///< Line width for tuplet brackets in @ref Efix.
    Evpu tupNUpstemOffset{};                ///< Offset for upstem tuplet numbers.
    Evpu tupNDownstemOffset{};              ///< Offset for downstem tuplet numbers.

    constexpr static std::string_view XmlNodeName = "tupletOptions"; ///< The XML node name for this type.
    static const xml::XmlElementArray<TupletOptions> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

} // namespace options
} // namespace dom
} // namespace musx