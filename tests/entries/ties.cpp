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

static void checkTie(const NoteInfoPtr& fromNote, const NoteInfoPtr& toNoteExpected, const std::optional<NoteInfoPtr>& backNoteExpected = std::nullopt, bool requireTie = true)
{
    ASSERT_TRUE(fromNote || toNoteExpected) << " null passed for both fromNote and toNoteExpected";

    auto entryInfoForMsg = (fromNote ? fromNote : toNoteExpected).getEntryInfo();
    std::string msg = "Staff " + std::to_string(entryInfoForMsg.getStaff()) + " measure " + std::to_string(entryInfoForMsg.getMeasure())
        + " from entry index " + std::to_string(entryInfoForMsg.getIndexInFrame());

    if (fromNote) {
        auto toNote = fromNote.calcTieTo();
        if (!toNoteExpected) {
            EXPECT_FALSE(toNote) << msg << " unexpected no tied to note";
            EXPECT_FALSE(toNoteExpected.isSameNote(toNote)) << msg << " unexpected isSameNote to return false";
        } else {
            EXPECT_TRUE(toNoteExpected.isSameNote(toNote)) << msg << " unexpected tied to note";
            ASSERT_TRUE(toNote) << msg << " unexpected tied to note";
            auto expectedFromNote = toNote.calcTieFrom(requireTie);
            if (backNoteExpected.has_value()) {
                EXPECT_EQ(bool(expectedFromNote), bool(backNoteExpected.value())) << msg << " expected and calculated back notes both null or not null";
                if (backNoteExpected.value()) {
                    ASSERT_TRUE(expectedFromNote) << msg << " expected from note was null";
                    EXPECT_TRUE(backNoteExpected.value().isSameNote(expectedFromNote)) << msg << " unexpected tied from note";
                }
            } else {
                ASSERT_TRUE(expectedFromNote) << msg << " expected from note was null";
                EXPECT_TRUE(fromNote.isSameNote(expectedFromNote)) << msg << " unexpected tied from note";
            }
        }
    } else {
        auto fromNoteExpected = toNoteExpected.calcTieFrom();
        EXPECT_FALSE(fromNoteExpected);
    }
}

static NoteInfoPtr createNoteInfo(const std::shared_ptr<const EntryFrame>& entryFrame, size_t entryIndex, size_t noteIndex)
{
    return NoteInfoPtr(EntryInfoPtr(entryFrame, entryIndex), noteIndex);
}

TEST(TieDetection, TiesInMeasure)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 0, 0), createNoteInfo(entryFrame, 2, 0));
    checkTie(createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 2, 1), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 2, 1), std::nullopt, false);
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 1, 0));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 2, 2));

    checkTie(createNoteInfo(entryFrame, 4, 0), createNoteInfo(entryFrame, 5, 0));
    checkTie(createNoteInfo(entryFrame, 4, 1), createNoteInfo(entryFrame, 5, 1));
    checkTie(createNoteInfo(entryFrame, 4, 2), createNoteInfo(entryFrame, 5, 2));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 5, 3));
}

TEST(TieDetection, V1V2TiesInMeasure2)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 4, 0));
    checkTie(createNoteInfo(entryFrame, 1, 1), createNoteInfo(entryFrame, 4, 1));
    checkTie(createNoteInfo(entryFrame, 2, 0), NoteInfoPtr());
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 2, 0));
    checkTie(createNoteInfo(entryFrame, 3, 0), createNoteInfo(entryFrame, 4, 2));

    checkTie(createNoteInfo(entryFrame, 4, 1), NoteInfoPtr());
}

TEST(TieDetection, V1V2TiesAcrossMeasure3_5)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold3 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
    ASSERT_TRUE(gfhold3) << " gfhold not found for 1, 2";
    auto entryFrame3 = gfhold3.createEntryFrame(0);
    ASSERT_TRUE(entryFrame3);

    auto gfhold4 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);
    ASSERT_TRUE(gfhold4) << " gfhold not found for 1, 4";
    auto entryFrame4 = gfhold4.createEntryFrame(0);
    ASSERT_TRUE(entryFrame4);

    checkTie(createNoteInfo(entryFrame3, 2, 0), createNoteInfo(entryFrame4, 0, 0));
    checkTie(createNoteInfo(entryFrame3, 3, 0), createNoteInfo(entryFrame4, 0, 1));

    auto gfhold5 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
    ASSERT_TRUE(gfhold5) << " gfhold not found for 1, 5";
    auto entryFrame5 = gfhold5.createEntryFrame(0);
    ASSERT_TRUE(entryFrame5);

    checkTie(createNoteInfo(entryFrame4, 2, 0), createNoteInfo(entryFrame5, 1, 0));
    checkTie(createNoteInfo(entryFrame4, 3, 0), createNoteInfo(entryFrame5, 0, 0));
}

