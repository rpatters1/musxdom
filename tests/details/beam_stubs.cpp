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
