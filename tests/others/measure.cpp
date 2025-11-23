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

constexpr static musxtest::string_view measureXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <measSpec cmper="1">
      <width>600</width>
      <keySig>
        <keyless/>
      </keySig>
      <beats>2</beats>
      <divbeat>2</divbeat>
      <dispBeats>4</dispBeats>
      <dispDivbeat>1024</dispDivbeat>
      <hasTextBlock/>
      <altNumTsig/>
      <altDenTsig/>
      <posMode>timesigPlusPos</posMode>
      <barline>normal</barline>
      <forRepBar/>
      <bacRepBar/>
      <barEnding/>
      <abbrvTime/>
      <hasChord/>
      <useDisplayTimesig/>
      <leftBarline>default</leftBarline>
    </measSpec>
    <measSpec cmper="1" part="1" shared="true">
      <width>420</width>
    </measSpec>
    <measSpec cmper="2">
      <width>600</width>
      <keySig>
        <key>4</key>
        <hideKeySigShowAccis/>
      </keySig>
      <beats>4</beats>
      <divbeat>1024</divbeat>
      <dispBeats>6</dispBeats>
      <dispDivbeat>6</dispDivbeat>
      <hasSmartShape/>
      <mnSepPlace/>
      <barline>normal</barline>
      <txtRepeats/>
      <useDisplayTimesig/>
      <displayAltNumTsig/>
      <displayAltDenTsig/>
    </measSpec>
    <measSpec cmper="2" part="1" shared="true">
      <width>368</width>
      <posMode>beatchartPlusPos</posMode>
    </measSpec>
    <measSpec cmper="3">
      <width>600</width>
      <beats>4</beats>
      <divbeat>4</divbeat>
      <dispBeats>4</dispBeats>
      <dispDivbeat>1024</dispDivbeat>
      <frontSpaceExtra>11</frontSpaceExtra>
      <backSpaceExtra>13</backSpaceExtra>
      <hideCaution/>
      <groupBarlineOverride/>
      <altNumTsig/>
      <altDenTsig/>
      <showKey>ignoreKey</showKey>
      <showTime>ignoreTime</showTime>
      <indivPosDef/>
      <posMode>beatchartPlusPos</posMode>
      <breakRest/>
      <barline>double</barline>
      <abbrvTime/>
      <useDisplayTimesig/>
      <leftBarline>final</leftBarline>
    </measSpec>
    <measSpec cmper="3" part="1" shared="true">
      <width>420</width>
      <frontSpaceExtra>0</frontSpaceExtra>
      <backSpaceExtra>0</backSpaceExtra>
    </measSpec>
  </others>
</finale>
)xml";

