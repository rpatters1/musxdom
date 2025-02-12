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

TEST(TimeSignatureOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <timeSignatureOptions>
      <timeUpperLift>3</timeUpperLift>
      <timeFront>24</timeFront>
      <timeBack>7</timeBack>
      <timeFrontParts>24</timeFrontParts>
      <timeBackParts>12</timeBackParts>
      <timeUpperLiftParts>4</timeUpperLiftParts>
      <timeLowerLiftParts>8</timeLowerLiftParts>
      <timeAbrvLiftParts>2</timeAbrvLiftParts>
      <timeSigDoAbrvCommon/>
      <timeSigDoAbrvCut/>
      <defFloat>6</defFloat>
      <cautionaryTimeChanges/>
      <timeLowerLift>5</timeLowerLift>
      <timeAbrvLift>1</timeAbrvLift>
    </timeSignatureOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto timeSignatureOptions = options->get<musx::dom::options::TimeSignatureOptions>();
    ASSERT_TRUE(timeSignatureOptions);

    // Test all properties of TimeSignatureOptions
    EXPECT_EQ(timeSignatureOptions->timeUpperLift, 3);
    EXPECT_EQ(timeSignatureOptions->timeFront, 24);
    EXPECT_EQ(timeSignatureOptions->timeBack, 7);
    EXPECT_EQ(timeSignatureOptions->timeFrontParts, 24);
    EXPECT_EQ(timeSignatureOptions->timeBackParts, 12);
    EXPECT_EQ(timeSignatureOptions->timeUpperLiftParts, 4);
    EXPECT_EQ(timeSignatureOptions->timeLowerLiftParts, 8);
    EXPECT_EQ(timeSignatureOptions->timeAbrvLiftParts, 2);
    EXPECT_TRUE(timeSignatureOptions->timeSigDoAbrvCommon);
    EXPECT_TRUE(timeSignatureOptions->timeSigDoAbrvCut);
    EXPECT_EQ(timeSignatureOptions->numCompositeDecimalPlaces, 6);
    EXPECT_TRUE(timeSignatureOptions->cautionaryTimeChanges);
    EXPECT_EQ(timeSignatureOptions->timeLowerLift, 5);
    EXPECT_EQ(timeSignatureOptions->timeAbrvLift, 1);
}
