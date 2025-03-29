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

#include "music_theory/music_theory.hpp"
 
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
 