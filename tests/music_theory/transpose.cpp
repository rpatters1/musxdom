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

#include <gtest/gtest.h>
 
 using namespace music_theory;
 
 TEST(TransposerTest, Basic12EDOEquivalents) {
     Transposer t(0, 0); // C natural
 
     // C == C
     EXPECT_TRUE(t.isEnharmonicEquivalent(0, 0));
 
     // Dbb == C
     EXPECT_TRUE(t.isEnharmonicEquivalent(1, -2));
 
     // B♯ == C
     EXPECT_TRUE(t.isEnharmonicEquivalent(-1, 1));
 
     // C in next octave
     EXPECT_FALSE(t.isEnharmonicEquivalent(7, 0));
 
     // C♯ != C
     EXPECT_FALSE(t.isEnharmonicEquivalent(0, 1));
 }
 
 TEST(TransposerTest, StepwiseTranspose12EDO) {
     Transposer t(0, 0); // C natural
     t.stepwiseTranspose(1); // Up one chromatic step (C#)
     EXPECT_EQ(t.alteration(), 1);
 
     t.stepwiseTranspose(1); // Up another chromatic step (D)
     EXPECT_EQ(t.alteration(), 0);
     EXPECT_EQ(t.displacement(), 1);
 }
 
 TEST(TransposerTest, ChromaticTransposeFromRandomBasesWithExpectedResults)
 {
    struct IntervalTestCase {
        int interval;
        int alteration;
    };

    struct State {
        int displacement;
        int alteration;
    };

    // 1. Interval + alteration to apply
    const std::array<IntervalTestCase, 7> intervals = {{
        { 0,  0 },  // unison
        { 1, -1 },  // minor 2nd
        { 2,  1 },  // augmented 3rd
        { 3,  0 },  // perfect 4th
        { 4, -2 },  // doubly diminished 5th
        { 5,  0 },  // major 6th
        { 6,  1 },  // augmented 7th
    }};

    // 2. Starting base values
    const std::array<State, 7> bases = {{
        { -5,  2 },  // E♭♭
        {  0, -1 },  // C♭
        { 12,  0 },  // A
        {  3, -3 },  // F♭♭♭
        { -9,  1 },  // A♯
        {  7,  0 },  // C
        { 14, -2 },  // C♭♭
    }};

    // 3. Expected results after transposition
    const std::array<State, 7> expectedResults = {{
        { -5,  2 },   // unison
        {  1, -2 },   // D♭ from C♭
        { 14,  2 },   // C♯♯ from A
        {  6, -4 },   // B♭♭♭♭ from F♭♭♭
        { -5, -1 },   // E♭ from A♯
        { 12, 0 },    // A from C
        { 20, -1 },   // B♭ from C♭♭
    }};

    for (size_t i = 0; i < intervals.size(); ++i) {
        const auto& [interval, alteration] = intervals[i];
        const auto& [baseDisp, baseAlt] = bases[i];
        const auto& [expectedDisp, expectedAlt] = expectedResults[i];

        SCOPED_TRACE("Case #" + std::to_string(i) +
                     ", Interval = " + std::to_string(interval) +
                     ", Alteration = " + std::to_string(alteration));

        Transposer t(baseDisp, baseAlt);
        t.chromaticTranspose(interval, alteration);

        EXPECT_EQ(t.displacement(), expectedDisp);
        EXPECT_EQ(t.alteration(), expectedAlt);

        t.chromaticTranspose(-interval, -alteration);
        EXPECT_EQ(t.displacement(), baseDisp);
        EXPECT_EQ(t.alteration(), baseAlt);
    }
 }
 
 TEST(TransposerTest, ChromaticTransposeFromRandomBasesWithExpectedResults31EDO)
{
    struct IntervalTestCase {
        int interval;
        int alteration;
    };

    struct State {
        int displacement;
        int alteration;
    };

    // 1. Interval + alteration to apply
    const std::array<IntervalTestCase, 7> intervals = {{
        { 0,  0 },  // unison
        { 1, -1 },  // minor 2nd
        { 2,  1 },  // augmented 3rd
        { 3,  0 },  // perfect 4th
        { 4, -2 },  // doubly diminished 5th
        { 5,  0 },  // major 6th
        { 6,  1 },  // augmented 7th
    }};

    // 2. Starting base values
    const std::array<State, 7> bases = {{
        { -5,  2 },  // E♭
        {  0, -2 },  // C♭
        { 12,  0 },  // A
        {  3, -3 },  // F 3/4 ♭
        { -9,  1 },  // A 1/4 ♯
        {  7,  0 },  // C
        { 14, -2 },  // C♭
    }};

    // 3. Expected results after transposition (computed for 31-EDO major scale)
    const std::array<State, 7> expectedResults = {{
        { -5,  2 },  // unison
        {  1, -4 },  // D♭ from C♭
        { 14,  4 },  // C♯♯ from A
        {  6, -5 },  // B 5/4 ♭ from F 3/4 ♭
        { -5, -3 },  // E 3/4 ♭ from A♯
        { 12, 0 },   // A from C
        { 20, 0 },   // B from C♭
    }};

    const std::vector<int> major31EDO = { 0, 5, 10, 13, 18, 23, 28 };

    for (size_t i = 0; i < intervals.size(); ++i) {
        const auto& [interval, alteration] = intervals[i];
        const auto& [baseDisp, baseAlt] = bases[i];
        const auto& [expectedDisp, expectedAlt] = expectedResults[i];

        SCOPED_TRACE("Case #" + std::to_string(i) +
                     ", Interval = " + std::to_string(interval) +
                     ", Alteration = " + std::to_string(alteration));

        Transposer t(baseDisp, baseAlt, false, 31, major31EDO);

        t.chromaticTranspose(interval, alteration);

        EXPECT_EQ(t.displacement(), expectedDisp);
        EXPECT_EQ(t.alteration(), expectedAlt);

        t.chromaticTranspose(-interval, -alteration);
        EXPECT_EQ(t.displacement(), baseDisp);
        EXPECT_EQ(t.alteration(), baseAlt);
    }
}

