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
#pragma once

#include <functional>

#include "musx/dom/DocumentElement.h"
#include "musx/dom/MusxInstance.h"

namespace musx::util {

/// @brief Request for resolving one staff origin relative to another staff origin.
struct StaffOriginOffsetRequest
{
    dom::StaffCmper sourceStaffId{}; ///< Staff whose vertical origin is the zero point for the returned offset.
    dom::StaffCmper targetStaffId{}; ///< Staff whose vertical origin is requested.
    dom::MeasCmper measureId{}; ///< Measure at which the staff origins are needed.
    dom::Edu eduPosition{}; ///< EDU position within @ref measureId at which the staff origins are needed.
};

/// @brief Resolver decision for a staff origin offset request.
enum class StaffOriginOffsetResolverDecision
{
    NotHandled,  ///< Resolver did not handle the request; caller should use fallback behavior.
    Unavailable, ///< Resolver handled the request and the staff origin offset is unavailable in the caller's layout.
    Offset       ///< Resolver handled the request and returned a source-relative staff origin offset in EVPU.
};

/// @brief Result returned by a staff origin offset resolver.
struct StaffOriginOffsetResolverResult
{
    StaffOriginOffsetResolverDecision decision{StaffOriginOffsetResolverDecision::NotHandled}; ///< Resolver decision.
    double targetOriginOffsetEvpu{}; ///< Target staff origin offset in EVPU when @ref decision is @ref StaffOriginOffsetResolverDecision::Offset.
};

/// @brief Callback type for caller-provided staff origin offset resolution.
using StaffOriginOffsetResolver = std::function<StaffOriginOffsetResolverResult(
    const dom::DocumentPtr& document,
    dom::Cmper partId,
    const StaffOriginOffsetRequest& request)>;

/// @brief Return the system staff list for @p measureId, falling back to Scroll View if no system list is available.
[[nodiscard]]
dom::MusxInstanceList<dom::others::StaffUsed> calcSystemStavesOrScrollView(
    const dom::DocumentPtr& document,
    dom::Cmper partId,
    dom::MeasCmper measureId);

/// @brief Resolve a staff origin offset request using musxdom's system-staff proxy logic.
/// @details Uses the @ref dom::others::StaffUsed list for the system containing @ref StaffOriginOffsetRequest::measureId
/// when available, falling back to Scroll View only when no system staff list can be resolved.
[[nodiscard]]
StaffOriginOffsetResolverResult calcStaffOriginOffsetUsingSystemStaffProxy(
    const dom::DocumentPtr& document,
    dom::Cmper partId,
    const StaffOriginOffsetRequest& request);

/// @brief Compatibility wrapper for the original scroll-view proxy API.
/// @details This now delegates to @ref calcStaffOriginOffsetUsingSystemStaffProxy.
[[nodiscard]]
StaffOriginOffsetResolverResult calcStaffOriginOffsetUsingScrollViewProxy(
    const dom::DocumentPtr& document,
    dom::Cmper partId,
    const StaffOriginOffsetRequest& request);

} // namespace musx::util
