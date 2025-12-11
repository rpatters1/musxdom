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
#include <limits>
#include <unordered_map>

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::util;

TEST(Fraction, HashTest)
{
    std::unordered_map<Fraction, int> fractionMap;

    fractionMap.emplace(Fraction(1, 2), 1);
    fractionMap.emplace(Fraction(3, 4), 2);

    EXPECT_FALSE(fractionMap.contains(Fraction(5, 6)));
    EXPECT_TRUE(fractionMap.contains(Fraction(2, 4)));
    EXPECT_TRUE(fractionMap.contains(Fraction(6, 8)));
    EXPECT_EQ(fractionMap[Fraction(1, 2)], 1);
    EXPECT_EQ(fractionMap[Fraction(3, 4)], 2);
}

TEST(Fraction, MaxMin)
{
    static constexpr auto maxFrac = (std::numeric_limits<Fraction>::max)();
    static constexpr auto maxEdu = (std::numeric_limits<musx::dom::Edu>::max)();
    EXPECT_EQ(maxFrac.calcEduDuration(), maxEdu);
    auto maxEduFrac = Fraction::fromEdu(maxEdu);
    EXPECT_EQ(maxEduFrac.calcEduDuration(), maxEdu);
}
