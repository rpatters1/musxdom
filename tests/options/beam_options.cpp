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

TEST(BeamOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <beamOptions>
      <beamStubLength>18</beamStubLength>
      <maxSlope>12</maxSlope>
      <beamSepar>18</beamSepar>
      <hmBeamTip>24</hmBeamTip>
      <beamingStyle>onExtremeNote</beamingStyle>
      <incEdgeRestsInBeamGroups/>
      <incRestsInClassicBeams/>
      <beamFourEighthsInCommonTime/>
      <beamThreeEighthsInCommonTime/>
      <doStemStubs/>
      <oldFinaleRestBeams/>
      <spanSpace/>
      <extendSecBeamsOverRests/>
      <beamWidth>768</beamWidth>
    </beamOptions>
  </options>
</finale>
)xml";

    using FlattenStyle = musx::dom::options::BeamOptions::FlattenStyle;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto beamOptions = options->get<musx::dom::options::BeamOptions>();
    ASSERT_TRUE(beamOptions);

    // Test all properties of BeamOptions
    EXPECT_EQ(beamOptions->beamStubLength, 18);
    EXPECT_EQ(beamOptions->maxSlope, 12);
    EXPECT_EQ(beamOptions->beamSepar, 18);
    EXPECT_EQ(beamOptions->maxFromMiddle, 24);
    EXPECT_EQ(beamOptions->beamingStyle, FlattenStyle::OnExtremeNote);
    EXPECT_TRUE(beamOptions->extendBeamsOverRests);
    EXPECT_TRUE(beamOptions->incRestsInFourGroups);
    EXPECT_TRUE(beamOptions->beamFourEighthsInCommonTime);
    EXPECT_TRUE(beamOptions->beamThreeEighthsInCommonTime);
    EXPECT_TRUE(beamOptions->dispHalfStemsOnRests);
    EXPECT_TRUE(beamOptions->oldFinaleRestBeams);
    EXPECT_TRUE(beamOptions->spanSpace);
    EXPECT_TRUE(beamOptions->extendSecBeamsOverRests);
    EXPECT_EQ(beamOptions->beamWidth, 768);
}

TEST(BeamOptionsTest, EnumDefaultsTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <beamOptions/>
  </options>
</finale>
)xml";

    using FlattenStyle = musx::dom::options::BeamOptions::FlattenStyle;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto beamOptions = options->get<musx::dom::options::BeamOptions>();
    ASSERT_TRUE(beamOptions);

    // Test enum takes correct default
    EXPECT_EQ(beamOptions->beamingStyle, FlattenStyle::OnEndNotes);
}
