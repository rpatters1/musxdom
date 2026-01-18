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

#include <string_view>
#include <vector>
#include <filesystem>
#include <iostream>

#include "musx/musx.h"

namespace musx::dom::others {
class StaffList;
}

namespace musxtest {

inline std::ostringstream g_endMessages;

inline std::string pathString(const std::filesystem::path& path)
{
    auto s = path.u8string();
    return std::string(reinterpret_cast<const char*>(s.data()), s.size());
}

class string_view : public std::string_view
{
public:
    using std::string_view::string_view;

    operator std::vector<char>() const
    {
        return std::vector<char>(begin(), end());
    }
};

inline std::filesystem::path getInputPath()
{ return std::filesystem::current_path(); }

inline std::filesystem::path getOutputPath()
{ return std::filesystem::current_path() / "output"; }

// STOOPID Google test can't ASSERT out of a non-void function
void readFile(const std::filesystem::path& filePath, std::vector<char>& contents);

inline bool stringHasDigit(const std::string& s)
{
    return std::find_if(s.begin(), s.end(),
                        [](unsigned char c){ return std::isdigit(c); })
           != s.end();
}

void staffListCheck(std::string_view staffListName, const musx::dom::MusxInstance<musx::dom::others::StaffList>& staffList, std::vector<int> expectedValues);

} // namespace musxtext