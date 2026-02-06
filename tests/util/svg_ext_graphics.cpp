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

#include <array>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "musx/musx.h"

using namespace musx::dom;

namespace {

std::filesystem::path dataRoot()
{
    return std::filesystem::path("tests/data/svg_ext_graphics");
}

std::string readTextFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    EXPECT_TRUE(file.is_open()) << "failed to open " << path.string();
    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

void replaceAllInPlace(std::string& value, std::string_view from, std::string_view to)
{
    if (from.empty()) {
        return;
    }
    size_t pos = 0;
    while ((pos = value.find(from.data(), pos, from.size())) != std::string::npos) {
        value.replace(pos, from.size(), to);
        pos += to.size();
    }
}

std::vector<std::uint8_t> readBinaryFile(const std::filesystem::path& path)
{
    std::ifstream file(path, std::ios::binary);
    EXPECT_TRUE(file.is_open()) << "failed to open " << path.string();
    return std::vector<std::uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

std::filesystem::path makeTempDir(const std::string& name)
{
    const auto nonce = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::filesystem::path path = std::filesystem::temp_directory_path()
        / (name + "_" + std::to_string(nonce));
    std::filesystem::create_directories(path);
    return path;
}

std::optional<std::array<double, 4>> parseViewBox(const std::string& svg)
{
    const std::string marker = "viewBox=\"";
    const auto start = svg.find(marker);
    if (start == std::string::npos) {
        return std::nullopt;
    }
    const auto valueStart = start + marker.size();
    const auto end = svg.find('"', valueStart);
    if (end == std::string::npos) {
        return std::nullopt;
    }
    std::istringstream iss(svg.substr(valueStart, end - valueStart));
    std::array<double, 4> values{};
    if (!(iss >> values[0] >> values[1] >> values[2] >> values[3])) {
        return std::nullopt;
    }
    return values;
}

void writeSvgOutput(const std::string& svg, const std::string& filename)
{
    const auto outDir = dataRoot() / "output";
    std::filesystem::create_directories(outDir);
    const auto outPath = outDir / filename;
    std::ofstream file(outPath, std::ios::binary);
    if (!file.is_open()) {
        return;
    }
    file.write(svg.data(), static_cast<std::streamsize>(svg.size()));
}

std::optional<Cmper> getExternalGraphicCmper(const MusxInstance<others::ShapeDef>& shape)
{
    std::optional<Cmper> result;
    if (!shape) {
        return result;
    }
    shape->iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
        if (inst.type != ShapeDefInstructionType::ExternalGraphic) {
            return true;
        }
        if (const auto* data = std::get_if<ShapeDefInstruction::ExternalGraphic>(&inst.data)) {
            result = data->cmper;
        }
        return false;
    });
    return result;
}

MusxInstance<others::ShapeDef> findExternalGraphicShape(const DocumentPtr& doc, Cmper targetCmper)
{
    const auto shape = doc->getOthers()->get<others::ShapeDef>(SCORE_PARTID, targetCmper);
    if (!shape) {
        return {};
    }
        bool hasExternal = false;
        shape->iterateInstructions([&](const ShapeDefInstruction::Decoded& inst) {
            if (inst.type == ShapeDefInstructionType::ExternalGraphic) {
                hasExternal = true;
                return false;
            }
            return true;
        });
    if (hasExternal) {
        return shape;
    }
    return {};
}

} // namespace

