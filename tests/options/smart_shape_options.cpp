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

constexpr static musxtest::string_view smartShapeOptsXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <smartShapeOptions>
      <shortHairpinOpeningWidth>24</shortHairpinOpeningWidth>
      <crescHeight>24</crescHeight>
      <maximumShortHairpinLength>250</maximumShortHairpinLength>
      <crescLineWidth>118</crescLineWidth>
      <hookLength>24</hookLength>
      <smartLineWidth>118</smartLineWidth>
      <showOctavaAsText/>
      <smartDashOn>18</smartDashOn>
      <smartDashOff>18</smartDashOff>
      <crescHorizontal/>
      <direction>under</direction>
      <slurThicknessCp1X>2</slurThicknessCp1X>
      <slurThicknessCp1Y>8</slurThicknessCp1Y>
      <slurThicknessCp2X>3</slurThicknessCp2X>
      <slurThicknessCp2Y>8</slurThicknessCp2Y>
      <slurAvoidAccidentals/>
      <slurAvoidStaffLinesAmt>8</slurAvoidStaffLinesAmt>
      <maxSlurStretch>1536</maxSlurStretch>
      <maxSlurLift>1536</maxSlurLift>
      <slurSymmetry>6000</slurSymmetry>
      <useEngraverSlurs/>
      <slurLeftBreakHorzAdj>1</slurLeftBreakHorzAdj>
      <slurRightBreakHorzAdj>-4</slurRightBreakHorzAdj>
      <slurBreakVertAdj>12</slurBreakVertAdj>
      <slurAvoidStaffLines/>
      <slurPadding>12</slurPadding>
      <maxSlurAngle>4500</maxSlurAngle>
      <slurAcciPadding>12</slurAcciPadding>
      <slurDoStretchFirst/>
      <slurStretchByPercent/>
      <maxSlurStretchPercent>1500</maxSlurStretchPercent>
      <articAvoidSlurAmt>8</articAvoidSlurAmt>
      <ssLineStyleCmpCustom>52</ssLineStyleCmpCustom>
      <ssLineStyleCmpGlissando>53</ssLineStyleCmpGlissando>
      <ssLineStyleCmpTabSlide>54</ssLineStyleCmpTabSlide>
      <ssLineStyleCmpTabBendCurve>55</ssLineStyleCmpTabBendCurve>
      <smartSlurTipWidth>2.500000e-01</smartSlurTipWidth>
      <guitarBendUseParens/>
      <guitarBendHideBendTo/>
      <guitarBendGenText/>
      <guitarBendUseFull/>
      <slurConnectStyle type="overNoteStart">
        <connectIndex>headRightTop</connectIndex>
        <yOffset>10</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overNoteEnd">
        <connectIndex>headRightTop</connectIndex>
        <yOffset>10</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overStemStart">
        <connectIndex>stemRightTop</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overStemEnd">
        <connectIndex>stemLeftTop</connectIndex>
        <xOffset>-12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underNoteStart">
        <connectIndex>headLeftBottom</connectIndex>
        <yOffset>-10</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underNoteEnd">
        <connectIndex>headLeftBottom</connectIndex>
        <yOffset>-10</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underStemStart">
        <connectIndex>stemRightBottom</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underStemEnd">
        <connectIndex>stemLeftBottom</connectIndex>
        <xOffset>-12</xOffset>
        <yOffset>48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overMixStemStart">
        <connectIndex>stemRightTop</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overMixStemEnd">
        <connectIndex>stemLeftTop</connectIndex>
        <xOffset>-12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overStemGrace">
        <connectIndex>stemRightTop</connectIndex>
        <xOffset>6</xOffset>
        <yOffset>4</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overStemPrincipal">
        <connectIndex>stemLeftTop</connectIndex>
        <xOffset>-12</xOffset>
        <yOffset>-24</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underStemGrace">
        <connectIndex>stemRightBottom</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>-4</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underStemPrincipal">
        <connectIndex>stemLeftBottom</connectIndex>
        <xOffset>-6</xOffset>
        <yOffset>24</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underNoteGrace">
        <connectIndex>headLeftBottom</connectIndex>
        <yOffset>-3</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underStemNotePrincipal">
        <connectIndex>headRightBottom</connectIndex>
        <xOffset>-18</xOffset>
        <yOffset>-3</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overNoteGrace">
        <connectIndex>headRightTop</connectIndex>
        <yOffset>3</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overStemNotePrincipal">
        <yOffset>9</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overBeamStart">
        <connectIndex>stemRightTop</connectIndex>
        <yOffset>6</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overBeamEnd">
        <connectIndex>stemLeftTop</connectIndex>
        <yOffset>6</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underBeamStart">
        <connectIndex>stemRightBottom</connectIndex>
        <yOffset>-6</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underBeamEnd">
        <connectIndex>stemLeftBottom</connectIndex>
        <yOffset>-6</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overMixFlagStart">
        <connectIndex>stemRightTop</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overFlagStart">
        <connectIndex>stemRightTop</connectIndex>
        <xOffset>12</xOffset>
        <yOffset>-48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underFlagStart">
        <connectIndex>stemRightBottom</connectIndex>
        <xOffset>18</xOffset>
        <yOffset>48</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overTabNumStart">
        <connectIndex>headRightTop</connectIndex>
        <yOffset>8</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="overTabNumEnd">
        <connectIndex>headRightTop</connectIndex>
        <yOffset>8</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underTabNumStart">
        <connectIndex>headLeftBottom</connectIndex>
        <yOffset>-8</yOffset>
      </slurConnectStyle>
      <slurConnectStyle type="underTabNumEnd">
        <connectIndex>headLeftBottom</connectIndex>
        <yOffset>-8</yOffset>
      </slurConnectStyle>
      <slurControlStyle type="shortSpan">
        <span>36</span>
        <inset>512</inset>
        <height>16</height>
      </slurControlStyle>
      <slurControlStyle type="mediumSpan">
        <span>288</span>
        <inset>512</inset>
        <height>48</height>
      </slurControlStyle>
      <slurControlStyle type="longSpan">
        <span>864</span>
        <inset>341</inset>
        <height>72</height>
      </slurControlStyle>
      <slurControlStyle type="extraLongSpan">
        <span>1152</span>
        <inset>341</inset>
        <height>72</height>
      </slurControlStyle>
      <tabSlideConnectStyle type="diffLevelPitchUpLineStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchUpLineEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchUpSpaceStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchUpSpaceEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchDownLineStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchDownLineEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchDownSpaceStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="diffLevelPitchDownSpaceEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchUpLineStart">
        <connectIndex>noteRightBottom</connectIndex>
        <xOffset>6</xOffset>
        <yOffset>1</yOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchUpLineEnd">
        <connectIndex>noteLeftTop</connectIndex>
        <xOffset>-6</xOffset>
        <yOffset>2</yOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchUpSpaceStart">
        <connectIndex>noteRightBottom</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchUpSpaceEnd">
        <connectIndex>noteLeftTop</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchDownLineStart">
        <connectIndex>noteRightTop</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchDownLineEnd">
        <connectIndex>noteLeftBottom</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchDownSpaceStart">
        <connectIndex>noteRightTop</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchDownSpaceEnd">
        <connectIndex>noteLeftBottom</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchSameStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>6</xOffset>
      </tabSlideConnectStyle>
      <tabSlideConnectStyle type="sameLevelPitchSameEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>-6</xOffset>
      </tabSlideConnectStyle>
      <glissandoConnectStyle type="defaultStart">
        <connectIndex>noteRightCenter</connectIndex>
        <xOffset>8</xOffset>
        <yOffset>-1</yOffset>
      </glissandoConnectStyle>
      <glissandoConnectStyle type="defaultEnd">
        <connectIndex>noteLeftCenter</connectIndex>
        <xOffset>-8</xOffset>
        <yOffset>-2</yOffset>
      </glissandoConnectStyle>
      <bendCurveConnectStyle type="noteStart">
        <connectIndex>noteRightTop</connectIndex>
        <xOffset>8</xOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffEnd">
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffStart">
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="noteEnd">
        <yOffset>4</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffToTopLineStart">
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffFromTopLineEnd">
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffEndOffset">
        <connectIndex>noteRightTop</connectIndex>
        <xOffset>36</xOffset>
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
      <bendCurveConnectStyle type="staffFromTopEndOffset">
        <connectIndex>noteRightTop</connectIndex>
        <xOffset>36</xOffset>
        <yOffset>40</yOffset>
      </bendCurveConnectStyle>
    </smartShapeOptions>
  </options>
