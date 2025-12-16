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

TEST(LyricsTest, WordExtensions)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "wordext.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto staff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, 1, 1, 0);
    ASSERT_TRUE(staff);
    auto lyricLineInfos = staff->createLyricsLineInfo(1);
    ASSERT_EQ(lyricLineInfos.size(), 2);

    {
        auto lyricLineInfo = lyricLineInfos[0];
        ASSERT_EQ(lyricLineInfo.assignments.size(), 3);
        {
            auto wextEndPoint = lyricLineInfo.assignments[0]->calcWordExtensionEndpoint();
            ASSERT_TRUE(wextEndPoint);
            EXPECT_EQ(wextEndPoint.getMeasure(), 1);
            EXPECT_EQ(wextEndPoint.getIndexInFrame(), 2);
        }
        {
            auto wextEndPoint = lyricLineInfo.assignments[1]->calcWordExtensionEndpoint();
            ASSERT_TRUE(wextEndPoint);
            EXPECT_EQ(wextEndPoint.getMeasure(), 2);
            EXPECT_EQ(wextEndPoint.getIndexInFrame(), 1);
        }
        {
            auto wextEndPoint = lyricLineInfo.assignments[2]->calcWordExtensionEndpoint();
            ASSERT_TRUE(wextEndPoint);
            EXPECT_EQ(wextEndPoint.getMeasure(), 2);
            EXPECT_EQ(wextEndPoint.getIndexInFrame(), 5);
        }
    }
    {
        auto lyricLineInfo = lyricLineInfos[1];
        ASSERT_EQ(lyricLineInfo.assignments.size(), 3);
        {
            auto wextEndPoint = lyricLineInfo.assignments[0]->calcWordExtensionEndpoint();
            EXPECT_FALSE(wextEndPoint);
        }
        {
            auto wextEndPoint = lyricLineInfo.assignments[1]->calcWordExtensionEndpoint();
            ASSERT_TRUE(wextEndPoint);
            EXPECT_EQ(wextEndPoint.getMeasure(), 2);
            EXPECT_EQ(wextEndPoint.getIndexInFrame(), 0);
        }
        {
            auto wextEndPoint = lyricLineInfo.assignments[2]->calcWordExtensionEndpoint();
            ASSERT_TRUE(wextEndPoint);
            EXPECT_EQ(wextEndPoint.getMeasure(), 2);
            EXPECT_EQ(wextEndPoint.getIndexInFrame(), 5);
        }
    }
}
