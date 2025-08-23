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

constexpr static musxtest::string_view noteRestOptionsXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <noteRestOptions>
      <doShapeNotes/>
      <doCrossOver/>
      <drop8thRest>1</drop8thRest>
      <drop16thRest>3</drop16thRest>
      <drop32ndRest>5</drop32ndRest>
      <drop64thRest>-24</drop64thRest>
      <drop128thRest>-48</drop128thRest>
      <scaleManualPositioning/>
      <drawOutline/>
      <noteColor id="0">
        <red>57856</red>
        <green>7168</green>
        <blue>18432</blue>
      </noteColor>
      <noteColor id="1">
        <red>62208</red>
        <green>28672</green>
        <blue>8448</blue>
      </noteColor>
      <noteColor id="2">
        <red>63744</red>
        <green>40192</green>
        <blue>7168</blue>
      </noteColor>
      <noteColor id="3">
        <red>65280</red>
        <green>52224</green>
        <blue>12800</blue>
      </noteColor>
      <noteColor id="4">
        <red>65280</red>
        <green>62208</green>
        <blue>11008</blue>
      </noteColor>
      <noteColor id="5">
        <red>54528</red>
        <green>57600</green>
        <blue>23296</blue>
      </noteColor>
      <noteColor id="6">
        <red>25088</red>
        <green>47872</green>
        <blue>17920</blue>
      </noteColor>
      <noteColor id="7">
        <green>39936</green>
        <blue>38144</blue>
      </noteColor>
      <noteColor id="8">
        <green>30464</green>
        <blue>49152</blue>
      </noteColor>
      <noteColor id="9">
        <red>23296</red>
        <green>22272</green>
        <blue>42496</blue>
      </noteColor>
      <noteColor id="10">
        <red>38400</red>
        <green>25856</green>
        <blue>43776</blue>
      </noteColor>
      <noteColor id="11">
        <red>60928</red>
        <green>19712</green>
        <blue>39680</blue>
      </noteColor>
    </noteRestOptions>
  </options>
</finale>
)xml";

TEST(NoteRestOptionsTest, PropertiesTest)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(noteRestOptionsXml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto n = options->get<options::NoteRestOptions>();
    ASSERT_TRUE(n);

    // Flags
    EXPECT_TRUE(n->doShapeNotes);
    EXPECT_TRUE(n->doCrossStaffNotes);         // <doCrossOver/> present; mapped directly
    EXPECT_TRUE(n->scaleManualPositioning);
    EXPECT_TRUE(n->drawOutline);

    // Rest drops (Evpu)
    EXPECT_EQ(n->drop8thRest, 1);
    EXPECT_EQ(n->drop16thRest, 3);
    EXPECT_EQ(n->drop32ndRest, 5);
    EXPECT_EQ(n->drop64thRest, -24);
    EXPECT_EQ(n->drop128thRest, -48);

    // Colors vector integrity
    ASSERT_EQ(n->noteColors.size(), music_theory::STANDARD_12EDO_STEPS);

    // Fail fast if any slot is null before we dereference anything.
    for (size_t i = 0; i < music_theory::STANDARD_12EDO_STEPS; ++i) {
        ASSERT_TRUE(n->noteColors[i]) << "noteColors[" << i << "] is null";
    }

    // Safe getter (no assertions inside a non-void lambda)
    auto c = [&](size_t i) { return n->noteColors[i]; };

    EXPECT_EQ(c(0)->red, 57856);   EXPECT_EQ(c(0)->green, 7168);   EXPECT_EQ(c(0)->blue, 18432);
    EXPECT_EQ(c(1)->red, 62208);   EXPECT_EQ(c(1)->green, 28672);  EXPECT_EQ(c(1)->blue, 8448);
    EXPECT_EQ(c(2)->red, 63744);   EXPECT_EQ(c(2)->green, 40192);  EXPECT_EQ(c(2)->blue, 7168);
    EXPECT_EQ(c(3)->red, 65280);   EXPECT_EQ(c(3)->green, 52224);  EXPECT_EQ(c(3)->blue, 12800);
    EXPECT_EQ(c(4)->red, 65280);   EXPECT_EQ(c(4)->green, 62208);  EXPECT_EQ(c(4)->blue, 11008);
    EXPECT_EQ(c(5)->red, 54528);   EXPECT_EQ(c(5)->green, 57600);  EXPECT_EQ(c(5)->blue, 23296);
    EXPECT_EQ(c(6)->red, 25088);   EXPECT_EQ(c(6)->green, 47872);  EXPECT_EQ(c(6)->blue, 17920);

    // id=7 has no <red>, should default to 0
    EXPECT_EQ(c(7)->red, 0);       EXPECT_EQ(c(7)->green, 39936);  EXPECT_EQ(c(7)->blue, 38144);

    // id=8 has no <red>, should default to 0
    EXPECT_EQ(c(8)->red, 0);       EXPECT_EQ(c(8)->green, 30464);  EXPECT_EQ(c(8)->blue, 49152);

    EXPECT_EQ(c(9)->red, 23296);   EXPECT_EQ(c(9)->green, 22272);  EXPECT_EQ(c(9)->blue, 42496);
    EXPECT_EQ(c(10)->red, 38400);  EXPECT_EQ(c(10)->green, 25856); EXPECT_EQ(c(10)->blue, 43776);
    EXPECT_EQ(c(11)->red, 60928);  EXPECT_EQ(c(11)->green, 19712); EXPECT_EQ(c(11)->blue, 39680);
}
