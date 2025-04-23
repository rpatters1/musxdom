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
    <staffStyle cmper="1">
      <customStaff>
        <staffLine>13</staffLine>
        <staffLine>11</staffLine>
      </customStaff>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
      <showNameParts/>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-4</stemReversal>
      <fullName>12</fullName>
      <abbrvName>13</abbrvName>
      <botRepeatDotOff>-5</botRepeatDotOff>
      <topRepeatDotOff>-3</topRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
      <stemDir>alwaysUp</stemDir>
      <styleName>Names and Stems</styleName>
      <addToMenu/>
      <mask>
        <defaultClef/>
        <floatKeys/>
        <floatTime/>
        <staffType/>
        <negNameScore/>
        <fullName/>
        <abrvName/>
        <showStems/>
        <showNameParts/>
      </mask>
    </staffStyle>
    <staffStyleAssign cmper="6" inci="0">
      <style>1</style>
      <startMeas>1</startMeas>
      <endEdu>2147483647</endEdu>
      <startEdu>512</startEdu>
      <endMeas>4</endMeas>
    </staffStyleAssign>
  </others>
</finale>
)xml";

TEST(StaffStyleTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Retrieve StaffStyle with cmper=1
    auto staffStyle = others->get<others::StaffStyle>(SCORE_PARTID, 1);
    ASSERT_TRUE(staffStyle) << "StaffStyle with cmper 1 not found";

    EXPECT_FALSE(staffStyle->staffLines);
    ASSERT_TRUE(staffStyle->customStaff);
    ASSERT_EQ(staffStyle->customStaff.value().size(), 2);
    EXPECT_EQ(staffStyle->customStaff.value()[0], 11) << "factory did not sort customStaff elements";
    EXPECT_EQ(staffStyle->customStaff.value()[1], 13) << "factory did not sort customStaff elements";
    EXPECT_EQ(staffStyle->lineSpace, 24);
    EXPECT_EQ(staffStyle->instUuid, "54422b22-4627-4100-abbf-064eedc15fe3");
    EXPECT_TRUE(staffStyle->showNameInParts);
    EXPECT_EQ(staffStyle->dwRestOffset, -4);
    EXPECT_EQ(staffStyle->wRestOffset, -4);
    EXPECT_EQ(staffStyle->hRestOffset, -4);
    EXPECT_EQ(staffStyle->otherRestOffset, -4);
    EXPECT_EQ(staffStyle->stemReversal, -4);
    EXPECT_EQ(staffStyle->fullNameTextId, 12);
    EXPECT_EQ(staffStyle->abbrvNameTextId, 13);
    EXPECT_EQ(staffStyle->botRepeatDotOff, -5);
    EXPECT_EQ(staffStyle->topRepeatDotOff, -3);
    EXPECT_EQ(staffStyle->vertTabNumOff, -1024);
    EXPECT_EQ(staffStyle->stemDirection, others::Staff::StemDirection::AlwaysUp);
    EXPECT_EQ(staffStyle->styleName, "Names and Stems");
    EXPECT_TRUE(staffStyle->addToMenu);

    // Verify mask properties
    ASSERT_TRUE(staffStyle->masks) << "Masks object is missing in StaffStyle";
    EXPECT_TRUE(staffStyle->masks->defaultClef);
    EXPECT_TRUE(staffStyle->masks->floatKeys);
    EXPECT_TRUE(staffStyle->masks->floatTime);
    EXPECT_TRUE(staffStyle->masks->staffType);
    EXPECT_TRUE(staffStyle->masks->negNameScore);
    EXPECT_TRUE(staffStyle->masks->fullName);
    EXPECT_TRUE(staffStyle->masks->abrvName);
    EXPECT_TRUE(staffStyle->masks->showStems);
    EXPECT_TRUE(staffStyle->masks->showNameParts);
}

TEST(StaffStyleAssignTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Retrieve StaffStyleAssign with cmper=6
    auto staffStyleAssign = others->get<others::StaffStyleAssign>(SCORE_PARTID, 6, 0);
    ASSERT_TRUE(staffStyleAssign) << "StaffStyleAssign with cmper 6 not found";

    EXPECT_EQ(staffStyleAssign->styleId, 1);
    EXPECT_EQ(staffStyleAssign->startMeas, 1);
    EXPECT_EQ(staffStyleAssign->startEdu, 512);
    EXPECT_EQ(staffStyleAssign->endMeas, 4);
    EXPECT_EQ(staffStyleAssign->endEdu, (std::numeric_limits<Edu>::max)());
}

TEST(StaffStyleInstrument, DetectInstrumentChange)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "inst_change.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto staff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 1, 0);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->instUuid, uuid::Oboe);
    }

    {
        auto staff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 2, 0);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->instUuid, uuid::EnglishHorn);
    }

    {
        auto staff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 3, 0);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->instUuid, uuid::EnglishHorn);
    }
}
