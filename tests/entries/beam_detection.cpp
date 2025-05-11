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

static void checkEntry(const std::shared_ptr<const EntryFrame>& entryFrame, size_t index, bool unbeamed, bool beamStart, size_t beamEndIndex)
{
    EntryInfoPtr entryInfo(entryFrame, index);
    ASSERT_TRUE(entryInfo);

    std::string msg = "Staff " + std::to_string(entryInfo.getStaff()) + " measure " + std::to_string(entryInfo.getMeasure())
        + " entry index " + std::to_string(entryInfo.getIndexInFrame());
    EXPECT_EQ(entryInfo.calcUnbeamed(), unbeamed) << msg << " unbeamed doesn't match";
    EXPECT_EQ(entryInfo.calcIsBeamStart(), beamStart) << msg << " beam start doesn't match";
    auto beamEnd = entryInfo.findBeamEnd();
    EXPECT_TRUE(unbeamed || beamEnd) << msg << " unbeamed and beamEnd are not compatible";
    if (beamEnd) {
        EXPECT_EQ(beamEnd.getIndexInFrame(), beamEndIndex) << msg << " beam end indices do not match ("
            << beamEnd.getIndexInFrame() << " vs. " << beamEndIndex << ".)";
    }
}

static void expectEntriesInBeam(const std::shared_ptr<const EntryFrame>& entryFrame, const std::vector<size_t>& expectedIndices, bool includeHidden = false)
{
    ASSERT_FALSE(expectedIndices.empty());
    auto next = EntryInfoPtr(entryFrame, expectedIndices[0]);
    ASSERT_TRUE(next);
    bool isStart = next.calcIsBeamStart();
    EXPECT_TRUE(isStart) << "entry is not start of beam";
    if (!isStart) return;
    size_t x = 0;
    while (next) {
        EXPECT_TRUE(x < expectedIndices.size()) << "number of entries in forward iteration exceeds number of expected values";
        if (x >= expectedIndices.size()) return;
        EXPECT_EQ(next.getIndexInFrame(), expectedIndices[x]) << "beam index " << x << " is not the expected value in forward iteration";
        next = next.getNextInBeamGroup(includeHidden);
        x++;
    }
    EXPECT_EQ(x, expectedIndices.size()) << "forward interation did not find enough values";
    x = expectedIndices.size() - 1;
    auto prev = EntryInfoPtr(entryFrame, expectedIndices[x]);
    ASSERT_TRUE(prev);
    while (prev) {
        // decrementing x past 0 causes x > size()
        EXPECT_TRUE(x < expectedIndices.size()) << "number of entries in backward iteration exceeds number of expected values";
        if (x >= expectedIndices.size()) return;
        EXPECT_EQ(prev.getIndexInFrame(), expectedIndices[x]) << "beam index " << x << " is not the expected value in backward iteration";
        prev = prev.getPreviousInBeamGroup(includeHidden);
        x--;
    }
    EXPECT_EQ(x + 1, 0) << "backward interation did not find enough values";
}

