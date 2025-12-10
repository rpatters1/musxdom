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

static void expectEntriesInBeam(const std::shared_ptr<const EntryFrame>& entryFrame, const std::vector<size_t>& expectedIndices,
    EntryInfoPtr::BeamIterationMode beamIterationMode = EntryInfoPtr::BeamIterationMode::Normal)
{
    ASSERT_FALSE(expectedIndices.empty());
    auto next = EntryInfoPtr(entryFrame, expectedIndices[0]);
    ASSERT_TRUE(next);
    bool isStart = next.calcIsBeamStart(beamIterationMode);
    EXPECT_TRUE(isStart) << "entry is not start of beam";
    if (!isStart) return;
    size_t x = 0;
    while (next) {
        EXPECT_TRUE(x < expectedIndices.size()) << "number of entries in forward iteration exceeds number of expected values";
        if (x >= expectedIndices.size()) return;
        EXPECT_EQ(next.getIndexInFrame(), expectedIndices[x]) << "beam index " << x << " is not the expected value in forward iteration";
        next = next.getNextInBeamGroup(beamIterationMode);
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
        prev = prev.getPreviousInBeamGroup(beamIterationMode);
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

    constexpr auto inclHidden = EntryInfoPtr::BeamIterationMode::IncludeAll;

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        checkEntry(entryFrame, 0, false, true, 3);                  // start
        checkEntry(entryFrame, 1, false, false, 3);                 // invisible (but also part of beam 0..3 beam)
        checkEntry(entryFrame, 2, true, false, 2);                  // v2 rest
        checkEntry(entryFrame, 3, false, false, 3);                 // end of beam
        expectEntriesInBeam(entryFrame, { 0, 3 });                  // invisible entry 1 should not be found
        expectEntriesInBeam(entryFrame, { 0, 1, 3 }, inclHidden);   // invisible entry 1 should be found with inclHidden
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
        expectEntriesInBeam(entryFrame, { 4, 5, 6, 7 }, inclHidden);
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

        expectEntriesInBeam(entryFrame, { 0, 1, 2, 4, 5, 6, 7 });
        expectEntriesInBeam(entryFrame, { 0, 1, 2, 3, 4, 5, 6, 7 }, inclHidden);

        checkEntry(entryFrame, 8, true, false, 0);              // quarter
    }
}

TEST(BeamDetection, BeamedRestWorkaround)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_invisibles.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);


    constexpr auto iterMode = EntryInfoPtr::BeamIterationMode::Interpreted;

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        expectEntriesInBeam(entryFrame, { 0, 3 });                  // invisible entry 1 should not be found
        expectEntriesInBeam(entryFrame, { 0, 1, 3 }, iterMode);     // invisible entry 1 should be found with Interpreted
    }
}

TEST(BeamDetection, SingletonBeams)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "singbeam.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto checkSingleton = [](const EntryInfoPtr entryInfo, bool isSingletonRight, bool isSingletonLeft) {
        EXPECT_EQ(entryInfo.calcCreatesSingletonBeamRight(), isSingletonRight);
        EXPECT_EQ(entryInfo.calcCreatesSingletonBeamLeft(), isSingletonLeft);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        checkSingleton(EntryInfoPtr(entryFrame, 1), true, false);
        expectEntriesInBeam(entryFrame, { 1 }, EntryInfoPtr::BeamIterationMode::Interpreted); // singleton right
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 2";

        checkSingleton(EntryInfoPtr(entryFrame, 1), false, true);
        expectEntriesInBeam(entryFrame, { 2 }, EntryInfoPtr::BeamIterationMode::Interpreted); // singleton left
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 3";

        checkSingleton(EntryInfoPtr(entryFrame, 1), false, false);
        checkSingleton(EntryInfoPtr(entryFrame, 2), false, false);
    }
}

