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

TEST(Cues, TestIndependentTime)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "indtime-cuebug.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 2, 1";
    auto entryFrame = gfhold.createEntryFrame(3);
    ASSERT_TRUE(entryFrame);

    EXPECT_FALSE(entryFrame->calcIsCueFrame());
    EXPECT_TRUE(entryFrame->calcIsCueFrame(/*includeInVisibileInScore*/ true));
}
