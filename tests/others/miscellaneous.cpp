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
#include <cmath>
#include <string>
#include <vector>

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

    // Each value is the shortest decimal that converts back to ratio 16777. The exact quotients are
    // 14.9998... at a quarter note and 3.7499... at a whole note, so the whole note reports 3.75
    // rather than the 4 that rounding to an integer would give.
    EXPECT_DOUBLE_EQ(tempo1->getAbsoluteTempo(NoteType::Whole), 3.75);
    EXPECT_DOUBLE_EQ(tempo1->getAbsoluteTempo(), 15.0);
    EXPECT_DOUBLE_EQ(tempo1->getAbsoluteTempo(NoteType::Eighth), 30.0);
    EXPECT_DOUBLE_EQ(tempo1->getAbsoluteTempo(NoteType::Note32nd), 120.0);
}

TEST(Miscellaneous, TempoChangeReportsARoundTempoAsRound)
{
    // 55924 is the closest ratio to a quarter-note tempo of 50 at unit 1000, and 174483 to 156.
    // The exact quotients are 49.99995... and 155.99995..., which is what the encoding can hold.
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
        <others>
        <tempoDef cmper="1" inci="0">
            <absoluteRatio>55924</absoluteRatio>
            <eldur>0</eldur>
            <unit>1000</unit>
        </tempoDef>
        <tempoDef cmper="1" inci="1">
            <absoluteRatio>174483</absoluteRatio>
            <eldur>0</eldur>
            <unit>1000</unit>
        </tempoDef>
        <tempoDef cmper="1" inci="2">
            <absoluteRatio>59628</absoluteRatio>
            <eldur>0</eldur>
            <unit>1000</unit>
        </tempoDef>
        </others>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto slowest = others->get<others::TempoChange>(SCORE_PARTID, 1, 0);
    ASSERT_TRUE(slowest);
    EXPECT_DOUBLE_EQ(slowest->getAbsoluteTempo(), 50.0);

    auto fastest = others->get<others::TempoChange>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(fastest);
    EXPECT_DOUBLE_EQ(fastest->getAbsoluteTempo(), 156.0);

    // A step between them is genuinely fractional, and keeps the digits the ratio distinguishes.
    auto between = others->get<others::TempoChange>(SCORE_PARTID, 1, 2);
    ASSERT_TRUE(between);
    EXPECT_DOUBLE_EQ(between->getAbsoluteTempo(), 53.312);
}

TEST(Miscellaneous, TempoChangeAlwaysConvertsBackToItsRatio)
{
    constexpr int unit = 1000;
    std::vector<int> ratios;
    for (int ratio = 4096; ratio <= 262144; ratio += 1531) {
        ratios.push_back(ratio);
    }

    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<finale>\n<others>\n";
    for (size_t i = 0; i < ratios.size(); i++) {
        xml += "<tempoDef cmper=\"1\" inci=\"" + std::to_string(i) + "\">"
               "<absoluteRatio>" + std::to_string(ratios[i]) + "</absoluteRatio>"
               "<eldur>0</eldur><unit>" + std::to_string(unit) + "</unit></tempoDef>\n";
    }
    xml += "</others>\n</finale>\n";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // The reported tempo must name the ratio it came from. This is the property the shortest-decimal
    // search rests on: a value that converted back to some other ratio would misreport the document.
    const std::vector<NoteType> noteTypes = {
        NoteType::Whole, NoteType::Half, NoteType::Quarter, NoteType::Eighth, NoteType::Note32nd
    };
    for (size_t i = 0; i < ratios.size(); i++) {
        auto tempoChange = others->get<others::TempoChange>(SCORE_PARTID, 1, Inci(i));
        ASSERT_TRUE(tempoChange) << "ratio " << ratios[i];
        for (const auto noteType : noteTypes) {
            const double tempo = tempoChange->getAbsoluteTempo(noteType);
            const long long encoded = std::llround((tempo * 65536.0 * double(noteType)) / (double(unit) * 60.0));
            EXPECT_EQ(encoded, ratios[i]) << "ratio " << ratios[i] << " at note type " << int(noteType);
        }
    }
}
