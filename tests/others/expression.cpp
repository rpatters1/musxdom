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

TEST(TextExpressionDef, ValidExpression)
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
      <textFont/>
      <musicFont/>
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
    EXPECT_EQ(expression->horzExprJustification, others::HorizontalTextJustification::Right);  // From XML
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

TEST(TextExpressionDef, InvalidPlaybackType)
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

    ASSERT_TRUE(markingCategory->numberFont);
    EXPECT_EQ(markingCategory->numberFont->fontSize, 0) << "NumberFont should contain nothing";

    // Other properties
    EXPECT_EQ(markingCategory->justification, others::HorizontalTextJustification::Right);  // From XML
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

TEST(TextExpressionDef, EnumDefaults)
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
      <textFont/>
      <musicFont/>
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
    EXPECT_EQ(expression->horzExprJustification, others::HorizontalTextJustification::Left);
    EXPECT_EQ(expression->vertMeasExprAlign, others::VerticalMeasExprAlign::AboveStaff);
}

TEST(MarkingCategory, MissingCategoryType)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <markingsCategory cmper="10">
      <textFont/>
      <musicFont/>
    </markingsCategory>
    <markingsCategoryName cmper="10">
      <name>Vocal Dynamics</name>
    </markingsCategoryName>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
        integrity_error
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
      <v2/>
      <channelSwitch>toL1</channelSwitch>
      <dontScaleWithEntry/>
      <staffGroup>1</staffGroup>
      <staffList>1</staffList>
      <hidden/>
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

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
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
    EXPECT_TRUE(expr1->voice2); // From XML
    EXPECT_TRUE(expr1->dontScaleWithEntry); // From XML
    EXPECT_EQ(expr1->staffGroup, 1);   // From XML
    EXPECT_EQ(expr1->staffList, 1);    // From XML
    EXPECT_TRUE(expr1->hidden); // From XML

    // Check second expression assignment (cmper=2)
    auto expr2 = others->get<others::MeasureExprAssign>(SCORE_PARTID, 2, 0);
    ASSERT_TRUE(expr2) << "MeasureExprAssign with cmper=2 not found but does exist";
    EXPECT_EQ(expr2->shapeExprId, 1);  // From XML
    EXPECT_EQ(expr2->eduPosition, 1024); // From XML
    EXPECT_EQ(expr2->horzEvpuOff, 11);  // From XML
    EXPECT_EQ(expr2->vertEvpuOff, 73);  // From XML
    EXPECT_EQ(expr2->layer, 0);        // From XML
    EXPECT_FALSE(expr2->dontScaleWithEntry); // From XML
    EXPECT_FALSE(expr2->voice2); // From XML
    EXPECT_EQ(expr2->staffAssign, 1);   // From XML
    EXPECT_EQ(expr2->staffGroup, 0);   // From XML
    EXPECT_EQ(expr2->staffList, 0);    // From XML
    EXPECT_FALSE(expr2->hidden); // From XML

    // Ensure that an invalid MeasureExprAssign isn't found
    auto exprInvalid = others->get<others::MeasureExprAssign>(SCORE_PARTID, 3);
    EXPECT_FALSE(exprInvalid) << "MeasureExprAssign with cmper=3 found but does not exist";
}

