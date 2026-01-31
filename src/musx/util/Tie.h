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
#include <utility>

#include "musx/dom/Details.h"
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

    /// @brief Calculates the connect style types for both endpoints of a tie.
    /// @param noteInfo The note whose tie is being analyzed.
    /// @param forTieEnd True if the note represents a tie end; false for tie starts.
    /// @returns A pair containing start and end connect style types.
    [[nodiscard]]
    static std::pair<dom::TieConnectStyleType, dom::TieConnectStyleType> calcConnectStyleTypes(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd);

    /// @brief Calculates the default connection type for a tie endpoint.
    /// @param noteInfo The note whose endpoint is being analyzed.
    /// @param forTieEnd True if the endpoint is for a tie end; false for a tie start.
    /// @param forEndPoint True if calculating the end point; false for the start point.
    /// @param forPageView True if calculating with page view system breaks, false for scroll/studio view.
    /// @returns The connection type, or std::nullopt if no applicable tie exists.
    [[nodiscard]]
    static std::optional<dom::details::TieAlterBase::ConnectionType> calcConnectionType(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint, bool forPageView = false);

private:
    /// @brief Calculates the connect style type for a single endpoint of a tie.
    /// @param noteInfo The note whose tie is being analyzed.
    /// @param forTieEnd True if the note represents a tie end; false for tie starts.
    /// @param forEndPoint True to calculate the end point; false for the start point.
    /// @returns The calculated connection style type, or std::nullopt when no matching tie exists.
    [[nodiscard]]
    static std::optional<dom::TieConnectStyleType> calcConnectStyleTypeAtEndPoint(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint);
};

} // namespace util
} // namespace musx
