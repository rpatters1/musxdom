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

TEST(FlagOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <flagOptions>
      <straightFlags/>
      <upHAdj>64</upHAdj>
      <downHAdj>-64</downHAdj>
      <upHAdj2>128</upHAdj2>
      <downHAdj2>-128</downHAdj2>
      <upHAdj16>192</upHAdj16>
      <downHAdj16>-192</downHAdj16>
      <eighthFlagHoist>1600</eighthFlagHoist>
      <stUpHAdj>256</stUpHAdj>
      <stDownHAdj>-256</stDownHAdj>
      <upVAdj>-64</upVAdj>
      <downVAdj>64</downVAdj>
      <upVadj2>-576</upVadj2>
      <downVAdj2>576</downVAdj2>
      <upVAdj16>-320</upVAdj16>
      <downVAdj16>512</downVAdj16>
      <stUpVAdj>-256</stUpVAdj>
      <stDownVAdj>256</stDownVAdj>
      <separ>20</separ>
      <extra>1</extra>
    </flagOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto flagOptions = options->get<musx::dom::options::FlagOptions>();
    ASSERT_TRUE(flagOptions);

    // Test all properties of FlagOptions
    EXPECT_TRUE(flagOptions->straightFlags);
    EXPECT_EQ(flagOptions->upHAdj, 64);
    EXPECT_EQ(flagOptions->downHAdj, -64);
    EXPECT_EQ(flagOptions->upHAdj2, 128);
    EXPECT_EQ(flagOptions->downHAdj2, -128);
    EXPECT_EQ(flagOptions->upHAdj16, 192);
    EXPECT_EQ(flagOptions->downHAdj16, -192);
    EXPECT_EQ(flagOptions->eighthFlagHoist, 1600);
    EXPECT_EQ(flagOptions->stUpHAdj, 256);
    EXPECT_EQ(flagOptions->stDownHAdj, -256);
    EXPECT_EQ(flagOptions->upVAdj, -64);
    EXPECT_EQ(flagOptions->downVAdj, 64);
    EXPECT_EQ(flagOptions->upVAdj2, -576);
    EXPECT_EQ(flagOptions->downVAdj2, 576);
    EXPECT_EQ(flagOptions->upVAdj16, -320);
    EXPECT_EQ(flagOptions->downVAdj16, 512);
    EXPECT_EQ(flagOptions->stUpVAdj, -256);
    EXPECT_EQ(flagOptions->stDownVAdj, 256);
    EXPECT_EQ(flagOptions->flagSpacing, 20);
    EXPECT_EQ(flagOptions->secondaryGroupAdj, 1);
}
