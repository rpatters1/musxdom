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
#include <algorithm>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "test_utils.h"

namespace musxtest {
namespace {

struct ViewBox
{
    double minX{};
    double minY{};
    double width{};
    double height{};
    bool valid{};
};

std::string readFileText(const std::filesystem::path& path)
{
    std::vector<char> contents;
    readFile(path, contents);
    return std::string(contents.begin(), contents.end());
}

ViewBox parseViewBox(const std::string& svg)
{
    ViewBox result;
    const std::string key = "viewBox=\"";
    auto pos = svg.find(key);
    if (pos == std::string::npos) {
        return result;
    }
    pos += key.size();
    auto end = svg.find('"', pos);
    if (end == std::string::npos) {
        return result;
    }
    std::istringstream input(svg.substr(pos, end - pos));
    if (input >> result.minX >> result.minY >> result.width >> result.height) {
        result.valid = true;
    }
    return result;
}

int countTag(const std::string& svg, const std::string& tag)
{
    int count = 0;
    std::string needle = "<" + tag;
    std::string::size_type pos = 0;
    while ((pos = svg.find(needle, pos)) != std::string::npos) {
        ++count;
        pos += needle.size();
    }
    return count;
}

bool containsAny(const std::string& svg, const std::vector<std::string>& needles)
{
    for (const auto& needle : needles) {
        if (svg.find(needle) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool viewBoxEncloses(const ViewBox& outer, const ViewBox& inner, double tolerance)
{
    double outerMaxX = outer.minX + outer.width;
    double outerMaxY = outer.minY + outer.height;
    double innerMaxX = inner.minX + inner.width;
    double innerMaxY = inner.minY + inner.height;
    return outer.minX <= inner.minX + tolerance
        && outer.minY <= inner.minY + tolerance
        && outerMaxX >= innerMaxX - tolerance
        && outerMaxY >= innerMaxY - tolerance;
}

} // namespace

TEST(SvgConvertTest, PattersonDefaultMatchesReferenceViewBox)
{
    const auto inputRoot = getInputPath() / "reference";
    const auto xmlPath = inputRoot / "PattersonDefault.enigmaxml";
    std::vector<char> enigmaXml;
    readFile(xmlPath, enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    const auto svgRoot = inputRoot / "PattersonDefault";
    ASSERT_TRUE(std::filesystem::is_directory(svgRoot));

    const auto svgOut = getOutputPath() / "PattersonDefault";
    std::error_code ec;
    std::filesystem::create_directories(svgOut, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << svgOut;
    ASSERT_TRUE(std::filesystem::is_directory(svgOut));

    std::vector<int> shapeIds;
    for (const auto& entry : std::filesystem::directory_iterator(svgRoot)) {
        if (!entry.is_regular_file()) {
            continue;
        }
        if (entry.path().extension() != ".svg") {
            continue;
        }
        const auto stem = entry.path().stem().string();
        if (!stringHasDigit(stem)) {
            continue;
        }
        shapeIds.push_back(std::stoi(stem));
    }

    std::sort(shapeIds.begin(), shapeIds.end());

    constexpr int kTargetSamples = 5;
    constexpr double kTolerance = 0.5;
    int checked = 0;

    for (int shapeId : shapeIds) {
        auto shape = doc->getOthers()->get<musx::dom::others::ShapeDef>(musx::dom::SCORE_PARTID, shapeId);
        ASSERT_TRUE(shape) << "Missing ShapeDef " << shapeId;

        const std::string ourSvg = musx::util::SvgConvert::toSvg(shape);
        ASSERT_FALSE(ourSvg.empty()) << "Empty SVG for ShapeDef " << shapeId;

        const auto outPath = svgOut / (std::to_string(shapeId) + ".svg");
        std::ofstream out(outPath, std::ios::binary);
        ASSERT_TRUE(out.is_open()) << "Failed to open file for writing: " << outPath;
        out << ourSvg;
        out.close();

        if (checked >= kTargetSamples) {
            break;
        }
        const auto refPath = svgRoot / (std::to_string(shapeId) + ".svg");
        const std::string referenceSvg = readFileText(refPath);
        if (referenceSvg.empty()) {
            continue;
        }
        if (containsAny(referenceSvg, {"<text", "<image", "<font"})) {
            continue;
        }

        ViewBox refBox = parseViewBox(referenceSvg);
        ViewBox ourBox = parseViewBox(ourSvg);
        ASSERT_TRUE(refBox.valid) << "Missing viewBox in reference SVG " << shapeId;
        ASSERT_TRUE(ourBox.valid) << "Missing viewBox in generated SVG " << shapeId;

        EXPECT_TRUE(viewBoxEncloses(ourBox, refBox, kTolerance))
            << "viewBox mismatch for ShapeDef " << shapeId
            << " ours=(" << ourBox.minX << ", " << ourBox.minY << ", "
            << ourBox.width << ", " << ourBox.height << ")"
            << " ref=(" << refBox.minX << ", " << refBox.minY << ", "
            << refBox.width << ", " << refBox.height << ")";

        EXPECT_GT(countTag(ourSvg, "path"), 0) << "No paths in generated SVG " << shapeId;
        EXPECT_GT(countTag(referenceSvg, "path"), 0) << "No paths in reference SVG " << shapeId;

        ++checked;
    }

    EXPECT_EQ(checked, kTargetSamples) << "Insufficient path-only SVG samples for PattersonDefault";

    std::filesystem::remove_all(svgOut, ec);
    ASSERT_FALSE(ec) << "Failed to remove directory: " << svgOut;
    ASSERT_FALSE(std::filesystem::exists(svgOut));
}

} // namespace musxtest
