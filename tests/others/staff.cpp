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
    <instUsed cmper="0" inci="0">
      <inst>15</inst>
      <trackType>staff</trackType>
      <distFromTop>0</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="1">
      <inst>16</inst>
      <trackType>staff</trackType>
      <distFromTop>-288</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <instUsed cmper="0" inci="2">
      <inst>11</inst>
      <trackType>staff</trackType>
      <distFromTop>-288</distFromTop>
      <range>
        <startMeas>1</startMeas>
        <startEdu>0</startEdu>
        <endMeas>32767</endMeas>
        <endEdu>2147483647</endEdu>
      </range>
    </instUsed>
    <staffSpec cmper="11">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
      <defaultClef>1</defaultClef>
      <transposedClef>2</transposedClef>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-1</stemReversal>
      <fullName>75</fullName>
      <abbrvName>76</abbrvName>
      <topRepeatDotOff>-3</topRepeatDotOff>
      <botRepeatDotOff>-5</botRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
      <useAutoNum/>
    </staffSpec>
    <staffSpec cmper="15">
      <staffLines>5</staffLines>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
      <floatKeys/>
      <showNameParts/>
      <defaultClef>1</defaultClef>
      <transposedClef>2</transposedClef>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-1</stemReversal>
      <fullName>75</fullName>
      <abbrvName>76</abbrvName>
      <topRepeatDotOff>-3</topRepeatDotOff>
      <botRepeatDotOff>-5</botRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
      <hideStems/>
      <stemDir>alwaysUp</stemDir>
    </staffSpec>
    <staffSpec cmper="16">
      <staffLines>0</staffLines>
      <lineSpace>24</lineSpace>
      <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
      <floatTime/>
      <defaultClef>3</defaultClef>
      <transposedClef>4</transposedClef>
      <hideStfNameInScore/>
      <dwRestOffset>-4</dwRestOffset>
      <wRestOffset>-4</wRestOffset>
      <hRestOffset>-4</hRestOffset>
      <otherRestOffset>-4</otherRestOffset>
      <stemReversal>-2</stemReversal>
      <fullName>77</fullName>
      <abbrvName>78</abbrvName>
      <topRepeatDotOff>-2</topRepeatDotOff>
      <botRepeatDotOff>-6</botRepeatDotOff>
      <vertTabNumOff>-1024</vertTabNumOff>
      <hideBeams/>
      <stemDir>alwaysDown</stemDir>
      <autoNum>arabicPrefix</autoNum>
      <useAutoNum/>
    </staffSpec>
  </others>
</finale>
    )xml";

