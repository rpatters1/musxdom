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
#include <vector>
#include <cstdlib>
#include <exception>

#include "musx/musx.h"

namespace musx {
namespace dom {

// ***************************************
// ***** MusxInstanceList<StaffUsed> *****
// ***************************************

MusxInstance<others::Staff> MusxInstanceList<others::StaffUsed>::getStaffInstanceAtIndex(size_t index) const
{
    const auto& iuArray = *this;
    if (index >= iuArray.size()) return nullptr;
    auto iuItem = iuArray[index];
    return iuItem->getStaffInstance();
}

MusxInstance<others::StaffComposite> MusxInstanceList<others::StaffUsed>::getStaffInstanceAtIndex(size_t index, MeasCmper measureId, Edu eduPosition) const
{
    const auto& iuArray = *this;
    if (index >= iuArray.size()) return nullptr;
    auto iuItem = iuArray[index];
    return iuItem->getStaffInstance(measureId, eduPosition);
}

std::optional<size_t> MusxInstanceList<others::StaffUsed>::getIndexForStaff(StaffCmper staffId) const
{
    const auto it = std::find_if(this->begin(), this->end(),
        [&](const auto& inst) {
            assert(inst && "inst should never be null");
            return inst->staffId == staffId;
        });
    if (it != this->end()) {
        return static_cast<size_t>(std::distance(this->begin(), it));
    }
    return std::nullopt;
}

StaffCmper MusxInstanceList<others::StaffUsed>::getTopStaffId() const
{
    return this->empty() ? 0 : this->front()->staffId;
}

StaffCmper MusxInstanceList<others::StaffUsed>::getBottomStaffId() const
{
    return this->empty() ? 0 : this->back()->staffId;
}

bool MusxInstanceList<others::StaffUsed>::iterateEntries(size_t startIndex, size_t endIndex, const MusicRange& range, std::function<bool(const EntryInfoPtr&)> iterator) const
{
    MUSX_ASSERT_IF(startIndex >= this->size() || endIndex >= this->size()) {
        throw std::logic_error("Start index [" + std::to_string(startIndex) + "] or end index [" + std::to_string(endIndex) + "] is out of range.");
    }

    using S = std::ptrdiff_t;
    const S start = static_cast<S>(startIndex);
    const S end = static_cast<S>(endIndex);
    const S step = start > end ? -1 : 1;

    for (S x = start; ; x += step) {
        const StaffCmper staffId = (*this)[static_cast<size_t>(x)]->staffId;
        for (MeasCmper nextMeasId = range.startMeasureId; nextMeasId <= range.endMeasureId; nextMeasId++) {
            if (auto gfHold = details::GFrameHoldContext(getDocument(), getRequestedPartId(), staffId, nextMeasId)) {
                const bool result = gfHold.iterateEntries([&](const EntryInfoPtr& entryInfo) -> bool {
                    if (range.contains(nextMeasId, entryInfo.calcGlobalElapsedDuration())) {
                        if (!iterator(entryInfo)) {
                            return false;
                        }
                    }
                    return true;
                });
                if (!result) {
                    return false;
                }
            }
        }
        if (x == end) {
            break;
        }
    }
    return true;
}

} // namespace dom
} // namespace musx