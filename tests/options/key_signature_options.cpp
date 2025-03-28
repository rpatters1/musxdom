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

TEST(KeySignatureOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <keySignatureOptions>
      <doKeyCancel/>
      <doCStart/>
      <doBankDiff/>
      <keyFront>24</keyFront>
      <keyMid>2</keyMid>
      <keyBack>1</keyBack>
      <acciAdd>3</acciAdd>
      <showKeyFirstSystemOnly/>
      <keyTime>12</keyTime>
      <simplifyKeyHoldOctave/>
      <cautionaryKeyChanges/>
      <doKeyCancelBetweenSharpsFlats/>
    </keySignatureOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto keySignatureOptions = options->get<musx::dom::options::KeySignatureOptions>();
    ASSERT_TRUE(keySignatureOptions);

    // Test all properties of KeySignatureOptions
    EXPECT_TRUE(keySignatureOptions->doKeyCancel);
    EXPECT_TRUE(keySignatureOptions->doCStart);
    EXPECT_TRUE(keySignatureOptions->redisplayOnModeChange);
    EXPECT_EQ(keySignatureOptions->keyFront, 24);
    EXPECT_EQ(keySignatureOptions->keyMid, 2);
    EXPECT_EQ(keySignatureOptions->keyBack, 1);
    EXPECT_EQ(keySignatureOptions->acciAdd, 3);
    EXPECT_TRUE(keySignatureOptions->showKeyFirstSystemOnly);
    EXPECT_EQ(keySignatureOptions->keyTimeSepar, 12);
    EXPECT_TRUE(keySignatureOptions->simplifyKeyHoldOctave);
    EXPECT_TRUE(keySignatureOptions->cautionaryKeyChanges);
    EXPECT_TRUE(keySignatureOptions->doKeyCancelBetweenSharpsFlats);
}
