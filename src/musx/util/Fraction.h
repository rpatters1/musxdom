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

#include <iostream>
#include <numeric>
#include <stdexcept>

namespace musx {
namespace util {

/**
 * @class Fraction
 * @brief A class to represent fractions with integer m_numerator and m_denominator, automatically reduced to simplest form.
 */
class Fraction {
private:
    int m_numerator; ///< The m_numerator of the fraction.
    int m_denominator; ///< The m_denominator of the fraction.

    /**
     * @brief Reduces the fraction to its simplest form.
     * Ensures the m_denominator is always positive.
     */
    void reduce() {
        int gcd = std::gcd(m_numerator, m_denominator);
        m_numerator /= gcd;
        m_denominator /= gcd;

        // Ensure the m_denominator is always positive
        if (m_denominator < 0) {
            m_numerator = -m_numerator;
            m_denominator = -m_denominator;
        }
    }

public:
    /**
     * @brief Constructs a Fraction object.
     * @param num The m_numerator of the fraction.
     * @param den The m_denominator of the fraction. Defaults to 1.
     * @throws std::invalid_argument if the m_denominator is zero.
     */
    Fraction(int num = 0, int den = 1) : m_numerator(num), m_denominator(den) {
        if (m_denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero.");
        }
        reduce();
    }

    /// @brief Constructs a Fraction from edu
    /// @param edu
    static Fraction fromEdu(int edu);

    /**
     * @brief Gets the m_numerator of the fraction.
     * @return The m_numerator.
     */
    int numerator() const { return m_numerator; }

    /**
     * @brief Gets the m_denominator of the fraction.
     * @return The m_denominator.
     */
    int denominator() const { return m_denominator; }

    /**
     * @brief Returns the integer (whole number) part of the fraction.
     * @return The integer part of the fraction.
     */
    int quotient() const {
        return m_numerator / m_denominator;
    }

    /**
     * @brief Returns the fractional part of the fraction.
     * @return The remainder as a fraction, satisfying -1 < remainder < 1.
     */
    Fraction remainder() const {
        return Fraction(m_numerator % m_denominator, m_denominator);
    }

    /**
     * @brief Calculates duration as a fraction of a whole note
     */
    // use `double` rather than `EduFloat` to manage circular header dependency
    double calcEduDuration() const;

    /**
     * @brief Adds two fractions.
     * @param other The other fraction to add.
     * @return The resulting fraction after addition.
     */
    Fraction operator+(const Fraction& other) const {
        return Fraction(
            m_numerator * other.m_denominator + other.m_numerator * m_denominator,
            m_denominator * other.m_denominator
        );
    }

    /**
     * @brief Subtracts one fraction from another.
     * @param other The other fraction to subtract.
     * @return The resulting fraction after subtraction.
     */
    Fraction operator-(const Fraction& other) const {
        return Fraction(
            m_numerator * other.m_denominator - other.m_numerator * m_denominator,
            m_denominator * other.m_denominator
        );
    }

    /**
     * @brief Multiplies two fractions.
     * @param other The other fraction to multiply.
     * @return The resulting fraction after multiplication.
     */
    Fraction operator*(const Fraction& other) const {
        return Fraction(
            m_numerator * other.m_numerator,
            m_denominator * other.m_denominator
        );
    }

    /**
     * @brief Divides one fraction by another.
     * @param other The other fraction to divide by.
     * @return The resulting fraction after division.
     * @throws std::invalid_argument if attempting to divide by a fraction with a zero m_numerator.
     */
    Fraction operator/(const Fraction& other) const {
        if (other.m_numerator == 0) {
            throw std::invalid_argument("Cannot divide by zero fraction.");
        }
        return Fraction(
            m_numerator * other.m_denominator,
            m_denominator * other.m_numerator
        );
    }

