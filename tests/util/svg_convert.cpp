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
#include <regex>
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

struct PathInfo
{
    double strokeWidth{};
    double scaleX{1.0};
    double scaleY{1.0};
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

std::vector<PathInfo> parsePathInfo(const std::string& svg)
{
    std::vector<PathInfo> paths;
    std::regex pathRegex("<path[^>]*>");
    std::regex strokeRegex("stroke-width=\\\"([^\\\"]+)\\\"");
    std::regex transformRegex("transform=\\\"matrix\\(([^\\\"]+)\\)\\\"");

    for (std::sregex_iterator it(svg.begin(), svg.end(), pathRegex), end; it != end; ++it) {
        const std::string tag = it->str();
        std::smatch strokeMatch;
        if (!std::regex_search(tag, strokeMatch, strokeRegex)) {
            continue;
        }
        PathInfo info;
        info.strokeWidth = std::stod(strokeMatch[1].str());
        std::smatch transformMatch;
        if (std::regex_search(tag, transformMatch, transformRegex)) {
            std::istringstream input(transformMatch[1].str());
            double a{}, b{}, c{}, d{};
            if (input >> a >> b >> c >> d) {
                info.scaleX = std::hypot(a, b);
                info.scaleY = std::hypot(c, d);
            }
        }
        info.valid = true;
        paths.push_back(info);
    }
    return paths;
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
    constexpr double kExactTolerance = 0.001;
    constexpr double kStrokeTolerance = 0.05;
    int checked = 0;

    for (int shapeId : shapeIds) {
        auto shape = doc->getOthers()->get<musx::dom::others::ShapeDef>(musx::dom::SCORE_PARTID, shapeId);
        ASSERT_TRUE(shape) << "Missing ShapeDef " << shapeId;

        if (shapeId == 8) {
            std::cout << "Cmper: " << shape->getCmper() << std::endl;
            std::cout << "instList: " << shape->instructionList << std::endl;
            std::cout << "dataList: " << shape->dataList << std::endl;
            shape->iterateInstructions([&](const musx::dom::ShapeDefInstruction::Decoded& info) {
                std::cout << "Inst " << int(info.type) << std::endl;
                /*
                std::cout << "Inst " << int(info.type) << ": [";
                for (size_t x = 0; x < data.size(); x++) {
                    if (x > 0) {
                        std::cout << ", ";
                    }
                    std::cout << data[x];
                }
                std::cout << std::endl;
                */
                return true;
            });
        }

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

        EXPECT_NEAR(ourBox.minX, refBox.minX, kExactTolerance) << "minX mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.minY, refBox.minY, kExactTolerance) << "minY mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.width, refBox.width, kExactTolerance) << "width mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.height, refBox.height, kExactTolerance) << "height mismatch for ShapeDef " << shapeId;

        if (shapeId == 8) {
            std::vector<PathInfo> refPaths = parsePathInfo(referenceSvg);
            std::vector<PathInfo> ourPaths = parsePathInfo(ourSvg);
            ASSERT_FALSE(refPaths.empty()) << "No reference paths parsed for ShapeDef 8";
            ASSERT_EQ(refPaths.size(), ourPaths.size()) << "Path count mismatch for ShapeDef 8";
            for (size_t i = 0; i < refPaths.size(); ++i) {
                ASSERT_TRUE(refPaths[i].valid);
                ASSERT_TRUE(ourPaths[i].valid);
                double refEffective = refPaths[i].strokeWidth * 0.5 * (refPaths[i].scaleX + refPaths[i].scaleY);
                double ourEffective = ourPaths[i].strokeWidth * 0.5 * (ourPaths[i].scaleX + ourPaths[i].scaleY);
                EXPECT_NEAR(ourEffective, refEffective, kStrokeTolerance)
                    << "effective stroke width mismatch for ShapeDef 8 path " << i;
            }
        }

        EXPECT_GT(countTag(ourSvg, "path"), 0) << "No paths in generated SVG " << shapeId;
        EXPECT_GT(countTag(referenceSvg, "path"), 0) << "No paths in reference SVG " << shapeId;

        ++checked;
    }

    EXPECT_EQ(checked, kTargetSamples) << "Insufficient path-only SVG samples for PattersonDefault";

    if (std::getenv("MUSX_KEEP_SVG_OUTPUT") == nullptr) {
        std::filesystem::remove_all(svgOut, ec);
        ASSERT_FALSE(ec) << "Failed to remove directory: " << svgOut;
        ASSERT_FALSE(std::filesystem::exists(svgOut));
    }
}

} // namespace musxtest