TEST(MeasureTest, PopulateScore)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(measureXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Measure 1 (Score)
    auto measure1 = others->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure1) << "Measure for score with cmper 1 not found";

    auto keySignature = measure1->createKeySignature();
    ASSERT_TRUE(keySignature);

    EXPECT_EQ(measure1->width, Evpu(600));
    EXPECT_EQ(keySignature->key, 0);
    EXPECT_TRUE(keySignature->keyless);
    EXPECT_FALSE(keySignature->hideKeySigShowAccis);
    EXPECT_EQ(measure1->beats, Cmper(2));
    EXPECT_EQ(measure1->divBeat, Cmper(2));
    EXPECT_EQ(measure1->dispBeats, Cmper(4));
    EXPECT_EQ(measure1->dispDivbeat, Cmper(1024));
    EXPECT_FALSE(measure1->hideCaution);
    EXPECT_FALSE(measure1->hasSmartShape);
    EXPECT_TRUE(measure1->hasTextBlock);
    EXPECT_FALSE(measure1->groupBarlineOverride);
    EXPECT_TRUE(measure1->compositeNumerator);
    EXPECT_TRUE(measure1->compositeDenominator);
    EXPECT_EQ(measure1->positioningMode, others::Measure::PositioningType::TimeSigPlusPositioning);
    EXPECT_EQ(measure1->barlineType, others::Measure::BarlineType::Normal);
    EXPECT_TRUE(measure1->forwardRepeatBar);
    EXPECT_TRUE(measure1->backwardsRepeatBar);
    EXPECT_TRUE(measure1->hasEnding);
    EXPECT_TRUE(measure1->hasChord);
    EXPECT_TRUE(measure1->abbrvTime);
    EXPECT_TRUE(measure1->useDisplayTimesig);
    EXPECT_EQ(measure1->leftBarlineType, others::Measure::BarlineType::OptionsDefault);

    {
        auto timeSig = measure1->createDisplayTimeSignature();
        ASSERT_TRUE(timeSig);
        ASSERT_EQ(timeSig->components.size(), 1);
        auto [count, unit] = timeSig->calcSimplified();
        EXPECT_EQ(count, 4);
        EXPECT_EQ(unit, NoteType::Quarter);
    }

    // Measure 2 (Score)
    auto measure2 = others->get<others::Measure>(SCORE_PARTID, 2);
    ASSERT_TRUE(measure2) << "Measure 2 for score not found";

    keySignature = measure2->createKeySignature();
    ASSERT_TRUE(keySignature);

    EXPECT_EQ(measure2->width, Evpu(600));
    EXPECT_EQ(keySignature->key, 4);
    EXPECT_FALSE(keySignature->keyless);
    EXPECT_TRUE(keySignature->hideKeySigShowAccis);
    EXPECT_EQ(measure2->beats, Cmper(4));
    EXPECT_EQ(measure2->divBeat, Cmper(1024));
    EXPECT_EQ(measure2->dispBeats, Cmper(6));
    EXPECT_EQ(measure2->dispDivbeat, Cmper(6));
    EXPECT_FALSE(measure2->hideCaution);
    EXPECT_TRUE(measure2->hasSmartShape);
    EXPECT_FALSE(measure2->hasTextBlock);
    EXPECT_FALSE(measure2->compositeNumerator);
    EXPECT_FALSE(measure2->compositeDenominator);
    EXPECT_EQ(measure2->barlineType, others::Measure::BarlineType::Normal);
    EXPECT_FALSE(measure2->hasExpression);
    EXPECT_TRUE(measure2->hasTextRepeat);
    EXPECT_FALSE(measure2->hasChord);
    EXPECT_TRUE(measure2->useDisplayTimesig);
    EXPECT_EQ(measure2->leftBarlineType, others::Measure::BarlineType::None);
    EXPECT_TRUE(measure2->compositeDispNumerator);
    EXPECT_TRUE(measure2->compositeDispDenominator);

    {
        auto timeSig = measure2->createTimeSignature();
        ASSERT_TRUE(timeSig);
        ASSERT_EQ(timeSig->components.size(), 1);
        auto [count, unit] = timeSig->calcSimplified();
        EXPECT_EQ(count, 4);
        EXPECT_EQ(unit, NoteType::Quarter);
    }

    // Measure 3 (Score)
    auto measure3 = others->get<others::Measure>(SCORE_PARTID, 3);
    ASSERT_TRUE(measure3) << "Measure 3 for score not found";

    keySignature = measure3->createKeySignature();
    ASSERT_TRUE(keySignature);

    EXPECT_EQ(measure3->width, Evpu(600));
    EXPECT_EQ(keySignature->key, 0);
    EXPECT_FALSE(keySignature->keyless);
    EXPECT_FALSE(keySignature->hideKeySigShowAccis);
    EXPECT_EQ(measure3->beats, Cmper(4));
    EXPECT_EQ(measure3->divBeat, Cmper(4));
    EXPECT_EQ(measure3->dispBeats, Cmper(4));
    EXPECT_EQ(measure3->dispDivbeat, Cmper(1024));
    EXPECT_EQ(measure3->frontSpaceExtra, Evpu(11));
    EXPECT_EQ(measure3->backSpaceExtra, Evpu(13));
    EXPECT_TRUE(measure3->hideCaution);
    EXPECT_FALSE(measure3->hasSmartShape);
    EXPECT_TRUE(measure3->groupBarlineOverride);
    EXPECT_TRUE(measure3->compositeNumerator);
    EXPECT_TRUE(measure3->compositeDenominator);
    EXPECT_EQ(measure3->showKey, others::Measure::ShowKeySigMode::Never);
    EXPECT_EQ(measure3->showTime, others::Measure::ShowTimeSigMode::Never);
    EXPECT_TRUE(measure3->evenlyAcrossMeasure);
    EXPECT_EQ(measure3->positioningMode, others::Measure::PositioningType::BeatChartPlusPositioning);
    EXPECT_TRUE(measure3->breakMmRest);
    EXPECT_EQ(measure3->barlineType, others::Measure::BarlineType::Double);
    EXPECT_TRUE(measure3->abbrvTime);
    EXPECT_FALSE(measure3->hasChord);
    EXPECT_TRUE(measure3->useDisplayTimesig);
    EXPECT_EQ(measure3->leftBarlineType, others::Measure::BarlineType::Final);
}

