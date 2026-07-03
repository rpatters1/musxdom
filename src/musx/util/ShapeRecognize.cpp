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

using dom::MusxInstance;

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
constexpr dom::Evpu RIGHT_HOOK_MAX_LENGTH_EVPU = 48;
constexpr dom::Evpu RIGHT_HOOK_VERTICAL_TOLERANCE_EVPU = 1;
constexpr dom::Evpu PIZZICATO_STEM_MIN_LENGTH_EVPU = 15;
constexpr dom::Evpu PIZZICATO_STEM_MAX_LENGTH_EVPU = 48;
constexpr dom::Evpu PIZZICATO_STEM_AXIS_TOLERANCE_EVPU = 1;
constexpr dom::Evpu PIZZICATO_STEM_CENTER_TOLERANCE_EVPU = 2;
constexpr dom::Evpu PIZZICATO_CIRCLE_SIZE_EVPU = 18;
constexpr dom::Evpu PIZZICATO_CIRCLE_SIZE_TOLERANCE_EVPU = 3;
constexpr dom::Evpu PIZZICATO_LINE_WIDTH_MIN_EFIX = dom::EFIX_PER_EVPU;
constexpr dom::Evpu PIZZICATO_LINE_WIDTH_MAX_EFIX = 3 * dom::EFIX_PER_EVPU;
constexpr dom::Evpu FINGERNAIL_PIZZ_MIN_WIDTH_EVPU = 240;
constexpr dom::Evpu FINGERNAIL_PIZZ_MAX_WIDTH_EVPU = 330;
constexpr dom::Evpu FINGERNAIL_PIZZ_MIN_BULGE_EVPU = 55;
constexpr dom::Evpu FINGERNAIL_PIZZ_MAX_BULGE_EVPU = 150;
constexpr dom::Evpu FINGERNAIL_PIZZ_ENDPOINT_TOLERANCE_EVPU = 2;

enum class PizzicatoStemOrientation
{
    Above,
    Below,
    Horizontal
};

enum class FingernailPizzDirection
{
    CurveUp,
    CurveDown
};

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

struct RightHookLine
{
    dom::Evpu dx{};
    dom::Evpu dy{};
};

struct RightHookState
{
    std::vector<RightHookLine> currentPath;
    bool recognizedPath = false;
};

static bool isRightHookLine(const RightHookLine& line)
{
    return std::abs(line.dy) <= RIGHT_HOOK_VERTICAL_TOLERANCE_EVPU
        && std::abs(line.dx) > 0
        && std::abs(line.dx) <= RIGHT_HOOK_MAX_LENGTH_EVPU;
}

static bool isVerticalLine(const RightHookLine& line)
{
    return line.dx == 0 && line.dy != 0;
}

static bool isRightHookedVerticalPath(const std::vector<RightHookLine>& path)
{
    if (path.size() < 3) {
        return false;
    }

    const auto& first = path.front();
    const auto& last = path.back();
    if (!isRightHookLine(first) || !isRightHookLine(last)) {
        return false;
    }

    // Finale draws this shape from the right end of one hook to the vertical
    // stroke, then from the vertical stroke to the right end of the other hook.
    if (first.dx >= 0 || last.dx <= 0) {
        return false;
    }

    dom::Evpu verticalLength = 0;
    for (size_t i = 1; i + 1 < path.size(); ++i) {
        if (!isVerticalLine(path[i])) {
            return false;
        }
        verticalLength += std::abs(path[i].dy);
    }

    return verticalLength > 0;
}

