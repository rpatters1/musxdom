/*
 * Copyright (C) 2024, Robert Patterson
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

TEST(MusicSpacingOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions>
      <minWidth>216</minWidth>
      <maxWidth>1800</maxWidth>
      <minDistance>12</minDistance>
      <minDistTiedNotes>24</minDistTiedNotes>
      <avoidColNotes/>
      <avoidColLyrics/>
      <avoidColChords/>
      <avoidColArtics/>
      <avoidColClefs/>
      <avoidColSeconds/>
      <avoidColStems/>
      <avoidColUnisons>all</avoidColUnisons>
      <avoidColLedgers/>
      <manualPositioning>incorp</manualPositioning>
      <ignoreHidden/>
      <useAllottmentTables/>
      <referenceDuration>1024</referenceDuration>
      <referenceWidth>96</referenceWidth>
      <scalingFactor>1.414100e+00</scalingFactor>
      <defaultAllotment>84</defaultAllotment>
      <minDistGrace>12</minDistGrace>
      <graceNoteManualPositioning>keep</graceNoteManualPositioning>
      <musFront>48</musFront>
      <musBack>24</musBack>
    </musicSpacingOptions>
  </options>
</finale>
)xml";

    using ColUnisonsChoice = musx::dom::options::MusicSpacingOptions::ColUnisonsChoice;
    using ManualPositioning = musx::dom::options::MusicSpacingOptions::ManualPositioning;
    using GraceNoteSpacing = musx::dom::options::MusicSpacingOptions::GraceNoteSpacing;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto musicSpacingOptions = options->get<musx::dom::options::MusicSpacingOptions>();
    ASSERT_TRUE(musicSpacingOptions);

    // Test all properties of MusicSpacingOptions
    EXPECT_EQ(musicSpacingOptions->minWidth, 216);
    EXPECT_EQ(musicSpacingOptions->maxWidth, 1800);
    EXPECT_EQ(musicSpacingOptions->minDistance, 12);
    EXPECT_EQ(musicSpacingOptions->minDistTiedNotes, 24);
    EXPECT_TRUE(musicSpacingOptions->avoidColNotes);
    EXPECT_TRUE(musicSpacingOptions->avoidColLyrics);
    EXPECT_TRUE(musicSpacingOptions->avoidColChords);
    EXPECT_TRUE(musicSpacingOptions->avoidColArtics);
    EXPECT_TRUE(musicSpacingOptions->avoidColClefs);
    EXPECT_TRUE(musicSpacingOptions->avoidColSeconds);
    EXPECT_TRUE(musicSpacingOptions->avoidColStems);
    EXPECT_EQ(musicSpacingOptions->avoidColUnisons, ColUnisonsChoice::All);
    EXPECT_TRUE(musicSpacingOptions->avoidColLedgers);
    EXPECT_EQ(musicSpacingOptions->manualPositioning, ManualPositioning::Incorporate);
    EXPECT_TRUE(musicSpacingOptions->ignoreHidden);
    EXPECT_TRUE(musicSpacingOptions->useAllottmentTables);
    EXPECT_EQ(musicSpacingOptions->referenceDuration, 1024);
    EXPECT_EQ(musicSpacingOptions->referenceWidth, 96);
    EXPECT_DOUBLE_EQ(musicSpacingOptions->scalingFactor, 1.4141);
    EXPECT_EQ(musicSpacingOptions->defaultAllotment, 84);
    EXPECT_EQ(musicSpacingOptions->minDistGrace, 12);
    EXPECT_EQ(musicSpacingOptions->graceNoteSpacing, GraceNoteSpacing::KeepCurrent);
    EXPECT_EQ(musicSpacingOptions->musFront, 48);
    EXPECT_EQ(musicSpacingOptions->musBack, 24);
}

TEST(MusicSpacingOptionsTest, EnumDefaultsTest)
{

    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <musicSpacingOptions/>
  </options>
</finale>
)xml";

    using ColUnisonsChoice = musx::dom::options::MusicSpacingOptions::ColUnisonsChoice;
    using ManualPositioning = musx::dom::options::MusicSpacingOptions::ManualPositioning;
    using GraceNoteSpacing = musx::dom::options::MusicSpacingOptions::GraceNoteSpacing;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto musicSpacingOptions = options->get<musx::dom::options::MusicSpacingOptions>();
    ASSERT_TRUE(musicSpacingOptions);

    // Test enums takes correct default
    EXPECT_EQ(musicSpacingOptions->avoidColUnisons, ColUnisonsChoice::None);
    EXPECT_EQ(musicSpacingOptions->manualPositioning, ManualPositioning::Clear);
    EXPECT_EQ(musicSpacingOptions->graceNoteSpacing, GraceNoteSpacing::ResetToEntry);
}