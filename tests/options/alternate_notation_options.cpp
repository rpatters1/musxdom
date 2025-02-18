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

TEST(AlternateNotationOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <alternateNotationOptions>
      <halfSlashLift>4</halfSlashLift>
      <wholeSlashLift>6</wholeSlashLift>
      <dWholeSlashLift>7</dWholeSlashLift>
      <halfSlashStemLift>5</halfSlashStemLift>
      <quartSlashStemLift>3</quartSlashStemLift>
      <quartSlashLift>2</quartSlashLift>
      <twoMeasNumLift>1</twoMeasNumLift>
    </alternateNotationOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto alternateNotationOptions = options->get<musx::dom::options::AlternateNotationOptions>();
    ASSERT_TRUE(alternateNotationOptions);

    // Test all properties of AlternateNotationOptions
    EXPECT_EQ(alternateNotationOptions->halfSlashLift, 4);
    EXPECT_EQ(alternateNotationOptions->wholeSlashLift, 6);
    EXPECT_EQ(alternateNotationOptions->dWholeSlashLift, 7);
    EXPECT_EQ(alternateNotationOptions->halfSlashStemLift, 5);
    EXPECT_EQ(alternateNotationOptions->quartSlashStemLift, 3);
    EXPECT_EQ(alternateNotationOptions->quartSlashLift, 2);
    EXPECT_EQ(alternateNotationOptions->twoMeasNumLift, 1);
}
