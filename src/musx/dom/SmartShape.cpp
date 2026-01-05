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
#include <string>
#include <cmath>
#include <limits>

#include "musx/musx.h"

namespace musx {
namespace dom {

// ********************
// ***** EndPoint *****
// ********************

EntryInfoPtr smartshape::EndPoint::calcAssociatedEntry(Cmper forPartId, bool findExact) const
{
    EntryInfoPtr result;
    if (entryNumber != 0) {
        result = EntryInfoPtr::fromEntryNumber(getDocument(), forPartId, entryNumber);
        if (!result) {
            MUSX_INTEGRITY_ERROR("SmartShape at Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId)
                + " contains endpoint with invalid entry number " + std::to_string(entryNumber));
        }
    } else if (auto gfHold = details::GFrameHoldContext(getDocument(), forPartId, staffId, measId)) {
        result = gfHold.calcNearestEntry(util::Fraction::fromEdu(eduPosition), findExact);
    }
    return result;
}

MusxInstance<others::SmartShapeMeasureAssign> smartshape::EndPoint::getMeasureAssignment() const
{
    auto shapeParent = getParent<others::SmartShape>();
    MUSX_ASSERT_IF (!shapeParent) {
        throw std::logic_error("Unknown parent type for SmartShape::EndPoint.");
    }
    if (auto measure = getDocument()->getOthers()->get<others::Measure>(shapeParent->getRequestedPartId(), measId)) {
        if (measure->hasSmartShape) {
            auto assigns = getDocument()->getOthers()->getArray<others::SmartShapeMeasureAssign>(shapeParent->getRequestedPartId(), measId);
            for (const auto& assign : assigns) {
                if (assign->shapeNum == shapeParent->getCmper()) {
                    return assign;
                }
            }
        }
    }
    return nullptr;
}

MusxInstance<details::SmartShapeEntryAssign> smartshape::EndPoint::getEntryAssignment() const
{
    auto shapeParent = getParent<others::SmartShape>();
    MUSX_ASSERT_IF (!shapeParent) {
        throw std::logic_error("Unknown parent type for SmartShape::EndPoint.");
    }
    if (entryNumber != 0) {
        Cmper shapeId = shapeParent->getCmper();
        if (auto entry = getDocument()->getEntries()->get(entryNumber)) {
            if (entry->smartShapeDetail) {
                auto assigns = getDocument()->getDetails()->getArray<details::SmartShapeEntryAssign>(shapeParent->getRequestedPartId(), entryNumber);
                for (const auto& assign : assigns) {
                    if (assign->shapeNum == shapeId) {
                        return assign;
                    }
                }
            }
        }
    }
    return nullptr;
}

bool smartshape::EndPoint::calcIsAssigned() const
{
    if (!getMeasureAssignment()) {
        return false;
    }
    if (entryNumber != 0 && !getEntryAssignment()) {
        return false;
    }
    return true;
}

util::Fraction smartshape::EndPoint::calcPosition() const
{
    if (!entryNumber) {
        return util::Fraction::fromEdu(eduPosition);
    }
    auto shapeParent = getParent<others::SmartShape>();
    MUSX_ASSERT_IF (!shapeParent) {
        throw std::logic_error("Unknown parent type for SmartShape::EndPoint.");
    }
    if (auto entryInfo = calcAssociatedEntry(shapeParent->getRequestedPartId())) {
        return entryInfo->elapsedDuration;
    }
    return 0;
}

util::Fraction smartshape::EndPoint::calcGlobalPosition() const
{
    auto shapeParent = getParent<others::SmartShape>();
    MUSX_ASSERT_IF (!shapeParent) {
        throw std::logic_error("Unknown parent type for SmartShape::EndPoint.");
    }
    if (!entryNumber) {
        const auto rawPosition = util::Fraction::fromEdu(eduPosition);
        if (auto meas = getDocument()->getOthers()->get<others::Measure>(shapeParent->getRequestedPartId(), measId)) {
            return rawPosition * meas->calcTimeStretch(staffId);
        }
        return rawPosition;
    }
    if (auto entryInfo = calcAssociatedEntry(shapeParent->getRequestedPartId())) {
        return entryInfo.calcGlobalElapsedDuration();
    }
    return 0;
}

int smartshape::EndPoint::compareMetricPosition(const EndPoint& other) const
{
    if (this->entryNumber != 0 && other.entryNumber != 0 && this->entryNumber == other.entryNumber) {
        MUSX_ASSERT_IF(measId != other.measId) {
            // nothing to do, but trap it in debug mode.
        }
        return 0;
    }
    if (measId != other.measId) {
        return (measId < other.measId) ? -1 : +1;
    }
    const auto thisPosition = this->calcPosition();
    const auto otherPosition = other.calcPosition();
    if (thisPosition != otherPosition) {
        return (thisPosition < otherPosition) ? -1 : +1;
    }
    return 0;
}

// **********************
// ***** SmartShape *****
// **********************

bool others::SmartShape::calcAppliesTo(const EntryInfoPtr& entryInfo) const
{
    auto entry = entryInfo->getEntry();
    if (entryBased) {
        if (entry->getEntryNumber() == startTermSeg->endPoint->entryNumber) return true;
        if (entry->getEntryNumber() == endTermSeg->endPoint->entryNumber) return true;
    }
    const StaffCmper entryStaffId = entryInfo.getStaff();
    const MeasCmper entryMeasureId = entryInfo.getMeasure();
    if (entryStaffId != startTermSeg->endPoint->staffId && entryStaffId != endTermSeg->endPoint->staffId) {
        return false;
    }
    if (entryMeasureId < startTermSeg->endPoint->measId || entryMeasureId > endTermSeg->endPoint->measId) {
        return false;
    }
    const auto entryPos = entryInfo->elapsedDuration;
    auto range = createMusicRange();
    if (auto meas = entry->getDocument()->getOthers()->get<others::Measure>(entryInfo.getFrame()->getRequestedPartId(), entryInfo.getMeasure())) {
        if (meas->hasSmartShape) {
            auto shapeAssigns = entry->getDocument()->getOthers()->getArray<others::SmartShapeMeasureAssign>(entryInfo.getFrame()->getRequestedPartId(), entryInfo.getMeasure());
            for (const auto& asgn : shapeAssigns) {
                if (asgn->shapeNum == getCmper()) {
                    if (range.contains(entryMeasureId, entryPos)) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

MusicRange others::SmartShape::createMusicRange() const
{
    return MusicRange(getDocument(), startTermSeg->endPoint->measId, startTermSeg->endPoint->calcPosition(), endTermSeg->endPoint->measId, endTermSeg->endPoint->calcPosition());
}

MusicRange others::SmartShape::createGlobalMusicRange() const
{
    return MusicRange(getDocument(), startTermSeg->endPoint->measId, startTermSeg->endPoint->calcGlobalPosition(), endTermSeg->endPoint->measId, endTermSeg->endPoint->calcGlobalPosition());
}

bool others::SmartShape::iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator, DeferredReference<MusxInstanceList<others::StaffUsed>> staffList) const
{
    if (!staffList) {
        staffList.emplace(getDocument()->getScrollViewStaves(getRequestedPartId()));
    }
    auto startIndex = staffList->getIndexForStaff(startTermSeg->endPoint->staffId);
    auto endIndex = staffList->getIndexForStaff(endTermSeg->endPoint->staffId);
    MUSX_ASSERT_IF(!startIndex || !endIndex) {
        throw std::logic_error("Smart shape spans staves that do not exist on the supplied staff list.");
    }
    return staffList->iterateEntries(*startIndex, *endIndex, createGlobalMusicRange(), iterator);
}

bool others::SmartShape::calcIsPotentialTie() const
{
    // 6 EVPU ≈ 1/4 staff space; threshold is applied strictly (|Δ| < 6 EVPU).
    constexpr Evpu HORIZONTAL_THRESHOLD = 6;

    switch (shapeType) {
    case ShapeType::SlurAuto:
    case ShapeType::SlurDown:
    case ShapeType::SlurUp:
        break;

    default:
        return false;
    }

    if (entryBased) {
        // both ends must be the same entry for entry-attached
        if (startTermSeg->endPoint->entryNumber != endTermSeg->endPoint->entryNumber) {
            return false;
        }
    } else {
        if (startTermSeg->endPoint->staffId != endTermSeg->endPoint->staffId) {
            return false;
        }
    }
    // both ends must match up with an equivalent entry for beat-attached
    auto startEntry = startTermSeg->endPoint->calcAssociatedEntry(getRequestedPartId());
    auto endEntry = endTermSeg->endPoint->calcAssociatedEntry(getRequestedPartId());
    if (!startEntry || !endEntry) {
        return false;
    }
    if (startEntry.calcDisplaysAsRest() || endEntry.calcDisplaysAsRest()) {
        return false;
    }
    // Allow arpeggiation ties (e.g., C -> CEG): end entry may add pitches, but must contain all start pitches.
    if (!endEntry.calcContainsPitchContent(startEntry)) {
        return false;
    }
    const Evpu vertDiff = startTermSeg->endPointAdj->vertOffset - endTermSeg->endPointAdj->vertOffset;
    return std::abs(vertDiff) < HORIZONTAL_THRESHOLD;
}

bool others::SmartShape::calcIsPotentialForwardTie() const
{
    if (!calcIsPotentialTie()) {
        return false;
    }
    const auto& startEndPoint = *startTermSeg->endPoint;
    const auto& endEndPoint   = *endTermSeg->endPoint;
    const int cmp = startEndPoint.compareMetricPosition(endEndPoint);
    if (cmp) {
        return cmp < 0;
    }
    return endTermSeg->endPointAdj->horzOffset > startTermSeg->endPointAdj->horzOffset;
}

NoteInfoPtr others::SmartShape::calcIsArpeggiatedTie() const
{
    if (!calcIsPotentialForwardTie()) {
        return {};
    }
    auto startEntry = startTermSeg->endPoint->calcAssociatedEntry(getRequestedPartId());
    MUSX_ASSERT_IF(!startEntry) {
        throw std::logic_error("calcIsPotentialForwardTie was true but startEntry did not exist.");
    }
    auto entry = startEntry->getEntry();
    MUSX_ASSERT_IF(entry->notes.empty()) {
        throw std::logic_error("calcIsPotentialForwardTie was true but startEntry had not notes.");
    }
    if (entry->notes.size() > 1) {
        return {};
    }
    auto endEntry = endTermSeg->endPoint->calcAssociatedEntry(getRequestedPartId());
    MUSX_ASSERT_IF(!startEntry) {
        throw std::logic_error("calcIsPotentialForwardTie was true but startEntry had no notes.");
    }
}

bool others::SmartShape::calcIsLaissezVibrerTie() const
{
    if (!calcIsPotentialForwardTie()) {
        return false;
    }
    return startTermSeg->endPoint->compareMetricPosition(*endTermSeg->endPoint) == 0;
}

// ********************************
// ***** SmartShapeCustomLine *****
// ********************************

util::EnigmaParsingContext others::SmartShapeCustomLine::getRawTextCtx(Cmper forPartId, Cmper rawTextId) const
{
    if (auto rawText = getDocument()->getTexts()->get<texts::SmartShapeText>(rawTextId)) {
        return rawText->getRawTextCtx(rawText, forPartId);
    }
    return {};
}

} // namespace dom
} // namespace musx