</finale>
)xml";

TEST(SmartShapeOptionsTest, PropertiesTest)
{
    using SmartShapeOptions = musx::dom::options::SmartShapeOptions;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(smartShapeOptsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto smartShapeOptions = options->get<SmartShapeOptions>();
    ASSERT_TRUE(smartShapeOptions);

    // Verify properties
    EXPECT_EQ(smartShapeOptions->shortHairpinOpeningWidth, 24);
    EXPECT_EQ(smartShapeOptions->crescHeight, 24);
    EXPECT_EQ(smartShapeOptions->maximumShortHairpinLength, 250);
    EXPECT_EQ(smartShapeOptions->crescLineWidth, 118);
    EXPECT_EQ(smartShapeOptions->hookLength, 24);
    EXPECT_EQ(smartShapeOptions->smartLineWidth, 118);
    EXPECT_TRUE(smartShapeOptions->showOctavaAsText);
    EXPECT_EQ(smartShapeOptions->smartDashOn, 18);
    EXPECT_EQ(smartShapeOptions->smartDashOff, 18);
    EXPECT_TRUE(smartShapeOptions->crescHorizontal);
    EXPECT_EQ(smartShapeOptions->direction, SmartShapeOptions::DefaultDirection::Under);
    EXPECT_EQ(smartShapeOptions->slurThicknessCp1X, 2);
    EXPECT_EQ(smartShapeOptions->slurThicknessCp1Y, 8);
    EXPECT_EQ(smartShapeOptions->slurThicknessCp2X, 3);
    EXPECT_EQ(smartShapeOptions->slurThicknessCp2Y, 8);
    EXPECT_TRUE(smartShapeOptions->slurAvoidAccidentals);
    EXPECT_EQ(smartShapeOptions->slurAvoidStaffLinesAmt, 8);
    EXPECT_EQ(smartShapeOptions->maxSlurStretch, 1536);
    EXPECT_EQ(smartShapeOptions->maxSlurLift, 1536);
    EXPECT_EQ(smartShapeOptions->slurSymmetry, 6000);
    EXPECT_TRUE(smartShapeOptions->useEngraverSlurs);
    EXPECT_EQ(smartShapeOptions->slurLeftBreakHorzAdj, 1);
    EXPECT_EQ(smartShapeOptions->slurRightBreakHorzAdj, -4);
    EXPECT_EQ(smartShapeOptions->slurBreakVertAdj, 12);
    EXPECT_TRUE(smartShapeOptions->slurAvoidStaffLines);
    EXPECT_EQ(smartShapeOptions->slurPadding, 12);
    EXPECT_EQ(smartShapeOptions->maxSlurAngle, 4500);
    EXPECT_EQ(smartShapeOptions->slurAcciPadding, 12);
    EXPECT_TRUE(smartShapeOptions->slurDoStretchFirst);
    EXPECT_TRUE(smartShapeOptions->slurStretchByPercent);
    EXPECT_EQ(smartShapeOptions->maxSlurStretchPercent, 1500);
    EXPECT_EQ(smartShapeOptions->articAvoidSlurAmt, 8);
    EXPECT_EQ(smartShapeOptions->ssLineStyleCmpCustom, 52);
    EXPECT_EQ(smartShapeOptions->ssLineStyleCmpGlissando, 53);
    EXPECT_EQ(smartShapeOptions->ssLineStyleCmpTabSlide, 54);
    EXPECT_EQ(smartShapeOptions->ssLineStyleCmpTabBendCurve, 55);
    EXPECT_DOUBLE_EQ(smartShapeOptions->smartSlurTipWidth, 0.25);
    EXPECT_TRUE(smartShapeOptions->guitarBendUseParens);
    EXPECT_TRUE(smartShapeOptions->guitarBendHideBendTo);
    EXPECT_TRUE(smartShapeOptions->guitarBendGenText);
    EXPECT_TRUE(smartShapeOptions->guitarBendUseFull);
}

TEST(SmartShapeOptions, ArrayTests)
{
    using SmartShapeOptions = musx::dom::options::SmartShapeOptions;
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(smartShapeOptsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto smartShapeOptions = options->get<SmartShapeOptions>();
    ASSERT_TRUE(smartShapeOptions);
        // Verify slurConnectStyles
    ASSERT_EQ(smartShapeOptions->slurConnectStyles.size(), 29);

    auto overNoteStart = smartShapeOptions->slurConnectStyles.at(SmartShapeOptions::SlurConnectStyleType::OverNoteStart);
    ASSERT_TRUE(overNoteStart);
    EXPECT_EQ(overNoteStart->connectIndex, SmartShapeOptions::ConnectionIndex::HeadRightTop);
    EXPECT_EQ(overNoteStart->yOffset, 10);

    auto underStemStart = smartShapeOptions->slurConnectStyles.at(SmartShapeOptions::SlurConnectStyleType::UnderStemStart);
    ASSERT_TRUE(underStemStart);
    EXPECT_EQ(underStemStart->connectIndex, SmartShapeOptions::ConnectionIndex::StemRightBottom);
    EXPECT_EQ(underStemStart->xOffset, 12);
    EXPECT_EQ(underStemStart->yOffset, 48);

    // Verify slurControlStyles
    ASSERT_EQ(smartShapeOptions->slurControlStyles.size(), 4);

    auto shortSpan = smartShapeOptions->slurControlStyles.at(SmartShapeOptions::SlurControlStyleType::ShortSpan);
    ASSERT_TRUE(shortSpan);
    EXPECT_EQ(shortSpan->span, 36);
    EXPECT_EQ(shortSpan->inset, 512);
    EXPECT_EQ(shortSpan->height, 16);

    auto longSpan = smartShapeOptions->slurControlStyles.at(SmartShapeOptions::SlurControlStyleType::LongSpan);
    ASSERT_TRUE(longSpan);
    EXPECT_EQ(longSpan->span, 864);
    EXPECT_EQ(longSpan->inset, 341);
    EXPECT_EQ(longSpan->height, 72);

    // Verify tabSlideConnectStyles
    ASSERT_EQ(smartShapeOptions->tabSlideConnectStyles.size(), 18);

    auto diffLevelPitchUpLineStart = smartShapeOptions->tabSlideConnectStyles.at(SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchUpLineStart);
    ASSERT_TRUE(diffLevelPitchUpLineStart);
    EXPECT_EQ(diffLevelPitchUpLineStart->connectIndex, SmartShapeOptions::ConnectionIndex::NoteRightCenter);
    EXPECT_EQ(diffLevelPitchUpLineStart->xOffset, -6);

    auto sameLevelPitchUpLineEnd = smartShapeOptions->tabSlideConnectStyles.at(SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchUpLineEnd);
    ASSERT_TRUE(sameLevelPitchUpLineEnd);
    EXPECT_EQ(sameLevelPitchUpLineEnd->connectIndex, SmartShapeOptions::ConnectionIndex::NoteLeftTop);
    EXPECT_EQ(sameLevelPitchUpLineEnd->xOffset, -6);
    EXPECT_EQ(sameLevelPitchUpLineEnd->yOffset, 2);

    // Verify glissandoConnectStyles
    ASSERT_EQ(smartShapeOptions->glissandoConnectStyles.size(), 2);

    auto glissDefaultStart = smartShapeOptions->glissandoConnectStyles.at(SmartShapeOptions::GlissandoConnectStyleType::DefaultStart);
    ASSERT_TRUE(glissDefaultStart);
    EXPECT_EQ(glissDefaultStart->connectIndex, SmartShapeOptions::ConnectionIndex::NoteRightCenter);
    EXPECT_EQ(glissDefaultStart->xOffset, 8);
    EXPECT_EQ(glissDefaultStart->yOffset, -1);

    auto glissDefaultEnd = smartShapeOptions->glissandoConnectStyles.at(SmartShapeOptions::GlissandoConnectStyleType::DefaultEnd);
    ASSERT_TRUE(glissDefaultEnd);
    EXPECT_EQ(glissDefaultEnd->connectIndex, SmartShapeOptions::ConnectionIndex::NoteLeftCenter);
    EXPECT_EQ(glissDefaultEnd->xOffset, -8);
    EXPECT_EQ(glissDefaultEnd->yOffset, -2);

    // Verify bendCurveConnectStyles
    ASSERT_EQ(smartShapeOptions->bendCurveConnectStyles.size(), 8);

    auto bendNoteStart = smartShapeOptions->bendCurveConnectStyles.at(SmartShapeOptions::BendCurveConnectStyleType::NoteStart);
    ASSERT_TRUE(bendNoteStart);
    EXPECT_EQ(bendNoteStart->connectIndex, SmartShapeOptions::ConnectionIndex::NoteRightTop);
    EXPECT_EQ(bendNoteStart->xOffset, 8);

    auto bendStaffEndOffset = smartShapeOptions->bendCurveConnectStyles.at(SmartShapeOptions::BendCurveConnectStyleType::StaffEndOffset);
    ASSERT_TRUE(bendStaffEndOffset);
    EXPECT_EQ(bendStaffEndOffset->connectIndex, SmartShapeOptions::ConnectionIndex::NoteRightTop);
    EXPECT_EQ(bendStaffEndOffset->xOffset, 36);
    EXPECT_EQ(bendStaffEndOffset->yOffset, 40);
}