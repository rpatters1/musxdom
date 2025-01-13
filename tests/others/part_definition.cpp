/*
 * Copyright (C) 2024, Robert Patterson
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
    <partDef cmper="1">
      <nameID>42</nameID>
      <partOrder>3</partOrder>
      <copies>2</copies>
      <extractPart/>
      <needsRecalc/>
      <useAsSmpInst/>
      <smartMusicInst>-1</smartMusicInst>
    </partDef>
    <textBlock cmper="42">
      <textID>33</textID>
      <lineSpacingPercent>100</lineSpacingPercent>
      <newPos36/>
      <showShape/>
      <wordWrap/>
      <roundCorners/>
      <cornerRadius>512</cornerRadius>
      <textTag>block</textTag>
    </textBlock>
  </others>
  <texts>
    <blockText number="33">^fontid(1)^size(14)^nfx(2)Alto Sax in E^flat()</blockText>
  </texts>
</finale>
    )xml";

TEST(PartDefinitionTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    
    auto partDef = others->get<others::PartDefinition>(SCORE_PARTID, 1);
    ASSERT_TRUE(partDef) << "PartDefinition with cmper 1 not found";
    
    EXPECT_EQ(partDef->nameId, 42);
    EXPECT_EQ(partDef->partOrder, 3);
    EXPECT_EQ(partDef->copies, 2);
    EXPECT_TRUE(partDef->extractPart);
    EXPECT_TRUE(partDef->needsRecalc);
    EXPECT_TRUE(partDef->useAsSmpInst);
    EXPECT_EQ(partDef->smartMusicInst, -1);
}

TEST(PartDefinitionTest, GetName)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);
    
    auto partDef = others->get<others::PartDefinition>(SCORE_PARTID, 1);
    ASSERT_TRUE(partDef) << "PartDefinition with cmper 1 not found";

    EXPECT_EQ(partDef->getName(), "Alto Sax in Eb");
}
