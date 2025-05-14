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

TEST(NoteAlterationsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <noteAlter entnum="5" inci="0">
      <noteID>1</noteID>
      <percent>97</percent>
      <nxdisp>1</nxdisp>
      <altNhead>208</altNhead>
      <useOwnFont/>
      <allowVertPos/>
      <fontID>7</fontID>
      <fontSize>24</fontSize>
      <efx>
        <bold/>
        <underline/>
      </efx>
      <nydisp>2</nydisp>
    </noteAlter>
    <noteAlter entnum="5" inci="0" part="1" shared="true">
      <enharmonic/>
    </noteAlter>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Score-level NoteAlterations
    {
        auto alter = details->get<details::NoteAlterations>(SCORE_PARTID, 5, 0);
        ASSERT_TRUE(alter);

        EXPECT_EQ(alter->noteId, 1);
        EXPECT_EQ(alter->percent, 97);
        EXPECT_EQ(alter->nxdisp, 1);
        EXPECT_EQ(alter->altNhead, 208);
        EXPECT_TRUE(alter->useOwnFont);
        EXPECT_TRUE(alter->allowVertPos);
        EXPECT_EQ(alter->nydisp, 2);
        EXPECT_FALSE(alter->enharmonic);

        ASSERT_TRUE(alter->customFont);
        EXPECT_EQ(alter->customFont->fontId, 7);
        EXPECT_EQ(alter->customFont->fontSize, 24);
        EXPECT_TRUE(alter->customFont->bold);
        EXPECT_TRUE(alter->customFont->underline);
    }

    // Part-level NoteAlterations
    {
        auto alter = details->get<details::NoteAlterations>(1, 5, 0);
        ASSERT_TRUE(alter);

        EXPECT_EQ(alter->noteId, 1);
        EXPECT_EQ(alter->percent, 97);
        EXPECT_EQ(alter->nxdisp, 1);
        EXPECT_EQ(alter->altNhead, 208);
        EXPECT_TRUE(alter->useOwnFont);
        EXPECT_TRUE(alter->allowVertPos);
        EXPECT_EQ(alter->nydisp, 2);
        EXPECT_TRUE(alter->enharmonic); // unlinked
    }
}