static ShapeRecognitionCandidate makeVerticalLineRightHooksRecognizer()
{
    auto state = std::make_shared<RightHookState>();

    ShapeRecognitionCandidate candidate;
    candidate.type = KnownShapeDefType::VerticalLineRightHooks;

    candidate.consume = [state](const ShapeDefInstruction::Decoded& inst) -> ShapeRecognitionStepResult {
        if (!inst.valid()) {
            return ShapeRecognitionStepResult::Reject;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartObject:
        case ShapeDefInstructionType::StartGroup:
        case ShapeDefInstructionType::EndGroup:
        case ShapeDefInstructionType::Bracket:
        case ShapeDefInstructionType::VerticalMode:
        case ShapeDefInstructionType::LineWidth:
            return ShapeRecognitionStepResult::Continue;

        case ShapeDefInstructionType::SetDash:
            return isZeroSpaceDash(inst) ? ShapeRecognitionStepResult::Continue : ShapeRecognitionStepResult::Reject;

        case ShapeDefInstructionType::RMoveTo:
            return state->currentPath.empty() ? ShapeRecognitionStepResult::Continue : ShapeRecognitionStepResult::Reject;

        case ShapeDefInstructionType::RLineTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RLineTo>(&inst.data);
            if (!data) {
                return ShapeRecognitionStepResult::Reject;
            }
            if (data->dx == 0 && data->dy == 0) {
                return ShapeRecognitionStepResult::Continue;
            }
            state->currentPath.push_back({data->dx, data->dy});
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::Stroke:
            if (!isRightHookedVerticalPath(state->currentPath)) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentPath.clear();
            state->recognizedPath = true;
            return ShapeRecognitionStepResult::Continue;

        default:
            return ShapeRecognitionStepResult::Reject;
        }
    };

    candidate.finalize = [state]() -> bool {
        return state->recognizedPath && state->currentPath.empty();
    };

    return candidate;
}

struct CircleStemState
{
    struct CircleBounds {
        double left{};
        double right{};
        double bottom{};
        double top{};
        double centerX{};
        double centerY{};
    };

    std::vector<std::pair<dom::Evpu, dom::Evpu>> groupOrigins;
    std::optional<ShapeDefInstruction::StartObject> currentStart;
    ShapeDefInstruction::RMoveTo currentMove;
    bool hasCurrentMove = false;
    bool sawValidLineWidth = false;
    std::optional<CircleBounds> circleBounds;
    std::optional<std::pair<double, double>> stemStart;
    std::optional<std::pair<double, double>> stemEnd;
    std::optional<std::pair<dom::Evpu, dom::Evpu>> stemDelta;
    std::optional<double> stemMidY;
};

static bool isPizzicatoLineWidth(const ShapeDefInstruction::Decoded& inst)
{
    const auto* data = std::get_if<ShapeDefInstruction::LineWidth>(&inst.data);
    return data && data->efix >= PIZZICATO_LINE_WIDTH_MIN_EFIX && data->efix <= PIZZICATO_LINE_WIDTH_MAX_EFIX;
}

static bool isPizzicatoCircleBounds(const ShapeDefInstruction::StartObject& startObject)
{
    const auto width = std::abs(startObject.right - startObject.left);
    const auto height = std::abs(startObject.top - startObject.bottom);
    return std::abs(width - PIZZICATO_CIRCLE_SIZE_EVPU) <= PIZZICATO_CIRCLE_SIZE_TOLERANCE_EVPU
        && std::abs(height - PIZZICATO_CIRCLE_SIZE_EVPU) <= PIZZICATO_CIRCLE_SIZE_TOLERANCE_EVPU;
}

static CircleStemState::CircleBounds calcPizzicatoCircleBounds(const ShapeDefInstruction::StartObject& startObject)
{
    CircleStemState::CircleBounds result;
    result.left = (std::min)(static_cast<double>(startObject.left), static_cast<double>(startObject.right));
    result.right = (std::max)(static_cast<double>(startObject.left), static_cast<double>(startObject.right));
    result.bottom = (std::min)(static_cast<double>(startObject.bottom), static_cast<double>(startObject.top));
    result.top = (std::max)(static_cast<double>(startObject.bottom), static_cast<double>(startObject.top));
    result.centerX = (result.left + result.right) / 2.0;
    result.centerY = (result.bottom + result.top) / 2.0;
    return result;
}

