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

#include "musx/util/Logger.h"
#include "musx/util/Fraction.h"

#include "BaseClasses.h"
#include "CommonClasses.h"
 // do not add other dom class dependencies. Use Implementations.cpp for implementations that need total class access.

namespace musx {
namespace dom {

class EntryInfoPtr;

namespace others {

/**
 * @class SmartShape
 * @brief Represents a Finale smart shape.
 *
 * This class is identified by the XML node name "smartShape".
 */
class SmartShape : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit SmartShape(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /**
     * @brief Represents an endpoint of the smart shape.
     */
    class EndPoint : public Base
    {
    public:
        /** @brief Constructor function. */
        explicit EndPoint(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All) {}

        InstCmper staffId{};            ///< Staff ID (xml node is `<inst>`)
        MeasCmper measId{};             ///< Measure ID (xml node is `<meas>`)
        Edu eduPosition{};              ///< Edu position of endpoint (xml node is `<edu>`)
        EntryNumber entryNumber{};      ///< Entry number. Zero if the endpoint is not entry-attached. (xml node is `<entryNum>`)

        /// @brief Calculates the staff-level position of the endpoint within its measure, based on whether it is measure- or entry-attached
        util::Fraction calcPosition() const;

        /// @brief Calculates the global position of the endpoint within its measure, based on whether it is measure- or entry-attached
        util::Fraction calcGlobalPosition() const;

        /// @brief Calculates the entry associated with the endpoint.
        /// @return The entry if the endpoint is entry-attached or within 1 Edu of an entry. Null if not.
        EntryInfoPtr calcAssociatedEntry() const;

        bool requireAllFields() const override { return false; }
        static const xml::XmlElementArray<EndPoint>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /**
     * @brief Represents the endpoint adjustment of the smart shape.
     */
    class EndPointAdjustment : public Base
    {
    public:
        /** @brief Constructor function. */
        explicit EndPointAdjustment(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All) {}

        Evpu horzOffset{};          ///< Horizontal offset (xml node is `<x>`)
        Evpu vertOffset{};          ///< Vertical offset (xml node is `<y>`)
        bool active{};              ///< If true, this adjustment should be used when it is applicable (xml node is `<on>`)

        bool requireAllFields() const override { return false; }    ///< ignore other fields because they are difficult to figure out
        static const xml::XmlElementArray<EndPointAdjustment>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
    };

    /**
     * @brief Represents the termination segment of the smart shape.
     */
    class TerminationSeg : public Base
    {
    public:
        /** @brief Constructor function. */
        explicit TerminationSeg(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All) {}

        std::shared_ptr<EndPoint> endPoint;                 ///< Endpoint information (xml node is `<endPt>`)
        std::shared_ptr<EndPointAdjustment> endPointAdj;    ///< Endpoint adjustment information (xml node is `<endPtAdj>`)
        std::shared_ptr<EndPointAdjustment> breakAdj;       ///< System break adjustment for first or last system (depending which endpoint it is)
                                                            ///< Systems other than the first or last are controlled with instances of @ref details::CenterShape.

        void integrityCheck() override
        {
            Base::integrityCheck();
            if (!endPoint) {
                endPoint = std::make_shared<EndPoint>(getDocument());
            }
            if (!endPointAdj) {
                endPointAdj = std::make_shared<EndPointAdjustment>(getDocument());
            }
            if (!breakAdj) {
                breakAdj = std::make_shared<EndPointAdjustment>(getDocument());
            }
        }
    
        bool requireAllFields() const override { return false; }    ///< ignore other fields because they are difficult to figure out
        static const xml::XmlElementArray<TerminationSeg>& xmlMappingArray();    ///< Required for musx::factory::FieldPopulator.
    };

    /**
     * @enum ShapeType
     * @brief All the smart shape types supported by Finale
     *
     * The enum values match Finale's internal values. (Note that 24 is skipped.)
     * However, they are saved in the xml as text values. These are camel case and otherwise match
     * the enum names, except as noted.
     */
    enum class ShapeType : int
    {
        SlurDown = 0,       ///< Downward Slur (tips point up). This is the default and may not appear in the xml, but the text is in the Finale binary.
        SlurUp,             ///< Upward Slur (tips point down).
        Decrescendo,        ///< Decrescendo hairpin. (xml value is "decresc")
        Crescendo,          ///< Crescendo hairpin. (xml value is "cresc")
        OctaveDown,         ///< Ottava bassa. (8vb)
        OctaveUp,           ///< Ottava. (8va)
        DashLineUp,         ///< Dashed line with upward hook.
        DashLineDown,       ///< Dashed line with downward hook.                                == 7
        DashSlurDown,       ///< Thin line dashed downward slur (tips point up).
        DashSlurUp,         ///< Thin line dashed upward slur (tips point down).
        DashLine,           ///< Dashed line.
        SolidLine,          ///< Solid line.
        SolidLineDown,      ///< Solid line with downward right hook.
        SolidLineUp,        ///< Solid line with upward right hook.
        Trill,              ///< Trill (includes "tr" symbol at the beginning).                 == 14
        SlurAuto,           ///< Slur with direction automatically determined.
        DashSlurAuto,       ///< Thin line dashed slur with direction automatically determined.
        TrillExtension,     ///< Trill extension: only the wavy line. (xml value is "trillExt")
        SolidLineDownBoth,  ///< Solid line with downward hooks on both ends. (xml value is "solidLineDown2")
        SolidLineUpBoth,    ///< Solid line with upward hooks on both ends. (xml value is "solidLineUp2")
        TwoOctaveDown,      ///< Quindicesima bassa (15ma bassa)
        TwoOctaveUp,        ///< Quindicesima (15ma)                                            == 21
        DashLineDownBoth,   ///< Dashed line with downward hooks on both ends. (xml value is "dashLineDown2")
        DashLineUpBoth,     ///< Dashed line with upward hooks on both ends. (xml value is "dashLineUp2")
        Glissando = 25,     ///< Glissando (shape is determined by #lineStyleId)                == 25 (24 has no value)
        TabSlide,           ///< Tab slide (a solid line that is often used as a note-attached glissando)
        BendHat,            ///< Bend hat (solid up-down line with a corner in the middle)
        BendCurve,          ///< Bend curve
        CustomLine,         ///< User-created custom line with shape determined by #lineStyleId. (xml value is "smartLine")
        SolidLineUpLeft,    ///< Solid line with upward left hook.
        SolidLineDownLeft,  ///< Solid line with downward left hook.
        DashLineUpLeft,     ///< Dashed line with upward left hook.                             == 32
        DashLineDownLeft,   ///< Dashed line with downward left hook.
        SolidLineUpDown,    ///< Solid line with upward left hook and downward right hook.
        SolidLineDownUp,    ///< Solid line with downward left hook and upward right hook.
        DashLineUpDown,     ///< Dashed line with upward left hook and downward right hook.
        DashLineDownUp,     ///< Dashed line with downward left hook and upward right hook.
        Hyphen,             ///< Lyric hyphen, used only with lyric assignments.
        WordExtension,      ///< Lyric word extension, used only with lyric assignments. (xml value is "wordExt")
        DashContourSlurDown, ///< Contoured line dashed downward slur (tips point up).
        DashContourSlurUp,  ///< Contoured line dashed upward slur (tips point down).
        DashContouSlurAuto, ///< Contoured line dashed slur with direction automatically determined.
    };

    ShapeType shapeType{};                          ///< Type of smart shape
    bool entryBased{};                              ///< Whether the shape is entry-based
    std::shared_ptr<TerminationSeg> startTermSeg;   ///< Start termination segment
    std::shared_ptr<TerminationSeg> endTermSeg;     ///< End termination segment
    bool hidden{};                                  ///< Inverse of "Show" option
    NoteNumber startNoteId{};                       ///< If non-zero, the specific note with the entry that this shape starts from. (xml node is `<startNoteID>`)
    NoteNumber endNoteId{};                         ///< If non-zero, the specific note with the entry that this shape ends on. (xml node is `<endNoteID>`)
    Cmper lineStyleId{};                            ///< If non-zero, the @ref SmartShapeCustomLine for this shape. Several #ShapeType values use it. (xml node is `<lineStyleID>`)

    /// @brief Calculates if the smart shape applies to the specified entry.
    ///
    /// This function is most useful for shape types like ottavas and hairpins. It does
    /// not check layers or staves in between start and end staves, so it may be less useful for slurs.
    /// @param entryInfo The entry to check
    bool calcAppliesTo(const EntryInfoPtr& entryInfo) const;

    void integrityCheck() override
    {
        OthersBase::integrityCheck();
        if (!startTermSeg) {
            startTermSeg = std::make_shared<TerminationSeg>(getDocument());
        }
        if (!endTermSeg) {
            endTermSeg = std::make_shared<TerminationSeg>(getDocument());
        }
        startTermSeg->integrityCheck();
        endTermSeg->integrityCheck();
    }

    bool requireAllFields() const override { return false; }    ///< ignore other fields because they are difficult to figure out
    constexpr static std::string_view XmlNodeName = "smartShape"; ///< XML node name
    static const xml::XmlElementArray<SmartShape>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class SmartShapeCustomLine
 * @brief Represents a Finale smart shape custom line style.
 *
 * This class is identified by the XML node name "ssLineStyle".
 */
class SmartShapeCustomLine : public OthersBase
{
public:
    /// @brief Constructor function
    explicit SmartShapeCustomLine(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper)
        : OthersBase(document, partId, shareMode, cmper) {}

    /// @enum LineStyle
    /// @brief The type of line style.
    enum class LineStyle { Char, Solid, Dashed };

    /// @enum LineCapType
    /// @brief The type of line cap.
    enum class LineCapType { None, Hook, ArrowheadPreset, ArrowheadCustom };

    /// @brief Hold parameters for LineStyle::Char.
    class CharParams : public Base
    {
    public:
    /// @brief Constructor function
        explicit CharParams(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All), font(std::make_shared<FontInfo>(document))
        {
        }

        char32_t lineChar{};            ///< The character to use for the line
        std::shared_ptr<FontInfo> font; ///< `<fontID>`, `<fontSize>`, `<fontEfx>`
        int baselineShiftEms{};         ///< The UI says the units are "EMs", but it may be 1/100 EMs.

        static const xml::XmlElementArray<CharParams>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /// @brief Hold parameters for LineStyle::Solid.
    class SolidParams : public Base
    {
    public:
    /// @brief Constructor function
        explicit SolidParams(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All)
        {
        }

        Efix lineWidth{};             ///< Solid line width

        static const xml::XmlElementArray<SolidParams>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    /// @brief Hold parameters for LineStyle::Dashed.
    class DashedParams : public Base {
    public:
    /// @brief Constructor function
        explicit DashedParams(const DocumentWeakPtr& document)
            : Base(document, SCORE_PARTID, ShareMode::All) {}

        Efix lineWidth{};             ///< Dashed line width
        Efix dashOn{};                ///< Dash length
        Efix dashOff{};               ///< Length of gap between dashes

        static const xml::XmlElementArray<DashedParams>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
    };

    LineStyle lineStyle{};                                ///< Line style
    std::shared_ptr<CharParams> charParams;               ///< Parameters for char line style. Only allocated if lineStyle is LineStyle::Char.
    std::shared_ptr<SolidParams> solidParams;             ///< Parameters for solid line style. Only allocated if lineStyle is LineStyle::Solid.
    std::shared_ptr<DashedParams> dashedParams;           ///< Parameters for dashed line style. Only allocated if lineStyle is LineStyle::Dashed.

    LineCapType lineCapStartType{};                       ///< Line cap start type
    LineCapType lineCapEndType{};                         ///< Line cap end type
    Cmper lineCapStartArrowId{};                          ///< Cmper of start arrowhead (preset or cmper of custom @ref ShapeDef). xml node is `<lineCapStartArrowID>`
    Cmper lineCapEndArrowId{};                            ///< Cmper of end arrowhead (preset or cmper of custom @ref ShapeDef). xml node is `<lineCapStartArrowID>`

    bool makeHorz{};                                      ///< "Horizontal"
    bool lineAfterLeftStartText{};                        ///< Line adjustments "Start H: After Text"
    bool lineBeforeRightEndText{};                        ///< Line adjustments "Right H: Before Text"
    bool lineAfterLeftContText{};                         ///< Line adjustments "Cont H: After Text"

    Cmper leftStartRawTextId{};                           ///< Cmper of @ref texts::SmartShapeText. xml node is `<leftStartRawTextID>`
    Cmper leftContRawTextId{};                            ///< Cmper of @ref texts::SmartShapeText. xml node is `<leftContRawTextID>`
    Cmper rightEndRawTextId{};                            ///< Cmper of @ref texts::SmartShapeText. xml node is `<rightEndRawTextID>`
    Cmper centerFullRawTextId{};                          ///< Cmper of @ref texts::SmartShapeText. xml node is `<centerFullRawTextID>`
    Cmper centerAbbrRawTextId{};                          ///< Cmper of @ref texts::SmartShapeText. xml node is `<centerAbbrRawTextID>`

    Evpu leftStartX{};                                    ///< Left start position X
    Evpu leftStartY{};                                    ///< Left start position Y
    Evpu leftContX{};                                     ///< Left continuation position X
    Evpu leftContY{};                                     ///< Left continuation position Y
    Evpu rightEndX{};                                     ///< Right end position X
    Evpu rightEndY{};                                     ///< Right end position Y
    Evpu centerFullX{};                                   ///< Center full position X
    Evpu centerFullY{};                                   ///< Center full position Y
    Evpu centerAbbrX{};                                   ///< Center abbreviation position X
    Evpu centerAbbrY{};                                   ///< Center abbreviation position Y

    Evpu lineStartX{};                                    ///< Line adjustments "Start H" value
    Evpu lineStartY{};                                    ///< Line adjustments "V" value (Finale syncs this with #lineEndY.)
    Evpu lineEndX{};                                      ///< Line adjustments "End H" value
    Evpu lineEndY{};                                      ///< Line adjustments "Start H" value
    Evpu lineContX{};                                     ///< Line adjustments "V" value (Finale syncs this with #lineStartY.)

    Efix lineCapStartHookLength{};                        ///< Length of start hook (if #lineStyle is Hook)
    Efix lineCapEndHookLength{};                          ///< Length of end hook (if #lineStyle is Hook)

    void integrityCheck() override
    {
        OthersBase::integrityCheck();

        if (lineStyle == LineStyle::Char && !charParams)
            charParams = std::make_shared<CharParams>(getDocument());

        if (lineStyle == LineStyle::Solid && !solidParams)
            solidParams = std::make_shared<SolidParams>(getDocument());

        if (lineStyle == LineStyle::Dashed && !dashedParams)
            dashedParams = std::make_shared<DashedParams>(getDocument());
    }

    constexpr static std::string_view XmlNodeName = "ssLineStyle"; ///< The XML node name for this type.
    static const xml::XmlElementArray<SmartShapeCustomLine>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class SmartShapeMeasureAssign
 * @brief Assigns a smart shape or center shape to a measure.
 *
 * Note that *every smart shape* has a measure assignment. This includes entry-assigned smart shapes that
 * *additionally* have an entry assignment.
 *
 * The cmper value is the measure Id.
 *
 * This class is identified by the XML node name "smartShapeMeasMark".
 */
class SmartShapeMeasureAssign : public OthersBase
{
public:
    /** @brief Constructor function */
    explicit SmartShapeMeasureAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper cmper, Inci inci)
        : OthersBase(document, partId, shareMode, cmper, inci)
    {
    }

    Cmper shapeNum{};       ///< The @ref Cmper of the @ref SmartShape that is being assigned.
    Cmper centerShapeNum{}; ///< If non-zero, the 2nd @ref Cmper of the @ref details::CenterShape.
                            ///< Zero if this measure starts or ends the smart shape.

    constexpr static std::string_view XmlNodeName = "smartShapeMeasMark"; ///< The XML node name for this type.
    static const xml::XmlElementArray<SmartShapeMeasureAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace others

namespace details {

/**
 * @class CenterShape
 * @brief Represents a center shape for a @ref others::SmartShape that spans three or more measures.
 *
 * A @ref others::SmartShape instance controls the endpoint information for the start and end measures. It
 * also controls the first and the final system breaks if the shape spans more than one system.
 *
 * Every measure spanned by the shape in between the first and last measures is assigned its own center shape.
 * These are only applicable if their measure is the first measure in a system that is not the first or last system.
 * That means that for any center shape to be applicable, the shape must span at least three systems. However, the center shape
 * records are created for every measure between the first and last, no matter how many or few systems the shape spans.
 *
 * Cmper1 is the shape number. Cmper2 is the center shape number. (See @ref others::SmartShapeMeasureAssign.)
 * This class is identified by the XML node name "centerShape".
 */
class CenterShape : public DetailsBase
{
public:

    /**
     * @brief Constructor
     * @param document A weak pointer to the associated document.
     * @param partId The part that this is for.
     * @param shareMode The sharing mode for this #CenterShape.
     * @param shapeNum The shape number from @ref others::SmartShapeMeasureAssign.
     * @param centerShapeNum The center shape number from @ref others::SmartShapeMeasureAssign.
     */
    explicit CenterShape(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper shapeNum, Cmper centerShapeNum)
        : DetailsBase(document, partId, shareMode, shapeNum, centerShapeNum)
    {}

    std::shared_ptr<others::SmartShape::EndPointAdjustment> startBreakAdj; ///< Adjustment at the start break (xml: `<startBreakAdj>`)
    std::shared_ptr<others::SmartShape::EndPointAdjustment> endBreakAdj;   ///< Adjustment at the end break (xml: `<endBreakAdj>`)

    void integrityCheck() override
    {
        if (!startBreakAdj) {
            startBreakAdj = std::make_shared<others::SmartShape::EndPointAdjustment>(getDocument());
        }
        if (!endBreakAdj) {
            endBreakAdj = std::make_shared<others::SmartShape::EndPointAdjustment>(getDocument());
        }
    }

    bool requireAllFields() const override { return false; }    ///< ignore other fields because they are difficult to figure out
    constexpr static std::string_view XmlNodeName = "centerShape"; ///< The XML node name for this type.
    static const xml::XmlElementArray<CenterShape>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

/**
 * @class SmartShapeEntryAssign
 * @brief Assigns a smart shape to an entry.
 *
 * This class is identified by the XML node name "smartShapeEntryMark".
 */
class SmartShapeEntryAssign : public EntryDetailsBase
{
public:
    /** @brief Constructor function */
    explicit SmartShapeEntryAssign(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, EntryNumber entnum, Inci inci)
        : EntryDetailsBase(document, partId, shareMode, entnum, inci)
    {
    }

    Cmper shapeNum{};       ///< The @ref Cmper of the @ref others::SmartShape that is being assigned.

    constexpr static std::string_view XmlNodeName = "smartShapeEntryMark"; ///< The XML node name for this type.
    static const xml::XmlElementArray<SmartShapeEntryAssign>& xmlMappingArray(); ///< Required for musx::factory::FieldPopulator.
};

} // namespace details

} // namespace dom
} // namespace musx