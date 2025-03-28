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
#include <cmath>
#include <algorithm>

/*
This header-only library has no dependencies and can be shared into any other library merely
by including it.
*/

/// @namespace music_theory
/// @brief A dependency free repository of useful functions for music theory
namespace music_theory {

constexpr int STANDARD_DIATONIC_STEPS = 7; ///< currently this is the only supported number of diatonic steps.
constexpr int STANDARD_12EDO_STEPS = 12;   ///< this can be overriden when constructing a @ref Transposer instance.

/// @brief Calculates the displacement value for a given absolute pitch class and octave
/// @param pitchClass 0..6 corresponding to C..B
/// @param octave Octave 4 is the middle-C octave
/// @return A displacement value that can be used to create a @ref Transposer instance.
inline int calcDisplacement(int pitchClass, int octave)
{
    pitchClass %= STANDARD_DIATONIC_STEPS;
    const int relativeOctave = octave - 4;

    return pitchClass + (STANDARD_DIATONIC_STEPS * relativeOctave);
}

/// @brief Calculates the sign of an integer.
/// @param n The integer to process.
/// @return -1 for negative; 1 for zero or positive.
template <typename T>
constexpr inline T sign(T n)
{
    static_assert(std::is_arithmetic_v<T>, "sign requires a numeric type");
    return n < T(0) ? T(-1) : T(1);
}

template <typename T>
constexpr T signedModulus(T n, T d) {
    static_assert(std::is_integral_v<T>, "signed_modulus requires an integer type");
    return sign(n) * (std::abs(n) % d);
}

/// @class Tranposer
/// @brief Provides dependency-free transposition utilities that work with any EDO that uses 7 diatonic steps.
///
/// The way to use this class is to intialize it in the constructor, run one or more transposition functions,
/// then extract the modified displacement and alteration values.
class Transposer
{
private:
    static constexpr std::array<int, STANDARD_DIATONIC_STEPS> MAJOR_KEYMAP = { 0, 2, 4, 5, 7, 9, 11 };
    static constexpr std::array<int, STANDARD_DIATONIC_STEPS> MINOR_KEYMAP = { 0, 2, 3, 5, 7, 8, 10 };
    int m_displacement; 
    int m_alteration;   ///< alteration from key signature
    int m_tonalCenter;
    int m_numberOfSteps; // number of steps in the EDO (default 12)
    std::vector<int> m_keyMap; // step map for the EDO

    static constexpr std::array<std::array<int, 2>, 7> DIATONIC_INTERVAL_ADJUSTMENTS = { {
        { 0,  0},  // unison
        { 2, -1},  // second
        { 4, -2},  // third
        {-1,  1},  // fourth
        { 1,  0},  // fifth
        { 3, -1},  // sixth
        { 5, -2}   // seventh
    }};
    
public:
    /// @brief Construction function
    /// @param displacement     ///< the scale step displacement value. 0 signifies the tonic in the C4 (middle-C) octave
    /// @param alteration       ///< the number of chromatic half-step alterations. This is not the same as EDO steps.
    /// @param tonalCenter      ///< 0..6, signifying C, D, E, F, G, A, B.
    /// @param isMinor          ///< true if the key is a minor key
    /// @param numberOfSteps    ///< The the number of steps in the EDO. (E.g., 31-EDO would pass 31.)
    /// @param keyMap           ///< A 7-element map specifying the starting EDO step of each diatonic step. 12-EDO Major (the default) is { 0, 2, 4, 5, 7, 9, 11 }. 
    Transposer(int displacement, int alteration, int tonalCenter = 0,
        bool isMinor = false, int numberOfSteps = STANDARD_12EDO_STEPS,
        const std::optional <std::vector<int>>& keyMap)
        : m_displacement(displacement), m_alteration(alteration), m_tonalCenter(tonalCenter)
    {
        if (keyMap) {
            if (keyMap.value().size() != STANDARD_DIATONIC_STEPS) {
                throw std::invalid_argument("The Transposer class only supports key map arrays of " + std::to_string(STANDARD_DIATONIC_STEPS) + " elements");
            }
            m_keyMap = keyMap.value();
        } else if (isMinor) {
            m_keyMap.emplace_back(MINOR_KEYMAP.begin(), MINOR_KEYMAP.end());
        } else {
            m_keyMap.emplace_back(MAJOR_KEYMAP.begin(), MAJOR_KEYMAP.end());
        }
    }

    /// @brief Return the current displacement value
    int displacement() const { return m_displacement; }

    /// @brief Return the current chromatic alteration value
    int alteration() const { return m_alteration; }

    /// @brief Transposes the displacement by the specified interval.
    /// @param interval 0 = unison, 1 = second, 2 = third, etc. The interval may be any size, and it can be negative for downward transposition.
    void diatonicTranspose(int interval)
    {
        m_displacement += interval;
    }

    /// @brief Transposes enharmonically relative to the current values
    /// @param direction negative or positive (usually -1 or 1)
    void enharmonicTranspose(int direction)
    {
        const int keyStepEnharmonic = calcStepsBetweenScaleDegrees(m_displacement, m_displacement + sign(direction));
        diatonicTranspose(sign(direction));
        m_alteration -= sign(direction) * keyStepEnharmonic;
    }
    
