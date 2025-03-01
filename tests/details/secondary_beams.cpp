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

TEST(SecondaryBeamBreakTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <secBeamBreak entnum="7">
          <do32nd/>
          <do64th/>
          <do128th/>
          <do256th/>
          <do512th/>
          <do1024th/>
          <do2048th/>
          <do4096th/>
          <beamThrough/>
        </secBeamBreak>
        <secBeamBreak entnum="9">
          <do16th/>
        </secBeamBreak>
        <secBeamBreak entnum="11">
          <do64th/>
          <do128th/>
          <do512th/>
          <do1024th/>
          <do2048th/>
          <do4096th/>
          <beamThrough/>
        </secBeamBreak>
      </details>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Test case 1: entnum = 7
    auto beamBreak7 = details->get<details::SecondaryBeamBreak>(SCORE_PARTID, 7);
    ASSERT_TRUE(beamBreak7) << "SecondaryBeamBreak with entnum 7 and inci 0 not found";

    EXPECT_EQ(beamBreak7->mask, 
              unsigned(NoteType::Note32nd) | unsigned(NoteType::Note64th) | 
              unsigned(NoteType::Note128th) | unsigned(NoteType::Note256th) | 
              unsigned(NoteType::Note512th) | unsigned(NoteType::Note1024th) | 
              unsigned(NoteType::Note2048th) | unsigned(NoteType::Note4096th));
    EXPECT_TRUE(beamBreak7->breakThrough);
    EXPECT_EQ(beamBreak7->calcLowestBreak(), 3);

    // Test case 2: entnum = 9
    auto beamBreak9 = details->get<details::SecondaryBeamBreak>(SCORE_PARTID, 9);
    ASSERT_TRUE(beamBreak9) << "SecondaryBeamBreak with entnum 9 and inci 0 not found";

    EXPECT_EQ(beamBreak9->mask, unsigned(NoteType::Note16th));
    EXPECT_FALSE(beamBreak9->breakThrough);
    EXPECT_EQ(beamBreak9->calcLowestBreak(), 2);

    // Test case 3: entnum = 11
    auto beamBreak11 = details->get<details::SecondaryBeamBreak>(SCORE_PARTID, 11);
    ASSERT_TRUE(beamBreak11) << "SecondaryBeamBreak with entnum 11 and inci 0 not found";

    EXPECT_EQ(beamBreak11->mask, 
              unsigned(NoteType::Note64th) | unsigned(NoteType::Note128th) | 
              unsigned(NoteType::Note512th) | unsigned(NoteType::Note1024th) | 
              unsigned(NoteType::Note2048th) | unsigned(NoteType::Note4096th));
    EXPECT_TRUE(beamBreak11->breakThrough);
    EXPECT_EQ(beamBreak11->calcLowestBreak(), 4);
}

TEST(SecondaryBeamBreakTest, NoBeamsSpecified)
{
    constexpr static musxtest::string_view xmlInvalid = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <secBeamBreak entnum="7">
          <beamThrough/>
        </secBeamBreak>
      </details>
    </finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xmlInvalid),
        integrity_error
    );
}
