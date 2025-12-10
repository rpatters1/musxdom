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

 TEST(RepeatBack, Populate)
 {
     constexpr static musxtest::string_view xml = R"xml(
     <?xml version="1.0" encoding="UTF-8"?>
     <finale>
         <others>
             <repeatBack cmper="6">
                 <actuate>2</actuate>
                 <target>-2</target>
                 <pos1>-35</pos1>
                 <indivPlac/>
                 <topStaffOnly/>
                 <hidden/>
                 <clrOnChange/>
                 <action>jumpRelative</action>
                 <trigger>onPass</trigger>
                 <staffList>1</staffList>
                 <pos2>81</pos2>
                 <line2>-23</line2>
             </repeatBack>
         </others>
     </finale>
     )xml";

     auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
     auto others = doc->getOthers();
     ASSERT_TRUE(others);

     auto repeatBack = others->get<others::RepeatBack>(SCORE_PARTID, 6);
     ASSERT_TRUE(repeatBack) << "RepeatBack with cmper 6 not found";

     EXPECT_EQ(repeatBack->passNumber, 2);
     EXPECT_EQ(repeatBack->targetValue, -2);
     EXPECT_EQ(repeatBack->leftHPos, -35);
     EXPECT_TRUE(repeatBack->individualPlacement);
     EXPECT_TRUE(repeatBack->topStaffOnly);
     EXPECT_TRUE(repeatBack->hidden);
     EXPECT_TRUE(repeatBack->resetOnAction);
     EXPECT_EQ(repeatBack->jumpAction, others::RepeatActionType::JumpRelative);
     EXPECT_EQ(repeatBack->trigger, others::RepeatTriggerType::OnPass);
     EXPECT_EQ(repeatBack->staffList, 1);
     EXPECT_EQ(repeatBack->rightHPos, 81);
     EXPECT_EQ(repeatBack->rightVPos, -23);
}

TEST(RepeatEndingStart, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
            <repeatEndingStart cmper="4">
                <nextEnd>12</nextEnd>
                <textPos>26</textPos>
                <pos1>39</pos1>
                <indivPlac/>
                <topStaffOnly/>
                <hidden/>
                <action>jumpAbsolute</action>
                <trigger>onPass</trigger>
                <jmpIgnore/>
                <endLine>-13</endLine>
                <textLine>24</textLine>
            </repeatEndingStart>
            <repeatEndingText cmper="4">
                <rptText>to continue</rptText>
            </repeatEndingText>
            <repeatPassList cmper="4">
                <act>2</act>
                <act>4</act>
                <act>7</act>
            </repeatPassList>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Test RepeatEndingStart
    auto repeatEndingStart = others->get<others::RepeatEndingStart>(SCORE_PARTID, 4);
    ASSERT_TRUE(repeatEndingStart) << "RepeatEndingStart with cmper 4 not found";

    EXPECT_EQ(repeatEndingStart->targetValue, 12);
    EXPECT_EQ(repeatEndingStart->textHPos, 26);
    EXPECT_EQ(repeatEndingStart->leftHPos, 39);
    EXPECT_TRUE(repeatEndingStart->individualPlacement);
    EXPECT_TRUE(repeatEndingStart->topStaffOnly);
    EXPECT_TRUE(repeatEndingStart->hidden);
    EXPECT_EQ(repeatEndingStart->jumpAction, others::RepeatActionType::JumpAbsolute);
    EXPECT_EQ(repeatEndingStart->trigger, others::RepeatTriggerType::OnPass);
    EXPECT_TRUE(repeatEndingStart->jumpIfIgnoring);
    EXPECT_EQ(repeatEndingStart->endLineVPos, -13);
    EXPECT_EQ(repeatEndingStart->textVPos, 24);

    // Test RepeatEndingText
    auto repeatEndingText = others->get<others::RepeatEndingText>(SCORE_PARTID, 4);
    ASSERT_TRUE(repeatEndingText) << "RepeatEndingText with cmper 4 not found";

    EXPECT_EQ(repeatEndingText->text, "to continue");

    // Test RepeatPassList
    auto repeatPassList = others->get<others::RepeatPassList>(SCORE_PARTID, 4);
    ASSERT_TRUE(repeatPassList) << "RepeatPassList with cmper 4 not found";

    ASSERT_EQ(repeatPassList->values.size(), 3);
    EXPECT_EQ(repeatPassList->values[0], 2);
    EXPECT_EQ(repeatPassList->values[1], 4);
    EXPECT_EQ(repeatPassList->values[2], 7);
}

