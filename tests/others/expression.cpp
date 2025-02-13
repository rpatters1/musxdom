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

TEST(TextExpressionDefTest, ValidExpression)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textExprDef cmper="3">
      <textIDKey>4</textIDKey>
      <categoryID>1</categoryID>
      <rehearsalMarkStyle>measNum</rehearsalMarkStyle>
      <value>101</value>
      <playPass>2</playPass>
      <hideMeasureNum/>
      <useAuxData/>
      <playType> midiController</playType>
      <horzMeasExprAlign>leftOfPrimaryNotehead</horzMeasExprAlign>
      <horzExprAlign>right</horzExprAlign>
      <yAdjustEntry>-54</yAdjustEntry>
      <useCategoryFonts/>
      <descStr>fortissimo (velocity = 101)</descStr>
    </textExprDef>
    <markingsCategory cmper="1">
      <categoryType>dynamics</categoryType>
    </markingsCategory>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    auto expression = others->get<others::TextExpressionDef>(SCORE_PARTID, 1);
    EXPECT_FALSE(expression) << "TextExpressionDef with cmper=1 found but does not exist";
    expression = others->get<others::TextExpressionDef>(SCORE_PARTID, 3);
    ASSERT_TRUE(expression) << "TextExpressionDef with cmper=3 not found but does exist";

    // Check every property
    EXPECT_EQ(expression->textIdKey, 4);  // From XML
    EXPECT_EQ(expression->categoryId, 1);  // From XML
    EXPECT_EQ(expression->rehearsalMarkStyle, others::RehearsalMarkStyle::MeasureNumber);  // From XML
    EXPECT_EQ(expression->value, 101);  // From XML
    EXPECT_EQ(expression->playPass, 2);  // From XML
    EXPECT_TRUE(expression->hideMeasureNum);  // From XML
    EXPECT_TRUE(expression->useAuxData);  // From XML
    EXPECT_FALSE(expression->hasEnclosure);  // Default
    EXPECT_FALSE(expression->breakMmRest);  // Default
    EXPECT_EQ(expression->playbackType, others::PlaybackType::MidiController);  // From XML
    EXPECT_EQ(expression->horzMeasExprAlign, others::HorizontalMeasExprAlign::LeftOfPrimaryNotehead);  // From XML
    EXPECT_EQ(expression->horzExprJustification, others::HorizontalExprJustification::Right);  // From XML
    EXPECT_EQ(expression->vertMeasExprAlign, others::VerticalMeasExprAlign::AboveStaff);  // Default
    EXPECT_EQ(expression->measXAdjust, 0);  // Default
    EXPECT_EQ(expression->yAdjustEntry, -54);  // From XML
    EXPECT_EQ(expression->yAdjustBaseline, 0);  // Default
    EXPECT_TRUE(expression->useCategoryFonts);  // From XML
    EXPECT_FALSE(expression->useCategoryPos);  // Default
    EXPECT_EQ(expression->description, "fortissimo (velocity = 101)");  // From XML

    // Check marking cat
    auto cat = others->get<others::MarkingCategory>(SCORE_PARTID, expression->categoryId);
    ASSERT_TRUE(cat);
    auto it = cat->textExpressions.find(expression->getCmper());
    ASSERT_NE(it, cat->textExpressions.end());
    ASSERT_TRUE(it->second.lock());
}

TEST(TextExpressionDefTest, InvalidPlaybackType)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textExprDef cmper="3">
      <textIDKey>4</textIDKey>
      <playType>invalidType</playType>
    </textExprDef>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
      auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml),
      musx::factory::unknown_xml_error
    );
}

