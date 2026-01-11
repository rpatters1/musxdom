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

#include <variant>

#include "musx/musx.h"

namespace musx {
namespace utils {

bool calcIsPseudoBackwardTie(dom::Evpu startOffset, dom::Evpu endOffset)
{
    if (endOffset < startOffset) {
        return true;
    }
    // End may be left of the EDU position, but not more than one space to the right.
    // Start must be at least one space to the left.
    constexpr auto MAX_LEFT = -3 * dom::EVPU_PER_SPACE; // adjust value if necessary
    return endOffset <= dom::EVPU_PER_SPACE && endOffset >= MAX_LEFT
        && startOffset <= -dom::EVPU_PER_SPACE;
}

bool calcIsPseudoForwardTie(dom::Evpu startOffset, dom::Evpu endOffset)
{
    return endOffset > startOffset && startOffset > -dom::EVPU_PER_SPACE;
}

dom::Evpu PseudoTieShapeInfo::calcWidthOffset() const
{
    constexpr dom::Evpu FALLBACK = static_cast<dom::Evpu>(dom::EVPU_PER_SPACE);
    MUSX_ASSERT_IF(!shape) {
        throw std::logic_error("PseudoTieShapeInfo::shape is null.");
    }
    const auto width = shape->calcWidth().value_or(FALLBACK);
    return (shapeType == dom::KnownShapeDefType::SlurTieCurveLeft) ? -width : width;
}

} // namespace utils
} // namespace musx
