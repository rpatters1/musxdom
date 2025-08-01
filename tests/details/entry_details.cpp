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

TEST(AccidentalAlterationsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <acciAlter entnum="5" inci="0">
      <noteID>1</noteID>
      <percent>91</percent>
      <ayDisp>13</ayDisp>
      <axDisp>-5</axDisp>
      <altChar>69</altChar>
      <fontID>15</fontID>
      <fontSize>18</fontSize>
      <efx>
        <italic/>
      </efx>
      <useOwnFont/>
      <allowVertPos/>
    </acciAlter>
    <acciAlter entnum="6" inci="0">
      <noteID>2</noteID>
      <percent>100</percent>
      <axDisp>-12</axDisp>
    </acciAlter>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Score-level AccidentalAlterations
    {
        auto alter = details->get<details::AccidentalAlterations>(SCORE_PARTID, 5, 0);
        ASSERT_TRUE(alter);

        EXPECT_EQ(alter->noteId, 1);
        EXPECT_EQ(alter->percent, 91);
        EXPECT_EQ(alter->vOffset, 13);
        EXPECT_EQ(alter->hOffset, -5);
        EXPECT_EQ(alter->altChar, 69);
        EXPECT_TRUE(alter->useOwnFont);
        EXPECT_TRUE(alter->allowVertPos);

        ASSERT_TRUE(alter->customFont);
        EXPECT_EQ(alter->customFont->fontId, 15);
        EXPECT_EQ(alter->customFont->fontSize, 18);
        EXPECT_TRUE(alter->customFont->italic);
        EXPECT_FALSE(alter->customFont->bold);
        EXPECT_FALSE(alter->customFont->underline);
    }

    // Score-level AccidentalAlterations with defaults
    {
        auto alter = details->get<details::AccidentalAlterations>(SCORE_PARTID, 6, 0);
        ASSERT_TRUE(alter);

        EXPECT_EQ(alter->noteId, 2);
        EXPECT_EQ(alter->percent, 100);
        EXPECT_EQ(alter->vOffset, 0); // default
        EXPECT_EQ(alter->hOffset, -12);
        EXPECT_EQ(alter->altChar, 0); // default
        EXPECT_FALSE(alter->useOwnFont);
        EXPECT_FALSE(alter->allowVertPos);

        ASSERT_TRUE(alter->customFont);
        EXPECT_EQ(alter->customFont->fontId, 0);
        EXPECT_EQ(alter->customFont->fontSize, 0);
        EXPECT_FALSE(alter->customFont->italic);
        EXPECT_FALSE(alter->customFont->bold);
        EXPECT_FALSE(alter->customFont->underline);
    }
}