TEST(TextRepeatDef, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
            <textRepeatDef cmper="1">
                <fontID>11</fontID>
                <fontSize>24</fontSize>
                <efx>
                    <bold/>
                    <italic/>
                </efx>
                <poundReplace>repeatID</poundReplace>
                <useThisFont/>
                <justify>right</justify>
            </textRepeatDef>
            <textRepeatDef cmper="2">
                <poundReplace>repeatID</poundReplace>
                <justify>center</justify>
            </textRepeatDef>
            <textRepeatText cmper="1">
                <rptText>Segno</rptText>
            </textRepeatText>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others) << "Failed to retrieve 'others' from document";

    // Test TextRepeatDef
    auto textRepeatDef = others->get<others::TextRepeatDef>(SCORE_PARTID, 1);
    ASSERT_TRUE(textRepeatDef) << "TextRepeatDef with cmper 1 not found";

    ASSERT_TRUE(textRepeatDef->font) << "FontInfo was not populated";
    EXPECT_EQ(textRepeatDef->font->fontId, 11);
    EXPECT_EQ(textRepeatDef->font->fontSize, 24);
    EXPECT_TRUE(textRepeatDef->font->bold);
    EXPECT_TRUE(textRepeatDef->font->italic);

    EXPECT_EQ(textRepeatDef->poundReplace, others::TextRepeatDef::PoundReplaceOption::RepeatID);
    EXPECT_TRUE(textRepeatDef->useThisFont);
    EXPECT_EQ(textRepeatDef->justification, others::HorizontalTextJustification::Right);

    // Test TextRepeatText
    auto textRepeatText = others->get<others::TextRepeatText>(SCORE_PARTID, 1);
    ASSERT_TRUE(textRepeatText) << "TextRepeatText with cmper 1 not found";

    EXPECT_EQ(textRepeatText->text, "Segno");

    // Test TextRepeatDef 2 (with no explicit font data)
    auto textRepeatDef2 = others->get<others::TextRepeatDef>(SCORE_PARTID, 2);
    ASSERT_TRUE(textRepeatDef2) << "TextRepeatDef with cmper 2 not found";
    EXPECT_TRUE(textRepeatDef2->font) << "TextRepeatDef with cmper 2 font not created";
    EXPECT_EQ(textRepeatDef2->justification, others::HorizontalTextJustification::Center);
}

