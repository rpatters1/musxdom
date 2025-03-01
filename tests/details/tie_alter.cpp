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
   <details>
     <tieAlterStart entnum="6" inci="0">
       <noteID>1</noteID>
       <xStart>9</xStart>
       <xEnd>-9</xEnd>
       <outerLocal/>
       <afterSingleDotLocal/>
       <afterSingleDotOn/>
       <afterMultiDotsLocal/>
       <afterMultiDotsOn/>
       <shiftForSecondsLocal/>
       <shiftForSecondsOn/>
       <beforeSingleAcciLocal/>
       <beforeSingleAcciOn/>
       <down/>
       <breakTimeLocal/>
       <breakTimeOn/>
       <breakKeyLocal/>
       <breakKeyOn/>
       <freeze/>
       <yStart>7</yStart>
       <yEnd>7</yEnd>
       <stPtAdjOn/>
       <enPtAdjOn/>
       <insetRatio1>410</insetRatio1>
       <height1>46</height1>
       <insetRatio2>410</insetRatio2>
       <height2>49</height2>
       <ctlPtAdjOn/>
     </tieAlterStart>
   </details>
 </finale>
 )xml";
 
 TEST(TieAlterTest, PopulateFields)
 {
     auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
     auto details = doc->getDetails();
     ASSERT_TRUE(details);
 
     auto tieAlter = details->get<details::TieAlterStart>(SCORE_PARTID, 6, 0);
     ASSERT_TRUE(tieAlter) << "TieAlterBase with entnum 6 not found";
 
     EXPECT_EQ(tieAlter->noteId, 1);
     EXPECT_EQ(tieAlter->xStart, Evpu(9));
     EXPECT_EQ(tieAlter->xEnd, Evpu(-9));
     EXPECT_EQ(tieAlter->yStart, Evpu(7));
     EXPECT_EQ(tieAlter->yEnd, Evpu(7));
 
     EXPECT_TRUE(tieAlter->outerLocal);
     EXPECT_TRUE(tieAlter->afterSingleDotLocal);
     EXPECT_TRUE(tieAlter->afterSingleDotOn);
     EXPECT_TRUE(tieAlter->afterMultiDotsLocal);
     EXPECT_TRUE(tieAlter->afterMultiDotsOn);
     EXPECT_TRUE(tieAlter->shiftForSecondsLocal);
     EXPECT_TRUE(tieAlter->shiftForSecondsOn);
     EXPECT_TRUE(tieAlter->beforeSingleAcciLocal);
     EXPECT_TRUE(tieAlter->beforeSingleAcciOn);
     EXPECT_TRUE(tieAlter->down);
     EXPECT_TRUE(tieAlter->breakTimeLocal);
     EXPECT_TRUE(tieAlter->breakTimeOn);
     EXPECT_TRUE(tieAlter->breakKeyLocal);
     EXPECT_TRUE(tieAlter->breakKeyOn);
     EXPECT_TRUE(tieAlter->freezeDirection);
     EXPECT_TRUE(tieAlter->stPtAdjOn);
     EXPECT_TRUE(tieAlter->enPtAdjOn);
     EXPECT_TRUE(tieAlter->ctlPtAdjOn);
 
     EXPECT_EQ(tieAlter->insetRatio1, Evpu(410));
     EXPECT_EQ(tieAlter->height1, Evpu(46));
     EXPECT_EQ(tieAlter->insetRatio2, Evpu(410));
     EXPECT_EQ(tieAlter->height2, Evpu(49));
 }
 