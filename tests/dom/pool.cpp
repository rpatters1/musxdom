/*merged/
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

constexpr static musxtest::string_view poolTestXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textExprDef cmper="1">
      <textIDKey>2</textIDKey>
      <categoryID>1</categoryID>
      <value>127</value>
      <horzMeasExprAlign>manual</horzMeasExprAlign>
      <vertMeasExprAlign>manual</vertMeasExprAlign>
      <yAdjustEntry>-54</yAdjustEntry>
      <useCategoryFonts/>
      <descStr>fortissississimo (velocity = 127)</descStr>
    </textExprDef>
    <textExprDef cmper="2">
      <textIDKey>3</textIDKey>
      <categoryID>1</categoryID>
      <value>114</value>
      <auxdata1>1024</auxdata1>
      <useAuxData/>
      <playType>time</playType>
      <horzMeasExprAlign>leftOfAllNoteheads</horzMeasExprAlign>
      <horzExprAlign>center</horzExprAlign>
      <vertMeasExprAlign>refLine</vertMeasExprAlign>
      <yAdjustEntry>-54</yAdjustEntry>
      <useCategoryFonts/>
      <descStr>fortississimo (velocity = 114)</descStr>
    </textExprDef>
    <textExprDef cmper="3">
      <textIDKey>4</textIDKey>
      <categoryID>1</categoryID>
      <value>101</value>
      <useAuxData/>
      <playType>midiController</playType>
      <horzMeasExprAlign>leftOfPrimaryNotehead</horzMeasExprAlign>
      <horzExprAlign>right</horzExprAlign>
      <yAdjustEntry>-54</yAdjustEntry>
      <useCategoryFonts/>
      <descStr>fortissimo (velocity = 101)</descStr>
    </textExprDef>
    <markingsCategory cmper="1">
      <categoryType>dynamics</categoryType>
      <textFont>
        <fontID>9</fontID>
        <fontSize>14</fontSize>
        <efx>
          <italic/>
        </efx>
      </textFont>
      <musicFont>
        <fontSize>24</fontSize>
      </musicFont>
      <numberFont/>
      <justification>center</justification>
      <horzAlign>centerPrimaryNotehead</horzAlign>
      <vertAlign>belowStaffOrEntry</vertAlign>
      <vertOffsetEntry>-54</vertOffsetEntry>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="2">
      <categoryType>tempoMarks</categoryType>
      <textFont>
        <fontID>14</fontID>
        <fontSize>14</fontSize>
      </textFont>
      <musicFont>
        <fontID>14</fontID>
        <fontSize>14</fontSize>
      </musicFont>
      <numberFont>
        <fontID>17</fontID>
        <fontSize>12</fontSize>
      </numberFont>
      <horzAlign>startTimeSig</horzAlign>
      <usesTextFont/>
      <usesMusicFont/>
      <usesNumberFont/>
      <usesPositioning/>
      <usesStaffList/>
      <usesBreakMmRests/>
      <breakMmRest/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="3">
      <categoryType>tempoAlts</categoryType>
      <textFont>
        <fontID>1</fontID>
        <fontSize>14</fontSize>
        <efx>
          <italic/>
        </efx>
      </textFont>
      <musicFont>
        <fontSize>12</fontSize>
      </musicFont>
      <numberFont/>
      <horzAlign>manual</horzAlign>
      <vertOffsetEntry>36</vertOffsetEntry>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <usesStaffList/>
      <usesBreakMmRests/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="4">
      <categoryType>expressiveText</categoryType>
      <textFont>
        <fontID>1</fontID>
        <fontSize>14</fontSize>
        <efx>
          <italic/>
        </efx>
      </textFont>
      <musicFont>
        <fontSize>24</fontSize>
      </musicFont>
      <numberFont/>
      <horzAlign>leftOfAllNoteheads</horzAlign>
      <vertAlign>belowStaffOrEntry</vertAlign>
      <vertOffsetEntry>-54</vertOffsetEntry>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="5">
      <categoryType>techniqueText</categoryType>
      <textFont>
        <fontID>1</fontID>
        <fontSize>14</fontSize>
        <efx>
          <italic/>
        </efx>
      </textFont>
      <musicFont>
        <fontSize>24</fontSize>
      </musicFont>
      <numberFont/>
      <horzAlign>leftOfAllNoteheads</horzAlign>
      <vertAlign>manual</vertAlign>
      <vertOffsetEntry>36</vertOffsetEntry>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="6">
      <categoryType>rehearsalMarks</categoryType>
      <textFont>
        <fontID>18</fontID>
        <fontSize>30</fontSize>
        <efx>
          <bold/>
        </efx>
      </textFont>
      <musicFont>
        <fontID>17</fontID>
        <fontSize>12</fontSize>
      </musicFont>
      <numberFont/>
      <justification>center</justification>
      <vertAlign>refLine</vertAlign>
      <vertOffsetBaseline>24</vertOffsetBaseline>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <usesStaffList/>
      <usesBreakMmRests/>
      <breakMmRest/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategory cmper="7">
      <categoryType>misc</categoryType>
      <textFont/>
      <musicFont/>
      <numberFont/>
      <horzAlign>manual</horzAlign>
      <vertAlign>manual</vertAlign>
      <staffList>1</staffList>
    </markingsCategory>
  </others>
</finale>
    )xml";

using namespace musx::dom;

TEST(PoolTest, Scalar)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(poolTestXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    EXPECT_TRUE(others->get<others::TextExpressionDef>(SCORE_PARTID, 3));
    EXPECT_FALSE(others->get<others::TextExpressionDef>(SCORE_PARTID, 4));
    EXPECT_TRUE(others->get<others::MarkingCategory>(SCORE_PARTID, 3));
    EXPECT_FALSE(others->get<others::MarkingCategory>(SCORE_PARTID, 8));
}

TEST(PoolTest, Vector)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(poolTestXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    EXPECT_EQ(others->getArray<others::MarkingCategory>(SCORE_PARTID).size(), 7);
    EXPECT_EQ(others->getArray<others::MarkingCategory>(SCORE_PARTID, 6).size(), 1);
    EXPECT_EQ(others->getArray<others::MarkingCategory>(SCORE_PARTID, 5)[0]->categoryType, others::MarkingCategory::CategoryType::TechniqueText);
    EXPECT_EQ(others->getArray<others::MarkingCategory>(SCORE_PARTID, 8).size(), 0);
    EXPECT_EQ(others->getArray<others::TextExpressionDef>(SCORE_PARTID).size(), 3);
    EXPECT_EQ(others->getArray<others::TextExpressionDef>(SCORE_PARTID, 2).size(), 1);
    EXPECT_EQ(others->getArray<others::TextExpressionDef>(SCORE_PARTID, 1)[0]->description, "fortissississimo (velocity = 127)");
    EXPECT_EQ(others->getArray<others::TextExpressionDef>(SCORE_PARTID, 4).size(), 0);
}

TEST(PoolTest, PhysicalSourcesAcrossParts)
{
    constexpr Cmper pageId = 2;
    constexpr Cmper orphanPartId = 22;
    constexpr Inci firstIncidence = 0;
    constexpr Inci secondIncidence = 1;
    constexpr StaffCmper staffId = 1;
    constexpr MeasCmper measureId = 3;

    auto session = musx::factory::DocumentFactory::begin();
    const auto document = session.getDocument();
    auto scorePage = std::make_shared<others::PageGraphicAssign>(
        document, SCORE_PARTID, EnigmaBase::ShareMode::All, pageId, firstIncidence);
    auto orphanPage = std::make_shared<others::PageGraphicAssign>(
        document, orphanPartId, EnigmaBase::ShareMode::Partial, pageId, firstIncidence);
    auto secondOrphanPage = std::make_shared<others::PageGraphicAssign>(
        document, orphanPartId, EnigmaBase::ShareMode::Partial, pageId, secondIncidence);
    document->getOthers()->add(others::PageGraphicAssign::XmlNodeName, scorePage);
    document->getOthers()->add(others::PageGraphicAssign::XmlNodeName, orphanPage);
    document->getOthers()->add(others::PageGraphicAssign::XmlNodeName, secondOrphanPage);

    const auto allPages = document->getOthers()->getAllSources<others::PageGraphicAssign>();
    ASSERT_EQ(allPages.size(), 3);
    EXPECT_EQ(allPages[0]->getSourcePartId(), SCORE_PARTID);
    EXPECT_EQ(allPages[1]->getSourcePartId(), orphanPartId);
    EXPECT_EQ(allPages[2]->getInci(), secondIncidence);
    const auto selectedPages = document->getOthers()->getAllSources<others::PageGraphicAssign>(
        pageId, firstIncidence);
    ASSERT_EQ(selectedPages.size(), 2);
    EXPECT_EQ(selectedPages[1].get(), orphanPage.get());

    auto scoreDetail = std::make_shared<details::GFrameHold>(
        document, SCORE_PARTID, EnigmaBase::ShareMode::All, staffId, measureId);
    auto orphanDetail = std::make_shared<details::GFrameHold>(
        document, orphanPartId, EnigmaBase::ShareMode::Partial, staffId, measureId);
    document->getDetails()->add(details::GFrameHold::XmlNodeName, scoreDetail);
    document->getDetails()->add(details::GFrameHold::XmlNodeName, orphanDetail);

    const auto allDetails = document->getDetails()->getAllSources<details::GFrameHold>();
    ASSERT_EQ(allDetails.size(), 2);
    EXPECT_EQ(allDetails[1]->getSourcePartId(), orphanPartId);
    const auto selectedDetails = document->getDetails()->getAllSources<details::GFrameHold>(
        staffId, measureId);
    ASSERT_EQ(selectedDetails.size(), 2);
    EXPECT_EQ(selectedDetails[1].get(), orphanDetail.get());
}
