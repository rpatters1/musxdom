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

TEST(ArticulationDefTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
<others>
    <articDef cmper="5">
    <charMain>58528</charMain>
    <fontMain>10</fontMain>
    <sizeMain>24</sizeMain>
    <efxMain>
        <bold/>
        <underline/>
    </efxMain>
    <autoHorz/>
    <autoVert/>
    <autoVertMode>autoNoteStem</autoVertMode>
    <outsideStaff/>
    <aboveSymbolAlt/>
    <autoStack/>
    <slurInteractionMode>avoidSlur</slurInteractionMode>
    <charAlt>58529</charAlt>
    <fontAlt>10</fontAlt>
    <xOffsetMain>1</xOffsetMain>
    <yOffsetMain>2</yOffsetMain>
    <defVertPos>14</defVertPos>
    <playArtic/>
    <sizeAlt>24</sizeAlt>
    <efxAlt>
        <bold/>
        <underline/>
    </efxAlt>
    <xOffsetAlt>3</xOffsetAlt>
    <yOffsetAlt>4</yOffsetAlt>
    <startTopNotePercent>7</startTopNotePercent>
    <startBotNotePercent>8</startBotNotePercent>
    <durTopNotePercent>1</durTopNotePercent>
    <durBotNotePercent>2</durBotNotePercent>
    <ampTopNoteDelta>125</ampTopNoteDelta>
    <ampBotNoteDelta>125</ampBotNoteDelta>
    </articDef>
</others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto articDef = others->get<others::ArticulationDef>(SCORE_PARTID, 5);
    ASSERT_TRUE(articDef) << "ArticulationDef 5 not found but does exist";

    EXPECT_EQ(articDef->charMain, 58528);
    EXPECT_EQ(articDef->fontMain->fontId, 10);
    EXPECT_EQ(articDef->fontMain->fontSize, 24);
    EXPECT_FALSE(articDef->fontMain->italic);
    EXPECT_TRUE(articDef->fontMain->bold);
    EXPECT_TRUE(articDef->fontMain->underline);
    EXPECT_TRUE(articDef->autoHorz);
    EXPECT_TRUE(articDef->autoVert);
    EXPECT_EQ(articDef->autoVertMode, others::ArticulationDef::AutoVerticalMode::AutoNoteStem);
    EXPECT_TRUE(articDef->outsideStaff);
    EXPECT_TRUE(articDef->aboveSymbolAlt);
    EXPECT_TRUE(articDef->autoStack);
    EXPECT_EQ(articDef->slurInteractionMode, others::ArticulationDef::SlurInteractionMode::AvoidSlur);
    EXPECT_EQ(articDef->charAlt, 58529);
    EXPECT_EQ(articDef->fontAlt->fontId, 10);
    EXPECT_EQ(articDef->fontAlt->fontSize, 24);
    EXPECT_FALSE(articDef->fontAlt->italic);
    EXPECT_TRUE(articDef->fontAlt->bold);
    EXPECT_TRUE(articDef->fontAlt->underline);
    EXPECT_EQ(articDef->xOffsetMain, 1);
    EXPECT_EQ(articDef->yOffsetMain, 2);
    EXPECT_EQ(articDef->defVertPos, 14);
    EXPECT_TRUE(articDef->playArtic);
    EXPECT_EQ(articDef->xOffsetAlt, 3);
    EXPECT_EQ(articDef->yOffsetAlt, 4);
    EXPECT_EQ(articDef->startTopNotePercent, 7);
    EXPECT_EQ(articDef->startBotNotePercent, 8);
    EXPECT_EQ(articDef->durTopNotePercent, 1);
    EXPECT_EQ(articDef->durBotNotePercent, 2);
    EXPECT_EQ(articDef->ampTopNoteDelta, 125);
    EXPECT_EQ(articDef->ampBotNoteDelta, 125);
}
