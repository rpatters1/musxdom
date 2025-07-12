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
#include <regex>
#include <unordered_map>
#include <cctype>
#include <string>

#include "musx/musx.h"

using namespace musx::dom;

namespace musx {
namespace util {

std::string EnigmaString::toU8(char32_t cp)
{
    std::string result;
    if (cp <= 0x7F) {
        result += static_cast<char>(cp);
    } else if (cp <= 0x7FF) {
        result += static_cast<char>(0xC0 | (cp >> 6));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        result += static_cast<char>(0xE0 | (cp >> 12));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp <= 0x10FFFF) {
        result += static_cast<char>(0xF0 | (cp >> 18));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    // Invalid code points are ignored (returning empty string).
    return result;
}

static const std::unordered_map<std::string_view, options::TextOptions::InsertSymbolType> acciInsertMap = {
    { "flat",       options::TextOptions::InsertSymbolType::Flat },
    { "natural",    options::TextOptions::InsertSymbolType::Natural },
    { "sharp",      options::TextOptions::InsertSymbolType::Sharp },
    { "dbflat",     options::TextOptions::InsertSymbolType::DblFlat },
    { "dbsharp",    options::TextOptions::InsertSymbolType::DblSharp },
};

static const std::unordered_map<options::TextOptions::InsertSymbolType, std::string>& getEnigmaAccidentalMap(EnigmaString::AccidentalStyle style)
{
    // Maps for SMuFL and plain text accidentals
    static const std::unordered_map<options::TextOptions::InsertSymbolType, std::string> smuflAccidentals = {
        { options::TextOptions::InsertSymbolType::Flat,     EnigmaString::fromU8(u8"\uE260") }, // SMuFL character for flat
        { options::TextOptions::InsertSymbolType::Sharp,    EnigmaString::fromU8(u8"\uE262") }, // SMuFL character for sharp
        { options::TextOptions::InsertSymbolType::Natural,  EnigmaString::fromU8(u8"\uE261") }, // SMuFL character for natural
        { options::TextOptions::InsertSymbolType::DblFlat,  EnigmaString::fromU8(u8"\uE264") }, // SMuFL double flat
        { options::TextOptions::InsertSymbolType::DblSharp, EnigmaString::fromU8(u8"\uE263") }, // SMuFL double sharp
    };
    static const std::unordered_map<options::TextOptions::InsertSymbolType, std::string> unicodeAccidentals = {
        { options::TextOptions::InsertSymbolType::Flat,     EnigmaString::fromU8(u8"\u266D") },   // Text flat: ♭
        { options::TextOptions::InsertSymbolType::Sharp,    EnigmaString::fromU8(u8"\u266F") },   // Text sharp: ♯
        { options::TextOptions::InsertSymbolType::Natural,  EnigmaString::fromU8(u8"\u266E") },   // Text natural: ♮
        { options::TextOptions::InsertSymbolType::DblFlat,  EnigmaString::fromU8(u8"\u1D12B") },  // Text double flat: 𝄫
        { options::TextOptions::InsertSymbolType::DblSharp, EnigmaString::fromU8(u8"\u1D12A") },  // Text double sharp: 𝄪
    };
    static const std::unordered_map<options::TextOptions::InsertSymbolType, std::string> asciiAccidentals = {
        { options::TextOptions::InsertSymbolType::Flat,     "b"  }, // Plain text representation for flat
        { options::TextOptions::InsertSymbolType::Sharp,    "#"  }, // Plain text representation for sharp
        { options::TextOptions::InsertSymbolType::Natural,  ""   }, // Plain text representation for natural (none)
        { options::TextOptions::InsertSymbolType::DblFlat,  "bb" }, // Plain text double flat
        { options::TextOptions::InsertSymbolType::DblSharp, "x"  }, // Plain text double sharp
    };

    switch (style) {
        default:            
        case EnigmaString::AccidentalStyle::Ascii: return asciiAccidentals;
        case EnigmaString::AccidentalStyle::Unicode: return unicodeAccidentals;
        case EnigmaString::AccidentalStyle::Smufl: return smuflAccidentals;
    }
}

bool EnigmaString::startsWithFontCommand(const std::string& text)
{
    static const std::vector<std::string> kEnigmaFontCommands = { "^font", "^fontid", "^Font", "^fontMus", "^fontTxt", "^fontNum", "^size", "^nfx" };

    for (const auto& textCmd : kEnigmaFontCommands) {
        if (text.rfind(textCmd, 0) == 0) { // Checks if text starts with textCmd
            return true;
        }
    }
    return false;
}

bool EnigmaString::startsWithStyleCommand(const std::string& text)
{
    static const std::vector<std::string> kEnigmaStyleCommands = { "^baseline", "^superscript", "^tracking" };
    if (startsWithFontCommand(text)) {
        return true;
    }
    for (const auto& textCmd : kEnigmaStyleCommands) {
        if (text.rfind(textCmd, 0) == 0) { // Checks if text starts with textCmd
            return true;
        }
    }
    return false;
}

std::vector<std::string> EnigmaString::parseComponents(const std::string& input, size_t* parsedLength)
{
    if (parsedLength) {
        *parsedLength = 0;
    }
    if (input.empty() || input[0] != '^') {
        return {}; // Invalid input
    }

    if (input.size() >= 2 && input[1] == '^') {
        if (parsedLength) {
            *parsedLength = 2;
        }
        return { "^" }; // "^^" returns "^"
    }

    size_t i = 1; // Start after '^'
    while (i < input.size() && std::isalpha(input[i])) 
        ++i;

    if (i == 1) {
        return {}; // No valid command found
    }

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

    // No parenthesis group — command is just ^command
    if (parsedLength) {
        *parsedLength = i;
    }
    return components;
}

bool EnigmaString::parseStyleCommand(std::vector<std::string> components, EnigmaStyles& styles)
{
    static const std::unordered_map<std::string_view, EnigmaStyles::CategoryTracking> trackingMap = {
        { "fontMus", EnigmaStyles::CategoryTracking::MusicFont },
        { "fontTxt", EnigmaStyles::CategoryTracking::TextFont},
        { "fontNum", EnigmaStyles::CategoryTracking::NumberFont },
    };

    if (components.size() < 2) {
        return false;
    }

    const std::string& commandPart = components[0];
    if (commandPart == "fontMus" || commandPart == "fontTxt" || commandPart == "fontNum" || commandPart == "font" || commandPart == "Font" || commandPart == "fontid") {
        const std::string& param1 = components[1];
        if (commandPart == "fontid") {
            styles.font->fontId = Cmper(std::stoi(param1));
        } else if (param1.find("Font") == 0) { // font name starts with "Font"
            const auto fontIdStr = param1.substr(4);
            if (!fontIdStr.empty() && std::all_of(fontIdStr.begin(), fontIdStr.end(), ::isdigit)) {
                styles.font->fontId = Cmper(std::stoi(fontIdStr));
            } else {
                styles.font->setFontIdByName(param1);
            }
        } else {
            styles.font->setFontIdByName(param1);
        }
        auto it = trackingMap.find(commandPart);
        if (it != trackingMap.end()) {
            styles.categoryFont = it->second;
        } else {
            styles.categoryFont = EnigmaStyles::CategoryTracking::None;
        }
        return true;
    } else if (commandPart == "nfx") {
        styles.font->setEnigmaStyles(uint16_t(std::stoi(components[1])));
        return true;
    } else if (commandPart == "size") {
        styles.font->fontSize = std::stoi(components[1]);
        return true;
    } else if (commandPart == "baseline") {
        styles.baseline = std::stoi(components[1]);
        return true;
    } else if (commandPart == "superscript") {
        styles.superscript = std::stoi(components[1]);
        return true;
    } else if (commandPart == "tracking") {
        styles.tracking = std::stoi(components[1]);
        return true;
    }

    return false;
}

bool EnigmaString::parseEnigmaTextImpl(const std::shared_ptr<dom::Document>& document, Cmper forPartId, const std::string& rawText,
    const TextChunkCallback& onText, const TextInsertCallback& onInsert, const EnigmaParsingOptions& options, const EnigmaStyles& startingStyles)
{
    auto currentStyles = startingStyles;
    std::string remaining = rawText;
    std::optional<std::string> textBuffer;

    auto addToBuf = [&](const std::string& text) {
        if (textBuffer) {
            textBuffer->append(text);
        } else {
            textBuffer.emplace(text);
        }
    };

    auto processChunk = [&](const EnigmaStyles& styles) -> bool {
        if (textBuffer.has_value() && !textBuffer->empty()) {
            bool result = onText(textBuffer.value(), styles);
            textBuffer = std::nullopt;
            if (!result) {
                return false;
            }
        }
        textBuffer.emplace(""); // after parsing a style command, make sure the style change is reported even if no text.
        return true;
    };

    while (!remaining.empty()) {
        size_t caretPos = remaining.find('^');

        // Emit text before next command
        if (caretPos != std::string::npos && caretPos > 0) {
            addToBuf(remaining.substr(0, caretPos));
            remaining.erase(0, caretPos);
        } else if (caretPos == std::string::npos) {
            addToBuf(remaining);
            break;
        }

        size_t parsedLen = 0;
        // Try to parse the next command
        auto components = parseComponents(remaining, &parsedLen);

        if (startsWithStyleCommand(remaining)) {
            if (!options.ignoreStyleTags) {
                if (!processChunk(currentStyles)) {
                    return false;
                }
                if (!parseStyleCommand(components, currentStyles)) {
                    throw std::invalid_argument("malformed style command encountered in Enigma text: " + rawText);
                }
            }
            remaining.erase(0, parsedLen);
            continue;
        }

        if (components.empty() || parsedLen == 0) {
            // Not a valid command — treat '^' as literal
            addToBuf("^");
            remaining.erase(0, 1);
            continue;
        }

        std::string fullCommand = remaining.substr(0, parsedLen);
        remaining.erase(0, parsedLen);

        // Handle ^^ (escaped caret)
        if (components.size() == 1 && components[0] == "^") {
            addToBuf("^");
            continue;
        }

        // Send command to the handler and use that if the handler handles it.
        std::optional<std::string> replacement = onInsert(components);
        if (replacement.has_value()) {
            addToBuf(replacement.value());
            continue;
        }

        auto insertIt = acciInsertMap.find(components[0]);
        if (insertIt != acciInsertMap.end()) {
            switch (options.insertHandling) {
                case AccidentalInsertHandling::Substitute:
                {
                    const auto& accidentalMap = getEnigmaAccidentalMap(options.substitutionStyle);
                    auto it = accidentalMap.find(insertIt->second);
                    if (it != accidentalMap.end()) {
                        addToBuf(it->second);
                        continue;
                    }
                    break;
                }
                case AccidentalInsertHandling::ParseToGlyphs:
                {
                    if (const auto textOptions = document->getOptions()->get<options::TextOptions>()) {
                        const auto& acciDataIt = textOptions->symbolInserts.find(insertIt->second);
                        if (acciDataIt != textOptions->symbolInserts.end()) {
                            EnigmaStyles acciStyles(document);
                            const auto& insertInfo = acciDataIt->second;
                            *acciStyles.font.get() = *insertInfo->symFont.get();
                            acciStyles.font->fontSize = int(std::lround(double(insertInfo->symFont->fontSize) * double(currentStyles.font->fontSize) / 100.0));
                            acciStyles.baseline = int(std::lround((double(insertInfo->baselineShiftPerc) * double(currentStyles.font->fontSize)) * (EVPU_PER_POINT / 100.0)));
                            acciStyles.tracking = insertInfo->trackingBefore;
                            if (!processChunk(currentStyles)) {
                                return false;
                            }
                            textBuffer.emplace(toU8(insertInfo->symChar));
                            if (!processChunk(acciStyles)) {
                                return false;
                            }
                        } else {
                            MUSX_INTEGRITY_ERROR("Document contains no accidental insert options for " + components[0] + ".");
                        }
                    } else {
                        MUSX_INTEGRITY_ERROR("Document contains no text options.");
                    }
                    continue;
                }
                default: break;
            }
        }

        auto processDate = [&](std::time_t time) -> std::string {
            util::DateTime::DateFormatStyle style = util::DateTime::DateFormatStyle::Short;
            if (components.size() > 1) {
                int i = std::stoi(components[1]);
                if (i >= 0 && i <= 2) {
                    style = util::DateTime::DateFormatStyle(i);
                } else {
                    MUSX_INTEGRITY_ERROR("Enigma string encounted tag " + components[0] + " with input value " + components[1]);
                }
            }
            return util::DateTime::formatDate(time, style);
        };

        if (options.ignoreTags.find(components[0]) != options.ignoreTags.end()) {
            continue;
        } else if (components[0] == "arranger") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Arranger))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "composer") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Composer))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "copyright") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Copyright))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "cprsym") {
            addToBuf("@");
        } else if (components[0] == "date") {
            addToBuf(processDate(std::time(nullptr)));
        } else if (components[0] == "description") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Description))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "fdate") {
            const auto& modified = document->getHeader()->modified;
            std::time_t fdate = util::DateTime::makeTimeT(modified.year, modified.month, modified.day);
            addToBuf(processDate(fdate));
        } else if (components[0] == "lyricist") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Lyricist))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "page") {
            addToBuf("#");
        } else if (components[0] == "partname") {
            if (auto linkedPart = document->getOthers()->get<others::PartDefinition>(SCORE_PARTID, forPartId)) {
                if (auto nameRawText = linkedPart->getNameRawTextCtx().getRawText()) {
                    EnigmaParsingOptions partnameOptions = options;
                    partnameOptions.ignoreStyleTags = true;
                    partnameOptions.ignoreTags = { "partname" };
                    if (!processChunk(currentStyles)) {
                        break;
                    }
                    bool parseResult = parseEnigmaTextImpl(document, forPartId, nameRawText->text, onText, onInsert, partnameOptions, currentStyles);
                    if (!parseResult) {
                        return false;
                    }
                }
            }
        } else if (components[0] == "subtitle") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Subtitle))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "time") {
            bool includeSeconds = false;
            if (components.size() > 1) {
                includeSeconds = bool(std::stoi(components[1]));
            }
            addToBuf(util::DateTime::formatTime(std::time(nullptr), includeSeconds));
        } else if (components[0] == "title") {
            if (auto textInsert = document->getTexts()->get<texts::FileInfoText>(Cmper(texts::FileInfoText::TextType::Title))) {
                addToBuf(trimTags(textInsert->text));
            }
        } else if (components[0] == "totpages") {
            auto pages = document->getOthers()->getArray<others::Page>(forPartId);
            addToBuf(std::to_string(pages.size()));
        } else {
            // fall-thru causes unhandled command to be stripped or inserted, based on configuration options
            if (!options.stripUnknownTags) {
                addToBuf(fullCommand);
            }
        }
    }

    // Emit any remaining buffered text
    if (textBuffer.has_value()) {
        onText(textBuffer.value(), currentStyles);
    }

    return true;
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