TEST(TextRepeatAssign, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
            <textRepeatAssign cmper="10" inci="0">
                <horzPos>-378</horzPos>
                <actuate>1</actuate>
                <target>12</target>
                <repnum>1</repnum>
                <vertPos>-116</vertPos>
                <indivPlac/>
                <topStaffOnly/>
                <hidden/>
                <clrOnChange/>
                <action>jumpAbsolute</action>
                <autoUpdate/>
                <trigger>onPass</trigger>
                <jmpIgnore/>
                <staffList>1</staffList>
            </textRepeatAssign>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto textRepeatAssign = others->get<others::TextRepeatAssign>(SCORE_PARTID, 10, 0);
    ASSERT_TRUE(textRepeatAssign) << "TextRepeatAssign with cmper 10 not found";

    EXPECT_EQ(textRepeatAssign->horzPos, -378);
    EXPECT_EQ(textRepeatAssign->passNumber, 1);
    EXPECT_EQ(textRepeatAssign->targetValue, 12);
    EXPECT_EQ(textRepeatAssign->textRepeatId, 1);
    EXPECT_EQ(textRepeatAssign->vertPos, -116);
    EXPECT_TRUE(textRepeatAssign->individualPlacement);
    EXPECT_TRUE(textRepeatAssign->topStaffOnly);
    EXPECT_TRUE(textRepeatAssign->hidden);
    EXPECT_TRUE(textRepeatAssign->resetOnAction);
    EXPECT_EQ(textRepeatAssign->jumpAction, others::RepeatActionType::JumpAbsolute);
    EXPECT_TRUE(textRepeatAssign->autoUpdate);
    EXPECT_EQ(textRepeatAssign->trigger, others::RepeatTriggerType::OnPass);
    EXPECT_TRUE(textRepeatAssign->jumpIfIgnoring);
    EXPECT_EQ(textRepeatAssign->staffList, 1);
}

TEST(StaffListRepeat, Populate)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "stafflists.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto names = others->getArray<others::StaffListRepeatName>(SCORE_PARTID);
    ASSERT_GE(names.size(), 2);
    EXPECT_EQ(names[0]->name, "Repeats Top Staff");
    EXPECT_EQ(names[1]->name, "Repeats Other");

    auto parts = others->getArray<others::StaffListRepeatParts>(SCORE_PARTID);
    ASSERT_GE(parts.size(), names.size());
    EXPECT_EQ(names[0]->getCmper(), parts[0]->getCmper());
    musxtest::staffListCheck(names[0]->name, parts[0], { -1 });
    EXPECT_EQ(names[1]->getCmper(), parts[1]->getCmper());
    musxtest::staffListCheck(names[1]->name, parts[1], { -2, 2, 3 });

    auto partsForced = others->getArray<others::StaffListRepeatPartsForced>(SCORE_PARTID);
    ASSERT_GE(partsForced.size(), 1);
    EXPECT_EQ(names[1]->getCmper(), partsForced[0]->getCmper()) << "partsForced is for the 2nd staff list";
    musxtest::staffListCheck(names[1]->name, partsForced[0], { 3 });

    auto score = others->getArray<others::StaffListRepeatScore>(SCORE_PARTID);
    ASSERT_GE(score.size(), names.size());
    EXPECT_EQ(names[0]->getCmper(), score[0]->getCmper());
    musxtest::staffListCheck(names[0]->name, score[0], { -1 });
    EXPECT_EQ(names[1]->getCmper(), score[1]->getCmper());
    musxtest::staffListCheck(names[1]->name, score[1], { -2, 1, 3 });

    auto scoreForced = others->getArray<others::StaffListRepeatScoreForced>(SCORE_PARTID);
    ASSERT_GE(scoreForced.size(), 1);
    EXPECT_EQ(names[1]->getCmper(), scoreForced[0]->getCmper()) << "partsForced is for the 2nd staff list";
    musxtest::staffListCheck(names[1]->name, scoreForced[0], { -2, 1 });
}

TEST(RepeatStaffListSet, TestContains)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "stafflists.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto scrollViewStaves = doc->getScrollViewStaves(SCORE_PARTID);
        auto repeatItem = others->get<others::RepeatEndingStart>(SCORE_PARTID, 2);
        auto staffListSet = repeatItem->createStaffListSet();
        EXPECT_TRUE(staffListSet.contains(1, scrollViewStaves, scrollViewStaves.getStaffInstanceAtIndex(0, 2)->hideRepeats));
        EXPECT_FALSE(staffListSet.contains(2, scrollViewStaves, scrollViewStaves.getStaffInstanceAtIndex(1, 2)->hideRepeats));
        EXPECT_TRUE(staffListSet.contains(3, scrollViewStaves, scrollViewStaves.getStaffInstanceAtIndex(2, 2)->hideRepeats));
    }

    /// @todo more tests
}

