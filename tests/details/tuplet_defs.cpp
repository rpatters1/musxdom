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
  <details>
    <tupletDef entnum="6" inci="0">
      <symbolicNum>3</symbolicNum>
      <symbolicDur>1024</symbolicDur>
      <refNum>2</refNum>
      <refDur>1024</refDur>
      <flat/>
      <avoidStaff/>
      <autoBracketStyle>neverBeamSide</autoBracketStyle>
      <tupOffY>67</tupOffY>
      <numStyle>number</numStyle>
      <posStyle>beamSide</posStyle>
      <breakBracket/>
      <matchHooks/>
      <brackStyle>bracket</brackStyle>
      <smartTuplet/>
      <leftHookLen>12</leftHookLen>
      <rightHookLen>12</rightHookLen>
    </tupletDef>
    <tupletDef entnum="6" inci="1">
      <symbolicNum>3</symbolicNum>
      <symbolicDur>512</symbolicDur>
      <refNum>2</refNum>
      <refDur>512</refDur>
      <avoidStaff/>
      <autoBracketStyle>neverBeamSide</autoBracketStyle>
      <tupOffY>24</tupOffY>
      <numStyle>number</numStyle>
      <posStyle>beamSide</posStyle>
      <breakBracket/>
      <matchHooks/>
      <brackStyle>bracket</brackStyle>
      <smartTuplet/>
      <leftHookLen>-12</leftHookLen>
      <rightHookLen>-12</rightHookLen>
    </tupletDef>
    <tupletDef entnum="6" inci="0" part="1" shared="true"/>
    <tupletDef entnum="6" inci="1" part="1" shared="true">
      <tupOffX>0</tupOffX>
      <tupOffY>16</tupOffY>
      <brackOffX>0</brackOffX>
      <brackOffY>0</brackOffY>
      <slope>0</slope>
    </tupletDef>
    <tupletDef entnum="7" inci="0">
      <symbolicNum>3</symbolicNum>
      <symbolicDur>512</symbolicDur>
      <refNum>2</refNum>
      <refDur>512</refDur>
      <flat/>
      <fullDura/>
      <metricCenter/>
      <avoidStaff/>
      <autoBracketStyle>neverBeamSide</autoBracketStyle>
      <tupOffX>1</tupOffX>
      <tupOffY>24</tupOffY>
      <brackOffX>2</brackOffX>
      <brackOffY>3</brackOffY>
      <numStyle>number</numStyle>
      <posStyle>beamSide</posStyle>
      <allowHorz/>
      <ignoreGlOffs/>
      <breakBracket/>
      <matchHooks/>
      <noteBelow/>
      <brackStyle>bracket</brackStyle>
      <smartTuplet/>
      <leftHookLen>-12</leftHookLen>
      <leftHookExt>4</leftHookExt>
      <rightHookLen>-12</rightHookLen>
      <rightHookExt>5</rightHookExt>
      <slope>6</slope>
    </tupletDef>
  </details>
</finale>
)xml";

TEST(TupletDefTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);
    
    auto tupletDef = details->get<details::TupletDef>(SCORE_PARTID, 7, 0);
    ASSERT_TRUE(tupletDef) << "TupletDef with entnum 7 and inci 0 not found";

    EXPECT_EQ(tupletDef->displayNumber, 3);
    EXPECT_EQ(tupletDef->displayDuration, 512);
    EXPECT_EQ(tupletDef->inTheTimeOfNumber, 2);
    EXPECT_EQ(tupletDef->inTheTimeOfDuration, 512);
    EXPECT_TRUE(tupletDef->alwaysFlat);
    EXPECT_TRUE(tupletDef->fullDura);
    EXPECT_TRUE(tupletDef->metricCenter);
    EXPECT_TRUE(tupletDef->avoidStaff);
    EXPECT_EQ(tupletDef->autoBracketStyle, options::TupletOptions::AutoBracketStyle::NeverBeamSide);
    EXPECT_EQ(tupletDef->tupOffX, 1);
    EXPECT_EQ(tupletDef->tupOffY, 24);
    EXPECT_EQ(tupletDef->brackOffX, 2);
    EXPECT_EQ(tupletDef->brackOffY, 3);
    EXPECT_EQ(tupletDef->numStyle, options::TupletOptions::NumberStyle::Number);
    EXPECT_EQ(tupletDef->posStyle, options::TupletOptions::PositioningStyle::BeamSide);
    EXPECT_TRUE(tupletDef->allowHorz);
    EXPECT_TRUE(tupletDef->ignoreHorzNumOffset);
    EXPECT_TRUE(tupletDef->breakBracket);
    EXPECT_TRUE(tupletDef->matchHooks);
    EXPECT_TRUE(tupletDef->useBottomNote);
    EXPECT_EQ(tupletDef->brackStyle, options::TupletOptions::BracketStyle::Bracket);
    EXPECT_TRUE(tupletDef->smartTuplet);
    EXPECT_EQ(tupletDef->leftHookLen, -12);
    EXPECT_EQ(tupletDef->leftHookExt, 4);
    EXPECT_EQ(tupletDef->rightHookLen, -12);
    EXPECT_EQ(tupletDef->rightHookExt, 5);
    EXPECT_EQ(tupletDef->manualSlopeAdj, 6);
}

TEST(TupletDefTest, PartCheck)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);
    
    auto tupletDef = details->getArray<details::TupletDef>(1, 6);
    ASSERT_EQ(tupletDef.size(), 2) << "TupletDef array for part1 entnum 6 had " << std::to_string(tupletDef.size()) << " items";
    
    tupletDef = details->getArray<details::TupletDef>(1, 7);
    ASSERT_EQ(tupletDef.size(), 1) << "TupletDef array for part1 entnum 7 had " << std::to_string(tupletDef.size()) << " items";
}
