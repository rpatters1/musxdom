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
    auto defaultFonts = options->get<musx::dom::options::DefaultFonts>();
    ASSERT_TRUE(defaultFonts);
    EXPECT_THROW(
        defaultFonts->getFontInfo(musx::dom::options::DefaultFonts::FontType::AbbrvStaffNames),
        std::invalid_argument
    );
    auto fontInfo = defaultFonts->getFontInfo(musx::dom::options::DefaultFonts::FontType::Ending);
    ASSERT_TRUE(fontInfo);
    ASSERT_EQ(fontInfo->fontID, 1);
    ASSERT_EQ(fontInfo->fontSize, 12);
    ASSERT_TRUE(fontInfo->bold);
    ASSERT_TRUE(fontInfo->italic);
    ASSERT_TRUE(fontInfo->underline);
    ASSERT_TRUE(fontInfo->strikeout);
    ASSERT_TRUE(fontInfo->absolute);
    ASSERT_TRUE(fontInfo->hidden);
    ASSERT_EQ(fontInfo->getFontName(), "Times");
}

TEST(FontTest, FontDefinitionProperties)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(fontProperties);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    auto fontDef = others->get<musx::dom::others::FontDefinition>(1);
    ASSERT_TRUE(fontDef);
    ASSERT_EQ(fontDef->charsetBank, "Mac");
    ASSERT_EQ(fontDef->charsetVal, 1);
    ASSERT_EQ(fontDef->pitch, 2);
    ASSERT_EQ(fontDef->family, 3);
    ASSERT_EQ(fontDef->name, "Times");
}

TEST(FontTest, FontInfoNoName)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fontProperties);
    auto fontInfo = musx::dom::options::DefaultFonts::getFontInfo(doc, musx::dom::options::DefaultFonts::FontType::Music);
    ASSERT_TRUE(fontInfo);
    ASSERT_EQ(fontInfo->fontID, 13);
    ASSERT_EQ(fontInfo->fontSize, 24);
    ASSERT_FALSE(fontInfo->bold);
    ASSERT_FALSE(fontInfo->italic);
    ASSERT_FALSE(fontInfo->underline);
    ASSERT_FALSE(fontInfo->strikeout);
    ASSERT_FALSE(fontInfo->absolute);
    ASSERT_FALSE(fontInfo->hidden);
    EXPECT_THROW(
        // no fontName record for this font in xml, so this will throw
        ASSERT_EQ(fontInfo->getFontName(), "Finale Maestro"),
        std::invalid_argument
    );
}
