/*
 * Copyright (C) 2024, Robert Patterson
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
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <sstream>

 // This header includes method implementations that need to see all the classes in the dom

#include "musx/musx.h"

#if ! defined(MUSX_RUNNING_ON_WINDOWS)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace musx {
namespace dom {

// ***********************
// ***** FontOptions *****
// ***********************

std::shared_ptr<FontInfo> options::FontOptions::getFontInfo(options::FontOptions::FontType type) const
{
    auto it = fontOptions.find(type);
    if (it == fontOptions.end()) {
        throw std::invalid_argument("Font type " + std::to_string(int(type)) + " not found in document");
    }
    return it->second;
}

std::shared_ptr<FontInfo> options::FontOptions::getFontInfo(const DocumentPtr& document, options::FontOptions::FontType type)
{
    auto options = document->getOptions();
    if (!options) {
        throw std::invalid_argument("No options found in document");
    }
    auto fontOptions = options->get<options::FontOptions>();
    if (!fontOptions) {
        throw std::invalid_argument("Default fonts not found in document");
    }
    return fontOptions->getFontInfo(type);
}

// ********************
// ***** FontInfo *****
// ********************

std::string FontInfo::getFontName() const
{
    auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(fontId);
    if (fontDef) {
        return fontDef->name;
    }
    throw std::invalid_argument("font definition not found for font id " + std::to_string(fontId));
}

void FontInfo::setFontIdByName(const std::string& name)
{
    auto fontDefs = getDocument()->getOthers()->getArray<others::FontDefinition>();
    for (auto fontDef : fontDefs) {
        if (fontDef->name == name) {
            fontId = fontDef->getCmper();
            return;
        }
    }
    throw std::invalid_argument("font definition not found for font \"" + name + "\"");
}

bool FontInfo::calcIsSMuFL() const
{
    auto name = getFontName();
    auto standardFontPaths = calcSMuFLPaths();
    for (const auto& path : standardFontPaths) {
        if (!path.empty()) {
            std::filesystem::path metaFilePath(path / name / name);
            metaFilePath.replace_extension(".json");
            if (std::filesystem::is_regular_file(metaFilePath)) {
                return true;
            }
        }
    }
    return false;
}

std::vector<std::filesystem::path> FontInfo::calcSMuFLPaths()
{
#if defined(MUSX_RUNNING_ON_WINDOWS)
    auto systemEnv = "COMMONPROGRAMFILES";
    auto userEnv = "LOCALAPPDATA";
#elif defined(MUSX_RUNNING_ON_MACOS)
    auto systemEnv = "";
    auto userEnv = "HOME";
#elif defined(MUSX_RUNNING_ON_LINUX_UNIX)
    auto systemEnv = "XDG_DATA_DIRS";
    auto userEnv = "XDG_DATA_HOME";
#else
    static_assert(false, "Unsupported OS for FontInfo::calcSMuFLPaths");
#endif

#if ! defined(MUSX_RUNNING_ON_WINDOWS)    
    auto getHomePath = []() -> std::string {
        auto homeEnv = getenv("HOME");
        if (homeEnv) {
            return homeEnv;
        }
        uid_t uid = getuid(); // Get the current user's UID
        struct passwd *pw = getpwuid(uid); // Fetch the password entry for the UID
        if (pw) {
            return pw->pw_dir;
        }
        return "";
    };
#else
    auto getHomePath = []() -> void {};
#endif
    
    auto getBasePaths = [getHomePath](const std::string& envVariable) -> std::vector<std::string> {
        std::vector<std::string> paths;
#if defined(MUSX_RUNNING_ON_WINDOWS)
        char* buffer = nullptr;
        size_t bufferSize = 0;
        if (_dupenv_s(&buffer, &bufferSize, envVariable.c_str()) == 0 && buffer != nullptr) {
            paths.emplace_back(buffer);
            free(buffer);
        } else {
            return {};
        }
#else
        if (envVariable == "HOME") {
            paths.emplace_back(getHomePath());
        } else if (!envVariable.empty()) {
            if (auto envValue = getenv(envVariable.c_str())) {
                std::stringstream ss(envValue);
                std::string path;
                while (std::getline(ss, path, ':')) {
                    paths.push_back(path);
                }
#if defined(MUSX_RUNNING_ON_LINUX_UNIX)
            } else if (envVariable == "XDG_DATA_HOME") {
                paths.emplace_back(getHomePath() + "/.local/share");
            } else if (envVariable == "XDG_DATA_DIRS") {
                paths.emplace_back("/usr/local/share");
                paths.emplace_back("/usr/share");
#endif         
            } else {
                return {};
            }
        }
        else {
            paths.emplace_back("/");
        }
#endif
        return paths;
    };
    auto paths = getBasePaths(userEnv);
    auto temp = getBasePaths(systemEnv);
    paths.insert(paths.end(),
                 std::make_move_iterator(temp.begin()),
                 std::make_move_iterator(temp.end()));
    std::vector<std::filesystem::path> retval;
    for (const auto& next : paths) {
        std::filesystem::path path = next;
#if defined(MUSX_RUNNING_ON_MACOS)
        path = path / "Library" / "Application Support";
#endif
        path = path / "SMuFL" / "Fonts";
        retval.emplace_back(std::move(path));
    }
    return retval;
}

// ****************************
// ***** MarkingCategiory *****
// ****************************

std::string others::MarkingCategory::getName() const
{
    auto catName = getDocument()->getOthers()->get<others::MarkingCategoryName>(getCmper());
    if (catName) {
        return catName->name;
    }
    return {};
}

// ********************
// ***** TextBase *****
// ********************

std::shared_ptr<FontInfo> TextsBase::parseFirstFontInfo() const
{
        std::string searchText = this->text;
        auto fontInfo = std::make_shared<FontInfo>(this->getDocument());
        bool foundTag = false;

        while (true) {
            if (!musx::util::EnigmaString::startsWithFontCommand(searchText)) {
                break;
            }

            size_t endOfTag = searchText.find_first_of(')');
            if (endOfTag == std::string::npos) {
                break;
            }

            std::string fontTag = searchText.substr(0, endOfTag + 1);
            if (!musx::util::EnigmaString::parseFontCommand(fontTag, *fontInfo.get())) {
                return nullptr;
            }

            searchText.erase(0, endOfTag + 1);
            foundTag = true;
        }

        if (foundTag) {
            return fontInfo;
        }

        return nullptr;
}

// *****************************
// ***** TextExpressionDef *****
// *****************************

std::shared_ptr<others::Enclosure> others::TextExpressionDef::getEnclosure() const
{
    if (!hasEnclosure) return nullptr;
    return getDocument()->getOthers()->get<others::TextExpressionEnclosure>(getCmper());
}

} // namespace dom    
} // namespace musx