TEST(TieDetection, V1V2TiesAcrossMeasure6_7)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold6 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
    ASSERT_TRUE(gfhold6) << " gfhold not found for 1, 6";
    auto entryFrame6 = gfhold6.createEntryFrame(0);
    ASSERT_TRUE(entryFrame6);

    auto gfhold7 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
    ASSERT_TRUE(gfhold7) << " gfhold not found for 1, 7";
    auto entryFrame7 = gfhold7.createEntryFrame(0);
    ASSERT_TRUE(entryFrame7);

    checkTie(createNoteInfo(entryFrame6, 2, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame6, 3, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame6, 4, 0), createNoteInfo(entryFrame7, 1, 0));

    checkTie(createNoteInfo(entryFrame7, 3, 0), createNoteInfo(entryFrame7, 5, 0));
    checkTie(createNoteInfo(entryFrame7, 4, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame7, 6, 0), NoteInfoPtr());
}

TEST(TieDetection, V1V2TiesAcrossMeasure8_9)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold8 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 8);
    ASSERT_TRUE(gfhold8) << " gfhold not found for 1, 8";
    auto entryFrame8 = gfhold8.createEntryFrame(0);
    ASSERT_TRUE(entryFrame8);

    auto gfhold9 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 9);
    ASSERT_TRUE(gfhold9) << " gfhold not found for 1, 9";
    auto entryFrame9 = gfhold9.createEntryFrame(0);
    ASSERT_TRUE(entryFrame9);

    checkTie(createNoteInfo(entryFrame8, 2, 0), createNoteInfo(entryFrame9, 0, 0));
    checkTie(createNoteInfo(entryFrame8, 3, 0), NoteInfoPtr());
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame8, 3, 0));
}

TEST(TieDetection, V1V2TiesInMeasure10)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 10);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 10";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 0, 0), createNoteInfo(entryFrame, 4, 0), createNoteInfo(entryFrame, 3, 0));
    checkTie(createNoteInfo(entryFrame, 1, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 3, 0), createNoteInfo(entryFrame, 4, 0));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 3, 0));
}

TEST(TieDetection, NoTiesInMeasure11)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 11);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 11";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 1, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 2, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 3, 0), NoteInfoPtr());
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 3, 0));
    checkTie(createNoteInfo(entryFrame, 3, 0), NoteInfoPtr());
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 4, 0));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 5, 0));
}

TEST(TieDetection, DuplicateNotes)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties_duplicate_notes.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkTie(createNoteInfo(entryFrame, 0, 0), createNoteInfo(entryFrame, 1, 0));
        checkTie(createNoteInfo(entryFrame, 0, 1), createNoteInfo(entryFrame, 1, 1));
        checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 1, 2));
        checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 3, 0));
        checkTie(createNoteInfo(entryFrame, 2, 0), createNoteInfo(entryFrame, 3, 1));
        checkTie(createNoteInfo(entryFrame, 2, 1), createNoteInfo(entryFrame, 3, 2));
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkTie(createNoteInfo(entryFrame, 0, 0), createNoteInfo(entryFrame, 1, 0));
        checkTie(createNoteInfo(entryFrame, 0, 1), createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 0, 0));
    }
}

