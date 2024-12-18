/*
 * Copyright (C) 2024, Robert Patterson
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

TEST(TieOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <tieOptions>
      <frontTieSepar>-8</frontTieSepar>
      <thicknessRight>8</thicknessRight>
      <thicknessLeft>8</thicknessLeft>
      <breakForTimeSigs/>
      <breakForKeySigs/>
      <breakTimeSigLeftHOffset>12</breakTimeSigLeftHOffset>
      <breakTimeSigRightHOffset>12</breakTimeSigRightHOffset>
      <breakKeySigLeftHOffset>12</breakKeySigLeftHOffset>
      <breakKeySigRightHOffset>12</breakKeySigRightHOffset>
      <sysBreakLeftHAdj>3</sysBreakLeftHAdj>
      <sysBreakRightHAdj>7</sysBreakRightHAdj>
      <secondsPlacement>both</secondsPlacement>
      <chordTieDirType>stemReversal</chordTieDirType>
      <chordTieDirOpposingSeconds/>
      <afterSingleDot/>
      <afterMultipleDots/>
      <beforeAcciSingleNote/>
      <specialPosMode>avoid</specialPosMode>
      <avoidStaffLinesDistance>8</avoidStaffLinesDistance>
      <insetStyle>percent</insetStyle>
      <useInterpolation/>
      <useTieEndCtlStyle/>
      <avoidStaffLinesOnly/>
      <tieTipWidth>2.500000e-01</tieTipWidth>
      <tieConnectStyle type="overStartPosInner">
        <offsetX>-4</offsetX>
        <offsetY>8</offsetY>
      </tieConnectStyle>
      <tieControlStyle type="shortSpan">
        <span>54</span>
        <cp1>
          <insetRatio>512</insetRatio>
          <height>12</height>
          <insetFixed>8</insetFixed>
        </cp1>
        <cp2>
          <insetRatio>512</insetRatio>
          <height>12</height>
          <insetFixed>8</insetFixed>
        </cp2>
      </tieControlStyle>
    </tieOptions>
  </options>
</finale>
)xml";

    using TieOptions = musx::dom::options::TieOptions;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto tieOptions = options->get<TieOptions>();
    ASSERT_TRUE(tieOptions);

    // Test all scalar fields
    EXPECT_EQ(tieOptions->frontTieSepar, -8);
    EXPECT_EQ(tieOptions->thicknessRight, 8);
    EXPECT_EQ(tieOptions->thicknessLeft, 8);
    EXPECT_TRUE(tieOptions->breakForTimeSigs);
    EXPECT_TRUE(tieOptions->breakForKeySigs);
    EXPECT_EQ(tieOptions->breakTimeSigLeftHOffset, 12);
    EXPECT_EQ(tieOptions->breakTimeSigRightHOffset, 12);
    EXPECT_EQ(tieOptions->breakKeySigLeftHOffset, 12);
    EXPECT_EQ(tieOptions->breakKeySigRightHOffset, 12);
    EXPECT_EQ(tieOptions->sysBreakLeftHAdj, 3);
    EXPECT_EQ(tieOptions->sysBreakRightHAdj, 7);
    EXPECT_EQ(tieOptions->secondsPlacement, TieOptions::SecondsPlacement::ShiftForSeconds);
    EXPECT_EQ(tieOptions->chordTieDirType, TieOptions::ChordTieDirType::StemReversal);
    EXPECT_TRUE(tieOptions->chordTieDirOpposingSeconds);
    EXPECT_TRUE(tieOptions->afterSingleDot);
    EXPECT_TRUE(tieOptions->afterMultipleDots);
    EXPECT_TRUE(tieOptions->beforeAcciSingleNote);
    EXPECT_EQ(tieOptions->specialPosMode, TieOptions::SpecialPosMode::Avoid);
    EXPECT_EQ(tieOptions->avoidStaffLinesDistance, 8);
    EXPECT_EQ(tieOptions->insetStyle, TieOptions::InsetStyle::Percent);
    EXPECT_TRUE(tieOptions->useInterpolation);
    EXPECT_TRUE(tieOptions->useTieEndCtlStyle);
    EXPECT_TRUE(tieOptions->avoidStaffLinesOnly);
    EXPECT_DOUBLE_EQ(tieOptions->tieTipWidth, 0.25);

    // Test tie connect styles
    ASSERT_TRUE(tieOptions->tieConnectStyles.count(TieOptions::ConnectStyleType::OverStartPosInner));
    const auto& connectStyle = tieOptions->tieConnectStyles.at(TieOptions::ConnectStyleType::OverStartPosInner);
    EXPECT_EQ(connectStyle->offsetX, -4);
    EXPECT_EQ(connectStyle->offsetY, 8);

    // Test tie control styles
    ASSERT_TRUE(tieOptions->tieControlStyles.count(TieOptions::ControlStyleType::ShortSpan));
    const auto& controlStyle = tieOptions->tieControlStyles.at(TieOptions::ControlStyleType::ShortSpan);
    EXPECT_EQ(controlStyle->span, 54);
    EXPECT_EQ(controlStyle->cp1->insetRatio, 512);
    EXPECT_EQ(controlStyle->cp1->height, 12);
    EXPECT_EQ(controlStyle->cp1->insetFixed, 8);
    EXPECT_EQ(controlStyle->cp2->insetRatio, 512);
    EXPECT_EQ(controlStyle->cp2->height, 12);
    EXPECT_EQ(controlStyle->cp2->insetFixed, 8);
}
