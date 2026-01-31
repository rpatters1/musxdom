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

namespace {

NoteInfoPtr calcTiedTo(const NoteInfoPtr& noteInfo, bool requireTie)
{
    if (!noteInfo) {
        return {};
    }
    if (requireTie && !noteInfo->tieStart) {
        return {};
    }
    auto tiedTo = noteInfo.calcTieTo();
    if (requireTie && tiedTo && !tiedTo->tieEnd) {
        return {};
    }
    return tiedTo;
}

enum class TiePlacement
{
    OverInner,
    UnderInner,
    OverOuterNote,
    UnderOuterNote,
    OverOuterStem,
    UnderOuterStem,
    Unknown
};

TiePlacement calcPlacementFromConnectStyle(TieConnectStyleType type)
{
    switch (type) {
        case TieConnectStyleType::OverStartPosInner:
        case TieConnectStyleType::OverEndPosInner:
            return TiePlacement::OverInner;
        case TieConnectStyleType::UnderStartPosInner:
        case TieConnectStyleType::UnderEndPosInner:
            return TiePlacement::UnderInner;
        case TieConnectStyleType::OverHighestNoteStartPosOver:
        case TieConnectStyleType::OverHighestNoteEndPosOver:
            return TiePlacement::OverOuterNote;
        case TieConnectStyleType::UnderLowestNoteStartPosUnder:
        case TieConnectStyleType::UnderLowestNoteEndPosUnder:
            return TiePlacement::UnderOuterNote;
        case TieConnectStyleType::OverHighestNoteStemStartPosOver:
        case TieConnectStyleType::OverHighestNoteStemEndPosOver:
            return TiePlacement::OverOuterStem;
        case TieConnectStyleType::UnderLowestNoteStemStartPosUnder:
        case TieConnectStyleType::UnderLowestNoteStemEndPosUnder:
            return TiePlacement::UnderOuterStem;
    }
    return TiePlacement::Unknown;
}

TieConnectStyleType toConnectStyleType(TiePlacement placement, bool isStartPos)
{
    switch (placement) {
        case TiePlacement::OverInner:
            return isStartPos ? TieConnectStyleType::OverStartPosInner : TieConnectStyleType::OverEndPosInner;
        case TiePlacement::UnderInner:
            return isStartPos ? TieConnectStyleType::UnderStartPosInner : TieConnectStyleType::UnderEndPosInner;
        case TiePlacement::OverOuterNote:
            return isStartPos ? TieConnectStyleType::OverHighestNoteStartPosOver
                              : TieConnectStyleType::OverHighestNoteEndPosOver;
        case TiePlacement::UnderOuterNote:
            return isStartPos ? TieConnectStyleType::UnderLowestNoteStartPosUnder
                              : TieConnectStyleType::UnderLowestNoteEndPosUnder;
        case TiePlacement::OverOuterStem:
            return isStartPos ? TieConnectStyleType::OverHighestNoteStemStartPosOver
                              : TieConnectStyleType::OverHighestNoteStemEndPosOver;
        case TiePlacement::UnderOuterStem:
            return isStartPos ? TieConnectStyleType::UnderLowestNoteStemStartPosUnder
                              : TieConnectStyleType::UnderLowestNoteStemEndPosUnder;
        case TiePlacement::Unknown:
            break;
    }
    return isStartPos ? TieConnectStyleType::OverStartPosInner : TieConnectStyleType::OverEndPosInner;
}

bool calcEntryHasNonAlignedSecond(const EntryInfoPtr& entryInfo)
{
    const auto entry = entryInfo->getEntry();
    for (const auto& note : entry->notes) {
        if (note->upStemSecond || note->downStemSecond) {
            return true;
        }
    }
    return false;
}

bool calcNoteIsNonAlignedSecond(const NoteInfoPtr& noteInfo, bool upStem)
{
    const auto note = noteInfo.operator->();
    return upStem ? note->upStemSecond : note->downStemSecond;
}

unsigned calcEntryDots(const EntryInfoPtr& entryInfo)
{
    return entryInfo->getEntry()->calcDurationInfo().second;
}

bool calcNoteHasAccidental(const NoteInfoPtr& noteInfo)
{
    const auto note = noteInfo.operator->();
    return note->showAcci;
}

bool calcUseAfterSingleDot(const NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryFrame = noteInfo.getEntryInfo().getFrame();
    if (auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->afterSingleDotLocal) {
            return tieAlter->afterSingleDotOn;
        }
    }
    if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
        return tieOptions->afterSingleDot;
    }
    return false;
}

