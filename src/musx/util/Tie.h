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
#include "musx/dom/Options.h"

namespace musx {
namespace dom {
class NoteInfoPtr;
}

namespace util {

/// @class Tie
/// @brief Static class containing utilites for ties.
class Tie
{
public:
    /// @enum EndPointKind
    /// @brief Classifies what the tie endpoint attaches to for contour-length calculations.
    enum class EndPointKind
    {
        Notehead,           ///< Endpoint attaches to a notehead.
        MeasureBoundary,    ///< Endpoint attaches to a measure boundary (not a notehead).
        SystemBoundary      ///< Endpoint attaches to a system boundary (start/end of system).
    };

    /// @brief Caller-supplied horizontal geometry used for contour span classification.
    struct ContourGeometry
    {
        dom::EvpuFloat startToEndLeft{}; ///< End-left X minus start-left X in EVPU.
        dom::EvpuFloat startNoteheadWidth{}; ///< Width of the start notehead in EVPU.
        std::optional<dom::EvpuFloat> endNoteheadWidth{}; ///< Width of the end notehead in EVPU; defaults to startNoteheadWidth when omitted.
        dom::EvpuFloat startAdjustment{}; ///< Caller-supplied start endpoint adjustment in EVPU (for engine-specific spacing/system rules).
        dom::EvpuFloat endAdjustment{}; ///< Caller-supplied end endpoint adjustment in EVPU (for engine-specific spacing/system rules).
        EndPointKind startPointKind{EndPointKind::Notehead}; ///< Attachment kind for the start endpoint.
        EndPointKind endPointKind{EndPointKind::Notehead}; ///< Attachment kind for the end endpoint.
    };

    /// @brief Result of contour style classification.
    struct ContourResult
    {
        dom::options::TieOptions::ControlStyleType styleType{}; ///< Selected control style span type.
        dom::Evpu length{}; ///< Computed tie length in EVPU used for span classification.
    };

    /// @brief Control point parameters for a tie contour.
    struct ContourControlPoint
    {
        dom::Efix insetRatio{}; ///< Inset ratio (percent-based inset).
        dom::Evpu height{}; ///< Height of the control point in EVPU.
        dom::Evpu insetFixed{}; ///< Fixed inset in EVPU (used when inset style is Fixed).
    };

    /// @brief Control points for a tie contour (left/right endpoints).
    struct ContourControlPoints
    {
        ContourControlPoint left; ///< Left control point.
        ContourControlPoint right; ///< Right control point.
        dom::options::TieOptions::InsetStyle insetStyle{}; ///< Which inset value is active.
    };

    /// @brief Calculates the default tie direction for the specified note.
    /// @param noteInfo The note whose default tie direction is being calculated.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @return The contour direction that Finale would choose for the tie, or
    ///         #dom::CurveContourDirection::Unspecified when the contour cannot be determined.
    [[nodiscard]]
    static dom::CurveContourDirection calcDefaultDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd = false);

    /// @brief Calculates the effective tie direction taking into account overrides and special rules.
    /// @param noteInfo The note whose effective tie direction is being calculated.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @return The contour direction that currently applies to the tie, or
    ///         #dom::CurveContourDirection::Unspecified when the contour cannot be determined.
    [[nodiscard]]
    static dom::CurveContourDirection calcEffectiveDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd = false);

    /// @brief Calculates the connect style types for both endpoints of a tie.
    /// @param noteInfo The note whose tie is being analyzed.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @returns A pair containing start and end connect style types, or std::nullopt if no tie exists.
    [[nodiscard]]
    static std::optional<std::pair<dom::TieConnectStyleType, dom::TieConnectStyleType>> calcConnectStyleTypes(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd);

    /// @brief Returns true if the connect style is an outer placement.
    /// @param type The connect style to classify.
    /// @returns True if @p type is an outer placement, otherwise false.
    [[nodiscard]]
    static bool calcIsOuterConnectStyle(dom::TieConnectStyleType type);

    /// @brief Calculates the default connection type for a tie endpoint.
    /// @param noteInfo The note whose endpoint is being analyzed.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @param forEndPoint True if calculating the end point; false for the start point.
    /// @param forPageView True if calculating with page view system breaks, false for scroll/studio view.
    /// @returns The connection type, or std::nullopt if no applicable tie exists.
    [[nodiscard]]
    static std::optional<dom::details::TieAlterBase::ConnectionType> calcConnectionType(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint, bool forPageView = true);

