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

constexpr static musxtest::string_view partVoicingXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <voicingDef cmper="1" part="1" shared="false">
      <enabled/>
      <voicingType>useMultipleLayers</voicingType>
      <singleVoiceType>botNote</singleVoiceType>
      <select1st/>
      <select3rd/>
      <selectFromBottom/>
      <selectSingleNote/>
      <singleLayer>1</singleLayer>
      <multiLayer>2</multiLayer>
    </voicingDef>
    <voicingDef cmper="1" part="2" shared="false">
      <enabled/>
      <singleVoiceType>selected</singleVoiceType>
      <select2nd/>
      <select4th/>
      <select5th/>
      <selectSingleNote/>
      <singleLayer>1</singleLayer>
      <multiLayer>1</multiLayer>
    </voicingDef>
  </others>
</finale>
)xml";

TEST(PartVoicingTest, PartVoicingPopulationTest)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(partVoicingXml);
    ASSERT_TRUE(doc);

    // cmper (staff) = 1 for both records in the XML above.
    // First record: part=1
    auto v1 = doc->getOthers()->get<others::PartVoicing>(1, 1);
    ASSERT_TRUE(v1);

    EXPECT_TRUE(v1->enabled);
    EXPECT_EQ(v1->voicingType, others::PartVoicing::VoicingType::UseMultipleLayers);
    EXPECT_EQ(v1->singleLayerVoiceType, others::PartVoicing::SingleLayerVoiceType::BottomNote);

    EXPECT_TRUE(v1->select1st);
    EXPECT_FALSE(v1->select2nd);
    EXPECT_TRUE(v1->select3rd);
    EXPECT_FALSE(v1->select4th);
    EXPECT_FALSE(v1->select5th);

    EXPECT_TRUE(v1->selectFromBottom);
    EXPECT_TRUE(v1->selectSingleNote);

    EXPECT_EQ(v1->singleLayer, 1);
    EXPECT_EQ(v1->multiLayer, 2);

    // Second record: part=2
    auto v2 = doc->getOthers()->get<others::PartVoicing>(2, 1);
    ASSERT_TRUE(v2);

    EXPECT_TRUE(v2->enabled);
    // voicingType not present in XML -> default (UseSingleLayer)
    EXPECT_EQ(v2->voicingType, others::PartVoicing::VoicingType::UseSingleLayer);
    EXPECT_EQ(v2->singleLayerVoiceType, others::PartVoicing::SingleLayerVoiceType::SelectedNotes);

    EXPECT_FALSE(v2->select1st);
    EXPECT_TRUE(v2->select2nd);
    EXPECT_FALSE(v2->select3rd);
    EXPECT_TRUE(v2->select4th);
    EXPECT_TRUE(v2->select5th);

    EXPECT_FALSE(v2->selectFromBottom); // not present -> default false
    EXPECT_TRUE(v2->selectSingleNote);

    EXPECT_EQ(v2->singleLayer, 1);
    EXPECT_EQ(v2->multiLayer, 1);

    // Nonexistent combination should return nothing
    auto v_missing = doc->getOthers()->get<others::PartVoicing>(3, 1);
    ASSERT_FALSE(v_missing);
}
