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

constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <staffSystemSpec cmper="1">
      <startMeas>1</startMeas>
      <endMeas>4</endMeas>
      <horzPercent>12050</horzPercent>
      <ssysPercent>70</ssysPercent>
      <staffHeight>6144</staffHeight>
      <top>-12</top>
      <left>576</left>
      <right>-13</right>
      <bottom>-110</bottom>
      <hasStaffScaling/>
      <placeEndSpaceBeforeBarline/>
      <scaleVert/>
      <scaleContentOnly/>
      <distanceToPrev>-490</distanceToPrev>
      <extraStartSystemSpace>24</extraStartSystemSpace>
      <extraEndSystemSpace>36</extraEndSystemSpace>
    </staffSystemSpec>
  </others>
</finale>
)xml";

TEST(StaffSystemTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staffSystem = others->get<others::StaffSystem>(SCORE_PARTID, 1);
    ASSERT_TRUE(staffSystem) << "StaffSystem with cmper 1 not found";

    EXPECT_EQ(staffSystem->startMeas, 1);
    EXPECT_EQ(staffSystem->endMeas, 4);
    EXPECT_DOUBLE_EQ(staffSystem->horzPercent, 120.50); // Divided by 100.0 in XML mapping.
    EXPECT_EQ(staffSystem->ssysPercent, 70);
    EXPECT_EQ(staffSystem->staffHeight, 6144);
    EXPECT_EQ(staffSystem->top, -12);
    EXPECT_EQ(staffSystem->left, 576);
    EXPECT_EQ(staffSystem->right, -13);
    EXPECT_EQ(staffSystem->bottom, -110);
    EXPECT_TRUE(staffSystem->hasStaffScaling);
    EXPECT_TRUE(staffSystem->placeEndSpaceBeforeBarline);
    EXPECT_TRUE(staffSystem->scaleVert);
    EXPECT_TRUE(staffSystem->holdMargins); // Corresponds to `<scaleContentOnly>`.
    EXPECT_EQ(staffSystem->distanceToPrev, -490);
    EXPECT_EQ(staffSystem->extraStartSystemSpace, 24);
    EXPECT_EQ(staffSystem->extraEndSystemSpace, 36);
}
