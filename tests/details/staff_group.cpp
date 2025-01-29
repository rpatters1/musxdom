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
    <instUsed cmper="0" inci="0">
      <inst>1</inst>
      <trackType>staff</trackType>
      <distFromTop>0</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="1">
      <inst>3</inst>
      <trackType>staff</trackType>
      <distFromTop>-264</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="2">
      <inst>2</inst>
      <trackType>staff</trackType>
      <distFromTop>-528</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="3">
      <inst>4</inst>
      <trackType>staff</trackType>
      <distFromTop>-956</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <partDef cmper="0">
      <nameID>1</nameID>
      <partOrder>0</partOrder>
      <copies>1</copies>
      <printPart/>
    </partDef>
  </others>
  <details>
    <staffGroup cmper1="0" cmper2="1">
      <startInst>1</startInst>
      <endInst>2</endInst>
      <startMeas>1</startMeas>
      <endMeas>32767</endMeas>
      <fullID>42</fullID>
      <fullXadj>-247</fullXadj>
      <fullYadj>6</fullYadj>
      <bracket>
        <id>8</id>
        <bracPos>-13</bracPos>
        <bracTop>1</bracTop>
        <bracBot>2</bracBot>
        <onSingle/>
      </bracket>
      <barline>double</barline>
      <fullJustify>center</fullJustify>
      <abbrvJustify>center</abbrvJustify>
      <groupBarlineStyle>Mensurstriche</groupBarlineStyle>
      <ownBarline/>
      <fullIndivPos/>
      <abbrvIndivPos/>
      <hideName/>
      <abbrvID>46</abbrvID>
      <abbrvXadj>-106</abbrvXadj>
      <abbrvYadj>4</abbrvYadj>
      <fullHAlign>center</fullHAlign>
      <abbrvHAlign>center</abbrvHAlign>
      <fullExpand/>
      <abbrvExpand/>
      <optimize>none</optimize>
    </staffGroup>
  </details>
</finale>
)xml";

TEST(StaffGroupTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto staffGroup = details->get<details::StaffGroup>(SCORE_PARTID, BASE_SYSTEM_ID, 1);
    ASSERT_TRUE(staffGroup) << "StaffGroup with cmper1 0 and cmper2 1 not found";

    EXPECT_EQ(staffGroup->startInst, 1);
    EXPECT_EQ(staffGroup->endInst, 2);
    EXPECT_EQ(staffGroup->startMeas, 1);
    EXPECT_EQ(staffGroup->endMeas, 32767);
    EXPECT_EQ(staffGroup->fullNameId, 42);
    EXPECT_EQ(staffGroup->fullNameXadj, -247);
    EXPECT_EQ(staffGroup->fullNameYadj, 6);

    ASSERT_TRUE(staffGroup->bracket);
    EXPECT_EQ(staffGroup->bracket->style, details::StaffGroup::BracketStyle::DeskBracket);
    EXPECT_EQ(staffGroup->bracket->horzAdjLeft, -13);
    EXPECT_EQ(staffGroup->bracket->vertAdjTop, 1);
    EXPECT_EQ(staffGroup->bracket->vertAdjBot, 2);
    EXPECT_TRUE(staffGroup->bracket->showOnSingleStaff);

    EXPECT_EQ(staffGroup->barlineType, details::StaffGroup::BarlineType::Double);
    EXPECT_EQ(staffGroup->fullNameJustify, details::StaffGroup::AlignJustify::Center);
    EXPECT_EQ(staffGroup->abbrvNameJustify, details::StaffGroup::AlignJustify::Center);
    EXPECT_EQ(staffGroup->drawBarlines, details::StaffGroup::DrawBarlineStyle::Mensurstriche);
    EXPECT_EQ(staffGroup->abbrvNameId, 46);
    EXPECT_EQ(staffGroup->abbrvNameXadj, -106);
    EXPECT_EQ(staffGroup->abbrvNameYadj, 4);
    EXPECT_EQ(staffGroup->fullNameAlign, details::StaffGroup::AlignJustify::Center);
    EXPECT_EQ(staffGroup->abbrvNameAlign, details::StaffGroup::AlignJustify::Center);
    EXPECT_TRUE(staffGroup->ownBarline);
    EXPECT_TRUE(staffGroup->fullNameIndivPos);
    EXPECT_TRUE(staffGroup->abbrvNameIndivPos);
    EXPECT_TRUE(staffGroup->fullNameExpand);
    EXPECT_TRUE(staffGroup->abbrvNameExpand);
    EXPECT_EQ(staffGroup->hideStaves, details::StaffGroup::HideStaves::None);
    EXPECT_TRUE(staffGroup->hideName);

    EXPECT_EQ(staffGroup->staves.size(), 3) << "group has 3 staves";
    EXPECT_NE(staffGroup->staves.find(1), staffGroup->staves.end()) << "group contains staff 1";
    EXPECT_NE(staffGroup->staves.find(2), staffGroup->staves.end()) << "group contains staff 2";
    EXPECT_NE(staffGroup->staves.find(3), staffGroup->staves.end()) << "group contains staff 3";
    EXPECT_EQ(staffGroup->staves.find(4), staffGroup->staves.end()) << "group does not contain staff 4";
}
