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

TEST(StemTests, DefaultDirection)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "stemdirection.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 8);

        EXPECT_TRUE(EntryInfoPtr(entryFrame, 1).calcUpStemDefault()); // 2nd grace note in beam
        EXPECT_FALSE(EntryInfoPtr(entryFrame, 4).calcUpStemDefault()); // 2nd note in beam
        EXPECT_FALSE(EntryInfoPtr(entryFrame, 5).calcUpStemDefault()); // cross-staffed quarter
        EXPECT_FALSE(EntryInfoPtr(entryFrame, 6).calcUpStemDefault()); // 1st note in cross-staffed beam
        EXPECT_FALSE(EntryInfoPtr(entryFrame, 7).calcUpStemDefault()); // 1st note in cross-staffed beam
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 8);

        EXPECT_FALSE(EntryInfoPtr(entryFrame, 1).calcUpStemDefault()); // 2nd note in beam
        EXPECT_FALSE(EntryInfoPtr(entryFrame, 6).calcUpStemDefault()); // Floating rest
        EXPECT_TRUE(EntryInfoPtr(entryFrame, 7).calcUpStemDefault()); // Final chord
    }
}
