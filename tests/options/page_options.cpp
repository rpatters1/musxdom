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

constexpr static musxtest::string_view pageOptionsXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <pageFormatOptions>
      <pageFormatScore>
        <pageHeight>1584</pageHeight>
        <pageWidth>2448</pageWidth>
        <pagePercent>100</pagePercent>
        <sysPercent>70</sysPercent>
        <rawStaffHeight>1536</rawStaffHeight>
        <leftPageMarginTop>-288</leftPageMarginTop>
        <leftPageMarginLeft>288</leftPageMarginLeft>
        <leftPageMarginBottom>288</leftPageMarginBottom>
        <leftPageMarginRight>-288</leftPageMarginRight>
        <rightPageMarginTop>-144</rightPageMarginTop>
        <rightPageMarginLeft>144</rightPageMarginLeft>
        <rightPageMarginBottom>144</rightPageMarginBottom>
        <rightPageMarginRight>-144</rightPageMarginRight>
        <sysMarginTop>-1</sysMarginTop>
        <sysMarginLeft>24</sysMarginLeft>
        <sysMarginBottom>-99</sysMarginBottom>
        <sysMarginRight>-2</sysMarginRight>
        <sysDistanceBetween>-216</sysDistanceBetween>
        <facingPages/>
        <differentFirstSysMargin/>
        <differentFirstPageMargin/>
        <firstPageMarginTop>-100</firstPageMarginTop>
        <firstSysMarginTop>-4</firstSysMarginTop>
        <firstSysMarginLeft>576</firstSysMarginLeft>
        <firstSysMarginDistance>-490</firstSysMarginDistance>
      </pageFormatScore>
      <pageFormatParts>
        <pageHeight>3744</pageHeight>
        <pageWidth>2880</pageWidth>
        <pagePercent>100</pagePercent>
        <sysPercent>85</sysPercent>
        <rawStaffHeight>1536</rawStaffHeight>
        <leftPageMarginTop>-288</leftPageMarginTop>
        <leftPageMarginLeft>288</leftPageMarginLeft>
        <leftPageMarginBottom>288</leftPageMarginBottom>
        <leftPageMarginRight>-288</leftPageMarginRight>
        <rightPageMarginTop>-288</rightPageMarginTop>
        <rightPageMarginLeft>288</rightPageMarginLeft>
        <rightPageMarginBottom>288</rightPageMarginBottom>
        <rightPageMarginRight>-288</rightPageMarginRight>
        <sysMarginBottom>-96</sysMarginBottom>
        <sysDistanceBetween>-162</sysDistanceBetween>
        <differentFirstSysMargin/>
        <firstPageMarginTop>-144</firstPageMarginTop>
        <firstSysMarginLeft>144</firstSysMarginLeft>
        <firstSysMarginDistance>-432</firstSysMarginDistance>
      </pageFormatParts>
      <avoidSystemMarginCollisions/>
      <adjustPageScope>range</adjustPageScope>
    </pageFormatOptions>
  </options>
</finale>
)xml";