TEST(SvgConvertExternalGraphicsTest, EmbeddedAndSourceDirectoryMatchBounds)
{
    const auto xmlPath = dataRoot() / "elephant.enigmaxml";
    const auto xml = readTextFile(xmlPath);
    const auto embeddedRoot = dataRoot() / "embedded";
    const auto sourceRoot = dataRoot() / "source_dir";

    musx::factory::DocumentFactory::CreateOptions embeddedOptions;
    auto embeddedDoc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(embeddedOptions));
    auto embeddedShape = findExternalGraphicShape(embeddedDoc, 6);
    ASSERT_TRUE(embeddedShape);
    auto graphicCmper = getExternalGraphicCmper(embeddedShape);
    ASSERT_TRUE(graphicCmper.has_value());

    const auto embeddedGraphicPath = embeddedRoot / (std::to_string(*graphicCmper) + ".jpg");
    const auto embeddedBytes = readBinaryFile(embeddedGraphicPath);

    musx::factory::DocumentFactory::CreateOptions options;
    musx::factory::DocumentFactory::CreateOptions::EmbeddedGraphicFile embeddedFile;
    embeddedFile.filename = embeddedGraphicPath.filename().string();
    embeddedFile.bytes = embeddedBytes;
    musx::factory::DocumentFactory::CreateOptions::EmbeddedGraphicFiles embeddedFiles;
    embeddedFiles.push_back(std::move(embeddedFile));
    options.setEmbeddedGraphics(std::move(embeddedFiles));

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(options));
    auto shape = findExternalGraphicShape(doc, 6);
    ASSERT_TRUE(shape);
    const std::string embeddedSvg = musx::util::SvgConvert::toSvg(*shape);
    if (!embeddedSvg.empty()) {
        writeSvgOutput(embeddedSvg, "embedded.svg");
    }
    EXPECT_FALSE(embeddedSvg.empty());
    EXPECT_NE(embeddedSvg.find("data:image/jpeg;base64,"), std::string::npos);

    auto embeddedViewBox = parseViewBox(embeddedSvg);
    ASSERT_TRUE(embeddedViewBox.has_value());

    const auto sourceGraphic = sourceRoot / "elephant_face_10x10.jpg";
    const auto tempDir = makeTempDir("musxdom_svg_ext_source");
    const auto tempXml = tempDir / "elephant.enigmaxml";
    std::filesystem::copy_file(xmlPath, tempXml, std::filesystem::copy_options::overwrite_existing);

    const auto others = doc->getOthers();
    ASSERT_TRUE(others);
    const auto filePaths = others->getArray<others::FilePath>(SCORE_PARTID);
    ASSERT_FALSE(filePaths.empty());
    for (const auto& filePath : filePaths) {
        if (filePath->path.empty()) {
            continue;
        }
        const auto tempGraphic = tempDir / std::filesystem::path(filePath->path).filename();
        std::filesystem::copy_file(sourceGraphic, tempGraphic, std::filesystem::copy_options::overwrite_existing);
    }

    musx::factory::DocumentFactory::CreateOptions sourceOptions;
    sourceOptions.setSourcePath(tempXml);
    auto sourceDoc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(sourceOptions));
    auto sourceShape = findExternalGraphicShape(sourceDoc, 6);
    ASSERT_TRUE(sourceShape);
    EXPECT_EQ(sourceShape->getRequestedPartId(), SCORE_PARTID);

    auto resolvedPath = sourceDoc->resolveExternalGraphicPath(2);
    ASSERT_TRUE(resolvedPath.has_value());
    EXPECT_TRUE(std::filesystem::is_regular_file(*resolvedPath));
    EXPECT_EQ(resolvedPath->extension().string(), ".jpg");
    EXPECT_GT(std::filesystem::file_size(*resolvedPath), 0U);

    const std::string sourceSvg = musx::util::SvgConvert::toSvg(*sourceShape);
    if (!sourceSvg.empty()) {
        writeSvgOutput(sourceSvg, "source.svg");
    }
    EXPECT_FALSE(sourceSvg.empty());
    EXPECT_NE(sourceSvg.find("data:image/jpeg;base64,"), std::string::npos);

    auto sourceViewBox = parseViewBox(sourceSvg);
    ASSERT_TRUE(sourceViewBox.has_value());
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_NEAR((*embeddedViewBox)[i], (*sourceViewBox)[i], 0.001);
    }

    std::filesystem::remove_all(tempDir);
}

TEST(SvgConvertExternalGraphicsTest, MissingGraphicReturnsEmpty)
{
    const auto xmlPath = dataRoot() / "elephant.enigmaxml";
    auto xml = readTextFile(xmlPath);
    replaceAllInPlace(xml, "<pathType>macAlias</pathType>", "<pathType>macPosixPath</pathType>");
    replaceAllInPlace(xml, "elephant_face_10x10.jpg", "missing.jpg");

    musx::factory::DocumentFactory::CreateOptions options;
    const auto tempDir = makeTempDir("musxdom_svg_ext_missing");
    options.setSourcePath(tempDir / "elephant.enigmaxml");

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(options));

    auto shape = findExternalGraphicShape(doc, 6);
    ASSERT_TRUE(shape);

    const std::string svg = musx::util::SvgConvert::toSvg(*shape);
    EXPECT_TRUE(svg.empty());

    std::filesystem::remove_all(tempDir);
}
