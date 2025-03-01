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

constexpr static musxtest::string_view fontProperties = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <fontOptions>
      <font type="music">
        <fontID>13</fontID>
        <fontSize>24</fontSize>
      </font>
      <font type="ending">
        <fontID>1</fontID>
        <fontSize>12</fontSize>
        <efx>
          <bold/>
          <italic/>
          <underline/>
          <strikeout/>
          <absolute/>
          <hidden/>
        </efx>
      </font>
    </fontOptions>
  </options>
  <others>
    <fontName cmper="1">
      <charsetBank>Mac</charsetBank>
      <charsetVal>1</charsetVal>
      <pitch>2</pitch>
      <family>3</family>
      <name>Times</name>
    </fontName>
  </others>
</finale>
    )xml";

TEST(FontTest, FontInfoPropertiesTest)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fontProperties);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);
    auto fontOptions = options->get<options::FontOptions>();
    ASSERT_TRUE(fontOptions);
    EXPECT_THROW(
        fontOptions->getFontInfo(options::FontOptions::FontType::AbbrvStaffNames),
        std::invalid_argument
    );
    auto fontInfo = fontOptions->getFontInfo(options::FontOptions::FontType::Ending);
    ASSERT_TRUE(fontInfo);
    EXPECT_EQ(fontInfo->fontId, 1);
    EXPECT_EQ(fontInfo->fontSize, 12);
    EXPECT_TRUE(fontInfo->bold);
    EXPECT_TRUE(fontInfo->italic);
    EXPECT_TRUE(fontInfo->underline);
    EXPECT_TRUE(fontInfo->strikeout);
    EXPECT_TRUE(fontInfo->absolute);
    EXPECT_TRUE(fontInfo->hidden);
    EXPECT_EQ(fontInfo->getName(), "Times");
}

TEST(FontTest, FontDefinitionProperties)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(fontProperties);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    auto fontDef = others->get<others::FontDefinition>(SCORE_PARTID, 1);
    ASSERT_TRUE(fontDef);
    EXPECT_EQ(fontDef->charsetBank, others::FontDefinition::CharacterSetBank::MacOS);
    EXPECT_EQ(fontDef->charsetVal, 1);
    EXPECT_EQ(fontDef->pitch, 2);
    EXPECT_EQ(fontDef->family, 3);
    EXPECT_EQ(fontDef->name, "Times");
}

TEST(FontTest, FontInfoNoName)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fontProperties);
    auto fontInfo = options::FontOptions::getFontInfo(doc, options::FontOptions::FontType::Music);
    ASSERT_TRUE(fontInfo);
    EXPECT_EQ(fontInfo->fontId, 13);
    EXPECT_EQ(fontInfo->fontSize, 24);
    EXPECT_FALSE(fontInfo->bold);
    EXPECT_FALSE(fontInfo->italic);
    EXPECT_FALSE(fontInfo->underline);
    EXPECT_FALSE(fontInfo->strikeout);
    EXPECT_FALSE(fontInfo->absolute);
    EXPECT_FALSE(fontInfo->hidden);
    EXPECT_THROW(
        // no fontName record for this font in xml, so this will throw
        ASSERT_EQ(fontInfo->getName(), "Finale Maestro"),
        std::invalid_argument
    );
}