TEST(PageFormatOptionsTest, PropertiesTest)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(pageOptionsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto pageFormatOptions = options->get<options::PageFormatOptions>();
    ASSERT_TRUE(pageFormatOptions);

    // Test adjustPageScope
    EXPECT_EQ(pageFormatOptions->adjustPageScope, options::PageFormatOptions::AdjustPageScope::PageRange);

    // Test pageFormatScore
    auto pageFormatScore = pageFormatOptions->pageFormatScore;
    ASSERT_TRUE(pageFormatScore);
    EXPECT_EQ(pageFormatScore->pageHeight, 1584);
    EXPECT_EQ(pageFormatScore->pageWidth, 2448);
    EXPECT_EQ(pageFormatScore->pagePercent, 100);
    EXPECT_EQ(pageFormatScore->sysPercent, 70);
    EXPECT_EQ(pageFormatScore->rawStaffHeight, 1536);
    EXPECT_EQ(pageFormatScore->leftPageMarginTop, -288);
    EXPECT_EQ(pageFormatScore->leftPageMarginLeft, 288);
    EXPECT_EQ(pageFormatScore->leftPageMarginBottom, 288);
    EXPECT_EQ(pageFormatScore->leftPageMarginRight, -288);
    EXPECT_EQ(pageFormatScore->rightPageMarginTop, -144);
    EXPECT_EQ(pageFormatScore->rightPageMarginLeft, 144);
    EXPECT_EQ(pageFormatScore->rightPageMarginBottom, 144);
    EXPECT_EQ(pageFormatScore->rightPageMarginRight, -144);
    EXPECT_EQ(pageFormatScore->sysMarginTop, -1);
    EXPECT_EQ(pageFormatScore->sysMarginLeft, 24);
    EXPECT_EQ(pageFormatScore->sysMarginBottom, -99);
    EXPECT_EQ(pageFormatScore->sysMarginRight, -2);
    EXPECT_EQ(pageFormatScore->sysDistanceBetween, -216);
    EXPECT_EQ(pageFormatScore->firstPageMarginTop, -100);
    EXPECT_EQ(pageFormatScore->firstSysMarginTop, -4);
    EXPECT_EQ(pageFormatScore->firstSysMarginLeft, 576);
    EXPECT_EQ(pageFormatScore->firstSysMarginDistance, -490);
    EXPECT_TRUE(pageFormatScore->facingPages);
    EXPECT_TRUE(pageFormatScore->differentFirstSysMargin);
    EXPECT_TRUE(pageFormatScore->differentFirstPageMargin);

    // Test pageFormatParts
    auto pageFormatParts = pageFormatOptions->pageFormatParts;
    ASSERT_TRUE(pageFormatParts);
    EXPECT_EQ(pageFormatParts->pageHeight, 3744);
    EXPECT_EQ(pageFormatParts->pageWidth, 2880);
    EXPECT_EQ(pageFormatParts->pagePercent, 100);
    EXPECT_EQ(pageFormatParts->sysPercent, 85);
    EXPECT_EQ(pageFormatParts->rawStaffHeight, 1536);
    EXPECT_EQ(pageFormatParts->leftPageMarginTop, -288);
    EXPECT_EQ(pageFormatParts->leftPageMarginLeft, 288);
    EXPECT_EQ(pageFormatParts->leftPageMarginBottom, 288);
    EXPECT_EQ(pageFormatParts->leftPageMarginRight, -288);
    EXPECT_EQ(pageFormatParts->rightPageMarginTop, -288);
    EXPECT_EQ(pageFormatParts->rightPageMarginLeft, 288);
    EXPECT_EQ(pageFormatParts->rightPageMarginBottom, 288);
    EXPECT_EQ(pageFormatParts->rightPageMarginRight, -288);
    EXPECT_EQ(pageFormatParts->sysMarginTop, 0); // Not present, default to 0
    EXPECT_EQ(pageFormatParts->sysMarginLeft, 0); // Not present, default to 0
    EXPECT_EQ(pageFormatParts->sysMarginBottom, -96);
    EXPECT_EQ(pageFormatParts->sysMarginRight, 0); // Not present, default to 0
    EXPECT_EQ(pageFormatParts->sysDistanceBetween, -162);
    EXPECT_EQ(pageFormatParts->firstPageMarginTop, -144);
    EXPECT_EQ(pageFormatParts->firstSysMarginTop, 0); // Not present, default to 0
    EXPECT_EQ(pageFormatParts->firstSysMarginLeft, 144);
    EXPECT_EQ(pageFormatParts->firstSysMarginDistance, -432);
    EXPECT_FALSE(pageFormatParts->facingPages); // Not present, default to false
    EXPECT_TRUE(pageFormatParts->differentFirstSysMargin);
    EXPECT_FALSE(pageFormatParts->differentFirstPageMargin); // Not present, default to false

    // Test avoidSystemMarginCollisions
    EXPECT_TRUE(pageFormatOptions->avoidSystemMarginCollisions);
}