TEST(BeamAlterationsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <beamOptions>
      <beamStubLength>18</beamStubLength>
      <maxSlope>12</maxSlope>
      <beamSepar>18</beamSepar>
      <beamingStyle>onStandardNote</beamingStyle>
      <incRestsInClassicBeams/>
      <beamFourEighthsInCommonTime/>
      <beamThreeEighthsInCommonTime/>
      <beamWidth>768</beamWidth>
    </beamOptions>
  </options>
  <details>
    <beamAltPrimDownStem entnum="11">
      <yAdd>-24</yAdd>
      <context>onExtremeNote</context>
      <beamWidth>1165</beamWidth>
    </beamAltPrimDownStem>
    <beamAltPrimUpStem entnum="5">
      <xAdd>-23</xAdd>
      <yAdd>40</yAdd>
      <sxAdd>29</sxAdd>
      <context>alwaysFlat</context>
      <beamWidth>1088</beamWidth>
    </beamAltPrimUpStem>
    <beamAltSecDownStem entnum="11" inci="0">
      <yAdd>20</yAdd>
      <syAdd>-12</syAdd>
      <dura>256</dura>
      <beamWidth>-1</beamWidth>
    </beamAltSecDownStem>
    <beamAltSecUpStem entnum="5" inci="0">
      <syAdd>-8</syAdd>
      <dura>256</dura>
      <context>onStandardNote</context>
      <beamWidth>-1</beamWidth>
    </beamAltSecUpStem>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Primary downstem
    auto primDown = details->get<details::BeamAlterationsDownStem>(SCORE_PARTID, 11);
    ASSERT_TRUE(primDown);
    EXPECT_EQ(primDown->leftOffsetH, Evpu(0));
    EXPECT_EQ(primDown->leftOffsetY, Evpu(-24));
    EXPECT_EQ(primDown->rightOffsetH, Evpu(0));
    EXPECT_EQ(primDown->rightOffsetY, Evpu(0));
    EXPECT_EQ(primDown->dura, Edu(0));
    EXPECT_EQ(primDown->flattenStyle, options::BeamOptions::FlattenStyle::OnExtremeNote);
    EXPECT_EQ(primDown->beamWidth, Efix(1165));
    EXPECT_FALSE(primDown->isActive());

    // Primary upstem
    auto primUp = details->get<details::BeamAlterationsUpStem>(SCORE_PARTID, 5);
    ASSERT_TRUE(primUp);
    EXPECT_EQ(primUp->leftOffsetH, Evpu(-23));
    EXPECT_EQ(primUp->leftOffsetY, Evpu(40));
    EXPECT_EQ(primUp->rightOffsetH, Evpu(29));
    EXPECT_EQ(primUp->rightOffsetY, Evpu(0));
    EXPECT_EQ(primUp->dura, Edu(0));
    EXPECT_EQ(primUp->flattenStyle, options::BeamOptions::FlattenStyle::AlwaysFlat);
    EXPECT_EQ(primUp->beamWidth, Efix(1088));
    EXPECT_FALSE(primUp->isActive());

    // Secondary downstem
    auto secDown = details->get<details::SecondaryBeamAlterationsDownStem>(SCORE_PARTID, 11, Inci(0));
    ASSERT_TRUE(secDown);
    EXPECT_EQ(secDown->leftOffsetH, Evpu(0));
    EXPECT_EQ(secDown->leftOffsetY, Evpu(20));
    EXPECT_EQ(secDown->rightOffsetH, Evpu(0));
    EXPECT_EQ(secDown->rightOffsetY, Evpu(-12));
    EXPECT_EQ(secDown->dura, Edu(256));
    EXPECT_EQ(secDown->flattenStyle, options::BeamOptions::FlattenStyle::OnEndNotes);  // default-initialized
    EXPECT_EQ(secDown->beamWidth, Efix(-1));
    EXPECT_FALSE(secDown->isActive());

    // Secondary upstem
    auto secUp = details->get<details::SecondaryBeamAlterationsUpStem>(SCORE_PARTID, 5, Inci(0));
    ASSERT_TRUE(secUp);
    EXPECT_EQ(secUp->leftOffsetH, Evpu(0));
    EXPECT_EQ(secUp->leftOffsetY, Evpu(0));
    EXPECT_EQ(secUp->rightOffsetH, Evpu(0));
    EXPECT_EQ(secUp->rightOffsetY, Evpu(-8));
    EXPECT_EQ(secUp->dura, Edu(256));
    EXPECT_EQ(secUp->flattenStyle, options::BeamOptions::FlattenStyle::OnStandardNote);
    EXPECT_EQ(secUp->beamWidth, Efix(-1));
    EXPECT_TRUE(secUp->isActive());
}

TEST(BeamAlterationsTest, BeamWidths)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beam_mods.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto beamOptions = options->get<options::BeamOptions>();
    ASSERT_TRUE(beamOptions);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);
    ASSERT_GE(entryFrame->getEntries().size(), 12);

    auto checkEntry = [&](size_t index, Efix primUpWidth, Efix secUpWidth, Efix primDnWidth, Efix secDnWidth) {
        EntryInfoPtr entryInfo(entryFrame, index);
        ASSERT_TRUE(entryInfo);
        EXPECT_TRUE(entryInfo->getEntry()->stemDetail);
        auto primUpAlts = details->get<details::BeamAlterationsUpStem>(SCORE_PARTID, entryInfo->getEntry()->getEntryNumber());
        auto primDnAlts = details->get<details::BeamAlterationsDownStem>(SCORE_PARTID, entryInfo->getEntry()->getEntryNumber());
        auto secUpAlts = [&]() -> MusxInstance<details::SecondaryBeamAlterationsUpStem> {
            auto result = details->getArray<details::SecondaryBeamAlterationsUpStem>(SCORE_PARTID, entryInfo->getEntry()->getEntryNumber());
            if (!result.empty()) {
                return result[0];
            }
            return nullptr;
        }();
        auto secDnAlts = [&]() -> MusxInstance<details::SecondaryBeamAlterationsDownStem> {
            auto result = details->getArray<details::SecondaryBeamAlterationsDownStem>(SCORE_PARTID, entryInfo->getEntry()->getEntryNumber());
            if (!result.empty()) {
                return result[0];
            }
            return nullptr;
        }();
        EXPECT_TRUE(primUpAlts);
        EXPECT_TRUE(primDnAlts);
        EXPECT_TRUE(secUpAlts);
        EXPECT_TRUE(secDnAlts);
        const Efix calcPrimUpWidth = primUpAlts ? primUpAlts->calcEffectiveBeamWidth() : beamOptions->beamWidth;
        EXPECT_EQ(calcPrimUpWidth, primUpWidth);
        const Efix calcPrimDnWidth = primDnAlts ? primDnAlts->calcEffectiveBeamWidth() : beamOptions->beamWidth;
        EXPECT_EQ(calcPrimDnWidth, primDnWidth);
        const Efix calcSecUpWidth = secUpAlts ? secUpAlts->calcEffectiveBeamWidth() : calcPrimUpWidth;
        EXPECT_EQ(calcSecUpWidth, secUpWidth);
        const Efix calcSecDnWidth = secDnAlts ? secDnAlts->calcEffectiveBeamWidth() : calcPrimDnWidth;
        EXPECT_EQ(calcSecDnWidth, secDnWidth);
    };

    checkEntry(0, beamOptions->beamWidth, beamOptions->beamWidth, beamOptions->beamWidth, beamOptions->beamWidth);
    checkEntry(6, 302, 302, 1165, 1165);    
}

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

