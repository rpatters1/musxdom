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

TEST(TextExpressionDefTest, ValidExpression)
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
      <playType>midiController</playType>
      <horzMeasExprAlign>leftOfPrimaryNotehead</horzMeasExprAlign>
      <horzExprAlign>right</horzExprAlign>
      <yAdjustEntry>-54</yAdjustEntry>
      <useCategoryFonts/>
      <descStr>fortissimo (velocity = 101)</descStr>
    </textExprDef>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    auto expression = others->get<musx::dom::others::TextExpressionDef>(1);
    EXPECT_FALSE(expression) << "TextExpressionDef with cmper=1 found but does not exist";
    expression = others->get<musx::dom::others::TextExpressionDef>(3);
    ASSERT_TRUE(expression) << "TextExpressionDef with cmper=3 not found but does exist";

    // Check every property
    EXPECT_EQ(expression->textIDKey, 4);  // From XML
    EXPECT_EQ(expression->categoryID, 1);  // From XML
    EXPECT_EQ(expression->value, 101);  // From XML
    EXPECT_EQ(expression->playbackType, musx::dom::others::PlaybackType::MidiController);  // From XML
    EXPECT_EQ(expression->horzMeasExprAlign, musx::dom::others::HorizontalMeasExprAlign::LeftOfPrimaryNotehead);  // From XML
    EXPECT_EQ(expression->horzExprJustification, musx::dom::others::HorizontalExprJustification::Right);  // From XML
    EXPECT_EQ(expression->vertMeasExprAlign, musx::dom::others::VerticalMeasExprAlign::AboveStaff);  // Default
    EXPECT_EQ(expression->measXAdjust, 0);  // Default
    EXPECT_EQ(expression->yAdjustEntry, -54);  // From XML
    EXPECT_EQ(expression->yAdjustBaseline, 0);  // Default
    EXPECT_TRUE(expression->useCategoryFonts);  // From XML
    EXPECT_FALSE(expression->useCategoryPos);  // Default
    EXPECT_FALSE(expression->hasEnclosure);  // Default
    EXPECT_FALSE(expression->breakMmRest);  // Default
    EXPECT_TRUE(expression->useAuxData);  // From XML
    EXPECT_EQ(expression->description, "fortissimo (velocity = 101)");  // From XML
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
        std::invalid_argument
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
    auto markingCategory = others->get<musx::dom::others::MarkingCategory>(10);
    ASSERT_TRUE(markingCategory) << "MarkingCategory with cmper=10 not found but does exist";

    // Check every property of MarkingCategory
    EXPECT_EQ(markingCategory->categoryType, musx::dom::others::MarkingCategory::CategoryType::Dynamics);  // From XML

    // textFont properties
    ASSERT_TRUE(markingCategory->textFont) << "TextFont is missing but exists in XML";
    EXPECT_EQ(markingCategory->textFont->fontId, 9);  // From XML
    EXPECT_EQ(markingCategory->textFont->fontSize, 14);  // From XML
    EXPECT_TRUE(markingCategory->textFont->italic()) << "Italic effect expected but not found";  // From XML

    // musicFont properties
    ASSERT_TRUE(markingCategory->musicFont) << "MusicFont is missing but exists in XML";
    EXPECT_EQ(markingCategory->musicFont->fontId, 0);  // Default value as fontID is missing
    EXPECT_EQ(markingCategory->musicFont->fontSize, 24);  // From XML
    EXPECT_EQ(markingCategory->musicFont->fontEfx, 0);

    // numberFont should be nullptr because it has no subnodes
    EXPECT_EQ(markingCategory->numberFont, nullptr) << "NumberFont should be nullptr but is not";

    // Other properties
    EXPECT_EQ(markingCategory->justification, musx::dom::others::HorizontalExprJustification::Right);  // From XML
    EXPECT_EQ(markingCategory->horzAlign, musx::dom::others::HorizontalMeasExprAlign::LeftOfAllNoteheads);  // From XML
    EXPECT_EQ(markingCategory->horzOffset, 12);  // From XML
    EXPECT_EQ(markingCategory->vertAlign, musx::dom::others::VerticalMeasExprAlign::AboveStaffOrEntry);  // From XML
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
    auto markingCategoryName = others->get<musx::dom::others::MarkingCategoryName>(10);
    ASSERT_TRUE(markingCategoryName) << "MarkingCategoryName with cmper=10 not found but does exist";
    EXPECT_EQ(markingCategoryName->name, "Vocal Dynamics");

    // Check the name using getName() method from MarkingCategory
    EXPECT_EQ(markingCategory->getName(), markingCategoryName->name);  // Should match the name from MarkingCategoryName
}
