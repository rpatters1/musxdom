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

TEST(CustomStemTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <stemDefDown entnum="5">
      <shapeDef>6</shapeDef>
    </stemDefDown>
    <stemDefUp entnum="6">
      <shapeDef>7</shapeDef>
      <xdisp>-18</xdisp>
      <ydisp>46</ydisp>
    </stemDefUp>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Downstem custom stem (entnum = 5)
    auto down = details->get<details::CustomDownStem>(SCORE_PARTID, 5);
    ASSERT_TRUE(down) << "CustomDownStem with entnum 5 not found";

    EXPECT_EQ(down->shapeDef, Cmper(6));
    EXPECT_EQ(down->xOffset, Evpu(0));
    EXPECT_EQ(down->yOffset, Evpu(0));

    // Upstem custom stem (entnum = 6)
    auto up = details->get<details::CustomUpStem>(SCORE_PARTID, 6);
    ASSERT_TRUE(up) << "CustomUpStem with entnum 6 not found";

    EXPECT_EQ(up->shapeDef, Cmper(7));
    EXPECT_EQ(up->xOffset, Evpu(-18));
    EXPECT_EQ(up->yOffset, Evpu(46));
}

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

TEST(TieAlterTest, PopulateFields)
{
  constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <tieAlterStart entnum="6" inci="0">
      <noteID>1</noteID>
      <xStart>9</xStart>
      <xEnd>-9</xEnd>
      <outerLocal/>
      <afterSingleDotLocal/>
      <afterSingleDotOn/>
      <afterMultiDotsLocal/>
      <afterMultiDotsOn/>
      <shiftForSecondsLocal/>
      <shiftForSecondsOn/>
      <beforeSingleAcciLocal/>
      <beforeSingleAcciOn/>
      <down/>
      <breakTimeLocal/>
      <breakTimeOn/>
      <breakKeyLocal/>
      <breakKeyOn/>
      <freeze/>
      <yStart>7</yStart>
      <yEnd>7</yEnd>
      <stPtAdjOn/>
      <enPtAdjOn/>
      <insetRatio1>410</insetRatio1>
      <height1>46</height1>
      <insetRatio2>410</insetRatio2>
      <height2>49</height2>
      <ctlPtAdjOn/>
    </tieAlterStart>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto tieAlter = details->get<details::TieAlterStart>(SCORE_PARTID, 6, 0);
    ASSERT_TRUE(tieAlter) << "TieAlterBase with entnum 6 not found";

    EXPECT_EQ(tieAlter->noteId, 1);
    EXPECT_EQ(tieAlter->xStart, Evpu(9));
    EXPECT_EQ(tieAlter->xEnd, Evpu(-9));
    EXPECT_EQ(tieAlter->yStart, Evpu(7));
    EXPECT_EQ(tieAlter->yEnd, Evpu(7));

    EXPECT_TRUE(tieAlter->outerLocal);
    EXPECT_TRUE(tieAlter->afterSingleDotLocal);
    EXPECT_TRUE(tieAlter->afterSingleDotOn);
    EXPECT_TRUE(tieAlter->afterMultiDotsLocal);
    EXPECT_TRUE(tieAlter->afterMultiDotsOn);
    EXPECT_TRUE(tieAlter->shiftForSecondsLocal);
    EXPECT_TRUE(tieAlter->shiftForSecondsOn);
    EXPECT_TRUE(tieAlter->beforeSingleAcciLocal);
    EXPECT_TRUE(tieAlter->beforeSingleAcciOn);
    EXPECT_TRUE(tieAlter->down);
    EXPECT_TRUE(tieAlter->breakTimeLocal);
    EXPECT_TRUE(tieAlter->breakTimeOn);
    EXPECT_TRUE(tieAlter->breakKeyLocal);
    EXPECT_TRUE(tieAlter->breakKeyOn);
    EXPECT_TRUE(tieAlter->freezeDirection);
    EXPECT_TRUE(tieAlter->stPtAdjOn);
    EXPECT_TRUE(tieAlter->enPtAdjOn);
    EXPECT_TRUE(tieAlter->ctlPtAdjOn);

    EXPECT_EQ(tieAlter->insetRatio1, Evpu(410));
    EXPECT_EQ(tieAlter->height1, Evpu(46));
    EXPECT_EQ(tieAlter->insetRatio2, Evpu(410));
    EXPECT_EQ(tieAlter->height2, Evpu(49));
}
