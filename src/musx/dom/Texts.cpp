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
    std::vector<LyricsSyllableInfo::StyleSpan> currentEnigmaStyles;
    bool inSeparator = false;
    bool currSeparatorHasHyphen = false;
    bool lastSeparatorHadHyphen = false;

    syllables.clear();
    syllableStyles.clear();
    auto parsingContext = getRawTextCtx(ptrToThis, SCORE_PARTID);
    parsingContext.parseEnigmaText([&](const std::string& nextChunk, const util::EnigmaStyles& styles) -> bool {
        syllableStyles.push_back(styles.createDeepCopy());
        currentEnigmaStyles.push_back({ current.size(), current.size(), syllableStyles.size() - 1 });
        for (auto c : nextChunk) {
            if (c == '-' || isspace(static_cast<unsigned char>(c))) {
                if (c == '-') {
                    currSeparatorHasHyphen = true;
                }
                inSeparator = true;
            } else {
                if (inSeparator) {
                    if (!current.empty()) {
                        syllables.push_back(std::shared_ptr<const LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen, std::move(currentEnigmaStyles))));
                        current.clear();
                        currentEnigmaStyles.clear();
                        currentEnigmaStyles.push_back({ current.size(), current.size(), syllableStyles.size() - 1 });
                    }
                    lastSeparatorHadHyphen = currSeparatorHasHyphen;
                    currSeparatorHasHyphen = false;
                    inSeparator = false;
                }
                current += c;
                currentEnigmaStyles.back().end++;
            }
        }
        return true;
    });

    if (!current.empty()) {
        syllables.push_back(MusxInstance<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen, std::move(currentEnigmaStyles))));
    }
}

bool LyricsTextBase::iterateStylesForSyllable(size_t syllableIndex, util::EnigmaString::TextChunkCallback callback) const
{
    if (syllableIndex >= syllables.size()) {
        return false;
    }
    const auto& syllable = syllables[syllableIndex];
    for (const auto& span : syllable->m_enigmaStyleMap) {
        MUSX_ASSERT_IF(span.start > syllable->syllable.size() || span.end > syllable->syllable.size()) {
            throw std::logic_error("syllable's enigmaStyles map contained out-of-range start or end values.");
        }
        MUSX_ASSERT_IF(span.start > span.end) {
            throw std::logic_error("syllable's enigmaStyles map contained start value greater than end value.");
        }
        if (span.start == syllable->syllable.size()) {
            continue;
        }
        MUSX_ASSERT_IF(span.styleIndex >= syllableStyles.size()) {
            throw std::logic_error("syllable's enigmaStyles map contained out-of-range styleIndex.");
        }
        std::string_view segment(syllable->syllable.data() + span.start, span.end - span.start);
        const auto& style = syllableStyles.at(span.styleIndex);
        if (!callback(std::string(segment), style)) {
            return false;
        }
    }
    return true;
}

} // namespace texts
} // namespace dom
} // namespace musx