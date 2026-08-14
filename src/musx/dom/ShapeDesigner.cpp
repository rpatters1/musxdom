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
#include <initializer_list>
#include <limits>
#include <memory>
#include <optional>
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
            data[0], // packedKindCodes
            data[1], // startArrowId (observed in current fixtures)
            data[2], // endArrowId (observed in current fixtures)
            data[3]  // extra (undocumented)
        };
    }
    return std::nullopt;
}

std::optional<ShapeDefInstruction::SetDash>
ShapeDefInstruction::parseSetDash(const std::vector<int>& data)
{
    if (data.size() >= 2) {
        return SetDash{
            Evpu{data[0]}, // dashLength
            Evpu{data[1]}  // spaceLength
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

std::vector<int> ShapeDefInstruction::encodeSetFont(const FontInfo& font)
{
    // Kept beside parseSetFont so the stored order is stated once. A caller rewriting a font
    // reference in place would otherwise have to restate which item holds the id.
    return {
        int(font.fontId),
        font.fontSize,
        int(font.getEnigmaStyles())
    };
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

// ********************
// ***** ShapeDef *****
// ********************

KnownShapeDefType ShapeDef::recognize() const
{
    const auto doc = getDocument();
    if (!doc) {
        return KnownShapeDefType::Unrecognized;
    }

    if (auto cached = doc->getCachedShapeRecognition(getCmper())) {
        return *cached;
    }

    const auto recognized = musx::util::recognizeShape(*this);
    doc->setCachedShapeRecognition(getCmper(), recognized);
    return recognized;
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
        return CurveContourDirection::Unspecified;
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
        return CurveContourDirection::Unspecified;
    }

    const Evpu topExtent = (maxTop && *maxTop > 0) ? *maxTop : Evpu{0};
    const Evpu bottomExtent = (minBottom && *minBottom < 0) ? static_cast<Evpu>(-(*minBottom)) : Evpu{0};

    if (topExtent == 0 && bottomExtent == 0) {
        return CurveContourDirection::Unspecified;
    }

    return (topExtent >= bottomExtent) ? CurveContourDirection::Up : CurveContourDirection::Down;
}

bool ShapeDef::isBlank() const
{
    if (instructionList == 0) {
        return true;
    }
    // Some legacy documents retain a nonzero reference to an intentionally empty
    // instruction collection. A missing collection is unresolved rather than blank.
    const auto instructions = getDocument()->getOthers()->get<ShapeInstructionList>(
        getRequestedPartId(), instructionList);
    if (!instructions) {
        util::Logger::log(util::Logger::LogLevel::Verbose,
            "ShapeDef " + std::to_string(getCmper()) + " references missing instruction list "
                + std::to_string(instructionList) + ".");
        return false;
    }
    return instructions->instructions.empty();
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


std::optional<Cmper> importShapeDefInto(const DocumentPtr& target,
    const MusxInstance<ShapeDef>& source)
{
    MUSX_ASSERT_IF(!target) {
        throw std::invalid_argument("importShapeDefInto received a null target document");
    }
    MUSX_ASSERT_IF(!source) {
        throw std::invalid_argument("importShapeDefInto received a null shape");
    }

    const auto sourceDocument = source->getDocument();
    const auto newShapeId = target->getOthers()->nextFreeCmper<ShapeDef>(SCORE_PARTID);
    if (!newShapeId) {
        return std::nullopt;
    }

    // SCORE_PARTID and ShareMode::All are correct for anything newly created: no linked part can
    // reference an object that did not exist a moment ago, so there is nothing to unshare from.
    // An importer for a ShareMode::None class would know to say so.
    auto shape = std::make_shared<ShapeDef>(
        target, SCORE_PARTID, EnigmaBase::ShareMode::All, *newShapeId);
    shape->shapeType = source->shapeType;

    // A shape that draws nothing owns no lists to copy.
    if (source->instructionList == 0 && source->dataList == 0) {
        target->getOthers()->add(ShapeDef::XmlNodeName, shape);
        return *newShapeId;
    }

    const auto instructions = sourceDocument->getOthers()->get<ShapeInstructionList>(
        source->getRequestedPartId(), source->instructionList);
    const auto data = sourceDocument->getOthers()->get<ShapeData>(
        source->getRequestedPartId(), source->dataList);
    if (!instructions || !data) {
        return std::nullopt;
    }

    // Every reference the instructions carry is resolved against the target before anything is
    // added, so a shape that cannot be copied faithfully leaves no partial shape behind.
    std::vector<int> values = data->values;
    std::size_t dataIndex = 0;
    for (const auto& instruction : instructions->instructions) {
        const auto count = static_cast<std::size_t>(instruction->numData);
        if (dataIndex + count > values.size()) {
            return std::nullopt;
        }
        if (instruction->type == ShapeDefInstructionType::ExternalGraphic) {
            /// @todo Copy the embedded graphic and remap the cmper, as is done for fonts below.
            // Until then the instruction names a graphic by a cmper meaningful only in the source
            // document, and refusing is the only answer that does not produce a shape pointing at
            // the wrong thing.
            return std::nullopt;
        }
        if (instruction->type == ShapeDefInstructionType::SetFont) {
            const std::vector<int> stored(values.begin() + dataIndex,
                values.begin() + dataIndex + count);
            const auto parsed = ShapeDefInstruction::parseSetFont(sourceDocument, stored);
            if (!parsed) {
                return std::nullopt;
            }
            FontInfo font = parsed->font;
            if (const auto sourceFont = sourceDocument->getOthers()->get<FontDefinition>(
                    SCORE_PARTID, font.fontId)) {
                const auto resolved = importFontDefinitionInto(target, sourceFont);
                if (!resolved) {
                    return std::nullopt;
                }
                font.fontId = *resolved;
            } else if (font.fontId != 0) {
                // The source names a font it does not itself define, so there is nothing to
                // resolve and the number cannot be carried across.
                return std::nullopt;
            }
            const auto encoded = ShapeDefInstruction::encodeSetFont(font);
            for (std::size_t i = 0; i < encoded.size() && i < count; ++i) {
                values[dataIndex + i] = encoded[i];
            }
        }
        dataIndex += count;
    }

    const auto newInstructionsId =
        target->getOthers()->nextFreeCmper<ShapeInstructionList>(SCORE_PARTID);
    const auto newDataId = target->getOthers()->nextFreeCmper<ShapeData>(SCORE_PARTID);
    if (!newInstructionsId || !newDataId) {
        return std::nullopt;
    }

    auto newInstructions = std::make_shared<ShapeInstructionList>(
        target, SCORE_PARTID, EnigmaBase::ShareMode::All, *newInstructionsId);
    for (const auto& instruction : instructions->instructions) {
        auto copy = std::make_shared<ShapeInstructionList::InstructionInfo>();
        copy->numData = instruction->numData;
        copy->type = instruction->type;
        newInstructions->instructions.push_back(std::move(copy));
    }

    auto newData = std::make_shared<ShapeData>(
        target, SCORE_PARTID, EnigmaBase::ShareMode::All, *newDataId);
    newData->values = std::move(values);

    // The three pools number independently, so the copied ShapeDef is rewired rather than
    // assuming the source's numbers travel together.
    shape->instructionList = *newInstructionsId;
    shape->dataList = *newDataId;

    target->getOthers()->add(ShapeInstructionList::XmlNodeName, newInstructions);
    target->getOthers()->add(ShapeData::XmlNodeName, newData);
    target->getOthers()->add(ShapeDef::XmlNodeName, shape);
    return *newShapeId;
}

} // namespace others
} // namespace dom
} // namespace musx