TEST(ShapeExpressionDef, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <shapeExprDef cmper="4">
      <shapeDef>22</shapeDef>
      <categoryID>12</categoryID>
      <value>121</value>
      <auxdata1>1024</auxdata1>
      <playPass>2</playPass>
      <breakMmRest/>
      <useAuxData/>
      <masterShape/>
      <noPrint/>
      <noHorzStretch/>
      <playType>time</playType>
      <horzMeasExprAlign>centerOverMusic</horzMeasExprAlign>
      <horzExprAlign>center</horzExprAlign>
      <measXAdjust>1</measXAdjust>
      <vertMeasExprAlign>aboveStaffOrEntry</vertMeasExprAlign>
      <yAdjustBaseline>2</yAdjustBaseline>
      <yAdjustEntry>3</yAdjustEntry>
      <descStr>Weird Number</descStr>
    </shapeExprDef>
    <markingsCategory cmper="12">
      <textFont/>
      <musicFont/>
      <categoryType>dynamics</categoryType>
    </markingsCategory>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Others node not found in XML";

    // Check the ShapeExpressionDef with cmper=4
    auto shapeExpr = others->get<others::ShapeExpressionDef>(SCORE_PARTID, 4);
    ASSERT_TRUE(shapeExpr) << "ShapeExpressionDef with cmper=4 not found but does exist";
    EXPECT_EQ(shapeExpr->shapeDef, 22);
    EXPECT_EQ(shapeExpr->categoryId, 12);
    EXPECT_EQ(shapeExpr->value, 121);
    EXPECT_EQ(shapeExpr->auxData1, 1024);
    EXPECT_EQ(shapeExpr->playPass, 2);
    EXPECT_TRUE(shapeExpr->breakMmRest);
    EXPECT_TRUE(shapeExpr->useAuxData);
    EXPECT_TRUE(shapeExpr->masterShape);
    EXPECT_TRUE(shapeExpr->noPrint);
    EXPECT_TRUE(shapeExpr->noHorzStretch);
    EXPECT_EQ(shapeExpr->playbackType, others::PlaybackType::Tempo);
    EXPECT_EQ(shapeExpr->horzMeasExprAlign, others::HorizontalMeasExprAlign::CenterOverMusic);
    EXPECT_EQ(shapeExpr->horzExprJustification, others::HorizontalTextJustification::Center);
    EXPECT_EQ(shapeExpr->measXAdjust, 1);
    EXPECT_EQ(shapeExpr->vertMeasExprAlign, others::VerticalMeasExprAlign::AboveStaffOrEntry);
    EXPECT_EQ(shapeExpr->yAdjustBaseline, 2);
    EXPECT_EQ(shapeExpr->yAdjustEntry, 3);
    EXPECT_EQ(shapeExpr->description, "Weird Number");

    // Ensure that a ShapeExpressionDef with a non-existent cmper isn't found.
    auto exprInvalid = others->get<others::ShapeExpressionDef>(SCORE_PARTID, 3);
    EXPECT_FALSE(exprInvalid) << "ShapeExpressionDef with cmper=3 found but does not exist";

    // Check marking cat
    auto cat = others->get<others::MarkingCategory>(SCORE_PARTID, shapeExpr->categoryId);
    ASSERT_TRUE(cat);
    auto it = cat->shapeExpressions.find(shapeExpr->getCmper());
    ASSERT_NE(it, cat->shapeExpressions.end());
    ASSERT_TRUE(it->second.lock());
}

TEST(StaffListCategory, Populate)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "stafflists.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto names = others->getArray<others::StaffListCategoryName>(SCORE_PARTID);
    ASSERT_GE(names.size(), 3);
    EXPECT_EQ(names[0]->name, "Top Staff");
    EXPECT_EQ(names[1]->name, "Bottom Staff");
    EXPECT_EQ(names[2]->name, "Tempo Staves");

    auto parts = others->getArray<others::StaffListCategoryParts>(SCORE_PARTID);
    ASSERT_EQ(parts.size(), 8) << " there should always be 8 canned staff lists (unless user removed all staff assignments for one)";
    EXPECT_EQ(names[0]->getCmper(), parts[0]->getCmper());
    musxtest::staffListCheck(names[0]->name, parts[0], { -1 });
    EXPECT_EQ(names[1]->getCmper(), parts[1]->getCmper());
    musxtest::staffListCheck(names[1]->name, parts[1], { -2 });
    EXPECT_EQ(names[2]->getCmper(), parts[2]->getCmper());
    musxtest::staffListCheck(names[2]->name, parts[2], { -1 });

    auto score = others->getArray<others::StaffListCategoryScore>(SCORE_PARTID);
    ASSERT_EQ(score.size(), 8) << " there should always be 8 canned staff lists (unless user removed all staff assignments for one)";
    EXPECT_EQ(names[0]->getCmper(), score[0]->getCmper());
    musxtest::staffListCheck(names[0]->name, score[0], { -1 });
    EXPECT_EQ(names[1]->getCmper(), score[1]->getCmper());
    musxtest::staffListCheck(names[1]->name, score[1], { -2 });
    EXPECT_EQ(names[2]->getCmper(), score[2]->getCmper());
    musxtest::staffListCheck(names[2]->name, score[2], { 1, 2, 3 });
}

