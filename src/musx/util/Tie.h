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
#pragma once

#include <optional>

#include "musx/dom/EnumClasses.h"

namespace musx {
namespace dom {
class NoteInfoPtr;
}
namespace util {

class Tie
{
public:
    /// @brief Calculates the default tie direction for the specified note.
    /// @param noteInfo The note whose default tie direction is being calculated.
    /// @param forTieEnd If true, the tie-end contour is returned; otherwise the tie-start contour.
    /// @return The contour direction that Finale would choose for the tie, or
    ///         #dom::CurveContourDirection::Unspecified when the contour cannot be determined.
    [[nodiscard]]
    static dom::CurveContourDirection calcDefaultDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd);

    /// @brief Calculates the effective tie direction taking into account overrides and special rules.
    /// @param noteInfo The note whose effective tie direction is being calculated.
    /// @param forTieEnd If true, uses the tie-end context; otherwise uses the tie-start context.
    /// @return The contour direction that currently applies to the tie, or
    ///         #dom::CurveContourDirection::Unspecified when the contour cannot be determined.
    [[nodiscard]]
    static dom::CurveContourDirection calcEffectiveDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd);

    /// @brief Chooses the connect style type that Finale would use for this tie point.
    /// @param noteInfo The note whose tie endpoint is being analyzed.
    /// @param forTieEnd True for the tie-end endpoint; false for the tie-start endpoint.
    /// @returns The calculated connection style type, or std::nullopt when no matching tie exists.
    [[nodiscard]]
    static std::optional<dom::TieConnectStyleType> calcConnectStyleType(const dom::NoteInfoPtr& noteInfo, bool forTieEnd);
};

} // namespace util
} // namespace musx
