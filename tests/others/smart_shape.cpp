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
                    <breakAdj>
                        <x>-124</x>
                        <y>12</y>
                        <on/>
                    </breakAdj>
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
                    <breakAdj>
                        <x>256</x>
                        <y>-72</y>
                        <on/>
                    </breakAdj>
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
        EXPECT_TRUE(smartShape->startTermSeg->endPointAdj->active);

        ASSERT_TRUE(smartShape->endTermSeg);
        ASSERT_TRUE(smartShape->endTermSeg->endPoint);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->staffId, 1);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->measId, 1);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->entryNumber, 6);

        ASSERT_TRUE(smartShape->endTermSeg->endPointAdj);
        EXPECT_EQ(smartShape->endTermSeg->endPointAdj->horzOffset, -1);
        EXPECT_EQ(smartShape->endTermSeg->endPointAdj->vertOffset, 37);
        EXPECT_TRUE(smartShape->endTermSeg->endPointAdj->active);

        ASSERT_TRUE(smartShape->startTermSeg->breakAdj);
        EXPECT_EQ(smartShape->startTermSeg->breakAdj->horzOffset, -124);
        EXPECT_EQ(smartShape->startTermSeg->breakAdj->vertOffset, 12);
        EXPECT_TRUE(smartShape->startTermSeg->breakAdj->active);

        ASSERT_TRUE(smartShape->endTermSeg->breakAdj);
        EXPECT_EQ(smartShape->endTermSeg->breakAdj->horzOffset, 256);
        EXPECT_EQ(smartShape->endTermSeg->breakAdj->vertOffset, -72);
        EXPECT_TRUE(smartShape->endTermSeg->breakAdj->active);
    }
    {
        auto smartShape = others->get<others::SmartShape>(SCORE_PARTID, 2);
        ASSERT_TRUE(smartShape) << "SmartShape with cmper 2 not found";

        EXPECT_EQ(smartShape->shapeType, others::SmartShape::ShapeType::Glissando);
        EXPECT_TRUE(smartShape->entryBased);
        EXPECT_EQ(smartShape->startNoteId, 1);
        EXPECT_EQ(smartShape->endNoteId, 2);
        EXPECT_EQ(smartShape->lineStyleId, 3);

        ASSERT_TRUE(smartShape->startTermSeg->breakAdj);
        EXPECT_FALSE(smartShape->startTermSeg->breakAdj->active);
        ASSERT_TRUE(smartShape->endTermSeg->breakAdj);
        EXPECT_FALSE(smartShape->endTermSeg->breakAdj->active);
    }
    {
        auto smartShape = others->get<others::SmartShape>(SCORE_PARTID, 3);
        ASSERT_TRUE(smartShape) << "SmartShape with cmper 3 not found";

        EXPECT_EQ(smartShape->shapeType, others::SmartShape::ShapeType::Crescendo);
        EXPECT_FALSE(smartShape->entryBased);
        EXPECT_EQ(smartShape->startTermSeg->endPoint->eduPosition, 0);
        EXPECT_EQ(smartShape->endTermSeg->endPoint->eduPosition, 2048);

        ASSERT_TRUE(smartShape->startTermSeg->breakAdj);
        ASSERT_TRUE(smartShape->endTermSeg->breakAdj);
    }
}

TEST(CenterShape, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <centerShape cmper1="1" cmper2="7">
          <startBreakAdj>
            <x>37</x>
            <y>-199</y>
            <on/>
          </startBreakAdj>
          <endBreakAdj>
            <x>-68</x>
            <y>-199</y>
            <on/>
          </endBreakAdj>
          <ctlPtAdj>
            <startCtlPtY>136</startCtlPtY>
            <on/>
          </ctlPtAdj>
        </centerShape>
      </details>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto centerShape = details->get<details::CenterShape>(SCORE_PARTID, 1, 7);
    ASSERT_TRUE(centerShape) << "CenterShape with cmper1=1, cmper2=7 not found";

    ASSERT_TRUE(centerShape->startBreakAdj);
    EXPECT_EQ(centerShape->startBreakAdj->horzOffset, 37);
    EXPECT_EQ(centerShape->startBreakAdj->vertOffset, -199);
    EXPECT_TRUE(centerShape->startBreakAdj->active);

    ASSERT_TRUE(centerShape->endBreakAdj);
    EXPECT_EQ(centerShape->endBreakAdj->horzOffset, -68);
    EXPECT_EQ(centerShape->endBreakAdj->vertOffset, -199);
    EXPECT_TRUE(centerShape->endBreakAdj->active);
}