TEST(RepeatBack, IndividualPositioning)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "endings-indivpos.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto repeatItem = others->get<others::RepeatBack>(SCORE_PARTID, 4);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getIndividualPositioning(1);
        ASSERT_TRUE(indivPos);
        EXPECT_TRUE(indivPos->hidden);
        EXPECT_EQ(indivPos->x1add, 0);
        EXPECT_EQ(indivPos->y1add, 0);
        EXPECT_EQ(indivPos->x2add, 0);
        EXPECT_EQ(indivPos->y2add, 0);
    }
    {
        auto repeatItem = others->get<others::RepeatBack>(SCORE_PARTID, 4);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getIndividualPositioning(2);
        ASSERT_TRUE(indivPos);
        EXPECT_FALSE(indivPos->hidden);
        EXPECT_EQ(indivPos->x1add, -36);
        EXPECT_EQ(indivPos->y1add, 31);
        EXPECT_EQ(indivPos->x2add, -104);
        EXPECT_EQ(indivPos->y2add, -31);
    }
    {
        auto repeatItem = others->get<others::RepeatBack>(SCORE_PARTID, 8);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getIndividualPositioning(2);
        EXPECT_FALSE(indivPos);
    }
}

TEST(RepeatEndingStart, IndividualPositioning)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "endings-indivpos.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto repeatItem = others->get<others::RepeatEndingStart>(SCORE_PARTID, 4);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getIndividualPositioning(1);
        ASSERT_TRUE(indivPos);
        EXPECT_TRUE(indivPos->hidden);
        EXPECT_EQ(indivPos->x1add, -12);
        EXPECT_EQ(indivPos->y1add, 37);
        EXPECT_EQ(indivPos->x2add, -244);
        EXPECT_EQ(indivPos->y2add, 35);
    }
    {
        auto repeatItem = others->get<others::RepeatEndingStart>(SCORE_PARTID, 4);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getTextIndividualPositioning(1);
        ASSERT_TRUE(indivPos);
        EXPECT_FALSE(indivPos->hidden);
        EXPECT_EQ(indivPos->x1add, 66);
        EXPECT_EQ(indivPos->y1add, -53);
        EXPECT_EQ(indivPos->x2add, 0);
        EXPECT_EQ(indivPos->y2add, 0);
    }
}

TEST(TextRepeatAssign, IndividualPositioning)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "endings-indivpos.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto repeatItem = others->get<others::TextRepeatAssign>(SCORE_PARTID, 2, 0);
        ASSERT_TRUE(repeatItem);
        auto indivPos = repeatItem->getIndividualPositioning(2);
        ASSERT_TRUE(indivPos);
        EXPECT_TRUE(indivPos->hidden);
        EXPECT_EQ(indivPos->x1add, -119);
        EXPECT_EQ(indivPos->y1add, -8);
        EXPECT_EQ(indivPos->x2add, 0);
        EXPECT_EQ(indivPos->y2add, 0);
    }
}

TEST(RepeatEndingStart, EndingText)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "endingtext.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 5);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "1, 2, 3, 6.");
    }
    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 6);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "2.");
    }
    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 9);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "user text");
    }

    auto repeatOptions = doc->getOptions()->get<options::RepeatOptions>();
    ASSERT_TRUE(repeatOptions);
    auto mutableOpts = const_cast<options::RepeatOptions*>(repeatOptions.get());
    mutableOpts->addPeriod = false;

    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 5);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "1, 2, 3, 6");
    }
    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 6);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "2");
    }
    {
        auto repeatEnding = others->get<others::RepeatEndingStart>(SCORE_PARTID, 9);
        ASSERT_TRUE(repeatEnding);
        EXPECT_EQ(repeatEnding->createEndingText(), "user text");
    }
}
