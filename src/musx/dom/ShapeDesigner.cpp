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
#include <array>
#include <cmath>
#include <cstdlib>
#include <exception>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

#include "musx/musx.h"

namespace musx {
namespace dom {

// *******************************
// ***** ShapeDefInstruction *****
// *******************************

std::optional<ShapeDefInstruction::Undocumented>
ShapeDefInstruction::parseUndocumented(const std::vector<int>& data)
{
    // Always valid: just wrap the raw data
    return Undocumented{data};
}

std::optional<ShapeDefInstruction::Bracket>
ShapeDefInstruction::parseBracket(const std::vector<int>& data)
{
    if (data.empty())
        return std::nullopt;

    const int raw = data[0];

    using BS = details::Bracket::BracketStyle;

    switch (raw) {
    case static_cast<int>(BS::None):
    case static_cast<int>(BS::ThickLine):
    case static_cast<int>(BS::BracketStraightHooks):
    case static_cast<int>(BS::PianoBrace):
    case static_cast<int>(BS::Unknown4):
    case static_cast<int>(BS::Unknown5):
    case static_cast<int>(BS::BracketCurvedHooks):
    case static_cast<int>(BS::Unknown7):
    case static_cast<int>(BS::DeskBracket):
        return Bracket{static_cast<BS>(raw)};

    default:
        return std::nullopt;
    }
}

std::optional<ShapeDefInstruction::CloneChar>
ShapeDefInstruction::parseCloneChar(const std::vector<int>& data)
{
    if (data.size() >= 5) {
        return CloneChar{
            Evpu{data[0]},                      // dx
            Evpu{data[1]},                      // dy
            data[2],                            // unused2
            data[3],                            // baselineShift
            static_cast<char32_t>(data[4])      // codePoint
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::CurveTo>
ShapeDefInstruction::parseCurveTo(const std::vector<int>& data)
{
    if (data.size() >= 6) {
        return CurveTo{
            Evpu{data[0]}, Evpu{data[1]}, // c1dx, c1dy
            Evpu{data[2]}, Evpu{data[3]}, // c2dx, c2dy
            Evpu{data[4]}, Evpu{data[5]}  // edx,  edy
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::DrawChar>
ShapeDefInstruction::parseDrawChar(const std::vector<int>& data)
{
    if (data.size() >= 1) {
        return DrawChar{static_cast<char32_t>(data[0])};
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::Ellipse>
ShapeDefInstruction::parseEllipse(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return Ellipse{
            Evpu{data[0]}, // width
            Evpu{data[1]}  // height
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::ExternalGraphic>
ShapeDefInstruction::parseExternalGraphic(const std::vector<int>& data)
{
    if (data.size() >= 3) {
        return ExternalGraphic{
            Evpu{data[0]},          // width
            Evpu{data[1]},          // height
            Cmper(data[2])          // cmper
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::LineWidth>
ShapeDefInstruction::parseLineWidth(const std::vector<int>& data)
{
    if (data.size() >= 1) {
        return ShapeDefInstruction::LineWidth{Efix{data[0]}};
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::Rectangle>
ShapeDefInstruction::parseRectangle(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return Rectangle{
            Evpu{data[0]}, // width
            Evpu{data[1]}  // height
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::RLineTo>
ShapeDefInstruction::parseRLineTo(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return ShapeDefInstruction::RLineTo{Evpu{data[0]}, Evpu{data[1]}};
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::RMoveTo>
ShapeDefInstruction::parseRMoveTo(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return RMoveTo{
            Evpu{data[0]}, // dx
            Evpu{data[1]}  // dy
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::SetArrowhead>
ShapeDefInstruction::parseSetArrowhead(const std::vector<int>& data)
{
    if (data.size() >= 4) {
        return SetArrowhead{
            data[0], // startArrowId
            data[1], // endArrowId
            data[2], // startFlags
            data[3]  // endFlags
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::SetDash>
ShapeDefInstruction::parseSetDash(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return SetDash{
            Efix{data[0]}, // dashLength
            Efix{data[1]}  // spaceLength
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::SetFont>
ShapeDefInstruction::parseSetFont(const DocumentWeakPtr& document, const std::vector<int>& data)
{
    if (data.size() >= 3) {
        ShapeDefInstruction::SetFont result(document);
        result.font.fontId = Cmper(data[0]);
        result.font.fontSize = data[1];
        result.font.setEnigmaStyles(static_cast<std::uint16_t>(data[2]));
        return result;
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::SetGray>
ShapeDefInstruction::parseSetGray(const std::vector<int>& data)
{
    if (data.size() >= 1) {
        return SetGray{
            data[0] // gray 0..100
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::Slur>
ShapeDefInstruction::parseSlur(const std::vector<int>& data)
{
    if (data.size() >= 6) {
        return Slur{
            Evpu16ths{data[0]}, Evpu16ths{data[1]}, // c1dx, c1dy
            Evpu16ths{data[2]}, Evpu16ths{data[3]}, // c2dx, c2dy
            Evpu16ths{data[4]}, Evpu16ths{data[5]}  // edx,  edy
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::StartGroup>
ShapeDefInstruction::parseStartGroup(const std::vector<int>& data)
{
    if (data.size() >= 11) {
        return StartGroup{
            Evpu{data[0]},  // originX
            Evpu{data[1]},  // originY
            Evpu{data[2]},  // left
            Evpu{data[3]},  // top
            Evpu{data[4]},  // right
            Evpu{data[5]},  // bottom
            data[6],        // scaleX
            data[7],        // scaleY
            data[8],        // rotation
            data[9],        // unused9
            data[10]        // unused10
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::StartObject>
ShapeDefInstruction::parseStartObject(const std::vector<int>& data)
{
    if (data.size() >= 11) {
        return StartObject{
            Evpu{data[0]},  // originX
            Evpu{data[1]},  // originY
            Evpu{data[2]},  // left
            Evpu{data[3]},  // top
            Evpu{data[4]},  // right
            Evpu{data[5]},  // bottom
            data[6],        // scaleX
            data[7],        // scaleY
            data[8],        // rotation
            data[9],        // unused9
            data[10]        // unused10
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::VerticalMode>
ShapeDefInstruction::parseVerticalMode(const std::vector<int>& data)
{
    if (data.size() >= 1) {
        auto modeValue = data[0];
        switch (modeValue) {
        case 1:
        case 2:
        case 3:
            return VerticalMode{
                static_cast<VerticalAlign>(modeValue)
            };
        default:
            // Unknown vertical alignment mode
            return std::nullopt;
        }
    }
    return std::nullopt;
}

namespace others {

// ***************************************
// ***** Shape recognition functions *****
// ***************************************

namespace {

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

constexpr Evpu SLUR_TIE_HORIZONTAL_TOLERANCE_EVPU = 6; // 1/4 space expressed in Evpu
constexpr Evpu16ths SLUR_TIE_HORIZONTAL_TOLERANCE_16THS = SLUR_TIE_HORIZONTAL_TOLERANCE_EVPU * 16;
constexpr int SLUR_TIE_BOUND_SAMPLE_COUNT = 32;
constexpr double SLUR_TIE_VERTICAL_SCALE_THRESHOLD = 4.0;

static Evpu16ths evpuTo16ths(Evpu value)
{
    return static_cast<Evpu16ths>(value * 16);
}

static Evpu16ths evpuDoubleTo16ths(double value)
{
    return static_cast<Evpu16ths>(std::llround(value * 16.0));
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

static std::optional<std::pair<Evpu16ths, Evpu16ths>> computeScaledSlurYPositions(
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

    std::pair<Evpu16ths, Evpu16ths> result;
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
        return inst.type == ShapeDefInstructionType::SetDash;
    };

    auto lineWidthValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::LineWidth>) {
                valid = instData.efix >= 4 * EFIX_PER_EVPU && instData.efix <= 6 * EFIX_PER_EVPU;
            }
        }, inst.data);
        return valid;
    };

    auto horizontalLineValidator = [](const ShapeDefInstruction::Decoded& inst) {
        bool valid = false;
        std::visit([&](auto&& instData) {
            using T = std::decay_t<decltype(instData)>;
            if constexpr (std::is_same_v<T, ShapeDefInstruction::RLineTo>) {
                valid = instData.dx >= EVPU_PER_SPACE &&
                    instData.dx <= 1.5 * EVPU_PER_SPACE &&
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
            {ShapeDefInstructionType::Stroke, nullptr},
        },
        skipPredicate
    );
}

struct SlurTieContour
{
    Evpu16ths startX = 0;
    Evpu16ths startY = 0;
    Evpu16ths endX = 0;
    Evpu16ths endY = 0;
    Evpu16ths deltaX = 0;
    Evpu16ths deltaY = 0;
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
            const Evpu startX = state->currentStart->originX + state->currentMove->dx;
            const Evpu startYEvpu = state->currentStart->originY + state->currentMove->dy;
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
        case ShapeDefInstructionType::SetDash:
            return ShapeRecognitionStepResult::Continue;

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
        Evpu16ths startSeparation = 0;
        Evpu16ths endSeparation = 0;

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

        const Evpu16ths diff = std::abs(startSeparation - endSeparation);
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

static ShapeRecognitionCandidates createShapeRecognizers(const ShapeDef&)
{
    ShapeRecognitionCandidates candidates;
    candidates.push_back(makeTenutoRecognizer());
    candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveRight));
    candidates.push_back(makeSlurTieRecognizer(SlurTieDirection::CurveLeft));
    return candidates;
}

} // anonymous namespace

// add recognition functions as needed...

// ********************
// ***** ShapeDef *****
// ********************

std::optional<KnownShapeDefType> ShapeDef::recognize() const
{
    if (isBlank()) {
        return KnownShapeDefType::Blank;
    }

    auto recognizers = createShapeRecognizers(*this);
    if (recognizers.empty()) {
        return std::nullopt;
    }

    std::optional<KnownShapeDefType> recognized;
    iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
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

    if (recognized) {
        return recognized;
    }

    for (auto& recognizer : recognizers) {
        if (!recognizer.rejected && recognizer.finalize()) {
            return recognizer.type;
        }
    }

    return std::nullopt;
}

std::optional<Evpu> ShapeDef::calcWidth() const
{
    if (isBlank()) {
        return Evpu{0};
    }

    Evpu minLeft = (std::numeric_limits<Evpu>::max)();
    Evpu maxRight = std::numeric_limits<Evpu>::lowest();
    bool hasBounds = false;
    bool unsupported = false;

    const auto updateBounds = [&](Evpu left, Evpu right) {
        const Evpu normalizedLeft = (std::min)(left, right);
        const Evpu normalizedRight = (std::max)(left, right);
        minLeft = hasBounds ? (std::min)(minLeft, normalizedLeft) : normalizedLeft;
        maxRight = hasBounds ? (std::max)(maxRight, normalizedRight) : normalizedRight;
        hasBounds = true;
    };

    iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
        if (!inst.valid()) {
            unsupported = true;
            return false;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            if (data) {
                if (data->left == (std::numeric_limits<Evpu>::min)() ||
                    data->right == (std::numeric_limits<Evpu>::min)() ||
                    data->left == (std::numeric_limits<Evpu>::max)() ||
                    data->right == (std::numeric_limits<Evpu>::max)()) {
                    unsupported = true;
                    return false;
                }
                updateBounds(data->left, data->right);
            }
            break;
        }
        case ShapeDefInstructionType::StartGroup: {
            const auto* data = std::get_if<ShapeDefInstruction::StartGroup>(&inst.data);
            if (data) {
                if (data->left == (std::numeric_limits<Evpu>::min)() ||
                    data->right == (std::numeric_limits<Evpu>::min)() ||
                    data->left == (std::numeric_limits<Evpu>::max)() ||
                    data->right == (std::numeric_limits<Evpu>::max)()) {
                    unsupported = true;
                    return false;
                }
                updateBounds(data->left, data->right);
            }
            break;
        }
        case ShapeDefInstructionType::SetFont:
        case ShapeDefInstructionType::DrawChar:
        case ShapeDefInstructionType::CloneChar:
            unsupported = true;
            return false;
        default:
            break;
        }

        return true;
    });

    if (unsupported || !hasBounds) {
        return std::nullopt;
    }
    if (maxRight <= minLeft) {
        return Evpu{0};
    }
    return maxRight - minLeft;
}

CurveContourDirection ShapeDef::calcSlurContour() const
{
    if (isBlank()) {
        return CurveContourDirection::Auto;
    }

    auto isSentinel = [](Evpu value) {
        return value == (std::numeric_limits<Evpu>::min)() ||
            value == (std::numeric_limits<Evpu>::max)();
    };

    auto boundsValid = [&](const ShapeDefInstruction::StartObject& obj) {
        return !isSentinel(obj.top) && !isSentinel(obj.bottom);
    };

    std::optional<Evpu> maxTop;
    std::optional<Evpu> minBottom;
    bool hasSlur = false;
    bool unsupported = false;
    std::optional<ShapeDefInstruction::StartObject> currentStart;

    iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
        if (!inst.valid()) {
            unsupported = true;
            return false;
        }

        switch (inst.type) {
        case ShapeDefInstructionType::StartObject: {
            const auto* data = std::get_if<ShapeDefInstruction::StartObject>(&inst.data);
            if (!data || !boundsValid(*data)) {
                unsupported = true;
                return false;
            }
            currentStart = *data;
            break;
        }
        case ShapeDefInstructionType::RMoveTo:
        case ShapeDefInstructionType::SetDash:
        case ShapeDefInstructionType::FillSolid:
        case ShapeDefInstructionType::Stroke:
            break;
        case ShapeDefInstructionType::Slur: {
            if (!currentStart) {
                unsupported = true;
                return false;
            }
            if (!std::get_if<ShapeDefInstruction::Slur>(&inst.data)) {
                unsupported = true;
                return false;
            }

            const Evpu top = currentStart->top;
            const Evpu bottom = currentStart->bottom;
            maxTop = maxTop ? (std::max)(*maxTop, top) : top;
            minBottom = minBottom ? (std::min)(*minBottom, bottom) : bottom;
            hasSlur = true;
            currentStart.reset();
            break;
        }
        case ShapeDefInstructionType::StartGroup:
        case ShapeDefInstructionType::SetFont:
        case ShapeDefInstructionType::DrawChar:
        case ShapeDefInstructionType::CloneChar:
        default:
            unsupported = true;
            return false;
        }

        return true;
    });

    if (unsupported || !hasSlur) {
        return CurveContourDirection::Auto;
    }

    const Evpu topExtent = (maxTop && *maxTop > 0) ? *maxTop : Evpu{0};
    const Evpu bottomExtent = (minBottom && *minBottom < 0) ? static_cast<Evpu>(-(*minBottom)) : Evpu{0};

    if (topExtent == 0 && bottomExtent == 0) {
        return CurveContourDirection::Auto;
    }

    return (topExtent >= bottomExtent) ? CurveContourDirection::Up : CurveContourDirection::Down;
}

bool ShapeDef::iterateInstructions(std::function<bool(ShapeDefInstructionType, std::vector<int>)> callback) const
{
    if (instructionList == 0 && dataList == 0) {
        return true; // nothing to do if no data
    }

    auto insts = getDocument()->getOthers()->get<ShapeInstructionList>(getRequestedPartId(), instructionList);
    auto data = getDocument()->getOthers()->get<ShapeData>(getRequestedPartId(), dataList);
    bool result = true;

    if (insts && data) {
        size_t currentDataIndex = 0;
        for (const auto& inst : insts->instructions) {
            if (currentDataIndex + inst->numData > data->values.size()) {
                throw std::invalid_argument("ShapeDef " + std::to_string(getCmper()) + " does not have enough data for instructions.");
            }
            if (!callback(inst->type, { data->values.begin() + currentDataIndex, data->values.begin() + currentDataIndex + inst->numData })) {
                return false;
            }
            currentDataIndex += inst->numData;
        }
    } else {
        result = false;
        MUSX_INTEGRITY_ERROR("ShapeDef " + std::to_string(getCmper()) + " is missing instructions and/or data.");
    }
    return result;
}

bool ShapeDef::iterateInstructions(std::function<bool(const ShapeDefInstruction::Decoded&)> callback) const
{
    return iterateInstructions([&](ShapeDefInstructionType instType, std::vector<int> instData) -> bool {
        using IT = ShapeDefInstructionType;
        ShapeDefInstruction::Decoded decoded;
        decoded.type = instType;
        bool result = true;

        switch (instType) {
            // --------------------------
            // Payload-bearing instructions
            // --------------------------
        case IT::Undocumented:
            decoded.setPayload(ShapeDefInstruction::parseUndocumented(instData));
            break;

        case IT::Bracket:
            decoded.setPayload(ShapeDefInstruction::parseBracket(instData));
            break;

        case IT::CloneChar:
            decoded.setPayload(ShapeDefInstruction::parseCloneChar(instData));
            break;

        case IT::CurveTo:
            decoded.setPayload(ShapeDefInstruction::parseCurveTo(instData));
            break;

        case IT::DrawChar:
            decoded.setPayload(ShapeDefInstruction::parseDrawChar(instData));
            break;

        case IT::Ellipse:
            decoded.setPayload(ShapeDefInstruction::parseEllipse(instData));
            break;

        case IT::ExternalGraphic:
            decoded.setPayload(ShapeDefInstruction::parseExternalGraphic(instData));
            break;

        case IT::LineWidth:
            decoded.setPayload(ShapeDefInstruction::parseLineWidth(instData));
            break;

        case IT::Rectangle:
            decoded.setPayload(ShapeDefInstruction::parseRectangle(instData));
            break;

        case IT::RLineTo:
            decoded.setPayload(ShapeDefInstruction::parseRLineTo(instData));
            break;

        case IT::RMoveTo:
            decoded.setPayload(ShapeDefInstruction::parseRMoveTo(instData));
            break;

        case IT::SetArrowhead:
            decoded.setPayload(ShapeDefInstruction::parseSetArrowhead(instData));
            break;

        case IT::SetDash:
            decoded.setPayload(ShapeDefInstruction::parseSetDash(instData));
            break;

        case IT::SetFont:
            decoded.setPayload(ShapeDefInstruction::parseSetFont(getDocument(), instData));
            break;

        case IT::SetGray:
            decoded.setPayload(ShapeDefInstruction::parseSetGray(instData));
            break;

        case IT::Slur:
            decoded.setPayload(ShapeDefInstruction::parseSlur(instData));
            break;

        case IT::StartGroup:
            decoded.setPayload(ShapeDefInstruction::parseStartGroup(instData));
            break;

        case IT::StartObject:
            decoded.setPayload(ShapeDefInstruction::parseStartObject(instData));
            break;

        case IT::VerticalMode:
            decoded.setPayload(ShapeDefInstruction::parseVerticalMode(instData));
            break;

            // --------------------------
            // No-payload instructions
            // --------------------------
        case IT::ClosePath:
        case IT::EndGroup:
        case IT::FillAlt:
        case IT::FillSolid:
        case IT::GoToOrigin:
        case IT::GoToStart:
        case IT::SetBlack:
        case IT::SetWhite:
        case IT::Stroke:
            // Leave as monostate + valid
            break;
        }

        if (!decoded.valid()) {
            result = false;
            MUSX_INTEGRITY_ERROR("ShapeDef " + std::to_string(getCmper()) +
                " has insufficient data for instruction type " + std::to_string(int(decoded.type)) + ".");
        }
        else if (!callback(decoded)) {
            result = false;
        }

        return result;
    });
}

} // namespace others
} // namespace dom
} // namespace musx
