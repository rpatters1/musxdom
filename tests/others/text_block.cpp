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

TEST(TextBlockTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textBlock cmper="32">
      <textID>31</textID>
      <lineSpacingPercent>100</lineSpacingPercent>
      <newPos36/>
      <showShape/>
      <noExpandSingleWord/>
      <wordWrap/>
      <width>436</width>
      <height>117</height>
      <roundCorners/>
      <cornerRadius>512</cornerRadius>
      <textTag>block</textTag>
    </textBlock>
    <textBlock cmper="33">
      <textID>221</textID>
      <lineSpacingPercent>100</lineSpacingPercent>
      <newPos36/>
      <showShape/>
      <wordWrap/>
      <textTag>expression</textTag>
    </textBlock>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Test first text block
    auto textBlock1 = others->get<others::TextBlock>(SCORE_PARTID, 32);
    ASSERT_TRUE(textBlock1) << "TextBlock 32 not found but does exist";
    EXPECT_EQ(textBlock1->textId, 31);
    EXPECT_EQ(textBlock1->lineSpacingPercentage, 100);
    EXPECT_TRUE(textBlock1->newPos36);
    EXPECT_TRUE(textBlock1->showShape);
    EXPECT_TRUE(textBlock1->noExpandSingleWord);
    EXPECT_TRUE(textBlock1->wordWrap);
    EXPECT_EQ(textBlock1->width, 436);
    EXPECT_EQ(textBlock1->height, 117);
    EXPECT_TRUE(textBlock1->roundCorners);
    EXPECT_EQ(textBlock1->cornerRadius, 512);
    EXPECT_EQ(textBlock1->textType, others::TextBlock::TextType::Block);

    // Test second text block
    auto textBlock2 = others->get<others::TextBlock>(SCORE_PARTID, 33);
    ASSERT_TRUE(textBlock2) << "TextBlock 33 not found but does exist";
    EXPECT_EQ(textBlock2->textId, 221);
    EXPECT_EQ(textBlock2->lineSpacingPercentage, 100);
    EXPECT_TRUE(textBlock2->newPos36);
    EXPECT_TRUE(textBlock2->showShape);
    EXPECT_FALSE(textBlock2->noExpandSingleWord);
    EXPECT_TRUE(textBlock2->wordWrap);
    EXPECT_EQ(textBlock2->width, 0);  // Default value for optional fields
    EXPECT_EQ(textBlock2->height, 0);
    EXPECT_FALSE(textBlock2->roundCorners);
    EXPECT_EQ(textBlock2->cornerRadius, 0);
    EXPECT_EQ(textBlock2->textType, others::TextBlock::TextType::Expression);
}

TEST(PageTextAssignTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <pageTextAssign cmper="0" inci="0">
      <block>2</block>
      <xdisp>5</xdisp>
      <ydisp>7</ydisp>
      <startPage>1</startPage>
      <endPage>2</endPage>
      <hposLp>right</hposLp>
      <hposRp>center</hposRp>
      <postIt/>
      <vpos>bottom</vpos>
      <indRpPos/>
      <rightPgXdisp>-13</rightPgXdisp>
      <rightPgYdisp>139</rightPgYdisp>
    </pageTextAssign>
    <pageTextAssign cmper="0" inci="0" part="1" shared="true">
      <postIt>
        <offInPart/>
      </postIt>
      <hposPageEdge/>
      <indRpPos/>
      <rightPgXdisp>70</rightPgXdisp>
      <rightPgYdisp>93</rightPgYdisp>
    </pageTextAssign>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Test first page text assignment (SCORE_PARTID)
    auto pageText1 = others->get<others::PageTextAssign>(SCORE_PARTID, 0, 0);
    ASSERT_TRUE(pageText1) << "PageTextAssign 0 not found for SCORE_PARTID but does exist";
    EXPECT_EQ(pageText1->block, 2);
    EXPECT_EQ(pageText1->xDisp, 5);
    EXPECT_EQ(pageText1->yDisp, 7);
    EXPECT_EQ(pageText1->startPage, 1);
    EXPECT_EQ(pageText1->endPage, 2);
    EXPECT_EQ(pageText1->hPosLp, others::PageTextAssign::HorizontalAlignment::Right);
    EXPECT_EQ(pageText1->hPosRp, others::PageTextAssign::HorizontalAlignment::Center);
    EXPECT_TRUE(pageText1->hidden);   // postIt sets screen-only visibility
    EXPECT_FALSE(pageText1->hPosPageEdge); // present postIt again
    EXPECT_EQ(pageText1->vPos, others::PageTextAssign::VerticalAlignment::Bottom);
    EXPECT_TRUE(pageText1->indRpPos);
    EXPECT_EQ(pageText1->rightPgXDisp, -13);
    EXPECT_EQ(pageText1->rightPgYDisp, 139);

    // Test second page text assignment (part 1) — should inherit from first
    auto pageText2 = others->get<others::PageTextAssign>(1, 0, 0);
    ASSERT_TRUE(pageText2) << "PageTextAssign 0 not found for part 1 but does exist";

    // Inherited fields
    EXPECT_EQ(pageText2->block, pageText1->block);
    EXPECT_EQ(pageText2->xDisp, pageText1->xDisp);
    EXPECT_EQ(pageText2->yDisp, pageText1->yDisp);
    EXPECT_EQ(pageText2->startPage, pageText1->startPage);
    EXPECT_EQ(pageText2->endPage, pageText1->endPage);
    EXPECT_EQ(pageText2->hPosLp, pageText1->hPosLp);
    EXPECT_EQ(pageText2->hPosRp, pageText1->hPosRp);
    EXPECT_EQ(pageText2->vPos, pageText1->vPos);
    EXPECT_TRUE(pageText2->hPosPageEdge);
    EXPECT_TRUE(pageText2->indRpPos);

    // Overridden fields
    EXPECT_FALSE(pageText2->hidden);   // postIt sets screen-only visibility
    EXPECT_EQ(pageText2->rightPgXDisp, 70);
    EXPECT_EQ(pageText2->rightPgYDisp, 93);
}

TEST(MeasureTextAssignTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <measTextAssign cmper1="2" cmper2="1" inci="0">
      <block>2</block>
      <xdispEdu>728</xdispEdu>
      <ydisp>104</ydisp>
      <postIt/>
    </measTextAssign>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto assign = details->get<details::MeasureTextAssign>(SCORE_PARTID, 2, 1, 0);
    ASSERT_TRUE(assign);

    EXPECT_EQ(assign->block, 2);
    EXPECT_EQ(assign->xDispEdu, 728);
    EXPECT_EQ(assign->yDisp, 104);
    EXPECT_TRUE(assign->hidden);
}