TEST(TransposerTest, Non12EDOEquivalentPitch) {
    std::vector<int> edo19Major = {0, 3, 6, 8, 11, 14, 17};
    Transposer t(0, 0, false, 19, edo19Major);

    // D♭ != C
    EXPECT_FALSE(t.isEnharmonicEquivalent(1, -2));
    // but one more flat is equivalent
    EXPECT_TRUE(t.isEnharmonicEquivalent(1, -3));

    // B♯ = C
    EXPECT_TRUE(t.isEnharmonicEquivalent(-1, 2));

    // C♯ != D♭
    t = Transposer(0, 1, false, 19, edo19Major);
    EXPECT_FALSE(t.isEnharmonicEquivalent(1, -1));
    // but one more flat is equivalent
    EXPECT_TRUE(t.isEnharmonicEquivalent(1, -2));
}
 
 TEST(TransposerTest, SimplifySpellingWorks) {
     Transposer t(1, -2); // D double flat
     t.simplifySpelling();
     // Should resolve to C natural
     EXPECT_EQ(t.displacement(), 0);
     EXPECT_EQ(t.alteration(), 0);
 }
 
 TEST(TransposerTest, LargeInterval)
 {
     Transposer t(1, -1); // D4 flat
     t.chromaticTranspose(12, -1); // up a minor 6th plus an octave
     EXPECT_EQ(t.displacement(), 13);
     EXPECT_EQ(t.alteration(), -2); // A5 double-flat
     t.simplifySpelling();
     EXPECT_EQ(t.displacement(), 12);
     EXPECT_EQ(t.alteration(), 0);  // G5 natural
 }

 TEST(TransposerTest, Calc12EdoHalfstepsInInterval)
{
    // Perfect unison, no alteration
    EXPECT_EQ(calc12EdoHalfstepsInInterval(0, 0), 0);

    // Major third, no alteration
    EXPECT_EQ(calc12EdoHalfstepsInInterval(2, 0), 4);

    // Minor third (by adding -1 alteration to major third)
    EXPECT_EQ(calc12EdoHalfstepsInInterval(2, -1), 3);

    // Perfect octave
    EXPECT_EQ(calc12EdoHalfstepsInInterval(7, 0), 12);

    // Negative interval (descending perfect fifth)
    EXPECT_EQ(calc12EdoHalfstepsInInterval(-4, 0), -7);
}

