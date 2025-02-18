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

TEST(TupletOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <tupletOptions>
      <flat/>
      <fullDura/>
      <metricCenter/>
      <avoidStaff/>
      <autoBracketStyle>neverBeamSide</autoBracketStyle>
      <tupOffX>1</tupOffX>
      <tupOffY>2</tupOffY>
      <brackOffX>3</brackOffX>
      <brackOffY>4</brackOffY>
      <numStyle>number</numStyle>
      <posStyle>beamSide</posStyle>
      <allowHorz/>
      <ignoreGlOffs/>
      <breakBracket/>
      <matchHooks/>
      <noteBelow/>
      <brackStyle>bracket</brackStyle>
      <smartTuplet/>
      <leftHookLen>-14</leftHookLen>
      <leftHookExt>5</leftHookExt>
      <rightHookLen>-14</rightHookLen>
      <rightHookExt>6</rightHookExt>
      <slope>7</slope>
      <tupMaxSlope>100</tupMaxSlope>
      <tupLineWidth>128</tupLineWidth>
      <tupNUpstemOffset>12</tupNUpstemOffset>
      <tupNDownstemOffset>-12</tupNDownstemOffset>
    </tupletOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto tupletOptions = options->get<musx::dom::options::TupletOptions>();
    ASSERT_TRUE(tupletOptions);

    // Test all properties of TupletOptions
    EXPECT_TRUE(tupletOptions->alwaysFlat);
    EXPECT_TRUE(tupletOptions->fullDura);
    EXPECT_TRUE(tupletOptions->metricCenter);
    EXPECT_TRUE(tupletOptions->avoidStaff);
    EXPECT_EQ(tupletOptions->autoBracketStyle, musx::dom::options::TupletOptions::AutoBracketStyle::NeverBeamSide);
    EXPECT_EQ(tupletOptions->tupOffX, 1);
    EXPECT_EQ(tupletOptions->tupOffY, 2);
    EXPECT_EQ(tupletOptions->brackOffX, 3);
    EXPECT_EQ(tupletOptions->brackOffY, 4);
    EXPECT_EQ(tupletOptions->numStyle, musx::dom::options::TupletOptions::NumberStyle::Number);
    EXPECT_EQ(tupletOptions->posStyle, musx::dom::options::TupletOptions::PositioningStyle::BeamSide);
    EXPECT_TRUE(tupletOptions->allowHorz);
    EXPECT_TRUE(tupletOptions->ignoreHorzNumOffset);
    EXPECT_TRUE(tupletOptions->breakBracket);
    EXPECT_TRUE(tupletOptions->matchHooks);
    EXPECT_TRUE(tupletOptions->useBottomNote);
    EXPECT_EQ(tupletOptions->brackStyle, musx::dom::options::TupletOptions::BracketStyle::Bracket);
    EXPECT_TRUE(tupletOptions->smartTuplet);
    EXPECT_EQ(tupletOptions->leftHookLen, -14);
    EXPECT_EQ(tupletOptions->leftHookExt, 5);
    EXPECT_EQ(tupletOptions->rightHookLen, -14);
    EXPECT_EQ(tupletOptions->rightHookExt, 6);
    EXPECT_EQ(tupletOptions->manualSlopeAdj, 7);
    EXPECT_EQ(tupletOptions->tupMaxSlope, 100);
    EXPECT_EQ(tupletOptions->tupLineWidth, 128);
    EXPECT_EQ(tupletOptions->tupNUpstemOffset, 12);
    EXPECT_EQ(tupletOptions->tupNDownstemOffset, -12);
}
