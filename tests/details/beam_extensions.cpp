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

TEST(BeamExtensionTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <beamExtendDownStem entnum="11">
          <x3Disp>-34</x3Disp>
          <x4Disp>24</x4Disp>
          <do8th/>
          <do64th/>
          <extBeyond8th/>
        </beamExtendDownStem>
        <beamExtendUpStem entnum="5">
          <x3Disp>16</x3Disp>
          <x4Disp>-20</x4Disp>
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
          <extBeyond8th/>
        </beamExtendUpStem>
      </details>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Downstem beam extension (entnum = 11)
    auto down = details->get<details::BeamExtensionDownStem>(SCORE_PARTID, 11);
    ASSERT_TRUE(down) << "BeamExtensionDownStem with entnum 11 not found";

    EXPECT_EQ(down->leftOffset, Evpu(-34));
    EXPECT_EQ(down->rightOffset, Evpu(24));
    EXPECT_EQ(down->mask, unsigned(NoteType::Eighth) | unsigned(NoteType::Note64th));
    EXPECT_TRUE(down->extBeyond8th);

    // Upstem beam extension (entnum = 5)
    auto up = details->get<details::BeamExtensionUpStem>(SCORE_PARTID, 5);
    ASSERT_TRUE(up) << "BeamExtensionUpStem with entnum 5 not found";

    EXPECT_EQ(up->leftOffset, Evpu(16));
    EXPECT_EQ(up->rightOffset, Evpu(-20));
    EXPECT_EQ(up->mask,
              unsigned(NoteType::Eighth)   | unsigned(NoteType::Note16th) |
              unsigned(NoteType::Note32nd)  | unsigned(NoteType::Note64th) |
              unsigned(NoteType::Note128th) | unsigned(NoteType::Note256th) |
              unsigned(NoteType::Note512th) | unsigned(NoteType::Note1024th) |
              unsigned(NoteType::Note2048th) | unsigned(NoteType::Note4096th));
    EXPECT_TRUE(up->extBeyond8th);
}
