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

TEST(MeasureNumberRegionTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <measNumbRegion cmper="1">
      <scoreData>
        <startFont>
          <fontID>1</fontID>
          <fontSize>10</fontSize>
          <efx>
            <italic/>
            <absolute/>
          </efx>
        </startFont>
        <multipleFont>
          <fontID>2</fontID>
          <fontSize>12</fontSize>
        </multipleFont>
        <mmRestFont>
          <fontID>3</fontID>
          <fontSize>14</fontSize>
        </mmRestFont>
        <startEnclosure>
          <xMargin>9</xMargin>
          <yMargin>9</yMargin>
          <lineWidth>256</lineWidth>
          <sides>1</sides>
          <notTall/>
          <roundCorners/>
          <cornerRadius>768</cornerRadius>
        </startEnclosure>
        <multipleEnclosure>
          <xMargin>44</xMargin>
          <yMargin>44</yMargin>
          <lineWidth>256</lineWidth>
          <sides>2</sides>
          <notTall/>
          <roundCorners/>
          <cornerRadius>768</cornerRadius>
        </multipleEnclosure>
        <startXdisp>1</startXdisp>
        <startYdisp>-144</startYdisp>
        <multipleXdisp>3</multipleXdisp>
        <multipleYdisp>-144</multipleYdisp>
        <mmRestXdisp>5</mmRestXdisp>
        <mmRestYdisp>-144</mmRestYdisp>
        <leftMmBracketChar>91</leftMmBracketChar>
        <rightMmBracketChar>93</rightMmBracketChar>
        <startWith>2</startWith>
        <incidence>1</incidence>
        <multipleAlign>center</multipleAlign>
        <mmRestAlign>center</mmRestAlign>
        <startOfLine/>
        <multipleOf/>
        <exceptFirstMeas/>
        <mmRestRange/>
        <mmRestRangeForce/>
        <useStartEncl/>
        <useMultipleEncl/>
        <showOnTop/>
        <showOnBottom/>
        <excludeOthers/>
        <breakMmRest/>
        <multipleJustify>center</multipleJustify>
        <mmRestJustify>center</mmRestJustify>
      </scoreData>
      <startMeas>1</startMeas>
      <endMeas>1000</endMeas>
      <startChar>164</startChar>
      <base>10</base>
      <offset>2</offset>
      <prefix>&lt;|</prefix>
      <suffix>&gt;|</suffix>
      <countFromOne/>
      <noZero/>
      <doubleUp/>
      <includeHours/>
      <smpteFrames/>
      <useScoreInfoForPart/>
      <region>1</region>
      <timePrecision>thousandths</timePrecision>
      <hideScroll/>
    </measNumbRegion>
  </others>
</finale>
)xml";

    using MeasureNumberRegion = musx::dom::others::MeasureNumberRegion;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto measureNumberRegion = others->get<MeasureNumberRegion>(SCORE_PARTID, 1);
    ASSERT_TRUE(measureNumberRegion);

    // Test basic region fields
    EXPECT_EQ(measureNumberRegion->startMeas, 1);
    EXPECT_EQ(measureNumberRegion->endMeas, 1000);
    EXPECT_EQ(measureNumberRegion->base, 10);
    EXPECT_EQ(measureNumberRegion->numberOffset, 2);
    EXPECT_EQ(measureNumberRegion->prefix, "<|");
    EXPECT_EQ(measureNumberRegion->suffix, ">|");
    EXPECT_TRUE(measureNumberRegion->countFromOne);
    EXPECT_TRUE(measureNumberRegion->noZero);
    EXPECT_TRUE(measureNumberRegion->doubleUp);
    EXPECT_TRUE(measureNumberRegion->includeHours);
    EXPECT_TRUE(measureNumberRegion->smpteFrames);
    EXPECT_TRUE(measureNumberRegion->useScoreInfoForPart);
    EXPECT_EQ(measureNumberRegion->region, 1);
    EXPECT_EQ(measureNumberRegion->timePrecision, MeasureNumberRegion::TimePrecision::Thousandths);
    EXPECT_TRUE(measureNumberRegion->hideScroll);

    // Test ScorePartData fields
    EXPECT_FALSE(measureNumberRegion->partData);
    const auto& scoreData = measureNumberRegion->scoreData;
    ASSERT_TRUE(scoreData);

    // Fonts
    ASSERT_TRUE(scoreData->startFont);
    EXPECT_EQ(scoreData->startFont->fontId, 1);
    EXPECT_EQ(scoreData->startFont->fontSize, 10);
    ASSERT_FALSE(scoreData->startFont->bold);
    ASSERT_TRUE(scoreData->startFont->italic);
    ASSERT_TRUE(scoreData->startFont->absolute);

    ASSERT_TRUE(scoreData->multipleFont);
    EXPECT_EQ(scoreData->multipleFont->fontId, 2);
    EXPECT_EQ(scoreData->multipleFont->fontSize, 12);

    ASSERT_TRUE(scoreData->mmRestFont);
    EXPECT_EQ(scoreData->mmRestFont->fontId, 3);
    EXPECT_EQ(scoreData->mmRestFont->fontSize, 14);

    // Enclosures
    ASSERT_TRUE(scoreData->startEnclosure);
    EXPECT_EQ(scoreData->startEnclosure->xMargin, 9);
    EXPECT_EQ(scoreData->startEnclosure->yMargin, 9);
    EXPECT_EQ(scoreData->startEnclosure->lineWidth, 256);
    EXPECT_EQ(scoreData->startEnclosure->shape, others::Enclosure::Shape::Rectangle);

    ASSERT_TRUE(scoreData->multipleEnclosure);
    EXPECT_EQ(scoreData->multipleEnclosure->xMargin, 44);
    EXPECT_EQ(scoreData->multipleEnclosure->yMargin, 44);
    EXPECT_EQ(scoreData->multipleEnclosure->lineWidth, 256);
    EXPECT_EQ(scoreData->multipleEnclosure->shape, others::Enclosure::Shape::Ellipse);

    // Other fields
    EXPECT_EQ(scoreData->startXdisp, 1);
    EXPECT_EQ(scoreData->startYdisp, -144);
    EXPECT_EQ(scoreData->multipleXdisp, 3);
    EXPECT_EQ(scoreData->multipleYdisp, -144);
    EXPECT_EQ(scoreData->mmRestXdisp, 5);
    EXPECT_EQ(scoreData->mmRestYdisp, -144);
    EXPECT_EQ(scoreData->leftMmBracketChar, 91);
    EXPECT_EQ(scoreData->rightMmBracketChar, 93);
    EXPECT_EQ(scoreData->startWith, 2);
    EXPECT_EQ(scoreData->incidence, 1);
    EXPECT_EQ(scoreData->multipleJustify, MeasureNumberRegion::AlignJustify::Center);
    EXPECT_EQ(scoreData->mmRestJustify, MeasureNumberRegion::AlignJustify::Center);

    // Boolean fields
    EXPECT_TRUE(scoreData->showOnStart);
    EXPECT_TRUE(scoreData->showOnEvery);
    EXPECT_TRUE(scoreData->hideFirstMeasure);
    EXPECT_TRUE(scoreData->showMmRange);
    EXPECT_TRUE(scoreData->showOnMmRest);
    EXPECT_TRUE(scoreData->useStartEncl);
    EXPECT_TRUE(scoreData->useMultipleEncl);
    EXPECT_TRUE(scoreData->showOnTop);
    EXPECT_TRUE(scoreData->showOnBottom);
    EXPECT_TRUE(scoreData->excludeOthers);
    EXPECT_TRUE(scoreData->breakMmRest);
}
TEST(MeasureNumberIndividualPositioningTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <measNumbIndivPos cmper1="1" cmper2="8" inci="0">
      <region>1</region>
      <x1add>-70</x1add>
      <y1add>-21</y1add>
      <forceHide>force</forceHide>
      <useEncl/>
      <encl>
        <xAdd>3</xAdd>
        <yAdd>5</yAdd>
        <xMargin>21</xMargin>
        <yMargin>20</yMargin>
        <lineWidth>256</lineWidth>
        <sides>1</sides>
        <fixedSize/>
        <equalAspect/>
        <opaque/>
        <roundCorners/>
        <cornerRadius>768</cornerRadius>
      </encl>
    </measNumbIndivPos>
    <measNumbIndivPos cmper1="1" cmper2="8" inci="0" part="1" shared="true">
      <x1add>131</x1add>
      <y1add>-44</y1add>
      <x2add>201</x2add>
    </measNumbIndivPos>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // First instance: part 0, staff 1, measure 8, inci 0
    auto pos1 = details->get<details::MeasureNumberIndividualPositioning>(SCORE_PARTID, 1, 8, 0);
    ASSERT_TRUE(pos1);

    EXPECT_EQ(pos1->measNumRegion, Cmper{1});
    EXPECT_EQ(pos1->xOffset, Evpu{-70});
    EXPECT_EQ(pos1->yOffset, Evpu{-21});
    EXPECT_EQ(pos1->xOffset2, Evpu{0}); // not present -> default
    EXPECT_EQ(pos1->forceVisibility, details::MeasureNumberIndividualPositioning::ForceVisibility::Show);
    EXPECT_TRUE(pos1->useEnclosure);
    EXPECT_TRUE(pos1->enclosure);

    // Second instance: part 1, staff 1, measure 8, inci 0
    auto pos2 = details->get<details::MeasureNumberIndividualPositioning>(1, 1, 8, 0);
    ASSERT_TRUE(pos2);

    EXPECT_EQ(pos2->measNumRegion, Cmper{1}); // not present -> shared from score.
    EXPECT_EQ(pos2->xOffset, Evpu{131});
    EXPECT_EQ(pos2->yOffset, Evpu{-44});
    EXPECT_EQ(pos2->xOffset2, Evpu{201});
    EXPECT_EQ(pos2->forceVisibility, details::MeasureNumberIndividualPositioning::ForceVisibility::Show);
    EXPECT_TRUE(pos2->useEnclosure); // not present -> shared from score.
    EXPECT_TRUE(bool(pos2->enclosure)); // not present -> shared from score.
}