TEST(BeamDetection, PrimaryNoIncludeRests)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, false, true, 11);     // start
        checkEntry(entryFrame, 1, false, true, 5);      // start
        checkEntry(entryFrame, 2, false, false, 5);
        checkEntry(entryFrame, 3, false, true, 4);      // start
        checkEntry(entryFrame, 4, false, false, 4);     // end
        checkEntry(entryFrame, 5, false, false, 5);     // end
        checkEntry(entryFrame, 6, true, false, 0);
        checkEntry(entryFrame, 7, false, false, 11);
        checkEntry(entryFrame, 8, false, true, 10);     // start
        checkEntry(entryFrame, 9, false, false, 10);
        checkEntry(entryFrame, 10, false, false, 10);   // end
        checkEntry(entryFrame, 11, false, false, 11);   // end
        checkEntry(entryFrame, 12, true, false, 0);
        checkEntry(entryFrame, 13, true, false, 0);
        checkEntry(entryFrame, 14, false, true, 15);    // start
        checkEntry(entryFrame, 15, false, false, 15);   // end
        checkEntry(entryFrame, 16, true, false, 0);
        checkEntry(entryFrame, 17, true, false, 0);

        expectEntriesInBeam(entryFrame, { 0, 7, 11 });
        expectEntriesInBeam(entryFrame, { 1, 2, 5 });
        expectEntriesInBeam(entryFrame, { 3, 4 });
        expectEntriesInBeam(entryFrame, { 8, 9, 10 });
        expectEntriesInBeam(entryFrame, { 14, 15 });
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 2, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, false, true, 2);      // start
        checkEntry(entryFrame, 2, false, false, 2);     // end
        checkEntry(entryFrame, 3, true, false, 0);
        checkEntry(entryFrame, 4, false, true, 5);      // start
        checkEntry(entryFrame, 5, false, false, 5);     // end
        checkEntry(entryFrame, 6, true, false, 0);
        checkEntry(entryFrame, 7, true, false, 0);
        checkEntry(entryFrame, 8, false, true, 11);     // start
        checkEntry(entryFrame, 9, false, false, 11);
        checkEntry(entryFrame, 10, false, false, 11);
        checkEntry(entryFrame, 11, false, false, 11);   // end
        checkEntry(entryFrame, 12, true, false, 0);
        checkEntry(entryFrame, 13, true, false, 0);

        expectEntriesInBeam(entryFrame, { 1, 2 });
        expectEntriesInBeam(entryFrame, { 4, 5 });
        expectEntriesInBeam(entryFrame, { 8, 9, 10, 11 });
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 3, 1);        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);
        checkEntry(entryFrame, 2, true, false, 0);
        checkEntry(entryFrame, 3, true, false, 0);
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 3, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);      
        checkEntry(entryFrame, 2, false, true, 3);      // fake start (real start hidden by no-stems staff style)
        checkEntry(entryFrame, 3, false, false, 3);     // end (start hidden by no-stems staff style)
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
        checkEntry(entryFrame, 6, false, true, 7);      // start
        checkEntry(entryFrame, 7, false, false, 7);     // end

        expectEntriesInBeam(entryFrame, { 2, 3 });
        expectEntriesInBeam(entryFrame, { 6, 7 });
    }
}

TEST(BeamDetection, PrimaryIncludeRests)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams_primrests.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, false, true, 12);     // start
        checkEntry(entryFrame, 1, false, true, 6);      // start
        checkEntry(entryFrame, 2, false, false, 6);
        checkEntry(entryFrame, 3, false, true, 4);      // start
        checkEntry(entryFrame, 4, false, false, 4);     // end
        checkEntry(entryFrame, 5, false, false, 6);
        checkEntry(entryFrame, 6, false, false, 6);     // end
        checkEntry(entryFrame, 7, false, false, 12);
        checkEntry(entryFrame, 8, false, true, 10);     // start
        checkEntry(entryFrame, 9, false, false, 10);
        checkEntry(entryFrame, 10, false, false, 10);   // end
        checkEntry(entryFrame, 11, false, false, 12);
        checkEntry(entryFrame, 12, false, false, 12);   // end
        checkEntry(entryFrame, 13, false, true, 16);    // start
        checkEntry(entryFrame, 14, false, false, 16);
        checkEntry(entryFrame, 15, false, false, 16);
        checkEntry(entryFrame, 16, false, false, 16);   // end
        checkEntry(entryFrame, 17, true, false, 0);

        expectEntriesInBeam(entryFrame, { 0, 7, 11, 12 });
        expectEntriesInBeam(entryFrame, { 1, 2, 5, 6 });
        expectEntriesInBeam(entryFrame, { 3, 4 });
        expectEntriesInBeam(entryFrame, { 8, 9, 10 });
        expectEntriesInBeam(entryFrame, { 13, 14, 15, 16 });
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 2, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, false, true, 3);      // start
        checkEntry(entryFrame, 1, false, false, 3);
        checkEntry(entryFrame, 2, false, false, 3);
        checkEntry(entryFrame, 3, false, false, 3);     // end
        checkEntry(entryFrame, 4, false, true, 5);      // start
        checkEntry(entryFrame, 5, false, false, 5);     // end
        checkEntry(entryFrame, 6, true, false, 0);
        checkEntry(entryFrame, 7, false, true, 12);     // start
        checkEntry(entryFrame, 8, false, true, 11);     // start
        checkEntry(entryFrame, 9, false, false, 11);
        checkEntry(entryFrame, 10, false, false, 11);
        checkEntry(entryFrame, 11, false, false, 11);   // end
        checkEntry(entryFrame, 12, false, false, 12);   // end
        checkEntry(entryFrame, 13, true, false, 0);

        expectEntriesInBeam(entryFrame, { 0, 1, 2, 3 });
        expectEntriesInBeam(entryFrame, { 4, 5 });
        expectEntriesInBeam(entryFrame, { 7, 12 });
        expectEntriesInBeam(entryFrame, { 8, 9, 10, 11 });
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 3, 1);        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);
        checkEntry(entryFrame, 2, true, false, 0);
        checkEntry(entryFrame, 3, true, false, 0);
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 3, 2);        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);      
        checkEntry(entryFrame, 2, false, true, 3);      // fake start (real start hidden by no-stems staff style)
        checkEntry(entryFrame, 3, false, false, 3);     // end (start hidden by no-stems staff style)
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
        checkEntry(entryFrame, 6, false, true, 7);      // start
        checkEntry(entryFrame, 7, false, false, 7);     // end

        expectEntriesInBeam(entryFrame, { 2, 3 });
        expectEntriesInBeam(entryFrame, { 6, 7 });
    }
}