TEST(TieDetection, V1toV2andV2toV1)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "v1v2Ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold1 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold1) << " gfhold not found for 1, 1";
    auto entryFrame1 = gfhold1.createEntryFrame(0);
    ASSERT_TRUE(entryFrame1);

    auto gfhold2 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold2) << " gfhold not found for 1, 2";
    auto entryFrame2 = gfhold2.createEntryFrame(0);
    ASSERT_TRUE(entryFrame2);

    checkTie(createNoteInfo(entryFrame1, 2, 0), createNoteInfo(entryFrame2, 0, 0));
    checkTie(createNoteInfo(entryFrame1, 2, 1), createNoteInfo(entryFrame2, 0, 1));
    checkTie(createNoteInfo(entryFrame1, 5, 0), createNoteInfo(entryFrame2, 0, 2));

    auto gfhold3 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
    ASSERT_TRUE(gfhold3) << " gfhold not found for 1, 3";
    auto entryFrame3 = gfhold3.createEntryFrame(0);
    ASSERT_TRUE(entryFrame3);

    checkTie(createNoteInfo(entryFrame2, 2, 0), createNoteInfo(entryFrame3, 1, 0));
    checkTie(createNoteInfo(entryFrame2, 2, 1), createNoteInfo(entryFrame3, 0, 0));
    checkTie(createNoteInfo(entryFrame2, 2, 2), createNoteInfo(entryFrame3, 0, 1));
}

TEST(TieDetection, V2toV1Special)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "v1v2Ties2.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold1 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold1) << " gfhold not found for 1, 1";
    auto entryFrame1 = gfhold1.createEntryFrame(0);
    ASSERT_TRUE(entryFrame1);

    auto gfhold2 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold2) << " gfhold not found for 1, 2";
    auto entryFrame2 = gfhold2.createEntryFrame(0);
    ASSERT_TRUE(entryFrame2);

    checkTie(createNoteInfo(entryFrame1, 8, 0), createNoteInfo(entryFrame1, 13, 0));
    checkTie(createNoteInfo(entryFrame1, 12, 0), createNoteInfo(entryFrame2, 0, 2));
    checkTie(createNoteInfo(entryFrame1, 13, 0), createNoteInfo(entryFrame2, 0, 0));
    checkTie(createNoteInfo(entryFrame1, 13, 1), createNoteInfo(entryFrame2, 0, 1));
}

TEST(TieDetection, GraceTieToEmptyMeasure)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "grace_tie_to_nothing.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 2, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 3, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 4, 0), NoteInfoPtr());
    checkTie(createNoteInfo(entryFrame, 5, 0), NoteInfoPtr());
}

TEST(TieDetection, AcrossKeyChange)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tie_across_key.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold1 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold1) << " gfhold not found for 1, 1";
    auto entryFrame1 = gfhold1.createEntryFrame(0);
    ASSERT_TRUE(entryFrame1);

    auto gfhold2 = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold2) << " gfhold not found for 1, 2";
    auto entryFrame2 = gfhold2.createEntryFrame(0);
    ASSERT_TRUE(entryFrame2);

    checkTie(createNoteInfo(entryFrame1, 1, 0), createNoteInfo(entryFrame2, 0, 0));
}