bool EnigmaStringContext::parseEnigmaText(const util::EnigmaString::TextChunkCallback& onText, const util::EnigmaString::TextInsertCallback& onInsert,
    const util::EnigmaString::EnigmaParsingOptions& options) const
{
    if (!m_rawText) {
        return false;
    }
    return util::EnigmaString::parseEnigmaText(m_rawText->getDocument(), m_forPartId, m_rawText->text, onText, [&](const std::vector<std::string>& components) -> std::optional<std::string> {
        if (auto result = onInsert(components)) {
            return result;
        }
        if (auto result = m_insertFunc(components)) {
            return result;
        }
        if (m_forPageNumber.has_value()) {
            if (components[0] == "page") {
                int pageOffset = components.size() > 1 ? std::stoi(components[1]) : 0;
                return std::to_string(pageOffset + m_forPageNumber.value());
            }
        }
        return std::nullopt;
    }, options);
}

std::string EnigmaStringContext::getText(bool trimTags, util::EnigmaString::AccidentalStyle accidentalStyle,
    const std::unordered_set<std::string_view>& ignoreTags) const
{
    util::EnigmaString::EnigmaParsingOptions options(accidentalStyle);
    options.stripUnknownTags = trimTags;
    options.ignoreTags = ignoreTags;
    std::string result;
    parseEnigmaText([&](const std::string& text, const musx::util::EnigmaStyles&) -> bool {
            result += text;
            return true;
        }, options);
    return result;
}

std::shared_ptr<FontInfo> EnigmaStringContext::parseFirstFontInfo() const
{
    if (!m_rawText || !musx::util::EnigmaString::startsWithFontCommand(m_rawText->text)) {
        return nullptr;
    }
    std::shared_ptr<FontInfo> result;
    util::EnigmaString::parseEnigmaText(m_rawText->getDocument(), m_forPartId, m_rawText->text, [&](const std::string&, const util::EnigmaStyles& styles) {
        result = styles.font;
        return false;
    });
    return result;
}

} // namespace util
} // namespace musx
