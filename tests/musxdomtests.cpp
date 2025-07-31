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
#include <filesystem>
#include <fstream>
#include <clocale>

#include "gtest/gtest.h"
#include "test_utils.h"

#include "musx/musx.h"

namespace musxtest {

using namespace ::musx::dom;

void readFile(const std::filesystem::path& filePath, std::vector<char>& contents)
{
    ASSERT_TRUE(std::filesystem::is_regular_file(filePath)) << pathString(filePath) << " is not a file";
    std::ifstream file(filePath, std::ios::binary);
    ASSERT_TRUE(file.is_open()) << "failed to open file: " << pathString(filePath);
    contents = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void staffListCheck(std::string_view staffListName, const std::shared_ptr<others::StaffList>& staffList, std::vector<int> expectedValues)
{
    ASSERT_TRUE(staffList) << staffListName << ": StaffList was null";
    ASSERT_EQ(staffList->values.size(), expectedValues.size()) << staffListName << ": StaffList size " << staffList->values.size()
        << " does not match expected values size " << expectedValues.size();
    for (size_t x = 0; x < expectedValues.size(); x++) {
        EXPECT_EQ(staffList->values[x], InstCmper(expectedValues[x])) << staffListName << ": StaffList element " << x << " does not match expected value.";
    }
}

} // namespace musxtest

// Optional setup/teardown for test suite
class TestEnvironment : public ::testing::Environment {
public:
    void SetUp() override {
        // Code to run before all tests
    }

    void TearDown() override {
        // Code to run after all tests
    }
};

int main(int argc, char **argv) {
    setlocale(LC_ALL, "");

    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnvironment);
    ::musx::util::TestConfiguration::setTestDataPath(std::filesystem::current_path());
    int result = RUN_ALL_TESTS();
    const auto endMessages = musxtest::g_endMessages.str();
    if (!endMessages.empty()) {
        std::cout << std::endl << endMessages << std::endl;
    }
    return result;
}