TEST(StaffTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staff1 = others->get<others::Staff>(SCORE_PARTID, 15);
    ASSERT_TRUE(staff1) << "Staff with cmper 15 not found";

    EXPECT_EQ(staff1->staffLines.value_or(-1), 5);
    EXPECT_EQ(staff1->lineSpace, 24);
    EXPECT_EQ(staff1->instUuid, "54422b22-4627-4100-abbf-064eedc15fe3");
    EXPECT_TRUE(staff1->floatKeys);
    EXPECT_FALSE(staff1->floatTime);
    EXPECT_EQ(staff1->defaultClef, 1);
    EXPECT_TRUE(staff1->showNameInParts);
    EXPECT_EQ(staff1->transposedClef, 2);
    EXPECT_FALSE(staff1->hideNameInScore);
    EXPECT_EQ(staff1->dwRestOffset, -4);
    EXPECT_EQ(staff1->wRestOffset, -4);
    EXPECT_EQ(staff1->hRestOffset, -4);
    EXPECT_EQ(staff1->otherRestOffset, -4);
    EXPECT_EQ(staff1->stemReversal, -1);
    EXPECT_EQ(staff1->fullNameTextId, 75);
    EXPECT_EQ(staff1->abbrvNameTextId, 76);
    EXPECT_EQ(staff1->topRepeatDotOff, -3);
    EXPECT_EQ(staff1->botRepeatDotOff, -5);
    EXPECT_EQ(staff1->vertTabNumOff, -1024);
    EXPECT_TRUE(staff1->hideStems);
    EXPECT_FALSE(staff1->hideBeams);
    EXPECT_EQ(staff1->stemDirection, others::Staff::StemDirection::AlwaysUp);
    EXPECT_EQ(staff1->autoNumbering, others::Staff::AutoNumberingStyle::ArabicSuffix);
    EXPECT_EQ(staff1->useAutoNumbering, false);

    auto staff2 = others->get<others::Staff>(SCORE_PARTID, 16);
    ASSERT_TRUE(staff2) << "Staff with cmper 16 not found";

    EXPECT_EQ(staff2->staffLines.value_or(-1), 0);
    EXPECT_EQ(staff2->lineSpace, 24);
    EXPECT_EQ(staff2->instUuid, "54422b22-4627-4100-abbf-064eedc15fe3");
    EXPECT_FALSE(staff2->floatKeys);
    EXPECT_TRUE(staff2->floatTime);
    EXPECT_EQ(staff2->defaultClef, 3);
    EXPECT_FALSE(staff2->showNameInParts);
    EXPECT_EQ(staff2->transposedClef, 4);
    EXPECT_TRUE(staff2->hideNameInScore);
    EXPECT_EQ(staff2->dwRestOffset, -4);
    EXPECT_EQ(staff2->wRestOffset, -4);
    EXPECT_EQ(staff2->hRestOffset, -4);
    EXPECT_EQ(staff2->otherRestOffset, -4);
    EXPECT_EQ(staff2->stemReversal, -2);
    EXPECT_EQ(staff2->fullNameTextId, 77);
    EXPECT_EQ(staff2->abbrvNameTextId, 78);
    EXPECT_EQ(staff2->topRepeatDotOff, -2);
    EXPECT_EQ(staff2->botRepeatDotOff, -6);
    EXPECT_EQ(staff2->vertTabNumOff, -1024);
    EXPECT_FALSE(staff2->hideStems);
    EXPECT_TRUE(staff2->hideBeams);
    EXPECT_EQ(staff2->stemDirection, others::Staff::StemDirection::AlwaysDown);
    EXPECT_EQ(staff2->autoNumbering, others::Staff::AutoNumberingStyle::ArabicPrefix);
    EXPECT_EQ(staff2->useAutoNumbering, true);
    
    auto staff3 = others->get<others::Staff>(SCORE_PARTID, 11);
    ASSERT_TRUE(staff2) << "Staff with cmper 11 not found";

    EXPECT_EQ(staff3->stemDirection, others::Staff::StemDirection::Default);
}

TEST(StaffTest, AutoNumbering)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staff1Const = others->get<others::Staff>(SCORE_PARTID, 15);
    ASSERT_TRUE(staff1Const) << "Staff with cmper 15 not found";
    auto staff1 = const_cast<others::Staff*>(staff1Const.get());

    EXPECT_FALSE(staff1->autoNumberValue.has_value());
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name");
    staff1->useAutoNumbering = true;
    others::Staff::calcAllAutoNumberValues(doc);
    EXPECT_EQ(staff1->autoNumberValue.value_or(-1), 1);
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name 1");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name I");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "1st Name");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "Name A");
    staff1->autoNumbering = others::Staff::AutoNumberingStyle::ArabicPrefix;
    EXPECT_EQ(staff1->addAutoNumbering("Name"), "1. Name");

    auto staff2Const = others->get<others::Staff>(SCORE_PARTID, 16);
    ASSERT_TRUE(staff2Const) << "Staff with cmper 16 not found";
    auto staff2 = const_cast<others::Staff*>(staff2Const.get());

    EXPECT_EQ(staff2->autoNumberValue.value_or(-1), 2);
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "2. Name");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name II");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "2nd Name");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name B");
    staff2->autoNumbering = others::Staff::AutoNumberingStyle::ArabicSuffix;
    EXPECT_EQ(staff2->addAutoNumbering("Name"), "Name 2");

    auto staff3Const = others->get<others::Staff>(SCORE_PARTID, 11);
    ASSERT_TRUE(staff3Const) << "Staff with cmper 11 not found";
    auto staff3 = const_cast<others::Staff*>(staff3Const.get());

    EXPECT_EQ(staff3->autoNumberValue.value_or(-1), 3);
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name 3");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::RomanSuffix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name III");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::OrdinalPrefix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "3rd Name");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::AlphaSuffix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "Name C");
    staff3->autoNumbering = others::Staff::AutoNumberingStyle::ArabicPrefix;
    EXPECT_EQ(staff3->addAutoNumbering("Name"), "3. Name");
}

