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

#include "musx/dom/MusxInstance.h"

namespace musx {
namespace dom {
class EntryInfoPtr;
class NoteInfoPtr;
enum class KnownSmartShapeType;
namespace others {
class SmartShape;
} // namespace others
} // namespace dom
namespace utils {
enum class PseudoTieMode;
} // namespace utils
} // namespace musx

namespace musx::util {

/// @brief Recognize well-known Smart Shape patterns.
/// @param smartShape The smart shape to evaluate.
/// @return The recognized type, or dom::KnownSmartShapeType::Unrecognized when no match is found.
[[nodiscard]]
dom::KnownSmartShapeType recognizeSmartShape(const dom::others::SmartShape& smartShape);

/// @brief Recognize well-known Smart Shape patterns.
/// @param smartShape The smart shape instance to evaluate.
/// @return The recognized type, or dom::KnownSmartShapeType::Unrecognized when no match is found.
[[nodiscard]]
dom::KnownSmartShapeType recognizeSmartShape(const dom::MusxInstance<dom::others::SmartShape>& smartShape);

/// @brief Returns true when a smart shape is acting as a pseudo tie for the specified entry context.
/// @param smartShape The smart shape to evaluate.
/// @param mode The pseudo tie mode to evaluate.
/// @param forStartEntry The entry context to check.
[[nodiscard]]
bool calcIsSmartShapePseudoTie(
    const dom::others::SmartShape& smartShape,
    musx::utils::PseudoTieMode mode,
    const dom::EntryInfoPtr& forStartEntry);

/// @brief Returns the tied-to note when a smart shape is acting as an arpeggiated tie.
/// @param smartShape The smart shape to evaluate.
/// @param forStartEntry The entry context to check.
[[nodiscard]]
dom::NoteInfoPtr calcSmartShapeArpeggiatedTieToNote(
    const dom::others::SmartShape& smartShape,
    const dom::EntryInfoPtr& forStartEntry);

} // namespace musx::util
