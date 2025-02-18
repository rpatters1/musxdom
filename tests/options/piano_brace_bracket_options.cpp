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

TEST(PianoBraceBracketOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <pianoBraceBracketOptions>
      <defBracketPos>-12</defBracketPos>
      <centerThickness>2.000000e+00</centerThickness>
      <endThickness>7.000000e+00</endThickness>
      <outerWingY>5.000000e+00</outerWingY>
      <innerTipY>3.000000e+00</innerTipY>
      <innerWingY>6.000000e+00</innerWingY>
      <outerTipX>1.000000e+00</outerTipX>
      <outerTipY>2.000000e+00</outerTipY>
      <outerWingX>4.000000e+00</outerWingX>
      <width>1.200000e+01</width>
      <innerTipX>1.200000e+01</innerTipX>
      <innerWingX>1.200000e+01</innerWingX>
    </pianoBraceBracketOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto pianoBraceBracketOptions = options->get<musx::dom::options::PianoBraceBracketOptions>();
    ASSERT_TRUE(pianoBraceBracketOptions);

    // Test all properties of PianoBraceBracketOptions
    EXPECT_EQ(pianoBraceBracketOptions->defBracketPos, Evpu(-12));
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->centerThickness, 2.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->tipThickness, 7.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->outerBodyV, 5.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->innerTipV, 3.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->innerBodyV, 6.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->outerTipH, 1.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->outerTipV, 2.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->outerBodyH, 4.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->width, 12.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->innerTipH, 12.0f);
    EXPECT_DOUBLE_EQ(pianoBraceBracketOptions->innerBodyH, 12.0f);
}