TEST(MeasureTest, PopulatePart)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(measureXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    const Cmper partId = 1;

    // Measure 1 (Part 1)
    auto measure1 = others->get<others::Measure>(partId, 1);
    ASSERT_TRUE(measure1) << "Measure 1 for part 1 not found";

    // Explicitly test every field, including inherited values
    EXPECT_EQ(measure1->width, Evpu(420)); // Overridden in part-specific measure
    EXPECT_EQ(measure1->beats, Cmper(2)); // Inherited from score
    EXPECT_EQ(measure1->divBeat, Cmper(2)); // Inherited from score
    EXPECT_EQ(measure1->dispBeats, Cmper(4)); // Inherited from score
    EXPECT_EQ(measure1->dispDivbeat, Cmper(1024)); // Inherited from score
    EXPECT_TRUE(measure1->compositeNumerator); // Inherited from score
    EXPECT_TRUE(measure1->compositeDenominator); // Inherited from score
    EXPECT_EQ(measure1->positioningMode, others::Measure::PositioningType::TimeSigPlusPositioning); // Inherited from score
    EXPECT_EQ(measure1->barlineType, others::Measure::BarlineType::Normal); // Inherited from score
    EXPECT_TRUE(measure1->abbrvTime); // Inherited from score
    EXPECT_TRUE(measure1->useDisplayTimesig); // Inherited from score
    EXPECT_EQ(measure1->leftBarlineType, others::Measure::BarlineType::OptionsDefault); // Inherited from score

    // Measure 2 (Part 1)
    auto measure2 = others->get<others::Measure>(partId, 2);
    ASSERT_TRUE(measure2) << "Measure 2 for part 1 not found";

    EXPECT_EQ(measure2->width, Evpu(368)); // Overridden in part-specific measure
    EXPECT_EQ(measure2->beats, Cmper(4)); // Inherited from score
    EXPECT_EQ(measure2->divBeat, Cmper(1024)); // Inherited from score
    EXPECT_EQ(measure2->dispBeats, Cmper(6)); // Inherited from score
    EXPECT_EQ(measure2->dispDivbeat, Cmper(6)); // Inherited from score
    EXPECT_FALSE(measure2->compositeNumerator); // Inherited from score
    EXPECT_FALSE(measure2->compositeDenominator); // Inherited from score
    EXPECT_EQ(measure2->positioningMode, others::Measure::PositioningType::BeatChartPlusPositioning); // Overridden
    EXPECT_EQ(measure2->barlineType, others::Measure::BarlineType::Normal); // Inherited from score
    EXPECT_TRUE(measure2->useDisplayTimesig); // Inherited from score
    EXPECT_EQ(measure2->leftBarlineType, others::Measure::BarlineType::None); // Inherited from score
    EXPECT_TRUE(measure2->compositeDispNumerator); // Inherited from score
    EXPECT_TRUE(measure2->compositeDispDenominator); // Inherited from score

    // Measure 3 (Part 1)
    auto measure3 = others->get<others::Measure>(partId, 3);
    ASSERT_TRUE(measure3) << "Measure 3 for part 1 not found";

    EXPECT_EQ(measure3->width, Evpu(420)); // Overridden in part-specific measure
    EXPECT_EQ(measure3->beats, Cmper(4)); // Inherited from score
    EXPECT_EQ(measure3->divBeat, Cmper(4)); // Inherited from score
    EXPECT_EQ(measure3->dispBeats, Cmper(4)); // Inherited from score
    EXPECT_EQ(measure3->dispDivbeat, Cmper(1024)); // Inherited from score
    EXPECT_EQ(measure3->frontSpaceExtra, Evpu(0)); // Overridden
    EXPECT_EQ(measure3->backSpaceExtra, Evpu(0)); // Overridden
    EXPECT_TRUE(measure3->hideCaution); // Inherited from score
    EXPECT_TRUE(measure3->groupBarlineOverride); // Inherited from score
    EXPECT_TRUE(measure3->compositeNumerator); // Inherited from score
    EXPECT_TRUE(measure3->compositeDenominator); // Inherited from score
    EXPECT_EQ(measure3->showKey, others::Measure::ShowKeySigMode::Never); // Inherited from score
    EXPECT_EQ(measure3->showTime, others::Measure::ShowTimeSigMode::Never); // Inherited from score
    EXPECT_TRUE(measure3->evenlyAcrossMeasure); // Inherited from score
    EXPECT_EQ(measure3->positioningMode, others::Measure::PositioningType::BeatChartPlusPositioning); // Inherited from score
    EXPECT_TRUE(measure3->breakMmRest); // Inherited from score
    EXPECT_EQ(measure3->barlineType, others::Measure::BarlineType::Double); // Inherited from score
    EXPECT_TRUE(measure3->abbrvTime); // Inherited from score
    EXPECT_TRUE(measure3->useDisplayTimesig); // Inherited from score
    EXPECT_EQ(measure3->leftBarlineType, others::Measure::BarlineType::Final); // Inherited from score
}

