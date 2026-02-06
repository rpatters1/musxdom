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

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

#include "gtest/gtest.h"
#include "musx/musx.h"

#if defined(MUSX_RUNNING_ON_MACOS)
#include <CoreFoundation/CoreFoundation.h>
#include <dlfcn.h>
#endif

using namespace musx::dom;

namespace {

std::filesystem::path makeTempDir(const std::string& name)
{
    const auto nonce = std::chrono::high_resolution_clock::now().time_since_epoch().count();
    std::filesystem::path path = std::filesystem::temp_directory_path()
        / (name + "_" + std::to_string(nonce));
    std::filesystem::create_directories(path);
    return path;
}

void writeFile(const std::filesystem::path& path, const std::string& contents)
{
    std::ofstream file(path, std::ios::binary);
    ASSERT_TRUE(file.is_open()) << "failed to open " << path.string();
    file.write(contents.data(), static_cast<std::streamsize>(contents.size()));
}

std::string buildGraphicsXml(const std::string& filePathValue)
{
    std::ostringstream xml;
    xml << R"xml(<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fileDesc cmper="1">
      <version>256</version>
      <pathType>macPosixPath</pathType>
      <pathID>1</pathID>
    </fileDesc>
    <filePath cmper="1">
      <path>)xml";
    xml << filePathValue;
    xml << R"xml(</path>
    </filePath>
  </others>
</finale>)xml";
    return xml.str();
}

#if defined(MUSX_RUNNING_ON_MACOS)
std::string hexEncode(const std::vector<std::uint8_t>& data)
{
    static constexpr char kHex[] = "0123456789ABCDEF";
    std::string out;
    out.reserve(data.size() * 2);
    for (std::uint8_t byte : data) {
        out.push_back(kHex[(byte >> 4) & 0x0F]);
        out.push_back(kHex[byte & 0x0F]);
    }
    return out;
}

std::string buildBookmarkGraphicsXml(const std::string& filePathValue, const std::string& bookmarkHex, size_t bookmarkLength)
{
    std::ostringstream xml;
    xml << R"xml(<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fileDesc cmper="1">
      <version>256</version>
      <pathType>macURLBookmark</pathType>
      <pathID>1</pathID>
    </fileDesc>
    <filePath cmper="1">
      <path>)xml";
    xml << filePathValue;
    xml << R"xml(</path>
    </filePath>
    <fileURLBookmark cmper="1">
      <length>)xml";
    xml << bookmarkLength;
    xml << R"xml(</length>
      <urlBookmarkData>)xml";
    xml << bookmarkHex;
    xml << R"xml(</urlBookmarkData>
    </fileURLBookmark>
  </others>
</finale>)xml";
    return xml.str();
}

std::optional<std::vector<std::uint8_t>> createAliasRecordBytes(const std::filesystem::path& target)
{
    CFURLRef url = CFURLCreateFromFileSystemRepresentation(
        kCFAllocatorDefault,
        reinterpret_cast<const UInt8*>(target.u8string().c_str()),
        static_cast<CFIndex>(target.u8string().size()),
        false);
    if (!url) {
        return std::nullopt;
    }
    CFDataRef bookmarkData = CFURLCreateBookmarkData(
        kCFAllocatorDefault,
        url,
        kCFURLBookmarkCreationMinimalBookmarkMask,
        nullptr,
        nullptr,
        nullptr);
    CFRelease(url);
    if (!bookmarkData) {
        return std::nullopt;
    }
    using AliasFromBookmarkFn = CFDataRef (*)(CFAllocatorRef, CFDataRef);
    auto fn = reinterpret_cast<AliasFromBookmarkFn>(dlsym(RTLD_DEFAULT, "CFURLCreateAliasRecordFromBookmarkData"));
    if (!fn) {
        CFRelease(bookmarkData);
        return std::nullopt;
    }
    CFDataRef aliasData = fn(kCFAllocatorDefault, bookmarkData);
    CFRelease(bookmarkData);
    if (!aliasData) {
        return std::nullopt;
    }
    std::vector<std::uint8_t> aliasBytes(static_cast<size_t>(CFDataGetLength(aliasData)));
    if (!aliasBytes.empty()) {
        CFDataGetBytes(aliasData, CFRangeMake(0, CFDataGetLength(aliasData)), aliasBytes.data());
    }
    CFRelease(aliasData);
    return aliasBytes;
}

std::string buildAliasGraphicsXml(const std::string& filePathValue, const std::string& aliasHex, size_t aliasLength)
{
    std::ostringstream xml;
    xml << R"xml(<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fileDesc cmper="1">
      <version>256</version>
      <pathType>macAlias</pathType>
      <pathID>1</pathID>
    </fileDesc>
    <filePath cmper="1">
      <path>)xml";
    xml << filePathValue;
    xml << R"xml(</path>
    </filePath>
    <fileAlias cmper="1">
      <length>)xml";
    xml << aliasLength;
    xml << R"xml(</length>
      <aliasHandle>)xml";
    xml << aliasHex;
    xml << R"xml(</aliasHandle>
    </fileAlias>
  </others>
</finale>)xml";
    return xml.str();
}
#endif

} // namespace

TEST(DocumentGraphicsTest, ResolvesFromSourceDirectory)
{
    const auto tempDir = makeTempDir("musxdom_graphics_source");
    const auto filename = std::filesystem::path("graphic.png");
    const auto graphicPath = tempDir / filename;
    writeFile(graphicPath, "fake");

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(tempDir / "score.musx");

    const auto xml = buildGraphicsXml(filename.string());
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    ASSERT_TRUE(resolved.has_value());
    EXPECT_EQ(std::filesystem::weakly_canonical(*resolved), std::filesystem::weakly_canonical(graphicPath));

    std::filesystem::remove_all(tempDir);
}

