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

#include <cmath>

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::dom;

TEST(PopulateTest, BeatChartElement)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <beatChart cmper="2" inci="0">
      <control>
        <totalDur>4096</totalDur>
        <totalWidth>539</totalWidth>
        <minWidth>1</minWidth>
        <allotWidth>932</allotWidth>
      </control>
    </beatChart>
    <beatChart cmper="2" inci="1">
      <endPos>43</endPos>
    </beatChart>
    <beatChart cmper="2" inci="2">
      <dur>256</dur>
      <pos>43</pos>
      <endPos>114</endPos>
      <minPos>1</minPos>
    </beatChart>
    <beatChart cmper="2" inci="3">
      <dur>512</dur>
      <pos>114</pos>
      <endPos>185</endPos>
      <minPos>1</minPos>
    </beatChart>
    <beatChart cmper="2" inci="4">
      <dur>768</dur>
      <pos>185</pos>
      <endPos>228</endPos>
      <minPos>1</minPos>
    </beatChart>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Control element (inci 0)
    auto bc0 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 0);
    ASSERT_TRUE(bc0);
    ASSERT_TRUE(bc0->control);
    EXPECT_EQ(bc0->control->totalDur, 4096);
    EXPECT_EQ(bc0->control->totalWidth, 539);
    EXPECT_EQ(bc0->control->minWidth, 1);
    EXPECT_EQ(bc0->control->allotWidth, 932);

    // Entry inci 1: endPos only; other values should default to 0
    auto bc1 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 1);
    ASSERT_TRUE(bc1);
    EXPECT_FALSE(bc1->control);
    EXPECT_EQ(bc1->endPos, 43);
    EXPECT_EQ(bc1->dur, 0);
    EXPECT_EQ(bc1->pos, 0);
    EXPECT_EQ(bc1->minPos, 0);

    // Entry inci 2
    auto bc2 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 2);
    ASSERT_TRUE(bc2);
    EXPECT_FALSE(bc2->control);
    EXPECT_EQ(bc2->dur, 256);
    EXPECT_EQ(bc2->pos, 43);
    EXPECT_EQ(bc2->endPos, 114);
    EXPECT_EQ(bc2->minPos, 1);

    // Entry inci 3
    auto bc3 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 3);
    ASSERT_TRUE(bc3);
    EXPECT_FALSE(bc3->control);
    EXPECT_EQ(bc3->dur, 512);
    EXPECT_EQ(bc3->pos, 114);
    EXPECT_EQ(bc3->endPos, 185);
    EXPECT_EQ(bc3->minPos, 1);

    // Entry inci 4
    auto bc4 = others->get<others::BeatChartElement>(SCORE_PARTID, 2, 4);
    ASSERT_TRUE(bc4);
    EXPECT_FALSE(bc4->control);
    EXPECT_EQ(bc4->dur, 768);
    EXPECT_EQ(bc4->pos, 185);
    EXPECT_EQ(bc4->endPos, 228);
    EXPECT_EQ(bc4->minPos, 1);
}

TEST(BeatChartElement, EduFromEvpuWithoutBeatChart)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions>
      <musFront>36</musFront>
    </musicSpacingOptions>
  </options>
  <others>
    <measSpec cmper="1">
      <width>600</width>
      <beats>4</beats>
      <divbeat>1024</divbeat>
    </measSpec>
    <measSpec cmper="2">
      <width>600</width>
      <beats>4</beats>
      <divbeat>1024</divbeat>
    </measSpec>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure);

    EXPECT_DOUBLE_EQ(measure->calcFirstBeatEvpu(), 36.0);
    // The music area runs from musFront to the measure width.
    EXPECT_EQ(std::lround(measure->calcEduFromEvpu(110)), 537);
    EXPECT_EQ(std::lround(measure->calcEduFromEvpu(278)), 1758);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(318), 2048.0);
    // Before the first beat clamps to the start; past the end continues at the same rate.
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(20), 0.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(741), 5120.0);
}

TEST(BeatChartElement, EduFromEvpuWithMusBack)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions>
      <musFront>24</musFront>
      <musBack>40</musBack>
    </musicSpacingOptions>
  </options>
  <others>
    <measSpec cmper="1">
      <width>600</width>
      <beats>3</beats>
      <divbeat>1024</divbeat>
    </measSpec>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure);

    EXPECT_DOUBLE_EQ(measure->calcFirstBeatEvpu(), 24.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(292), 1536.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(560), 3072.0);
}

static std::string beatChartMeasureXml(int width)
{
    return R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions>
      <musFront>36</musFront>
    </musicSpacingOptions>
  </options>
  <others>
    <beatChart cmper="1" inci="0">
      <control>
        <totalDur>4096</totalDur>
        <totalWidth>510</totalWidth>
      </control>
    </beatChart>
    <beatChart cmper="1" inci="1">
      <pos>10</pos>
      <endPos>60</endPos>
    </beatChart>
    <beatChart cmper="1" inci="2">
      <dur>2048</dur>
      <pos>210</pos>
      <endPos>260</endPos>
    </beatChart>
    <beatChart cmper="1" inci="3">
      <dur>3072</dur>
      <pos>460</pos>
      <endPos>510</endPos>
    </beatChart>
    <measSpec cmper="1">
      <width>)xml" + std::to_string(width) + R"xml(</width>
      <beats>4</beats>
      <divbeat>1024</divbeat>
    </measSpec>
  </others>
</finale>
    )xml";
}

