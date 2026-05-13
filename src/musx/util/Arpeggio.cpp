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

#include <cmath>
#include <functional>
#include <limits>

using namespace musx::dom;

namespace musx::util {

namespace {

constexpr double EVPU_PER_STAFF_POSITION = EVPU_PER_SPACE / 2.0;

bool calcIsLikelyArpeggioAssignment(const MusxInstance<details::ArticulationAssign>& assign,
    const MusxInstance<others::ArticulationDef>& def,
    const EntryInfoPtr& sourceEntry,
    const std::function<bool(const details::ArticulationAssign::SelectedSymbolContext&)>& symbolContextFilter)
{
    if (!assign || !def || !sourceEntry) {
        return false;
    }
    if (def->copyMode != others::ArticulationDef::CopyMode::Vertical) {
        return false;
    }
    // Restrict to character-based symbols, matching the importer's arpeggio-char expectation.
    const auto symbolContext = assign->calcSelectedSymbolContext(sourceEntry);
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
    if (!entry || !source) {
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

int calcStaffIndex(const MusxInstanceList<others::StaffUsed>& staffOrder, StaffCmper staffId)
{
    if (const auto index = staffOrder.getIndexForStaff(staffId)) {
        return static_cast<int>(*index);
    }
    return (std::numeric_limits<int>::max)();
}

bool calcEntryIsAbove(const EntryInfoPtr& a, const EntryInfoPtr& b, const MusxInstanceList<others::StaffUsed>& staffOrder)
{
    if (!a || !b) {
        return false;
    }
    const int aIndex = calcStaffIndex(staffOrder, a.getStaff());
    const int bIndex = calcStaffIndex(staffOrder, b.getStaff());
    if (aIndex != bIndex) {
        return aIndex < bIndex;
    }
    const auto [aTop, aBottom] = a.calcTopBottomStaffPositions();
    const auto [bTop, bBottom] = b.calcTopBottomStaffPositions();
    if (aTop != bTop) {
        return aTop > bTop;
    }
    return aBottom > bBottom;
}

} // namespace

std::optional<ArpeggioSpanCandidate> calcArpeggioSpanForAssignment(
    const EntryInfoPtr& sourceEntry,
    const MusxInstance<details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options,
    std::function<bool(const details::ArticulationAssign::SelectedSymbolContext&)> symbolContextFilter)
{
    if (!sourceEntry || !assign) {
        return std::nullopt;
    }
    const auto sourceData = sourceEntry->getEntry();
    if (!sourceData || !sourceData->isNote || sourceEntry.calcDisplaysAsRest()) {
        return std::nullopt;
    }
    if (options.skipGraceEntries && sourceData->graceNote) {
        return std::nullopt;
    }

    auto frame = sourceEntry.getFrame();
    if (!frame) {
        return std::nullopt;
    }
    auto document = frame->getDocument();
    if (!document) {
        return std::nullopt;
    }

    const Cmper partId = frame->getRequestedPartId();
    const auto def = document->getOthers()->get<others::ArticulationDef>(partId, assign->articDef);
    if (!def) {
        return std::nullopt;
    }
    if (!calcIsLikelyArpeggioAssignment(assign, def, sourceEntry, symbolContextFilter)) {
        return std::nullopt;
    }

    const auto [sourceTop, sourceBottom] = sourceEntry.calcTopBottomStaffPositions();
    const int topOffsetPositions = static_cast<int>(std::lround((def->yOffsetMain + assign->vertOffset) / EVPU_PER_STAFF_POSITION));
    const int bottomOffsetPositions = static_cast<int>(std::lround((def->yOffsetMain + assign->vertOffset + assign->vertAdd) / EVPU_PER_STAFF_POSITION));
    const int topTarget = sourceTop - topOffsetPositions;
    const int bottomTarget = sourceBottom - bottomOffsetPositions;

    EntryInfoPtr topEntry;
    EntryInfoPtr bottomEntry;
    int topDiff = (std::numeric_limits<int>::max)();
    int bottomDiff = (std::numeric_limits<int>::max)();

    const auto scrollView = document->getScrollViewStaves(partId);
    if (scrollView.empty()) {
        return std::nullopt;
    }

    const MusicRange range(document, sourceEntry.getMeasure(), sourceEntry.calcGlobalElapsedDuration(),
        sourceEntry.getMeasure(), sourceEntry.calcGlobalElapsedDuration());

    scrollView.iterateEntries(0, scrollView.size() - 1, range, [&](const EntryInfoPtr& candidate) -> bool {
        if (!calcIsCandidateEntry(candidate, sourceEntry, options)) {
            return true;
        }
        const auto [candTop, candBottom] = candidate.calcTopBottomStaffPositions();
        const int nextTopDiff = std::abs(candTop - topTarget);
        if (!topEntry || nextTopDiff < topDiff) {
            topEntry = candidate;
            topDiff = nextTopDiff;
        }
        const int nextBottomDiff = std::abs(candBottom - bottomTarget);
        if (!bottomEntry || nextBottomDiff < bottomDiff) {
            bottomEntry = candidate;
            bottomDiff = nextBottomDiff;
        }
        return true;
    });

    if (!topEntry || !bottomEntry) {
        return std::nullopt;
    }

    ArpeggioSpanCandidate result;
    result.sourceEntry = sourceEntry;
    result.assign = assign;
    result.definition = def;
    result.topEntry = topEntry;
    result.bottomEntry = bottomEntry;
    result.topStaffPosTarget = topTarget;
    result.bottomStaffPosTarget = bottomTarget;

    const auto staffOrder = document->getScrollViewStaves(partId);
    if (calcEntryIsAbove(result.bottomEntry, result.topEntry, staffOrder)) {
        std::swap(result.topEntry, result.bottomEntry);
        result.clampedToPartOrStaffBounds = true;
    }

    return result;
}

} // namespace musx::util
