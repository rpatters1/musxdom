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

#include "musx/dom/Fundamentals.h"
#include "musx/dom/MusxInstance.h"

namespace musx {
namespace dom {
enum class KnownShapeDefType;

namespace others {
class ShapeDef;
} // namespace others
} // namespace dom

namespace utils {

/// @enum PseudoTieMode
/// @brief Specifies which pseudo tie behavior is being evaluated.
/// @details A pseudo tie may be represented by either a shape definition (e.g., @ref musx::dom::others::ShapeDef) or a
/// smart shape (@ref musx::dom::others::SmartShape). These helpers assume you have already determined that the
/// candidate object is acting as a surrogate tie and are only responsible for evaluating its placement behavior.
enum class PseudoTieMode
{
    LaissezVibrer,  ///< Pseudo laissez vibrer tie (forward surrogate).
    TieEnd          ///< Pseudo tie-end (backward or left-shifted surrogate).
};

/// @struct PseudoTieShapeInfo
/// @brief Information about a shape that could be used as a surrogate for a tie.
struct PseudoTieShapeInfo
{
    dom::MusxInstance<dom::others::ShapeDef> shape;             ///< The resolved shape definition.
    dom::KnownShapeDefType shapeType{};                         ///< The recognized shape category (e.g., SlurTieCurveRight/Left).
};

/// @brief Returns true if a forward tie surrogate is shifted backward to act as a tie end.
/// @details A pseudo tie may be represented by either a shape definition (e.g., @ref musx::dom::others::ShapeDef) or a
/// smart shape (@ref musx::dom::others::SmartShape). These helpers assume you have already determined that the
/// candidate object is acting as a surrogate tie and are only responsible for evaluating the horizontal offsets that
/// distinguish forward vs. backward usage.
[[nodiscard]] bool calcIsPseudoBackwardTie(dom::Evpu startOffset, dom::Evpu endOffset);

/// @brief Returns true if a forward tie surrogate is positioned as a valid pseudo forward tie.
/// @details A pseudo tie may be represented by either a shape definition (e.g., @ref musx::dom::others::ShapeDef) or a
/// smart shape (@ref musx::dom::others::SmartShape). These helpers assume you have already determined that the
/// candidate object is acting as a surrogate tie and are only responsible for evaluating the horizontal offsets that
/// distinguish forward vs. backward usage.
[[nodiscard]] bool calcIsPseudoForwardTie(dom::Evpu startOffset, dom::Evpu endOffset);

} // namespace utils
} // namespace musx
