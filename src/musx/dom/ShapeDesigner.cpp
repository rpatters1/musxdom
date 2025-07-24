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
#include <string>
#include <vector>
#include <cstdlib>
#include <exception>

#include "musx/musx.h"

namespace musx {
namespace dom {
namespace others {

// ***************************************
// ***** Shape recognition functions *****
// ***************************************

static bool isTenutoMark(const ShapeDef& shape)
{
    static const std::vector<ShapeDef::InstructionType> expectedInsts = {
        ShapeDef::InstructionType::StartObject,
        ShapeDef::InstructionType::RMoveTo,
        ShapeDef::InstructionType::LineWidth,
        ShapeDef::InstructionType::RLineTo,
        ShapeDef::InstructionType::Stroke,
    };

    size_t nextIndex = 0;
    shape.iterateInstructions([&](ShapeDef::InstructionType inst, std::vector<int> data) {
        if (inst == ShapeDef::InstructionType::SetDash) {
            return true; // skip SetDash
        }
        if (nextIndex >= expectedInsts.size()) {
            nextIndex++; // assure no tenuto is returned
            return false;
        }
        if (inst != expectedInsts[nextIndex]) {
            return false;
        }
        if (const auto lineWidth = ShapeInstruction::parseLineWidth(inst, data)) {
            if (lineWidth->efix < 4 * EFIX_PER_EVPU || lineWidth->efix > 6 * EFIX_PER_EVPU) {
                return false;
            }
        }
        else if (const auto rLineTo = ShapeInstruction::parseRLineTo(inst, data)) {
            if (rLineTo->dx < EVPU_PER_SPACE || rLineTo->dx > 1.5 * EVPU_PER_SPACE || rLineTo->dy != 0) {
                return false;
            }
        }
        nextIndex++;
        return true;
    });
    return nextIndex == expectedInsts.size();
}

// add recognition functions as needed...

// ********************
// ***** ShapeDef *****
// ********************

std::optional<KnownShapeDefType> ShapeDef::recognize() const
{
    if (isTenutoMark(*this)) {
        return KnownShapeDefType::TenutoMark;
    }
    return std::nullopt;
}

void ShapeDef::iterateInstructions(std::function<bool(ShapeDef::InstructionType, std::vector<int>)> callback) const
{
    auto insts = getDocument()->getOthers()->get<ShapeInstructionList>(getPartId(), instructionList);
    auto data = getDocument()->getOthers()->get<ShapeData>(getPartId(), dataList);
    if (insts && data) {
        size_t currentDataIndex = 0;
        for (const auto& inst : insts->instructions) {
            if (currentDataIndex + inst->numData > data->values.size()) {
                throw std::invalid_argument("ShapeDef " + std::to_string(getCmper()) + " does not have enough data for instructions.");
            }
            if (!callback(inst->type, { data->values.begin() + currentDataIndex, data->values.begin() + currentDataIndex + inst->numData })) {
                break;
            }
            currentDataIndex += inst->numData;
        }
    } else {
        MUSX_INTEGRITY_ERROR("ShapeDef " + std::to_string(getCmper()) + " is missing instructions and/or data.");
    }
}

// ****************************
// ***** ShapeInstruction *****
// ****************************

std::optional<ShapeInstruction::LineWidth> ShapeInstruction::parseLineWidth(ShapeDef::InstructionType type, const std::vector<int>& data)
{
    if (type == ShapeDef::InstructionType::LineWidth && data.size() >= 1)
        return ShapeInstruction::LineWidth{ data[0] };
    return std::nullopt;
}

std::optional<ShapeInstruction::RLineTo> ShapeInstruction::parseRLineTo(ShapeDef::InstructionType type, const std::vector<int>& data)
{
    if (type == ShapeDef::InstructionType::RLineTo && data.size() >= 2)
        return ShapeInstruction::RLineTo{data[0], data[1]};
    return std::nullopt;
}

std::optional<FontInfo> ShapeInstruction::parseSetFont(const DocumentWeakPtr& document, ShapeDef::InstructionType type, const std::vector<int>& data)
{
    if (type == ShapeDef::InstructionType::SetFont && data.size() >= 3) {
        FontInfo result(document);
        result.fontId = Cmper(data[0]);
        result.fontSize = data[1];
        result.setEnigmaStyles(uint16_t(data[2]));
        return result;
    }
    return std::nullopt;
}

} // namespace others
} // namespace dom
} // namespace musx