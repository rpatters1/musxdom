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

ShapeInstructionSummary summarizeShape(const others::ShapeDef& shape)
{
    ShapeInstructionSummary summary;
    shape.iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) -> bool {
        summary.types.push_back(inst.type);
        EXPECT_TRUE(inst.valid()) << "Invalid decoded instruction for shape " << shape.getCmper();
        if (!inst.valid()) {
            return false;
        }

        if (const auto* data = std::get_if<ShapeDefInstruction::SetArrowhead>(&inst.data)) {
            summary.setArrowhead = *data;
            ++summary.setArrowheadCount;
        } else if (const auto* data = std::get_if<ShapeDefInstruction::RMoveTo>(&inst.data)) {
            summary.move = *data;
        } else if (const auto* data = std::get_if<ShapeDefInstruction::LineWidth>(&inst.data)) {
            summary.lineWidth = *data;
        } else if (const auto* data = std::get_if<ShapeDefInstruction::SetDash>(&inst.data)) {
            summary.setDash = *data;
        } else if (const auto* data = std::get_if<ShapeDefInstruction::RLineTo>(&inst.data)) {
            summary.lineSegments.push_back(*data);
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
    for (int shapeId : {6, 7, 8, 9, 10, 11, 12, 13, 14}) {
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
    std::vector<char> enigmaXml;
    readFile(getInputPath() / "arrowheads.enigmaxml", enigmaXml);

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    const auto svgRoot = getInputPath() / "arrowheads";
    const auto outRoot = getOutputPath() / "arrowheads";
    std::error_code ec;
    std::filesystem::create_directories(outRoot, ec);
    ASSERT_FALSE(ec) << "Failed to create output directory: " << outRoot;

    for (int shapeId : {6, 7, 8, 9, 10, 11, 12, 13, 14}) {
        const auto shape = requireShape(doc, shapeId);
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

        EXPECT_EQ(countSvgTag(generated, "path"), countSvgTag(reference, "path")) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("<marker"), std::string::npos) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("marker-start="), std::string::npos) << "ShapeDef " << shapeId;
        EXPECT_EQ(generated.find("marker-end="), std::string::npos) << "ShapeDef " << shapeId;
    }

    if (!keepArrowheadSvgOutput()) {
        std::filesystem::remove_all(outRoot, ec);
        ASSERT_FALSE(ec) << "Failed to remove output directory: " << outRoot;
    }
}

} // namespace
} // namespace musxtest