TEST(TransposerTest, CalcAlterationFrom12EdoHalfsteps)
{
    // Major third: interval=2, halfsteps=4 → alteration=0
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(2, 4), 0);

    // Minor third: interval=2, halfsteps=3 → alteration=-1
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(2, 3), -1);

    // Augmented third: interval=2, halfsteps=5 → alteration=+1
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(2, 5), +1);

    // Perfect octave: interval=7, halfsteps=12 → alteration=0
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(7, 12), 0);

    // Descending perfect fifth: interval=-4, halfsteps=-7 → alteration=0
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(-4, -7), 0);

    // Interval = +3 (perfect fourth), halfsteps = -3
    // Expected unaltered halfsteps = +5
    // Therefore alteration = -3 - 5 = -8 (8x diminished 4th)
    EXPECT_EQ(calcAlterationFrom12EdoHalfsteps(3, -3), -8);
}

TEST(TransposerTest, CalcFunctionsAreInverse)
{
    // Test symmetry for a variety of intervals and alterations
    for (int interval = -14; interval <= 14; ++interval)
    {
        for (int alteration = -7; alteration <= 7; ++alteration)
        {
            int halfsteps = calc12EdoHalfstepsInInterval(interval, alteration);
            int calcAlt = calcAlterationFrom12EdoHalfsteps(interval, halfsteps);
            EXPECT_EQ(calcAlt, alteration) << "Failed on interval " << interval << " alteration " << alteration;
        }
    }
}

TEST(TransposerTest, CalcAlterationFromKeySigChange_Basic)
{
    // Bb clarinet transposes a major second up
    // Expected for major second up is +2 sharps
    EXPECT_EQ(calcAlterationFromKeySigChange(1, +2), 0);

    // A clarinet transposes a minor third up
    // Expected for major third up (+2) is +3 flats
    EXPECT_EQ(calcAlterationFromKeySigChange(2, -3), -1);

    // Eb clarinet transposes a minor third down
    // Expected for major third up is +3 sharps
    EXPECT_EQ(calcAlterationFromKeySigChange(-2, +3), 1);

    // Eb bari sax transposes up a major 13th (octave + major sixth)
    // Expected for major sixth up (+5) is +3 sharps
    EXPECT_EQ(calcAlterationFromKeySigChange(12, +3), 0);

    // F Horn transposes a perfect fifth up
    // Expected for perfect fifth up (+4) is +1 sharp
    EXPECT_EQ(calcAlterationFromKeySigChange(4, +1), 0);

    // D Horn transposes a minor 7th up
    // Expected for minor 7th up (+6) is +2 flats
    EXPECT_EQ(calcAlterationFromKeySigChange(6, -2), -1);

    // B Horn transposes a minor 9th up
    // Expected for minor 9th up (+8) is +5 flats
    EXPECT_EQ(calcAlterationFromKeySigChange(8, -5), -1);

    // G-basso Horn transposes a perfect fifth down
    // Expected for perfect fifth up (+4) is +1 flat
    EXPECT_EQ(calcAlterationFromKeySigChange(-4, -1), 0);
}

TEST(TransposerTest, CalcAlterationFromKeySigChange_Theoretical)
{
    // Fb Transposition up
    EXPECT_EQ(calcAlterationFromKeySigChange(4, +8), 1) << "resulting interval should be up aug 5th {4, 1}";
    // Fb Transposition down
    EXPECT_EQ(calcAlterationFromKeySigChange(-3, +8), 1) << "resulting interval should be down dim 4th {-3, 1}";

    // Gb Transposition up
    EXPECT_EQ(calcAlterationFromKeySigChange(3, 6), 1) << "resulting interval should be up aug 4th {3, 1}";
    // Gb Transposition down
    EXPECT_EQ(calcAlterationFromKeySigChange(-4, 6), 1) << "resulting interval should be down dim 5th {-4, 1}";

    // F# Transposition up
    EXPECT_EQ(calcAlterationFromKeySigChange(4, -6), -1) << "resulting interval should be up dim 5th {4, -1}";

    // Ebb Transposition up
    EXPECT_EQ(calcAlterationFromKeySigChange(5, 10), 1) << "resulting interval should be up aug 6th {5, 1}";
    // Ebb Transposition dn
    EXPECT_EQ(calcAlterationFromKeySigChange(-2, 10), 2) << "resulting interval should be down dim 3rd {-2, 2}";

    // D# Transposition up
    EXPECT_EQ(calcAlterationFromKeySigChange(6, -9), -2) << "resulting interval should be up dim 7th {6, -2}";
    // D# Transposition dn
    EXPECT_EQ(calcAlterationFromKeySigChange(-1, -9), -1) << "resulting interval should be down aug 2nd {-1, -1}";
}
