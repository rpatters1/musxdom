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

TEST(IndependentStaffDetailsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <floats cmper1="2" cmper2="3">
      <keySig>
        <key>257</key>
      </keySig>
      <hasKey/>
    </floats>
    <floats cmper1="5" cmper2="7">
      <beats>8</beats>
      <divBeat>4</divBeat>
      <dispBeats>9</dispBeats>
      <dispDivBeat>5</dispDivBeat>
      <displayAltNumTsig/>
      <displayAltDenTsig/>
      <altDenTsig/>
      <altNumTsig/>
      <hasDispTime/>
      <hasTime/>
    </floats>
    <floats cmper1="2" cmper2="2">
      <beats>4</beats>
      <divBeat>1024</divBeat>
      <dispBeats>4</dispBeats>
      <dispDivBeat>1024</dispDivBeat>
      <displayAbbrvTime/>
      <hasDispTime/>
      <hasTime/>
    </floats>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Test IndependentStaffDetails for cmper1=2, cmper2=3 (has key signature)
    {
        auto floats = details->get<details::IndependentStaffDetails>(SCORE_PARTID, 2, 3);
        ASSERT_TRUE(floats);

        ASSERT_TRUE(floats->hasKey);
        ASSERT_TRUE(floats->keySig);
        EXPECT_EQ(floats->keySig->key, 257);

        EXPECT_FALSE(floats->hasTime);
        EXPECT_FALSE(floats->hasDispTime);
    }

    // Test IndependentStaffDetails for cmper1=5, cmper2=7 (full time signature overrides)
    {
        auto floats = details->get<details::IndependentStaffDetails>(SCORE_PARTID, 5, 7);
        ASSERT_TRUE(floats);

        EXPECT_TRUE(floats->hasTime);
        EXPECT_TRUE(floats->hasDispTime);

        EXPECT_EQ(floats->beats, 8);
        EXPECT_EQ(floats->divBeat, 4);
        EXPECT_EQ(floats->dispBeats, 9);
        EXPECT_EQ(floats->dispDivBeat, 5);

        EXPECT_TRUE(floats->displayAltNumTsig);
        EXPECT_TRUE(floats->displayAltDenTsig);
        EXPECT_TRUE(floats->altNumTsig);
        EXPECT_TRUE(floats->altDenTsig);
    }

    // Test IndependentStaffDetails for cmper1=2, cmper2=2 (abbreviated time signature)
    {
        auto floats = details->get<details::IndependentStaffDetails>(SCORE_PARTID, 2, 2);
        ASSERT_TRUE(floats);

        EXPECT_TRUE(floats->hasTime);
        EXPECT_TRUE(floats->hasDispTime);
        EXPECT_TRUE(floats->displayAbbrvTime);

        EXPECT_EQ(floats->beats, 4);
        EXPECT_EQ(floats->divBeat, 1024);
        EXPECT_EQ(floats->dispBeats, 4);
        EXPECT_EQ(floats->dispDivBeat, 1024);

        EXPECT_FALSE(floats->hasKey);
        EXPECT_FALSE(floats->keySig);
    }
}
