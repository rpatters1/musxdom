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

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::dom;

constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
    <others>
        <shapeData cmper="5">
            <data>0</data>
            <data>2</data>
            <data>0</data>
            <data>2</data>
            <data>28</data>
            <data>2</data>
            <data>1000</data>
            <data>1000</data>
            <data>0</data>
            <data>0</data>
            <data>0</data>
            <data>0</data>
            <data>0</data>
            <data>256</data>
            <data>18</data>
            <data>0</data>
            <data>28</data>
            <data>0</data>
        </shapeData>
        <shapeDef cmper="6">
            <instList>7</instList>
            <dataList>5</dataList>
            <shapeType>articulation</shapeType>
        </shapeDef>
        <shapeList cmper="7">
            <instruct>
                <numData>11</numData>
                <tag>startObject</tag>
            </instruct>
            <instruct>
                <numData>2</numData>
                <tag>rMoveTo</tag>
            </instruct>
            <instruct>
                <numData>1</numData>
                <tag>lineWidth</tag>
            </instruct>
            <instruct>
                <numData>2</numData>
                <tag>setDash</tag>
            </instruct>
            <instruct>
                <numData>2</numData>
                <tag>rLineTo</tag>
            </instruct>
            <instruct>
                <tag>stroke</tag>
            </instruct>
        </shapeList>
    </others>
</finale>
    )xml";
    
TEST(ShapeDefTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto shapeDef = others->get<others::ShapeDef>(SCORE_PARTID, 6);
    ASSERT_TRUE(shapeDef) << "ShapeDef 6 not found but does exist";
    EXPECT_EQ(shapeDef->shapeType, others::ShapeDef::ShapeType::Articulation);
    EXPECT_EQ(shapeDef->instructionList, 7);
    EXPECT_EQ(shapeDef->dataList, 5);

    auto shapeData = others->get<others::ShapeData>(SCORE_PARTID, 5);
    ASSERT_TRUE(shapeData) << "ShapeData 5 not found but does exist";
    ASSERT_EQ(shapeData->data.size(), 18);
    EXPECT_EQ(shapeData->data[0], 0);
    EXPECT_EQ(shapeData->data[1], 2);
    EXPECT_EQ(shapeData->data[2], 0);
    EXPECT_EQ(shapeData->data[3], 2);
    EXPECT_EQ(shapeData->data[4], 28);
    EXPECT_EQ(shapeData->data[5], 2);
    EXPECT_EQ(shapeData->data[6], 1000);
    EXPECT_EQ(shapeData->data[7], 1000);

    auto shapeList = others->get<others::ShapeInstructionList>(SCORE_PARTID, 7);
    ASSERT_TRUE(shapeList) << "ShapeInstructionList 7 not found but does exist";
    ASSERT_EQ(shapeList->instructions.size(), 6);
    EXPECT_EQ(shapeList->instructions[0]->type, others::ShapeDef::InstructionType::StartObject);
    EXPECT_EQ(shapeList->instructions[0]->numData, 11);
    EXPECT_EQ(shapeList->instructions[1]->type, others::ShapeDef::InstructionType::RMoveTo);
    EXPECT_EQ(shapeList->instructions[1]->numData, 2);
    EXPECT_EQ(shapeList->instructions[2]->type, others::ShapeDef::InstructionType::LineWidth);
    EXPECT_EQ(shapeList->instructions[2]->numData, 1);
    EXPECT_EQ(shapeList->instructions[3]->type, others::ShapeDef::InstructionType::SetDash);
    EXPECT_EQ(shapeList->instructions[3]->numData, 2);
    EXPECT_EQ(shapeList->instructions[4]->type, others::ShapeDef::InstructionType::RLineTo);
    EXPECT_EQ(shapeList->instructions[4]->numData, 2);
    EXPECT_EQ(shapeList->instructions[5]->type, others::ShapeDef::InstructionType::Stroke);
}

TEST(ShapeDefTest, InterateInstructions)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto shapeDef = others->get<others::ShapeDef>(SCORE_PARTID, 6);
    ASSERT_TRUE(shapeDef) << "ShapeDef 6 not found but does exist";
    EXPECT_EQ(shapeDef->shapeType, others::ShapeDef::ShapeType::Articulation);
    EXPECT_EQ(shapeDef->instructionList, 7);
    EXPECT_EQ(shapeDef->dataList, 5);

    auto shapeData = others->get<others::ShapeData>(SCORE_PARTID, shapeDef->dataList);
    ASSERT_TRUE(shapeData) << "ShapeData " << shapeDef->dataList << " not found but does exist";
    ASSERT_EQ(shapeData->data.size(), 18);

    auto shapeList = others->get<others::ShapeInstructionList>(SCORE_PARTID, shapeDef->instructionList);
    ASSERT_TRUE(shapeList) << "ShapeInstructionList " << shapeDef->instructionList << " not found but does exist";
    ASSERT_EQ(shapeList->instructions.size(), 6);

    size_t nextIndex = 0;
    size_t nextDataIndex = 0;

    shapeDef->iterateInstructions([&](others::ShapeDef::InstructionType type, std::vector<int> data) {
        EXPECT_LT(nextIndex, shapeList->instructions.size()) << "next index exceeds size of instructions array";
        if (nextIndex >= shapeList->instructions.size()) {
            return false;
        }
        EXPECT_LE(nextDataIndex + data.size(), shapeData->data.size()) << "next data index exceeds size of data array";
        if (nextDataIndex + data.size() > shapeData->data.size()) {
            return false;
        }
        EXPECT_EQ(type, shapeList->instructions[nextIndex]->type);
        EXPECT_EQ(data.size(), shapeList->instructions[nextIndex]->numData);
        for (size_t x = 0; x < data.size(); x++) {
            EXPECT_EQ(data[x], shapeData->data[nextDataIndex + x]);
        }
        nextIndex++;
        nextDataIndex += data.size();
        return true;
    });
    EXPECT_EQ(nextIndex, shapeList->instructions.size()) << "iteration value " << nextIndex
        << " does not equal number of instructions " << shapeList->instructions.size();
}