TEST(BeamDetection, InvisibleEntries)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_invisibles.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto measures = others->getArray<others::Measure>(SCORE_PARTID);
    EXPECT_GE(measures.size(), 4);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        checkEntry(entryFrame, 0, false, true, 3);              // start
        checkEntry(entryFrame, 1, false, false, 3);             // invisible (but also part of beam 0..3 beam)
        checkEntry(entryFrame, 2, true, false, 2);              // v2 rest
        checkEntry(entryFrame, 3, false, false, 3);             // end of beam
        expectEntriesInBeam(entryFrame, { 0, 3 });              // invisible entry 1 should not be found
        expectEntriesInBeam(entryFrame, { 0, 1, 3 }, true);     // invisible entry 1 should be found with includeHidden = true
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 2";

        checkEntry(entryFrame, 0, true, false, 0);              // invisible (unbeamed)
        checkEntry(entryFrame, 1, false, true, 3);              // start
        checkEntry(entryFrame, 2, false, false, 3);              //
        checkEntry(entryFrame, 3, false, false, 3);             // end of beam
        expectEntriesInBeam(entryFrame, { 1, 2, 3 });

        checkEntry(entryFrame, 4, false, true, 6);              // start
        checkEntry(entryFrame, 5, false, false, 6);             //
        checkEntry(entryFrame, 6, false, false, 6);             // end of beam
        checkEntry(entryFrame, 7, true, false, 0);              // invisible (unbeamed)
        expectEntriesInBeam(entryFrame, { 4, 5, 6 });
        expectEntriesInBeam(entryFrame, { 4, 5, 6, 7 }, true);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 3";

        checkEntry(entryFrame, 0, true, false, 0);              // visible 8th
        checkEntry(entryFrame, 1, true, false, 0);              // invisible
        checkEntry(entryFrame, 2, true, false, 0);              // invisible
        checkEntry(entryFrame, 3, true, false, 0);              // visible 8th rest
        checkEntry(entryFrame, 4, true, false, 0);              // invisible
        checkEntry(entryFrame, 5, true, false, 0);              // invisible
        checkEntry(entryFrame, 6, true, false, 0);              // invisible
        checkEntry(entryFrame, 7, true, false, 0);              // visible 8th
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 4";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 4";

        checkEntry(entryFrame, 0, false, true, 7);              // start
        checkEntry(entryFrame, 1, false, false, 7);             //
        checkEntry(entryFrame, 2, false, false, 7);             //
        checkEntry(entryFrame, 3, false, false, 7);             // invisible
        checkEntry(entryFrame, 4, false, false, 7);             //
        checkEntry(entryFrame, 5, false, false, 7);             //
        checkEntry(entryFrame, 6, false, false, 7);             //
        checkEntry(entryFrame, 7, false, false, 7);             // end
        expectEntriesInBeam(entryFrame, { 0, 1, 2, 4, 5, 6, 7 });
        expectEntriesInBeam(entryFrame, { 0, 1, 2, 3, 4, 5, 6, 7 }, true);

        checkEntry(entryFrame, 8, true, false, 0);              // quarter
    }
}