constexpr static musxtest::string_view noPageScopexml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <pageFormatOptions/>
  </options>
</finale>
)xml";

TEST(PageFormatOptionsTest, EnumDefaultsTest)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(noPageScopexml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto pageFormatOptions = options->get<options::PageFormatOptions>();
    ASSERT_TRUE(pageFormatOptions);

    // Test adjustPageScope takes correct default
    EXPECT_EQ(pageFormatOptions->adjustPageScope, options::PageFormatOptions::AdjustPageScope::Current);
}

constexpr static musxtest::string_view pageDiffThanOptsXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <pageFormatOptions>
      <pageFormatScore>
        <pageHeight>3744</pageHeight>
        <pageWidth>2880</pageWidth>
        <pagePercent>100</pagePercent>
        <sysPercent>70</sysPercent>
        <rawStaffHeight>1536</rawStaffHeight>
        <leftPageMarginTop>-288</leftPageMarginTop>
        <leftPageMarginLeft>288</leftPageMarginLeft>
        <leftPageMarginBottom>288</leftPageMarginBottom>
        <leftPageMarginRight>-288</leftPageMarginRight>
        <rightPageMarginTop>-144</rightPageMarginTop>
        <rightPageMarginLeft>144</rightPageMarginLeft>
        <rightPageMarginBottom>144</rightPageMarginBottom>
        <rightPageMarginRight>-144</rightPageMarginRight>
        <sysMarginLeft>24</sysMarginLeft>
        <sysMarginBottom>-96</sysMarginBottom>
        <sysDistanceBetween>-216</sysDistanceBetween>
        <differentFirstSysMargin/>
        <firstPageMarginTop>-100</firstPageMarginTop>
        <firstSysMarginLeft>576</firstSysMarginLeft>
        <firstSysMarginDistance>-490</firstSysMarginDistance>
      </pageFormatScore>
      <pageFormatParts>
        <pageHeight>3744</pageHeight>
        <pageWidth>2880</pageWidth>
        <pagePercent>100</pagePercent>
        <sysPercent>85</sysPercent>
        <rawStaffHeight>1536</rawStaffHeight>
        <leftPageMarginTop>-288</leftPageMarginTop>
        <leftPageMarginLeft>288</leftPageMarginLeft>
        <leftPageMarginBottom>288</leftPageMarginBottom>
        <leftPageMarginRight>-288</leftPageMarginRight>
        <rightPageMarginTop>-288</rightPageMarginTop>
        <rightPageMarginLeft>288</rightPageMarginLeft>
        <rightPageMarginBottom>288</rightPageMarginBottom>
        <rightPageMarginRight>-288</rightPageMarginRight>
        <sysMarginBottom>-96</sysMarginBottom>
        <sysDistanceBetween>-162</sysDistanceBetween>
        <differentFirstSysMargin/>
        <firstPageMarginTop>-144</firstPageMarginTop>
        <firstSysMarginLeft>144</firstSysMarginLeft>
        <firstSysMarginDistance>-432</firstSysMarginDistance>
      </pageFormatParts>
      <avoidSystemMarginCollisions/>
      <adjustPageScope>leftOrRight</adjustPageScope>
    </pageFormatOptions>
  </options>
  <others>
    <pageSpec cmper="1">
      <height>3168</height>
      <width>2448</width>
      <percent>100</percent>
      <firstSystem>1</firstSystem>
      <scaleContentOnly/>
      <margTop>-512</margTop>
      <margLeft>288</margLeft>
      <margBottom>288</margBottom>
      <margRight>-216</margRight>
    </pageSpec>
    <pageSpec cmper="2">
      <height>3168</height>
      <width>2448</width>
      <percent>100</percent>
      <firstSystem>5</firstSystem>
      <scaleContentOnly/>
      <margTop>-288</margTop>
      <margLeft>216</margLeft>
      <margBottom>288</margBottom>
      <margRight>-288</margRight>
    </pageSpec>
    <pageSpec cmper="3">
      <height>3168</height>
      <width>2448</width>
      <percent>100</percent>
      <firstSystem>11</firstSystem>
      <scaleContentOnly/>
      <margTop>-288</margTop>
      <margLeft>288</margLeft>
      <margBottom>288</margBottom>
      <margRight>-216</margRight>
    </pageSpec>
    <staffSystemSpec cmper="1">
      <startMeas>1</startMeas>
      <endMeas>3</endMeas>
      <horzPercent>12142</horzPercent>
      <ssysPercent>90</ssysPercent>
      <staffHeight>5760</staffHeight>
      <top>-490</top>
      <left>576</left>
      <bottom>-96</bottom>
      <scaleVert/>
      <scaleContentOnly/>
    </staffSystemSpec>
    <staffSystemSpec cmper="2">
      <startMeas>3</startMeas>
      <endMeas>6</endMeas>
      <horzPercent>12072</horzPercent>
      <ssysPercent>90</ssysPercent>
      <staffHeight>5760</staffHeight>
      <left>24</left>
      <bottom>-96</bottom>
      <scaleVert/>
      <scaleContentOnly/>
      <distanceToPrev>-216</distanceToPrev>
    </staffSystemSpec>
  </others>
