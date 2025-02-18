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
