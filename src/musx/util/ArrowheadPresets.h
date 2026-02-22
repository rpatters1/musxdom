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

#include "musx/dom/Fundamentals.h"
#include "musx/dom/EnumClasses.h"

namespace musx {
namespace util {

/// @enum ArrowheadPresetFillStyle
/// @brief Visual fill style used by Finale's built-in preset arrowheads.
enum class ArrowheadPresetFillStyle
{
    SolidFill,              ///< Solid-filled arrowhead using the caller's primary color.
    WhiteFillWithOutline    ///< White fill plus primary-color outline (Finale's "unfilled" look).
};

/// @brief Geometry and appearance for a Finale preset arrowhead.
///
/// Values are expressed in EVPU and are independent of line thickness.
struct ArrowheadPresetSpec
{
    dom::EvpuFloat length{}; ///< Distance from the base midpoint to the tip.
    dom::EvpuFloat baseWidth{}; ///< Full width of the base.
    bool curvedBack{}; ///< True for curved-back presets, false for straight-sided presets.
    ArrowheadPresetFillStyle fillStyle{ArrowheadPresetFillStyle::SolidFill}; ///< Visual fill style.
};

/// @brief Returns the geometry and fill style for a Finale preset arrowhead.
/// @param preset The built-in Finale preset arrowhead type.
/// @return Preset geometry and fill style in EVPU units.
[[nodiscard]]
constexpr ArrowheadPresetSpec getArrowheadPresetSpec(dom::ArrowheadPreset preset)
{
    switch (preset) {
    case dom::ArrowheadPreset::SmallFilled:
        return {20.0, 20.0, false, ArrowheadPresetFillStyle::SolidFill};
    case dom::ArrowheadPreset::SmallOutline:
        return {20.0, 20.0, false, ArrowheadPresetFillStyle::WhiteFillWithOutline};
    case dom::ArrowheadPreset::SmallCurved:
        return {20.0, 20.0, true, ArrowheadPresetFillStyle::SolidFill};
    case dom::ArrowheadPreset::LargeCurved:
        return {100.0, 100.0, true, ArrowheadPresetFillStyle::SolidFill};
    case dom::ArrowheadPreset::MediumCurved:
        return {60.0, 60.0, true, ArrowheadPresetFillStyle::SolidFill};
    }
    return {};
}

} // namespace util
} // namespace musx
