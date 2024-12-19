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

TEST(StemOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <stemOptions>
      <halfStemLength>18</halfStemLength>
      <stemLength>84</stemLength>
      <stem2>60</stem2>
      <revStemAdj>216</revStemAdj>
      <noReverseStems/>
      <stemWidth>128</stemWidth>
      <stemLift>64</stemLift>
      <useStemConnections/>
    </stemOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto stemOptions = options->get<musx::dom::options::StemOptions>();
    ASSERT_TRUE(stemOptions);

    // Test all properties of StemOptions
    EXPECT_EQ(stemOptions->halfStemLength, 18);
    EXPECT_EQ(stemOptions->stemLength, 84);
    EXPECT_EQ(stemOptions->shortStemLength, 60);
    EXPECT_EQ(stemOptions->revStemAdj, 216);
    EXPECT_TRUE(stemOptions->noReverseStems);
    EXPECT_EQ(stemOptions->stemWidth, 128);
    EXPECT_EQ(stemOptions->stemOffset, 64);
    EXPECT_TRUE(stemOptions->useStemConnections);
}