TEST(MeasureTest, UnlinkedNodes)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(measureXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    const Cmper partId = 1;

    // Measure 1 (Part 1)
    auto measure1 = others->get<others::Measure>(partId, 1);
    ASSERT_TRUE(measure1) << "Measure 1 for part 1 not found";

    auto unlinkedNodes = measure1->getUnlinkedNodes();

    // Check overridden nodes
    EXPECT_NE(unlinkedNodes.find("width"), unlinkedNodes.end()) << "Width should be unlinked in Measure 1 Part 1";

    // Check linked nodes
    EXPECT_EQ(unlinkedNodes.find("beats"), unlinkedNodes.end()) << "Beats should be linked in Measure 1 Part 1";
    EXPECT_EQ(unlinkedNodes.find("groupBarlineOverride"), unlinkedNodes.end()) << "GroupBarlineOverride should be linked in Measure 1 Part 1";

    // Measure 2 (Part 1)
    auto measure2 = others->get<others::Measure>(partId, 2);
    ASSERT_TRUE(measure2) << "Measure 2 for part 1 not found";

    unlinkedNodes = measure2->getUnlinkedNodes();

    // Check overridden nodes
    EXPECT_NE(unlinkedNodes.find("width"), unlinkedNodes.end()) << "Width should be unlinked in Measure 2 Part 1";
    EXPECT_NE(unlinkedNodes.find("posMode"), unlinkedNodes.end()) << "Positioning mode should be unlinked in Measure 2 Part 1";

    // Check linked nodes
    EXPECT_EQ(unlinkedNodes.find("beats"), unlinkedNodes.end()) << "Beats should be linked in Measure 2 Part 1";
    EXPECT_EQ(unlinkedNodes.find("useDisplayTimesig"), unlinkedNodes.end()) << "UseDisplayTimesig should be linked in Measure 2 Part 1";

    // Measure 3 (Part 1)
    auto measure3 = others->get<others::Measure>(partId, 3);
    ASSERT_TRUE(measure3) << "Measure 3 for part 1 not found";

    unlinkedNodes = measure3->getUnlinkedNodes();

    // Check overridden nodes
    EXPECT_NE(unlinkedNodes.find("width"), unlinkedNodes.end()) << "Width should be unlinked in Measure 3 Part 1";
    EXPECT_NE(unlinkedNodes.find("frontSpaceExtra"), unlinkedNodes.end()) << "FrontSpaceExtra should be unlinked in Measure 3 Part 1";
    EXPECT_NE(unlinkedNodes.find("backSpaceExtra"), unlinkedNodes.end()) << "BackSpaceExtra should be unlinked in Measure 3 Part 1";

    // Check linked nodes
    EXPECT_EQ(unlinkedNodes.find("barline"), unlinkedNodes.end()) << "Barline should be linked in Measure 3 Part 1";
    EXPECT_EQ(unlinkedNodes.find("compositeNumerator"), unlinkedNodes.end()) << "CompositeNumerator should be linked in Measure 3 Part 1";
}

