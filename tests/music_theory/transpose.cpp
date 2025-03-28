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

#include "music_theory/transposer.hpp"
 
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
 
 TEST(TransposerTest, ChromaticTransposeExamples) {
     Transposer t(0, 0); // C natural
     t.chromaticTranspose(2, -1); // Minor third up
     EXPECT_EQ(t.displacement(), 2);
     EXPECT_EQ(t.alteration(), -1);
 
     t.chromaticTranspose(-2, 1); // Back down a minor third
     EXPECT_EQ(t.displacement(), 0);
     EXPECT_EQ(t.alteration(), 0);
 }
 
 TEST(TransposerTest, Non12EDOEquivalentPitch) {
     std::vector<int> edo19Major = {0, 3, 6, 8, 11, 14, 17};
     Transposer t(0, 0, false, 19, edo19Major);
 
     // D♭ = C
     EXPECT_TRUE(t.isEnharmonicEquivalent(1, -1));
 
     // B♯ = C
     EXPECT_TRUE(t.isEnharmonicEquivalent(-1, 1));
 
     // C♯ != C
     EXPECT_FALSE(t.isEnharmonicEquivalent(0, 1));
 }
 
 TEST(TransposerTest, SimplifySpellingWorks) {
     Transposer t(1, -2); // D double flat
     t.simplifySpelling();
     // Should resolve to C natural
     EXPECT_EQ(t.displacement(), 0);
     EXPECT_EQ(t.alteration(), 0);
 }
 