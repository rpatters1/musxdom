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

constexpr static musxtest::string_view textOptsXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <textOptions>
      <textLineSpacingPercent>100</textLineSpacingPercent>
      <showTimeSeconds/>
      <dateFormat>abbrev</dateFormat>
      <tabSpaces>4</tabSpaces>
      <insertSymbolInfo type="sharp">
        <trackingBefore>35</trackingBefore>
        <trackingAfter>23</trackingAfter>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontID>15</fontID>
          <fontSize>100</fontSize>
          <efx>
            <italic/>
          </efx>
        </symFont>
        <symChar>35</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="flat">
        <trackingBefore>60</trackingBefore>
        <baselineShiftPerc>19</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>98</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="natural">
        <trackingBefore>50</trackingBefore>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>110</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="dblSharp">
        <trackingBefore>40</trackingBefore>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>220</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="dblFlat">
        <trackingBefore>60</trackingBefore>
        <baselineShiftPerc>19</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>186</symChar>
      </insertSymbolInfo>
      <textTracking>5</textTracking>
      <textBaselineShift>1</textBaselineShift>
      <textSuperscript>7</textSuperscript>
      <textWordWrap/>
      <textPageOffset>3</textPageOffset>
      <textJustify>center</textJustify>
      <textExpandSingleWord/>
      <textHorzAlign>right</textHorzAlign>
      <textVertAlign>bottom</textVertAlign>
      <textIsEdgeAligned/>
    </textOptions>
  </options>
</finale>
)xml";

TEST(TextOptionsTest, PropertiesAndSymbolInserts)
{
    using TextOptions = musx::dom::options::TextOptions;
    using AccidentalInsertSymbolType = musx::dom::options::AccidentalInsertSymbolType;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(textOptsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto textOptions = options->get<TextOptions>();
    ASSERT_TRUE(textOptions);

    EXPECT_EQ(textOptions->textLineSpacingPercent, 100);
    EXPECT_TRUE(textOptions->showTimeSeconds);
    EXPECT_EQ(textOptions->dateFormat, musx::dom::DateFormat::Abbrev);
    EXPECT_EQ(textOptions->tabSpaces, 4);
    EXPECT_EQ(textOptions->textTracking, 5);
    EXPECT_EQ(textOptions->textBaselineShift, 1);
    EXPECT_EQ(textOptions->textSuperscript, 7);
    EXPECT_TRUE(textOptions->textWordWrap);
    EXPECT_EQ(textOptions->textPageOffset, 3);
    EXPECT_EQ(textOptions->textJustify, TextOptions::TextJustify::Center);
    EXPECT_TRUE(textOptions->textExpandSingleWord);
    EXPECT_EQ(textOptions->textHorzAlign, TextOptions::HorizontalAlignment::Right);
    EXPECT_EQ(textOptions->textVertAlign, TextOptions::VerticalAlignment::Bottom);
    EXPECT_TRUE(textOptions->textIsEdgeAligned);

    // insertSymbolInfo
    EXPECT_EQ(textOptions->symbolInserts.size(), 5);

    auto sharpInfo = textOptions->symbolInserts.at(AccidentalInsertSymbolType::Sharp);
    ASSERT_TRUE(sharpInfo);
    EXPECT_EQ(sharpInfo->trackingBefore, 35);
    EXPECT_EQ(sharpInfo->trackingAfter, 23);
    EXPECT_EQ(sharpInfo->baselineShiftPerc, 34);
    EXPECT_TRUE(sharpInfo->symFont);
    EXPECT_EQ(sharpInfo->symChar, U'#');

    auto flatInfo = textOptions->symbolInserts.at(AccidentalInsertSymbolType::Flat);
    ASSERT_TRUE(flatInfo);
    EXPECT_EQ(flatInfo->trackingBefore, 60);
    EXPECT_EQ(flatInfo->trackingAfter, 0); // not in xml
    EXPECT_EQ(flatInfo->baselineShiftPerc, 19);
    EXPECT_TRUE(flatInfo->symFont);
    EXPECT_EQ(flatInfo->symChar, U'b');

    auto naturalInfo = textOptions->symbolInserts.at(AccidentalInsertSymbolType::Natural);
    ASSERT_TRUE(naturalInfo);
    EXPECT_EQ(naturalInfo->trackingBefore, 50);
    EXPECT_EQ(naturalInfo->trackingAfter, 0); // not in xml
    EXPECT_EQ(naturalInfo->baselineShiftPerc, 34);
    EXPECT_TRUE(naturalInfo->symFont);
    EXPECT_EQ(naturalInfo->symChar, U'n');

    auto dblSharpInfo = textOptions->symbolInserts.at(AccidentalInsertSymbolType::DblSharp);
    ASSERT_TRUE(dblSharpInfo);
    EXPECT_EQ(dblSharpInfo->trackingBefore, 40);
    EXPECT_EQ(dblSharpInfo->baselineShiftPerc, 34);
    EXPECT_TRUE(dblSharpInfo->symFont);
    EXPECT_EQ(dblSharpInfo->symChar, U'\u00DC'); // Unicode 220 is Ü (you may adjust if your codepoint->char32_t mapping differs)

    auto dblFlatInfo = textOptions->symbolInserts.at(AccidentalInsertSymbolType::DblFlat);
    ASSERT_TRUE(dblFlatInfo);
    EXPECT_EQ(dblFlatInfo->trackingBefore, 60);
    EXPECT_EQ(dblFlatInfo->baselineShiftPerc, 19);
    EXPECT_TRUE(dblFlatInfo->symFont);
    EXPECT_EQ(dblFlatInfo->symChar, U'\u00BA'); // Unicode 186 is º (same note as above)
}
