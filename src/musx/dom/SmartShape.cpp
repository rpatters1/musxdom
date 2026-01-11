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

EntryInfoPtr smartshape::EndPoint::calcAssociatedEntry(bool findExact) const
{
    const auto doc = getDocument();
    auto shapeParent = getParent<others::SmartShape>();
    MUSX_ASSERT_IF (!shapeParent) {
        throw std::logic_error("Unknown parent type for SmartShape::EndPoint.");
    }
    const Cmper forPartId = shapeParent->getRequestedPartId();

    EntryInfoPtr result;
    if (entryNumber != 0) {
        result = EntryInfoPtr::fromEntryNumber(doc, forPartId, entryNumber);
        if (!result) {
            MUSX_INTEGRITY_ERROR("SmartShape at Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId)
                + " contains endpoint with invalid entry number " + std::to_string(entryNumber));
        }
        return result;
    }

    auto gfHold = details::GFrameHoldContext(doc, forPartId, staffId, measId);
    if (!gfHold) {
        return {};
    }

    const auto pos = util::Fraction::fromEdu(eduPosition);
    result = gfHold.calcNearestEntry(pos, findExact);
    if (!result || findExact) {
        return result;
    }

    const auto measure = doc->getOthers()->get<others::Measure>(forPartId, result.getMeasure());
    MUSX_ASSERT_IF(!measure) {
        // measure should exist, but there is nothing to do here: trap it in debug mode
        return {};
    }
    const auto beatVal = measure->createTimeSignature(staffId)->calcBeatValueAt(eduPosition);

    constexpr auto BEAT_THRESHOLD_MULTIPLIER = util::Fraction(1, 4); // modify this if too many false positives or negatives
    const auto beatThreshold = beatVal * BEAT_THRESHOLD_MULTIPLIER;
    using std::abs;
    if (abs(result->elapsedDuration - pos) > beatThreshold) {
        return {};
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
    if (auto entryInfo = calcAssociatedEntry()) {
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
    if (auto entryInfo = calcAssociatedEntry()) {
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

// ******************************
// ***** EndPointAdjustment *****
// ******************************

bool smartshape::EndPointAdjustment::calcHasVerticalEquivalentConnection(const smartshape::EndPointAdjustment& other) const
{
    using E = EntryConnectionType;

    const auto verticalGroup = [](E v) -> int {
        switch (v) {
        // System: always comparable with each other
        case E::SystemLeft:
        case E::SystemRight:
            return 100;

        // Head rectangle
        case E::HeadLeftTop:
        case E::HeadRightTop:
            return 200;
        case E::HeadLeftBottom:
        case E::HeadRightBottom:
            return 201;

        // Stem rectangle
        case E::StemLeftTop:
        case E::StemRightTop:
            return 210;
        case E::StemLeftBottom:
        case E::StemRightBottom:
            return 211;

        // Notehead
        case E::NoteLeftTop:
        case E::NoteRightTop:
            return 220;
        case E::NoteLeftBottom:
        case E::NoteRightBottom:
            return 221;
        case E::NoteLeftCenter:
        case E::NoteRightCenter:
            return 222;

        // Lyrics (obvious groupings)
        case E::LyricLeftCenter:
        case E::LyricRightCenter:
            return 230;

        case E::HeadRightLyricBaseline:
        case E::DotRightLyricBaseline:
        case E::DurationLyricBaseline:
            return 231;

        case E::LyricRightBottom:
            return 232;
        }

        return -1; // should be unreachable if enum is complete
    };

    return verticalGroup(contextEntCnct) == verticalGroup(other.contextEntCnct);
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

bool others::SmartShape::calcIsPotentialTie(const EntryInfoPtr& forStartEntry) const
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
        const auto startEntryNum = startTermSeg->endPoint->entryNumber;
        if (startEntryNum != endTermSeg->endPoint->entryNumber || startEntryNum != forStartEntry->getEntry()->getEntryNumber()) {
            return false;
        }
    }  else {
        if (startTermSeg->endPoint->staffId != endTermSeg->endPoint->staffId) {
            return false;
        }
        const auto thisStartEntry = startTermSeg->endPoint->calcAssociatedEntry();
        if (!forStartEntry.isSameEntry(thisStartEntry)) {
            return false;
        }
    }
    // both ends must match up with an equivalent entry for beat-attached
    auto endEntry = endTermSeg->endPoint->calcAssociatedEntry();
    if (!endEntry) {
        return false;
    }
    if (forStartEntry.calcDisplaysAsRest() || endEntry.calcDisplaysAsRest()) {
        return false;
    }
    // Allow arpeggiation ties (e.g., C -> CEG): end entry may add pitches, but must contain all start pitches.
    if (!endEntry.calcContainsPitchContent(forStartEntry)) {
        return false;
    }
    // if either endpoint is inactive, we can't compare the vertical positions, so just permissively allow them
    // for now.
    if (entryBased) {
        if (!startTermSeg->endPointAdj->active || !endTermSeg->endPointAdj->active) {
            return true;
        }
    }
    const Evpu vertDiff = startTermSeg->endPointAdj->calcVertOffset() - endTermSeg->endPointAdj->calcVertOffset();
    return std::abs(vertDiff) < HORIZONTAL_THRESHOLD;
}

bool others::SmartShape::calcIsPotentialForwardTie(const EntryInfoPtr& forStartEntry) const
{
    if (!calcIsPotentialTie(forStartEntry)) {
        return false;
    }
    const auto& startEndPoint = *startTermSeg->endPoint;
    const auto& endEndPoint   = *endTermSeg->endPoint;
    const int cmp = startEndPoint.compareMetricPosition(endEndPoint);
    if (cmp) {
        return cmp < 0;
    }
    const auto startOffset = startTermSeg->endPointAdj->calcHorzOffset();
    const auto endOffset = endTermSeg->endPointAdj->calcHorzOffset();
    return utils::calcIsPseudoForwardTie(startOffset, endOffset);
}

NoteInfoPtr others::SmartShape::calcArpeggiatedTieToNote(const EntryInfoPtr& forStartEntry) const
{
    if (!calcIsPotentialForwardTie(forStartEntry)) {
        return {};
    }
    const auto entry = forStartEntry->getEntry();
    MUSX_ASSERT_IF(entry->notes.empty()) {
        throw std::logic_error("calcIsPotentialForwardTie was true but startEntry had no notes.");
    }
    if (entry->notes.size() != 1) {
        return {};
    }
    const auto endEntry = endTermSeg->endPoint->calcAssociatedEntry();
    MUSX_ASSERT_IF(!endEntry) {
        throw std::logic_error("calcIsPotentialForwardTie was true but endEntry did not exist.");
    }
    if (forStartEntry.isSameEntry(endEntry)) {
        return {}; // must be a different entry.
    }
    const auto noteInfoPtr = NoteInfoPtr(forStartEntry, 0);
    if (noteInfoPtr.calcTieTo()) {
        // If Finale already provides a real tie target, this smart shape is not a stand-in.
        return {};
    }
    auto endNote = noteInfoPtr.findEqualPitch(endEntry);
    MUSX_ASSERT_IF(!endNote) {
        throw std::logic_error("calcIsPotentialForwardTie was true but no matching pitch was found in endEntry.");
    }
    return endNote;
}

CurveContourDirection others::SmartShape::calcContourDirection() const
{
    using ST = ShapeType;
    switch (shapeType) {
    case ST::SlurUp:
    case ST::DashSlurUp:
    case ST::DashContourSlurUp:
    case ST::OctaveUp:
    case ST::TwoOctaveUp:
        return CurveContourDirection::Up;

    case ST::SlurDown:
    case ST::DashSlurDown:
    case ST::DashContourSlurDown:
    case ST::OctaveDown:
    case ST::TwoOctaveDown:
        return CurveContourDirection::Down;

    default:
        return CurveContourDirection::Auto;
    }
}

bool others::SmartShape::calcIsLaissezVibrerTie(const EntryInfoPtr& forStartEntry) const
{
    if (!calcIsPotentialForwardTie(forStartEntry)) {
        return false;
    }
    if (startTermSeg->endPoint->compareMetricPosition(*endTermSeg->endPoint) != 0) {
        return false;
    }
    return true;
}

bool others::SmartShape::calcIsUsedAsTieEnd(const EntryInfoPtr& forStartEntry) const
{
    if (!calcIsPotentialTie(forStartEntry)) {
        return false;
    }
    if (startTermSeg->endPoint->compareMetricPosition(*endTermSeg->endPoint) != 0) {
        return false;
    }
    const auto startOffset = startTermSeg->endPointAdj->calcHorzOffset();
    const auto endOffset = endTermSeg->endPointAdj->calcHorzOffset();
    return utils::calcIsPseudoBackwardTie(startOffset, endOffset);
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
