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

TEST(BaselinesTest, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <baselinesChords cmper1="0" cmper2="0">
      <basedisp>288</basedisp>
    </baselinesChords>
    <baselinesExprAboveStaff cmper1="0" cmper2="0">
      <basedisp>-8</basedisp>
    </baselinesExprAboveStaff>
    <baselinesExprAboveStaff cmper1="0" cmper2="1">
      <basedisp>1</basedisp>
    </baselinesExprAboveStaff>
    <baselinesExprBelowStaff cmper1="0" cmper2="0">
      <basedisp>66</basedisp>
    </baselinesExprBelowStaff>
    <baselinesExprBelowStaff cmper1="0" cmper2="1">
      <basedisp>-106</basedisp>
    </baselinesExprBelowStaff>
    <baselinesFingerboards cmper1="0" cmper2="0">
      <basedisp>246</basedisp>
    </baselinesFingerboards>
    <baselinesFingerboards cmper1="0" cmper2="1">
      <basedisp>-60</basedisp>
    </baselinesFingerboards>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="0">
      <basedisp>-144</basedisp>
      <lyricNumber>1</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="1">
      <basedisp>-184</basedisp>
      <lyricNumber>2</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="2">
      <basedisp>-224</basedisp>
      <lyricNumber>3</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="3">
      <basedisp>-264</basedisp>
      <lyricNumber>4</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="4">
      <basedisp>-304</basedisp>
      <lyricNumber>5</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="5">
      <basedisp>-344</basedisp>
      <lyricNumber>6</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="6">
      <basedisp>-384</basedisp>
      <lyricNumber>7</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="7">
      <basedisp>-424</basedisp>
      <lyricNumber>8</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="8">
      <basedisp>-464</basedisp>
      <lyricNumber>9</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsChorus cmper1="0" cmper2="0" inci="9">
      <basedisp>-504</basedisp>
      <lyricNumber>10</lyricNumber>
    </baselinesLyricsChorus>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="0">
      <basedisp>-144</basedisp>
      <lyricNumber>1</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="1">
      <basedisp>-184</basedisp>
      <lyricNumber>2</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="2">
      <basedisp>-224</basedisp>
      <lyricNumber>3</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="3">
      <basedisp>-264</basedisp>
      <lyricNumber>4</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="4">
      <basedisp>-304</basedisp>
      <lyricNumber>5</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="5">
      <basedisp>-344</basedisp>
      <lyricNumber>6</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="6">
      <basedisp>-384</basedisp>
      <lyricNumber>7</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="7">
      <basedisp>-424</basedisp>
      <lyricNumber>8</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="8">
      <basedisp>-464</basedisp>
      <lyricNumber>9</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsSection cmper1="0" cmper2="0" inci="9">
      <basedisp>-504</basedisp>
      <lyricNumber>10</lyricNumber>
    </baselinesLyricsSection>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="0">
      <basedisp>-144</basedisp>
      <lyricNumber>1</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="1">
      <basedisp>-184</basedisp>
      <lyricNumber>2</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="2">
      <basedisp>-324</basedisp>
      <lyricNumber>3</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="3">
      <basedisp>-264</basedisp>
      <lyricNumber>4</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="4">
      <basedisp>-304</basedisp>
      <lyricNumber>5</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="5">
      <basedisp>-344</basedisp>
      <lyricNumber>6</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="6">
      <basedisp>-384</basedisp>
      <lyricNumber>7</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="7">
      <basedisp>-424</basedisp>
      <lyricNumber>8</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="8">
      <basedisp>-464</basedisp>
      <lyricNumber>9</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="0" inci="9">
      <basedisp>-504</basedisp>
      <lyricNumber>10</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesLyricsVerse cmper1="0" cmper2="1" inci="0">
      <basedisp>54</basedisp>
      <lyricNumber>3</lyricNumber>
    </baselinesLyricsVerse>
    <baselinesSysChords cmper1="1" cmper2="1">
      <basedisp>-51</basedisp>
    </baselinesSysChords>
    <baselinesSysExprAboveStaff cmper1="1" cmper2="1">
      <basedisp>20</baseddisp>
    </baselinesSysExprAboveStaff>
    <baselinesSysExprBelowStaff cmper1="1" cmper2="1">
      <basedisp>46</basedisp>
    </baselinesSysExprBelowStaff>
    <baselinesSysExprBelowStaff cmper1="65527" cmper2="2">
      <basedisp>-40</basedisp>
    </baselinesSysExprBelowStaff>
    <baselinesSysFingerboards cmper1="1" cmper2="1">
      <basedisp>-52</basedisp>
    </baselinesSysFingerboards>
    <baselinesSysLyricsVerse cmper1="1" cmper2="1" inci="0">
      <basedisp>-42</basedisp>
      <lyricNumber>3</lyricNumber>
    </baselinesSysLyricsVerse>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // ---- Global (cmper1=0, cmper2=0) ----
    {
        auto chords = details->get<details::BaselineChords>(SCORE_PARTID, 0, 0);
        ASSERT_TRUE(chords);
        EXPECT_EQ(chords->baselineDisplacement, 288);

        auto exprAbove = details->get<details::BaselineExpressionsAbove>(SCORE_PARTID, 0, 0);
        ASSERT_TRUE(exprAbove);
        EXPECT_EQ(exprAbove->baselineDisplacement, -8);

        auto exprBelow = details->get<details::BaselineExpressionsBelow>(SCORE_PARTID, 0, 0);
        ASSERT_TRUE(exprBelow);
        EXPECT_EQ(exprBelow->baselineDisplacement, 66);

        auto fret = details->get<details::BaselineFretboards>(SCORE_PARTID, 0, 0);
        ASSERT_TRUE(fret);
        EXPECT_EQ(fret->baselineDisplacement, 246);
    }

    // ---- Staff (cmper1=0, cmper2=1) ----
    {
        auto exprAboveS1 = details->get<details::BaselineExpressionsAbove>(SCORE_PARTID, 0, 1);
        ASSERT_TRUE(exprAboveS1);
        EXPECT_EQ(exprAboveS1->baselineDisplacement, 1);

        auto exprBelowS1 = details->get<details::BaselineExpressionsBelow>(SCORE_PARTID, 0, 1);
        ASSERT_TRUE(exprBelowS1);
        EXPECT_EQ(exprBelowS1->baselineDisplacement, -106);

        auto fretS1 = details->get<details::BaselineFretboards>(SCORE_PARTID, 0, 1);
        ASSERT_TRUE(fretS1);
        EXPECT_EQ(fretS1->baselineDisplacement, -60);
    }

    // ---- Lyrics (global; cmper1=0, cmper2=0; inci=0..9) ----
    auto expectLyric = [&](auto rec, int disp, int number) {
        ASSERT_TRUE(rec);
        EXPECT_EQ(rec->baselineDisplacement, disp);
        ASSERT_TRUE(rec->lyricNumber);
        EXPECT_EQ(*rec->lyricNumber, number);
    };

    // Chorus
    {
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 0), -144, 1);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 1), -184, 2);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 2), -224, 3);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 3), -264, 4);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 4), -304, 5);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 5), -344, 6);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 6), -384, 7);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 7), -424, 8);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 8), -464, 9);
        expectLyric(details->get<details::BaselineLyricsChorus>(SCORE_PARTID, 0, 0, 9), -504, 10);
    }

    // Section
    {
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 0), -144, 1);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 1), -184, 2);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 2), -224, 3);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 3), -264, 4);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 4), -304, 5);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 5), -344, 6);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 6), -384, 7);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 7), -424, 8);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 8), -464, 9);
        expectLyric(details->get<details::BaselineLyricsSection>(SCORE_PARTID, 0, 0, 9), -504, 10);
    }

    // Verse (global)
    {
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 0), -144, 1);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 1), -184, 2);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 2), -324, 3);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 3), -264, 4);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 4), -304, 5);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 5), -344, 6);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 6), -384, 7);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 7), -424, 8);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 8), -464, 9);
        expectLyric(details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 0, 9), -504, 10);
    }

    // Verse (staff 1 override at inci 0)
    {
        auto verseS1i0 = details->get<details::BaselineLyricsVerse>(SCORE_PARTID, 0, 1, 0);
        expectLyric(verseS1i0, 54, 3);
    }

    // ---- Per-System (cmper1>0; system+staff) ----
    {
        auto sysChords_1_1 = details->get<details::BaselineSystemChords>(SCORE_PARTID, 1, 1);
        ASSERT_TRUE(sysChords_1_1);
        EXPECT_EQ(sysChords_1_1->baselineDisplacement, -51);

        auto sysExprA_1_1 = details->get<details::BaselineSystemExpressionsAbove>(SCORE_PARTID, 1, 1);
        ASSERT_TRUE(sysExprA_1_1);
        EXPECT_EQ(sysExprA_1_1->baselineDisplacement, 20);

        auto sysExprB_1_1 = details->get<details::BaselineSystemExpressionsBelow>(SCORE_PARTID, 1, 1);
        ASSERT_TRUE(sysExprB_1_1);
        EXPECT_EQ(sysExprB_1_1->baselineDisplacement, 46);

        auto sysExprB_scroll_2 = details->get<details::BaselineSystemExpressionsBelow>(SCORE_PARTID, 65527, 2);
        ASSERT_TRUE(sysExprB_scroll_2);
        EXPECT_EQ(sysExprB_scroll_2->baselineDisplacement, -40);

        auto sysFret_1_1 = details->get<details::BaselineSystemFretboards>(SCORE_PARTID, 1, 1);
        ASSERT_TRUE(sysFret_1_1);
        EXPECT_EQ(sysFret_1_1->baselineDisplacement, -52);

        auto sysVerse_1_1_i0 = details->get<details::BaselineSystemLyricsVerse>(SCORE_PARTID, 1, 1, 0);
        ASSERT_TRUE(sysVerse_1_1_i0);
        EXPECT_EQ(sysVerse_1_1_i0->baselineDisplacement, -42);
        ASSERT_TRUE(sysVerse_1_1_i0->lyricNumber);
        EXPECT_EQ(*sysVerse_1_1_i0->lyricNumber, 3);
    }
}

TEST(BaselinesTest, BaselinePosition)
{
    std::vector<char> fileXml;
    musxtest::readFile(musxtest::getInputPath() / "baselines.enigmaxml", fileXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fileXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    {
        auto staff = others->get<others::Staff>(SCORE_PARTID, 1);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->calcBaselineZeroPosition(), -48);
        Evpu exprUnderPos = staff->calcBaselinePosition<details::BaselineExpressionsBelow>(1);
        EXPECT_EQ(exprUnderPos, -110);
    }
    {
        auto staff = others->get<others::Staff>(SCORE_PARTID, 2);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->calcBaselineZeroPosition(), -36);
        Evpu exprUnderPos = staff->calcBaselinePosition<details::BaselineLyricsVerse>(1, 3);
        EXPECT_EQ(exprUnderPos, -123);
    }
    {
        auto staff = others->get<others::Staff>(SCORE_PARTID, 3);
        ASSERT_TRUE(staff);
        EXPECT_EQ(staff->calcBaselineZeroPosition(), -24);
        Evpu exprUnderPos = staff->calcBaselinePosition<details::BaselineFretboards>(1);
        EXPECT_EQ(exprUnderPos, 8);
    }
}