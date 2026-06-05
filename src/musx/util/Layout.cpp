/*
 * Copyright (C) 2026, Robert Patterson
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
#include "musx/util/Layout.h"

#include "music_theory/music_theory.hpp"
#include "musx/dom/Document.h"

namespace musx::util {

StaffOriginOffsetResolverResult calcStaffOriginOffsetUsingScrollViewProxy(
    const dom::DocumentPtr& document,
    dom::Cmper partId,
    const StaffOriginOffsetRequest& request)
{
    MUSX_ASSERT_IF(!document) {
        return { StaffOriginOffsetResolverDecision::Unavailable, 0.0 };
    }

    const auto scrollView = document->getScrollViewStaves(partId);
    const auto sourceStaffIndex = scrollView.getIndexForStaff(request.sourceStaffId);
    const auto targetStaffIndex = scrollView.getIndexForStaff(request.targetStaffId);
    if (!sourceStaffIndex || !targetStaffIndex) {
        return { StaffOriginOffsetResolverDecision::Unavailable, 0.0 };
    }

    return {
        StaffOriginOffsetResolverDecision::Offset,
        static_cast<double>(scrollView[*sourceStaffIndex]->distFromTop - scrollView[*targetStaffIndex]->distFromTop)
    };
}

} // namespace musx::util
