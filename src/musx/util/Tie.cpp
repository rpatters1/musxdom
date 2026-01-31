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
#include "musx/musx.h"

using namespace musx::dom;

namespace musx {
namespace util {

CurveContourDirection Tie::calcDefaultDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entryFrame = entryInfo.getFrame();
    const auto thisNote = noteInfo.operator->();

    if (forTieEnd) {
        if (!thisNote->tieEnd) {
            return CurveContourDirection::Unspecified;
        }
    } else {
        if (!thisNote->tieStart) {
            return CurveContourDirection::Unspecified;
        }
    }

    const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>();
    size_t noteCount = entryInfo->getEntry()->notes.size();
    const bool upStem = entryInfo.calcUpStem();

    auto tryMixedStemDirection = [&]() -> std::optional<CurveContourDirection> {
        std::optional<bool> adjacentUpStem;
        if (forTieEnd) {
            // There seems to be a "bug" in how Finale (as of 2K) determines mixed-stem values for Tie-Ends.
            // It looks at the stem direction of the immediately preceding entry, even if that entry
            // is not the entry that started the tie. Therefore, do not use TiedFromNote() to
            // get the stem direction.
            if (EntryInfoPtr prevEntry = entryInfo.getPreviousInLayer()) {
                adjacentUpStem = prevEntry.calcUpStem();
            }
        } else {
            if (NoteInfoPtr endNote = noteInfo.calcTieTo()) {
                adjacentUpStem = endNote.getEntryInfo().calcUpStem();
            } else {
                // Finale (as of v2K) has the following Mickey Mouse behavior. When no Tie-To note exists,
                // it determines the mixed stem value based on
                //      1. If the next entry is a rest, the adjStemDir is indeterminate so use stemDir
                //      2. If the next entry is a note with its stem frozen, use it
                //      3. If the next entry floats, but it has a V2Launch, then if EITHER the V1 or
                //              the V2 has a stem in the opposite direction, use it.
                auto nextEntry = entryInfo.getNextInLayer();
                if (nextEntry && !nextEntry.calcDisplaysAsRest()) {
                    auto nextStem = nextEntry.calcUpStem();
                    auto [freezeStem, freezeDir] = nextEntry.calcEntryStemSettings();
                    if (!freezeStem && nextEntry->getEntry()->v2Launch && nextStem == upStem) {
                        nextEntry = nextEntry.getNextInLayer();
                        if (nextEntry) {
                            nextStem = nextEntry.calcUpStem();
                        }
                    }
                    adjacentUpStem = nextStem;
                }
            }
        }
        if (!adjacentUpStem.has_value()) {
            return std::nullopt;
        }
        if (tieOptions->mixedStemDirection == options::TieOptions::MixedStemDirection::OppositeFirst) {
            return std::nullopt;
        }
        if (*adjacentUpStem == upStem) {
            return std::nullopt;
        }
        const bool tieUp = tieOptions->mixedStemDirection == options::TieOptions::MixedStemDirection::Over;
        return tieUp ? CurveContourDirection::Up : CurveContourDirection::Down;
    };

    if (noteCount > 1) {
        const bool opposingSeconds = tieOptions->chordTieDirOpposingSeconds;
        auto applyOpposingSeconds = [&](CurveContourDirection direction) {
            if (!opposingSeconds || direction == CurveContourDirection::Unspecified) {
                return direction;
            }
            if (direction == CurveContourDirection::Up && !thisNote->upStemSecond && thisNote->downStemSecond) {
                return CurveContourDirection::Down;
            }
            if (direction == CurveContourDirection::Down && thisNote->upStemSecond && !thisNote->downStemSecond) {
                return CurveContourDirection::Up;
            }
            return direction;
        };

        // This code depends on observed Finale behavior that the notes are always sorted
        // from lowest-to-highest inside the entry. If Finale's behavior ever changes, this
        // code is screwed.
        const size_t noteIndex = noteInfo.getNoteIndex();

        if (noteIndex == 0) {
            if (auto mixed = tryMixedStemDirection()) {
                return *mixed;
            }
            return CurveContourDirection::Down;
        }
        if (noteIndex + 1 == noteCount) {
            if (auto mixed = tryMixedStemDirection()) {
                return *mixed;
            }
            return CurveContourDirection::Up;
        }

        if (tieOptions->chordTieDirType != options::TieOptions::ChordTieDirType::StemReversal) {
            if (noteIndex < noteCount / 2) {
                return applyOpposingSeconds(CurveContourDirection::Down);
            }
            if (noteIndex >= (noteCount + 1) / 2) {
                return applyOpposingSeconds(CurveContourDirection::Up);
            }
            if (tieOptions->chordTieDirType == options::TieOptions::ChordTieDirType::OutsideInside) {
                return applyOpposingSeconds(upStem ? CurveContourDirection::Down : CurveContourDirection::Up);
            }
        }

        const int staffPos = std::get<3>(noteInfo.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
        const auto staff = entryInfo.createCurrentStaff();
        int stemReversalPos = staff->stemReversal;
        return applyOpposingSeconds((staffPos < stemReversalPos) ? CurveContourDirection::Down : CurveContourDirection::Up);
    }

    if (auto mixed = tryMixedStemDirection()) {
        return *mixed;
    }

    return upStem ? CurveContourDirection::Down : CurveContourDirection::Up;
}

CurveContourDirection Tie::calcEffectiveDirection(const dom::NoteInfoPtr& noteInfo, bool forTieEnd)
{
    if (const auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->freezeDirection) {
            return tieAlter->down ? CurveContourDirection::Down : CurveContourDirection::Up;
        }
    }

    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entryFrame = entryInfo.getFrame();
    const auto entry = entryInfo->getEntry();

