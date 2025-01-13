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

using namespace musx::dom;

TEST(StaffTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <staffSpec cmper="15">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <defaultClef>1</defaultClef>
      <transposedClef>2</transposedClef>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-1</stemReversal>
      <fullName>75</fullName>
      <abbrvName>76</abbrvName>
      <topRepeatDotOff>-3</topRepeatDotOff>
      <botRepeatDotOff>-5</botRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
    </staffSpec>
    <staffSpec cmper="16">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <defaultClef>3</defaultClef>
      <transposedClef>4</transposedClef>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-2</stemReversal>
      <fullName>77</fullName>
      <abbrvName>78</abbrvName>
      <topRepeatDotOff>-2</topRepeatDotOff>
      <botRepeatDotOff>-6</botRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
    </staffSpec>
  </others>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
        auto others = doc->getOthers();
        ASSERT_TRUE(others);

        auto staff1 = others->get<others::Staff>(SCORE_PARTID, 15);
        ASSERT_TRUE(staff1) << "Staff with cmper 15 not found";

        EXPECT_EQ(staff1->staffLines, 5);
        EXPECT_EQ(staff1->lineSpace, 24);
        EXPECT_EQ(staff1->defaultClef, 1);
        EXPECT_EQ(staff1->transposedClef, 2);
        EXPECT_EQ(staff1->dwRestOffset, -4);
        EXPECT_EQ(staff1->wRestOffset, -4);
        EXPECT_EQ(staff1->hRestOffset, -4);
        EXPECT_EQ(staff1->otherRestOffset, -4);
        EXPECT_EQ(staff1->stemReversal, -1);
        EXPECT_EQ(staff1->fullNameTextId, 75);
        EXPECT_EQ(staff1->abbrvNameTextId, 76);
        EXPECT_EQ(staff1->topRepeatDotOff, -3);
        EXPECT_EQ(staff1->botRepeatDotOff, -5);
        EXPECT_EQ(staff1->vertTabNumOff, -1024);

        auto staff2 = others->get<others::Staff>(SCORE_PARTID, 16);
        ASSERT_TRUE(staff2) << "Staff with cmper 16 not found";

        EXPECT_EQ(staff2->staffLines, 5);
        EXPECT_EQ(staff2->lineSpace, 24);
        EXPECT_EQ(staff2->defaultClef, 3);
        EXPECT_EQ(staff2->transposedClef, 4);
        EXPECT_EQ(staff2->dwRestOffset, -4);
        EXPECT_EQ(staff2->wRestOffset, -4);
        EXPECT_EQ(staff2->hRestOffset, -4);
        EXPECT_EQ(staff2->otherRestOffset, -4);
        EXPECT_EQ(staff2->stemReversal, -2);
        EXPECT_EQ(staff2->fullNameTextId, 77);
        EXPECT_EQ(staff2->abbrvNameTextId, 78);
        EXPECT_EQ(staff2->topRepeatDotOff, -2);
        EXPECT_EQ(staff2->botRepeatDotOff, -6);
        EXPECT_EQ(staff2->vertTabNumOff, -1024);
    }
}
