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

// ***********************
// ***** OssiaHeader *****
// ***********************

MusxInstance<TimeSignature> OssiaHeader::createTimeSignature() const
{
    // Ossias do not appear to have an option to abbreviate cut/common time. If we discover a way to do it, either
    // - remove the `false` parameter, which will cause it to come from TimeSignatureOptions, or
    // - calculate the the abbreviated value and pass it instead of `false`.
    // However, as observed in F27.4, even setting the value in TimeSignatureOptions does not cause 2/2 to be abbreviated to cut time.
    return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, compositeNumerator, compositeDenominator,
        TimeSignature::Abbreviation::Numeric));
}

MusxInstanceList<details::Bracket> OssiaHeader::getBrackets() const
{
    // It appears that cmper2 is always zero. The fact that there is even a cmper2 at all may be a holdover
    // from some legacy plan to use top-level brackets with staff groups rather than embedding the brackets
    // in the staff groups. Or maybe there was an ancient design where brackets were entirely independent
    // of staff groups and instead attached to system or staves. We may never know the reason.
    return getDocument()->getDetails()->getArray<details::Bracket>(getRequestedPartId(), bracketGroup, 0);
}

} // namespace others
} // namespace dom
} // namespace musx