TEST(DotAlterationsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <dotOffset entnum="5" inci="0">
      <noteID>1</noteID>
      <xadd>11</xadd>
      <yadd>13</yadd>
      <posIncr>8</posIncr>
    </dotOffset>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto adjust = details->get<details::DotAlterations>(SCORE_PARTID, 5, 0);
    ASSERT_TRUE(adjust) << "DotAlterations with entnum 5 and inci 0 not found";

    EXPECT_EQ(adjust->noteId, NoteNumber(1));
    EXPECT_EQ(adjust->hOffset, Evpu(11));
    EXPECT_EQ(adjust->vOffset, Evpu(13));
    EXPECT_EQ(adjust->interdotSpacing, Evpu(8));
}

TEST(LyricEntryInfoTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <lyricEntryInfo entnum="5">
      <justify>left</justify>
    </lyricEntryInfo>
    <lyricEntryInfo entnum="6">
      <align>center</align>
    </lyricEntryInfo>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Score-level LyricEntryInfo with justify only
    {
        auto info = details->get<details::LyricEntryInfo>(SCORE_PARTID, 5);
        ASSERT_TRUE(info);

        ASSERT_TRUE(info->justify);
        EXPECT_EQ(*info->justify, details::LyricEntryInfo::AlignJustify::Left);
        EXPECT_FALSE(info->align);
    }

    // Score-level LyricEntryInfo with align only
    {
        auto info = details->get<details::LyricEntryInfo>(SCORE_PARTID, 6);
        ASSERT_TRUE(info);

        EXPECT_FALSE(info->justify);
        ASSERT_TRUE(info->align);
        EXPECT_EQ(*info->align, details::LyricEntryInfo::AlignJustify::Center);
    }
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

TEST(StemAlterationsTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <stemAdjust entnum="5">
      <upVertAdjust>49</upVertAdjust>
      <downVertAdjust>-51</downVertAdjust>
      <upHorzAdjust>23</upHorzAdjust>
      <downHorzAdjust>-21</downHorzAdjust>
    </stemAdjust>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto adjust = details->get<details::StemAlterations>(SCORE_PARTID, 5);
    ASSERT_TRUE(adjust) << "StemAlterations with entnum 5 not found";

    EXPECT_EQ(adjust->upVertAdjust, Evpu(49));
    EXPECT_EQ(adjust->downVertAdjust, Evpu(-51));
    EXPECT_EQ(adjust->upHorzAdjust, Evpu(23));
    EXPECT_EQ(adjust->downHorzAdjust, Evpu(-21));
}

TEST(StemAlterationsUnderBeamTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <beamStemAdjust entnum="6">
      <upVertAdjust>-31</upVertAdjust>
      <downVertAdjust>42</downVertAdjust>
      <upHorzAdjust>23</upHorzAdjust>
      <downHorzAdjust>-23</downHorzAdjust>
    </beamStemAdjust>
   </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto adjust = details->get<details::StemAlterationsUnderBeam>(SCORE_PARTID, 6);
    ASSERT_TRUE(adjust) << "StemAlterations with entnum 6 not found";

    EXPECT_EQ(adjust->upVertAdjust, Evpu(-31));
    EXPECT_EQ(adjust->downVertAdjust, Evpu(42));
    EXPECT_EQ(adjust->upHorzAdjust, Evpu(23));
    EXPECT_EQ(adjust->downHorzAdjust, Evpu(-23));
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
