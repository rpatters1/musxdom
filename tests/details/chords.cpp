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
 using namespace musx::util;
 
 TEST(ChordAssignTest, PopulateFields)
 {
     constexpr static musxtest::string_view xml = R"xml(
 <?xml version="1.0" encoding="UTF-8"?>
 <finale>
   <details>
     <chordAssign cmper1="1" cmper2="3" inci="0">
       <suffix>67</suffix>
       <fbStyleID>2</fbStyleID>
       <rootScaleNum>5</rootScaleNum>
       <rootAlter>1</rootAlter>
       <rootLowerCase/>
       <playSuffix/>
       <showRoot/>
       <playRoot/>
       <showFretboard/>
       <showSuffix/>
       <playFretboard/>
       <bassScaleNum>3</bassScaleNum>
       <bassAlter>2</bassAlter>
       <bassLowerCase/>
       <bassPosition>underRoot</bassPosition>
       <showAltBass/>
       <playAltBass/>
       <capoValue>13</capoValue>
       <useLocalCapo/>
       <useFretFont/>
       <horzOff>11</horzOff>
       <vertOff>-142</vertOff>
       <fbHorzOff>-116</fbHorzOff>
       <fbVertOff>-29</fbVertOff>
       <horzEdu>2048</horzEdu>
       <chPercent>95</chPercent>
       <fbPercent>85</fbPercent>
     </chordAssign>
   </details>
 </finale>
     )xml";
 
     auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
     ASSERT_TRUE(doc);
 
     auto details = doc->getDetails();
     ASSERT_TRUE(details);
 
     auto assign = details->get<details::ChordAssign>(SCORE_PARTID, 1, 3, 0);
     ASSERT_TRUE(assign);
 
     EXPECT_EQ(assign->suffixId, Cmper{67});
     EXPECT_EQ(assign->fbStyleId, Cmper{2});
     EXPECT_EQ(assign->rootScaleNum, 5);
     EXPECT_EQ(assign->rootAlter, 1);
     EXPECT_TRUE(assign->rootLowerCase);
     EXPECT_TRUE(assign->playSuffix);
     EXPECT_TRUE(assign->showRoot);
     EXPECT_TRUE(assign->playRoot);
     EXPECT_TRUE(assign->showFretboard);
     EXPECT_TRUE(assign->showSuffix);
     EXPECT_TRUE(assign->playFretboard);
     EXPECT_EQ(assign->bassScaleNum, 3);
     EXPECT_EQ(assign->bassAlter, 2);
     EXPECT_TRUE(assign->bassLowerCase);
     EXPECT_EQ(assign->bassPosition, details::ChordAssign::BassPosition::UnderRoot);
     EXPECT_TRUE(assign->showAltBass);
     EXPECT_TRUE(assign->playAltBass);
     EXPECT_EQ(assign->capoValue, 13);
     EXPECT_TRUE(assign->useLocalCapo);
     EXPECT_TRUE(assign->useFretFont);
     EXPECT_EQ(assign->horzOff, Evpu{11});
     EXPECT_EQ(assign->vertOff, Evpu{-142});
     EXPECT_EQ(assign->fbHorzOff, Evpu{-116});
     EXPECT_EQ(assign->fbVertOff, Evpu{-29});
     EXPECT_EQ(assign->horzEdu, Edu{2048});
     EXPECT_EQ(assign->chPercent, 95);
     EXPECT_EQ(assign->fbPercent, 85);
 }

TEST(ChordSuffixElementTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <chordSuffix cmper="178" inci="0">
      <suffix>1</suffix>
      <fontID>10</fontID>
      <fontSize>12</fontSize>
      <efx>
        <italic/>
        <absolute/>
        <hidden/>
      </efx>
      <xdisp>1</xdisp>
      <ydisp>2</ydisp>
      <isNumber/>
    </chordSuffix>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto element = others->get<others::ChordSuffixElement>(SCORE_PARTID, 178, 0);
    ASSERT_TRUE(element);

    EXPECT_EQ(element->symbol, char32_t{0x01});
    EXPECT_TRUE(element->font);
    EXPECT_EQ(element->font->fontId, 10);
    EXPECT_EQ(element->font->fontSize, 12);
    EXPECT_TRUE(element->font->italic);
    EXPECT_FALSE(element->font->bold);
    EXPECT_FALSE(element->font->underline);
    EXPECT_FALSE(element->font->strikeout);
    EXPECT_TRUE(element->font->absolute);
    EXPECT_TRUE(element->font->hidden);
    EXPECT_EQ(element->xdisp, Evpu{1});
    EXPECT_EQ(element->ydisp, Evpu{2});
    EXPECT_TRUE(element->isNumber);
}

TEST(ChordSuffixPlayTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <chordSuffixPlay cmper="178">
      <data>1</data>
      <data>2</data>
      <data>3</data>
      <data>4</data>
      <data>5</data>
      <data>0</data>
    </chordSuffixPlay>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto playback = others->get<others::ChordSuffixPlayback>(SCORE_PARTID, 178);
    ASSERT_TRUE(playback);

    ASSERT_EQ(playback->values.size(), 6);
    EXPECT_EQ(playback->values[0], 1);
    EXPECT_EQ(playback->values[1], 2);
    EXPECT_EQ(playback->values[2], 3);
    EXPECT_EQ(playback->values[3], 4);
    EXPECT_EQ(playback->values[4], 5);
    EXPECT_EQ(playback->values[5], 0);
}
