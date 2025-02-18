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
    <frameSpec cmper="1" inci="0">
      <startTime>512</startTime>
    </frameSpec>
    <frameSpec cmper="1" inci="1">
      <startEntry>1</startEntry>
      <endEntry>2</endEntry>
    </frameSpec>
    <frameSpec cmper="2">
      <startEntry>2140</startEntry>
      <endEntry>2142</endEntry>
    </frameSpec>
    <frameSpec cmper="3" inci="0">
      <startEntry>2144</startEntry>
      <endEntry>2146</endEntry>
    </frameSpec>
  </others>
</finale>
    )xml";

TEST(FrameTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    
    auto frame = others->get<others::Frame>(SCORE_PARTID, 1, 0);
    ASSERT_TRUE(frame) << "Frame with cmper 1 inci 0 not found";

    EXPECT_EQ(frame->startTime, 512);

    frame = others->get<others::Frame>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(frame) << "Frame with cmper 1 inci 1 not found";

    EXPECT_EQ(frame->startEntry, 1);
    EXPECT_EQ(frame->endEntry, 2);

    frame = others->get<others::Frame>(SCORE_PARTID, 2, 0);
    ASSERT_TRUE(frame) << "Frame with cmper 2 not found";

    EXPECT_EQ(frame->startEntry, 2140);
    EXPECT_EQ(frame->endEntry, 2142);

    frame = others->get<others::Frame>(SCORE_PARTID, 3, 0);
    ASSERT_TRUE(frame) << "Frame with cmper 3 not found";

    EXPECT_EQ(frame->startEntry, 2144);
    EXPECT_EQ(frame->endEntry, 2146);
}

TEST(FrameTest, IntegrityCheck)
{
    constexpr static musxtest::string_view xmlWrongInci = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <frameSpec cmper="1" inci="1">
      <startEntry>1</startEntry>
      <endEntry>2</endEntry>
    </frameSpec>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xmlWrongInci),
        musx::dom::integrity_error
    ) << "inci is out of sequence";

    constexpr static musxtest::string_view xmlStartTime = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <frameSpec cmper="1" inci="0">
      <startEntry>1</startEntry>
      <endEntry>2</endEntry>
      <startTime>512</startTime>
    </frameSpec>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xmlStartTime),
        musx::dom::integrity_error
    ) << "startTime supplied along with start and end entries";
        
    constexpr static musxtest::string_view xmlMissingStart = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <frameSpec cmper="1" inci="0">
      <endEntry>2</endEntry>
    </frameSpec>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xmlMissingStart),
        musx::dom::integrity_error
    ) << "missing start entry";
        
    constexpr static musxtest::string_view xmlMissingEnd = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <frameSpec cmper="1" inci="0">
      <startEntry>1</startEntry>
      <endEntry>0</endEntry>
    </frameSpec>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xmlMissingEnd),
        musx::dom::integrity_error
    ) << "end entry is zero";
}