TEST(DocumentGraphicsTest, ReturnsNulloptWhenMissing)
{
    const auto tempDir = makeTempDir("musxdom_graphics_missing");

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(tempDir / "score.musx");

    const auto xml = buildGraphicsXml("missing.png");
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    EXPECT_FALSE(resolved.has_value());

    std::filesystem::remove_all(tempDir);
}

TEST(DocumentGraphicsTest, PrefersSourceDirectoryOverAbsolutePath)
{
    const auto sourceDir = makeTempDir("musxdom_graphics_source_priority");
    const auto otherDir = makeTempDir("musxdom_graphics_absolute");
    const auto filename = std::filesystem::path("graphic.png");

    const auto sourceGraphic = sourceDir / filename;
    const auto absoluteGraphic = otherDir / filename;
    writeFile(sourceGraphic, "source");
    writeFile(absoluteGraphic, "absolute");

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(sourceDir / "score.musx");

    const auto xml = buildGraphicsXml(absoluteGraphic.string());
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    ASSERT_TRUE(resolved.has_value());
    EXPECT_EQ(std::filesystem::weakly_canonical(*resolved), std::filesystem::weakly_canonical(sourceGraphic));

    std::filesystem::remove_all(sourceDir);
    std::filesystem::remove_all(otherDir);
}

TEST(DocumentGraphicsTest, FallsBackToAbsolutePath)
{
    const auto sourceDir = makeTempDir("musxdom_graphics_source_missing");
    const auto absoluteDir = makeTempDir("musxdom_graphics_absolute_only");
    const auto filename = std::filesystem::path("graphic.png");
    const auto absoluteGraphic = absoluteDir / filename;
    writeFile(absoluteGraphic, "absolute");

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(sourceDir / "score.musx");

    const auto xml = buildGraphicsXml(absoluteGraphic.string());
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    ASSERT_TRUE(resolved.has_value());
    EXPECT_EQ(std::filesystem::weakly_canonical(*resolved), std::filesystem::weakly_canonical(absoluteGraphic));

    std::filesystem::remove_all(sourceDir);
    std::filesystem::remove_all(absoluteDir);
}

#if defined(MUSX_RUNNING_ON_MACOS)
TEST(DocumentGraphicsTest, ResolvesFromMacUrlBookmark)
{
    const auto sourceDir = makeTempDir("musxdom_graphics_bookmark_source");
    const auto targetDir = makeTempDir("musxdom_graphics_bookmark_target");
    const auto filename = std::filesystem::path("graphic.png");
    const auto targetGraphic = targetDir / filename;
    writeFile(targetGraphic, "bookmark");

    CFURLRef url = CFURLCreateFromFileSystemRepresentation(
        kCFAllocatorDefault,
        reinterpret_cast<const UInt8*>(targetGraphic.u8string().c_str()),
        static_cast<CFIndex>(targetGraphic.u8string().size()),
        false);
    ASSERT_TRUE(url != nullptr);
    CFDataRef bookmarkData = CFURLCreateBookmarkData(
        kCFAllocatorDefault,
        url,
        kCFURLBookmarkCreationMinimalBookmarkMask,
        nullptr,
        nullptr,
        nullptr);
    CFRelease(url);
    ASSERT_TRUE(bookmarkData != nullptr);

    std::vector<std::uint8_t> bookmarkBytes(static_cast<size_t>(CFDataGetLength(bookmarkData)));
    if (!bookmarkBytes.empty()) {
        CFDataGetBytes(bookmarkData, CFRangeMake(0, CFDataGetLength(bookmarkData)), bookmarkBytes.data());
    }
    CFRelease(bookmarkData);

    const std::string bookmarkHex = hexEncode(bookmarkBytes);
    const auto xml = buildBookmarkGraphicsXml(filename.string(), bookmarkHex, bookmarkBytes.size());

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(sourceDir / "score.musx");
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    ASSERT_TRUE(resolved.has_value());
    EXPECT_EQ(std::filesystem::weakly_canonical(*resolved), std::filesystem::weakly_canonical(targetGraphic));

    std::filesystem::remove_all(sourceDir);
    std::filesystem::remove_all(targetDir);
}

TEST(DocumentGraphicsTest, ResolvesFromMacAlias)
{
    const auto sourceDir = makeTempDir("musxdom_graphics_alias_source");
    const auto targetDir = makeTempDir("musxdom_graphics_alias_target");
    const auto filename = std::filesystem::path("graphic.png");
    const auto targetGraphic = targetDir / filename;
    writeFile(targetGraphic, "alias");

    auto aliasBytes = createAliasRecordBytes(targetGraphic);
    std::string aliasHex;
    size_t aliasLength = 0;
    bool expectResolve = false;
    if (aliasBytes) {
        aliasHex = hexEncode(*aliasBytes);
        aliasLength = aliasBytes->size();
        expectResolve = true;
    } else {
        aliasHex = "00";
        aliasLength = 1;
    }

    const auto xml = buildAliasGraphicsXml(filename.string(), aliasHex, aliasLength);

    musx::factory::DocumentFactory::CreateOptions options;
    options.setSourcePath(sourceDir / "score.musx");
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(
        xml.data(), xml.size(), std::move(options));

    auto resolved = doc->resolveExternalGraphicPath(1);
    if (expectResolve) {
        ASSERT_TRUE(resolved.has_value());
        EXPECT_EQ(std::filesystem::weakly_canonical(*resolved), std::filesystem::weakly_canonical(targetGraphic));
    } else {
        EXPECT_FALSE(resolved.has_value());
    }

    std::filesystem::remove_all(sourceDir);
    std::filesystem::remove_all(targetDir);
}
#endif
