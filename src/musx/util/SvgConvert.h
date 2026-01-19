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
    /// @struct ExternalGraphicPayload
    /// @brief Payload data for external graphics.
    struct ExternalGraphicPayload
    {
        std::string mimeType; ///< MIME type for the graphic data.
        std::vector<std::uint8_t> bytes; ///< Raw graphic bytes (caller-provided).
    };

    /// @struct ExternalGraphicInfo
    /// @brief Metadata describing an external graphic instruction.
    struct ExternalGraphicInfo
    {
        dom::Evpu width{}; ///< Width of the external graphic in EVPU units.
        dom::Evpu height{}; ///< Height of the external graphic in EVPU units.
        dom::Cmper cmper{}; ///< Cmper matching #dom::others::PageGraphicAssign::graphicCmper.
    };

    /// @brief Metrics for sizing SVG text bounds.
    struct GlyphMetrics
    {
        /// @brief The advance width of the text in EVPU units.
        double advance{};
        /// @brief The ascent above the baseline in EVPU units.
        double ascent{};
        /// @brief The descent below the baseline in EVPU units (positive value).
        double descent{};
    };

    /// @brief Optional callback that returns glyph metrics in EVPU units.
    /// @note The callback receives the resolved font and the glyph(s) to measure.
    /// @note Return std::nullopt to fall back to heuristic metrics.
    using GlyphMetricsFn = std::function<std::optional<GlyphMetrics>(const dom::FontInfo&, std::u32string_view)>;

    /// @brief Optional callback that resolves external graphics to a MIME type and byte buffer.
    using ExternalGraphicFn = std::function<std::optional<ExternalGraphicPayload>(const ExternalGraphicInfo&)>;

    /// @brief Convert a ShapeDef into an SVG string buffer.
    /// @param shape The shape definition to convert.
    /// @param glyphMetrics Optional callback that returns glyph metrics in EVPU units.
    /// @param externalGraphicResolver Optional callback that resolves external graphics to bytes and MIME types.
    /// @return An SVG buffer encoded as a string.
    /// @note The glyph callback receives the resolved font and the glyph(s) to measure.
    /// @note If an external graphic is encountered and cannot be resolved, this returns an empty string.
    static std::string toSvg(const dom::MusxInstance<dom::others::ShapeDef>& shape,
                             GlyphMetricsFn glyphMetrics = nullptr,
                             ExternalGraphicFn externalGraphicResolver = nullptr);
};

} // namespace util
} // namespace musx
