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

constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <instUsed cmper="0" inci="0">
      <inst>15</inst>
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
      <inst>16</inst>
      <trackType>staff</trackType>
      <distFromTop>-288</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="2">
      <inst>11</inst>
      <trackType>staff</trackType>
      <distFromTop>-288</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <staffSpec cmper="11">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
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
      <useAutoNum/>
    </staffSpec>
    <staffSpec cmper="15">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
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
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
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
      <autoNum>arabicPrefix</autoNum>
      <useAutoNum/>
    </staffSpec>
  </others>
</finale>
    )xml";

TEST(StaffTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staff1 = others->get<others::Staff>(SCORE_PARTID, 15);
    ASSERT_TRUE(staff1) << "Staff with cmper 15 not found";

    EXPECT_EQ(staff1->staffLines, 5);
    EXPECT_EQ(staff1->lineSpace, 24);
    EXPECT_EQ(staff1->instUuid, "54422b22-4627-4100-abbf-064eedc15fe3");
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
    EXPECT_EQ(staff1->autoNumbering, others::Staff::AutoNumberingStyle::ArabicSuffix);
    EXPECT_EQ(staff1->useAutoNumbering, false);

    auto staff2 = others->get<others::Staff>(SCORE_PARTID, 16);
    ASSERT_TRUE(staff2) << "Staff with cmper 16 not found";

    EXPECT_EQ(staff2->staffLines, 5);
    EXPECT_EQ(staff2->lineSpace, 24);
    EXPECT_EQ(staff1->instUuid, "54422b22-4627-4100-abbf-064eedc15fe3");
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
    EXPECT_EQ(staff2->autoNumbering, others::Staff::AutoNumberingStyle::ArabicPrefix);
    EXPECT_EQ(staff2->useAutoNumbering, true);
}

TEST(StaffTest, AutoNumbering)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staff1 = others->get<others::Staff>(SCORE_PARTID, 15);
    ASSERT_TRUE(staff1) << "Staff with cmper 15 not found";

    EXPECT_FALSE(staff1->getAutoNumberValue().has_value());
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name");
    staff1->useAutoNumbering = true;
    EXPECT_EQ(staff1->getAutoNumberValue().value_or(-1), 1);
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name 1");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name I");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "1st Name");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name A");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::ArabicPrefix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "1. Name");

    auto staff2 = others->get<others::Staff>(SCORE_PARTID, 16);
    ASSERT_TRUE(staff2) << "Staff with cmper 16 not found";

    EXPECT_EQ(staff2->getAutoNumberValue().value_or(-1), 2);
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "2. Name");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name II");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "2nd Name");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name B");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::ArabicSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name 2");

    auto staff3 = others->get<others::Staff>(SCORE_PARTID, 11);
    ASSERT_TRUE(staff3) << "Staff with cmper 11 not found";

    EXPECT_EQ(staff3->getAutoNumberValue().value_or(-1), 3);
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name 3");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name III");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "3rd Name");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name C");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::ArabicPrefix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "3. Name");
}
