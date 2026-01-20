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
#include <algorithm>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <limits>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

#include "musx/musx.h"

namespace musx::util {
namespace {

using dom::KnownShapeDefType;
using dom::ShapeDefInstruction;
using dom::ShapeDefInstructionType;
using dom::others::ShapeDef;

enum class ShapeRecognitionStepResult
{
    Continue,
    Reject,
    Accept
};

struct ShapeRecognitionCandidate
{
    KnownShapeDefType type;
    std::function<ShapeRecognitionStepResult(const ShapeDefInstruction::Decoded&)> consume;
    std::function<bool()> finalize = [] { return false; };
    bool rejected = false;
};

using ShapeRecognitionCandidates = std::vector<ShapeRecognitionCandidate>;

enum class SlurTieDirection
{
    CurveRight,
    CurveLeft,
};

constexpr dom::Evpu SLUR_TIE_HORIZONTAL_TOLERANCE_EVPU = 6; // 1/4 space expressed in Evpu
constexpr dom::Evpu16ths SLUR_TIE_HORIZONTAL_TOLERANCE_16THS = SLUR_TIE_HORIZONTAL_TOLERANCE_EVPU * 16;
constexpr int SLUR_TIE_BOUND_SAMPLE_COUNT = 32;
constexpr double SLUR_TIE_VERTICAL_SCALE_THRESHOLD = 4.0;

static dom::Evpu16ths evpuTo16ths(dom::Evpu value)
{
    return static_cast<dom::Evpu16ths>(value * 16);
}

static dom::Evpu16ths evpuDoubleTo16ths(double value)
{
    return static_cast<dom::Evpu16ths>(std::llround(value * 16.0));
}

static std::pair<double, double> computeSlurYBounds(const ShapeDefInstruction::Slur& slur)
{
    const double startY = 0.0;
    const double c1y = static_cast<double>(slur.c1dy) / 16.0;
    const double c2y = static_cast<double>(slur.c2dy) / 16.0;
    const double endY = static_cast<double>(slur.edy) / 16.0;

    double minY = std::numeric_limits<double>::infinity();
    double maxY = -std::numeric_limits<double>::infinity();

    for (int i = 0; i <= SLUR_TIE_BOUND_SAMPLE_COUNT; ++i) {
        const double t = static_cast<double>(i) / SLUR_TIE_BOUND_SAMPLE_COUNT;
        const double nt = 1.0 - t;
        const double sampleY =
            (nt * nt * nt * startY) +
            (3.0 * nt * nt * t * (startY + c1y)) +
            (3.0 * nt * t * t * (startY + c2y)) +
            (t * t * t * (startY + endY));

        minY = (std::min)(minY, sampleY);
        maxY = (std::max)(maxY, sampleY);
    }

    if (!std::isfinite(minY) || !std::isfinite(maxY)) {
        return {0.0, 0.0};
    }

    return {minY, maxY};
}

static std::optional<std::pair<dom::Evpu16ths, dom::Evpu16ths>> computeScaledSlurYPositions(
    const ShapeDefInstruction::Slur& slur,
    const ShapeDefInstruction::StartObject& startObject,
    const ShapeDefInstruction::RMoveTo& move)
{
    const auto [rawMin, rawMax] = computeSlurYBounds(slur);
    const double rawHeight = rawMax - rawMin;
    const double targetHeight = static_cast<double>(startObject.top - startObject.bottom);

    if (rawHeight <= std::numeric_limits<double>::epsilon() || std::abs(targetHeight) <= std::numeric_limits<double>::epsilon()) {
        return std::nullopt;
    }

    const double ratio = std::abs(targetHeight) / rawHeight;
    if (ratio <= SLUR_TIE_VERTICAL_SCALE_THRESHOLD) {
        return std::nullopt;
    }

    const double scale = ratio;
    const double offset = static_cast<double>(startObject.bottom) - rawMin * scale;
    const double base = static_cast<double>(startObject.originY + move.dy);
    const double rawEnd = static_cast<double>(slur.edy) / 16.0;

    std::pair<dom::Evpu16ths, dom::Evpu16ths> result;
    result.first = evpuDoubleTo16ths(base + offset);
    result.second = evpuDoubleTo16ths(base + offset + rawEnd * scale);
    return result;
}

struct InstructionExpectation
{
    ShapeDefInstructionType type;
    std::function<bool(const ShapeDefInstruction::Decoded&)> validate;
};

using InstructionExpectations = std::vector<InstructionExpectation>;

static bool isZeroSpaceDash(const ShapeDefInstruction::Decoded& inst)
{
    if (inst.type != ShapeDefInstructionType::SetDash) {
        return false;
    }
    bool valid = false;
    std::visit([&](auto&& instData) {
        using T = std::decay_t<decltype(instData)>;
        if constexpr (std::is_same_v<T, ShapeDefInstruction::SetDash>) {
            valid = instData.spaceLength == 0;
        }
    }, inst.data);
    return valid;
}

static ShapeRecognitionCandidate makeSequenceRecognizer(
    KnownShapeDefType type,
    InstructionExpectations expectations,
    std::function<bool(const ShapeDefInstruction::Decoded&)> skipPredicate = {})
{
    struct SequenceRecognizerState {
        InstructionExpectations expectations;
        std::function<bool(const ShapeDefInstruction::Decoded&)> skipPredicate;
        size_t nextIndex = 0;
    };

    auto state = std::make_shared<SequenceRecognizerState>();
    state->expectations = std::move(expectations);
    state->skipPredicate = std::move(skipPredicate);

    ShapeRecognitionCandidate candidate;
    candidate.type = type;
    candidate.consume = [state](const ShapeDefInstruction::Decoded& inst) -> ShapeRecognitionStepResult {
        if (!inst.valid()) {
            return ShapeRecognitionStepResult::Reject;
        }

        if (state->skipPredicate && state->skipPredicate(inst)) {
            return ShapeRecognitionStepResult::Continue;
        }

        if (state->nextIndex >= state->expectations.size()) {
            return ShapeRecognitionStepResult::Reject;
        }

        const auto& expected = state->expectations[state->nextIndex];
        if (inst.type != expected.type) {
            return ShapeRecognitionStepResult::Reject;
        }

        if (expected.validate && !expected.validate(inst)) {
            return ShapeRecognitionStepResult::Reject;
        }

        state->nextIndex++;
        return ShapeRecognitionStepResult::Continue;
    };

    candidate.finalize = [state]() -> bool {
        return state->nextIndex == state->expectations.size();
    };

    return candidate;
}

static ShapeRecognitionCandidate makeTenutoRecognizer()
{
    auto skipPredicate = [](const ShapeDefInstruction::Decoded& inst) {
        return isZeroSpaceDash(inst);
    };

    auto lineWidthValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::LineWidth>) {
                valid = instData.efix >= 4 * dom::EFIX_PER_EVPU && instData.efix <= 6 * dom::EFIX_PER_EVPU;
            }
        }, inst.data);
        return valid;
    };

    auto horizontalLineValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::RLineTo>) {
                valid = instData.dx >= dom::EVPU_PER_SPACE &&
                    instData.dx <= 1.5 * dom::EVPU_PER_SPACE &&
                    instData.dy == 0;
            }
        }, inst.data);
        return valid;
    };

    return makeSequenceRecognizer(
        KnownShapeDefType::TenutoMark,
        InstructionExpectations{
            {ShapeDefInstructionType::StartObject, nullptr},
            {ShapeDefInstructionType::RMoveTo, nullptr},
            {ShapeDefInstructionType::LineWidth, lineWidthValidator},
            {ShapeDefInstructionType::RLineTo, horizontalLineValidator},
            {ShapeDefInstructionType::Stroke, nullptr}
        },
        skipPredicate);
}