static bool isPointInPizzicatoCircleBounds(const CircleStemState::CircleBounds& bounds, double x, double y)
{
    return x >= bounds.left - PIZZICATO_STEM_AXIS_TOLERANCE_EVPU
        && x <= bounds.right + PIZZICATO_STEM_AXIS_TOLERANCE_EVPU
        && y >= bounds.bottom - PIZZICATO_STEM_AXIS_TOLERANCE_EVPU
        && y <= bounds.top + PIZZICATO_STEM_AXIS_TOLERANCE_EVPU;
}

static std::optional<PizzicatoStemOrientation> calcCircleStemOrientation(const CircleStemState& state)
{
    if (!state.circleBounds || !state.stemStart || !state.stemEnd || !state.stemDelta || !state.stemMidY) {
        return std::nullopt;
    }

    const auto& bounds = *state.circleBounds;
    const auto [startX, startY] = *state.stemStart;
    const auto [endX, endY] = *state.stemEnd;
    const bool startInside = isPointInPizzicatoCircleBounds(bounds, startX, startY);
    const bool endInside = isPointInPizzicatoCircleBounds(bounds, endX, endY);
    if (startInside == endInside) {
        return std::nullopt;
    }

    const double insideX = startInside ? startX : endX;
    const double insideY = startInside ? startY : endY;

    const auto [dx, dy] = *state.stemDelta;
    if (std::abs(dx) >= PIZZICATO_STEM_MIN_LENGTH_EVPU && std::abs(dx) <= PIZZICATO_STEM_MAX_LENGTH_EVPU
        && std::abs(dy) <= PIZZICATO_STEM_AXIS_TOLERANCE_EVPU
        && std::abs(insideY - bounds.centerY) <= PIZZICATO_STEM_CENTER_TOLERANCE_EVPU) {
        return PizzicatoStemOrientation::Horizontal;
    }
    if (std::abs(dy) >= PIZZICATO_STEM_MIN_LENGTH_EVPU && std::abs(dy) <= PIZZICATO_STEM_MAX_LENGTH_EVPU
        && std::abs(dx) <= PIZZICATO_STEM_AXIS_TOLERANCE_EVPU
        && std::abs(insideX - bounds.centerX) <= PIZZICATO_STEM_CENTER_TOLERANCE_EVPU) {
        if (*state.stemMidY > bounds.centerY + PIZZICATO_STEM_AXIS_TOLERANCE_EVPU) {
            return PizzicatoStemOrientation::Above;
        }
        if (*state.stemMidY < bounds.centerY - PIZZICATO_STEM_AXIS_TOLERANCE_EVPU) {
            return PizzicatoStemOrientation::Below;
        }
    }
    return std::nullopt;
}

