/*
 * Copyright (C) 2026, Robert Patterson
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
#include <optional>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <regex>
#include <cctype>
#include <cmath>

#include "gtest/gtest.h"
#include "test_utils.h"

using namespace musx::dom;

namespace musxtest {
namespace {

struct ShapeInstructionSummary
{
    std::vector<ShapeDefInstructionType> types;
    std::optional<ShapeDefInstruction::SetArrowhead> setArrowhead;
    std::optional<ShapeDefInstruction::RMoveTo> move;
    std::optional<ShapeDefInstruction::LineWidth> lineWidth;
    std::optional<ShapeDefInstruction::SetDash> setDash;
    std::vector<ShapeDefInstruction::RLineTo> lineSegments;
    int setArrowheadCount{};
};

struct ArrowheadViewBox
{
    double minX{};
    double minY{};
    double width{};
    double height{};
    bool valid{};
};

struct ArrowheadPathGeometry
{
    std::string commands;
    std::vector<double> numbers;
    std::string rawD;
};

struct ArrowheadGeomPoint
{
    double x{};
    double y{};
};

struct ArrowheadPresetFilledShape
{
    enum class Kind { Unknown, Triangle, CubicWedge };
    Kind kind{Kind::Unknown};
    ArrowheadGeomPoint tip{};
    ArrowheadGeomPoint a{};   // First edge/base point after tip.
    ArrowheadGeomPoint b{};   // Second base point (triangle) or cubic end point.
    ArrowheadGeomPoint c1{};  // Cubic only.
    ArrowheadGeomPoint c2{};  // Cubic only.
    std::string rawD;
};

ShapeInstructionSummary summarizeShape(const others::ShapeDef& shape)
{
    ShapeInstructionSummary summary;
    shape.iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) -> bool {
        summary.types.push_back(inst.type);
        EXPECT_TRUE(inst.valid()) << "Invalid decoded instruction for shape " << shape.getCmper();
        if (!inst.valid()) {
            return false;
        }

        if (const auto* setArrowheadInst = std::get_if<ShapeDefInstruction::SetArrowhead>(&inst.data)) {
            summary.setArrowhead = *setArrowheadInst;
            ++summary.setArrowheadCount;
        } else if (const auto* moveInst = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data)) {
            summary.move = *moveInst;
        } else if (const auto* lineWidthInst = std::get_if<ShapeDefInstruction::LineWidth>(&inst.data)) {
            summary.lineWidth = *lineWidthInst;
        } else if (const auto* setDashInst = std::get_if<ShapeDefInstruction::SetDash>(&inst.data)) {
            summary.setDash = *setDashInst;
        } else if (const auto* lineToInst = std::get_if<ShapeDefInstruction::RLineTo>(&inst.data)) {
            summary.lineSegments.push_back(*lineToInst);
        }
        return true;
    });
    return summary;
}

int countSvgTag(const std::string& svg, const std::string& tag)
{
    int count = 0;
    const std::string needle = "<" + tag;
    std::string::size_type pos = 0;
    while ((pos = svg.find(needle, pos)) != std::string::npos) {
        ++count;
        pos += needle.size();
    }
    return count;
}

ArrowheadViewBox parseArrowheadViewBox(const std::string& svg)
{
    ArrowheadViewBox result;
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

std::vector<std::string> extractArrowheadFilledPathDs(const std::string& svg)
{
    std::vector<std::string> result;
    static const std::regex pathTagRegex(R"(<path\b[^>]*>)");
    static const std::regex dAttrRegex(R"(\bd=\"([^\"]+)\")");
    for (std::sregex_iterator it(svg.begin(), svg.end(), pathTagRegex), end; it != end; ++it) {
        const std::string tag = it->str();
        if (tag.find("stroke=\"none\"") == std::string::npos) {
            continue;
        }
        if (tag.find("fill=\"none\"") != std::string::npos) {
            continue;
        }
        std::smatch dMatch;
        if (std::regex_search(tag, dMatch, dAttrRegex)) {
            result.push_back(dMatch[1].str());
        }
    }
    return result;
}

std::vector<std::string> extractArrowheadOutlineStrokePathDs(const std::string& svg)
{
    std::vector<std::string> result;
    static const std::regex pathTagRegex(R"(<path\b[^>]*>)");
    static const std::regex dAttrRegex(R"(\bd=\"([^\"]+)\")");
    static const std::regex strokeWidthRegex(R"(\bstroke-width=\"([^\"]+)\")");
    for (std::sregex_iterator it(svg.begin(), svg.end(), pathTagRegex), end; it != end; ++it) {
        const std::string tag = it->str();
        if (tag.find("fill=\"none\"") == std::string::npos) {
            continue;
        }
        if (tag.find("stroke=\"none\"") != std::string::npos) {
            continue;
        }
        std::smatch swMatch;
        if (!std::regex_search(tag, swMatch, strokeWidthRegex)) {
            continue;
        }
        const double sw = std::stod(swMatch[1].str());
        if (sw > 2.5) { // Skip the main line stroke; keep the outline-arrowhead strokes (~1.8)
            continue;
        }
        std::smatch dMatch;
        if (std::regex_search(tag, dMatch, dAttrRegex)) {
            result.push_back(dMatch[1].str());
        }
    }
    return result;
}

ArrowheadPathGeometry parseArrowheadPathGeometry(const std::string& d)
{
    ArrowheadPathGeometry result;
    result.rawD = d;
    size_t pos = 0;
    auto skipSeparators = [&]() {
        while (pos < d.size()) {
            const unsigned char ch = static_cast<unsigned char>(d[pos]);
            if (std::isspace(ch) || d[pos] == ',') {
                ++pos;
            } else {
                break;
            }
        }
    };
    auto readNumber = [&](double& value) -> bool {
        skipSeparators();
        if (pos >= d.size()) {
            return false;
        }
        size_t start = pos;
        bool sawDigit = false;
        if (d[pos] == '+' || d[pos] == '-') {
            ++pos;
        }
        while (pos < d.size() && std::isdigit(static_cast<unsigned char>(d[pos]))) {
            sawDigit = true;
            ++pos;
        }
        if (pos < d.size() && d[pos] == '.') {
            ++pos;
            while (pos < d.size() && std::isdigit(static_cast<unsigned char>(d[pos]))) {
                sawDigit = true;
                ++pos;
            }
        }
        if (pos < d.size() && (d[pos] == 'e' || d[pos] == 'E')) {
            size_t expPos = pos + 1;
            if (expPos < d.size() && (d[expPos] == '+' || d[expPos] == '-')) {
                ++expPos;
            }
            bool expDigits = false;
            while (expPos < d.size() && std::isdigit(static_cast<unsigned char>(d[expPos]))) {
                expDigits = true;
                ++expPos;
            }
            if (expDigits) {
                pos = expPos;
            }
        }
        if (!sawDigit) {
            pos = start;
            return false;
        }
        value = std::stod(d.substr(start, pos - start));
        return true;
    };

    while (pos < d.size()) {
        skipSeparators();
        if (pos >= d.size()) {
            break;
        }
        const unsigned char ch = static_cast<unsigned char>(d[pos]);
        if (std::isalpha(ch)) {
            result.commands.push_back(static_cast<char>(std::toupper(ch)));
            ++pos;
            continue;
        }
        double value{};
        if (readNumber(value)) {
            result.numbers.push_back(value);
            continue;
        }
        ++pos;
    }
    return result;
}

bool arrowheadGeomPointNear(const ArrowheadGeomPoint& a,
                            const ArrowheadGeomPoint& b,
                            double tolerance)
{
    return std::abs(a.x - b.x) <= tolerance && std::abs(a.y - b.y) <= tolerance;
}

std::optional<ArrowheadPresetFilledShape> parseArrowheadPresetFilledShape(const ArrowheadPathGeometry& geom)
{
    auto readPoint = [&](size_t numberIndex) -> ArrowheadGeomPoint {
        return ArrowheadGeomPoint{geom.numbers[numberIndex], geom.numbers[numberIndex + 1]};
    };
    auto pointEq = [&](const ArrowheadGeomPoint& p, const ArrowheadGeomPoint& q) {
        return std::abs(p.x - q.x) <= 1e-6 && std::abs(p.y - q.y) <= 1e-6;
    };

    std::vector<char> cmds(geom.commands.begin(), geom.commands.end());
    std::vector<double> nums = geom.numbers;

    // Canonicalize redundant explicit close line: ... L <tip> Z
    if (!cmds.empty() && cmds.back() == 'Z' && cmds.size() >= 2 && cmds[cmds.size() - 2] == 'L') {
        // Compute number offset to the final L command.
        size_t numOffset = 0;
        for (size_t i = 0; i + 2 < cmds.size(); ++i) {
            switch (cmds[i]) {
            case 'M':
            case 'L':
                numOffset += 2;
                break;
            case 'C':
                numOffset += 6;
                break;
            case 'Q':
                numOffset += 4;
                break;
            case 'Z':
                break;
            default:
                return std::nullopt;
            }
        }
        if (numOffset + 1 < nums.size() && nums.size() >= 4) {
            ArrowheadGeomPoint lastL{nums[numOffset], nums[numOffset + 1]};
            ArrowheadGeomPoint tip{nums[0], nums[1]};
            if (pointEq(lastL, tip)) {
                nums.resize(numOffset);
                cmds.erase(cmds.end() - 2); // remove the redundant L before Z
            }
        }
    }

    ArrowheadPresetFilledShape out;
    out.rawD = geom.rawD;
    if (cmds == std::vector<char>{'M', 'L', 'L', 'Z'} && nums.size() == 6) {
        out.kind = ArrowheadPresetFilledShape::Kind::Triangle;
        out.tip = readPoint(0);
        out.a = readPoint(2);
        out.b = readPoint(4);
        return out;
    }
    if (cmds == std::vector<char>{'M', 'L', 'C', 'Z'} && nums.size() == 10) {
        out.kind = ArrowheadPresetFilledShape::Kind::CubicWedge;
        out.tip = readPoint(0);
        out.a = readPoint(2);
        out.c1 = readPoint(4);
        out.c2 = readPoint(6);
        out.b = readPoint(8);
        return out;
    }
    return std::nullopt;
}

bool arrowheadPresetFilledShapeNear(const ArrowheadPresetFilledShape& a,
                                    const ArrowheadPresetFilledShape& b,
                                    double tolerance)
{
    if (a.kind != b.kind) {
        return false;
    }
    if (!arrowheadGeomPointNear(a.tip, b.tip, tolerance)) {
        return false;
    }
    switch (a.kind) {
    case ArrowheadPresetFilledShape::Kind::Triangle:
        return (arrowheadGeomPointNear(a.a, b.a, tolerance)
                && arrowheadGeomPointNear(a.b, b.b, tolerance))
            || (arrowheadGeomPointNear(a.a, b.b, tolerance)
                && arrowheadGeomPointNear(a.b, b.a, tolerance));
    case ArrowheadPresetFilledShape::Kind::CubicWedge:
        return (arrowheadGeomPointNear(a.a, b.a, tolerance)
                && arrowheadGeomPointNear(a.c1, b.c1, tolerance)
                && arrowheadGeomPointNear(a.c2, b.c2, tolerance)
                && arrowheadGeomPointNear(a.b, b.b, tolerance))
            || (arrowheadGeomPointNear(a.a, b.b, tolerance)
                && arrowheadGeomPointNear(a.c1, b.c2, tolerance)
                && arrowheadGeomPointNear(a.c2, b.c1, tolerance)
                && arrowheadGeomPointNear(a.b, b.a, tolerance));
    case ArrowheadPresetFilledShape::Kind::Unknown:
        return false;
    }
    return false;
}

bool keepArrowheadSvgOutput()
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

MusxInstance<others::ShapeDef> requireShape(const DocumentPtr& doc, Cmper cmper)
{
    if (!doc) {
        ADD_FAILURE() << "Missing document";
        return {};
    }
    auto others = doc->getOthers();
    if (!others) {
        ADD_FAILURE() << "Document has no <others>";
        return {};
    }
    auto shape = others ? others->get<others::ShapeDef>(SCORE_PARTID, cmper)
                        : MusxInstance<others::ShapeDef>{};
    EXPECT_TRUE(shape) << "Missing ShapeDef " << cmper;
    return shape;
}

void expectLineFixtureShape(const ShapeInstructionSummary& summary, int expectedLineWidthEfix, int dx, int dy)
{
    ASSERT_TRUE(summary.move);
    EXPECT_EQ(summary.move->dx, 0);
    EXPECT_EQ(summary.move->dy, 0);

    ASSERT_TRUE(summary.lineWidth);
    EXPECT_EQ(summary.lineWidth->efix, expectedLineWidthEfix);

    ASSERT_TRUE(summary.setDash);
    EXPECT_EQ(summary.setDash->dashLength, 18);
    EXPECT_EQ(summary.setDash->spaceLength, 0);

    ASSERT_EQ(summary.lineSegments.size(), 1u);
    EXPECT_EQ(summary.lineSegments.front().dx, dx);
    EXPECT_EQ(summary.lineSegments.front().dy, dy);

    ASSERT_FALSE(summary.types.empty());
    EXPECT_EQ(summary.types.front(), ShapeDefInstructionType::StartObject);
    EXPECT_EQ(summary.types.back(), ShapeDefInstructionType::Stroke);
}

TEST(SvgArrowheadsStage0Test, ArrowheadExamplesDecodeRawSetArrowheadWords)
{
    std::vector<char> enigmaXml;
    readFile(getInputPath() / "arrowheads.enigmaxml", enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    struct Expected {
        Cmper shapeId;
        others::ShapeDef::ShapeType shapeType;
        ShapeDefInstruction::SetArrowhead raw;
        int lineWidthEfix;
        int dx;
        int dy;
    };

    // The first SetArrowhead word packs the start/end kind codes:
    // start in low 16 bits, end in high 16 bits.
    const std::vector<Expected> expected = {
        {6, others::ShapeDef::ShapeType::Expression, {65537, 1, 1, 0},  256,  192,  188},
        {7, others::ShapeDef::ShapeType::Expression, {65537, 2, 2, 0},  256, -232,   88},
        {8, others::ShapeDef::ShapeType::Expression, {65537, 3, 3, 0},  256, -188, -144},
        {9, others::ShapeDef::ShapeType::Expression, {65537, 4, 4, 0}, 1280,  224, -112},
        {10, others::ShapeDef::ShapeType::Expression, {65537, 5, 5, 0}, 1280, 332,  200},
        {11, others::ShapeDef::ShapeType::Expression, {131074, 12, 12, 0}, 1280, 332, 200},
        {13, others::ShapeDef::ShapeType::Expression, {65538, 12, 3, 0},  512, 332, 200},
        {14, others::ShapeDef::ShapeType::Expression, {131073, 5, 12, 0}, 512, 332, 200},
    };

    for (const auto& item : expected) {
        const auto shape = requireShape(doc, item.shapeId);
        ASSERT_TRUE(shape);
        EXPECT_EQ(shape->shapeType, item.shapeType) << "ShapeDef " << item.shapeId;

        const auto summary = summarizeShape(*shape);
        ASSERT_EQ(summary.setArrowheadCount, 1) << "ShapeDef " << item.shapeId;
        ASSERT_TRUE(summary.setArrowhead) << "ShapeDef " << item.shapeId;

        EXPECT_EQ(summary.setArrowhead->packedKindCodes, item.raw.packedKindCodes) << "ShapeDef " << item.shapeId;
        EXPECT_EQ(summary.setArrowhead->startArrowId, item.raw.startArrowId) << "ShapeDef " << item.shapeId;
        EXPECT_EQ(summary.setArrowhead->endArrowId, item.raw.endArrowId) << "ShapeDef " << item.shapeId;
        EXPECT_EQ(summary.setArrowhead->extra, item.raw.extra) << "ShapeDef " << item.shapeId;

        if (item.shapeId >= 6 && item.shapeId <= 10) {
            EXPECT_EQ(summary.setArrowhead->startKindCode(), 1) << "ShapeDef " << item.shapeId;
            EXPECT_EQ(summary.setArrowhead->endKindCode(), 1) << "ShapeDef " << item.shapeId;
        } else if (item.shapeId == 11) {
            EXPECT_EQ(summary.setArrowhead->startKindCode(), 2) << "ShapeDef " << item.shapeId;
            EXPECT_EQ(summary.setArrowhead->endKindCode(), 2) << "ShapeDef " << item.shapeId;
        } else if (item.shapeId == 13) {
            EXPECT_EQ(summary.setArrowhead->startKindCode(), 2) << "ShapeDef " << item.shapeId;
            EXPECT_EQ(summary.setArrowhead->endKindCode(), 1) << "ShapeDef " << item.shapeId;
        } else if (item.shapeId == 14) {
            EXPECT_EQ(summary.setArrowhead->startKindCode(), 1) << "ShapeDef " << item.shapeId;
            EXPECT_EQ(summary.setArrowhead->endKindCode(), 2) << "ShapeDef " << item.shapeId;
        }

        expectLineFixtureShape(summary, item.lineWidthEfix, item.dx, item.dy);
    }
}

TEST(SvgArrowheadsStage0Test, CustomArrowheadShape12IsStandaloneArrowheadGeometry)
{
    std::vector<char> enigmaXml;
    readFile(getInputPath() / "arrowheads.enigmaxml", enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    const auto shape = requireShape(doc, 12);
    ASSERT_TRUE(shape);
    EXPECT_EQ(shape->shapeType, others::ShapeDef::ShapeType::Arrowhead);

    const auto summary = summarizeShape(*shape);
    EXPECT_EQ(summary.setArrowheadCount, 0);

    ASSERT_EQ(summary.lineSegments.size(), 5u);
    // Finale stores/emits a redundant first segment at the origin (also visible in `12.svg` as `L0,0`).
    EXPECT_EQ(summary.lineSegments[0].dx, 0);
    EXPECT_EQ(summary.lineSegments[0].dy, 0);
    EXPECT_EQ(summary.lineSegments[1].dx, 8);
    EXPECT_EQ(summary.lineSegments[1].dy, 9);
    EXPECT_EQ(summary.lineSegments[2].dx, 2);
    EXPECT_EQ(summary.lineSegments[2].dy, -12);
    EXPECT_EQ(summary.lineSegments[3].dx, 15);
    EXPECT_EQ(summary.lineSegments[3].dy, 20);
    EXPECT_EQ(summary.lineSegments[4].dx, 7);
    EXPECT_EQ(summary.lineSegments[4].dy, -28);
}

TEST(SvgArrowheadsStage0Test, FinaleReferenceSvgsUseExplicitPathsNotSvgMarkers)
{
    const auto svgRoot = getInputPath() / "arrowheads";
    for (int shapeId : {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}) {
        std::vector<char> bytes;
        const auto path = svgRoot / (std::to_string(shapeId) + ".svg");
        readFile(path, bytes);
        const std::string svg(bytes.begin(), bytes.end());
        ASSERT_FALSE(svg.empty()) << path;
        EXPECT_EQ(svg.find("<marker"), std::string::npos) << path;
        EXPECT_EQ(svg.find("marker-start="), std::string::npos) << path;
        EXPECT_EQ(svg.find("marker-end="), std::string::npos) << path;
    }
}

TEST(SvgArrowheadsStage0Test, SvgConvertRendersArrowheadsAsExplicitPaths)
{
    constexpr double kViewBoxTolerance = 2.0;

    std::vector<char> enigmaXml;
    readFile(getInputPath() / "arrowheads.enigmaxml", enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    const auto svgRoot = getInputPath() / "arrowheads";
    const auto outRoot = getOutputPath() / "arrowheads";
    std::error_code ec;
    std::filesystem::create_directories(outRoot, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << outRoot;

    for (int shapeId : {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}) {
        const auto shape = requireShape(doc, static_cast<Cmper>(shapeId));
        ASSERT_TRUE(shape);

        const std::string generated = musx::util::SvgConvert::toSvg(*shape);
        ASSERT_FALSE(generated.empty()) << "ShapeDef " << shapeId;

        const auto outPath = outRoot / (std::to_string(shapeId) + "_generated.svg");
        std::ofstream out(outPath, std::ios::binary);
        ASSERT_TRUE(out.is_open()) << "Failed to open output file: " << outPath;
        out << generated;
        out.close();

        if (shapeId == 12) {
            continue;
        }

        std::vector<char> bytes;
        const auto refPath = svgRoot / (std::to_string(shapeId) + ".svg");
        readFile(refPath, bytes);
        const std::string reference(bytes.begin(), bytes.end());
        ASSERT_FALSE(reference.empty()) << refPath;

        const auto refBox = parseArrowheadViewBox(reference);
        const auto genBox = parseArrowheadViewBox(generated);
        ASSERT_TRUE(refBox.valid) << "Missing viewBox in reference SVG for ShapeDef " << shapeId;
        ASSERT_TRUE(genBox.valid) << "Missing viewBox in generated SVG for ShapeDef " << shapeId;

        EXPECT_EQ(countSvgTag(generated, "path"), countSvgTag(reference, "path")) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("<marker"), std::string::npos) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("marker-start="), std::string::npos) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("marker-end="), std::string::npos) << "ShapeDef " << shapeId;
        EXPECT_NEAR(genBox.minX, refBox.minX, kViewBoxTolerance) << "ShapeDef " << shapeId;
        EXPECT_NEAR(genBox.minY, refBox.minY, kViewBoxTolerance) << "ShapeDef " << shapeId;
        EXPECT_NEAR(genBox.width, refBox.width, kViewBoxTolerance) << "ShapeDef " << shapeId;
        EXPECT_NEAR(genBox.height, refBox.height, kViewBoxTolerance) << "ShapeDef " << shapeId;
    }

    if (!keepArrowheadSvgOutput()) {
        std::filesystem::remove_all(outRoot, ec);
        ASSERT_FALSE(ec) << "Failed to remove output directory: " << outRoot;
    }
}

TEST(SvgArrowheadsStage0Test, PresetArrowheadStandaloneGeometryMatchesTemplates)
{
    constexpr double kPathGeometryTolerance = 0.01;
    using Kind = ArrowheadPresetFilledShape::Kind;
    using Preset = ArrowheadPreset;
    auto pt = [](double x, double y) { return ArrowheadGeomPoint{x, y}; };
    auto triangle = [&](Preset preset, double x, double y) {
        ArrowheadPresetFilledShape s;
        s.kind = Kind::Triangle;
        s.tip = pt(0.0, 0.0);
        s.a = pt(x, y);
        s.b = pt(x, -y);
        s.rawD = std::to_string(static_cast<int>(preset));
        return s;
    };
    auto cubic = [&](Preset preset, double baseX, double baseY, double ctrlX, double ctrlY) {
        ArrowheadPresetFilledShape s;
        s.kind = Kind::CubicWedge;
        s.tip = pt(0.0, 0.0);
        s.a = pt(baseX, baseY);
        s.c1 = pt(ctrlX, ctrlY);
        s.c2 = pt(ctrlX, -ctrlY);
        s.b = pt(baseX, -baseY);
        s.rawD = std::to_string(static_cast<int>(preset));
        return s;
    };

    struct ExpectedCase {
        Preset preset;
        ArrowheadPresetFilledShape expectedFill;
    };
    const std::vector<ExpectedCase> cases = {
        {Preset::SmallFilled,  triangle(Preset::SmallFilled, -20.0, 10.0)},
        {Preset::SmallOutline, triangle(Preset::SmallOutline, -20.0, 10.0)},
        {Preset::SmallCurved,  cubic(Preset::SmallCurved, -20.0, 10.0, -16.0, 3.0)},
        {Preset::MediumCurved, cubic(Preset::MediumCurved, -60.0, 30.0, -48.0, 9.0)},
        {Preset::LargeCurved,  cubic(Preset::LargeCurved, -100.0, 50.0, -80.0, 15.0)},
    };

    for (const auto& item : cases) {
        const std::string svg = musx::util::SvgConvert::presetArrowheadAsSvg(item.preset);
        ASSERT_FALSE(svg.empty());
        const auto fills = extractArrowheadFilledPathDs(svg);
        ASSERT_EQ(fills.size(), 1u) << static_cast<int>(item.preset);
        const auto geom = parseArrowheadPathGeometry(fills.front());
        const auto parsed = parseArrowheadPresetFilledShape(geom);
        ASSERT_TRUE(parsed.has_value()) << fills.front();
        EXPECT_TRUE(arrowheadPresetFilledShapeNear(*parsed, item.expectedFill, kPathGeometryTolerance))
            << "Preset " << static_cast<int>(item.preset) << " path: " << fills.front();
    }
}

TEST(SvgArrowheadsStage0Test, SmallOutlineStrokePathRepeatsTipBeforeClose)
{
    constexpr double kTolerance = 0.01;

    std::vector<char> enigmaXml;
    readFile(getInputPath() / "arrowheads.enigmaxml", enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);
    const auto shape = requireShape(doc, 7);
    ASSERT_TRUE(shape);

    const std::string generated = musx::util::SvgConvert::toSvg(*shape);
    ASSERT_FALSE(generated.empty());

    const auto outlineDs = extractArrowheadOutlineStrokePathDs(generated);
    ASSERT_EQ(outlineDs.size(), 2u);
    for (const auto& d : outlineDs) {
        const auto geom = parseArrowheadPathGeometry(d);
        EXPECT_EQ(geom.commands, "MLLLLZ") << d;
        ASSERT_GE(geom.numbers.size(), 10u) << d;
        const ArrowheadGeomPoint tip{geom.numbers[0], geom.numbers[1]};
        const ArrowheadGeomPoint penultimate{geom.numbers[8], geom.numbers[9]};
        EXPECT_TRUE(arrowheadGeomPointNear(tip, penultimate, kTolerance)) << d;
    }
}

TEST(SvgArrowheadsStage0Test, StandalonePresetArrowheadSvgApi)
{
    using Preset = ArrowheadPreset;
    struct Expected {
        Preset preset;
        int pathCount;
    };
    const std::vector<Expected> cases = {
        {Preset::SmallFilled, 1},
        {Preset::SmallOutline, 2},
        {Preset::SmallCurved, 1},
        {Preset::LargeCurved, 1},
        {Preset::MediumCurved, 1},
    };

    for (const auto& item : cases) {
        const std::string svg = musx::util::SvgConvert::presetArrowheadAsSvg(item.preset);
        ASSERT_FALSE(svg.empty());
        EXPECT_NE(svg.find("<g id=\"MusxDom\">"), std::string::npos);
        EXPECT_EQ(svg.find("<marker"), std::string::npos);
        EXPECT_EQ(countSvgTag(svg, "path"), item.pathCount);

        const auto box = parseArrowheadViewBox(svg);
        ASSERT_TRUE(box.valid);
        EXPECT_LE(box.minX, 0.0);
        EXPECT_LE(box.minY, 0.0);
        EXPECT_GE(box.minX + box.width, 0.0);
        EXPECT_GE(box.minY + box.height, 0.0);
    }

    const std::string baseSvg = musx::util::SvgConvert::presetArrowheadAsSvg(Preset::SmallFilled);
    const std::string scaledMmSvg = musx::util::SvgConvert::presetArrowheadAsSvg(
        Preset::SmallFilled, 2.0, musx::util::SvgConvert::SvgUnit::Millimeters);
    ASSERT_FALSE(baseSvg.empty());
    ASSERT_FALSE(scaledMmSvg.empty());
    EXPECT_NE(scaledMmSvg.find("mm\""), std::string::npos);

    const auto baseBox = parseArrowheadViewBox(baseSvg);
    const auto scaledBox = parseArrowheadViewBox(scaledMmSvg);
    ASSERT_TRUE(baseBox.valid);
    ASSERT_TRUE(scaledBox.valid);
    const double expectedScale = 2.0 / EVPU_PER_MM;
    EXPECT_NEAR(scaledBox.width, baseBox.width * expectedScale, 0.01);
    EXPECT_NEAR(scaledBox.height, baseBox.height * expectedScale, 0.01);
}

TEST(SvgArrowheadsStage0Test, PresetArrowheadSpecApi)
{
    using Preset = musx::dom::ArrowheadPreset;
    using Fill = musx::util::ArrowheadPresetFillStyle;

    const auto smallFilled = musx::util::getArrowheadPresetSpec(Preset::SmallFilled);
    EXPECT_DOUBLE_EQ(smallFilled.length, 20.0);
    EXPECT_DOUBLE_EQ(smallFilled.baseWidth, 20.0);
    EXPECT_FALSE(smallFilled.curvedBack);
    EXPECT_EQ(smallFilled.fillStyle, Fill::SolidFill);

    const auto smallOutline = musx::util::getArrowheadPresetSpec(Preset::SmallOutline);
    EXPECT_DOUBLE_EQ(smallOutline.length, 20.0);
    EXPECT_DOUBLE_EQ(smallOutline.baseWidth, 20.0);
    EXPECT_FALSE(smallOutline.curvedBack);
    EXPECT_EQ(smallOutline.fillStyle, Fill::WhiteFillWithOutline);

    const auto smallCurved = musx::util::getArrowheadPresetSpec(Preset::SmallCurved);
    EXPECT_DOUBLE_EQ(smallCurved.length, 20.0);
    EXPECT_DOUBLE_EQ(smallCurved.baseWidth, 20.0);
    EXPECT_TRUE(smallCurved.curvedBack);
    EXPECT_EQ(smallCurved.fillStyle, Fill::SolidFill);

    const auto mediumCurved = musx::util::getArrowheadPresetSpec(Preset::MediumCurved);
    EXPECT_DOUBLE_EQ(mediumCurved.length, 60.0);
    EXPECT_DOUBLE_EQ(mediumCurved.baseWidth, 60.0);
    EXPECT_TRUE(mediumCurved.curvedBack);
    EXPECT_EQ(mediumCurved.fillStyle, Fill::SolidFill);

    const auto largeCurved = musx::util::getArrowheadPresetSpec(Preset::LargeCurved);
    EXPECT_DOUBLE_EQ(largeCurved.length, 100.0);
    EXPECT_DOUBLE_EQ(largeCurved.baseWidth, 100.0);
    EXPECT_TRUE(largeCurved.curvedBack);
    EXPECT_EQ(largeCurved.fillStyle, Fill::SolidFill);
}

TEST(SvgArrowheadsStage0Test, StandaloneSmallOutlinePresetDrawOrder)
{
    const std::string svg = musx::util::SvgConvert::presetArrowheadAsSvg(ArrowheadPreset::SmallOutline);
    ASSERT_FALSE(svg.empty());

    static const std::regex pathTagRegex(R"(<path\b[^>]*>)");
    std::vector<std::string> pathTags;
    for (std::sregex_iterator it(svg.begin(), svg.end(), pathTagRegex), end; it != end; ++it) {
        pathTags.push_back(it->str());
    }
    ASSERT_EQ(pathTags.size(), 2u);

    EXPECT_NE(pathTags[0].find("stroke=\"none\""), std::string::npos) << pathTags[0];
    EXPECT_NE(pathTags[0].find("fill=\"rgb(255,255,255)\""), std::string::npos) << pathTags[0];

    EXPECT_NE(pathTags[1].find("fill=\"none\""), std::string::npos) << pathTags[1];
    EXPECT_NE(pathTags[1].find("stroke=\"rgb(0,0,0)\""), std::string::npos) << pathTags[1];
}

} // namespace
} // namespace musxtest
