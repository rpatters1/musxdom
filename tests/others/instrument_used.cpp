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

TEST(InstrumentUsedTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <instUsed cmper="1" inci="0">
      <inst>6</inst>
      <trackType>staff</trackType>
      <distFromTop>-751</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="1" inci="1">
      <inst>16</inst>
      <trackType>staff</trackType>
      <distFromTop>-1026</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Test StaffUsed with cmper=1 (system) and inci=0
    {
        auto instUsed = others->get<others::StaffUsed>(SCORE_PARTID, 1, 0);
        ASSERT_TRUE(instUsed);

        EXPECT_EQ(instUsed->staffId, 6);
        EXPECT_EQ(instUsed->distFromTop, -751);
        ASSERT_TRUE(instUsed->range);

        EXPECT_EQ(instUsed->range->startMeas, 1);
        EXPECT_EQ(instUsed->range->startEdu, 0);
        EXPECT_EQ(instUsed->range->endMeas, 32767);
        EXPECT_EQ(instUsed->range->endEdu, 2147483647);
    }

    // Test StaffUsed with cmper=1 (system) and inci=1
    {
        auto instUsed = others->get<others::StaffUsed>(SCORE_PARTID, 1, 1);
        ASSERT_TRUE(instUsed);

        EXPECT_EQ(instUsed->staffId, 16);
        EXPECT_EQ(instUsed->distFromTop, -1026);
        ASSERT_TRUE(instUsed->range);

        EXPECT_EQ(instUsed->range->startMeas, 1);
        EXPECT_EQ(instUsed->range->startEdu, 0);
        EXPECT_EQ(instUsed->range->endMeas, 32767);
        EXPECT_EQ(instUsed->range->endEdu, 2147483647);
    }
}