TEST(StaffText, StaffLines)
{
  constexpr static musxtest::string_view staffLinesXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
  <finale>
    <others>
      <staffSpec cmper="1">
        <staffLines>5</staffLines>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <dwRestOffset>-4</dwRestOffset>
        <wRestOffset>-4</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
      <staffSpec cmper="2">
        <customStaff>
          <staffLine>13</staffLine>
        </customStaff>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <botBarlineOffset>-24</botBarlineOffset>
        <topBarlineOffset>24</topBarlineOffset>
        <dwRestOffset>-5</dwRestOffset>
        <wRestOffset>-6</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
      <staffSpec cmper="3">
        <staffLines>1</staffLines>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <dwRestOffset>-4</dwRestOffset>
        <wRestOffset>-4</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
      <staffSpec cmper="4">
        <customStaff>
          <staffLine>11</staffLine>
          <staffLine>12</staffLine>
          <staffLine>13</staffLine>
          <staffLine>14</staffLine>
          <staffLine>15</staffLine>
          <staffLine>18</staffLine>
        </customStaff>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <dwRestOffset>-4</dwRestOffset>
        <wRestOffset>-4</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
      <staffSpec cmper="5">
        <staffLines>3</staffLines>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <dwRestOffset>-4</dwRestOffset>
        <wRestOffset>-4</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
      <staffSpec cmper="6">
        <customStaff>
          <staffLine>12</staffLine>
          <staffLine>13</staffLine>
          <staffLine>14</staffLine>
        </customStaff>
        <lineSpace>24</lineSpace>
        <instUuid>54422b22-4627-4100-abbf-064eedc15fe3</instUuid>
        <dwRestOffset>-4</dwRestOffset>
        <wRestOffset>-4</wRestOffset>
        <hRestOffset>-4</hRestOffset>
        <otherRestOffset>-4</otherRestOffset>
        <stemReversal>-4</stemReversal>
        <botRepeatDotOff>-5</botRepeatDotOff>
        <topRepeatDotOff>-3</topRepeatDotOff>
        <vertTabNumOff>-1024</vertTabNumOff>
      </staffSpec>
    </others>
  </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(staffLinesXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staves = others->getArray<others::Staff>(SCORE_PARTID);

    const std::vector<int> expectedValues = { -4, -4, 0, -7, -2, -4 };
    size_t x = 0;
    for (; x < staves.size(); x++) {
      EXPECT_LT(x, expectedValues.size()) << "too few expected values";
      if (x >= expectedValues.size()) break;
      const auto& staff = staves[x];
      EXPECT_EQ(staff->calcMiddleStaffPosition(), expectedValues[x]);
    }
    EXPECT_EQ(x, staves.size());
}

TEST(StaffTest, Transposition)
{
    std::vector<char> transposeXml;
    musxtest::readFile(musxtest::getInputPath() / "transpose.enigmaxml", transposeXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(transposeXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    using Result = std::tuple<int, int, int, int>; // note index, octave, alteration, staff line

    static constexpr std::array<Result, 2> expectedConcertResult = {
        Result{5, 4, 0, 1}, // A4
        Result{6, 4, 0, 2}  // B4
    };

    static const std::array<std::array<Result, 2>, 5> expectedWrittenResults = {{
        { Result{5, 4, 0, 1}, Result{6, 4, 0, 2} },
        { Result{0, 5, 0, 3}, Result{1, 5, 0, 4} },
        { Result{6, 5, 0, 3}, Result{0, 6, 1, 4} },
        { Result{4, 5, -1, 1}, Result{5, 5, -1, 2} },
        { Result{3, 6, 1, 7}, Result{4, 6, 1, 8} },
    }};

    for (Cmper measId = 1; measId <= 5; measId++) {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, measId);
        ASSERT_TRUE(gfhold);

        auto entries = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entries);
        ASSERT_GE(entries->getEntries().size(), 2);

        for (size_t x = 0; x < 2; x++) {
            auto note = NoteInfoPtr(EntryInfoPtr(entries, x), 0);

            auto [wNote, wOctave, wAlter, wStaff] = note.calcNoteProperties();
            auto [cNote, cOctave, cAlter, cStaff] = note.calcNotePropertiesConcert();

            const auto& [expWNote, expWOct, expWAlt, expWStaff] = expectedWrittenResults[measId - 1][x];
            const auto& [expCNote, expCOct, expCAlt, expCStaff] = expectedConcertResult[x];

            // Written checks
            EXPECT_EQ(int(wNote), expWNote) << "Written note mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wOctave, expWOct) << "Written octave mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wAlter, expWAlt) << "Written alteration mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wStaff, expWStaff) << "Written staff mismatch at entry " << x << " in measure " << measId;

            // Concert checks
            EXPECT_EQ(int(cNote), expCNote) << "Concert note mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cOctave, expCOct) << "Concert octave mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cAlter, expCAlt) << "Concert alteration mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cStaff, expCStaff) << "Concert staff mismatch at entry " << x << " in measure " << measId;

    /*
            // Optional: print results
            static constexpr std::array<char, 7> noteNames = { 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
            std::cout << "Entry index " << x << " measure " << measId << std::endl;
            std::cout << "    W: " << noteNames[int(wNote)] << ", " << wOctave << ", " << wAlter << ", " << wStaff << std::endl;
            std::cout << "    C: " << noteNames[int(cNote)] << ", " << cOctave << ", " << cAlter << ", " << cStaff << std::endl;
    */
        }
    }
}