static ShapeRecognitionCandidate makeCircleStemPizzicatoRecognizer(KnownShapeDefType type, PizzicatoStemOrientation expectedOrientation)
{
    auto state = std::make_shared<CircleStemState>();

    ShapeRecognitionCandidate candidate;
    candidate.type = type;
    candidate.consume = [state](const ShapeDefInstruction::Decoded& inst) -> ShapeRecognitionStepResult {
        if (!inst.valid()) {
            return ShapeRecognitionStepResult::Reject;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartGroup: {
            const auto* data = std::get_if<ShapeDefInstruction::StartGroup>(&inst.data);
            if (!data) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->groupOrigins.emplace_back(data->originX, data->originY);
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::EndGroup:
            if (state->groupOrigins.empty() || state->currentStart) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->groupOrigins.pop_back();
            return ShapeRecognitionStepResult::Continue;

        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            if (!data || state->currentStart) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentStart = *data;
            state->currentMove = {};
            state->hasCurrentMove = false;
            state->sawValidLineWidth = false;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::LineWidth:
            if (!state->currentStart || !isPizzicatoLineWidth(inst)) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->sawValidLineWidth = true;
            return ShapeRecognitionStepResult::Continue;

        case ShapeDefInstructionType::SetDash:
            return state->currentStart && isZeroSpaceDash(inst) ? ShapeRecognitionStepResult::Continue : ShapeRecognitionStepResult::Reject;

        case ShapeDefInstructionType::RMoveTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data);
            if (!state->currentStart || !data || state->hasCurrentMove) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentMove = *data;
            state->hasCurrentMove = true;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::Ellipse: {
            const auto* data = std::get_if<ShapeDefInstruction::Ellipse>(&inst.data);
            if (!state->currentStart || !state->sawValidLineWidth || !data || state->circleBounds ||
                !isPizzicatoCircleBounds(*state->currentStart)) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->circleBounds = calcPizzicatoCircleBounds(*state->currentStart);
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::RLineTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RLineTo>(&inst.data);
            if (!state->currentStart || !state->sawValidLineWidth || !data || state->stemDelta) {
                return ShapeRecognitionStepResult::Reject;
            }
            if (std::abs(data->dx) <= PIZZICATO_STEM_AXIS_TOLERANCE_EVPU) {
                const double lineX = (static_cast<double>(state->currentStart->left) + static_cast<double>(state->currentStart->right)) / 2.0;
                state->stemStart = {lineX, static_cast<double>(state->currentStart->bottom)};
                state->stemEnd = {lineX, static_cast<double>(state->currentStart->top)};
            } else if (std::abs(data->dy) <= PIZZICATO_STEM_AXIS_TOLERANCE_EVPU) {
                const double lineY = (static_cast<double>(state->currentStart->bottom) + static_cast<double>(state->currentStart->top)) / 2.0;
                state->stemStart = {static_cast<double>(state->currentStart->left), lineY};
                state->stemEnd = {static_cast<double>(state->currentStart->right), lineY};
            } else {
                return ShapeRecognitionStepResult::Reject;
            }
            state->stemDelta = {data->dx, data->dy};
            state->stemMidY = (static_cast<double>(state->currentStart->top) + static_cast<double>(state->currentStart->bottom)) / 2.0;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::Stroke:
            if (!state->currentStart) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentStart.reset();
            state->currentMove = {};
            state->hasCurrentMove = false;
            state->sawValidLineWidth = false;
            return ShapeRecognitionStepResult::Continue;

        default:
            return ShapeRecognitionStepResult::Reject;
        }
    };

    candidate.finalize = [state, expectedOrientation]() -> bool {
        return state->groupOrigins.empty()
            && !state->currentStart
            && calcCircleStemOrientation(*state) == expectedOrientation;
    };

    return candidate;
}

struct FingernailPizzContour
{
    dom::Evpu startX{};
    dom::Evpu startY{};
    dom::Evpu endX{};
    dom::Evpu endY{};
    dom::Evpu c1dy{};
    dom::Evpu c2dy{};
    dom::Evpu edy{};
};

struct FingernailPizzState
{
    std::optional<ShapeDefInstruction::StartObject> currentStart;
    std::optional<ShapeDefInstruction::RMoveTo> currentMove;
    bool sawValidLineWidth = false;
    std::vector<FingernailPizzContour> contours;
};

static bool isFingernailPizzLineWidth(const ShapeDefInstruction::Decoded& inst)
{
    const auto* data = std::get_if<ShapeDefInstruction::LineWidth>(&inst.data);
    return data && data->efix >= PIZZICATO_LINE_WIDTH_MIN_EFIX && data->efix <= PIZZICATO_LINE_WIDTH_MAX_EFIX;
}

static std::optional<FingernailPizzDirection> calcFingernailPizzDirection(const std::vector<FingernailPizzContour>& contours)
{
    if (contours.size() != 2) {
        return std::nullopt;
    }

    std::optional<FingernailPizzDirection> direction;
    for (const auto& contour : contours) {
        const auto width = std::abs(contour.endX - contour.startX);
        const auto endpointDeltaY = std::abs(contour.endY - contour.startY);
        const auto bulge1 = contour.c1dy;
        if (width < FINGERNAIL_PIZZ_MIN_WIDTH_EVPU || width > FINGERNAIL_PIZZ_MAX_WIDTH_EVPU ||
            endpointDeltaY > FINGERNAIL_PIZZ_ENDPOINT_TOLERANCE_EVPU ||
            std::abs(bulge1) < FINGERNAIL_PIZZ_MIN_BULGE_EVPU || std::abs(bulge1) > FINGERNAIL_PIZZ_MAX_BULGE_EVPU ||
            std::abs(contour.c2dy) > FINGERNAIL_PIZZ_ENDPOINT_TOLERANCE_EVPU ||
            std::abs(contour.edy + bulge1) > FINGERNAIL_PIZZ_ENDPOINT_TOLERANCE_EVPU) {
            return std::nullopt;
        }

        const auto contourDirection = (bulge1 > 0) ? FingernailPizzDirection::CurveUp : FingernailPizzDirection::CurveDown;
        if (!direction) {
            direction = contourDirection;
        } else if (*direction != contourDirection) {
            return std::nullopt;
        }
    }

    return direction;
}

static ShapeRecognitionCandidate makeFingernailPizzRecognizer(KnownShapeDefType type, FingernailPizzDirection expectedDirection)
{
    auto state = std::make_shared<FingernailPizzState>();

    ShapeRecognitionCandidate candidate;
    candidate.type = type;
    candidate.consume = [state](const ShapeDefInstruction::Decoded& inst) -> ShapeRecognitionStepResult {
        if (!inst.valid()) {
            return ShapeRecognitionStepResult::Reject;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            if (!data || state->currentStart) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentStart = *data;
            state->currentMove.reset();
            state->sawValidLineWidth = false;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::LineWidth:
            if (!state->currentStart || !isFingernailPizzLineWidth(inst)) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->sawValidLineWidth = true;
            return ShapeRecognitionStepResult::Continue;

        case ShapeDefInstructionType::RMoveTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data);
            if (!state->currentStart || state->currentMove || !data) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentMove = *data;
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::SetDash:
            return state->currentStart && isZeroSpaceDash(inst) ? ShapeRecognitionStepResult::Continue : ShapeRecognitionStepResult::Reject;

        case ShapeDefInstructionType::CurveTo: {
            const auto* data = std::get_if<ShapeDefInstruction::CurveTo>(&inst.data);
            if (!state->currentStart || !state->currentMove || !state->sawValidLineWidth || !data) {
                return ShapeRecognitionStepResult::Reject;
            }
            FingernailPizzContour contour;
            contour.startX = state->currentStart->originX + state->currentMove->dx;
            contour.startY = state->currentStart->originY + state->currentMove->dy;
            contour.endX = contour.startX + data->c1dx + data->c2dx + data->edx;
            contour.endY = contour.startY + data->c1dy + data->c2dy + data->edy;
            contour.c1dy = data->c1dy;
            contour.c2dy = data->c2dy;
            contour.edy = data->edy;
            state->contours.push_back(contour);
            return ShapeRecognitionStepResult::Continue;
        }

        case ShapeDefInstructionType::Stroke:
            if (!state->currentStart || !state->currentMove) {
                return ShapeRecognitionStepResult::Reject;
            }
            state->currentStart.reset();
            state->currentMove.reset();
            state->sawValidLineWidth = false;
            return ShapeRecognitionStepResult::Continue;

        default:
            return ShapeRecognitionStepResult::Reject;
        }
    };

    candidate.finalize = [state, expectedDirection]() -> bool {
        return !state->currentStart
            && !state->currentMove
            && calcFingernailPizzDirection(state->contours) == expectedDirection;
    };

    return candidate;
}