    if (entry->splitStem) {
        return noteInfo->upSplitStem ? CurveContourDirection::Up : CurveContourDirection::Down;
    }

    if (const auto layerInfo = entryFrame->getLayerAttributes()) {
        if (layerInfo->freezeLayer && entryInfo.calcIfLayerSettingsApply()) {
            const bool tieUp = layerInfo->freezeStemsUp == layerInfo->freezTiesToStems;
            return tieUp ? CurveContourDirection::Up : CurveContourDirection::Down;
        }
    }

    if (entry->v2Launch || entry->voice2) {
        return entryInfo.calcUpStem() ? CurveContourDirection::Up : CurveContourDirection::Down;
    }

    if (entry->flipTie) {
        return entryInfo.calcUpStem() ? CurveContourDirection::Up : CurveContourDirection::Down;
    }

    const auto scrollViewStaves = entryFrame->getDocument()->getScrollViewStaves(entryFrame->getRequestedPartId());
    const int crossStaffDir = noteInfo.calcCrossStaffDirection(scrollViewStaves);
    if (crossStaffDir != 0) {
        return (crossStaffDir > 0) ? CurveContourDirection::Up : CurveContourDirection::Down;
    }

    return calcDefaultDirection(noteInfo, forTieEnd);
}

std::optional<TieConnectStyleType> Tie::calcConnectStyleType(const dom::NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entryFrame = entryInfo.getFrame();
    const auto entry = entryInfo->getEntry();

    const auto direction = noteInfo.calcEffectiveTieDirection(forTieEnd);
    if (direction == CurveContourDirection::Unspecified) {
        return std::nullopt;
    }
    const bool isLowest = noteInfo.calcIsBottom();
    const bool isHighest = noteInfo.calcIsTop();

    const bool useOuter = [&]() -> bool {
        if (!isLowest && !isHighest) {
            return false;
        }
        if (!isLowest || !isHighest) {
            if (isLowest && direction != CurveContourDirection::Down) {
                return false;
            }
            if (isHighest && direction != CurveContourDirection::Up) {
                return false;
            }
        }
        if (auto tieAlts = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
            if (tieAlts->outerLocal) {
                return tieAlts->outerOn;
            }
        }
        if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
            return tieOptions->useOuterPlacement;
        }
        return true;
    }();

    const bool isOver = direction == CurveContourDirection::Up;
    const bool isUnder = direction == CurveContourDirection::Down;
    const bool isStartPos = !forTieEnd;

    if (useOuter) {
        const bool hasStem = entry->hasStem();
        if (hasStem) {
            const bool upStem = entryInfo.calcUpStem();
            const auto thisNote = noteInfo.operator->();
            const bool upSecBit = thisNote->upStemSecond;
            const bool dwSecBit = thisNote->downStemSecond;

            bool useOuterStem = false;
            if (forTieEnd) {
                if (!upStem && isUnder && !dwSecBit) {
                    useOuterStem = true;
                } else if (upStem && isOver && upSecBit) {
                    useOuterStem = true;
                }
            } else {
                if (upStem && isOver && !upSecBit) {
                    useOuterStem = true;
                } else if (!upStem && isUnder && dwSecBit) {
                    useOuterStem = true;
                }
            }

            if (useOuterStem) {
                if (isOver) {
                    return isStartPos
                        ? TieConnectStyleType::OverHighestNoteStemStartPosOver
                        : TieConnectStyleType::OverHighestNoteStemEndPosOver;
                }
                return isStartPos
                    ? TieConnectStyleType::UnderLowestNoteStemStartPosUnder
                    : TieConnectStyleType::UnderLowestNoteStemEndPosUnder;
            }
        }

        if (isOver) {
            return isStartPos
                ? TieConnectStyleType::OverHighestNoteStartPosOver
                : TieConnectStyleType::OverHighestNoteEndPosOver;
        }
        return isStartPos
            ? TieConnectStyleType::UnderLowestNoteStartPosUnder
            : TieConnectStyleType::UnderLowestNoteEndPosUnder;
    }

    if (isOver) {
        return isStartPos
            ? TieConnectStyleType::OverStartPosInner
            : TieConnectStyleType::OverEndPosInner;
    }
    return isStartPos
        ? TieConnectStyleType::UnderStartPosInner
        : TieConnectStyleType::UnderEndPosInner;
}

} // namespace util
} // namespace musx
