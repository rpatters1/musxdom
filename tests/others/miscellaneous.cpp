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

TEST(Miscellaneous, TempoChange)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
        <tempoDef cmper="1" inci="0">
            <relativeRatio>750</relativeRatio>
            <eldur>512</eldur>
            <unit>1000</unit>
        </tempoDef>
        <tempoDef cmper="1" inci="1">
            <absoluteRatio>16777</absoluteRatio>
            <eldur>1999</eldur>
            <unit>1000</unit>
        </tempoDef>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto tempo0 = others->get<others::TempoChange>(SCORE_PARTID, 1, 0);
    ASSERT_TRUE(tempo0) << "TempoChange for measure 1, inci 0 not found";
    EXPECT_EQ(tempo0->ratio, 750);
    EXPECT_TRUE(tempo0->isRelative);
    EXPECT_EQ(tempo0->eduPosition, 512);
    EXPECT_EQ(tempo0->unit, 1000);

    EXPECT_THROW(
        tempo0->getAbsoluteTempo(),
        std::logic_error
    );

    auto tempo1 = others->get<others::TempoChange>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(tempo1) << "TempoChange for measure 1, inci 1 not found";
    EXPECT_EQ(tempo1->ratio, 16777);
    EXPECT_FALSE(tempo1->isRelative);
    EXPECT_EQ(tempo1->eduPosition, 1999);
    EXPECT_EQ(tempo1->unit, 1000);

    EXPECT_EQ(tempo1->getAbsoluteTempo(NoteType::Whole), 4);
    EXPECT_EQ(tempo1->getAbsoluteTempo(), 15);
    EXPECT_EQ(tempo1->getAbsoluteTempo(NoteType::Eighth), 30);
    EXPECT_EQ(tempo1->getAbsoluteTempo(NoteType::Note32nd), 120);
}