static ShapeRecognitionCandidate makePedalArrowheadRecognizer(
    KnownShapeDefType type,
    std::vector<std::pair<dom::Evpu, dom::Evpu>> segments)
{
    auto skipPredicate = [](const ShapeDefInstruction::Decoded& inst) {
        return isZeroSpaceDash(inst);
    };

    auto lineWidthValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::LineWidth>) {
                valid = instData.efix >= dom::EFIX_PER_EVPU && instData.efix <= 4 * dom::EFIX_PER_EVPU;
            }
        }, inst.data);
        return valid;
    };

    auto moveValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::RMoveTo>) {
                valid = instData.dx == 0 && instData.dy == 0;
            }
        }, inst.data);
        return valid;
    };

    auto rLineToValidator = [](dom::Evpu dx, dom::Evpu dy) {
        return [dx, dy](const ShapeDefInstruction::Decoded& inst) {
            bool valid = false;
            std::visit([&](auto&& instData) {
                using T = std::decay_t<decltype(instData)>;
                if constexpr (std::is_same_v<T, ShapeDefInstruction::RLineTo>) {
                    valid = instData.dx == dx && instData.dy == dy;
                }
            }, inst.data);
            return valid;
        };
    };

    InstructionExpectations expectations;
    expectations.reserve(5 + segments.size());
    expectations.push_back({ShapeDefInstructionType::StartObject, nullptr});
    expectations.push_back({ShapeDefInstructionType::LineWidth, lineWidthValidator});
    expectations.push_back({ShapeDefInstructionType::RMoveTo, moveValidator});
    for (const auto& segment : segments) {
        expectations.push_back({ShapeDefInstructionType::RLineTo, rLineToValidator(segment.first, segment.second)});
    }
    expectations.push_back({ShapeDefInstructionType::Stroke, nullptr});

    return makeSequenceRecognizer(type, std::move(expectations), skipPredicate);
}

