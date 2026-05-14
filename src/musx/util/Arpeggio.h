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

#include <functional>
#include <optional>

#include "musx/dom/Details.h"
#include "musx/dom/Entries.h"
#include "musx/dom/Others.h"

namespace musx::util {

/// @brief Resolved arpeggio span for one articulation assignment.
struct ArpeggioSpanCandidate
{
    dom::EntryInfoPtr sourceEntry; ///< Entry that owns the articulation assignment.
    dom::MusxInstance<dom::details::ArticulationAssign> assign; ///< Source articulation assignment.
    dom::MusxInstance<dom::others::ArticulationDef> definition; ///< Resolved articulation definition.
    dom::EntryInfoPtr topEntry; ///< Chosen top entry for the arpeggio span.
    dom::EntryInfoPtr bottomEntry; ///< Chosen bottom entry for the arpeggio span.
    int topStaffPosTarget{}; ///< Calculated top target position in source-relative staff-position units; smaller is higher.
    int bottomStaffPosTarget{}; ///< Calculated bottom target position in source-relative staff-position units; smaller is higher.
    bool clampedToPartOrStaffBounds{}; ///< True if the final result was normalized to a valid top/bottom ordering.
};

/// @brief Options controlling arpeggio span resolution.
struct ArpeggioSpanOptions
{
    bool includeHiddenEntries{false}; ///< Include hidden entries in candidate matching.
    bool skipGraceEntries{true}; ///< Skip grace-note source entries and grace-note candidates.
    bool constrainToSourceStaffOnly{false}; ///< Restrict matching to entries on the source staff only.
};

/// @brief Resolve top/bottom entries for one articulation assignment interpreted as an arpeggio.
[[nodiscard]]
std::optional<ArpeggioSpanCandidate> calcArpeggioSpanForAssignment(
    const dom::EntryInfoPtr& sourceEntry,
    const dom::MusxInstance<dom::details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options = {},
    std::function<bool(const dom::details::ArticulationAssign::SelectedSymbolContext&)> symbolContextFilter = {});

} // namespace musx::util
