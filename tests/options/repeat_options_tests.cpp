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

TEST(RepeatOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <repeatOptions>
      <bracketHeight>108</bracketHeight>
      <maxPasses>20</maxPasses>
      <addPeriod/>
      <thickLineWidth>1152</thickLineWidth>
      <thinLineWidth>384</thinLineWidth>
      <lineSpace>768</lineSpace>
      <backToBackStyle>thick</backToBackStyle>
      <forwardDotHPos>12</forwardDotHPos>
      <backwardDotHPos>12</backwardDotHPos>
      <upperDotVPos>1</upperDotVPos>
      <lowerDotVPos>2</lowerDotVPos>
      <wingStyle>curved</wingStyle>
      <afterClefSpace>60</afterClefSpace>
      <afterKeySpace>60</afterKeySpace>
      <afterTimeSpace>60</afterTimeSpace>
      <bracketHookLen>108</bracketHookLen>
      <bracketLineWidth>384</bracketLineWidth>
      <bracketStartInset>2</bracketStartInset>
      <bracketEndInset>24</bracketEndInset>
      <bracketTextHPos>24</bracketTextHPos>
      <bracketTextVPos>36</bracketTextVPos>
      <bracketEndHookLen>1</bracketEndHookLen>
      <bracketEndAnchorThinLine/>
      <showOnStaffListNumber>1</showOnStaffListNumber>
    </repeatOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto repeatOptions = options->get<musx::dom::options::RepeatOptions>();
    ASSERT_TRUE(repeatOptions);

    // Test all properties of RepeatOptions
    EXPECT_EQ(repeatOptions->bracketHeight, 108);
    EXPECT_EQ(repeatOptions->maxPasses, 20);
    EXPECT_TRUE(repeatOptions->addPeriod);
    EXPECT_EQ(repeatOptions->thickLineWidth, 1152);
    EXPECT_EQ(repeatOptions->thinLineWidth, 384);
    EXPECT_EQ(repeatOptions->lineSpace, 768);
    EXPECT_EQ(repeatOptions->backToBackStyle, musx::dom::options::RepeatOptions::BackToBackStyle::Thick);
    EXPECT_EQ(repeatOptions->forwardDotHPos, 12);
    EXPECT_EQ(repeatOptions->backwardDotHPos, 12);
    EXPECT_EQ(repeatOptions->upperDotVPos, 1);
    EXPECT_EQ(repeatOptions->lowerDotVPos, 2);
    EXPECT_EQ(repeatOptions->wingStyle, musx::dom::options::RepeatOptions::WingStyle::Curved);
    EXPECT_EQ(repeatOptions->afterClefSpace, 60);
    EXPECT_EQ(repeatOptions->afterKeySpace, 60);
    EXPECT_EQ(repeatOptions->afterTimeSpace, 60);
    EXPECT_EQ(repeatOptions->bracketHookLen, 108);
    EXPECT_EQ(repeatOptions->bracketLineWidth, 384);
    EXPECT_EQ(repeatOptions->bracketStartInset, 2);
    EXPECT_EQ(repeatOptions->bracketEndInset, 24);
    EXPECT_EQ(repeatOptions->bracketTextHPos, 24);
    EXPECT_EQ(repeatOptions->bracketTextVPos, 36);
    EXPECT_EQ(repeatOptions->bracketEndHookLen, 1);
    EXPECT_TRUE(repeatOptions->bracketEndAnchorThinLine);
    EXPECT_EQ(repeatOptions->showOnStaffListNumber, 1);
}


TEST(RepeatOptionsTest, DefaultEnumsTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <repeatOptions/>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto repeatOptions = options->get<musx::dom::options::RepeatOptions>();
    ASSERT_TRUE(repeatOptions);

    // Test default values of enums
    EXPECT_EQ(repeatOptions->backToBackStyle, musx::dom::options::RepeatOptions::BackToBackStyle::Thin);
    EXPECT_EQ(repeatOptions->wingStyle, musx::dom::options::RepeatOptions::WingStyle::None);
}