    /**
     * @brief Chromatically transposes by a specified interval and alteration.
     *
     * This function transposes the note chromatically in any EDO. The chromatic interval is defined by a diatonic
     * displacement (e.g., 0 = unison, 1 = second, 2 = third, etc.) and a chromatic alteration.
     *
     * A chromatic alteration of 0 corresponds to major or perfect intervals. For example:
     * - `{2, -1}` is a minor third up
     * - `{3,  0}` is a perfect fourth up
     * - `{5,  1}` is an augmented sixth up
     *
     * Reversing the signs of both `interval` and `alteration` allows for downward transposition.
     *
     * @param interval    The diatonic displacement (negative for downward transposition).
     * @param alteration  The chromatic half-step alteration that defines the chromatic interval (negative for downward).
     *
     * @return true on success. Always returns true in this implementation, as error conditions are not possible.
     */
    void chromaticTranspose(int interval, int alteration)
    {
        const int intervalNormalized = signedModulus(interval, STANDARD_DIATONIC_STEPS);
        const int stepsInAlteration = calcStepsInAlteration(interval, alteration);
        const int stepsInInterval = calcStepsInNormalizedInterval(intervalNormalized);
        const int stepsInDiatonicInterval = calcStepsBetweenScaleDegrees(m_displacement, m_displacement + intervalNormalized);
    
        const int effectiveAlteration = stepsInAlteration + stepsInInterval - sign(interval) * stepsInDiatonicInterval;
    
        diatonicTranspose(interval);
        m_alteration += effectiveAlteration;
    }

    /**
     * @brief Simplifies the spelling by reducing its alteration while preserving pitch.
     *
     * This is typically used after a chromatic or enharmonic transposition to improve a note’s readability.
     * It performs enharmonic transpositions in the appropriate direction until the absolute alteration is minimized.
     */
    void simplifySpelling()
    {
        while (std::abs(m_alteration) > 0) {
            const int currSign = sign(m_alteration);
            const int currAbsDisp = std::abs(m_alteration);    
            enharmonicTranspose(currSign);
            if (std::abs(m_alteration) >= currAbsDisp) {
                enharmonicTranspose(-currSign);
                return;
            }
            if (currSign != sign(m_alteration)) {
                break;
            }
        }
    }

    /**
     * @brief Transposes by the given number of EDO steps and simplifies the spelling.
     *
     * In standard key signatures, each step corresponds to a half-step. In microtonal systems, each step
     * is a single division of the octave value.
     *
     * After transposition, the note's spelling is simplified to reduce the alteration while preserving pitch.
     *
     * @param numberOfSteps The number of diatonic steps to transpose (positive for up, negative for down).
     */
    void stepwiseTranspose(int numberOfSteps)
    {
        m_alteration += numberOfSteps;
        simplifySpelling();
    }

    /**
     * @brief Determines if the given displacement and alteration refer to the same pitch as the current state.
     *
     * This compares the total EDO step position derived from the current displacement and alteration
     * with that of the input values. It accounts for microtonal tuning via the custom key map and EDO size.
     *
     * @param displacement The scale step displacement to compare.
     * @param alteration   The chromatic alteration to compare.
     * @return true if the given values are enharmonically equivalent to the current values.
     */
    bool isEnharmonicEquivalent(int displacement, int alteration) const {
        return calcAbsoluteStep(displacement, alteration) == calcAbsoluteStep(m_displacement, m_alteration);
    }
    
private:
    int calcFifthSteps() const
    {
        // std::log(3.0 / 2.0) / std::log(2.0) is 0.5849625007211562.
        static constexpr double kFifthsMultiplier = 0.5849625007211562;
        return static_cast<int>(std::floor((double(m_keyMap.size()) * kFifthsMultiplier) + 0.5));
    }
    
    int calcScaleDegree(int interval) const
    {
        int intervalNormalized = signedModulus(interval, int(m_keyMap.size()));
        if (intervalNormalized < 0) {
            intervalNormalized += int(m_keyMap.size());
        }
        return intervalNormalized;
    }

    int calcStepsBetweenScaleDegrees(int firstDisplacement, int secondDisplacement) const
    {
        const int fifthSteps = calcFifthSteps();
        const int firstScaleDegree = calcScaleDegree(firstDisplacement);
        const int secondScaleDegree = calcScaleDegree(secondDisplacement);
        int result = sign(secondDisplacement - firstDisplacement) * (m_keyMap[secondScaleDegree] - m_keyMap[firstScaleDegree]);
        if (result < 0) {
            result += m_numberOfSteps;
        }
    }

    int calcStepsInAlteration(int interval, int alteration) const
    {
        const int fifthSteps = calcFifthSteps();
        const int plusFifths = sign(interval) * alteration * 7;     // number of fifths to add for a chromatic halfstep alteration (in any EDO)
        const int minusOctaves = sign(interval) * alteration * -4;  // number of octaves to subtract for a chromatic halfstep alteration (in any EDO)
        const int result = sign(interval) * ((plusFifths * fifthSteps) + (minusOctaves * m_numberOfSteps));
        return result;
    }

    int calcStepsInNormalizedInterval(int intervalNormalized) const
    {
        const int fifthSteps = calcFifthSteps();
        const int index = std::abs(intervalNormalized);
        const int plusFifths = DIATONIC_INTERVAL_ADJUSTMENTS[index][0];    // number of fifths
        const int minusOctaves = DIATONIC_INTERVAL_ADJUSTMENTS[index][1];  // number of octaves

        return sign(intervalNormalized) * ((plusFifths * fifthSteps) + (minusOctaves * m_numberOfSteps));
    }

    int calcAbsoluteStep(int displacement, int alteration) const {
        const int scaleDegree = calcScaleDegree(displacement); // 0..6
        const int baseStep = m_keyMap[scaleDegree];
    
        const int octaveSteps = (displacement / STANDARD_DIATONIC_STEPS) * m_numberOfSteps;
        const int chromaticSteps = calcStepsInAlteration(/*interval=*/+1, alteration);
    
        return baseStep + chromaticSteps + octaveSteps;
    }
};

} // namespace music_theory