TEST(MeasureTest, CompositeTimeSig1)
{
  constexpr static musxtest::string_view testXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <measSpec cmper="1">
      <width>600</width>
      <beats>7</beats>
      <divbeat>8</divbeat>
      <dispBeats>4</dispBeats>
      <dispDivbeat>1024</dispDivbeat>
      <altNumTsig/>
      <altDenTsig/>
      <posMode>timesigPlusPos</posMode>
      <barline>normal</barline>
      <leftBarline>default</leftBarline>
    </measSpec>
    <timeLower cmper="8">
      <tldata>
        <integer>2048</integer>
        <startGroup/>
      </tldata>
      <tldata>
        <integer>1024</integer>
      </tldata>
      <tldata>
        <integer>256</integer>
        <startGroup/>
      </tldata>
      <tldata>
        <integer>512</integer>
        <startGroup/>
      </tldata>
      <tldata>
        <integer>256</integer>
      </tldata>
    </timeLower>
    <timeUpper cmper="7">
      <tudata>
        <integer>1</integer>
        <startGroup/>
      </tudata>
      <tudata>
        <integer>2</integer>
        <frac>15480</frac>
      </tudata>
      <tudata>
        <integer>3</integer>
        <startGroup/>
      </tudata>
      <tudata>
        <integer>2</integer>
      </tudata>
      <tudata>
        <integer>1</integer>
        <frac>15480</frac>
        <startGroup/>
      </tudata>
      <tudata>
        <integer>5</integer>
      </tudata>
    </timeUpper>
  </others>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(testXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);


    // Measure 1 (Score)
    auto measure = others->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure) << "Measure for score with cmper 1 not found";

    EXPECT_FALSE(measure->useDisplayTimesig);
    EXPECT_TRUE(measure->createTimeSignature()->isSame(*measure->createDisplayTimeSignature().get()));

    auto timeSig = measure->createTimeSignature();
    ASSERT_TRUE(timeSig);
    ASSERT_EQ(timeSig->components.size(), 3);
    {
        auto [count, unit] = timeSig->calcSimplified();
        EXPECT_EQ(count, musx::util::Fraction(133, 2));
        EXPECT_EQ(unit, NoteType::Note16th);
    }
    {
        auto component = timeSig->createComponent(0);
        ASSERT_TRUE(component);
        auto [count, unit] = component->calcSimplified();
        EXPECT_EQ(count, musx::util::Fraction(21, 2));
        EXPECT_EQ(count.quotient(), 10);
        EXPECT_EQ(count.remainder(), musx::util::Fraction(1, 2));
        EXPECT_EQ(unit, NoteType::Quarter);
    }
    {
        auto component = timeSig->createComponent(1);
        ASSERT_TRUE(component);
        auto [count, unit] = component->calcSimplified();
        EXPECT_EQ(count, 5);
        EXPECT_EQ(count.quotient(), 5);
        EXPECT_EQ(count.remainder(), 0);
        EXPECT_EQ(unit, NoteType::Note16th);
    }
    {
        auto component = timeSig->createComponent(2);
        ASSERT_TRUE(component);
        auto [count, unit] = component->calcSimplified();
        EXPECT_EQ(count, musx::util::Fraction(39, 2));
        EXPECT_EQ(count.quotient(), 19);
        EXPECT_EQ(count.remainder(), musx::util::Fraction(1, 2));
        EXPECT_EQ(unit, NoteType::Note16th);
    }
}


