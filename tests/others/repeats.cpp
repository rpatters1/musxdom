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
    EXPECT_TRUE(textRepeatAssign->resetOnAction);
    EXPECT_EQ(textRepeatAssign->jumpAction, others::RepeatActionType::JumpAbsolute);
    EXPECT_TRUE(textRepeatAssign->autoUpdate);
    EXPECT_EQ(textRepeatAssign->trigger, others::RepeatTriggerType::OnPass);
    EXPECT_TRUE(textRepeatAssign->jumpIfIgnoring);
    EXPECT_EQ(textRepeatAssign->staffList, 1);
}