TEST(TieDetection, ShapeTies)
{
    using PseudoTieMode = musx::utils::PseudoTieMode;

    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "slur_ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    constexpr size_t EXPECTED_ELEMENTS = 8;
    constexpr std::array<Cmper, EXPECTED_ELEMENTS> expectedCmpers = { 2, 3, 4, 5, 6, 7, 9, 10 };
    constexpr std::array<bool, EXPECTED_ELEMENTS> expectedLv = { false, false, true, true, false, false, false, false };
    constexpr std::array<bool, EXPECTED_ELEMENTS> expectedArp = { true, true, false, false, false, false, false, false };
    constexpr std::array<bool, EXPECTED_ELEMENTS> expectedTieEnd = { false, false, false, false, false, false, true, true };
    constexpr std::array<size_t, EXPECTED_ELEMENTS> expectedEntryIdx = { 1, 2, 2, 0, 2, 1, 1, 3 }; // measure order 2, 2, 3, 1, 1, 4, 5, 5
    constexpr size_t NA = (std::numeric_limits<size_t>::max)();
    constexpr std::array<size_t, EXPECTED_ELEMENTS> expectedNoteIdx = { 0, 1, NA, NA, NA, NA, NA, NA };

    auto smartShapes = doc->getOthers()->getArray<others::SmartShape>(SCORE_PARTID);
    ASSERT_EQ(smartShapes.size(), EXPECTED_ELEMENTS) << "expected " << EXPECTED_ELEMENTS << " smart shapes but got " << smartShapes.size();

    size_t x = 0;
    for (const auto& smartShape : smartShapes) {
        ASSERT_LT(x, EXPECTED_ELEMENTS);
        EXPECT_EQ(smartShape->getCmper(), expectedCmpers[x]) << "expected cmper " << expectedCmpers[x] << " but got " << smartShape->getCmper();
        auto startEntry = smartShape->startTermSeg->endPoint->calcAssociatedEntry();
        ASSERT_TRUE(startEntry) << "start entry not found";
        EXPECT_EQ(startEntry.getIndexInFrame(), expectedEntryIdx[x]) << "start entry is not expected entry";
        EXPECT_EQ(smartShape->calcIsPseudoTie(PseudoTieMode::LaissezVibrer, startEntry), expectedLv[x])
            << "lv tie value not the expected value for " << smartShape->getCmper();
        EXPECT_EQ(smartShape->calcIsPseudoTie(PseudoTieMode::TieEnd, startEntry), expectedTieEnd[x])
            << "tie end value not the expected value for " << smartShape->getCmper();
        auto arpTieEnd = smartShape->calcArpeggiatedTieToNote(startEntry);
        EXPECT_EQ(bool(arpTieEnd), expectedArp[x]) << "arppegio tie value not the expected value for " << smartShape->getCmper();
        if (arpTieEnd) {
            EXPECT_EQ(arpTieEnd.getEntryInfo().getMeasure(), 3);
            EXPECT_EQ(arpTieEnd.getEntryInfo().getIndexInFrame(), 0);
            EXPECT_EQ(arpTieEnd.getNoteIndex(), expectedNoteIdx[x]);
        }
        x++;
    }
}

TEST(TieDetection, SlursAsLvTies)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "lvslurs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        auto entryInfo = EntryInfoPtr(entryFrame, 1);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 2)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 1);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
}

TEST(TieDetection, ShapesAsLvTies)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "lvshapes.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // lv using shape expressions
        auto entryInfo = EntryInfoPtr(entryFrame, 1);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 2)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // lv using articulation
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 1);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // first chord uses articulations but only two. 3 are required.
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        // second chord has lv using articulations
        entryInfo = EntryInfoPtr(entryFrame, 2);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using exprs
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 6";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using artics
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 7";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using smart slurs
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoLvTie(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
}

TEST(TieDetection, ShapesAsTieEnds)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "lvshapes.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // lv using shape expressions
        auto entryInfo = EntryInfoPtr(entryFrame, 1);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 2)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // lv using articulation
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 1);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // first chord uses articulations but only two. 3 are required.
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 3);
        CurveContourDirection tieDirection = CurveContourDirection::Up;
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 2)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        // second chord has lv using articulations
        entryInfo = EntryInfoPtr(entryFrame, 2);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
        EXPECT_FALSE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Unspecified);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using exprs
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 6";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using artics
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 7";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // tie-ends using smart slurs
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        ASSERT_TRUE(entryInfo);
        ASSERT_EQ(entryInfo->getEntry()->notes.size(), 2);
        CurveContourDirection tieDirection = CurveContourDirection::Unspecified;
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 0)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Down);
        EXPECT_TRUE((NoteInfoPtr(entryInfo, 1)).calcHasPseudoTieEnd(&tieDirection));
        EXPECT_EQ(tieDirection, CurveContourDirection::Up);
    }
}

