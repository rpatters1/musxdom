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

#include "musx/musx.h"

namespace musx {
namespace dom {

// **********************
// ***** SmartShape *****
// **********************

EntryInfoPtr others::SmartShape::EndPoint::calcAssociatedEntry(Cmper forPartId) const
{
    EntryInfoPtr result;
    if (entryNumber != 0) {
        result = EntryInfoPtr::fromPositionOrNull(getDocument(), forPartId, staffId, measId, entryNumber);
    } else if (auto gfhold = details::GFrameHoldContext(getDocument(), forPartId, staffId, measId)) {
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) {
            unsigned eduDiff = static_cast<unsigned>(std::labs(eduPosition - entryInfo->elapsedDuration.calcEduDuration()));
            if (eduDiff <= 1) {
                result = entryInfo;
                return false; // stop iterating
            }
            return true;
        });
    }
    if (!result && entryNumber != 0) {
        MUSX_INTEGRITY_ERROR("SmartShape at Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId)
            + " contains endpoint with invalid entry number " + std::to_string(entryNumber));
    }
    return result;
}

std::shared_ptr<others::SmartShapeMeasureAssign> others::SmartShape::EndPoint::getMeasureAssignment() const
{
    if (auto measure = getDocument()->getOthers()->get<others::Measure>(getPartId(), measId)) {
        if (measure->hasSmartShape) {
            auto assigns = getDocument()->getOthers()->getArray<others::SmartShapeMeasureAssign>(getPartId(), measId);
            Cmper shapeId = getParent()->getCmper();
            for (const auto& assign : assigns) {
                if (assign->shapeNum == shapeId) {
                    return assign;
                }
            }
        }
    }
    return nullptr;
}

std::shared_ptr<details::SmartShapeEntryAssign> others::SmartShape::EndPoint::getEntryAssignment() const
{
    if (entryNumber != 0) {
        if (auto entry = getDocument()->getEntries()->get(entryNumber)) {
            if (entry->smartShapeDetail) {
                auto assigns = getDocument()->getDetails()->getArray<details::SmartShapeEntryAssign>(getPartId(), entryNumber);
                Cmper shapeId = getParent()->getCmper();
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

bool others::SmartShape::EndPoint::calcIsAssigned() const
{
    if (!getMeasureAssignment()) {
        return false;
    }
    if (entryNumber != 0 && !getEntryAssignment()) {
        return false;
    }
    return true;
}

util::Fraction others::SmartShape::EndPoint::calcPosition() const
{
    if (!entryNumber) {
        return util::Fraction::fromEdu(eduPosition);
    }
    if (auto entryInfo = calcAssociatedEntry(getPartId())) {
        return entryInfo->elapsedDuration;
    }
    return 0;
}

util::Fraction others::SmartShape::EndPoint::calcGlobalPosition() const
{
    if (!entryNumber) {
        const auto rawPosition = util::Fraction::fromEdu(eduPosition);
        if (auto meas = getDocument()->getOthers()->get<others::Measure>(getPartId(), measId)) {
            return rawPosition * meas->calcTimeStretch(staffId);
        }
        return rawPosition;
    }
    if (auto entryInfo = calcAssociatedEntry(getPartId())) {
        return entryInfo.calcGlobalElapsedDuration();
    }
    return 0;
}

bool others::SmartShape::calcAppliesTo(const EntryInfoPtr& entryInfo) const
{
    auto entry = entryInfo->getEntry();
    if (entryBased) {
        if (entry->getEntryNumber() == startTermSeg->endPoint->entryNumber) return true;
        if (entry->getEntryNumber() == endTermSeg->endPoint->entryNumber) return true;
    }
    if (entryInfo.getStaff() != startTermSeg->endPoint->staffId && entryInfo.getStaff() != endTermSeg->endPoint->staffId) {
        return false;
    }
    if (auto meas = entry->getDocument()->getOthers()->get<others::Measure>(entryInfo.getFrame()->getRequestedPartId(), entryInfo.getMeasure())) {
        if (meas->hasSmartShape) {
            auto shapeAssigns = entry->getDocument()->getOthers()->getArray<others::SmartShapeMeasureAssign>(entryInfo.getFrame()->getRequestedPartId(), entryInfo.getMeasure());
            for (const auto& asgn : shapeAssigns) {
                if (asgn->shapeNum == getCmper()) {
                    if (entryInfo.getMeasure() > startTermSeg->endPoint->measId && entryInfo.getMeasure() < endTermSeg->endPoint->measId) {
                        return true;
                    } else if (entryInfo.getMeasure() == startTermSeg->endPoint->measId && entryInfo.getMeasure() == endTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration >= startTermSeg->endPoint->calcPosition()
                               && entryInfo->elapsedDuration <= endTermSeg->endPoint->calcPosition();
                    } else if (entryInfo.getMeasure() == startTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration >= startTermSeg->endPoint->calcPosition();
                    } else if (entryInfo.getMeasure() == endTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration <= endTermSeg->endPoint->calcPosition();
                    }
                }
            }
        }
    }
    return false;
}

// ******************&*************
// ***** SmartShapeCustomLine *****
// ********************************

util::EnigmaParsingContext others::SmartShapeCustomLine::getRawTextCtx(Cmper forPartId, Cmper rawTextId) const
{
    if (auto rawText = getDocument()->getTexts()->get<texts::SmartShapeText>(rawTextId)) {
        return rawText->getRawTextCtx(forPartId);
    }
    return {};
}

} // namespace dom    
} // namespace musx
