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

/// @brief Resolved or synthesized arpeggio roll direction.
enum class ArpeggioDirection
{
    Auto, ///< Direction is automatic or unknown.
    Up,   ///< Roll from bottom to top.
    Down  ///< Roll from top to bottom.
};

/// @brief Resolved or synthesized arpeggio arrowhead state.
enum class ArpeggioArrow
{
    Auto, ///< Arrow state is automatic or unknown.
    None, ///< No arrowhead.
    Up,   ///< Upward arrowhead.
    Down  ///< Downward arrowhead.
};

/// @brief High-level vertical connector type resolved for a span.
enum class ArpeggioSpanType
{
    Normal,  ///< Arpeggio-like vertical connector.
    Bracket  ///< Non-arpeggio bracket-like vertical connector.
};

/// @brief Resolved arpeggio span for one articulation assignment.
struct ArpeggioSpanCandidate
{
    ArpeggioSpanType type{ArpeggioSpanType::Normal}; ///< Connector type for the resolved span.
    dom::EntryInfoPtr sourceEntry; ///< Entry that owns the articulation assignment.
    dom::EntryInfoPtr topEntry; ///< Chosen top entry for the arpeggio span.
    dom::EntryInfoPtr bottomEntry; ///< Chosen bottom entry for the arpeggio span.
    ArpeggioDirection direction{ArpeggioDirection::Auto}; ///< Roll direction when known.
    ArpeggioArrow arrow{ArpeggioArrow::Auto}; ///< Arrowhead state when known.
};

/// @brief Options controlling arpeggio span resolution.
struct ArpeggioSpanOptions
{
    bool includeHiddenEntries{false}; ///< Include hidden entries in candidate matching.
    bool skipGraceEntries{false}; ///< Skip grace-note source entries and grace-note candidates.
    bool constrainToSourceStaffOnly{false}; ///< Restrict matching to entries on the source staff only.
};

/// @brief Resolve top/bottom entries for one articulation assignment interpreted as an arpeggio.
[[nodiscard]]
std::optional<ArpeggioSpanCandidate> calcArpeggioSpanForAssignment(
    const dom::EntryInfoPtr& sourceEntry,
    const dom::MusxInstance<dom::details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options = {},
    std::function<bool(const dom::details::ArticulationAssign::SelectedSymbolContext&)> symbolContextFilter = {});

[[nodiscard]]
std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForAssignment(
    const dom::EntryInfoPtr& sourceEntry,
    const dom::MusxInstance<dom::details::ArticulationAssign>& assign,
    const ArpeggioSpanOptions& options = {});

[[nodiscard]]
std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForAssignment(
    const dom::EntryInfoPtr& sourceEntry,
    const dom::MusxInstance<dom::others::MeasureExprAssign>& assign,
    const ArpeggioSpanOptions& options = {});

[[nodiscard]]
std::optional<ArpeggioSpanCandidate> calcNonArpeggioSpanForSmartShape(
    const dom::EntryInfoPtr& sourceEntry,
    const dom::MusxInstance<dom::others::SmartShape>& smartShape,
    const ArpeggioSpanOptions& options = {});

} // namespace musx::util
