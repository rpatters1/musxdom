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

TEST(MusicSymbolOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
 <finale>
   <options>
     <musicSymbolOptions>
       <noteheadQuarter>57508</noteheadQuarter>
       <noteheadHalf>57507</noteheadHalf>
       <noteheadWhole>57506</noteheadWhole>
       <noteheadDblWhole>57504</noteheadDblWhole>
       <natural>57953</natural>
       <flat>57952</flat>
       <sharp>57954</sharp>
       <dblFlat>57956</dblFlat>
       <dblSharp>57955</dblSharp>
       <parenNatural>62934</parenNatural>
       <parenFlat>62933</parenFlat>
       <parenSharp>62935</parenSharp>
       <parenDblFlat>62937</parenDblFlat>
       <parenDblSharp>62936</parenDblSharp>
       <chordNatural>57953</chordNatural>
       <chordFlat>57952</chordFlat>
       <chordSharp>57954</chordSharp>
       <chordDblFlat>57956</chordDblFlat>
       <chordDblSharp>57955</chordDblSharp>
       <keySigNatural>57953</keySigNatural>
       <keySigFlat>57952</keySigFlat>
       <keySigSharp>57954</keySigSharp>
       <keySigDblFlat>57956</keySigDblFlat>
       <keySigDblSharp>57955</keySigDblSharp>
       <restLonga>57505</restLonga>
       <restDblWhole>58594</restDblWhole>
       <restWhole>58595</restWhole>
       <restHalf>58596</restHalf>
       <restQuarter>58597</restQuarter>
       <restEighth>58598</restEighth>
       <rest16th>58599</rest16th>
       <rest32nd>58600</rest32nd>
       <rest64th>58601</rest64th>
       <rest128th>58602</rest128th>
       <restDefMeas>58595</restDefMeas>
       <oneBarRepeat>58624</oneBarRepeat>
       <twoBarRepeat>58625</twoBarRepeat>
       <slashBar>58628</slashBar>
       <quarterSlash>57600</quarterSlash>
       <halfSlash>57604</halfSlash>
       <wholeSlash>57604</wholeSlash>
       <dblWholeSlash>57602</dblWholeSlash>
       <timeSigPlus>57485</timeSigPlus>
       <timeSigPlusParts>57485</timeSigPlusParts>
       <timeSigAbrvCommon>57484</timeSigAbrvCommon>
       <timeSigAbrvCut>57486</timeSigAbrvCut>
       <timeSigAbrvCommonParts>57482</timeSigAbrvCommonParts>
       <timeSigAbrvCutParts>57483</timeSigAbrvCutParts>
       <augDot>57831</augDot>
       <forwardRepeatDot>57412</forwardRepeatDot>
       <backRepeatDot>57412</backRepeatDot>
       <eightVaUp>58641</eightVaUp>
       <eightVbDown>58641</eightVbDown>
       <fifteenMaUp>58645</fifteenMaUp>
       <fifteenMbDown>58645</fifteenMbDown>
       <trillChar>58726</trillChar>
       <wiggleChar>60068</wiggleChar>
       <flagUp>57920</flagUp>
       <flagDown>57921</flagDown>
       <flag16Up>57922</flag16Up>
       <flag16Down>57923</flag16Down>
       <flag2Up>57936</flag2Up>
       <flag2Down>57937</flag2Down>
       <flagStraightUp>62479</flagStraightUp>
       <flagStraightDown>62481</flagStraightDown>
     </musicSymbolOptions>
   </options>
 </finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto musicSymbolOptions = options->get<musx::dom::options::MusicSymbolOptions>();
    ASSERT_TRUE(musicSymbolOptions);

    EXPECT_EQ(musicSymbolOptions->noteheadQuarter, 57508);
    EXPECT_EQ(musicSymbolOptions->noteheadHalf, 57507);
    EXPECT_EQ(musicSymbolOptions->noteheadWhole, 57506);
    EXPECT_EQ(musicSymbolOptions->noteheadDblWhole, 57504);
    EXPECT_EQ(musicSymbolOptions->natural, 57953);
    EXPECT_EQ(musicSymbolOptions->flat, 57952);
    EXPECT_EQ(musicSymbolOptions->sharp, 57954);
    EXPECT_EQ(musicSymbolOptions->dblFlat, 57956);
    EXPECT_EQ(musicSymbolOptions->dblSharp, 57955);
    EXPECT_EQ(musicSymbolOptions->parenNatural, 62934);
    EXPECT_EQ(musicSymbolOptions->parenFlat, 62933);
    EXPECT_EQ(musicSymbolOptions->parenSharp, 62935);
    EXPECT_EQ(musicSymbolOptions->parenDblFlat, 62937);
    EXPECT_EQ(musicSymbolOptions->parenDblSharp, 62936);
    EXPECT_EQ(musicSymbolOptions->chordNatural, 57953);
    EXPECT_EQ(musicSymbolOptions->chordFlat, 57952);
    EXPECT_EQ(musicSymbolOptions->chordSharp, 57954);
    EXPECT_EQ(musicSymbolOptions->chordDblFlat, 57956);
    EXPECT_EQ(musicSymbolOptions->chordDblSharp, 57955);
    EXPECT_EQ(musicSymbolOptions->keySigNatural, 57953);
    EXPECT_EQ(musicSymbolOptions->keySigFlat, 57952);
    EXPECT_EQ(musicSymbolOptions->keySigSharp, 57954);
    EXPECT_EQ(musicSymbolOptions->keySigDblFlat, 57956);
    EXPECT_EQ(musicSymbolOptions->keySigDblSharp, 57955);
    EXPECT_EQ(musicSymbolOptions->restLonga, 57505);
    EXPECT_EQ(musicSymbolOptions->restDblWhole, 58594);
    EXPECT_EQ(musicSymbolOptions->restWhole, 58595);
    EXPECT_EQ(musicSymbolOptions->restHalf, 58596);
    EXPECT_EQ(musicSymbolOptions->restQuarter, 58597);
    EXPECT_EQ(musicSymbolOptions->restEighth, 58598);
    EXPECT_EQ(musicSymbolOptions->rest16th, 58599);
    EXPECT_EQ(musicSymbolOptions->rest32nd, 58600);
    EXPECT_EQ(musicSymbolOptions->rest64th, 58601);
    EXPECT_EQ(musicSymbolOptions->rest128th, 58602);
    EXPECT_EQ(musicSymbolOptions->restDefMeas, 58595);
    EXPECT_EQ(musicSymbolOptions->oneBarRepeat, 58624);
    EXPECT_EQ(musicSymbolOptions->twoBarRepeat, 58625);
    EXPECT_EQ(musicSymbolOptions->slashBar, 58628);
    EXPECT_EQ(musicSymbolOptions->quarterSlash, 57600);
    EXPECT_EQ(musicSymbolOptions->halfSlash, 57604);
    EXPECT_EQ(musicSymbolOptions->wholeSlash, 57604);
    EXPECT_EQ(musicSymbolOptions->dblWholeSlash, 57602);
    EXPECT_EQ(musicSymbolOptions->timeSigPlus, 57485);
    EXPECT_EQ(musicSymbolOptions->timeSigPlusParts, 57485);
    EXPECT_EQ(musicSymbolOptions->timeSigAbrvCommon, 57484);
    EXPECT_EQ(musicSymbolOptions->timeSigAbrvCut, 57486);
    EXPECT_EQ(musicSymbolOptions->timeSigAbrvCommonParts, 57482);
    EXPECT_EQ(musicSymbolOptions->timeSigAbrvCutParts, 57483);
    EXPECT_EQ(musicSymbolOptions->augDot, 57831);
    EXPECT_EQ(musicSymbolOptions->forwardRepeatDot, 57412);
    EXPECT_EQ(musicSymbolOptions->backRepeatDot, 57412);
    EXPECT_EQ(musicSymbolOptions->eightVaUp, 58641);
    EXPECT_EQ(musicSymbolOptions->eightVbDown, 58641);
    EXPECT_EQ(musicSymbolOptions->fifteenMaUp, 58645);
    EXPECT_EQ(musicSymbolOptions->fifteenMbDown, 58645);
    EXPECT_EQ(musicSymbolOptions->trillChar, 58726);
    EXPECT_EQ(musicSymbolOptions->wiggleChar, 60068);
    EXPECT_EQ(musicSymbolOptions->flagUp, 57920);
    EXPECT_EQ(musicSymbolOptions->flagDown, 57921);
    EXPECT_EQ(musicSymbolOptions->flag16Up, 57922);
    EXPECT_EQ(musicSymbolOptions->flag16Down, 57923);
    EXPECT_EQ(musicSymbolOptions->flag2Up, 57936);
    EXPECT_EQ(musicSymbolOptions->flag2Down, 57937);
    EXPECT_EQ(musicSymbolOptions->flagStraightUp, 62479);
    EXPECT_EQ(musicSymbolOptions->flagStraightDown, 62481);
}