TEST(MeasureTest, CompositeTimeSig2)
{
  constexpr static musxtest::string_view testXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <measSpec cmper="1">
      <width>600</width>
      <beats>19</beats>
      <divbeat>1024</divbeat>
      <dispBeats>4</dispBeats>
      <dispDivbeat>1024</dispDivbeat>
      <altNumTsig/>
      <posMode>timesigPlusPos</posMode>
      <barline>normal</barline>
      <leftBarline>default</leftBarline>
    </measSpec>
    <timeUpper cmper="19">
      <tudata>
        <integer>2</integer>
        <frac>10360</frac>
        <startGroup/>
      </tudata>
    </timeUpper>
  </others>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(testXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);


    // Measure 1 (Score)
    auto measure = others->get<others::Measure>(SCORE_PARTID, 1);
    ASSERT_TRUE(measure) << "Measure for score with cmper 1 not found";

    EXPECT_FALSE(measure->useDisplayTimesig);
    EXPECT_TRUE(measure->createTimeSignature()->isSame(*measure->createDisplayTimeSignature().get()));

    auto timeSig = measure->createTimeSignature();
    ASSERT_TRUE(timeSig);
    ASSERT_EQ(timeSig->components.size(), 1);
    {
        auto [count, unit] = timeSig->calcSimplified();
        EXPECT_EQ(count, musx::util::Fraction(7, 3));
        EXPECT_EQ(unit, NoteType::Quarter);
    }
}

TEST(MeasureTest, LegacyPickupSpacers)
{
    std::vector<char> enigmaXml;
    // NOTE: This enigmaxml has been hand-edited to remove Part 3's non-shared MultiStaffGroupId record.
    musxtest::readFile(musxtest::getInputPath() / "pickup-legacy.enigmaxml", enigmaXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(enigmaXml);
    ASSERT_TRUE(doc);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    {
        auto measure = others->get<others::Measure>(SCORE_PARTID, 1);
        ASSERT_TRUE(measure);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(1), musx::util::Fraction(3, 4));
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(2), musx::util::Fraction(9, 8));
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(), musx::util::Fraction(3, 4));
    }
    {
        auto measure = others->get<others::Measure>(SCORE_PARTID, 2);
        ASSERT_TRUE(measure);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(1), 0);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(2), 0);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(), 0);
    }
    {
        auto measure = others->get<others::Measure>(SCORE_PARTID, 3);
        ASSERT_TRUE(measure);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(1), 0);
        EXPECT_EQ(measure->calcMinLegacyPickupSpacer(2), 0);
    }
}

TEST(SplitMeasureTest, Populate)
{
    constexpr static musxtest::string_view testXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <splitMeas cmper="1">
      <data>112</data>
      <data>368</data>
    </splitMeas>
  </others>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(testXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto splits = others->get<others::SplitMeasure>(SCORE_PARTID, 1);
    ASSERT_TRUE(splits);
    ASSERT_EQ(splits->values.size(), 2);
    EXPECT_EQ(splits->values[0], Evpu{112});
    EXPECT_EQ(splits->values[1], Evpu{368});
}