TEST(MarkingCategoryTest, ValidMarkingCategory)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <markingsCategory cmper="10">
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
      <justification>right</justification>
      <horzAlign>leftOfAllNoteheads</horzAlign>
      <horzOffset>12</horzOffset>
      <vertAlign>aboveStaffOrEntry</vertAlign>
      <vertOffsetEntry>36</vertOffsetEntry>
      <vertOffsetBaseline>36</vertOffsetBaseline>
      <usesTextFont/>
      <usesMusicFont/>
      <usesPositioning/>
      <userCreated/>
      <staffList>1</staffList>
    </markingsCategory>
    <markingsCategoryName cmper="10">
      <name>Vocal Dynamics</name>
    </markingsCategoryName>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    // Get MarkingCategory with cmper=10
    auto markingCategory = others->get<others::MarkingCategory>(SCORE_PARTID, 10);
    ASSERT_TRUE(markingCategory) << "MarkingCategory with cmper=10 not found but does exist";

    // Check every property of MarkingCategory
    EXPECT_EQ(markingCategory->categoryType, others::MarkingCategory::CategoryType::Dynamics);  // From XML

    // textFont properties
    ASSERT_TRUE(markingCategory->textFont) << "TextFont is missing but exists in XML";
    EXPECT_EQ(markingCategory->textFont->fontId, 9);  // From XML
    EXPECT_EQ(markingCategory->textFont->fontSize, 14);  // From XML
    EXPECT_FALSE(markingCategory->textFont->bold);
    EXPECT_TRUE(markingCategory->textFont->italic) << "Italic effect expected but not found";
    EXPECT_FALSE(markingCategory->textFont->underline);
    EXPECT_FALSE(markingCategory->textFont->strikeout);
    EXPECT_FALSE(markingCategory->textFont->absolute);
    EXPECT_FALSE(markingCategory->textFont->hidden);

    // musicFont properties
    ASSERT_TRUE(markingCategory->musicFont) << "MusicFont is missing but exists in XML";
    EXPECT_EQ(markingCategory->musicFont->fontId, 0);  // Default value as fontID is missing
    EXPECT_EQ(markingCategory->musicFont->fontSize, 24);  // From XML
    EXPECT_FALSE(markingCategory->musicFont->bold);
    EXPECT_FALSE(markingCategory->musicFont->italic);
    EXPECT_FALSE(markingCategory->musicFont->underline);
    EXPECT_FALSE(markingCategory->musicFont->strikeout);
    EXPECT_FALSE(markingCategory->musicFont->absolute);
    EXPECT_FALSE(markingCategory->musicFont->hidden);

    // numberFont should be nullptr because it has no subnodes
    EXPECT_EQ(markingCategory->numberFont, nullptr) << "NumberFont should be nullptr but is not";

    // Other properties
    EXPECT_EQ(markingCategory->justification, others::HorizontalExprJustification::Right);  // From XML
    EXPECT_EQ(markingCategory->horzAlign, others::HorizontalMeasExprAlign::LeftOfAllNoteheads);  // From XML
    EXPECT_EQ(markingCategory->horzOffset, 12);  // From XML
    EXPECT_EQ(markingCategory->vertAlign, others::VerticalMeasExprAlign::AboveStaffOrEntry);  // From XML
    EXPECT_EQ(markingCategory->vertOffsetEntry, 36);  // From XML
    EXPECT_EQ(markingCategory->vertOffsetBaseline, 36);  // From XML

    EXPECT_TRUE(markingCategory->usesTextFont);  // From XML
    EXPECT_TRUE(markingCategory->usesMusicFont);  // From XML
    EXPECT_TRUE(markingCategory->usesPositioning);  // From XML
    EXPECT_FALSE(markingCategory->usesNumberFont);  // Default
    EXPECT_FALSE(markingCategory->usesStaffList);  // Default
    EXPECT_FALSE(markingCategory->usesBreakMmRests);  // Default
    EXPECT_FALSE(markingCategory->breakMmRest);  // Default
    EXPECT_TRUE(markingCategory->userCreated);  // From XML

    EXPECT_EQ(markingCategory->staffList, 1);  // From XML

    // Get MarkingCategoryName with cmper=10
    auto markingCategoryName = others->get<others::MarkingCategoryName>(SCORE_PARTID, 10);
    ASSERT_TRUE(markingCategoryName) << "MarkingCategoryName with cmper=10 not found but does exist";
    EXPECT_EQ(markingCategoryName->name, "Vocal Dynamics");

    // Check the name using getName() method from MarkingCategory
    EXPECT_EQ(markingCategory->getName(), markingCategoryName->name);  // Should match the name from MarkingCategoryName
}