    /**
     * @brief Compound addition assignment operator.
     * @param other The other fraction to add.
     * @return A reference to the updated fraction.
     */
    Fraction& operator+=(const Fraction& other) {
        *this = *this + other;
        return *this;
    }

    /**
     * @brief Compound subtraction assignment operator.
     * @param other The other fraction to subtract.
     * @return A reference to the updated fraction.
     */
    Fraction& operator-=(const Fraction& other) {
        *this = *this - other;
        return *this;
    }

    /**
     * @brief Compound multiplication assignment operator.
     * @param other The other fraction to multiply.
     * @return A reference to the updated fraction.
     */
    Fraction& operator*=(const Fraction& other) {
        *this = *this * other;
        return *this;
    }

    /**
     * @brief Compound division assignment operator.
     * @param other The other fraction to divide by.
     * @return A reference to the updated fraction.
     * @throws std::invalid_argument if attempting to divide by a fraction with a zero m_numerator.
     */
    Fraction& operator/=(const Fraction& other) {
        *this = *this / other;
        return *this;
    }

    /**
     * @brief Equality comparison operator.
     * @param other The other fraction to compare.
     * @return True if the fractions are equal, false otherwise.
     */
    bool operator==(const Fraction& other) const {
        Fraction lhs = *this;
        Fraction rhs = other;
        lhs.reduce();
        rhs.reduce();
        return lhs.m_numerator == rhs.m_numerator && lhs.m_denominator == rhs.m_denominator;
    }

    /**
     * @brief Inequality comparison operator.
     * @param other The other fraction to compare.
     * @return True if the fractions are not equal, false otherwise.
     */
    bool operator!=(const Fraction& other) const {
        return !(*this == other);
    }

    /**
     * @brief Less-than comparison operator.
     * @param other The other fraction to compare.
     * @return True if this fraction is less than the other, false otherwise.
     */
    bool operator<(const Fraction& other) const {
        double lhs = static_cast<double>(m_numerator) / m_denominator;
        double rhs = static_cast<double>(other.m_numerator) / other.m_denominator;
        return lhs < rhs;
    }

    /**
     * @brief Less-than-or-equal-to comparison operator.
     * @param other The other fraction to compare.
     * @return True if this fraction is less than or equal to the other, false otherwise.
     */
    bool operator<=(const Fraction& other) const {
        return *this < other || *this == other;
    }

    /**
     * @brief Greater-than comparison operator.
     * @param other The other fraction to compare.
     * @return True if this fraction is greater than the other, false otherwise.
     */
    bool operator>(const Fraction& other) const {
        return !(*this <= other);
    }

    /**
     * @brief Greater-than-or-equal-to comparison operator.
     * @param other The other fraction to compare.
     * @return True if this fraction is greater than or equal to the other, false otherwise.
     */
    bool operator>=(const Fraction& other) const {
        return !(*this < other);
    }

    /**
     * @brief Checks if the fraction is nonzero.
     * @return True if the fraction is nonzero, false otherwise.
     */
    explicit operator bool() const {
        return m_numerator != 0;
    }

    /**
     * @brief Stream output operator.
     * @param os The output stream.
     * @param frac The fraction to output.
     * @return A reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& os, const Fraction& frac) {
        os << frac.m_numerator;
        if (frac.m_denominator != 1) {
            os << "/" << frac.m_denominator;
        }
        return os;
    }

    /**
     * @brief Stream input operator.
     * @param is The input stream.
     * @param frac The fraction to populate.
     * @return A reference to the input stream.
     * @throws std::invalid_argument if the input format is invalid or the m_denominator is zero.
     */
    friend std::istream& operator>>(std::istream& is, Fraction& frac) {
        int num, den;
        char sep;
        is >> num >> sep >> den;
        if (sep != '/' || den == 0) {
            throw std::invalid_argument("Invalid fraction format or zero m_denominator.");
        }
        frac = Fraction(num, den);
        return is;
    }
};

} // namespace util
} // namespace musx
