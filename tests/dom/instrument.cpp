/*
 * Copyright (C) 2026, Robert Patterson
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

using namespace musx::dom;

TEST(InstrumentTest, InstrumentFamilyFromUuid)
{
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Violin), InstrumentFamily::Strings);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::TrumpetBFlat), InstrumentFamily::Brass);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Flute), InstrumentFamily::Woodwinds);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::DrumSet), InstrumentFamily::Percussion);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Marimba), InstrumentFamily::PitchedPercussion);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Piano), InstrumentFamily::Keyboard);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Organ), InstrumentFamily::Organ);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::SopranoVoice), InstrumentFamily::Voice);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Guitar), InstrumentFamily::PluckedStrings);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::SynthPad), InstrumentFamily::Synth);
}

TEST(InstrumentTest, InstrumentFamilyFollowsMusicXmlSoundIdTaxonomy)
{
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Kora), InstrumentFamily::PluckedStrings);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Melodica), InstrumentFamily::Woodwinds);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::SynthBrass), InstrumentFamily::Brass);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::HammeredDulcimer), InstrumentFamily::PitchedPercussion);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Timpani), InstrumentFamily::Percussion);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::StringBass), InstrumentFamily::Strings);
}

TEST(InstrumentTest, InstrumentFamilySpecialAndUnknownUuids)
{
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::BlankStaff), InstrumentFamily::Unspecified);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::Unknown), InstrumentFamily::Unspecified);
    EXPECT_EQ(instrumentFamilyFromUuid(uuid::HornF_WWQuintet), InstrumentFamily::Brass);
    EXPECT_EQ(instrumentFamilyFromUuid("not-a-finale-instrument-uuid"), InstrumentFamily::Unspecified);
}
