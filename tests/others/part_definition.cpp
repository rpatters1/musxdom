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

constexpr static musxtest::string_view partDefXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <textOptions>
      <textLineSpacingPercent>100</textLineSpacingPercent>
      <tabSpaces>4</tabSpaces>
      <insertSymbolInfo type="sharp">
        <trackingBefore>35</trackingBefore>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontSize>110</fontSize>
        </symFont>
        <symChar>35</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="flat">
        <trackingBefore>60</trackingBefore>
        <baselineShiftPerc>19</baselineShiftPerc>
        <symFont>
          <fontSize>110</fontSize>
        </symFont>
        <symChar>98</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="natural">
        <trackingBefore>50</trackingBefore>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontSize>110</fontSize>
        </symFont>
        <symChar>110</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="dblSharp">
        <trackingBefore>40</trackingBefore>
        <baselineShiftPerc>34</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>220</symChar>
      </insertSymbolInfo>
      <insertSymbolInfo type="dblFlat">
        <trackingBefore>60</trackingBefore>
        <baselineShiftPerc>19</baselineShiftPerc>
        <symFont>
          <fontSize>100</fontSize>
        </symFont>
        <symChar>186</symChar>
      </insertSymbolInfo>
      <textWordWrap/>
      <textExpandSingleWord/>
    </textOptions>
  </options>
  <others>
    <measNumbRegion cmper="1">
      <scoreData>
        <startFont>
          <fontID>1</fontID>
          <fontSize>10</fontSize>
          <efx>
            <italic/>
            <absolute/>
          </efx>
        </startFont>
        <multipleFont>
          <fontID>2</fontID>
          <fontSize>12</fontSize>
        </multipleFont>
        <mmRestFont>
          <fontID>3</fontID>
          <fontSize>14</fontSize>
        </mmRestFont>
        <startEnclosure>
          <xMargin>9</xMargin>
          <yMargin>9</yMargin>
          <lineWidth>256</lineWidth>
          <sides>1</sides>
          <notTall/>
          <roundCorners/>
          <cornerRadius>768</cornerRadius>
        </startEnclosure>
        <multipleEnclosure>
          <xMargin>44</xMargin>
          <yMargin>44</yMargin>
          <lineWidth>256</lineWidth>
          <sides>2</sides>
          <notTall/>
          <roundCorners/>
          <cornerRadius>768</cornerRadius>
        </multipleEnclosure>
        <startXdisp>1</startXdisp>
        <startYdisp>-144</startYdisp>
        <multipleXdisp>3</multipleXdisp>
        <multipleYdisp>-144</multipleYdisp>
        <mmRestXdisp>5</mmRestXdisp>
        <mmRestYdisp>-144</mmRestYdisp>
        <leftMmBracketChar>91</leftMmBracketChar>
        <rightMmBracketChar>93</rightMmBracketChar>
        <startWith>2</startWith>
        <incidence>1</incidence>
        <multipleAlign>center</multipleAlign>
        <mmRestAlign>center</mmRestAlign>
        <startOfLine/>
        <multipleOf/>
        <exceptFirstMeas/>
        <mmRestRange/>
        <mmRestRangeForce/>
        <useStartEncl/>
        <useMultipleEncl/>
        <showOnTop/>
        <showOnBottom/>
        <excludeOthers/>
        <breakMmRest/>
        <multipleJustify>center</multipleJustify>
        <mmRestJustify>center</mmRestJustify>
      </scoreData>
      <partData/>
      <startMeas>1</startMeas>
      <endMeas>1000</endMeas>
      <startChar>164</startChar>
      <base>10</base>
      <offset>2</offset>
      <prefix>&lt;|</prefix>
      <suffix>&gt;|</suffix>
      <countFromOne/>
      <noZero/>
      <doubleUp/>
      <includeHours/>
      <smpteFrames/>
      <useScoreInfoForPart/>
      <region>1</region>
      <timePrecision>thousandths</timePrecision>
      <hideScroll/>
    </measNumbRegion>
    <partDef cmper="1">
      <nameID>42</nameID>
      <partOrder>3</partOrder>
      <copies>2</copies>
      <extractPart/>
      <needsRecalc/>
      <useAsSmpInst/>
      <smartMusicInst>-1</smartMusicInst>
    </partDef>
    <partGlobals cmper="65534">
      <showTransposed/>
      <studioViewIUlist>65400</studioViewIUlist>
    </partGlobals>
    <partGlobals cmper="65534" part="1" shared="false">
      <showTransposed/>
      <studioViewIUlist>65400</studioViewIUlist>
    </partGlobals>
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
    <blockText number="33">^fontid(1)^size(14)^nfx(2)Alto Sax in E^flat()^partname()</blockText>
  </texts>
</finale>
    )xml";

TEST(PartDefinitionTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(partDefXml);
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
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(partDefXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    constexpr static Cmper kAltoSaxPartId = 1;

    auto partDef = others->get<others::PartDefinition>(SCORE_PARTID, kAltoSaxPartId);
    ASSERT_TRUE(partDef) << "PartDefinition with cmper 1 not found";

    EXPECT_EQ(partDef->getName(), "Alto Sax in Eb");

    std::string parsedName;
    int iterationCount = 0;
    bool result = musx::util::EnigmaString::parseEnigmaText(doc, kAltoSaxPartId, "^fontid(2)^size(10)^nfx(2)This is ^baseline(12)part ^partname().^nfx(0)",
        [&](const std::string& chunk, const musx::util::EnigmaStyles& styles) {
            if (iterationCount == 3) {
                EXPECT_EQ(styles.font->fontId, 0);
            } else {
                EXPECT_EQ(styles.font->fontId, 2);
            }
            parsedName += chunk;
            iterationCount++;
            return true;
        });
    EXPECT_EQ(parsedName, "This is part Alto Sax in Eb.");
    EXPECT_EQ(iterationCount, 7) << "4 iterations for the string and 3 for the partname insert";
    EXPECT_TRUE(result);
}

TEST(PartDefinitionTest, GetArrayForScore)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(partDefXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    static constexpr Cmper kPartId = 1;
    auto measNums = others->getArray<others::MeasureNumberRegion>(kPartId);
    ASSERT_EQ(measNums.size(), 1) << "getArray should return the score's list, since meas numbs always linked";
    EXPECT_EQ(measNums[0]->getSourcePartId(), SCORE_PARTID);
    EXPECT_EQ(measNums[0]->getRequestedPartId(), kPartId);
    EXPECT_EQ(measNums.getRequestedPartId(), kPartId);
}

TEST(PartDefinitionTest, MissingPartGlobals)
{
    constexpr static musxtest::string_view missingPartGlobalXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <partDef cmper="0">
      <partOrder>0</partOrder>
      <copies>1</copies>
      <printPart/>
      <needsRecalc/>
    </partDef>
  </others>
</finale>
)xml";

    EXPECT_THROW(
        musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(missingPartGlobalXml),
        musx::dom::integrity_error
    );
}
