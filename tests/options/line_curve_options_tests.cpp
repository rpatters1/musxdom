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

TEST(LineCurveOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <lineCurveOptions>
      <bezierStep>32</bezierStep>
      <enclosureWidth>256</enclosureWidth>
      <enclosureRoundCorners/>
      <enclosureCornerRadius>768</enclosureCornerRadius>
      <staffLineWidth>128</staffLineWidth>
      <legerLineWidth>256</legerLineWidth>
      <legerFrontLength>6</legerFrontLength>
      <legerBackLength>6</legerBackLength>
      <restLegerFrontLength>6</restLegerFrontLength>
      <restLegerBackLength>6</restLegerBackLength>
      <psUlDepth>-1.000000e+00</psUlDepth>
      <psUlWidth>1.000000e+00</psUlWidth>
      <pathSlurTipWidth>1.200000e+01</pathSlurTipWidth>
    </lineCurveOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto lineCurveOptions = options->get<musx::dom::options::LineCurveOptions>();
    ASSERT_TRUE(lineCurveOptions);

    // Test all properties of LineCurveOptions
    EXPECT_EQ(lineCurveOptions->bezierStep, 32);
    EXPECT_EQ(lineCurveOptions->enclosureWidth, 256);
    EXPECT_TRUE(lineCurveOptions->enclosureRoundCorners);
    EXPECT_EQ(lineCurveOptions->enclosureCornerRadius, 768);
    EXPECT_EQ(lineCurveOptions->staffLineWidth, 128);
    EXPECT_EQ(lineCurveOptions->legerLineWidth, 256);
    EXPECT_EQ(lineCurveOptions->legerFrontLength, 6);
    EXPECT_EQ(lineCurveOptions->legerBackLength, 6);
    EXPECT_EQ(lineCurveOptions->restLegerFrontLength, 6);
    EXPECT_EQ(lineCurveOptions->restLegerBackLength, 6);
    EXPECT_DOUBLE_EQ(lineCurveOptions->psUlDepth, -1.0);
    EXPECT_DOUBLE_EQ(lineCurveOptions->psUlWidth, 1.0);
    EXPECT_DOUBLE_EQ(lineCurveOptions->pathSlurTipWidth, 12.0);
}