TEST(TieDetection, JumpTieContinuations)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "crazy_jumps.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";

    EntryInfoPtr startEntry;
    for (LayerIndex layerIndex = 0; layerIndex < 4 && !startEntry; ++layerIndex) {
        auto entryFrame = gfhold.createEntryFrame(layerIndex);
        if (!entryFrame) {
            continue;
        }
        for (size_t entryIndex = 0; entryIndex < entryFrame->getEntries().size(); ++entryIndex) {
            EntryInfoPtr entryInfo(entryFrame, entryIndex);
            if (!entryInfo || !entryInfo->getEntry()->isNote || entryInfo.getMeasure() != 3) {
                continue;
            }
            if (entryInfo->elapsedDuration != musx::util::Fraction()) {
                continue;
            }
            if (entryInfo->getEntry()->notes.size() >= 2) {
                startEntry = entryInfo;
                break;
            }
        }
    }
    ASSERT_TRUE(startEntry) << " chord entry not found at measure start";

    auto findNoteByPitch = [&](int harmLev, int harmAlt) -> NoteInfoPtr {
        const auto entry = startEntry->getEntry();
        for (size_t noteIndex = 0; noteIndex < entry->notes.size(); ++noteIndex) {
            if (entry->notes[noteIndex]->harmLev == harmLev && entry->notes[noteIndex]->harmAlt == harmAlt) {
                return NoteInfoPtr(startEntry, noteIndex);
            }
        }
        return {};
    };

    auto collectMeasures = [](const std::vector<std::pair<NoteInfoPtr, CurveContourDirection>>& results) {
        std::vector<MeasCmper> measures;
        measures.reserve(results.size());
        for (const auto& [note, direction] : results) {
            if (!note) {
                ADD_FAILURE() << "null note in jump tie continuation results";
                continue;
            }
            measures.push_back(note.getEntryInfo().getMeasure());
            (void)direction;
        }
        return measures;
    };

    const auto cNote = findNoteByPitch(0, 0);
    ASSERT_TRUE(cNote);
    auto cResults = cNote.calcJumpTieContinuationsFrom();
    std::vector<MeasCmper> expectedCMeasures{ 4, 5 };
    EXPECT_EQ(collectMeasures(cResults), expectedCMeasures);

    const auto eNote = findNoteByPitch(2, 0);
    ASSERT_TRUE(eNote);
    auto eResults = eNote.calcJumpTieContinuationsFrom();
    std::vector<MeasCmper> expectedEMeasures{ 8, 2 };
    EXPECT_EQ(collectMeasures(eResults), expectedEMeasures);
}

TEST(TieDetection, JumpTieTargetTypes)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tie_target_types.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 4";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    EntryInfoPtr chordEntry;
    for (size_t entryIndex = 0; entryIndex < entryFrame->getEntries().size(); ++entryIndex) {
        EntryInfoPtr entryInfo(entryFrame, entryIndex);
        if (!entryInfo || !entryInfo->getEntry()->isNote || entryInfo.getMeasure() != 4) {
            continue;
        }
        if (entryInfo->elapsedDuration != musx::util::Fraction()) {
            continue;
        }
        if (entryInfo->getEntry()->notes.size() >= 3) {
            chordEntry = entryInfo;
            break;
        }
    }
    ASSERT_TRUE(chordEntry) << " chord entry not found at measure start";

    auto findNoteByPitch = [&](int harmLev, int harmAlt) -> NoteInfoPtr {
        const auto entry = chordEntry->getEntry();
        for (size_t noteIndex = 0; noteIndex < entry->notes.size(); ++noteIndex) {
            if (entry->notes[noteIndex]->harmLev == harmLev && entry->notes[noteIndex]->harmAlt == harmAlt) {
                return NoteInfoPtr(chordEntry, noteIndex);
            }
        }
        return {};
    };

    auto collectMeasures = [](const std::vector<std::pair<NoteInfoPtr, CurveContourDirection>>& results) {
        std::vector<MeasCmper> measures;
        measures.reserve(results.size());
        for (const auto& [note, direction] : results) {
            if (!note) {
                ADD_FAILURE() << "null note in jump tie continuation results";
                continue;
            }
            measures.push_back(note.getEntryInfo().getMeasure());
            (void)direction;
        }
        return measures;
    };

    const auto eNote = findNoteByPitch(2, 0);
    ASSERT_TRUE(eNote);
    auto eResults = eNote.calcJumpTieContinuationsFrom();
    std::vector<MeasCmper> expectedEMeasures{ 2 };
    EXPECT_EQ(collectMeasures(eResults), expectedEMeasures);

    const auto gNote = findNoteByPitch(4, 0);
    ASSERT_TRUE(gNote);
    auto gResults = gNote.calcJumpTieContinuationsFrom();
    std::vector<MeasCmper> expectedGMeasures{ 2 };
    EXPECT_EQ(collectMeasures(gResults), expectedGMeasures);

    const auto cNote = findNoteByPitch(7, 0);
    ASSERT_TRUE(cNote);
    auto cResults = cNote.calcJumpTieContinuationsFrom();
    std::vector<MeasCmper> expectedCMeasures{ 2 };
    EXPECT_EQ(collectMeasures(cResults), expectedCMeasures);
}

