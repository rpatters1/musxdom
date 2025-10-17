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
#include <string>
#include <vector>
#include <cstdlib>
#include <exception>

#include "musx/musx.h"

namespace musx {
namespace dom {

// *********************
// ***** TextsBase *****
// *********************

util::EnigmaParsingContext TextsBase::getRawTextCtx(const MusxInstance<TextsBase>& ptrToThis, Cmper forPartId, std::optional<Cmper> forPageId,
    util::EnigmaString::TextInsertCallback defaultInsertFunc) const
{
    return util::EnigmaParsingContext(ptrToThis, forPartId, forPageId, defaultInsertFunc);
}

namespace texts {

// **************************
// ***** LyricsTextBase *****
// **************************

void LyricsTextBase::createSyllableInfo(const MusxInstance<TextsBase>& ptrToThis)
{
    std::string current;
    bool inSeparator = false;
    bool currSeparatorHasHyphen = false;
    bool lastSeparatorHadHyphen = false;

    syllables.clear();
    syllableStyles.clear();
    auto parsingContext = getRawTextCtx(ptrToThis, SCORE_PARTID);
    parsingContext.parseEnigmaText([&](const std::string& nextChunk, const util::EnigmaStyles& styles) -> bool {
        syllableStyles.push_back(styles);
        for (auto c : nextChunk) {
            if (c == '-' || isspace(static_cast<unsigned char>(c))) {
                if (c == '-') {
                    currSeparatorHasHyphen = true;
                }
                inSeparator = true;
            }
            else {
                if (inSeparator) {
                    if (!current.empty()) {
                        syllables.push_back(std::shared_ptr<const LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen, syllableStyles.size() - 1)));
                        current.clear();
                    }
                    lastSeparatorHadHyphen = currSeparatorHasHyphen;
                    currSeparatorHasHyphen = false;
                    inSeparator = false;
                }
                current += c;
            }
        }
        return true;
    });

    if (!current.empty()) {
        syllables.push_back(MusxInstance<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen, syllableStyles.size() - 1)));
    }
}

std::optional<util::EnigmaStyles> LyricsTextBase::getStylesForSyllable(size_t syllableIndex)
{
    if (syllableIndex >= syllables.size()) {
        return std::nullopt;
    }
    const auto& syllable = syllables[syllableIndex];
    if (syllable->m_enigmaStylesIndex >= syllableStyles.size()) {
        return std::nullopt;
    }
    return syllableStyles[syllable->m_enigmaStylesIndex];
}

} // namespace texts
} // namespace dom
} // namespace musx