struct SlurTieContour
{
    dom::Evpu16ths startX = 0;
    dom::Evpu16ths startY = 0;
    dom::Evpu16ths endX = 0;
    dom::Evpu16ths endY = 0;
    dom::Evpu16ths deltaX = 0;
    dom::Evpu16ths deltaY = 0;
    bool scaledY = false;
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
                contour.scaledY = true;
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
        const bool hasParallelContours = diff <= SLUR_TIE_HORIZONTAL_TOLERANCE_16THS;
        const bool hasScaledContour = std::any_of(state->contours.begin(), state->contours.end(),
            [](const SlurTieContour& contour) { return contour.scaledY; });
        const bool hasTaperedContours = state->contours.size() > 1
            && !hasScaledContour
            && (std::min)(std::abs(startSeparation), std::abs(endSeparation)) <= SLUR_TIE_HORIZONTAL_TOLERANCE_16THS;
        if (!hasParallelContours && !hasTaperedContours) {
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
    candidates.push_back(makeCircleStemPizzicatoRecognizer(KnownShapeDefType::SnapPizzicatoAbove, PizzicatoStemOrientation::Above));
    candidates.push_back(makeCircleStemPizzicatoRecognizer(KnownShapeDefType::SnapPizzicatoBelow, PizzicatoStemOrientation::Below));
    candidates.push_back(makeCircleStemPizzicatoRecognizer(KnownShapeDefType::BuzzPizzicato, PizzicatoStemOrientation::Horizontal));
    candidates.push_back(makeFingernailPizzRecognizer(KnownShapeDefType::FingernailPizzCurveUp, FingernailPizzDirection::CurveUp));
    candidates.push_back(makeFingernailPizzRecognizer(KnownShapeDefType::FingernailPizzCurveDown, FingernailPizzDirection::CurveDown));
    if (allowsShapeType({ShapeDef::ShapeType::Articulation, ShapeDef::ShapeType::Expression})) {
        candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveRight));
        candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveLeft));
        candidates.push_back(makeVerticalLineRightHooksRecognizer());
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

