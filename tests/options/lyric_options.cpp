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

constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <lyricOptions>
      <hyphenChar>45</hyphenChar>
      <maxHyphenSeparation>144</maxHyphenSeparation>
      <wordExtVertOffset>1</wordExtVertOffset>
      <wordExtHorzOffset>4</wordExtHorzOffset>
      <useSmartWordExtensions/>
      <useSmartHyphens/>
      <smartHyphenStart>sometimes</smartHyphenStart>
      <wordExtMinLength>38</wordExtMinLength>
      <wordExtOffsetToNotehead/>
      <lyricAutoNumType>align</lyricAutoNumType>
      <wordExtLineWidth>115</wordExtLineWidth>
      <lyricSyllPosStyle type="default">
        <align>center</align>
        <justify>center</justify>
        <on/>
      </lyricSyllPosStyle>
      <lyricSyllPosStyle type="wordExt">
        <align>left</align>
        <justify>left</justify>
        <on/>
      </lyricSyllPosStyle>
      <lyricSyllPosStyle type="first">
        <align>center</align>
        <justify>left</justify>
        <on/>
      </lyricSyllPosStyle>
      <lyricSyllPosStyle type="systemStart">
        <align>center</align>
        <justify>left</justify>
        <on/>
      </lyricSyllPosStyle>
      <wordExtConnectStyle type="defaultStart">
        <connectIndex>lyricRightBottom</connectIndex>
        <xOffset>4</xOffset>
        <yOffset>1</yOffset>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="defaultEnd">
        <connectIndex>headRightLyrBaseline</connectIndex>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="systemStart">
        <connectIndex>systemLeft</connectIndex>
        <xOffset>12</xOffset>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="systemEnd">
        <connectIndex>systemRight</connectIndex>
        <xOffset>-12</xOffset>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="dottedEnd">
        <connectIndex>dotRightLyrBaseline</connectIndex>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="durationEnd">
        <connectIndex>durationLyrBaseline</connectIndex>
        <xOffset>-8</xOffset>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="oneEntryEnd">
        <connectIndex>lyricRightBottom</connectIndex>
        <xOffset>42</xOffset>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="zeroLengthEnd">
        <connectIndex>lyricRightBottom</connectIndex>
      </wordExtConnectStyle>
      <wordExtConnectStyle type="zeroOffset">
        <connectIndex>headRightLyrBaseline</connectIndex>
      </wordExtConnectStyle>
    </lyricOptions>
  </options>
</finale>
)xml";

TEST(LyricOptionsTest, PropertiesAndSubnodes)
{
    using LyricOptions = musx::dom::options::LyricOptions;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto lyric = options->get<LyricOptions>();
    ASSERT_TRUE(lyric);

    EXPECT_EQ(lyric->hyphenChar, U'-');
    EXPECT_EQ(lyric->maxHyphenSeparation, 144);
    EXPECT_EQ(lyric->wordExtVertOffset, 1);
    EXPECT_EQ(lyric->wordExtHorzOffset, 4);
    EXPECT_TRUE(lyric->useSmartWordExtensions);
    EXPECT_TRUE(lyric->useSmartHyphens);
    EXPECT_EQ(lyric->smartHyphenStart, LyricOptions::SmartHyphenStart::Sometimes);
    EXPECT_EQ(lyric->wordExtMinLength, 38);
    EXPECT_TRUE(lyric->wordExtOffsetToNotehead);
    EXPECT_EQ(lyric->lyricAutoNumType, LyricOptions::AutoNumberingAlign::Align);
    EXPECT_EQ(lyric->wordExtLineWidth, 115);
    EXPECT_TRUE(lyric->altHyphenFont); // populated in integrityCheck
    EXPECT_EQ(lyric->lyricPunctuationToIgnore, ",.?!;:'\"“”‘’"); // default

    // These were not in the XML and should remain default:
    EXPECT_FALSE(lyric->useAltHyphenFont);
    EXPECT_FALSE(lyric->wordExtNeedUnderscore);
    EXPECT_FALSE(lyric->lyricUseEdgePunctuation);

    // syllablePosStyles
    EXPECT_EQ(lyric->syllablePosStyles.size(), 4);
    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::Default]->align, LyricOptions::AlignJustify::Center);
    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::Default]->justify, LyricOptions::AlignJustify::Center);
    EXPECT_TRUE(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::Default]->on);

    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::WordExt]->align, LyricOptions::AlignJustify::Left);
    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::WordExt]->justify, LyricOptions::AlignJustify::Left);

    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::First]->justify, LyricOptions::AlignJustify::Left);
    EXPECT_EQ(lyric->syllablePosStyles[LyricOptions::SyllablePosStyleType::SystemStart]->justify, LyricOptions::AlignJustify::Left);

    // wordExtConnectStyles
    EXPECT_EQ(lyric->wordExtConnectStyles.size(), 9);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::DefaultStart]->connectIndex, LyricOptions::WordExtConnectIndex::LyricRightBottom);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::DefaultStart]->xOffset, 4);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::DefaultStart]->yOffset, 1);

    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::DefaultEnd]->connectIndex, LyricOptions::WordExtConnectIndex::HeadRightLyrBaseline);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::SystemStart]->xOffset, 12);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::SystemEnd]->xOffset, -12);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::DurationEnd]->xOffset, -8);
    EXPECT_EQ(lyric->wordExtConnectStyles[LyricOptions::WordExtConnectStyleType::OneEntryEnd]->xOffset, 42);
}
