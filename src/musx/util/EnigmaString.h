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
#pragma once

#include <string>
#include <memory>

#include "musx/dom/BaseClasses.h"

using namespace musx::dom;

namespace musx {
namespace util {

/**
 * @brief Static class that provides utilties to extract information from enigma strings
 */
class EnigmaString
{
public:
    static bool startsWithFontCommand(const std::string& text)
    {
        const std::vector<std::string> textCmds = { "^font", "^fontid", "^Font", "^fontMus", "^fontTxt", "^fontNum", "^size", "^nfx" };
        for (const auto& textCmd : textCmds) {
            if (text.rfind(textCmd, 0) == 0) { // Checks if text starts with textCmd
                return true;
            }
        }
        return false;
    }
    
    static std::vector<std::string> parseEnigmaComponents(const std::string& input)
    {
        if (input.empty() || input[0] != '^')
            return {}; // Invalid input

        if (input.size() == 2 && input[1] == '^') {
            return { "^" }; // "^^" returns "^"
        }

        size_t i = 1; // Start after '^'
        while (i < input.size() && std::isalpha(input[i])) 
            ++i;

        if (i == 1) 
            return {}; // No valid command found

        std::vector<std::string> components;
        components.push_back(input.substr(1, i - 1)); // Extract command

        if (i < input.size() && input[i] == '(') {
            size_t start = ++i, depth = 1;
            while (i < input.size() && depth > 0) {
                if (input[i] == '(') 
                    ++depth;
                else if (input[i] == ')') 
                    --depth;
                ++i;
            }

            if (depth != 0) 
                return {}; // Unbalanced parentheses

            std::string params = input.substr(start, i - start - 1);
            size_t j = 0, paramStart = 0, parenDepth = 0;

            // Split parameters by ',' while respecting nested parentheses
            while (j <= params.size()) {
                if (j == params.size() || (params[j] == ',' && parenDepth == 0)) {
                    if (j > paramStart) 
                        components.push_back(params.substr(paramStart, j - paramStart));
                    paramStart = j + 1;
                } else if (params[j] == '(') 
                    ++parenDepth;
                else if (params[j] == ')') 
                    --parenDepth;
                ++j;
            }
        }

        return components;
    }

    static bool parseFontCommand(const std::string& fontTag, FontInfo& fontInfo) {
        if (fontTag.empty() || fontTag[0] != '^') {
            return false;
        }

        std::vector<std::string> components = parseEnigmaComponents(fontTag);
        if (components.size() < 2) {
            return false;
        }

        const std::string& commandPart = components[0];
        if (commandPart == "fontMus" || commandPart == "fontTxt" || commandPart == "fontNum" || commandPart == "font" || commandPart == "fontid") {
            const std::string& param1 = components[1];
            if (commandPart == "fontid") {
                fontInfo.fontId = std::stoi(param1);
            } else if (param1.find("Font") == 0) { // Starts with "Font"
                fontInfo.fontId = std::stoi(param1.substr(4));
            } else {
                fontInfo.setFontIdByName(param1);
            }
            return true;
        } else if (commandPart == "nfx") {
            fontInfo.setEnigmaStyles(std::stoi(components[1]));
            return true;
        } else if (commandPart == "size") {
            fontInfo.fontSize = std::stoi(components[1]);
            return true;
        }

        return false;
    }
};

} // namespace util
} // namespace musx
