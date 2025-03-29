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

TEST(PopulateTest, BeatChartElement)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <beatChart cmper="2" inci="0">
      <control>
        <totalDur>4096</totalDur>
        <totalWidth>539</totalWidth>
        <minWidth>1</minWidth>
        <allotWidth>932</allotWidth>
      </control>
    </beatChart>
    <beatChart cmper="2" inci="1">
      <endPos>43</endPos>
    </beatChart>
    <beatChart cmper="2" inci="2">
      <dur>256</dur>
      <pos>43</pos>
      <endPos>114</endPos>
      <minPos>1</minPos>
    </beatChart>
    <beatChart cmper="2" inci="3">
      <dur>512</dur>
      <pos>114</pos>
      <endPos>185</endPos>
      <minPos>1</minPos>
    </beatChart>
    <beatChart cmper="2" inci="4">
      <dur>768</dur>
      <pos>185</pos>
      <endPos>228</endPos>
      <minPos>1</minPos>
    </beatChart>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Control element (inci 0)
    auto bc0 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 0);
    ASSERT_TRUE(bc0);
    ASSERT_TRUE(bc0->control);
    EXPECT_EQ(bc0->control->totalDur, 4096);
    EXPECT_EQ(bc0->control->totalWidth, 539);
    EXPECT_EQ(bc0->control->minWidth, 1);
    EXPECT_EQ(bc0->control->allotWidth, 932);

    // Entry inci 1: endPos only; other values should default to 0
    auto bc1 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 1);
    ASSERT_TRUE(bc1);
    EXPECT_FALSE(bc1->control);
    EXPECT_EQ(bc1->endPos, 43);
    EXPECT_EQ(bc1->dur, 0);
    EXPECT_EQ(bc1->pos, 0);
    EXPECT_EQ(bc1->minPos, 0);

    // Entry inci 2
    auto bc2 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 2);
    ASSERT_TRUE(bc2);
    EXPECT_FALSE(bc2->control);
    EXPECT_EQ(bc2->dur, 256);
    EXPECT_EQ(bc2->pos, 43);
    EXPECT_EQ(bc2->endPos, 114);
    EXPECT_EQ(bc2->minPos, 1);

    // Entry inci 3
    auto bc3 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 3);
    ASSERT_TRUE(bc3);
    EXPECT_FALSE(bc3->control);
    EXPECT_EQ(bc3->dur, 512);
    EXPECT_EQ(bc3->pos, 114);
    EXPECT_EQ(bc3->endPos, 185);
    EXPECT_EQ(bc3->minPos, 1);

    // Entry inci 4
    auto bc4 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 4);
    ASSERT_TRUE(bc4);
    EXPECT_FALSE(bc4->control);
    EXPECT_EQ(bc4->dur, 768);
    EXPECT_EQ(bc4->pos, 185);
    EXPECT_EQ(bc4->endPos, 228);
    EXPECT_EQ(bc4->minPos, 1);
}
