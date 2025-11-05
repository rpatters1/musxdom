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
        result = EntryInfoPtr::fromPositionOrNull(getDocument(), forPartId, staffId, measId, entryNumber);
        if (!result) {
            MUSX_INTEGRITY_ERROR("SmartShape at Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId)
                + " contains endpoint with invalid entry number " + std::to_string(entryNumber));
        }
    } else if (auto gfhold = details::GFrameHoldContext(getDocument(), forPartId, staffId, measId)) {
        unsigned bestDiff = (std::numeric_limits<unsigned>::max)();
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) {
            if (entryInfo->getEntry()->graceNote) {
                return true; // iterate past grace notes
            }
            unsigned eduDiff = static_cast<unsigned>(std::labs(eduPosition - entryInfo->elapsedDuration.calcEduDuration()));
            if (eduDiff <= 1) {
                result = entryInfo;
                return false; // stop iterating
            }
            if (!findExact && eduDiff < bestDiff) {
                bestDiff = eduDiff;
                result = entryInfo;
            }
            return true;
        });
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
        staffList.emplace(getDocument()->getOthers()->getArray<others::StaffUsed>(getRequestedPartId(), BASE_SYSTEM_ID));
    }
    auto startIndex = staffList->getIndexForStaff(startTermSeg->endPoint->staffId);
    auto endIndex = staffList->getIndexForStaff(endTermSeg->endPoint->staffId);
    MUSX_ASSERT_IF(!startIndex || !endIndex) {
        throw std::logic_error("Smart shape spans staves that do not exist on the supplied staff list.");
    }
    return staffList->iterateEntries(*startIndex, *endIndex, createGlobalMusicRange(), iterator);
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
