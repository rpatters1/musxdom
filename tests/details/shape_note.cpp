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

TEST(PopulateTest, ShapeNoteBase)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <shapeNote cmper1="0" cmper2="0">
      <noteShapes index="0">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="1">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>208</q>
      </noteShapes>
      <noteShapes index="2">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>209</q>
      </noteShapes>
      <noteShapes index="3">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>210</q>
      </noteShapes>
      <noteShapes index="4">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>211</q>
      </noteShapes>
      <noteShapes index="5">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>212</q>
      </noteShapes>
      <noteShapes index="6">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>213</q>
      </noteShapes>
      <noteShapes index="7">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="8">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="9">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="10">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="11">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="12">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="13">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="14">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="15">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
    </shapeNote>
    <shapeNoteStyle cmper1="25" cmper2="0">
      <noteShapes index="0">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="1">
        <d>87</d>
        <w>120</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="2">
        <d>87</d>
        <w>121</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="3">
        <d>87</d>
        <w>122</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="4">
        <d>87</d>
        <w>123</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="5">
        <d>87</d>
        <w>124</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="6">
        <d>87</d>
        <w>125</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="7">
        <d>87</d>
        <w>126</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <noteShapes index="8">
        <d>87</d>
        <w>119</w>
        <h>250</h>
        <q>207</q>
      </noteShapes>
      <arrangedByPitch/>
    </shapeNoteStyle>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // shapeNote
    auto shapeNote = details->get<details::ShapeNote>(SCORE_PARTID, 0, 0);
    ASSERT_TRUE(shapeNote);
    ASSERT_EQ(shapeNote->noteShapes.size(), 16);
    EXPECT_FALSE(shapeNote->arrangedByPitch);
    EXPECT_EQ(shapeNote->noteShapes[0]->doubleWhole, 87);
    EXPECT_EQ(shapeNote->noteShapes[0]->whole, 119);
    EXPECT_EQ(shapeNote->noteShapes[0]->half, 250);
    EXPECT_EQ(shapeNote->noteShapes[0]->quarter, 207);
    EXPECT_EQ(shapeNote->noteShapes[6]->quarter, 213);
    EXPECT_EQ(shapeNote->noteShapes[15]->quarter, 207);

    // shapeNoteStyle
    auto shapeStyle = details->get<details::ShapeNoteStyle>(SCORE_PARTID, 25, 0);
    ASSERT_TRUE(shapeStyle);
    ASSERT_EQ(shapeStyle->noteShapes.size(), 9);
    EXPECT_TRUE(shapeStyle->arrangedByPitch);
    EXPECT_EQ(shapeStyle->noteShapes[0]->doubleWhole, 87);
    EXPECT_EQ(shapeStyle->noteShapes[0]->half, 250);
    EXPECT_EQ(shapeStyle->noteShapes[0]->quarter, 207);
    EXPECT_EQ(shapeStyle->noteShapes[0]->whole, 119);
    EXPECT_EQ(shapeStyle->noteShapes[1]->whole, 120);
    EXPECT_EQ(shapeStyle->noteShapes[6]->whole, 125);
    EXPECT_EQ(shapeStyle->noteShapes[8]->whole, 119);
}