TEST(BeatChartElement, EduFromEvpuWithBeatChart)
{
    // The elements span 510 Evpu unstretched, which exactly fills a music area from musFront (36) to the width.
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(beatChartMeasureXml(546));
    auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure);

    EXPECT_DOUBLE_EQ(measure->calcFirstBeatEvpu(), 46.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(46), 0.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(146), 1024.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(246), 2048.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(371), 2560.0);
    // The last element's span ends at the barline.
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(521), 3584.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(546), 4096.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(571), 4608.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(0), 0.0);
}

TEST(BeatChartElement, EduFromEvpuWithStretchedBeatChart)
{
    // The same chart in a measure whose music area is twice as wide is stretched by two.
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(beatChartMeasureXml(1056));
    auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure);

    EXPECT_DOUBLE_EQ(measure->calcFirstBeatEvpu(), 56.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(256), 1024.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(456), 2048.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(1006), 3584.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(1056), 4096.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(1106), 4608.0);
}

TEST(BeatChartElement, EduFromEvpuWithAutomaticBeatChart)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions>
      <musFront>36</musFront>
    </musicSpacingOptions>
  </options>
  <others>
    <beatChart cmper="1" inci="0">
      <control>
        <totalDur>4096</totalDur>
        <totalWidth>312</totalWidth>
        <minWidth>216</minWidth>
        <allotWidth>273</allotWidth>
      </control>
    </beatChart>
    <beatChart cmper="1" inci="1">
      <endPos>96</endPos>
      <minPos>3</minPos>
    </beatChart>
    <beatChart cmper="1" inci="2">
      <dur>2048</dur>
      <pos>96</pos>
      <endPos>192</endPos>
      <minPos>51</minPos>
    </beatChart>
    <measSpec cmper="1">
      <width>351</width>
      <beats>4</beats>
      <divbeat>1024</divbeat>
    </measSpec>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure);

    // The ideal layout (totalWidth 312) is wider than the minimum one (minWidth 216), so the slots are at their
    // positions, moved right by the first element's minPos (3). The spacing is 312 + 3 = 315 Evpu and fills the
    // music area exactly.
    const auto spacing = measure->calcSpacing();
    EXPECT_EQ(spacing.musicStart, 36);
    EXPECT_EQ(spacing.musicWidth, 315);
    EXPECT_EQ(spacing.spacingWidth, 315);
    EXPECT_EQ(spacing.endEdu, 4096);
    ASSERT_EQ(spacing.slots.size(), 2u);
    EXPECT_EQ(spacing.slots[0].offset, 3);
    EXPECT_EQ(spacing.slots[0].edu, 0);
    EXPECT_EQ(spacing.slots[1].offset, 99);
    EXPECT_EQ(spacing.slots[1].edu, 2048);

    EXPECT_DOUBLE_EQ(measure->calcFirstBeatEvpu(), 39.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(87), 1024.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(135), 2048.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(243), 3072.0);
    EXPECT_DOUBLE_EQ(measure->calcEduFromEvpu(351), 4096.0);
}

TEST(BeatChartElement, EduFromEvpuForSplitPoints)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "beatchart_splitpoint.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);

    // Every measure has an automatically spaced beat chart whose display positions follow minPos, and extra space at
    // each end. Respacing with musBack 73 widened every measure without changing its chart, and the splits were
    // realigned; the helper stretches each chart to fill its measure. Each split point was placed by eye: in measure 1 at the start of a hairpin that begins at Edu 3584,
    // and in measure 2 at the end of a hairpin that ends at Edu 1600. Measure 3 copies measure 1 without its time
    // signature, which changes nothing stored. Measure 4 copies measure 2 with the split at the start of a hairpin
    // that begins at Edu 3840, after the last beat chart element. Measure 5 copies measure 3 with its last three
    // beat chart elements removed and the split at the end of a hairpin that ends at Edu 3968, in the last element's
    // span. Measures 4 and 5 place the end of the music area at the barline, not before the extra space at the back.
    // These values check the conversion, not its accuracy: each split was aligned by eye.
    struct Expected
    {
        MeasCmper measureId;
        Evpu firstBeat;
        Edu splitEdu;
    };
    for (const auto& expected : {Expected{1, 176, 3597}, Expected{2, 140, 1529}, Expected{3, 176, 3597}, Expected{4, 140, 3972},
             Expected{5, 176, 4008}}) {
        auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, expected.measureId);
        auto split = doc->getOthers()->get<others::SplitMeasure>(SCORE_PARTID, expected.measureId);
        ASSERT_TRUE(measure);
        ASSERT_TRUE(split);
        ASSERT_FALSE(split->values.empty());
        EXPECT_EQ(std::lround(measure->calcFirstBeatEvpu()), expected.firstBeat);
        EXPECT_EQ(std::lround(measure->calcEduFromEvpu(split->values[0])), expected.splitEdu);
    }
}
