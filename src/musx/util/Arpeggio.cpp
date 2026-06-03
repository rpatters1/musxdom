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
#include "musx/musx.h"

#include <algorithm>
#include <cmath>
#include <functional>
#include <limits>

using namespace musx::dom;

namespace musx::util {

namespace {

constexpr double EVPU_PER_STAFF_POSITION = EVPU_PER_SPACE / 2.0;

bool calcIsLikelyArpeggioAssignment(const MusxInstance<details::ArticulationAssign>& assign,
    const MusxInstance<others::ArticulationDef>& def,
    const EntryInfoPtr& sourceEntryInfo,
    const std::function<bool(const details::ArticulationAssign::SelectedSymbolContext&)>& symbolContextFilter)
{
    MUSX_ASSERT_IF(!assign || !def || !sourceEntryInfo) {
        return false;
    }
    if (def->copyMode != others::ArticulationDef::CopyMode::Vertical) {
        return false;
    }
    // Restrict to character-based symbols, matching the importer's arpeggio-char expectation.
    const auto symbolContext = assign->calcSelectedSymbolContext(sourceEntryInfo);
    if (!symbolContext || symbolContext->symbol.isShape) {
        return false;
    }
    if (symbolContextFilter) {
        return symbolContextFilter(*symbolContext);
    }
    return symbolContext->symbol.character == smufl_glyph::arpeggioVerticalSegment;
}

bool calcIsCandidateEntry(const EntryInfoPtr& entry, const EntryInfoPtr& source, const ArpeggioSpanOptions& options)
{
    MUSX_ASSERT_IF(!entry || !source) {
        return false;
    }
    const auto entryData = entry->getEntry();
    if (!entryData || !entryData->isNote || entry.calcDisplaysAsRest()) {
        return false;
    }
    if (!options.includeHiddenEntries && entryData->isHidden) {
        return false;
    }
    if (options.skipGraceEntries && entryData->graceNote) {
        return false;
    }
    if (options.constrainToSourceStaffOnly && entry.getStaff() != source.getStaff()) {
        return false;
    }
    if (entry.getMeasure() != source.getMeasure()) {
        return false;
    }
    if (entry.calcGlobalElapsedDuration() != source.calcGlobalElapsedDuration()) {
        return false;
    }
    if (entry.calcGraceElapsedDuration() != source.calcGraceElapsedDuration()) {
        return false;
    }
    return true;
}

double calcSourceRelativeEvpuFromStaffPosition(int staffPosition, double staffOriginOffset)
{
    return staffOriginOffset - static_cast<double>(staffPosition) * EVPU_PER_STAFF_POSITION;
}

double calcSourceRelativeEvpu(Evpu evpu, double staffOriginOffset)
{
    return staffOriginOffset - static_cast<double>(evpu);
}

std::optional<double> calcStaffOriginOffset(
    const DocumentPtr& document,
    Cmper partId,
    StaffCmper sourceStaffId,
    StaffCmper targetStaffId,
    MeasCmper measureId,
    Edu eduPosition,
    const ArpeggioSpanOptions& options)
{
    StaffOriginOffsetRequest request;
    request.sourceStaffId = sourceStaffId;
    request.targetStaffId = targetStaffId;
    request.measureId = measureId;
    request.eduPosition = eduPosition;

    if (options.staffOriginOffsetResolver) {
        const auto result = options.staffOriginOffsetResolver(document, partId, request);
        if (result.decision == StaffOriginOffsetResolverDecision::Offset) {
            return result.targetOriginOffsetEvpu;
        }
        if (result.decision == StaffOriginOffsetResolverDecision::Unavailable) {
            return std::nullopt;
        }
    }

    const auto fallbackResult = calcStaffOriginOffsetUsingScrollViewProxy(document, partId, request);
    if (fallbackResult.decision == StaffOriginOffsetResolverDecision::Offset) {
        return fallbackResult.targetOriginOffsetEvpu;
    }
    return std::nullopt;
}

bool calcPositionRangesOverlap(double firstTop, double firstBottom, double secondTop, double secondBottom)
{
    const double firstMin = (std::min)(firstTop, firstBottom);
    const double firstMax = (std::max)(firstTop, firstBottom);
    const double secondMin = (std::min)(secondTop, secondBottom);
    const double secondMax = (std::max)(secondTop, secondBottom);
    return firstMin <= secondMax && secondMin <= firstMax;
}

std::optional<std::pair<double, double>> calcSourceInstrumentEvpuBounds(
    const DocumentPtr& document,
    const MusxInstanceList<others::StaffUsed>& scrollView,
    const InstrumentInfo& sourceInstrument,
    Cmper partId,
    const EntryInfoPtr& sourceEntryInfo,
    MeasCmper measureId,
    Edu eduPosition,
    const ArpeggioSpanOptions& options)
{
    double instrumentTop = (std::numeric_limits<double>::max)();
    double instrumentBottom = (std::numeric_limits<double>::lowest)();

    for (size_t staffIndex = 0; staffIndex < scrollView.size(); ++staffIndex) {
        const StaffCmper staffId = scrollView[staffIndex]->staffId;
        if (sourceInstrument.staves.find(staffId) == sourceInstrument.staves.end()) {
            continue;
        }

        const auto staff = others::StaffComposite::createCurrent(document, partId, staffId, measureId, eduPosition);
        if (!staff) {
            continue;
        }

        const auto staffOriginOffset = calcStaffOriginOffset(
            document, partId, sourceEntryInfo.getStaff(), staffId, measureId, eduPosition, options);
        if (!staffOriginOffset) {
            continue;
        }
        const double staffTop = calcSourceRelativeEvpuFromStaffPosition(staff->calcTopLinePosition(), *staffOriginOffset);
        const double staffBottom = calcSourceRelativeEvpuFromStaffPosition(staff->calcBottomLinePosition(), *staffOriginOffset);

        instrumentTop = (std::min)(instrumentTop, (std::min)(staffTop, staffBottom));
        instrumentBottom = (std::max)(instrumentBottom, (std::max)(staffTop, staffBottom));
    }

    if (instrumentTop == (std::numeric_limits<double>::max)()
        || instrumentBottom == (std::numeric_limits<double>::lowest)()) {
        return std::nullopt;
    }

    return std::make_pair(instrumentTop, instrumentBottom);
}

std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForVerticalTargets(
    const EntryInfoPtr& sourceEntryInfo,
    double startTarget,
    double endTarget,
    const ArpeggioSpanOptions& options)
{
    MUSX_ASSERT_IF(!sourceEntryInfo) {
        return std::nullopt;
    }
    const auto sourceEntry = sourceEntryInfo->getEntry();
    MUSX_ASSERT_IF(!sourceEntry) {
        return std::nullopt;
    }
    if (!sourceEntry->isNote || sourceEntryInfo.calcDisplaysAsRest()) {
        return std::nullopt;
    }
    if (options.skipGraceEntries && sourceEntry->graceNote) {
        return std::nullopt;
    }

    auto frame = sourceEntryInfo.getFrame();
    MUSX_ASSERT_IF(!frame) {
        return std::nullopt;
    }
    auto document = frame->getDocument();
    MUSX_ASSERT_IF(!document) {
        return std::nullopt;
    }

    const Cmper partId = frame->getRequestedPartId();
    const auto scrollView = document->getScrollViewStaves(partId);
    if (scrollView.empty()) {
        return std::nullopt;
    }

    const auto sourceInstrument = document->getInstrumentForStaff(partId, sourceEntryInfo.getStaff());
    if (!sourceInstrument) {
        return std::nullopt;
    }

    const auto sourceInstrumentBounds = calcSourceInstrumentEvpuBounds(
        document,
        scrollView,
        *sourceInstrument,
        partId,
        sourceEntryInfo,
        sourceEntryInfo.getMeasure(),
        sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
        options);
    if (!sourceInstrumentBounds) {
        return std::nullopt;
    }

    const double targetTop = (std::min)(startTarget, endTarget);
    const double targetBottom = (std::max)(startTarget, endTarget);
    if (targetTop < sourceInstrumentBounds->first || targetBottom > sourceInstrumentBounds->second) {
        return std::nullopt;
    }

    EntryInfoPtr topEntry;
    EntryInfoPtr bottomEntry;
    double topEntryPosition = (std::numeric_limits<double>::max)();
    double bottomEntryPosition = (std::numeric_limits<double>::lowest)();

    const MusicRange range(document, sourceEntryInfo.getMeasure(), sourceEntryInfo.calcGlobalElapsedDuration(),
        sourceEntryInfo.getMeasure(), sourceEntryInfo.calcGlobalElapsedDuration());

    for (size_t staffIndex = 0; staffIndex < scrollView.size(); ++staffIndex) {
        const StaffCmper candidateStaff = scrollView[staffIndex]->staffId;
        if (sourceInstrument->staves.find(candidateStaff) == sourceInstrument->staves.end()) {
            continue;
        }

        const auto candidateStaffOriginOffset = calcStaffOriginOffset(
            document,
            partId,
            sourceEntryInfo.getStaff(),
            candidateStaff,
            sourceEntryInfo.getMeasure(),
            sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
            options);
        if (!candidateStaffOriginOffset) {
            continue;
        }
        scrollView.iterateEntries(staffIndex, staffIndex, range, [&](const EntryInfoPtr& candidate) -> bool {
            if (!calcIsCandidateEntry(candidate, sourceEntryInfo, options)) {
                return true;
            }

            const auto [candTop, candBottom] = candidate.calcTopBottomStaffPositions();
            const double candidateTop = calcSourceRelativeEvpuFromStaffPosition(candTop, *candidateStaffOriginOffset);
            const double candidateBottom = calcSourceRelativeEvpuFromStaffPosition(candBottom, *candidateStaffOriginOffset);
            const bool isSourceEntry = candidate->getEntry()->getEntryNumber() == sourceEntry->getEntryNumber();

            if (!isSourceEntry && !calcPositionRangesOverlap(candidateTop, candidateBottom, startTarget, endTarget)) {
                return true;
            }

            if (!topEntry || candidateTop < topEntryPosition) {
                topEntry = candidate;
                topEntryPosition = candidateTop;
            }
            if (!bottomEntry || candidateBottom > bottomEntryPosition) {
                bottomEntry = candidate;
                bottomEntryPosition = candidateBottom;
            }
            return true;
        });
    }

    if (!topEntry || !bottomEntry) {
        return std::nullopt;
    }

    ArpeggioSpanCandidate result;
    result.type = ArpeggioSpanType::Bracket;
    result.sourceEntry = sourceEntryInfo;
    result.topEntry = topEntry;
    result.bottomEntry = bottomEntry;
    return result;
}

} // namespace

std::optional<ArpeggioSpanCandidate> calcArpeggioSpanForAssignment(
    const EntryInfoPtr& sourceEntryInfo,
    const MusxInstance<details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options,
    std::function<bool(const details::ArticulationAssign::SelectedSymbolContext&)> symbolContextFilter)
{
    /// @todo refine this function as needed for better precision on edge cases
    
    MUSX_ASSERT_IF(!sourceEntryInfo || !assign) {
        return std::nullopt;
    }
    const auto sourceEntry = sourceEntryInfo->getEntry();
    MUSX_ASSERT_IF(!sourceEntry) {
        return std::nullopt;
    }
    if (!sourceEntry->isNote || sourceEntryInfo.calcDisplaysAsRest()) {
        return std::nullopt;
    }
    if (options.skipGraceEntries && sourceEntry->graceNote) {
        return std::nullopt;
    }

    auto frame = sourceEntryInfo.getFrame();
    MUSX_ASSERT_IF(!frame) {
        return std::nullopt;
    }
    auto document = frame->getDocument();
    MUSX_ASSERT_IF(!document) {
        return std::nullopt;
    }

    const Cmper partId = frame->getRequestedPartId();
    const auto def = document->getOthers()->get<others::ArticulationDef>(partId, assign->articDef);
    if (!def) {
        return std::nullopt;
    }
    if (!calcIsLikelyArpeggioAssignment(assign, def, sourceEntryInfo, symbolContextFilter)) {
        return std::nullopt;
    }

    const auto [sourceTop, sourceBottom] = sourceEntryInfo.calcTopBottomStaffPositions();
    const double topTarget = calcSourceRelativeEvpuFromStaffPosition(sourceTop, 0.0)
        - static_cast<double>(def->yOffsetMain + assign->vertOffset);
    const double bottomTarget = calcSourceRelativeEvpuFromStaffPosition(sourceBottom, 0.0)
        - static_cast<double>(def->yOffsetMain + assign->vertOffset + assign->vertAdd);

    EntryInfoPtr topEntry;
    EntryInfoPtr bottomEntry;
    double topEntryPosition = (std::numeric_limits<double>::max)();
    double bottomEntryPosition = (std::numeric_limits<double>::lowest)();

    const MusicRange range(document, sourceEntryInfo.getMeasure(), sourceEntryInfo.calcGlobalElapsedDuration(),
        sourceEntryInfo.getMeasure(), sourceEntryInfo.calcGlobalElapsedDuration());

    const auto scrollView = document->getScrollViewStaves(partId);
    if (scrollView.empty()) {
        return std::nullopt;
    }
    for (size_t staffIndex = 0; staffIndex < scrollView.size(); ++staffIndex) {
        const StaffCmper candidateStaff = scrollView[staffIndex]->staffId;
        const auto candidateStaffOriginOffset = calcStaffOriginOffset(
            document,
            partId,
            sourceEntryInfo.getStaff(),
            candidateStaff,
            sourceEntryInfo.getMeasure(),
            sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
            options);
        if (!candidateStaffOriginOffset) {
            continue;
        }

        scrollView.iterateEntries(staffIndex, staffIndex, range, [&](const EntryInfoPtr& candidate) -> bool {
            if (!calcIsCandidateEntry(candidate, sourceEntryInfo, options)) {
                return true;
            }
            const auto [candTop, candBottom] = candidate.calcTopBottomStaffPositions();
            const double candidateTop = calcSourceRelativeEvpuFromStaffPosition(candTop, *candidateStaffOriginOffset);
            const double candidateBottom = calcSourceRelativeEvpuFromStaffPosition(candBottom, *candidateStaffOriginOffset);
            const bool isSourceEntry = candidate->getEntry()->getEntryNumber() == sourceEntry->getEntryNumber();

            if (!isSourceEntry && !calcPositionRangesOverlap(candidateTop, candidateBottom, topTarget, bottomTarget)) {
                return true;
            }

            if (!topEntry || candidateTop < topEntryPosition) {
                topEntry = candidate;
                topEntryPosition = candidateTop;
            }
            if (!bottomEntry || candidateBottom > bottomEntryPosition) {
                bottomEntry = candidate;
                bottomEntryPosition = candidateBottom;
            }
            return true;
        });
    }

    if (!topEntry || !bottomEntry) {
        return std::nullopt;
    }

    ArpeggioSpanCandidate result;
    result.type = ArpeggioSpanType::Normal;
    result.sourceEntry = sourceEntryInfo;
    result.topEntry = topEntry;
    result.bottomEntry = bottomEntry;

    return result;
}

std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForAssignment(
    const dom::EntryInfoPtr& sourceEntryInfo,
    const dom::MusxInstance<dom::details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options)
{
    MUSX_ASSERT_IF(!sourceEntryInfo || !assign) {
        return std::nullopt;
    }
    const auto sourceEntry = sourceEntryInfo->getEntry();
    MUSX_ASSERT_IF(!sourceEntry) {
        return std::nullopt;
    }
    if (!sourceEntry->isNote || sourceEntryInfo.calcDisplaysAsRest()) {
        return std::nullopt;
    }
    if (options.skipGraceEntries && sourceEntry->graceNote) {
        return std::nullopt;
    }

    const auto symbolContext = assign->calcSelectedSymbolContext(sourceEntryInfo);
    if (!symbolContext || !symbolContext->symbol.isShape || !symbolContext->symbol.shapeId) {
        return std::nullopt;
    }

    const auto document = assign->getDocument();
    MUSX_ASSERT_IF(!document) {
        return std::nullopt;
    }
    const auto shape = document->getOthers()->get<others::ShapeDef>(
        assign->getRequestedPartId(), symbolContext->symbol.shapeId);
    if (!shape || shape->recognize() != KnownShapeDefType::VerticalLineRightHooks) {
        return std::nullopt;
    }

    ArpeggioSpanCandidate result;
    result.type = ArpeggioSpanType::Bracket;
    result.sourceEntry = sourceEntryInfo;
    result.topEntry = sourceEntryInfo;
    result.bottomEntry = sourceEntryInfo;
    return result;
}

std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForAssignment(
    const dom::MusxInstance<dom::others::MeasureExprAssign>& assign,
    const ArpeggioSpanOptions& options)
{
    MUSX_ASSERT_IF(!assign) {
        return std::nullopt;
    }

    const auto sourceEntryInfo = assign->calcAssociatedEntry();
    if (!sourceEntryInfo) {
        return std::nullopt;
    }

    const auto shapeExpression = assign->getShapeExpression();
    if (!shapeExpression) {
        return std::nullopt;
    }
    const auto shape = shapeExpression->getShape();
    if (!shape) {
        return std::nullopt;
    }

    const auto localYBounds = calcVerticalLineRightHooksLocalYBounds(*shape);
    if (!localYBounds) {
        return std::nullopt;
    }

    auto frame = sourceEntryInfo.getFrame();
    MUSX_ASSERT_IF(!frame) {
        return std::nullopt;
    }
    auto document = frame->getDocument();
    MUSX_ASSERT_IF(!document) {
        return std::nullopt;
    }

    const Cmper partId = frame->getRequestedPartId();
    const StaffCmper assignedStaffId = assign->calcAssignedStaffId(/*forPageView*/false);
    const auto assignedStaffOriginOffset = calcStaffOriginOffset(
        document,
        partId,
        sourceEntryInfo.getStaff(),
        assignedStaffId,
        sourceEntryInfo.getMeasure(),
        sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
        options);
    if (!assignedStaffOriginOffset) {
        return std::nullopt;
    }

    const double startTarget = *assignedStaffOriginOffset - (static_cast<double>(assign->vertEvpuOff) + localYBounds->first);
    const double endTarget = *assignedStaffOriginOffset - (static_cast<double>(assign->vertEvpuOff) + localYBounds->second);

    return calcNonArpeggioSpanForVerticalTargets(sourceEntryInfo, startTarget, endTarget, options);
}

std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForSmartShape(
    const dom::MusxInstance<dom::others::SmartShape>& smartShape,
    const ArpeggioSpanOptions& options)
{
    MUSX_ASSERT_IF(!smartShape) {
        return std::nullopt;
    }
    const auto sourceEntryInfo = smartShape->startTermSeg->endPoint->calcAssociatedEntry(/*findExact*/true);
    if (!sourceEntryInfo) {
        return std::nullopt;
    }
    const auto sourceEntry = sourceEntryInfo->getEntry();
    MUSX_ASSERT_IF(!sourceEntry) {
        return std::nullopt;
    }

    const auto shapeType = util::recognizeSmartShape(smartShape);
    if (shapeType != KnownSmartShapeType::VerticalLineRightHooks) {
        return std::nullopt;
    }

    auto frame = sourceEntryInfo.getFrame();
    MUSX_ASSERT_IF(!frame) {
        return std::nullopt;
    }
    auto document = frame->getDocument();
    MUSX_ASSERT_IF(!document) {
        return std::nullopt;
    }
    const Cmper partId = frame->getRequestedPartId();
    if (smartShape->startTermSeg->endPoint->measId != sourceEntryInfo.getMeasure()
        || smartShape->endTermSeg->endPoint->measId != sourceEntryInfo.getMeasure()
        || smartShape->startTermSeg->endPoint->calcGlobalPosition() != sourceEntryInfo.calcGlobalElapsedDuration()
        || smartShape->endTermSeg->endPoint->calcGlobalPosition() != sourceEntryInfo.calcGlobalElapsedDuration()) {
        return std::nullopt;
    }

    const auto startStaffOriginOffset = calcStaffOriginOffset(
        document,
        partId,
        sourceEntryInfo.getStaff(),
        smartShape->startTermSeg->endPoint->staffId,
        sourceEntryInfo.getMeasure(),
        sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
        options);
    const auto endStaffOriginOffset = calcStaffOriginOffset(
        document,
        partId,
        sourceEntryInfo.getStaff(),
        smartShape->endTermSeg->endPoint->staffId,
        sourceEntryInfo.getMeasure(),
        sourceEntryInfo.calcGlobalElapsedDuration().calcEduDuration(),
        options);
    if (!startStaffOriginOffset || !endStaffOriginOffset) {
        return std::nullopt;
    }

    const double startTarget = calcSourceRelativeEvpu(
        smartShape->startTermSeg->endPointAdj->calcVertOffset(),
        *startStaffOriginOffset);
    const double endTarget = calcSourceRelativeEvpu(
        smartShape->endTermSeg->endPointAdj->calcVertOffset(),
        *endStaffOriginOffset);
    return calcNonArpeggioSpanForVerticalTargets(sourceEntryInfo, startTarget, endTarget, options);
}

} // namespace musx::util
