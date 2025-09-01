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

constexpr static musxtest::string_view ossiaXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <ossiaBounds cmper="1">
      <topAdd>-12</topAdd>
      <leftAdd>5</leftAdd>
      <rightAdd>7</rightAdd>
      <centerPull>11</centerPull>
    </ossiaBounds>
    <ossiaBounds cmper="2">
      <topAdd>2</topAdd>
      <leftAdd>4</leftAdd>
      <rightAdd>6</rightAdd>
      <centerPull>8</centerPull>
    </ossiaBounds>
    <ossiaHeader cmper="1">
      <clef>5</clef>
      <keySig>
        <key>1</key>
      </keySig>
      <beats>1</beats>
      <divbeat>1</divbeat>
      <group>1</group>
      <altNumTsig/>
      <altDenTsig/>
      <barline>double</barline>
    </ossiaHeader>
    <ossiaHeader cmper="2">
      <clef>3</clef>
      <beats>4</beats>
      <divbeat>1024</divbeat>
      <group>2</group>
    </ossiaHeader>
    <ossiaMusic cmper="1">
      <percent>75</percent>
      <qinst>2</qinst>
      <qmeasure>1</qmeasure>
      <negLeftLine/>
      <negKey/>
      <negTime/>
      <negDynamic/>
      <negStaff/>
      <negRepeat/>
      <negClef/>
      <posMode>ossiaDefault</posMode>
    </ossiaMusic>
    <ossiaMusic cmper="2">
      <percent>81</percent>
      <qinst>2</qinst>
      <qmeasure>1</qmeasure>
      <posMode>ossiaDefault</posMode>
    </ossiaMusic>
    <timeLower cmper="1">
      <tldata>
        <integer>512</integer>
        <startGroup/>
      </tldata>
      <tldata>
        <integer>256</integer>
      </tldata>
    </timeLower>
    <timeUpper cmper="1">
      <tudata>
        <integer>2</integer>
        <startGroup/>
      </tudata>
      <tudata>
        <integer>3</integer>
      </tudata>
    </timeUpper>
  </others>
  <details>
    <brackSpec cmper1="1" cmper2="0" inci="0">
      <id>6</id>
      <bracPos>-12</bracPos>
      <bracTop>13</bracTop>
      <bracBot>-13</bracBot>
    </brackSpec>
    <brackSpec cmper1="2" cmper2="0" inci="0">
      <id>3</id>
      <bracPos>-24</bracPos>
      <bracTop>13</bracTop>
      <bracBot>-13</bracBot>
    </brackSpec>
    <brackSpec cmper1="2" cmper2="0" inci="1">
      <id>8</id>
      <bracPos>-12</bracPos>
      <bracTop>1</bracTop>
      <bracBot>-1</bracBot>
    </brackSpec>
  </details>
</finale>
)xml";