TEST(BeamDetection, BeamsOverBarlines)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams_over_barlines.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto checkBeamOvers = [](const EntryInfoPtr entryInfo, bool isBeamOverRight, bool isBeamOverLeft) -> void {
        auto nextRight = entryInfo.calcBeamContinuesRightOverBarline();
        EXPECT_EQ(bool(nextRight), isBeamOverRight);
        if (nextRight) {
            auto tryPrev = nextRight.calcBeamContinuesLeftOverBarline();
            EXPECT_TRUE(entryInfo.isSameEntry(tryPrev));
            tryPrev = nextRight.getPreviousInBeamGroupAcrossBars();
            EXPECT_TRUE(entryInfo.isSameEntry(tryPrev));
        }
        auto prevLeft = entryInfo.calcBeamContinuesLeftOverBarline();
        EXPECT_EQ(bool(prevLeft), isBeamOverLeft);
        if (prevLeft) {
            auto tryNext = prevLeft.calcBeamContinuesRightOverBarline();
            EXPECT_TRUE(entryInfo.isSameEntry(tryNext));
            tryNext = prevLeft.getNextInBeamGroupAcrossBars();
            EXPECT_TRUE(entryInfo.isSameEntry(tryNext));
        }
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        checkBeamOvers(EntryInfoPtr(entryFrame, 3), true, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 2";

        checkBeamOvers(EntryInfoPtr(entryFrame, 0), false, true);
        checkBeamOvers(EntryInfoPtr(entryFrame, 5), true, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 3";

        checkBeamOvers(EntryInfoPtr(entryFrame, 0), false, true);
        checkBeamOvers(EntryInfoPtr(entryFrame, 5), true, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 4";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 4";

        checkBeamOvers(EntryInfoPtr(entryFrame, 1), false, true);
        checkBeamOvers(EntryInfoPtr(entryFrame, 6), true, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 5";

        checkBeamOvers(EntryInfoPtr(entryFrame, 1), false, true);
        checkBeamOvers(EntryInfoPtr(entryFrame, 7), true, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 6";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 6";

        checkBeamOvers(EntryInfoPtr(entryFrame, 0), false, true);
        checkBeamOvers(EntryInfoPtr(entryFrame, 7), true, false);
    }
}

TEST(BeamDetection, BeamsOverBarlinesTraversal)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams_over_barlines.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto expectedEntries = std::vector<EntryNumber>{};
    auto traverseBeam = [&](const EntryInfoPtr startEntry) {
        EntryInfoPtr endEntry;
        size_t nextIndex = 0;
        for (auto entryInfo = startEntry; entryInfo; entryInfo = entryInfo.getNextInBeamGroupAcrossBars()) {
            ASSERT_LT(nextIndex, expectedEntries.size());
            EXPECT_EQ(entryInfo->getEntry()->getEntryNumber(), expectedEntries[nextIndex]);
            endEntry = entryInfo;
            nextIndex++;
        }
        EXPECT_EQ(nextIndex, expectedEntries.size());
        for (auto entryInfo = endEntry; entryInfo; entryInfo = entryInfo.getPreviousInBeamGroupAcrossBars()) {
            ASSERT_GT(nextIndex, 0);
            nextIndex--;
            EXPECT_EQ(entryInfo->getEntry()->getEntryNumber(), expectedEntries[nextIndex]);
            endEntry = entryInfo;
        }
        EXPECT_EQ(nextIndex, 0);
        EXPECT_TRUE(startEntry.isSameEntry(endEntry));
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 5";

        expectedEntries = { 75, 76, 77, 91 };
        traverseBeam(EntryInfoPtr(entryFrame, 5));
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 6";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 6";

        expectedEntries = { 82, 83, 84, 85, 86, 87 };
        traverseBeam(EntryInfoPtr(entryFrame, 5));
    }
}

TEST(BeamDetection, BeamsOverBarlinesHiddenSourceDetect)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_over_graces.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";
        ASSERT_GE(entryFrame->getEntries().size(), 22);

        for (size_t x = 3; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto hiddenEntry = entryInfoPtr.findHiddenSourceForBeamOverBarline();
            EXPECT_TRUE((x < 6 && !hiddenEntry) || (x >= 6 && hiddenEntry)) << "Unexpected hidden entry value for index " << x;
            MeasCmper expectedMeas = 1;
            size_t indexOffset = 0;
            if (x >= 6 && x < 18) {
                expectedMeas = 2;
                indexOffset = 6;
            } else if (x >= 18) {
                expectedMeas = 3;
                indexOffset = 18;
            }
            if (hiddenEntry) {
                EXPECT_EQ(hiddenEntry.getIndexInFrame(), x - indexOffset);
                EXPECT_EQ(hiddenEntry.getMeasure(), expectedMeas);
            }
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 5";
        ASSERT_GE(entryFrame->getEntries().size(), 16);

        for (size_t x = 3; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto hiddenEntry = entryInfoPtr.findHiddenSourceForBeamOverBarline();
            EXPECT_TRUE((x < 4 && !hiddenEntry) || (x >= 4 && hiddenEntry)) << "Unexpected hidden entry value for index " << x;
            MeasCmper expectedMeas = 5;
            size_t indexOffset = 0;
            if (x >= 4 && x < 14) {
                expectedMeas = 6;
                indexOffset = 4;
            } else if (x >= 14) {
                expectedMeas = 7;
                indexOffset = 14;
            }
            if (hiddenEntry) {
                EXPECT_EQ(hiddenEntry.getIndexInFrame(), x - indexOffset);
                EXPECT_EQ(hiddenEntry.getMeasure(), expectedMeas);
            }
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 7";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 7";
        ASSERT_GE(entryFrame->getEntries().size(), 7);
        auto last = EntryInfoPtr(entryFrame, 6);
        EXPECT_FALSE(last.findHiddenSourceForBeamOverBarline());
    }
}

TEST(BeamDetection, BeamsOverBarlinesDisplayEntryDetect)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_over_graces.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_FALSE(displayEntry);
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 2";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 2";
        ASSERT_GE(entryFrame->getEntries().size(), 16);

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_EQ(displayEntry.getMeasure(), 1);
            EXPECT_EQ(displayEntry.getIndexInFrame(), entryInfoPtr.getIndexInFrame() + 6);
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 3";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 3";
        ASSERT_GE(entryFrame->getEntries().size(), 6);

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_TRUE((x < 4 && displayEntry) || (x >= 4 && !displayEntry));
            if (displayEntry) {
                EXPECT_EQ(displayEntry.getMeasure(), 1);
                EXPECT_EQ(displayEntry.getIndexInFrame(), entryInfoPtr.getIndexInFrame() + 18);
            }
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 5";

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_FALSE(displayEntry);
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 6";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 6";
        ASSERT_GE(entryFrame->getEntries().size(), 12);

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_EQ(displayEntry.getMeasure(), 5);
            EXPECT_EQ(displayEntry.getIndexInFrame(), entryInfoPtr.getIndexInFrame() + 4);
        }
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 7";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 7";
        ASSERT_GE(entryFrame->getEntries().size(), 7);

        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            auto entryInfoPtr = EntryInfoPtr(entryFrame, x);
            auto displayEntry = entryInfoPtr.findDisplayEntryForBeamOverBarline();
            EXPECT_TRUE((x < 2 && displayEntry) || (x >= 2 && !displayEntry));
            if (displayEntry) {
                EXPECT_EQ(displayEntry.getMeasure(), 5);
                EXPECT_EQ(displayEntry.getIndexInFrame(), entryInfoPtr.getIndexInFrame() + 14);
            }
        }
    }
}

