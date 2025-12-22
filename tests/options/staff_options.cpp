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

TEST(StaffOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <staffOptions>
      <staffSeparation>-288</staffSeparation>
      <staffSeparIncr>72</staffSeparIncr>
      <autoAdjustStaffSepar/>
      <namePos>
        <horzOff>-248</horzOff>
        <vertOff>-24</vertOff>
        <halign>right</halign>
        <expand/>
      </namePos>
      <namePosAbbrv>
        <horzOff>-108</horzOff>
        <vertOff>-26</vertOff>
        <justify>center</justify>
        <halign>right</halign>
      </namePosAbbrv>
      <groupNameFullPos>
        <horzOff>-248</horzOff>
        <justify>center</justify>
        <halign>center</halign>
        <expand/>
      </groupNameFullPos>
      <groupNameAbbrvPos>
        <horzOff>-108</horzOff>
        <justify>center</justify>
        <halign>center</halign>
        <expand/>
      </groupNameAbbrvPos>
    </staffOptions>
  </options>
</finale>
)xml";

    using StaffOptions = musx::dom::options::StaffOptions;
    using AlignJustify = musx::dom::AlignJustify;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto staffOptions = options->get<StaffOptions>();
    ASSERT_TRUE(staffOptions);

    // Test main properties
    EXPECT_EQ(staffOptions->staffSeparation, -288);
    EXPECT_EQ(staffOptions->staffSeparIncr, 72);
    EXPECT_TRUE(staffOptions->autoAdjustStaffSepar);

    // Test NamePositioning properties
    ASSERT_TRUE(staffOptions->namePos);
    EXPECT_EQ(staffOptions->namePos->horzOff, -248);
    EXPECT_EQ(staffOptions->namePos->vertOff, -24);
    EXPECT_EQ(staffOptions->namePos->justify, AlignJustify::Left);
    EXPECT_EQ(staffOptions->namePos->hAlign, AlignJustify::Right);
    EXPECT_TRUE(staffOptions->namePos->expand);

    ASSERT_TRUE(staffOptions->namePosAbbrv);
    EXPECT_EQ(staffOptions->namePosAbbrv->horzOff, -108);
    EXPECT_EQ(staffOptions->namePosAbbrv->vertOff, -26);
    EXPECT_EQ(staffOptions->namePosAbbrv->justify, AlignJustify::Center);
    EXPECT_EQ(staffOptions->namePosAbbrv->hAlign, AlignJustify::Right);
    EXPECT_FALSE(staffOptions->namePosAbbrv->expand);

    ASSERT_TRUE(staffOptions->groupNameFullPos);
    EXPECT_EQ(staffOptions->groupNameFullPos->horzOff, -248);
    EXPECT_EQ(staffOptions->groupNameFullPos->justify, AlignJustify::Center);
    EXPECT_EQ(staffOptions->groupNameFullPos->hAlign, AlignJustify::Center);
    EXPECT_TRUE(staffOptions->groupNameFullPos->expand);

    ASSERT_TRUE(staffOptions->groupNameAbbrvPos);
    EXPECT_EQ(staffOptions->groupNameAbbrvPos->horzOff, -108);
    EXPECT_EQ(staffOptions->groupNameAbbrvPos->justify, AlignJustify::Center);
    EXPECT_EQ(staffOptions->groupNameAbbrvPos->hAlign, AlignJustify::Center);
    EXPECT_TRUE(staffOptions->groupNameAbbrvPos->expand);
}
