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

static void checkCrossStaff(const NoteInfoPtr& note, InstCmper expectedStaff, ClefIndex expectedClef, int expectedStaffPos)
{
    ASSERT_TRUE(note);
    auto entryInfo = note.getEntryInfo();
    std::string msg = "Staff " + std::to_string(entryInfo.getStaff()) + " measure " + std::to_string(entryInfo.getMeasure())
        + " from entry index " + std::to_string(entryInfo.getIndexInFrame());

    EXPECT_EQ(note.calcStaff(), expectedStaff) << msg << " staff does not match";
    auto staff = note.getEntryInfo().createCurrentStaff(expectedStaff);
    ASSERT_TRUE(staff) << msg << " unable to create current staff";
    EXPECT_EQ(staff->calcClefIndexAt(note.getEntryInfo().getMeasure(), note.getEntryInfo()->elapsedDuration.calcEduDuration()), expectedClef)
        << msg << " clef does note match";
    EXPECT_EQ(std::get<3>(note.calcNoteProperties()), expectedStaffPos) << msg << " staff position does not match";
}

static NoteInfoPtr createNoteInfo(const std::shared_ptr<const EntryFrame>& entryFrame, size_t entryIndex, size_t noteIndex)
{
    return NoteInfoPtr(EntryInfoPtr(entryFrame, entryIndex), noteIndex);
}

TEST(CrossStaff, Measure3)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "cross_staffs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);


    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 3";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkCrossStaff(createNoteInfo(entryFrame, 1, 0), 2, 3, 0);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 1), 1, 0, -7);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 2), 1, 0, -5);
}

TEST(CrossStaff, Measure4)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "cross_staffs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);


    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 4";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkCrossStaff(createNoteInfo(entryFrame, 0, 0), 2, 3, 0);
    checkCrossStaff(createNoteInfo(entryFrame, 0, 1), 1, 0, -7);
    checkCrossStaff(createNoteInfo(entryFrame, 0, 2), 1, 0, -5);

    checkCrossStaff(createNoteInfo(entryFrame, 1, 0), 2, 1, -6);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 1), 1, 0, -7);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 2), 1, 0, -5);
}

TEST(CrossStaff, Measure5)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "cross_staffs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);


    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);    ASSERT_TRUE(gfhold) << " gfhold not found for 1, 5";
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);

    checkCrossStaff(createNoteInfo(entryFrame, 1, 0), 2, 1, -6);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 1), 1, 0, -7);
    checkCrossStaff(createNoteInfo(entryFrame, 1, 2), 1, 0, -5);
}