bool calcUseAfterMultipleDots(const NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryFrame = noteInfo.getEntryInfo().getFrame();
    if (auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->afterMultiDotsLocal) {
            return tieAlter->afterMultiDotsOn;
        }
    }
    if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
        return tieOptions->afterMultipleDots;
    }
    return false;
}

bool calcUseBeforeSingleAcci(const NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryFrame = noteInfo.getEntryInfo().getFrame();
    if (auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->beforeSingleAcciLocal) {
            return tieAlter->beforeSingleAcciOn;
        }
    }
    if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
        return tieOptions->beforeAcciSingleNote;
    }
    return false;
}

bool calcShiftForSeconds(const NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryFrame = noteInfo.getEntryInfo().getFrame();
    if (auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->shiftForSecondsLocal) {
            return tieAlter->shiftForSecondsOn;
        }
    }
    if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
        return tieOptions->secondsPlacement == options::TieOptions::SecondsPlacement::ShiftForSeconds;
    }
    return false;
}

bool calcUseOuterPlacement(const NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto entryFrame = noteInfo.getEntryInfo().getFrame();
    if (auto tieAlter = details::TieAlterBase::fromNoteInfo(noteInfo, forTieEnd)) {
        if (tieAlter->outerLocal) {
            return tieAlter->outerOn;
        }
    }
    if (const auto tieOptions = entryFrame->getDocument()->getOptions()->get<options::TieOptions>()) {
        return tieOptions->useOuterPlacement;
    }
    return true;
}

TiePlacement calcPlacementForEndpoint(const NoteInfoPtr& noteInfo, bool forTieEnd, CurveContourDirection direction, bool stemUp,
    bool forEndPoint, std::optional<size_t> noteIndexOverride = std::nullopt, std::optional<size_t> noteCountOverride = std::nullopt,
    std::optional<bool> upStemSecondOverride = std::nullopt, std::optional<bool> downStemSecondOverride = std::nullopt)
{
    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entry = entryInfo->getEntry();
    const auto note = noteInfo.operator->();

    const size_t noteIndex = noteIndexOverride.value_or(noteInfo.getNoteIndex());
    const size_t noteCount = noteCountOverride.value_or(entry->notes.size());
    const bool upStemSecond = upStemSecondOverride.value_or(note->upStemSecond);
    const bool downStemSecond = downStemSecondOverride.value_or(note->downStemSecond);

    if ((noteIndex == 0 && direction == CurveContourDirection::Down)
        || (noteIndex + 1 == noteCount && direction == CurveContourDirection::Up)) {
        bool useOuter = calcUseOuterPlacement(noteInfo, forTieEnd);
        if (useOuter) {
            if (entry->hasStem()) {
                if (forEndPoint) {
                    if (!stemUp && direction == CurveContourDirection::Down && !downStemSecond) {
                        return TiePlacement::UnderOuterStem;
                    }
                    if (stemUp && direction == CurveContourDirection::Up && upStemSecond) {
                        return TiePlacement::OverOuterStem;
                    }
                } else {
                    if (stemUp && direction == CurveContourDirection::Up && !upStemSecond) {
                        return TiePlacement::OverOuterStem;
                    }
                    if (!stemUp && direction == CurveContourDirection::Down && downStemSecond) {
                        return TiePlacement::UnderOuterStem;
                    }
                }
            }
            return (direction == CurveContourDirection::Down) ? TiePlacement::UnderOuterNote : TiePlacement::OverOuterNote;
        }
    }

    return (direction == CurveContourDirection::Down) ? TiePlacement::UnderInner : TiePlacement::OverInner;
}

std::optional<MeasCmper> calcLastMeasureId(const DocumentPtr& document, Cmper partId)
{
    std::optional<MeasCmper> lastMeasure;
    const auto pages = document->getOthers()->getArray<others::Page>(partId);
    for (const auto& page : pages) {
        if (page->lastMeasureId) {
            if (!lastMeasure || page->lastMeasureId.value() > *lastMeasure) {
                lastMeasure = page->lastMeasureId.value();
            }
        }
    }
    return lastMeasure;
}

bool calcIsEndOfSystem(const NoteInfoPtr& noteInfo, bool forPageView)
{
    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entryFrame = entryInfo.getFrame();
    const auto document = entryFrame->getDocument();

    if (!entryInfo.getNextInLayer()) {
        if (const auto lastMeasure = calcLastMeasureId(document, entryFrame->getRequestedPartId())) {
            if (entryInfo.getMeasure() == *lastMeasure) {
                return true;
            }
        }
    }

    if (forPageView) {
        if (auto endNote = calcTiedTo(noteInfo, true)) {
            const auto startSystem = document->calcSystemFromMeasure(entryFrame->getRequestedPartId(), entryInfo.getMeasure());
            const auto endSystem = document->calcSystemFromMeasure(entryFrame->getRequestedPartId(), endNote.getEntryInfo().getMeasure());
            if (startSystem && endSystem && startSystem->getCmper() != endSystem->getCmper()) {
                return true;
            }
        }
    }

    return false;
}

} // namespace

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

