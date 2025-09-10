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
namespace others {

// *****************************
// ***** PageGraphicAssign *****
// *****************************

MusxInstance<PageGraphicAssign> PageGraphicAssign::getForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId, Inci inci)
{
    if (auto part = document->getOthers()->get<PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->get<PageGraphicAssign>(partId, pageAssignmentId, inci);
    }
    return nullptr;
}

MusxInstanceList<PageGraphicAssign> PageGraphicAssign::getArrayForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId)
{
    if (auto part = document->getOthers()->get<PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->getArray<PageGraphicAssign>(partId, pageAssignmentId);
    }
    return MusxInstanceList<PageGraphicAssign>(document, partId);
}

std::optional<PageCmper> PageGraphicAssign::calcStartPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<PartDefinition>(SCORE_PARTID, forPartId)) {
        if (auto calcValue = part->calcPageNumberFromAssignmentId(getCmper() ? getCmper() : startPage)) {
            if (calcValue.value() <= part->numberOfPages) {
                return calcValue;
            }
        } else if (isMultiPage()) {
            if (auto endPageNum = calcEndPageNumber(forPartId)) {
                if (part->numberOfLeadingBlankPages < endPageNum.value()) {
                    return PageCmper(part->numberOfLeadingBlankPages) + 1;
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<PageCmper> PageGraphicAssign::calcEndPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<PartDefinition>(SCORE_PARTID, forPartId)) {
        if (isMultiAssignedThruLastPage()) {
            return PageCmper(part->numberOfPages);
        }
        if (auto calcValue = part->calcPageNumberFromAssignmentId(getCmper() ? getCmper() : endPage)) {
            if (calcValue.value() > part->numberOfPages) {
                calcValue = PageCmper(part->numberOfPages);
            }
            return calcValue;
        }
    }
    return std::nullopt;
}

} // namespace others
} // namespace dom
} // namespace musx