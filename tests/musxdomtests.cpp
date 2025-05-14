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

#include "gtest/gtest.h"
#include "test_utils.h"

namespace musxtest {

void readFile(const std::filesystem::path& filePath, std::vector<char>& contents)
{
    ASSERT_TRUE(std::filesystem::is_regular_file(filePath)) << pathString(filePath) << " is not a file";
    std::ifstream file(filePath, std::ios::binary);
    ASSERT_TRUE(file.is_open()) << "failed to open file: " << pathString(filePath);
    contents = std::vector<char>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
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
    ::testing::InitGoogleTest(&argc, argv);
    ::testing::AddGlobalTestEnvironment(new TestEnvironment);
    return RUN_ALL_TESTS();
}
