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

constexpr static musxtest::string_view mmRestXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <mmRest cmper="27" part="1" shared="false">
      <meaSpace>288</meaSpace>
      <nextMeas>32</nextMeas>
      <numdec>-28</numdec>
      <shapeDef>119</shapeDef>
      <numStart>5</numStart>
      <threshold>2</threshold>
      <spacing>48</spacing>
      <numAdjX>3</numAdjX>
      <startAdjust>1</startAdjust>
      <endAdjust>2</endAdjust>
      <useCharRestStyle/>
    </mmRest>
  </others>
</finale>
    )xml";

TEST(MultimeasureRestTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(mmRestXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto mmRest = others->get<others::MultimeasureRest>(1, 27);
    ASSERT_TRUE(mmRest) << "MultimeasureRest with partId 1, measure 27 not found";

    EXPECT_EQ(mmRest->measWidth, 288);
    EXPECT_EQ(mmRest->nextMeas, 32);
    EXPECT_EQ(mmRest->numVertAdj, -28);
    EXPECT_EQ(mmRest->shapeDef, 119);
    EXPECT_EQ(mmRest->numStart, 5);
    EXPECT_EQ(mmRest->symbolThreshold, 2);
    EXPECT_EQ(mmRest->symbolSpacing, 48);
    EXPECT_EQ(mmRest->numHorzAdj, 3);
    EXPECT_EQ(mmRest->shapeStartAdjust, 1);
    EXPECT_EQ(mmRest->shapeEndAdjust, 2);
    EXPECT_TRUE(mmRest->useSymbols);
}


TEST(MultimeasureRestTest, IntegrityCheckFailure)
{

  constexpr static musxtest::string_view xmlBadMmRest = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <mmRest cmper="7" part="1" shared="false">
      <meaSpace>288</meaSpace>
      <nextMeas>7</nextMeas>
      <numdec>-28</numdec>
      <shapeDef>119</shapeDef>
      <numStart>1</numStart>
      <threshold>2</threshold>
      <spacing>48</spacing>
      <useCharRestStyle/>
    </mmRest>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xmlBadMmRest),
        musx::dom::integrity_error
    ) << "mmRest spans 0 bars should cause integrity error";
}