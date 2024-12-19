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

TEST(BarlineOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <barlineOptions>
      <drawCloseSystemBarline/>
      <drawCloseFinalBarline/>
      <drawFinalBarlineOnLastMeas/>
      <leftBarlineUsePrevStyle/>
      <drawLeftBarlineSingleStaff/>
      <drawLeftBarlineMultipleStaves/>
      <drawBarlines/>
      <barlineWidth>256</barlineWidth>
      <thickBarlineWidth>1152</thickBarlineWidth>
      <doubleBarlineSpace>768</doubleBarlineSpace>
      <finalBarlineSpace>768</finalBarlineSpace>
      <barlineDashOn>18</barlineDashOn>
      <barlineDashOff>18</barlineDashOff>
      <drawDoubleBarlineBeforeKeyChanges/>
    </barlineOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto barlineOptions = options->get<musx::dom::options::BarlineOptions>();
    ASSERT_TRUE(barlineOptions);

    // Test all properties of BarlineOptions
    EXPECT_TRUE(barlineOptions->drawBarlines);
    EXPECT_TRUE(barlineOptions->drawCloseSystemBarline);
    EXPECT_TRUE(barlineOptions->drawCloseFinalBarline);
    EXPECT_TRUE(barlineOptions->drawFinalBarlineOnLastMeas);
    EXPECT_TRUE(barlineOptions->drawDoubleBarlineBeforeKeyChanges);
    EXPECT_TRUE(barlineOptions->drawLeftBarlineSingleStaff);
    EXPECT_TRUE(barlineOptions->drawLeftBarlineMultipleStaves);
    EXPECT_TRUE(barlineOptions->leftBarlineUsePrevStyle);
    EXPECT_EQ(barlineOptions->thickBarlineWidth, 1152);
    EXPECT_EQ(barlineOptions->barlineWidth, 256);
    EXPECT_EQ(barlineOptions->doubleBarlineSpace, 768);
    EXPECT_EQ(barlineOptions->finalBarlineSpace, 768);
    EXPECT_EQ(barlineOptions->barlineDashOn, 18);
    EXPECT_EQ(barlineOptions->barlineDashOff, 18);
}

TEST(BarlineOptionsTest, FalseBooleans)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <barlineOptions>
      <thickBarlineWidth>2</thickBarlineWidth>
      <doubleBarlineSpace>3</doubleBarlineSpace>
      <finalBarlineSpace>4</finalBarlineSpace>
      <barlineDashOn>5</barlineDashOn>
      <barlineDashOff>6</barlineDashOff>
    </barlineOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto barlineOptions = options->get<musx::dom::options::BarlineOptions>();
    ASSERT_TRUE(barlineOptions);

    // Test all properties of BarlineOptions
    EXPECT_FALSE(barlineOptions->drawBarlines);
    EXPECT_FALSE(barlineOptions->drawCloseSystemBarline);
    EXPECT_FALSE(barlineOptions->drawCloseFinalBarline);
    EXPECT_FALSE(barlineOptions->drawFinalBarlineOnLastMeas);
    EXPECT_FALSE(barlineOptions->drawDoubleBarlineBeforeKeyChanges);
    EXPECT_FALSE(barlineOptions->drawLeftBarlineSingleStaff);
    EXPECT_FALSE(barlineOptions->drawLeftBarlineMultipleStaves);
    EXPECT_FALSE(barlineOptions->leftBarlineUsePrevStyle);
    EXPECT_EQ(barlineOptions->thickBarlineWidth, 2);
    EXPECT_EQ(barlineOptions->barlineWidth, 0);
    EXPECT_EQ(barlineOptions->doubleBarlineSpace, 3);
    EXPECT_EQ(barlineOptions->finalBarlineSpace, 4);
    EXPECT_EQ(barlineOptions->barlineDashOn, 5);
    EXPECT_EQ(barlineOptions->barlineDashOff, 6);
}