</finale>
)xml";

TEST(PageFormatOptionsTest, CalcPageFormat)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(pageDiffThanOptsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto pageFormatOptions = options->get<options::PageFormatOptions>();
    ASSERT_TRUE(pageFormatOptions);
    auto actualFormat = pageFormatOptions->calcPageFormatForPart(SCORE_PARTID);
    ASSERT_TRUE(actualFormat);

    EXPECT_EQ(actualFormat->pageHeight, 3168);
    EXPECT_EQ(actualFormat->pageWidth, 2448);
    EXPECT_EQ(actualFormat->pagePercent, 100);
    EXPECT_EQ(actualFormat->sysPercent, 90);
    EXPECT_EQ(actualFormat->rawStaffHeight, 1440);
    EXPECT_EQ(actualFormat->leftPageMarginTop, -288);
    EXPECT_EQ(actualFormat->leftPageMarginLeft, 216);
    EXPECT_EQ(actualFormat->leftPageMarginBottom, 288);
    EXPECT_EQ(actualFormat->leftPageMarginRight, -288);
    EXPECT_EQ(actualFormat->rightPageMarginTop, -288);
    EXPECT_EQ(actualFormat->rightPageMarginLeft, 288);
    EXPECT_EQ(actualFormat->rightPageMarginBottom, 288);
    EXPECT_EQ(actualFormat->rightPageMarginRight, -216);
    EXPECT_EQ(actualFormat->sysMarginTop, 0);
    EXPECT_EQ(actualFormat->sysMarginLeft, 24);
    EXPECT_EQ(actualFormat->sysMarginBottom, -96);
    EXPECT_EQ(actualFormat->sysMarginRight, 0);
    EXPECT_EQ(actualFormat->sysDistanceBetween, -216);
    EXPECT_EQ(actualFormat->firstPageMarginTop, -512);
    EXPECT_EQ(actualFormat->firstSysMarginTop, -490);
    EXPECT_EQ(actualFormat->firstSysMarginLeft, 576);
    EXPECT_EQ(actualFormat->firstSysMarginDistance, -490);
    EXPECT_TRUE(actualFormat->facingPages);
    EXPECT_TRUE(actualFormat->differentFirstSysMargin);
    EXPECT_TRUE(actualFormat->differentFirstPageMargin);
}