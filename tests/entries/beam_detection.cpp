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

TEST(BeamDetection, PrimaryNoIncludeRests)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
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
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 2, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 2, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
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
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 3, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 1";
        auto entryFrame = gfhold->createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);
        checkEntry(entryFrame, 2, true, false, 0);
        checkEntry(entryFrame, 3, true, false, 0);
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 3, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);      
        checkEntry(entryFrame, 2, false, true, 3);      // fake start (real start hidden by no-stems staff style)
        checkEntry(entryFrame, 3, false, false, 3);     // end (start hidden by no-stems staff style)
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
        checkEntry(entryFrame, 6, false, true, 7);      // start
        checkEntry(entryFrame, 7, false, false, 7);     // end
    }
}

TEST(BeamDetection, PrimaryIncludeRests)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beams_primrests.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 1, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
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
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 2, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 2, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
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
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 3, 1);
        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 1";
        auto entryFrame = gfhold->createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);
        checkEntry(entryFrame, 2, true, false, 0);
        checkEntry(entryFrame, 3, true, false, 0);
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
    }
    {
        auto gfhold = doc->getDetails()->get<details::GFrameHold>(SCORE_PARTID, 3, 2);
        ASSERT_TRUE(gfhold) << " gfhold not found for 3, 2";
        auto entryFrame = gfhold->createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        checkEntry(entryFrame, 0, true, false, 0);
        checkEntry(entryFrame, 1, true, false, 0);      
        checkEntry(entryFrame, 2, false, true, 3);      // fake start (real start hidden by no-stems staff style)
        checkEntry(entryFrame, 3, false, false, 3);     // end (start hidden by no-stems staff style)
        checkEntry(entryFrame, 4, true, false, 0);
        checkEntry(entryFrame, 5, true, false, 0);
        checkEntry(entryFrame, 6, false, true, 7);      // start
        checkEntry(entryFrame, 7, false, false, 7);     // end
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
    for (const auto& meas : measures) {
        auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, meas->getCmper());
        //EXPECT_TRUE(gfhold);
        if (gfhold) {
            std::cout << "Measure " << gfhold->getCmper2() << std::endl;
            gfhold->iterateEntries([](const EntryInfoPtr& entryInfo) {
                if (entryInfo.calcIsBeamStart()) {
                    auto next = entryInfo;
                    std::cout << "    Beam:";
                    while (next) {
                        std::cout << " " << next.getIndexInFrame();
                        next = next.getNextInBeamGroup();
                    }
                    std::cout << std::endl;
                } else if (entryInfo.calcLowestBeamEnd() == 1) {
                    auto prev = entryInfo;
                    std::cout << "    Reverse Beam:";
                    while (prev) {
                        std::cout << " " << prev.getIndexInFrame();
                        prev = prev.getPreviousInBeamGroup();
                    }
                    std::cout << std::endl;
                }
                if (entryInfo.calcLowestBeamStart() > 1) {
                    std::cout << "    entry index " << entryInfo.getIndexInFrame();
                    std::cout << " secondary beam start " << entryInfo.calcLowestBeamStart() << std::endl;
                }
                if (entryInfo.calcLowestBeamStub()) {
                    std::cout << "    entry index " << entryInfo.getIndexInFrame();
                    std::cout << (entryInfo.calcBeamStubIsLeft() ? " stub left" : " stub right") << std::endl;
                }
                return true;
            });
        }
    }
}