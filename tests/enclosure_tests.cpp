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


TEST(EnclosureTest, TextExpressionEnclosure)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textExpressionEnclosure cmper="25">
      <xAdd>-2</xAdd>
      <yAdd>2</yAdd>
      <xMargin>-9</xMargin>
      <yMargin>23</yMargin>
      <lineWidth>128</lineWidth>
      <sides>3</sides>
      <fixedSize/>
      <notTall/>
      <opaque/>
      <roundCorners/>
      <cornerRadius>1088</cornerRadius>
    </textExpressionEnclosure>
  </others>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
        auto others = doc->getOthers();
        ASSERT_TRUE(others);
        auto enclosure = others->get<musx::dom::others::TextExpressionEnclosure>(1);
        EXPECT_FALSE(enclosure) << "Enclosure 1 found but does not exists";
        enclosure = others->get<musx::dom::others::TextExpressionEnclosure>(25);
        ASSERT_TRUE(enclosure) << "Enclosure 25 not found but does exist";
        EXPECT_EQ(enclosure->xAdd, -2);
        EXPECT_EQ(enclosure->yAdd, 2);
        EXPECT_EQ(enclosure->xMargin, -9);
        EXPECT_EQ(enclosure->yMargin, 23);
        EXPECT_EQ(enclosure->lineWidth, 128);
        EXPECT_EQ(enclosure->shape, musx::dom::others::Enclosure::Shape::Triangle);
        EXPECT_EQ(enclosure->cornerRadius, 1088);
        EXPECT_TRUE(enclosure->fixedSize);
        EXPECT_TRUE(enclosure->notTall);
        EXPECT_TRUE(enclosure->opaque);
        EXPECT_TRUE(enclosure->roundCorners);
    }
}

TEST(EnclosureTest, TextRepeatEnclosure)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textRepeatEnclosure cmper="12">
      <xMargin>9</xMargin>
      <yMargin>9</yMargin>
      <lineWidth>256</lineWidth>
      <sides>1</sides>
      <notTall/>
    </textRepeatEnclosure>
  </others>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
        auto others = doc->getOthers();
        ASSERT_TRUE(others);
        auto enclosure = others->get<musx::dom::others::TextRepeatEnclosure>(1);
        EXPECT_FALSE(enclosure) << "Enclosure 1 found but does not exists";
        enclosure = others->get<musx::dom::others::TextRepeatEnclosure>(12);
        ASSERT_TRUE(enclosure) << "Enclosure 12 not found but does exist";
        EXPECT_EQ(enclosure->xAdd, 0);
        EXPECT_EQ(enclosure->yAdd, 0);
        EXPECT_EQ(enclosure->xMargin, 9);
        EXPECT_EQ(enclosure->yMargin, 9);
        EXPECT_EQ(enclosure->lineWidth, 256);
        EXPECT_EQ(enclosure->shape, musx::dom::others::Enclosure::Shape::Rectangle);
        EXPECT_EQ(enclosure->cornerRadius, 0);
        EXPECT_FALSE(enclosure->fixedSize);
        EXPECT_TRUE(enclosure->notTall);
        EXPECT_FALSE(enclosure->opaque);
        EXPECT_FALSE(enclosure->roundCorners);
    }
}

TEST(EnclosureTest, InvalidShape)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <textRepeatEnclosure cmper="12">
      <sides>9</sides>
    </textRepeatEnclosure>
  </others>
</finale>
    )xml";
    {
        EXPECT_THROW(
            auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
            std::invalid_argument
        );
    }
}
