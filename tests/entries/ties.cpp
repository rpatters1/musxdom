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


static void checkTie(const NoteInfoPtr& fromNote, const NoteInfoPtr& toNoteExpected)
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
            auto expectedFromNote = toNote.calcTieFrom();
            ASSERT_TRUE(expectedFromNote) << msg << " expected from note was null";
            EXPECT_TRUE(fromNote.isSameNote(expectedFromNote)) << msg << " unexpected tied from note";
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

    auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 1";
    auto entryFrame = gfhold->createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 0, 0), createNoteInfo(entryFrame, 2, 0));
    checkTie(createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 2, 1));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 1, 0));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 2, 2));

    checkTie(createNoteInfo(entryFrame, 4, 0), createNoteInfo(entryFrame, 5, 0));
    checkTie(createNoteInfo(entryFrame, 4, 1), createNoteInfo(entryFrame, 5, 1));
    checkTie(createNoteInfo(entryFrame, 4, 2), createNoteInfo(entryFrame, 5, 2));
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 5, 3));
}

TEST(TieDetection, V1V2TiesInMeasure)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
    auto entryFrame = gfhold->createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkTie(createNoteInfo(entryFrame, 1, 0), createNoteInfo(entryFrame, 4, 0));
    checkTie(createNoteInfo(entryFrame, 1, 1), createNoteInfo(entryFrame, 4, 1));
    checkTie(createNoteInfo(entryFrame, 2, 0), NoteInfoPtr());
    checkTie(NoteInfoPtr(), createNoteInfo(entryFrame, 2, 0));
    checkTie(createNoteInfo(entryFrame, 3, 0), createNoteInfo(entryFrame, 4, 2));

    checkTie(createNoteInfo(entryFrame, 4, 1), NoteInfoPtr());
}

TEST(TieDetection, V1V2TiesAcrossMeasure)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "ties.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold3 = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 3);
    ASSERT_TRUE(gfhold3) << " gfhold not found for 1, 2";
    auto entryFrame3 = gfhold3->createEntryFrame(0);
    ASSERT_TRUE(entryFrame3);

    auto gfhold4 = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 4);
    ASSERT_TRUE(gfhold4) << " gfhold not found for 1, 4";
    auto entryFrame4 = gfhold4->createEntryFrame(0);
    ASSERT_TRUE(entryFrame4);

    checkTie(createNoteInfo(entryFrame3, 2, 0), createNoteInfo(entryFrame4, 0, 0));
    checkTie(createNoteInfo(entryFrame3, 3, 0), createNoteInfo(entryFrame4, 0, 1));

    auto gfhold5 = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 5);
    ASSERT_TRUE(gfhold5) << " gfhold not found for 1, 5";
    auto entryFrame5 = gfhold5->createEntryFrame(0);
    ASSERT_TRUE(entryFrame5);

    checkTie(createNoteInfo(entryFrame4, 2, 0), createNoteInfo(entryFrame5, 1, 0));
    checkTie(createNoteInfo(entryFrame4, 3, 0), createNoteInfo(entryFrame5, 0, 0));
}
