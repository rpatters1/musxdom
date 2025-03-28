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
#pragma once

#include <array>
#include <vector>

/*
This header-only library has no dependencies and can be shared into any other library merely
by including it.
*/

/// @namespace music_theory
/// @brief A dependency free repository of useful functions for music theory
namespace music_theory {

constexpr unsigned STANDARD_DIATONIC_STEPS = 7; ///< currently this is the only supported number of diatonic steps.
constexpr unsigned STANDARD_12EDO_STEPS = 12;   ///< this can be overriden when constructing a @ref Transposer instance.

/// @brief Calculates the displacement value for a given absolute pitch class and octave
/// @param pitchClass 0..6 corresponding to C..B
/// @param octave Octave 4 is the middle-C octave
/// @param tonalCenter 0..6 corresponding to C..B
/// @return A displacement value that can be used to create a @ref Transposer instance.
inline int calcDisplacement(unsigned pitchClass, int octave, unsigned tonalCenter = 0)
{
    pitchClass %= STANDARD_DIATONIC_STEPS;
    tonalCenter %= STANDARD_DIATONIC_STEPS;

    int relativeStep = static_cast<int>(pitchClass) - static_cast<int>(tonalCenter);
    int relativeOctave = octave - 4;

    return relativeStep + STANDARD_DIATONIC_STEPS * relativeOctave;
}

class Transposer
{
private:
    static constexpr std::array<unsigned, STANDARD_DIATONIC_STEPS> MAJOR_KEYMAP = { 0, 2, 4, 5, 7, 9, 11 };
    static constexpr std::array<unsigned, STANDARD_DIATONIC_STEPS> MINOR_KEYMAP = { 0, 2, 3, 5, 7, 8, 10 };
    int m_displacement; ///< 0 signifies the tonic in the C4 (middle-C) octave
    int m_alteration;   ///< alteration from key signature
    unsigned m_tonalCenter;
    unsigned m_numberOfSteps; // number of steps in the EDO (default 12)
    std::vector<unsigned> m_keyMap; // step map for the EDO

public:
    Transposer(int displacement, int alteration, unsigned tonalCenter = 0,
        bool isMinor = false, unsigned numberOfSteps = STANDARD_12EDO_STEPS,
        const std::optional <std::vector<unsigned>>& keyMap)
        : m_displacement(displacement), m_alteration(alteration), m_tonalCenter(tonalCenter)
    {
        if (keyMap) {
            m_keyMap = keyMap.value();
        } else if (isMinor) {
            m_keyMap.emplace_back(MINOR_KEYMAP.begin(), MINOR_KEYMAP.end());
        } else {
            m_keyMap.emplace_back(MAJOR_KEYMAP.begin(), MAJOR_KEYMAP.end());
        }
    }
};

} // namespace music_theory