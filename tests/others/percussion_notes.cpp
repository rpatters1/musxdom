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

TEST(PercussionNoteCodeTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <percussionNoteCode entnum="126" inci="0">
      <noteID>1</noteID>
      <noteCode>11</noteCode>
    </percussionNoteCode>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto pcode = details->get<details::PercussionNoteCode>(SCORE_PARTID, 126, 0);
    ASSERT_TRUE(pcode) << "PercussionNoteCode with entnum=126 inci=0 not found";

    EXPECT_EQ(pcode->noteId, NoteNumber(1));
    EXPECT_EQ(pcode->noteCode, PercussionNoteTypeId(11));
}

constexpr static musxtest::string_view percNoteXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <percussionNoteInfo cmper="4" inci="0">
      <percNoteType>1</percNoteType>
      <harmLev>6</harmLev>
      <closedNotehead>57508</closedNotehead>
      <halfNotehead>57507</halfNotehead>
      <wholeNotehead>57506</wholeNotehead>
      <dwholeNotehead>57504</dwholeNotehead>
    </percussionNoteInfo>
    <percussionNoteInfo cmper="4" inci="1">
      <percNoteType>236</percNoteType>
      <harmLev>7</harmLev>
      <closedNotehead>57508</closedNotehead>
      <halfNotehead>57507</halfNotehead>
      <wholeNotehead>57506</wholeNotehead>
      <dwholeNotehead>57504</dwholeNotehead>
    </percussionNoteInfo>
    <percussionNoteInfo cmper="4" inci="2">
      <percNoteType>8428</percNoteType>
      <harmLev>7</harmLev>
      <closedNotehead>57508</closedNotehead>
      <halfNotehead>57507</halfNotehead>
      <wholeNotehead>57506</wholeNotehead>
      <dwholeNotehead>57504</dwholeNotehead>
    </percussionNoteInfo>
  </others>
</finale>
)xml";

TEST(PercussionNoteInfoTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(percNoteXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto info = others->get<others::PercussionNoteInfo>(SCORE_PARTID, 4, 0);
        ASSERT_TRUE(info);
        EXPECT_EQ(info->percNoteType, PercussionNoteTypeId(1));
        EXPECT_EQ(info->staffPosition, 6);
        EXPECT_EQ(info->closedNotehead, U'\uE0A4');  // 57508
        EXPECT_EQ(info->halfNotehead, U'\uE0A3');    // 57507
        EXPECT_EQ(info->wholeNotehead, U'\uE0A2');   // 57506
        EXPECT_EQ(info->dwholeNotehead, U'\uE0A0');  // 57504
    }

    {
        auto info = others->get<others::PercussionNoteInfo>(SCORE_PARTID, 4, 1);
        ASSERT_TRUE(info);
        EXPECT_EQ(info->percNoteType, PercussionNoteTypeId(236));
        EXPECT_EQ(info->staffPosition, 7);
        EXPECT_EQ(info->closedNotehead, U'\uE0A4');  // 57508
        EXPECT_EQ(info->halfNotehead, U'\uE0A3');    // 57507
        EXPECT_EQ(info->wholeNotehead, U'\uE0A2');   // 57506
        EXPECT_EQ(info->dwholeNotehead, U'\uE0A0');  // 57504
    }
}


TEST(PercussionNoteInfoTest, PercussionNoteTypes)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(percNoteXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto info = others->get<others::PercussionNoteInfo>(SCORE_PARTID, 4, 0);
        ASSERT_TRUE(info);
        auto typeInfo = info->getNoteType();
        EXPECT_EQ(typeInfo.instrumentId, info->percNoteType);
        EXPECT_EQ(typeInfo.instrumentId, info->getBaseNoteTypeId());
        EXPECT_EQ(typeInfo.createName(info->getNoteTypeOrderId()), "Snare Drum");
    }

    {
        auto info = others->get<others::PercussionNoteInfo>(SCORE_PARTID, 4, 1);
        ASSERT_TRUE(info);
        auto typeInfo = info->getNoteType();
        EXPECT_EQ(typeInfo.instrumentId, info->percNoteType);
        EXPECT_EQ(typeInfo.instrumentId, info->getBaseNoteTypeId());
        EXPECT_EQ(typeInfo.createName(info->getNoteTypeOrderId()), "Snare Drum LH");
    }

    {
        auto info = others->get<others::PercussionNoteInfo>(SCORE_PARTID, 4, 2);
        ASSERT_TRUE(info);
        auto typeInfo = info->getNoteType();
        EXPECT_NE(typeInfo.instrumentId, info->percNoteType);
        EXPECT_EQ(typeInfo.instrumentId, info->getBaseNoteTypeId());
        EXPECT_EQ(typeInfo.createName(info->getNoteTypeOrderId()), "Snare Drum (3) LH");
    }
}

TEST(StaffTest, CalcPercussionNoteInfo)
{
    std::vector<char> transposeXml;
    musxtest::readFile(musxtest::getInputPath() / "drumset.enigmaxml", transposeXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(transposeXml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto checkNoteInfo = [&](const NoteInfoPtr& noteInfo, bool expectedSuccess, int expectedStaffRefPos, char32_t expectedClosed) {
        auto percNoteInfo = noteInfo.calcPercussionNoteInfo();
        EXPECT_EQ(bool(percNoteInfo), expectedSuccess);
        if (!percNoteInfo) {
            return;
        }
        EXPECT_EQ(percNoteInfo->calcStaffReferencePosition(), expectedStaffRefPos);
        EXPECT_EQ(percNoteInfo->closedNotehead, expectedClosed);
    };

    {
        auto gfHold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfHold);
        auto entryFrame = gfHold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 3);
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 0), 0), true, -7, U'\uE0A4');
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0), true, -4, U'\uE0BE');
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0), false, 0, 0);   // this note is invalid in the perc map
    }

    {
        auto gfHold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfHold);
        auto entryFrame = gfHold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 3);
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 0), 0), false, 0, 0);
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 1), 0), false, 0, 0);
        checkNoteInfo(NoteInfoPtr(EntryInfoPtr(entryFrame, 2), 0), false, 0, 0);
    }
}