TEST(BeamDetection, GraceNoteMainNoteDetection)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_over_graces.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto checkEntry = [](const EntryInfoPtr entPtr, bool expectedValue, size_t expectedIndex = 0, bool pastEndOfFrame = true) {
        const auto mainEntry = entPtr.findMainEntryForGraceNote();
        EXPECT_EQ(bool(mainEntry), expectedValue);
        if (mainEntry) {
            EXPECT_EQ(mainEntry.getIndexInFrame(), expectedIndex);
        }
        EXPECT_EQ(entPtr.asInterpretedIterator().calcIsPastLogicalEndOfFrame(), pastEndOfFrame);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";
        ASSERT_GE(entryFrame->getEntries().size(), 22);

        checkEntry(EntryInfoPtr(entryFrame, 3), false, 0, false); // not a grace note
        checkEntry(EntryInfoPtr(entryFrame, 4), false, 0, false);
        checkEntry(EntryInfoPtr(entryFrame, 5), false, 0, false);
        checkEntry(EntryInfoPtr(entryFrame, 10), true, 12);
        checkEntry(EntryInfoPtr(entryFrame, 11), true, 12);
        checkEntry(EntryInfoPtr(entryFrame, 16), false);
        checkEntry(EntryInfoPtr(entryFrame, 17), false);
        checkEntry(EntryInfoPtr(entryFrame, 18), true, 20);
        checkEntry(EntryInfoPtr(entryFrame, 19), true, 20);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 5";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 5";
        ASSERT_GE(entryFrame->getEntries().size(), 16);

        checkEntry(EntryInfoPtr(entryFrame, 4), true, 6);
        checkEntry(EntryInfoPtr(entryFrame, 5), true, 6);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 7";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 7";
        ASSERT_GE(entryFrame->getEntries().size(), 7);

        checkEntry(EntryInfoPtr(entryFrame, 6), false, 0, false);
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 8);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 8";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 8";
        ASSERT_GE(entryFrame->getEntries().size(), 5);

        checkEntry(EntryInfoPtr(entryFrame, 0), true, 1, false);
    }
}

TEST(BeamDetection, AdjacentRests)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beamrests.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold) << "gfhold not found for 1, 1";
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame) << "entry frame not created for 1, 1";
        ASSERT_GE(entryFrame->getEntries().size(), 6);

        auto lastEntry = EntryInfoPtr(entryFrame, 5); // final rest
        EXPECT_TRUE(lastEntry.calcUnbeamed());
    }
}