std::optional<std::pair<double, double>> calcVerticalLineRightHooksLocalYBounds(const ShapeDef& shape)
{
    if (shape.recognize() != KnownShapeDefType::VerticalLineRightHooks) {
        return std::nullopt;
    }

    std::optional<double> currentY;
    double scaleY = 1.0;
    std::optional<double> minY;
    std::optional<double> maxY;
    bool unsupported = false;

    const auto updateY = [&](double y) {
        minY = minY ? (std::min)(*minY, y) : y;
        maxY = maxY ? (std::max)(*maxY, y) : y;
    };

    const auto startObject = [&](dom::Evpu originY, int rawScaleY, int rotation) {
        if (rotation != 0) {
            unsupported = true;
            return false;
        }
        currentY = static_cast<double>(originY);
        scaleY = static_cast<double>(rawScaleY) / 1000.0;
        return true;
    };

    shape.iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
        if (!inst.valid()) {
            unsupported = true;
            return false;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            return data && startObject(data->originY, data->scaleY, data->rotation);
        }
        case ShapeDefInstructionType::StartGroup: {
            const auto* data = std::get_if<ShapeDefInstruction::StartGroup>(&inst.data);
            return data && startObject(data->originY, data->scaleY, data->rotation);
        }
        case ShapeDefInstructionType::RMoveTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data);
            if (!data || !currentY) {
                unsupported = true;
                return false;
            }
            *currentY += static_cast<double>(data->dy) * scaleY;
            return true;
        }
        case ShapeDefInstructionType::RLineTo: {
            const auto* data = std::get_if<ShapeDefInstruction::RLineTo>(&inst.data);
            if (!data || !currentY) {
                unsupported = true;
                return false;
            }
            updateY(*currentY);
            *currentY += static_cast<double>(data->dy) * scaleY;
            updateY(*currentY);
            return true;
        }
        default:
            return true;
        }
    });

    if (unsupported || !minY || !maxY) {
        return std::nullopt;
    }
    return std::make_pair(*minY, *maxY);
}

} // namespace musx::util