TEST(StaffTest, Transposition31Edo)
{
    std::vector<char> transposeXml;
    musxtest::readFile(musxtest::getInputPath() / "transpose_31edo.enigmaxml", transposeXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(transposeXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    using Result = std::tuple<int, int, int, int>; // note index, octave, alteration, staff line

    [[maybe_unused]] static constexpr std::array<Result, 2> expectedConcertResult = {
        Result{5, 4, 0, 1}, // A4
        Result{6, 4, 0, 2}  // B4
    };

    [[maybe_unused]] static const std::array<std::array<Result, 2>, 5> expectedWrittenResults = {{
        { Result{5, 4, 0, 1}, Result{6, 4, 0, 2} },
        { Result{0, 5, 0, 3}, Result{1, 5, 0, 4} },
        { Result{6, 5, 0, 3}, Result{0, 6, 2, 4} },
        { Result{4, 5, -2, 1}, Result{5, 5, -2, 2} },
        { Result{3, 6, 2, 7}, Result{4, 6, 2, 8} },
    }};

    for (Cmper measId = 1; measId <= 5; measId++) {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, measId);
        ASSERT_TRUE(gfhold);

        auto entries = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entries);
        ASSERT_GE(entries->getEntries().size(), 2);

        for (size_t x = 0; x < 2; x++) {
            auto note = NoteInfoPtr(EntryInfoPtr(entries, x), 0);

            auto [wNote, wOctave, wAlter, wStaff] = note.calcNoteProperties();
            auto [cNote, cOctave, cAlter, cStaff] = note.calcNotePropertiesConcert();

            const auto& [expWNote, expWOct, expWAlt, expWStaff] = expectedWrittenResults[measId - 1][x];
            const auto& [expCNote, expCOct, expCAlt, expCStaff] = expectedConcertResult[x];

            // Written checks
            EXPECT_EQ(int(wNote), expWNote) << "Written note mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wOctave, expWOct) << "Written octave mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wAlter, expWAlt) << "Written alteration mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(wStaff, expWStaff) << "Written staff mismatch at entry " << x << " in measure " << measId;

            // Concert checks
            EXPECT_EQ(int(cNote), expCNote) << "Concert note mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cOctave, expCOct) << "Concert octave mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cAlter, expCAlt) << "Concert alteration mismatch at entry " << x << " in measure " << measId;
            EXPECT_EQ(cStaff, expCStaff) << "Concert staff mismatch at entry " << x << " in measure " << measId;

    /*
            // Optional: print results
            static constexpr std::array<char, 7> noteNames = { 'C', 'D', 'E', 'F', 'G', 'A', 'B' };
            std::cout << "Entry index " << x << " measure " << measId << std::endl;
            std::cout << "    W: " << noteNames[int(wNote)] << ", " << wOctave << ", " << wAlter << ", " << wStaff << std::endl;
            std::cout << "    C: " << noteNames[int(cNote)] << ", " << cOctave << ", " << cAlter << ", " << cStaff << std::endl;
    */
        }
    }
}

