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
    <copyMode>both</copyMode>
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
    EXPECT_EQ(articDef->copyMode, others::ArticulationDef::CopyMode::Vertical);
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

TEST(ArticulationAssignTest, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <articAssign entnum="125" inci="0">
      <articDef>67</articDef>
      <horzOff>-35</horzOff>
      <vertOff>-2</vertOff>
      <vertAdd>-86</vertAdd>
      <overridePlacement/>
      <numSlursAvoided>-1</numSlursAvoided>
    </articAssign>
    <articAssign entnum="125" inci="1">
      <articDef>17</articDef>
      <horzOff>16</horzOff>
      <horzAdd>204</horzAdd>
      <vertOff>51</vertOff>
      <hide/>
      <neverStack/>
      <avoidSlur/>
      <overridePlacement/>
      <aboveEntry/>
      <numSlursAvoided>-1</numSlursAvoided>
    </articAssign>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details) << "Details node not found in XML";

    // Check first articulation assignment
    auto artic1 = details->get<details::ArticulationAssign>(SCORE_PARTID, 125, 0);
    ASSERT_TRUE(artic1) << "ArticulationAssign with entnum=125, inci=0 not found but does exist";
    EXPECT_EQ(artic1->articDef, 67);         // From XML
    EXPECT_EQ(artic1->horzOffset, -35);      // From XML
    EXPECT_EQ(artic1->vertOffset, -2);       // From XML
    EXPECT_EQ(artic1->vertAdd, -86);         // From XML
    EXPECT_TRUE(artic1->overridePlacement);  // From XML
    EXPECT_EQ(artic1->numSlursAvoided, -1);  // From XML
    EXPECT_FALSE(artic1->hide);              // Not present in XML
    EXPECT_FALSE(artic1->neverStack);        // Not present in XML
    EXPECT_FALSE(artic1->avoidSlur);         // Not present in XML
    EXPECT_FALSE(artic1->aboveEntry);        // Not present in XML

    // Check second articulation assignment
    auto artic2 = details->get<details::ArticulationAssign>(SCORE_PARTID, 125, 1);
    ASSERT_TRUE(artic2) << "ArticulationAssign with entnum=125, inci=1 not found but does exist";
    EXPECT_EQ(artic2->articDef, 17);         // From XML
    EXPECT_EQ(artic2->horzOffset, 16);       // From XML
    EXPECT_EQ(artic2->horzAdd, 204);         // From XML
    EXPECT_EQ(artic2->vertOffset, 51);       // From XML
    EXPECT_TRUE(artic2->hide);               // From XML
    EXPECT_TRUE(artic2->neverStack);         // From XML
    EXPECT_TRUE(artic2->avoidSlur);          // From XML
    EXPECT_TRUE(artic2->overridePlacement);  // From XML
    EXPECT_TRUE(artic2->aboveEntry);         // From XML
    EXPECT_EQ(artic2->numSlursAvoided, -1);  // From XML

    // Ensure that an invalid articulation assignment isn't found
    auto articInvalid = details->get<details::ArticulationAssign>(SCORE_PARTID, 999, 0);
    EXPECT_FALSE(articInvalid) << "ArticulationAssign with entnum=999 found but does not exist";
}
