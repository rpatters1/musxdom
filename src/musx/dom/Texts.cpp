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
#include <cctype>
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

namespace {

bool isLyricsWhitespace(unsigned char value)
{
    // U+0001 and U+0006 have been seen in legacy Finale lyric text where they
    // behave as whitespace. Treat other control characters this way as they
    // are confirmed in the data.
    return value == 0x01 || value == 0x06 || (value < 128 && std::isspace(value) != 0);
}

} // namespace

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
    bool sepSawSpaceOrUnderscore = false;
    int underscores = 0;

    const bool stripUnderscores = [&]() {
        if (auto lyricOptions = getDocument()->getOptions()->get<options::LyricOptions>()) {
            return lyricOptions->useSmartWordExtensions;
        }
        return false;
    }();

    // ptrToThis is guaranteed to actually point to this LyricsTextBase: createSyllableInfo is
    // only ever called (by the factory, or directly) with ptrToThis pointing to the same object
    // it is a method of.
    const MusxInstance<LyricsTextBase> self = std::static_pointer_cast<const LyricsTextBase>(ptrToThis);

    syllables.clear();
    m_syllableStyles.clear();
    auto parsingContext = getRawTextCtx(ptrToThis, SCORE_PARTID);
    parsingContext.parseEnigmaText([&](const std::string& nextChunk, const util::EnigmaStyles& styles) -> bool {
        m_syllableStyles.push_back(styles.createDeepCopy());
        currentEnigmaStyles.push_back({ current.size(), current.size(), m_syllableStyles.size() - 1 });
        for (auto c : nextChunk) {
            const unsigned char uc = static_cast<unsigned char>(c);
            if (c == '-' || isLyricsWhitespace(uc) || (stripUnderscores && c == '_')) {
                if (!inSeparator) {
                    inSeparator = true;
                    sepSawSpaceOrUnderscore = false;
                }
                if (c == '-') {
                    // Count hyphen only if it immediately follows an existing syllable,
                    // and no space/underscore has appeared yet in this separator run.
                    if (!sepSawSpaceOrUnderscore && !current.empty()) {
                        currSeparatorHasHyphen = true;
                    }
                } else {
                    if (c == '_') {
                        underscores++;
                    }
                    sepSawSpaceOrUnderscore = true;
                }
            } else {
                if (inSeparator) {
                    if (!current.empty()) {
                        syllables.push_back(MusxInstance<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), self, current, lastSeparatorHadHyphen, currSeparatorHasHyphen, underscores, std::move(currentEnigmaStyles))));
                        current.clear();
                        currentEnigmaStyles.clear();
                        currentEnigmaStyles.push_back({ current.size(), current.size(), m_syllableStyles.size() - 1 });
                    }
                    lastSeparatorHadHyphen = currSeparatorHasHyphen;
                    currSeparatorHasHyphen = false;
                    inSeparator = false;
                    sepSawSpaceOrUnderscore = false;
                    underscores = 0;
                }
                current += c;
                currentEnigmaStyles.back().end++;
            }
        }
        return true;
    });

    if (!current.empty()) {
        syllables.push_back(MusxInstance<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), self, current, lastSeparatorHadHyphen, currSeparatorHasHyphen, underscores, std::move(currentEnigmaStyles))));
    }
}

bool LyricsTextBase::iterateStylesForSyllable(size_t syllableIndex, util::EnigmaString::TextChunkCallback callback) const
{
    if (syllableIndex >= syllables.size()) {
        return false;
    }
    return syllables[syllableIndex]->iterateStyles(std::move(callback));
}

} // namespace texts
} // namespace dom
} // namespace musx
