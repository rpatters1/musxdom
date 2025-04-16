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

TEST(ChordOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <chordOptions>
      <fretPercent>7.500000e+01</fretPercent>
      <chordPercent>1.100000e+02</chordPercent>
      <chordSharpLift>12</chordSharpLift>
      <chordFlatLift>13</chordFlatLift>
      <chordNaturalLift>14</chordNaturalLift>
      <showFretboards/>
      <fretStyleID>2</fretStyleID>
      <fretInstID>1</fretInstID>
      <multiFretItemsPerStr/>
      <italicizeCapoChords/>
      <chordAlignment>center</chordAlignment>
      <chordStyle>solfeggio</chordStyle>
      <useSimpleChordSpelling/>
      <chordPlayback/>
    </chordOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto chordOptions = options->get<options::ChordOptions>();
    ASSERT_TRUE(chordOptions);

    // Test all properties of ChordOptions
    EXPECT_DOUBLE_EQ(chordOptions->fretPercent, 75.0);
    EXPECT_DOUBLE_EQ(chordOptions->chordPercent, 110.0);
    EXPECT_EQ(chordOptions->chordSharpLift, 12);
    EXPECT_EQ(chordOptions->chordFlatLift, 13);
    EXPECT_EQ(chordOptions->chordNaturalLift, 14);
    EXPECT_TRUE(chordOptions->showFretboards);
    EXPECT_EQ(chordOptions->fretStyleId, 2);
    EXPECT_EQ(chordOptions->fretInstId, 1);
    EXPECT_TRUE(chordOptions->multiFretItemsPerStr);
    EXPECT_TRUE(chordOptions->italicizeCapoChords);
    EXPECT_EQ(chordOptions->chordAlignment, options::ChordOptions::ChordAlignment::Center);
    EXPECT_EQ(chordOptions->chordStyle, options::ChordOptions::ChordStyle::Solfeggio);
    EXPECT_TRUE(chordOptions->useSimpleChordSpelling);
    EXPECT_TRUE(chordOptions->chordPlayback);
}

TEST(ChordOptionsTest, PropertiesDefaultTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <chordOptions>
      <fretPercent>7.500000e+01</fretPercent>
      <chordPercent>1.100000e+02</chordPercent>
      <chordSharpLift>12</chordSharpLift>
      <chordFlatLift>13</chordFlatLift>
      <chordNaturalLift>14</chordNaturalLift>
      <fretStyleID>2</fretStyleID>
      <fretInstID>1</fretInstID>
      <multiFretItemsPerStr/>
      <italicizeCapoChords/>
      <useSimpleChordSpelling/>
      <chordPlayback/>
    </chordOptions>
  </options>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto chordOptions = options->get<options::ChordOptions>();
    ASSERT_TRUE(chordOptions);

    // Test all properties of ChordOptions
    EXPECT_DOUBLE_EQ(chordOptions->fretPercent, 75.0);
    EXPECT_DOUBLE_EQ(chordOptions->chordPercent, 110.0);
    EXPECT_EQ(chordOptions->chordSharpLift, 12);
    EXPECT_EQ(chordOptions->chordFlatLift, 13);
    EXPECT_EQ(chordOptions->chordNaturalLift, 14);
    EXPECT_FALSE(chordOptions->showFretboards);
    EXPECT_EQ(chordOptions->fretStyleId, 2);
    EXPECT_EQ(chordOptions->fretInstId, 1);
    EXPECT_TRUE(chordOptions->multiFretItemsPerStr);
    EXPECT_TRUE(chordOptions->italicizeCapoChords);
    EXPECT_EQ(chordOptions->chordAlignment, options::ChordOptions::ChordAlignment::Left);
    EXPECT_EQ(chordOptions->chordStyle, options::ChordOptions::ChordStyle::Standard);
    EXPECT_TRUE(chordOptions->useSimpleChordSpelling);
    EXPECT_TRUE(chordOptions->chordPlayback);
}
