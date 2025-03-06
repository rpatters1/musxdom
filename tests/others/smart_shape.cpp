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
 
TEST(SmartShape, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
            <smartShape cmper="1">
                <shapeType>slurAuto</shapeType>
                <entryBased/>
                <rotate/>
                <yBreakType>opposite</yBreakType>
                <startTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                        <entryNum>5</entryNum>
                    </endPt>
                    <endPtAdj>
                        <y>24</y>
                        <on/>
                        <contextDir>over</contextDir>
                        <contextEntCnct>headRightTop</contextEntCnct>
                    </endPtAdj>
                </startTermSeg>
                <endTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                        <entryNum>6</entryNum>
                    </endPt>
                    <endPtAdj>
                        <x>-1</x>
                        <y>37</y>
                        <on/>
                        <contextDir>over</contextDir>
                        <contextEntCnct>headRightTop</contextEntCnct>
                    </endPtAdj>
                </endTermSeg>
                <fullCtlPtAdj>
                    <startCtlPtX>573</startCtlPtX>
                    <startCtlPtY>35</startCtlPtY>
                    <endCtlPtX>-559</endCtlPtX>
                    <endCtlPtY>64</endCtlPtY>
                    <on/>
                    <contextDir>over</contextDir>
                </fullCtlPtAdj>
            </smartShape>
            <smartShape cmper="2">
                <shapeType>glissando</shapeType>
                <entryBased/>
                <yBreakType>opposite</yBreakType>
                <startTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                        <entryNum>5</entryNum>
                    </endPt>
                    <endPtAdj>
                        <x>27</x>
                        <y>14</y>
                        <on/>
                        <contextEntCnct>noteRightCenter</contextEntCnct>
                    </endPtAdj>
                </startTermSeg>
                <endTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                        <entryNum>6</entryNum>
                    </endPt>
                    <endPtAdj>
                        <x>-25</x>
                        <y>9</y>
                        <on/>
                        <contextEntCnct>noteLeftCenter</contextEntCnct>
                    </endPtAdj>
                </endTermSeg>
                <startNoteID>1</startNoteID>
                <endNoteID>2</endNoteID>
                <lineStyleID>3</lineStyleID>
            </smartShape>
            <smartShape cmper="3">
                <shapeType>cresc</shapeType>
                <startTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                    </endPt>
                    <endPtAdj>
                        <x>24</x>
                        <y>-185</y>
                        <on/>
                    </endPtAdj>
                </startTermSeg>
                <endTermSeg>
                    <endPt>
                        <inst>1</inst>
                        <meas>1</meas>
                        <edu>2048</edu>
                    </endPt>
                    <endPtAdj>
                        <x>-22</x>
                        <y>-131</y>
                        <on/>
                    </endPtAdj>
                </endTermSeg>
                <fullCtlPtAdj>
                    <startCtlPtY>40</startCtlPtY>
                    <on/>
                </fullCtlPtAdj>
            </smartShape>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto smartShape = others->get<others::SmartShape>(SCORE_PARTID, 1);
        ASSERT_TRUE(smartShape) << "SmartShape with cmper 1 not found";

        EXPECT_EQ(smartShape->shapeType, others::SmartShape::ShapeType::SlurAuto);
        EXPECT_TRUE(smartShape->entryBased);
        EXPECT_EQ(smartShape->startNoteId, 0);
        EXPECT_EQ(smartShape->endNoteId, 0);
        EXPECT_EQ(smartShape->lineStyleId, 0);

        ASSERT_TRUE(smartShape->startTermSeg);
        ASSERT_TRUE(smartShape->startTermSeg->endPoint);
        EXPECT_EQ(smartShape->startTermSeg->endPoint->staffId, 1);
        EXPECT_EQ(smartShape->startTermSeg->endPoint->measId, 1);
        EXPECT_EQ(smartShape->startTermSeg->endPoint->entryNumber, 5);

        ASSERT_TRUE(smartShape->startTermSeg->endPointAdj);
        EXPECT_EQ(smartShape->startTermSeg->endPointAdj->vertOffset, 24);

        ASSERT_TRUE(smartShape->endTermSeg);
        ASSERT_TRUE(smartShape->endTermSeg->endPoint);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->staffId, 1);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->measId, 1);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->entryNumber, 6);

        ASSERT_TRUE(smartShape->endTermSeg->endPointAdj);
        EXPECT_EQ(smartShape->endTermSeg->endPointAdj->horzOffset, -1);
        EXPECT_EQ(smartShape->endTermSeg->endPointAdj->vertOffset, 37);
    }
    {
        auto smartShape = others->get<others::SmartShape>(SCORE_PARTID, 2);
        ASSERT_TRUE(smartShape) << "SmartShape with cmper 2 not found";

        EXPECT_EQ(smartShape->shapeType, others::SmartShape::ShapeType::Glissando);
        EXPECT_TRUE(smartShape->entryBased);
        EXPECT_EQ(smartShape->startNoteId, 1);
        EXPECT_EQ(smartShape->endNoteId, 2);
        EXPECT_EQ(smartShape->lineStyleId, 3);
    }
}
