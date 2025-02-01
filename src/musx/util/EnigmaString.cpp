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
#include <regex>

#include "musx/musx.h"

using namespace musx::dom;

namespace musx {
namespace util {

static const std::vector<std::string> kEnigmaFontCommands = { "^font", "^fontid", "^Font", "^fontMus", "^fontTxt", "^fontNum", "^size", "^nfx" };

bool EnigmaString::startsWithFontCommand(const std::string& text)
{
    for (const auto& textCmd : kEnigmaFontCommands) {
        if (text.rfind(textCmd, 0) == 0) { // Checks if text starts with textCmd
            return true;
        }
    }
    return false;
}

std::vector<std::string> EnigmaString::parseComponents(const std::string& input)
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

bool EnigmaString::parseFontCommand(const std::string& fontTag, FontInfo& fontInfo)
{
    if (fontTag.empty() || fontTag[0] != '^') {
        return false;
    }

    std::vector<std::string> components = parseComponents(fontTag);
    if (components.size() < 2) {
        return false;
    }

    const std::string& commandPart = components[0];
    if (commandPart == "fontMus" || commandPart == "fontTxt" || commandPart == "fontNum" || commandPart == "font" || commandPart == "fontid") {
        const std::string& param1 = components[1];
        if (commandPart == "fontid") {
            fontInfo.fontId = Cmper(std::stoi(param1));
        } else if (param1.find("Font") == 0) { // Starts with "Font"
            fontInfo.fontId = Cmper(std::stoi(param1.substr(4)));
        } else {
            fontInfo.setFontIdByName(param1);
        }
        return true;
    } else if (commandPart == "nfx") {
        fontInfo.setEnigmaStyles(uint16_t(std::stoi(components[1])));
        return true;
    } else if (commandPart == "size") {
        fontInfo.fontSize = std::stoi(components[1]);
        return true;
    }

    return false;
}

std::string EnigmaString::trimFontTags(const std::string& input)
{
    std::string output;
    std::regex fontTagRegex = []() {
        std::string pattern = R"(\^(?:)";
        for (const auto& command : kEnigmaFontCommands) {
            if (pattern.size() > 6) pattern += "|"; // Add '|' separator between commands
            pattern += std::regex_replace(command.substr(1), std::regex(R"([\^])"), R"(\^)"); // Escape '^' in each command
        }
        pattern += R"()\([^)]*\))"; // Match the enclosing parenthesis and their content
        return std::regex(pattern);
    }();
    output = std::regex_replace(input, fontTagRegex, "");
    return output;
}

std::string EnigmaString::trimTags(const std::string& input)
{
    std::string output;

    // Define the regex for Enigma tags: ^cmd(...)
    std::regex enigmaTagRegex(R"(\^[^(]+\([^)]*\))");
    std::string::size_type pos = 0;

    while (pos < input.size()) {
        if (input[pos] == '^') {
            if (pos + 1 < input.size() && input[pos + 1] == '^') {
                // Handle escaped caret: add a single caret to output
                output += '^';
                pos += 2; // Skip both carets
            } else {
                // Check for an Enigma tag
                std::smatch match;
                if (std::regex_search(input.begin() + pos, input.end(), match, enigmaTagRegex) &&
                    match.position() == 0) {
                    // Skip the matched Enigma tag
                    pos += match.length();
                } else {
                    // It's a lone caret or an invalid tag; add it to output
                    output += '^';
                    ++pos;
                }
            }
        } else {
            // Regular character; add it to output
            output += input[pos++];
        }
    }
    return output;
}

std::string EnigmaString::replaceAccidentalTags(const std::string& input, AccidentalStyle style)
{
    // Define regex for ^flat() and ^sharp()
    std::regex accidentalTagRegex(R"(\^(flat|sharp|natural)\(\))");

    // Maps for SMuFL and plain text accidentals
    const std::unordered_map<std::string, std::string> smuflAccidentals = {
        {"flat", u8"\uE260"},    // SMuFL character for flat
        {"sharp", u8"\uE262"},   // SMuFL character for sharp
        {"natural", u8"\uE261"}  // SMuFL character for natural
    };
    const std::unordered_map<std::string, std::string> unicodeAccidentals = {
        {"flat", u8"\u266D"},    // Text flat: ♭
        {"sharp", u8"\u266F"},   // Text sharp: ♯
        {"natural", u8"\u266E"}  // Text natural: ♮
    };
    const std::unordered_map<std::string, std::string> asciiAccidentals = {
        {"flat", "b"},         // Plain text representation for flat
        {"sharp", "#"},        // Plain text representation for sharp
        {"natural", ""},       // Plain text representation for natural (none)
    };
    const auto& accidentalMap = [&]() {
        switch (style) {
            default:            
            case AccidentalStyle::Ascii: return asciiAccidentals;
            case AccidentalStyle::Unicode: return unicodeAccidentals;
            case AccidentalStyle::Smufl: return smuflAccidentals;
        }
    }();

    std::string output;
    std::sregex_iterator currentMatch(input.begin(), input.end(), accidentalTagRegex);
    std::sregex_iterator endMatch;

    std::string::size_type lastPos = 0;

    // Iterate through all matches
    for (; currentMatch != endMatch; ++currentMatch) {
        const std::smatch& match = *currentMatch;

        // Append the portion of the string before the match
        output.append(input, lastPos, match.position() - lastPos);

        // Replace the match based on the captured group
        const auto& accidental = match[1].str();
        auto it = accidentalMap.find(accidental);
        if (it != accidentalMap.end()) {
            output += it->second;  // Append the replacement character
        }

        // Update last processed position
        lastPos = match.position() + match.length();
    }

    // Append the remainder of the input string
    output.append(input, lastPos, input.size() - lastPos);

    return output;
}

} // namespace util
} // namespace musx