TEST(OssiaBoundsTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(ossiaXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto b1 = others->get<others::OssiaBounds>(SCORE_PARTID, 1);
    ASSERT_TRUE(b1) << "OssiaBounds with cmper 1 not found";
    EXPECT_EQ(b1->topMargin,   Evpu(-12));
    EXPECT_EQ(b1->leftMargin,  Evpu(5));
    EXPECT_EQ(b1->rightAdd,    Evpu(7));
    EXPECT_EQ(b1->centerPull,  Evpu(11));

    auto b2 = others->get<others::OssiaBounds>(SCORE_PARTID, 2);
    ASSERT_TRUE(b2) << "OssiaBounds with cmper 2 not found";
    EXPECT_EQ(b2->topMargin,   Evpu(2));
    EXPECT_EQ(b2->leftMargin,  Evpu(4));
    EXPECT_EQ(b2->rightAdd,    Evpu(6));
    EXPECT_EQ(b2->centerPull,  Evpu(8));
}

TEST(OssiaHeaderTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(ossiaXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto h1 = others->get<others::OssiaHeader>(SCORE_PARTID, 1);
    ASSERT_TRUE(h1) << "OssiaHeader with cmper 1 not found";
    EXPECT_EQ(h1->clefId, static_cast<ClefIndex>(5));
    ASSERT_TRUE(h1->keySig) << "keySig should be populated";
    EXPECT_EQ(h1->keySig->getAlteration(KeySignature::KeyContext::Concert), 1);
    EXPECT_EQ(h1->beats, 1);
    EXPECT_EQ(h1->divBeat, 1);
    EXPECT_EQ(h1->bracketGroup, 1);
    EXPECT_TRUE(h1->compositeNumerator);
    EXPECT_TRUE(h1->compositeDenominator);
    EXPECT_EQ(h1->barlineType, others::Measure::BarlineType::Double);

    auto h1Brackets = h1->getBrackets();
    ASSERT_EQ(h1Brackets.size(), 1);
    EXPECT_EQ(h1Brackets[0]->style, details::Bracket::BracketStyle::BracketCurvedHooks);

    auto h1TimeSig = h1->createTimeSignature();
    ASSERT_TRUE(h1TimeSig);
    ASSERT_EQ(h1TimeSig->components.size(), 1);
    EXPECT_EQ(h1TimeSig->components[0].counts.size(), 2);
    EXPECT_EQ(h1TimeSig->components[0].units.size(), 2);
    EXPECT_EQ(h1TimeSig->calcTotalDuration(), musx::util::Fraction(15, 16));

    auto h2 = others->get<others::OssiaHeader>(SCORE_PARTID, 2);
    ASSERT_TRUE(h2) << "OssiaHeader with cmper 2 not found";
    EXPECT_EQ(h2->clefId, static_cast<ClefIndex>(3));
    ASSERT_TRUE(h2->keySig) << "no <keySig> present, C Major populated anyway";
    EXPECT_EQ(h2->keySig->getAlteration(KeySignature::KeyContext::Concert), 0);
    EXPECT_EQ(h2->beats, 4);
    EXPECT_EQ(h2->divBeat, 1024);
    EXPECT_EQ(h2->bracketGroup, 2);
    EXPECT_FALSE(h2->compositeNumerator);
    EXPECT_FALSE(h2->compositeDenominator);
    EXPECT_EQ(h2->barlineType, others::Measure::BarlineType::None);

    auto h2Brackets = h2->getBrackets();
    ASSERT_EQ(h2Brackets.size(), 2);
    EXPECT_EQ(h2Brackets[0]->style, details::Bracket::BracketStyle::PianoBrace);
    EXPECT_EQ(h2Brackets[1]->style, details::Bracket::BracketStyle::DeskBracket);

    auto h2TimeSig = h2->createTimeSignature();
    ASSERT_TRUE(h2TimeSig);
    ASSERT_EQ(h2TimeSig->components.size(), 1);
    EXPECT_EQ(h2TimeSig->components[0].counts.size(), 1);
    EXPECT_EQ(h2TimeSig->components[0].units.size(), 1);
    EXPECT_EQ(h2TimeSig->calcTotalDuration(), 1);
}

TEST(OssiaMusicTest, PopulateFields)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(ossiaXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto m1 = others->get<others::OssiaMusic>(SCORE_PARTID, 1);
    ASSERT_TRUE(m1) << "OssiaMusic with cmper 1 not found";
    EXPECT_EQ(m1->percent, 75);
    EXPECT_EQ(m1->sourceStaffId, 2);
    EXPECT_EQ(m1->sourceMeasureId, 1);
    EXPECT_TRUE(m1->hideLeftLine);
    EXPECT_TRUE(m1->hideKey);
    EXPECT_TRUE(m1->hideTime);
    EXPECT_TRUE(m1->hideDynamic);
    EXPECT_TRUE(m1->hideStaff);
    EXPECT_TRUE(m1->hideRepeat);
    EXPECT_TRUE(m1->hideClef);

    auto m2 = others->get<others::OssiaMusic>(SCORE_PARTID, 2);
    ASSERT_TRUE(m2) << "OssiaMusic with cmper 2 not found";
    EXPECT_EQ(m2->percent, 81);
    EXPECT_EQ(m2->sourceStaffId, 2);
    EXPECT_EQ(m2->sourceMeasureId, 1);
    EXPECT_FALSE(m2->hideLeftLine);
    EXPECT_FALSE(m2->hideKey);
    EXPECT_FALSE(m2->hideTime);
    EXPECT_FALSE(m2->hideDynamic);
    EXPECT_FALSE(m2->hideStaff);
    EXPECT_FALSE(m2->hideRepeat);
    EXPECT_FALSE(m2->hideClef);
}

TEST(PageOssiaAssignTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <pageOssiaAssign cmper="1" inci="0">
      <arbnum>2</arbnum>
      <topAdd>-2202</topAdd>
      <leftAdd>401</leftAdd>
      <mwidth>216</mwidth>
      <hidden/>
    </pageOssiaAssign>
  </others>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto assign = others->get<others::PageOssiaAssign>(SCORE_PARTID, 1, 0);
    ASSERT_TRUE(assign) << "PageOssiaAssign cmper 1 inci 0 not found";

    EXPECT_EQ(assign->ossiaId, Cmper(2));
    EXPECT_EQ(assign->xOffset, Evpu(-2202));
    EXPECT_EQ(assign->yOffset, Evpu(401));
    EXPECT_EQ(assign->measureWidth, Evpu(216));
    EXPECT_TRUE(assign->hidden);
}

TEST(MeasureOssiaAssignTest, Populate)
{
    constexpr static musxtest::string_view xml = R"xml(
    <?xml version="1.0" encoding="UTF-8"?>
    <finale>
      <details>
        <measOssiaAssign cmper1="1" cmper2="2" inci="0">
          <arbnum>1</arbnum>
          <topAdd>145</topAdd>
          <leftAdd>124</leftAdd>
          <hidden/>
        </measOssiaAssign>
        <measOssiaAssign cmper1="2" cmper2="2" inci="0">
          <arbnum>3</arbnum>
          <topAdd>-241</topAdd>
          <leftAdd>331</leftAdd>
        </measOssiaAssign>
      </details>
    </finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Test case 1: staffId = 1, measureId = 2
    auto assign1 = details->get<details::MeasureOssiaAssign>(SCORE_PARTID, 1, 2, 0);
    ASSERT_TRUE(assign1) << "MeasureOssiaAssign with staff 1, meas 2, inci 0 not found";

    EXPECT_EQ(assign1->ossiaId, Cmper(1));
    EXPECT_EQ(assign1->xOffset, Evpu(145));
    EXPECT_EQ(assign1->yOffset, Evpu(124));
    EXPECT_TRUE(assign1->hidden);

    // Test case 2: staffId = 2, measureId = 2
    auto assign2 = details->get<details::MeasureOssiaAssign>(SCORE_PARTID, 2, 2, 0);
    ASSERT_TRUE(assign2) << "MeasureOssiaAssign with staff 2, meas 2, inci 0 not found";

    EXPECT_EQ(assign2->ossiaId, Cmper(3));
    EXPECT_EQ(assign2->xOffset, Evpu(-241));
    EXPECT_EQ(assign2->yOffset, Evpu(331));
    EXPECT_FALSE(assign2->hidden);
}
