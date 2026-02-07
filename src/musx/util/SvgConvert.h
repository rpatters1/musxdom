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

#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "musx/dom/Fundamentals.h"
#include "musx/dom/MusxInstance.h"

namespace musx {
namespace dom {
class FontInfo;
namespace others {
class ShapeDef;
} // namespace others
} // namespace dom

namespace util {

/// @class SvgConvert
/// @brief Converts a Shape Designer definition into an SVG buffer.
class SvgConvert
{
public:
    /// @brief SVG unit suffixes supported by the converter.
    /// @note The absolute units (px/pt/pc/cm/mm/in) map to EVPU conversions in this converter.
    enum class SvgUnit
    {
        None, ///< Unitless (no suffix).
        Pixels,        ///< "px"
        Points,        ///< "pt"
        Picas,         ///< "pc"
        Centimeters,   ///< "cm"
        Millimeters,   ///< "mm"
        Inches         ///< "in"
    };

    /// @brief Metrics for sizing SVG text bounds.
    struct GlyphMetrics
    {
        /// @brief The advance width of the text in EVPU units.
        dom::EvpuFloat advance{};
        /// @brief The ascent above the baseline in EVPU units.
        dom::EvpuFloat ascent{};
        /// @brief The descent below the baseline in EVPU units (positive value).
        dom::EvpuFloat descent{};
    };

    /// @brief Optional callback that returns glyph metrics in EVPU units.
    /// @note The callback receives the resolved font and the glyph(s) to measure.
    /// @note Return std::nullopt to fall back to heuristic metrics.
    using GlyphMetricsFn = std::function<std::optional<GlyphMetrics>(const dom::FontInfo&, std::u32string_view)>;

    /// @brief Convert a ShapeDef into an SVG string buffer.
    /// @param shape The shape definition to convert.
    /// @return An SVG buffer encoded as a string.
    /// @note External graphics are resolved from embedded graphics in the @ref musx::dom::Document when
    ///       available. Otherwise the converter tries a file with the same filename in the directory
    ///       containing the source musx/EnigmaXML (provided via
    ///       @ref musx::factory::DocumentFactory::CreateOptions), then the original file location. If an
    ///       external graphic still cannot be resolved, #toSvg returns an empty string.
    /// @todo ShapeDef SetArrowhead instructions are decoded but not yet rendered in SVG output.
    static std::string toSvg(const dom::others::ShapeDef& shape);

    /// @brief Convert a ShapeDef into an SVG string buffer.
    /// @param shape The shape definition to convert.
    /// @param glyphMetrics Callback that receives the resolved font and the glyph(s) to measure and returns
    ///        glyph metrics in EVPU units.
    /// @return An SVG buffer encoded as a string.
    static std::string toSvg(const dom::others::ShapeDef& shape,
                             GlyphMetricsFn glyphMetrics);

    /// @brief Convert a ShapeDef into an SVG string buffer with explicit scaling and units.
    /// @param shape The shape definition to convert.
    /// @param scaling Scale factor applied to EVPU-based coordinates before unit conversion.
    /// @param unit Unit suffix for width/height (e.g., @ref SvgUnit::Millimeters).
    /// @return An SVG buffer encoded as a string.
    static std::string toSvg(const dom::others::ShapeDef& shape,
                             double scaling,
                             SvgUnit unit);

    /// @brief Convert a ShapeDef into an SVG string buffer with explicit scaling and units.
    /// @param shape The shape definition to convert.
    /// @param scaling Scale factor applied to EVPU-based coordinates before unit conversion.
    /// @param unit Unit suffix for width/height (e.g., @ref SvgUnit::Millimeters).
    /// @param glyphMetrics Callback that receives the resolved font and the glyph(s) to measure and returns
    ///        glyph metrics in EVPU units.
    /// @return An SVG buffer encoded as a string.
    static std::string toSvg(const dom::others::ShapeDef& shape,
                             double scaling,
                             SvgUnit unit,
                             GlyphMetricsFn glyphMetrics);

    /// @brief Convert a ShapeDef into an SVG string buffer using the document's page format scaling.
    /// @param shape The shape definition to convert.
    /// @param unit Unit suffix for width/height (e.g., @ref SvgUnit::Millimeters).
    /// @return An SVG buffer encoded as a string.
    static std::string toSvgWithPageFormatScaling(const dom::others::ShapeDef& shape,
                                                  SvgUnit unit = SvgUnit::Millimeters);

    /// @brief Convert a ShapeDef into an SVG string buffer using the document's page format scaling.
    /// @param shape The shape definition to convert.
    /// @param unit Unit suffix for width/height (e.g., @ref SvgUnit::Millimeters).
    /// @param glyphMetrics Callback that receives the resolved font and the glyph(s) to measure and returns
    ///        glyph metrics in EVPU units.
    /// @return An SVG buffer encoded as a string.
    static std::string toSvgWithPageFormatScaling(const dom::others::ShapeDef& shape,
                                                  SvgUnit unit,
                                                  GlyphMetricsFn glyphMetrics);
};

} // namespace util
} // namespace musx