TEST(TextExpressionDefTest, EnumDefaults)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textExprDef cmper="3">
      <textIDKey>4</textIDKey>
      <categoryID>1</categoryID>
      <value>101</value>
      <useAuxData/>
      <useCategoryFonts/>
      <descStr>fortissimo (velocity = 101)</descStr>
    </textExprDef>
    <markingsCategory cmper="1">
      <categoryType>dynamics</categoryType>
    </markingsCategory>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    auto expression = others->get<others::TextExpressionDef>(SCORE_PARTID, 3);
    ASSERT_TRUE(expression) << "TextExpressionDef with cmper=3 not found but does exist";

    EXPECT_EQ(expression->rehearsalMarkStyle, others::RehearsalMarkStyle::None);
    EXPECT_EQ(expression->playbackType, others::PlaybackType::None);
    EXPECT_EQ(expression->horzMeasExprAlign, others::HorizontalMeasExprAlign::LeftBarline);
    EXPECT_EQ(expression->horzExprJustification, others::HorizontalExprJustification::Left);
    EXPECT_EQ(expression->vertMeasExprAlign, others::VerticalMeasExprAlign::AboveStaff);
}

TEST(MarkingCategoryTest, MissingCategoryType)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <markingsCategory cmper="10"/>
    <markingsCategoryName cmper="10">
      <name>Vocal Dynamics</name>
    </markingsCategoryName>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
        std::invalid_argument
    );
}

TEST(MeasureExprAssign, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <measExprAssign cmper="1" inci="0">
      <textExprID>24</textExprID>
      <horzEvpuOff>1</horzEvpuOff>
      <vertOff>2</vertOff>
      <staffAssign>-1</staffAssign>
      <layer>1</layer>
      <channelSwitch>toL1</channelSwitch>
      <dontScaleWithEntry/>
      <staffGroup>1</staffGroup>
      <staffList>1</staffList>
    </measExprAssign>
    <measExprAssign cmper="2" inci="0">
      <shapeExprID>1</shapeExprID>
      <horzEduOff>1024</horzEduOff>
      <horzEvpuOff>11</horzEvpuOff>
      <vertOff>73</vertOff>
      <staffAssign>1</staffAssign>
    </measExprAssign>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    // Check first expression assignment (cmper=1)
    auto expr1 = others->get<others::MeasureExprAssign>(SCORE_PARTID, 1, 0);
    ASSERT_TRUE(expr1) << "MeasureExprAssign with cmper=1 not found but does exist";
    EXPECT_EQ(expr1->textExprId, 24);  // From XML
    EXPECT_EQ(expr1->horzEvpuOff, 1);  // From XML
    EXPECT_EQ(expr1->vertEvpuOff, 2);  // From XML
    EXPECT_EQ(expr1->staffAssign, -1); // From XML
    EXPECT_EQ(expr1->layer, 1);        // From XML
    EXPECT_TRUE(expr1->dontScaleWithEntry); // From XML
    EXPECT_EQ(expr1->staffGroup, 1);   // From XML
    EXPECT_EQ(expr1->staffList, 1);    // From XML

    // Check second expression assignment (cmper=2)
    auto expr2 = others->get<others::MeasureExprAssign>(SCORE_PARTID, 2, 0);
    ASSERT_TRUE(expr2) << "MeasureExprAssign with cmper=2 not found but does exist";
    EXPECT_EQ(expr2->shapeExprId, 1);  // From XML
    EXPECT_EQ(expr2->eduPosition, 1024); // From XML
    EXPECT_EQ(expr2->horzEvpuOff, 11);  // From XML
    EXPECT_EQ(expr2->vertEvpuOff, 73);  // From XML
    EXPECT_EQ(expr2->layer, 0);        // From XML
    EXPECT_FALSE(expr2->dontScaleWithEntry); // From XML
    EXPECT_EQ(expr2->staffAssign, 1);   // From XML
    EXPECT_EQ(expr2->staffGroup, 0);   // From XML
    EXPECT_EQ(expr2->staffList, 0);    // From XML

    // Ensure that an invalid MeasureExprAssign isn't found
    auto exprInvalid = others->get<others::MeasureExprAssign>(SCORE_PARTID, 3);
    EXPECT_FALSE(exprInvalid) << "MeasureExprAssign with cmper=3 found but does not exist";
}