TEST(ExpressionAssignments, CalcAssociatedEntry)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "exprlayers.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto exprs = others->getArray<others::MeasureExprAssign>(SCORE_PARTID, 1);
    for (const auto& expr : exprs) {
        if (expr->textExprId == 1) { // forte
            auto entryInfo = expr->calcAssociatedEntry();
            ASSERT_TRUE(entryInfo);
            EXPECT_EQ(entryInfo.getLayerIndex(), 0);
            EXPECT_EQ(entryInfo.getIndexInFrame(), 0);
            EXPECT_EQ(entryInfo->elapsedDuration, 0);
        } else if (expr->textExprId == 2) { // piano
            auto entryInfo = expr->calcAssociatedEntry();
            EXPECT_FALSE(entryInfo) << "no Layer 2 entry at 3rd triplet position";
            entryInfo = expr->calcAssociatedEntry(/*findExact*/ false);
            ASSERT_TRUE(entryInfo);
            EXPECT_EQ(entryInfo.getLayerIndex(), 1);
            EXPECT_EQ(entryInfo.getIndexInFrame(), 1);
            EXPECT_EQ(entryInfo->elapsedDuration, musx::util::Fraction(1, 6));
        } else if (expr->textExprId == 3) { // mezzopiano
            auto entryInfo = expr->calcAssociatedEntry();
            ASSERT_TRUE(entryInfo);
            EXPECT_EQ(entryInfo.getLayerIndex(), 1);
            EXPECT_EQ(entryInfo.getIndexInFrame(), 1);
            EXPECT_EQ(entryInfo->elapsedDuration, musx::util::Fraction(1, 6));
        } else if (expr->textExprId == 4) { // pianissimo
            auto entryInfo = expr->calcAssociatedEntry();
            ASSERT_TRUE(entryInfo);
            EXPECT_EQ(entryInfo.getLayerIndex(), 0);
            EXPECT_EQ(entryInfo.getIndexInFrame(), 2);
            EXPECT_EQ(entryInfo->elapsedDuration, musx::util::Fraction(1, 3));
        }
    }
}

TEST(ExpressionAssignments, Voice2Entries)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "exprv2.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto exps = others->getArray<others::MeasureExprAssign>(SCORE_PARTID, 1);
    ASSERT_GE(exps.size(), 2);

    EXPECT_TRUE(exps[0]->voice2);
    EXPECT_FALSE(exps[0]->calcAssociatedEntry());

    EXPECT_FALSE(exps[1]->voice2);
    EXPECT_FALSE(exps[1]->calcAssociatedEntry());
}

TEST(ExpressionAssignments, HiddenByAltNotation)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "expdef.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto exps = others->getArray<others::MeasureExprAssign>(SCORE_PARTID, 1);
    ASSERT_GE(exps.size(), 8);

    int processedExps = 0;
    for (const auto& exp : exps) {
        switch (exp->textExprId) {
        case 1:
            if (exp->staffAssign < 0) {
                EXPECT_EQ(exp->calcAssignedStaffId(), 2);
            } else {
                EXPECT_EQ(exp->calcAssignedStaffId(), 3);
            }
            EXPECT_FALSE(exp->calcIsHiddenByAlternateNotation());
            processedExps++;
            break;
        case 2:
            if (exp->staffAssign == 2) {
                EXPECT_TRUE(exp->calcIsHiddenByAlternateNotation());
            } else {
                EXPECT_FALSE(exp->calcIsHiddenByAlternateNotation());
            }
            processedExps++;
            break;
        case 3:
            if (exp->staffAssign == 2) {
                EXPECT_FALSE(exp->calcIsHiddenByAlternateNotation());
            } else {
                EXPECT_TRUE(exp->calcIsHiddenByAlternateNotation());
            }
            processedExps++;
            break;
        case 4:
            if (exp->staffAssign == 2) {
                EXPECT_TRUE(exp->calcIsHiddenByAlternateNotation());
            } else {
                EXPECT_FALSE(exp->calcIsHiddenByAlternateNotation());
            }
            processedExps++;
            break;
        }
    }
    EXPECT_EQ(processedExps, 8);
}

TEST(ExpressionDefinitions, ExpressionsWithNoCategory)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "exps_orphan.enigmaxml", xml);
    EXPECT_NO_THROW(
        static_cast<void>(musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml));
    ) << "this document contains expressions with no catagory id and should load without errors.";
}