TEST(TieDetection, ArpeggioTieAcrossGap)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tie_across_gap.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 2, 1";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);
    {
        auto noteInfo = NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0);
        EXPECT_TRUE(noteInfo.calcArpeggiatedTieToNote());
    }
    {
        auto noteInfo = NoteInfoPtr(EntryInfoPtr(entryFrame, 3), 0);
        EXPECT_TRUE(noteInfo.calcArpeggiatedTieToNote());
    }
}

TEST(TieDetection, TieDirectionChordsSeconds)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tied_chords.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // forward
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0).calcEffectiveTieDirection(), CurveContourDirection::Down);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 1).calcEffectiveTieDirection(), CurveContourDirection::Down);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 2).calcEffectiveTieDirection(), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 3).calcEffectiveTieDirection(), CurveContourDirection::Up);
        // backward
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0).calcEffectiveTieDirection(true), CurveContourDirection::Down);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 1).calcEffectiveTieDirection(true), CurveContourDirection::Down);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 2).calcEffectiveTieDirection(true), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 3).calcEffectiveTieDirection(true), CurveContourDirection::Up);
    }
    {
        // entries have flipTie set, so default direction is towards stems
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        // forward
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0).calcEffectiveTieDirection(), CurveContourDirection::Down); // (the tie-start is overridden)
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 1).calcEffectiveTieDirection(), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 2).calcEffectiveTieDirection(), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 3).calcEffectiveTieDirection(), CurveContourDirection::Up);
        // backward
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0).calcEffectiveTieDirection(true), CurveContourDirection::Up); // (the tie-end is not overridden)
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 1).calcEffectiveTieDirection(true), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 2).calcEffectiveTieDirection(true), CurveContourDirection::Up);
        EXPECT_EQ(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 3).calcEffectiveTieDirection(true), CurveContourDirection::Up);
    }
}

TEST(TieDetection, TieDirectionOppositeStems)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tied_opposite_stems.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto checkTieDirections = [](const NoteInfoPtr& startNote, CurveContourDirection expectedDir) {
        ASSERT_TRUE(startNote);
        EXPECT_EQ(startNote.calcEffectiveTieDirection(), expectedDir);
        auto endNote = startNote.calcTieTo();
        ASSERT_TRUE(endNote);
        EXPECT_EQ(endNote.calcEffectiveTieDirection(true), expectedDir);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 0), 1), CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0), CurveContourDirection::Up);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0), CurveContourDirection::Down);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 1), CurveContourDirection::Up);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 2), CurveContourDirection::Up);
        checkTieDirections(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 3), CurveContourDirection::Up);
    }
}

TEST(TieDetection, TieConnectStyleType)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tied_opposite_stems.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto checkTie = [](const NoteInfoPtr& startNote, TieConnectStyleType expectedStart, TieConnectStyleType expectedEnd) {
        ASSERT_TRUE(startNote);
        auto styles = musx::util::Tie::calcConnectStyleTypes(startNote, false);
        ASSERT_TRUE(styles);
        const auto& [startStyle, endStyle] = *styles;
        EXPECT_EQ(startStyle, expectedStart);
        EXPECT_EQ(endStyle, expectedEnd);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 0), 1), TieConnectStyleType::OverHighestNoteStemStartPosOver, TieConnectStyleType::OverHighestNoteEndPosOver);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0), TieConnectStyleType::OverStartPosInner, TieConnectStyleType::OverEndPosInner);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0), TieConnectStyleType::UnderLowestNoteStartPosUnder, TieConnectStyleType::UnderLowestNoteEndPosUnder);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 1), TieConnectStyleType::OverStartPosInner, TieConnectStyleType::OverEndPosInner);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 2), TieConnectStyleType::OverStartPosInner, TieConnectStyleType::OverEndPosInner);
        checkTie(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 3), TieConnectStyleType::OverHighestNoteStartPosOver, TieConnectStyleType::OverHighestNoteEndPosOver);
    }
}
