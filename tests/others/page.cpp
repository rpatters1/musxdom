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

constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <pageSpec cmper="1">
      <height>3744</height>
      <width>2880</width>
      <percent>80</percent>
      <firstSystem>1</firstSystem>
      <margTop>-288</margTop>
      <margLeft>288</margLeft>
      <margBottom>288</margBottom>
      <margRight>-288</margRight>
    </pageSpec>
    <pageSpec cmper="2">
      <height>1584</height>
      <width>2448</width>
      <percent>95</percent>
      <firstSystem>-1</firstSystem>
      <margTop>-288</margTop>
      <margLeft>288</margLeft>
      <margBottom>288</margBottom>
      <margRight>-288</margRight>
    </pageSpec>
  </others>
</finale>
)xml";

TEST(PageTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    
    auto page = others->get<others::Page>(SCORE_PARTID, 1);
    ASSERT_TRUE(page) << "Page with cmper 1 not found";
    
    EXPECT_EQ(page->height, 3744);
    EXPECT_EQ(page->width, 2880);
    EXPECT_EQ(page->percent, 80);
    EXPECT_EQ(page->firstSystem, 1);
    EXPECT_FALSE(page->holdMargins);
    EXPECT_EQ(page->margTop, -288);
    EXPECT_EQ(page->margLeft, 288);
    EXPECT_EQ(page->margBottom, 288);
    EXPECT_EQ(page->margRight, -288);
    EXPECT_FALSE(page->isBlank());

    page = others->get<others::Page>(SCORE_PARTID, 2);
    ASSERT_TRUE(page) << "Page with cmper 2 not found";
    EXPECT_TRUE(page->isBlank());
}
