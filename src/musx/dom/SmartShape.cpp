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

EntryInfoPtr others::SmartShape::EndPoint::calcAssociatedEntry() const
{
    EntryInfoPtr result;
    if (auto gfhold = details::GFrameHoldContext(getDocument(), getPartId(), staffId, measId)) {
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) {
            if (!entryNumber) {
                unsigned eduDiff = static_cast<unsigned>(std::labs(eduPosition - entryInfo->elapsedDuration.calcEduDuration()));
                if (eduDiff <= 1) {
                    result = entryInfo;
                    return false; // stop iterating
                }
            } else if (entryInfo->getEntry()->getEntryNumber() == entryNumber) {
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

util::Fraction others::SmartShape::EndPoint::calcPosition() const
{
    if (!entryNumber) {
        return util::Fraction::fromEdu(eduPosition);
    }
    if (auto entryInfo = calcAssociatedEntry()) {
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
    if (auto entryInfo = calcAssociatedEntry()) {
        return entryInfo.calcGlobalElapsedDuration().calcEduDuration();
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

} // namespace dom    
} // namespace musx
