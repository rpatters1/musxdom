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
      <stemConnect index="0">
        <symbol>192</symbol>
        <upStemVert>768</upStemVert>
        <downStemVert>-768</downStemVert>
      </stemConnect>
      <stemConnect index="4">
        <font>15</font>
        <symbol>57534</symbol>
        <upStemVert>-1024</upStemVert>
        <downStemVert>-512</downStemVert>
        <upStemHorz>-32</upStemHorz>
        <downStemHorz>45</downStemHorz>
      </stemConnect>
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

    // Test stemConnections population (factory fills missing indices with zero-initialized entries)
    ASSERT_FALSE(stemOptions->stemConnections.empty());
    ASSERT_EQ(stemOptions->stemConnections.size(), 5); // indices 0..4 present

    // Index 0 (from XML)
    ASSERT_NE(stemOptions->stemConnections[0], nullptr);
    {
        const auto& c = *stemOptions->stemConnections[0];
        EXPECT_EQ(c.fontId, 0); // default music font when <font> omitted
        EXPECT_EQ(c.symbol, static_cast<char32_t>(192));
        EXPECT_EQ(c.upStemVert, 768);
        EXPECT_EQ(c.downStemVert, -768);
        EXPECT_EQ(c.upStemHorz, 0);
        EXPECT_EQ(c.downStemHorz, 0);
    }

    // Index 1 (missing in XML) should be zero-initialized by factory
    ASSERT_NE(stemOptions->stemConnections[1], nullptr);
    {
        const auto& c = *stemOptions->stemConnections[1];
        EXPECT_EQ(c.fontId, 0);
        EXPECT_EQ(c.symbol, static_cast<char32_t>(0));
        EXPECT_EQ(c.upStemVert, 0);
        EXPECT_EQ(c.downStemVert, 0);
        EXPECT_EQ(c.upStemHorz, 0);
        EXPECT_EQ(c.downStemHorz, 0);
    }

    // Index 4 (from XML)
    ASSERT_NE(stemOptions->stemConnections[4], nullptr);
    {
        const auto& c = *stemOptions->stemConnections[4];
        EXPECT_EQ(c.fontId, 15);
        EXPECT_EQ(c.symbol, static_cast<char32_t>(57534));
        EXPECT_EQ(c.upStemVert, -1024);
        EXPECT_EQ(c.downStemVert, -512);
        EXPECT_EQ(c.upStemHorz, -32);
        EXPECT_EQ(c.downStemHorz, 45);
    }
}