std::optional<TieConnectStyleType> Tie::calcConnectStyleTypeAtEndPoint(
    const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint)
{
    if (!noteInfo) {
        return std::nullopt;
    }
    if ((forTieEnd && !noteInfo->tieEnd) || (!forTieEnd && !noteInfo->tieStart)) {
        return std::nullopt;
    }
    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entry = entryInfo->getEntry();

    auto direction = noteInfo.calcEffectiveTieDirection(forTieEnd);
    if (direction == CurveContourDirection::Unspecified) {
        return std::nullopt;
    }

    const bool stemUp = entryInfo.calcUpStem();
    TiePlacement startPlacement = calcPlacementForEndpoint(noteInfo, forTieEnd, direction, stemUp, false);
    TiePlacement endPlacement = startPlacement;

    if (forTieEnd) {
        endPlacement = calcPlacementForEndpoint(noteInfo, forTieEnd, direction, stemUp, true);
    } else {
        if (auto endNote = calcTiedTo(noteInfo, true)) {
            const bool endStemUp = endNote.getEntryInfo().calcUpStem();
            endPlacement = calcPlacementForEndpoint(endNote, forTieEnd, direction, endStemUp, true);
        } else {
            auto nextEntry = entryInfo.getNextInLayer();
            if (nextEntry) {
                if (!nextEntry.calcDisplaysAsRest() && !nextEntry->getEntry()->notes.empty()) {
                    if (direction == CurveContourDirection::Down) {
                        NoteInfoPtr nextNote(nextEntry, 0);
                        const int nextStaffPos = std::get<3>(nextNote.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
                        const int currStaffPos = std::get<3>(noteInfo.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
                        if (nextStaffPos < currStaffPos) {
                            endPlacement = TiePlacement::UnderInner;
                        } else {
                            const bool nextStemUp = nextEntry.calcUpStem();
                            endPlacement = calcPlacementForEndpoint(nextNote, forTieEnd, direction, nextStemUp, true);
                        }
                    } else {
                        const auto nextEntryNoteCount = nextEntry->getEntry()->notes.size();
                        NoteInfoPtr nextNote(nextEntry, nextEntryNoteCount - 1);
                        const int nextStaffPos = std::get<3>(nextNote.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
                        const int currStaffPos = std::get<3>(noteInfo.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
                        if (nextStaffPos > currStaffPos) {
                            endPlacement = TiePlacement::OverInner;
                        } else {
                            bool upStemSecond = nextNote->upStemSecond;
                            bool downStemSecond = nextNote->downStemSecond;
                            bool nextStemUp = nextEntry.calcUpStem();
                            if (nextStemUp) {
                                for (size_t noteIndex = 0; noteIndex < nextEntryNoteCount; noteIndex++) {
                                    NoteInfoPtr checkNote(nextEntry, noteIndex);
                                    if (checkNote->upStemSecond) {
                                        upStemSecond = true;
                                    }
                                }
                                nextStemUp = (direction == CurveContourDirection::Up);
                            }
                            endPlacement = calcPlacementForEndpoint(
                                nextNote, forTieEnd, direction, nextStemUp, true,
                                nextNote.getNoteIndex(), nextEntryNoteCount, upStemSecond, downStemSecond);
                        }
                    }
                } else {
                    const bool nextStemUp = (direction == CurveContourDirection::Up);
                    endPlacement = calcPlacementForEndpoint(noteInfo, forTieEnd, direction, nextStemUp, true,
                        noteInfo.getNoteIndex(), entry->notes.size(), false, false);
                }
            } else {
                if (calcIsEndOfSystem(noteInfo, false)) {
                    endPlacement = (direction == CurveContourDirection::Down) ? TiePlacement::UnderOuterStem : TiePlacement::OverOuterStem;
                } else {
                    endPlacement = (direction == CurveContourDirection::Down) ? TiePlacement::UnderInner : TiePlacement::OverInner;
                }
            }
        }
    }

    if (startPlacement == TiePlacement::OverInner || startPlacement == TiePlacement::UnderInner) {
        endPlacement = startPlacement;
    } else if (endPlacement == TiePlacement::OverInner || endPlacement == TiePlacement::UnderInner) {
        startPlacement = endPlacement;
    }

    return forEndPoint ? toConnectStyleType(endPlacement, false) : toConnectStyleType(startPlacement, true);
}

std::pair<TieConnectStyleType, TieConnectStyleType> Tie::calcConnectStyleTypes(const dom::NoteInfoPtr& noteInfo, bool forTieEnd)
{
    const auto startStyle = calcConnectStyleTypeAtEndPoint(noteInfo, forTieEnd, false)
        .value_or(TieConnectStyleType::OverStartPosInner);
    const auto endStyle = calcConnectStyleTypeAtEndPoint(noteInfo, forTieEnd, true)
        .value_or(TieConnectStyleType::OverEndPosInner);
    return { startStyle, endStyle };
}

std::optional<details::TieAlterBase::ConnectionType> Tie::calcConnectionType(
    const dom::NoteInfoPtr& noteInfo, bool forTieEnd, bool forEndPoint, bool forPageView)
{
    const auto direction = noteInfo.calcEffectiveTieDirection(forTieEnd);
    if (direction == CurveContourDirection::Unspecified) {
        return std::nullopt;
    }

    if (!forEndPoint && forTieEnd) {
        return details::TieAlterBase::ConnectionType::SystemStart;
    }
    if (forEndPoint && !forTieEnd && calcIsEndOfSystem(noteInfo, forPageView)) {
        return details::TieAlterBase::ConnectionType::SystemEnd;
    }

    const auto [startStyle, endStyle] = calcConnectStyleTypes(noteInfo, forTieEnd);
    const auto connectStyle = forEndPoint ? endStyle : startStyle;

    const auto entryInfo = noteInfo.getEntryInfo();
    const auto entry = entryInfo->getEntry();
    const bool upStem = entryInfo.calcUpStem();
    const bool noteIsNonAligned = calcNoteIsNonAlignedSecond(noteInfo, upStem);
    const bool hasNonAligned = calcEntryHasNonAlignedSecond(entryInfo);
    const bool shiftForSeconds = calcShiftForSeconds(noteInfo, forTieEnd);

    const bool tieOver = direction == CurveContourDirection::Up;
    const bool tieUnder = direction == CurveContourDirection::Down;

    switch (calcPlacementFromConnectStyle(connectStyle)) {
        case TiePlacement::OverInner:
        case TiePlacement::UnderInner:
            if (forEndPoint) {
                if (calcUseBeforeSingleAcci(noteInfo, forTieEnd) && entry->notes.size() == 1 && calcNoteHasAccidental(noteInfo)) {
                    return details::TieAlterBase::ConnectionType::AccidentalLeftNoteCenter;
                }
                if (shiftForSeconds && hasNonAligned) {
                    if ((upStem && !tieUnder && noteIsNonAligned) || (!upStem && !noteIsNonAligned)) {
                        return details::TieAlterBase::ConnectionType::NoteLeftNoteCenter;
                    }
                }
                return details::TieAlterBase::ConnectionType::EntryLeftNoteCenter;
            }

            {
                const unsigned numDots = calcEntryDots(entryInfo);
                if ((calcUseAfterSingleDot(noteInfo, forTieEnd) && numDots == 1)
                    || (calcUseAfterMultipleDots(noteInfo, forTieEnd) && numDots > 1)) {
                    return details::TieAlterBase::ConnectionType::DotRightNoteCenter;
                }
            }

            if (shiftForSeconds && hasNonAligned) {
                if ((upStem && !noteIsNonAligned) || (!upStem && !tieOver && noteIsNonAligned)) {
                    return details::TieAlterBase::ConnectionType::NoteRightNoteCenter;
                }
            }
            return details::TieAlterBase::ConnectionType::EntryRightNoteCenter;

        case TiePlacement::OverOuterNote:
            return details::TieAlterBase::ConnectionType::NoteCenterNoteTop;
        case TiePlacement::UnderOuterNote:
            return details::TieAlterBase::ConnectionType::NoteCenterNoteBottom;
        case TiePlacement::OverOuterStem:
            return forEndPoint
                ? details::TieAlterBase::ConnectionType::NoteLeftNoteTop
                : details::TieAlterBase::ConnectionType::NoteRightNoteTop;
        case TiePlacement::UnderOuterStem:
            return forEndPoint
                ? details::TieAlterBase::ConnectionType::NoteLeftNoteBottom
                : details::TieAlterBase::ConnectionType::NoteRightNoteBottom;
        case TiePlacement::Unknown:
            break;
    }

    return std::nullopt;
}

} // namespace util
} // namespace musx