struct SlurTieContour
{
    dom::Evpu16ths startX = 0;
    dom::Evpu16ths startY = 0;
    dom::Evpu16ths endX = 0;
    dom::Evpu16ths endY = 0;
    dom::Evpu16ths deltaX = 0;
    dom::Evpu16ths deltaY = 0;
};

struct SlurTieState
{
    std::optional<ShapeDefInstruction::StartObject> currentStart;
    std::optional<ShapeDefInstruction::RMoveTo> currentMove;
    std::optional<bool> currentRightHint;
    std::optional<bool> expectedRight;
    std::vector<SlurTieContour> contours;
};

static ShapeRecognitionCandidate makeSlurTieRecognizer(SlurTieDirection direction)
{
    auto state = std::make_shared<SlurTieState>();

    ShapeRecognitionCandidate candidate;
    candidate.type = (direction == SlurTieDirection::CurveRight) ? KnownShapeDefType::SlurTieCurveRight : KnownShapeDefType::SlurTieCurveLeft;

    candidate.consume = [state, direction](const ShapeDefInstruction::Decoded& inst) -> ShapeRecognitionStepResult {
        switch (inst.type) {
        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            if (!data) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentStart = *data;
            state->currentMove.reset();
            state->currentRightHint.reset();
            const bool extendsLeft = data->right <= 0 && data->left <= data->right;
            const bool extendsRight = data->left >= 0 && data->left <= data->right;
            if (extendsLeft != extendsRight) {
                state->currentRightHint = extendsRight;
            }
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::RMoveTo: {
            if (!state->currentStart) {
                return ShapeRecognitionStepResult::Reject;
            }
            const auto* data = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data);
            if (!data) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentMove = *data;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::Slur: {
            if (!state->currentStart || !state->currentMove) {
                return ShapeRecognitionStepResult::Reject;
            }
            const auto* slur = std::get_if<ShapeDefInstruction::Slur>(&inst.data);
            if (!slur) {
                return ShapeRecognitionStepResult::Reject;
            }

            const bool contourRight = state->currentRightHint.value_or(slur->edx > 0);
            if (!state->expectedRight) {
                state->expectedRight = contourRight;
            } else if (contourRight != *state->expectedRight) {
                return ShapeRecognitionStepResult::Reject;
            }

            if ((direction == SlurTieDirection::CurveRight && !contourRight) ||
                (direction == SlurTieDirection::CurveLeft && contourRight)) {
                return ShapeRecognitionStepResult::Reject;
            }

            SlurTieContour contour;
            const dom::Evpu startX = state->currentStart->originX + state->currentMove->dx;
            const dom::Evpu startYEvpu = state->currentStart->originY + state->currentMove->dy;
            contour.startX = evpuTo16ths(startX);
            contour.deltaX = slur->edx;
            contour.endX = contour.startX + contour.deltaX;

            auto scaledY = computeScaledSlurYPositions(*slur, *state->currentStart, *state->currentMove);
            if (scaledY) {
                contour.startY = scaledY->first;
                contour.endY = scaledY->second;
                contour.deltaY = contour.endY - contour.startY;
            } else {
                contour.startY = evpuTo16ths(startYEvpu);
                contour.deltaY = slur->edy;
                contour.endY = contour.startY + contour.deltaY;
            }
            state->contours.push_back(contour);

            state->currentStart.reset();
            state->currentMove.reset();
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::FillSolid:
            return ShapeRecognitionStepResult::Continue;
        case ShapeDefInstructionType::SetDash:
            return isZeroSpaceDash(inst) ? ShapeRecognitionStepResult::Continue : ShapeRecognitionStepResult::Reject;

        default:
            return ShapeRecognitionStepResult::Reject;
        }
    };

    candidate.finalize = [state]() -> bool {
        if (!state->contours.empty() && (state->currentStart || state->currentMove)) {
            return false;
        }
        if (state->contours.empty()) {
            return false;
        }
        if (!state->expectedRight) {
            return false;
        }
        for (const auto& contour : state->contours) {
            if (contour.deltaX == 0) {
                return false;
            }
        }

        const bool expectingRight = *state->expectedRight;
        dom::Evpu16ths startSeparation = 0;
        dom::Evpu16ths endSeparation = 0;

        if (state->contours.size() == 1) {
            startSeparation = 0;
            endSeparation = state->contours.front().deltaY;
        } else {
            const auto compareStartY = [](const SlurTieContour& a, const SlurTieContour& b) {
                return a.startY < b.startY;
            };
            const auto compareEndY = [](const SlurTieContour& a, const SlurTieContour& b) {
                return a.endY < b.endY;
            };
            const auto startMinMax = std::minmax_element(state->contours.begin(), state->contours.end(), compareStartY);
            const auto endMinMax = std::minmax_element(state->contours.begin(), state->contours.end(), compareEndY);
            if (startMinMax.first == state->contours.end() || startMinMax.second == state->contours.end() ||
                endMinMax.first == state->contours.end() || endMinMax.second == state->contours.end()) {
                return false;
            }
            startSeparation = startMinMax.second->startY - startMinMax.first->startY;
            endSeparation = endMinMax.second->endY - endMinMax.first->endY;
        }

        const dom::Evpu16ths diff = std::abs(startSeparation - endSeparation);
        if (diff > SLUR_TIE_HORIZONTAL_TOLERANCE_16THS) {
            return false;
        }

        if (expectingRight) {
            const auto leftmost = std::min_element(state->contours.begin(), state->contours.end(),
                [](const SlurTieContour& a, const SlurTieContour& b) { return a.startX < b.startX; });
            const auto rightmost = std::max_element(state->contours.begin(), state->contours.end(),
                [](const SlurTieContour& a, const SlurTieContour& b) { return a.endX < b.endX; });
            return leftmost != state->contours.end() && rightmost != state->contours.end();
        } else {
            const auto rightmostStart = std::max_element(state->contours.begin(), state->contours.end(),
                [](const SlurTieContour& a, const SlurTieContour& b) { return a.startX < b.startX; });
            const auto leftmostEnd = std::min_element(state->contours.begin(), state->contours.end(),
                [](const SlurTieContour& a, const SlurTieContour& b) { return a.endX < b.endX; });
            return rightmostStart != state->contours.end() && leftmostEnd != state->contours.end();
        }
    };

    return candidate;
}

static ShapeRecognitionCandidates createShapeRecognizers(const ShapeDef& shape)
{
    const auto allowsShapeType = [&](std::initializer_list<ShapeDef::ShapeType> allowedTypes) {
        if (shape.shapeType == ShapeDef::ShapeType::Other) {
            // legacy files do not use shape types, so we always need to check Other.
            return true;
        }
        return std::find(allowedTypes.begin(), allowedTypes.end(), shape.shapeType) != allowedTypes.end();
    };

    ShapeRecognitionCandidates candidates;
    if (allowsShapeType({ShapeDef::ShapeType::Articulation})) {
        candidates.push_back(makeTenutoRecognizer());
    }
    if (allowsShapeType({ShapeDef::ShapeType::Articulation, ShapeDef::ShapeType::Expression})) {
        candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveRight));
        candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveLeft));
    }
    if (allowsShapeType({ShapeDef::ShapeType::Arrowhead})) {
        candidates.push_back(makePedalArrowheadRecognizer(
            KnownShapeDefType::PedalArrowheadDown,
            {{0, 0}, {12, -24}, {12, 24}}));
        candidates.push_back(makePedalArrowheadRecognizer(
            KnownShapeDefType::PedalArrowheadUp,
            {{0, 0}, {12, 24}, {12, -24}}));
        candidates.push_back(makePedalArrowheadRecognizer(
            KnownShapeDefType::PedalArrowheadShortUpDownLongUp,
            {{0, 0}, {9, 24}, {14, -36}, {13, 12}}));
        candidates.push_back(makePedalArrowheadRecognizer(
            KnownShapeDefType::PedalArrowheadLongUpDownShortUp,
            {{0, 0}, {13, 12}, {14, -36}, {9, 24}, {0, 0}}));
    }
    return candidates;
}

} // namespace

KnownShapeDefType recognizeShape(const ShapeDef& shape)
{
    if (shape.isBlank()) {
        return KnownShapeDefType::Blank;
    }

    auto recognizers = createShapeRecognizers(shape);
    if (recognizers.empty()) {
        return KnownShapeDefType::Unrecognized;
    }

    auto recognized = KnownShapeDefType::Unrecognized;
    shape.iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
        bool anyActive = false;

        for (auto& recognizer : recognizers) {
            if (recognizer.rejected) {
                continue;
            }

            switch (recognizer.consume(inst)) {
            case ShapeRecognitionStepResult::Continue:
                anyActive = true;
                break;

            case ShapeRecognitionStepResult::Reject:
                recognizer.rejected = true;
                break;

            case ShapeRecognitionStepResult::Accept:
                recognized = recognizer.type;
                return false;
            }
        }

        if (!anyActive) {
            return false;
        }

        return true;
    });

    if (recognized != KnownShapeDefType::Unrecognized) {
        return recognized;
    }

    for (auto& recognizer : recognizers) {
        if (!recognizer.rejected && recognizer.finalize()) {
            return recognizer.type;
        }
    }

    return KnownShapeDefType::Unrecognized;
}

} // namespace musx::util