    /// @brief Resolves endpoint offsets for a tie endpoint from default policy and tie-alter overrides.
    /// @param noteInfo The note whose tie alterations are consulted.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @param forEndPoint True for end-point offset, false for start-point offset.
    /// @param direction Optional tie contour direction. If omitted or Unspecified, effective direction is calculated.
    /// @param forPageView True to apply page-view system-break rules in endpoint connection evaluation.
    /// @return The effective endpoint offsets in EVPU.
    [[nodiscard]]
    static dom::options::TieOptions::ConnectStyle calcEndpointOffsets(const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint,
        std::optional<dom::CurveContourDirection> direction = std::nullopt, bool forPageView = true);

    /// @brief Calculates the tie control style span using caller-supplied horizontal geometry.
    /// @param noteInfo The note whose tie context is being evaluated.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @param geometry Horizontal endpoint geometry in EVPU. `startAdjustment` and `endAdjustment` let the caller inject
    ///        layout-engine-specific endpoint adjustments (for example system-boundary behavior or custom spacing policy)
    ///        while this function still applies tie placement rules around noteheads.
    /// @return `std::nullopt` if no applicable tie can be resolved; otherwise the selected span style and computed EVPU length.
    [[nodiscard]]
    static std::optional<ContourResult> calcContourStyleType(
        const dom::NoteInfoPtr& noteInfo, const ContourGeometry& geometry, bool forTieEnd = false);

    /// @brief Calculates default contour control points (inset/height) for a given control style.
    /// @param noteInfo The note whose tie options are consulted.
    /// @param styleType The control style type to evaluate (Short/Medium/Long/TieEnds).
    /// @param length The tie length in EVPU used for interpolation when enabled.
    /// @return Calculated control points, or std::nullopt if inputs/styles are unavailable.
    ///
    /// When tie options enable interpolation and @p styleType is MediumSpan, heights are interpolated between
    /// Short/Medium or Medium/Long depending on @p length. Relative inset ratios are interpolated only when the
    /// inset style is Percent; fixed insets remain the MediumSpan fixed values.
    [[nodiscard]]
    static std::optional<ContourControlPoints> calcDefaultContourControlPoints(
        const dom::NoteInfoPtr& noteInfo, dom::options::TieOptions::ControlStyleType styleType, dom::Evpu length);

    /// @brief Calculates effective contour control points (inset/height) for a tie.
    /// @param noteInfo The note whose tie options and tie alterations are consulted.
    /// @param forTieEnd True for tie-end stub ties, false for regular tie-forward ties.
    /// @param styleType The control style type to evaluate (Short/Medium/Long/TieEnds).
    /// @param length The tie length in EVPU used for interpolation when enabled.
    /// @return Calculated control points, or std::nullopt if inputs/styles are unavailable.
    ///
    /// If a tie alteration record exists and control point adjustments are enabled, this returns the alteration
    /// control points (and inset style from the alteration). Otherwise it falls back to
    /// #calcDefaultContourControlPoints.
    [[nodiscard]]
    static std::optional<ContourControlPoints> calcEffectiveContourControlPoints(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd, dom::options::TieOptions::ControlStyleType styleType, dom::Evpu length);

private:
    /// @brief Calculates the connect style type for a single endpoint (placement_for_endpoint).
    /// @param noteInfo The note whose tie endpoint is being analyzed.
    /// @param forTieEnd True if the note represents a tie end; false for tie starts.
    /// @param direction The effective tie direction.
    /// @param stemUp The stem direction for the endpoint's entry.
    /// @param tieAlterContext The note whose tie alterations should be consulted for overrides.
    /// @param forEndPoint True to calculate the end point; false for the start point.
    /// @param noteIndexOverride Optional override for the note index used to evaluate outer placement.
    /// @param noteCountOverride Optional override for the note count used to evaluate outer placement.
    /// @param upStemSecondOverride Optional override for the up-stem second flag.
    /// @param downStemSecondOverride Optional override for the down-stem second flag.
    /// @returns The calculated connection style type, or std::nullopt when the input is invalid.
    [[nodiscard]]
    static std::optional<dom::TieConnectStyleType> calcConnectStyleTypeAtEndPoint(
        const dom::NoteInfoPtr& noteInfo, bool forTieEnd, dom::CurveContourDirection direction, bool stemUp,
        const dom::NoteInfoPtr& tieAlterContext, bool forEndPoint,
        std::optional<size_t> noteIndexOverride = std::nullopt, std::optional<size_t> noteCountOverride = std::nullopt,
        std::optional<bool> upStemSecondOverride = std::nullopt, std::optional<bool> downStemSecondOverride = std::nullopt);
};

} // namespace util
} // namespace musx