TEST(StaffTest, PercussionMapStyle)
{
    std::vector<char> transposeXml;
    musxtest::readFile(musxtest::getInputPath() / "percstaffstyle.enigmaxml", transposeXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(transposeXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto staff1 = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 1, 0);
    EXPECT_EQ(staff1->percussionMapId, 12);

    auto staff2 = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 2, 0);
    EXPECT_EQ(staff2->percussionMapId, 25);
}

TEST(StaffTest, NamePositioning)
{
    std::vector<char> transposeXml;
    musxtest::readFile(musxtest::getInputPath() / "namepos.enigmaxml", transposeXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(transposeXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    using Align = others::NamePositioning::AlignJustify;

    auto checkNamePos = [&](InstCmper staffId, MeasCmper measId,
            Evpu expectedFullX, Evpu expectedFullY, Align expectedFullAlign,
            Evpu expectedAbrvX, Evpu expectedAbrvY, Align expectedAbrvAlign) {
        std::string msg = "Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId);
        auto staff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, staffId, measId, 0);
        ASSERT_TRUE(staff) << msg;
        auto namePos = staff->getFullNamePosition();
        EXPECT_EQ(namePos->horzOff, expectedFullX);
        EXPECT_EQ(namePos->vertOff, expectedFullY);
        EXPECT_EQ(namePos->hAlign, expectedFullAlign);
        namePos = staff->getAbbreviatedNamePosition();
        EXPECT_EQ(namePos->horzOff, expectedAbrvX);
        EXPECT_EQ(namePos->vertOff, expectedAbrvY);
        EXPECT_EQ(namePos->hAlign, expectedAbrvAlign);
    };

    // bar 2 (1st system)
    checkNamePos(1, 1, -72, 0, Align::Right, -64, -12, Align::Right);  // forced by staff style "Default Namepos"
    checkNamePos(2, 1, -72, 0, Align::Right, -144, -48, Align::Left);  // staff values (abbrv is overridden in the staff)
    // bar 4 (2nd system)
    checkNamePos(1, 4, -72, -24, Align::Center, -64, -12, Align::Right);  // staff values (full is overridden in the staff)
    checkNamePos(2, 4, -72, 0, Align::Right, -64, -12, Align::Right);  // forced by staff style "Default Namepos"
    // bar 7 (3rd system)
    checkNamePos(1, 7, 0, 24, Align::Right, 0, -60, Align::Right);  // staff style values from "Override staff style"
    checkNamePos(2, 7, 0, 24, Align::Right, 0, -60, Align::Right);  // staff style values from "Override staff style"
    // bar 8 (4th system)
    checkNamePos(1, 8, -72, -24, Align::Center, -64, -12, Align::Right);  // staff values (full is overridden in the staff)
    checkNamePos(2, 8, -72, 0, Align::Right, -144, -48, Align::Left);  // staff values (abbrv is overridden in the staff)
    // bar 11 (5th system)
    checkNamePos(1, 11, -72, -24, Align::Center, -64, -12, Align::Right);  // staff values (full is overridden in the staff)
    checkNamePos(2, 11, -72, 0, Align::Right, -144, -48, Align::Left);  // staff values (abbrv is overridden in the staff)
}
