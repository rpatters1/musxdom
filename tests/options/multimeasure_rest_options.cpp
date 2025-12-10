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

TEST(MultimeasureRestOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <multimeasureRestOptions>
      <meaSpace>216</meaSpace>
      <numdec>-28</numdec>
      <shapeDef>119</shapeDef>
      <numStart>2</numStart>
      <threshold>9</threshold>
      <spacing>48</spacing>
      <numAdjX>5</numAdjX>
      <startAdjust>1</startAdjust>
      <endAdjust>3</endAdjust>
      <useCharRestStyle/>
      <autoUpdateMmRests/>
    </multimeasureRestOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto mmRestOptions = options->get<musx::dom::options::MultimeasureRestOptions>();
    ASSERT_TRUE(mmRestOptions);

    // Test all properties of MultimeasureRestOptions
    EXPECT_EQ(mmRestOptions->measWidth, 216);
    EXPECT_EQ(mmRestOptions->numAdjY, -28);
    EXPECT_EQ(mmRestOptions->shapeDef, 119);
    EXPECT_EQ(mmRestOptions->numStart, 2);
    EXPECT_EQ(mmRestOptions->useSymsThreshold, 9);
    EXPECT_EQ(mmRestOptions->symSpacing, 48);
    EXPECT_EQ(mmRestOptions->numAdjX, 5);
    EXPECT_EQ(mmRestOptions->startAdjust, 1);
    EXPECT_EQ(mmRestOptions->endAdjust, 3);
    EXPECT_TRUE(mmRestOptions->useSymbols);
    EXPECT_TRUE(mmRestOptions->autoUpdateMmRests);
}
