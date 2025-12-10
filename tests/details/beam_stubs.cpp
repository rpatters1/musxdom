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

TEST(BeamStubsTest, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <beamStub entnum="6">
          <do8th/>
          <do16th/>
          <do32nd/>
          <do64th/>
          <do128th/>
          <do256th/>
          <do512th/>
          <do1024th/>
          <do2048th/>
          <do4096th/>
        </beamStub>
        <beamStub entnum="7"/>
      </details>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Test case 1: entnum = 6
    auto beamStub6 = details->get<details::BeamStubDirection>(SCORE_PARTID, 6);
    ASSERT_TRUE(beamStub6) << "BeamStubDirection with entnum 6 not found";

    EXPECT_EQ(beamStub6->mask,
              unsigned(NoteType::Eighth) | unsigned(NoteType::Note16th) |
              unsigned(NoteType::Note32nd) | unsigned(NoteType::Note64th) |
              unsigned(NoteType::Note128th) | unsigned(NoteType::Note256th) |
              unsigned(NoteType::Note512th) | unsigned(NoteType::Note1024th) |
              unsigned(NoteType::Note2048th) | unsigned(NoteType::Note4096th));
    EXPECT_TRUE(beamStub6->isLeft());

    // Test case 2: entnum = 7
    auto beamStub7 = details->get<details::BeamStubDirection>(SCORE_PARTID, 7);
    ASSERT_TRUE(beamStub7) << "BeamStubDirection with entnum 7 not found";

    EXPECT_EQ(beamStub7->mask, 0);
    EXPECT_FALSE(beamStub7->isLeft());
}

TEST(BeamStubsTest, DetectDirection)
{
    std::vector<char> fileXml;
    musxtest::readFile(musxtest::getInputPath() / "beam_stubs.enigmaxml", fileXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fileXml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

/*
    auto measures = others->getArray<others::Measure>(SCORE_PARTID);
    EXPECT_GE(measures.size(), 10);
    for (const auto& meas : measures) {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, meas->getCmper());
        EXPECT_TRUE(gfhold);
        if (gfhold) {
            gfhold->iterateEntries([](const EntryInfoPtr& entryInfo) {
                if (entryInfo.calcLowestBeamStub()) {
                    std::cout << "Measure " << entryInfo.getMeasure() << " entry index " << entryInfo.getIndexInFrame();
                    std::cout << (entryInfo.calcBeamStubIsLeft() ? " stub left" : " stub right") << std::endl;
                }
                return true;
            });
        }
    }
*/

    auto testStub = [&](const EntryInfoPtr& entryInfo, bool expectLeft, bool expectStub = true) {
        ASSERT_TRUE(entryInfo);
        std::string msg = "Staff " + std::to_string(entryInfo.getStaff()) + " measure " + std::to_string(entryInfo.getMeasure())
        + " entry index " + std::to_string(entryInfo.getIndexInFrame());

        EXPECT_EQ(entryInfo.calcLowestBeamStub() > 0, expectStub) << msg << " stub expected did not match";
        if (expectStub) {
            EXPECT_EQ(entryInfo.calcBeamStubIsLeft(), expectLeft) << msg << " stub direction did not match";
        }
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 2), false);
        testStub(EntryInfoPtr(frame, 3), false);
        testStub(EntryInfoPtr(frame, 4), false);
        testStub(EntryInfoPtr(frame, 6), false);
        testStub(EntryInfoPtr(frame, 8), true);
        testStub(EntryInfoPtr(frame, 10), false);
        testStub(EntryInfoPtr(frame, 13), true);
        testStub(EntryInfoPtr(frame, 14), true);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 2), false, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 2), false, false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 4);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 2), false, false);
        testStub(EntryInfoPtr(frame, 3), true);
        testStub(EntryInfoPtr(frame, 5), true);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 5);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 2), false, false);
        testStub(EntryInfoPtr(frame, 3), true);
        testStub(EntryInfoPtr(frame, 5), true);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 6);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 0), false, false);
        testStub(EntryInfoPtr(frame, 2), false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 7);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 1), true);
        testStub(EntryInfoPtr(frame, 3), false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 8);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 1), false);
        testStub(EntryInfoPtr(frame, 4), true); // manually overridden
        testStub(EntryInfoPtr(frame, 7), false);
        testStub(EntryInfoPtr(frame, 9), false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 9);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 1), false);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 10);
        ASSERT_TRUE(gfhold);
        auto frame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(frame);
        testStub(EntryInfoPtr(frame, 1), false);
        testStub(EntryInfoPtr(frame, 3), true);
        testStub(EntryInfoPtr(frame, 5), true); // because of 2ndary beam break
        testStub(EntryInfoPtr(frame, 6), false);
    }
}
