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
#include <fstream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>

#include "gtest/gtest.h"
#include "test_utils.h"

using namespace musx::dom;

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

struct TextInfo
{
    double x{};
    double y{};
    std::string text;
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
    std::regex pathRegex("<(?:path|rect|ellipse)[^>]*>");
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

std::vector<TextInfo> parseTextInfo(const std::string& svg)
{
    std::vector<TextInfo> texts;
    std::regex textRegex("<text[^>]*transform=\\\"translate\\(([^,]+),([^\\)]+)\\)\\\"[^>]*>([\\s\\S]*?)</text>");

    for (std::sregex_iterator it(svg.begin(), svg.end(), textRegex), end; it != end; ++it) {
        TextInfo info;
        info.x = std::stod((*it)[1].str());
        info.y = std::stod((*it)[2].str());
        info.text = (*it)[3].str();
        texts.push_back(info);
    }
    return texts;
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

bool keepSvgOutput()
{
#ifdef _WIN32
    char* buf = nullptr;
    size_t len = 0;
    if (_dupenv_s(&buf, &len, "MUSX_KEEP_SVG_OUTPUT") != 0 || !buf) {
        return false;
    }
    free(buf);
    return true;
#else
    return std::getenv("MUSX_KEEP_SVG_OUTPUT") != nullptr;
#endif
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

using SvgBuilder = std::function<std::string(const others::ShapeDef&)>;

void runSvgReferenceComparison(const std::filesystem::path& xmlPath,
                               const std::filesystem::path& svgRoot,
                               const std::filesystem::path& svgOut,
                               const SvgBuilder& makeSvg)
{
    std::vector<char> enigmaXml;
    readFile(xmlPath, enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    ASSERT_TRUE(std::filesystem::is_directory(svgRoot));
    std::error_code ec;
    std::filesystem::create_directories(svgOut, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << svgOut;
    ASSERT_TRUE(std::filesystem::is_directory(svgOut));

    std::vector<Cmper> shapeIds;
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
        shapeIds.push_back(static_cast<Cmper>(std::stoi(stem)));
    }

    std::sort(shapeIds.begin(), shapeIds.end());

    constexpr double kTolerance = 1.9;
    constexpr double kExactTolerance = 1.9;
    constexpr double kStrokeTolerance = 0.05;

    for (Cmper shapeId : shapeIds) {
        auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, shapeId);
        ASSERT_TRUE(shape) << "Missing ShapeDef " << shapeId;

        const std::string ourSvg = makeSvg(*shape);
        ASSERT_FALSE(ourSvg.empty()) << "Empty SVG for ShapeDef " << shapeId;

        const auto outPath = svgOut / (std::to_string(shapeId) + "_test.svg");
        std::ofstream out(outPath, std::ios::binary);
        ASSERT_TRUE(out.is_open()) << "Failed to open file for writing: " << outPath;
        out << ourSvg;
        out.close();

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

        double tolerance = kTolerance;
        double exactTolerance = kExactTolerance;
        if (shapeId == 128) {
            tolerance = 5.0;
            exactTolerance = 5.0;
        }

        EXPECT_TRUE(viewBoxEncloses(ourBox, refBox, tolerance))
            << "viewBox mismatch for ShapeDef " << shapeId
            << " ours=(" << ourBox.minX << ", " << ourBox.minY << ", "
            << ourBox.width << ", " << ourBox.height << ")"
            << " ref=(" << refBox.minX << ", " << refBox.minY << ", "
            << refBox.width << ", " << refBox.height << ")";

        EXPECT_NEAR(ourBox.minX, refBox.minX, exactTolerance) << "minX mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.minY, refBox.minY, exactTolerance) << "minY mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.width, refBox.width, exactTolerance) << "width mismatch for ShapeDef " << shapeId;
        EXPECT_NEAR(ourBox.height, refBox.height, exactTolerance) << "height mismatch for ShapeDef " << shapeId;

        std::vector<PathInfo> refPaths = parsePathInfo(referenceSvg);
        std::vector<PathInfo> ourPaths = parsePathInfo(ourSvg);
        EXPECT_EQ(refPaths.size(), ourPaths.size()) << "Path count mismatch for ShapeDef " << shapeId;
        if (refPaths.size() == ourPaths.size()) {
            for (size_t i = 0; i < refPaths.size(); ++i) {
                ASSERT_TRUE(refPaths[i].valid);
                ASSERT_TRUE(ourPaths[i].valid);
                double refEffective = refPaths[i].strokeWidth * 0.5 * (refPaths[i].scaleX + refPaths[i].scaleY);
                double ourEffective = ourPaths[i].strokeWidth * 0.5 * (ourPaths[i].scaleX + ourPaths[i].scaleY);
                EXPECT_NEAR(ourEffective, refEffective, kStrokeTolerance)
                    << "effective stroke width mismatch for ShapeDef " << shapeId << " path " << i;
            }
        }

        int ourDrawable = countTag(ourSvg, "path") + countTag(ourSvg, "ellipse");
        int refDrawable = countTag(referenceSvg, "path") + countTag(referenceSvg, "ellipse");
        EXPECT_GT(ourDrawable, 0) << "No drawable elements in generated SVG " << shapeId;
        EXPECT_GT(refDrawable, 0) << "No drawable elements in reference SVG " << shapeId;
    }

    if (!keepSvgOutput()) {
        std::filesystem::remove_all(svgOut, ec);
        ASSERT_FALSE(ec) << "Failed to remove directory: " << svgOut;
        ASSERT_FALSE(std::filesystem::exists(svgOut));
    }
}

void runReverseBulgeTest(const std::filesystem::path& xmlPath,
                         const std::filesystem::path& svgRoot,
                         const std::filesystem::path& svgOut,
                         const SvgBuilder& makeSvg)
{
    if (!std::filesystem::is_regular_file(xmlPath)) {
        GTEST_SKIP() << "Missing reverse bulge test data: " << xmlPath;
    }

    std::vector<Cmper> shapeIds = {5, 6, 7, 8};
    std::vector<char> enigmaXml;
    readFile(xmlPath, enigmaXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    std::error_code ec;
    std::filesystem::create_directories(svgOut, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << svgOut;

    std::vector<std::filesystem::path> outputs;
    for (Cmper shapeId : shapeIds) {
        auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, shapeId);
        ASSERT_TRUE(shape) << "Missing ShapeDef " << shapeId;

        const std::string ourSvg = makeSvg(*shape);
        ASSERT_FALSE(ourSvg.empty()) << "Empty SVG for ShapeDef " << shapeId;

        const auto outPath = svgOut / (std::to_string(shapeId) + "_test.svg");
        std::ofstream out(outPath, std::ios::binary);
        ASSERT_TRUE(out.is_open()) << "Failed to open file for writing: " << outPath;
        out << ourSvg;
        out.close();
        outputs.push_back(outPath);

        const auto refPath = svgRoot / (std::to_string(shapeId) + ".svg");
        if (!std::filesystem::is_regular_file(refPath)) {
            continue;
        }

        const std::string referenceSvg = readFileText(refPath);
        if (referenceSvg.empty()) {
            continue;
        }

        ViewBox refBox = parseViewBox(referenceSvg);
        ViewBox ourBox = parseViewBox(ourSvg);
        ASSERT_TRUE(refBox.valid) << "Missing viewBox in reference SVG " << shapeId;
        ASSERT_TRUE(ourBox.valid) << "Missing viewBox in generated SVG " << shapeId;

        constexpr double kTolerance = 1.9;
        EXPECT_TRUE(viewBoxEncloses(ourBox, refBox, kTolerance))
            << "reverse bulge viewBox mismatch for ShapeDef " << shapeId;
    }

    if (!keepSvgOutput()) {
        for (const auto& outPath : outputs) {
            std::filesystem::remove(outPath, ec);
            ASSERT_FALSE(ec) << "Failed to remove file: " << outPath;
        }
    }
}

} // namespace

TEST(SvgConvertTest, MatchesViewBoxAndPathsAndStrokes)
{
    const auto inputRoot = getInputPath() / "reference";
    const auto xmlPath = inputRoot / "PattersonDefault.enigmaxml";
    const auto svgRoot = inputRoot / "PattersonDefault";
    const auto svgOut = getOutputPath() / "PattersonDefault";
    runSvgReferenceComparison(xmlPath, svgRoot, svgOut, [](const others::ShapeDef& shape) {
        return musx::util::SvgConvert::toSvg(shape);
    });
}

TEST(SvgConvertTest, ReverseBulgeShapes)
{
    const auto inputRoot = getInputPath();
    const auto xmlPath = inputRoot / "shapecurve_dragged.enigmaxml";
    const auto svgRoot = inputRoot / "shapecurve_dragged";
    const auto svgOut = getOutputPath() / "shapecurve_dragged";
    runReverseBulgeTest(xmlPath, svgRoot, svgOut, [](const others::ShapeDef& shape) {
        return musx::util::SvgConvert::toSvg(shape);
    });
}

TEST(SvgConvertTest, MatchesViewBoxAndPathsAndStrokesPageScaled)
{
    const auto inputRoot = getInputPath() / "reference";
    const auto xmlPath = inputRoot / "PattersonDefault.enigmaxml";
    const auto svgRoot = inputRoot / "PattersonDefault_page_scaled";
    const auto svgOut = getOutputPath() / "PattersonDefault_page_scaled";
    runSvgReferenceComparison(xmlPath, svgRoot, svgOut, [](const others::ShapeDef& shape) {
        return musx::util::SvgConvert::toSvgWithPageFormatScaling(
            shape, musx::util::SvgConvert::SvgUnit::Millimeters);
    });
}

TEST(SvgConvertTest, ReverseBulgeShapesPageScaled)
{
    const auto inputRoot = getInputPath();
    const auto xmlPath = inputRoot / "shapecurve_dragged.enigmaxml";
    const auto svgRoot = inputRoot / "shapecurve_dragged_page_scaled";
    const auto svgOut = getOutputPath() / "shapecurve_dragged_page_scaled";
    runReverseBulgeTest(xmlPath, svgRoot, svgOut, [](const others::ShapeDef& shape) {
        return musx::util::SvgConvert::toSvgWithPageFormatScaling(
            shape, musx::util::SvgConvert::SvgUnit::Millimeters);
    });
}

TEST(SvgConvertTest, TextMetricsMatches)
{
    const auto inputRoot = getInputPath() / "reference";
    const auto xmlPath = inputRoot / "PattersonDefault.enigmaxml";
    std::vector<char> enigmaXml;
    readFile(xmlPath, enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    const auto svgRoot = inputRoot / "PattersonDefault";

    auto metrics = [](const FontInfo& font,
                      std::u32string_view text) -> std::optional<musx::util::SvgConvert::GlyphMetrics> {
        if (text.empty()) {
            return std::nullopt;
        }
        std::string name;
        try {
            name = font.getName();
        } catch (...) {
            name.clear();
        }
        bool isTimesNewRoman = name.find("Times New Roman") != std::string::npos;
        bool isHelvetica = name.find("Helvetica") != std::string::npos;
        bool isArial = name.find("Arial") != std::string::npos;
        bool isTimes = !isTimesNewRoman && name.find("Times") != std::string::npos;
        bool isFinaleMaestro = name.find("Finale Maestro") != std::string::npos;
        EXPECT_TRUE(isHelvetica || isArial || isTimes || isTimesNewRoman || isFinaleMaestro)
            << "Unexpected font: " << name;
        if (isHelvetica || isTimes) {
            EXPECT_TRUE(font.fontSize == 12) << "Unexpected font size: " << font.fontSize;
        } else if (isArial || isTimesNewRoman) {
            EXPECT_TRUE(font.fontSize == 9) << "Unexpected font size: " << font.fontSize;
        } else if (isFinaleMaestro) {
            EXPECT_TRUE(font.fontSize == 24) << "Unexpected font size: " << font.fontSize;
        }
        if (isHelvetica || isArial) {
            EXPECT_TRUE(font.bold) << "Expected bold font";
            EXPECT_FALSE(font.italic) << "Expected non-italic font";
        }
        if (isTimes || isTimesNewRoman) {
            EXPECT_FALSE(font.bold) << "Expected non-bold font";
            EXPECT_FALSE(font.italic) << "Expected non-italic font";
        }
        if (isHelvetica) {
            EXPECT_TRUE(font.bold) << "Expected Helvetica bold";
            EXPECT_FALSE(font.italic) << "Expected Helvetica non-italic";
        }
        musx::util::SvgConvert::GlyphMetrics result;

        char32_t codePoint = text.front();
        if (isFinaleMaestro) {
            switch (codePoint) {
            case U'\uE566':
                result.advance = 46.463989257812;
                result.ascent = 36.578125;
                result.descent = 0.296875;
                break;
            case U'\uF427':
                result.advance = 105.609375;
                result.ascent = 193.0;
                result.descent = 193.0;
                break;
            default:
                ADD_FAILURE() << "Unexpected Finale Maestro glyph: " << static_cast<std::uint32_t>(codePoint);
                result.advance = 0.0;
                result.ascent = 0.0;
                result.descent = 0.0;
                break;
            }
        } else if (isHelvetica) {
            switch (codePoint) {
            case U'F': result.advance = 29.3203125; break;
            case U'O': result.advance = 37.3359375; break;
            case U'R': result.advance = 34.6640625; break;
            case U'T': result.advance = 29.3203125; break;
            case U'A': result.advance = 34.6640625; break;
            case U'B': result.advance = 34.6640625; break;
            default:
                ADD_FAILURE() << "Unexpected Helvetica glyph: " << static_cast<std::uint32_t>(codePoint);
                result.advance = 0.0;
                break;
            }
            result.ascent = 37.0;
            result.descent = 11.0;
        } else if (isArial) {
            switch (codePoint) {
            case U'T': result.advance = 21.990234375; break;
            case U'A': result.advance = 25.998046875; break;
            case U'B': result.advance = 25.998046875; break;
            default:
                ADD_FAILURE() << "Unexpected Arial glyph: " << static_cast<std::uint32_t>(codePoint);
                result.advance = 0.0;
                break;
            }
            result.ascent = 26.140625;
            result.descent = 0.125;
        } else if (isTimes) {
            if (codePoint == U' ') {
                result.advance = 12.0;
                result.ascent = 36.0;
                result.descent = 12.0;
            } else {
                ADD_FAILURE() << "Unexpected Times glyph: " << static_cast<std::uint32_t>(codePoint);
                result.advance = 0.0;
                result.ascent = 0.0;
                result.descent = 0.0;
            }
        } else if (isTimesNewRoman) {
            switch (codePoint) {
            case U'T':
                result.advance = 21.990234375;
                result.ascent = 23.765625;
                result.descent = 0.125;
                break;
            case U'A':
                result.advance = 25.998046875;
                result.ascent = 24.3125;
                result.descent = 0.125;
                break;
            case U'B':
                result.advance = 24.01171875;
                result.ascent = 23.765625;
                result.descent = 0.125;
                break;
            default:
                ADD_FAILURE() << "Unexpected Times New Roman glyph: " << static_cast<std::uint32_t>(codePoint);
                result.advance = 0.0;
                result.ascent = 0.0;
                result.descent = 0.0;
                break;
            }
        } else {
            ADD_FAILURE() << "Unexpected font family for glyph: " << static_cast<std::uint32_t>(codePoint);
            result.advance = 0.0;
            result.ascent = 0.0;
            result.descent = 0.0;
        }
        return result;
    };

    const auto svgOut = getOutputPath() / "PattersonDefault";
    std::error_code ec;
    std::filesystem::create_directories(svgOut, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << svgOut;

    const std::vector<Cmper> textShapeIds{2, 3, 4, 7};
    std::vector<std::filesystem::path> outputs;
    constexpr double kTextTolerance = 0.05;
    constexpr double kViewBoxTolerance = 0.5;

    for (Cmper shapeId : textShapeIds) {
        auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, shapeId);
        ASSERT_TRUE(shape);
        const auto refPath = svgRoot / (std::to_string(shapeId) + ".svg");
        const std::string referenceSvg = readFileText(refPath);
        ASSERT_FALSE(referenceSvg.empty());

        const std::string ourSvg = musx::util::SvgConvert::toSvg(*shape, metrics);
        ASSERT_FALSE(ourSvg.empty());

        const auto outPath = svgOut / (std::to_string(shapeId) + "_text.svg");
        std::ofstream out(outPath, std::ios::binary);
        ASSERT_TRUE(out.is_open()) << "Failed to open file for writing: " << outPath;
        out << ourSvg;
        out.close();
        outputs.push_back(outPath);

        const auto refTexts = parseTextInfo(referenceSvg);
        const auto ourTexts = parseTextInfo(ourSvg);
        ASSERT_EQ(refTexts.size(), ourTexts.size()) << "Text count mismatch for ShapeDef " << shapeId;
        for (size_t i = 0; i < refTexts.size(); ++i) {
            EXPECT_NEAR(refTexts[i].x, ourTexts[i].x, kTextTolerance)
                << "text x mismatch for ShapeDef " << shapeId << " index " << i;
            EXPECT_NEAR(refTexts[i].y, ourTexts[i].y, kTextTolerance)
                << "text y mismatch for ShapeDef " << shapeId << " index " << i;
        }

        ViewBox refBox = parseViewBox(referenceSvg);
        ViewBox ourBox = parseViewBox(ourSvg);
        ASSERT_TRUE(refBox.valid);
        ASSERT_TRUE(ourBox.valid);
        EXPECT_TRUE(viewBoxEncloses(ourBox, refBox, kViewBoxTolerance))
            << "viewBox mismatch for ShapeDef " << shapeId
            << " ours=(" << ourBox.minX << ", " << ourBox.minY << ", "
            << ourBox.width << ", " << ourBox.height << ")"
            << " ref=(" << refBox.minX << ", " << refBox.minY << ", "
            << refBox.width << ", " << refBox.height << ")";
    }

    if (!keepSvgOutput()) {
        for (const auto& outPath : outputs) {
            std::filesystem::remove(outPath, ec);
            ASSERT_FALSE(ec) << "Failed to remove file: " << outPath;
        }
    }
}

} // namespace musxtest
