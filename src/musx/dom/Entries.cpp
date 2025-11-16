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

// *****************
// ***** Entry *****
// *****************

MusxInstance<Entry> Entry::getNext() const
{
    if (!m_next) return nullptr;
    auto retval = getDocument()->getEntries()->get(m_next);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Entry " + std::to_string(m_entnum) + " has next entry " + std::to_string(m_next) + " that does not exist.");
    }
    return retval;
}

MusxInstance<Entry> Entry::getPrevious() const
{
    if (!m_prev) return nullptr;
    auto retval = getDocument()->getEntries()->get(m_prev);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Entry " + std::to_string(m_entnum) + " has previous entry " + std::to_string(m_prev) + " that does not exist.");
    }
    return retval;
}

std::pair<NoteType, unsigned> calcNoteInfoFromEdu(Edu duration)
{
    if (duration < 1 || duration >= 0x10000) {
        throw std::invalid_argument("Duration is out of valid range for NoteType.");
    }

    // Find the most significant bit position
    Edu value = duration;
    Edu noteValueMsb = 1;
    while (value > 1) {
        value >>= 1;
        noteValueMsb <<= 1;
    }

    unsigned count = 0;
    for (Edu dotMsb = noteValueMsb >> 1; duration & dotMsb; dotMsb >>= 1) {
        count++;
    }

    return std::make_pair(NoteType(noteValueMsb), count);
}

// **********************
// ***** EntryFrame *****
// **********************

DocumentPtr EntryFrame::getDocument() const { return m_context->getDocument(); }

StaffCmper EntryFrame::getStaff() const { return m_context->getStaff(); }

MeasCmper EntryFrame::getMeasure() const { return m_context->getMeasure(); }

MusxInstance<others::LayerAttributes> EntryFrame::getLayerAttributes() const
{
    if (!m_cachedLayerAttributes) {
        m_cachedLayerAttributes = getDocument()->getOthers()->get<others::LayerAttributes>(getRequestedPartId(), getLayerIndex());
    }
    return m_cachedLayerAttributes;
}

EntryInfoPtr EntryFrame::getFirstInVoice(int voice) const
{
    bool forV2 = voice == 2;
    auto firstEntry = EntryInfoPtr(shared_from_this(), 0);
    if (firstEntry->getEntry()->voice2) {
        MUSX_INTEGRITY_ERROR("Entry frame for staff " + std::to_string(getStaff()) + " measure " + std::to_string(getMeasure())
            + " layer " + std::to_string(m_layerIndex + 1) + " starts with voice2.");
        if (!forV2) {
            firstEntry = firstEntry.getNextInVoice(voice);
        }
    } else if (forV2) {
        firstEntry = firstEntry.getNextInVoice(voice);
    }
    return firstEntry;
}

EntryInfoPtr EntryFrame::getLastInVoice(int voice) const
{
    bool forV2 = voice == 2;
    auto lastEntry = EntryInfoPtr(shared_from_this(), m_entries.size() - 1);
    if (!lastEntry || lastEntry->getEntry()->voice2 == forV2) {
        return lastEntry;
    }
    return lastEntry.getPreviousInVoice(voice);
}

std::shared_ptr<const EntryFrame> EntryFrame::getNext() const
{
    if (auto gfhold = details::GFrameHoldContext(getDocument(), getRequestedPartId(), getStaff(), getMeasure() + 1)) {
        return gfhold.createEntryFrame(m_layerIndex);
    }
    return nullptr;
}

std::shared_ptr<const EntryFrame> EntryFrame::getPrevious() const
{
    if (getMeasure() > 1) {
        if (auto gfhold = details::GFrameHoldContext(getDocument(), getRequestedPartId(), getStaff(), getMeasure() - 1)) {
            return gfhold.createEntryFrame(m_layerIndex);
        }
    }
    return nullptr;
}

MusxInstance<others::StaffComposite> EntryFrame::createCurrentStaff(Edu eduPosition, const std::optional<StaffCmper>& forStaffId) const
{
    const StaffCmper requestedStaffId = forStaffId.value_or(getStaff());
    if (eduPosition == 0 && getStartStaffInstance()->getCmper() == requestedStaffId) {
        return getStartStaffInstance();
    }
    return others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), requestedStaffId, getMeasure(), eduPosition);
}

MusxInstance<others::Measure> EntryFrame::getMeasureInstance() const
{
    return getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), getMeasure());
}

bool EntryFrame::calcIsCueFrame(bool includeVisibleInScore) const
{
    bool foundCueEntry = false;
    for (size_t x = 0; x < m_entries.size(); x++) {
        if (!m_entries[x]->getEntry()->isHidden) {
            EntryInfoPtr entryInfo(shared_from_this(), x);
            if (entryInfo.calcIsCue(includeVisibleInScore)) {
                foundCueEntry = true;
            } else {
                return false; // return false on the first non-cue visible entry found
            }
        }
    }
    return foundCueEntry;
}

bool EntryFrame::calcAreAllEntriesHiddenInFrame() const
{
    for (const auto& entry : m_entries) {
        if (!entry->getEntry()->isHidden) {
            /// @todo check if the entry is hidden by voiced parts.
            return false;
        }
    }
    return true;
}

bool EntryFrame::TupletInfo::calcIsTremolo() const
{
    MUSX_ASSERT_IF(!tuplet) {
        throw std::logic_error("TupletInfo contains no tuplet.");
    }

    // must have at least 2 entries
    const auto entryCount = static_cast<int>(numEntries());
    if (entryCount < 2) {
        return false;
    }

    // must be invisible
    if (!tuplet->hidden) {
        if (tuplet->numStyle != details::TupletDef::NumberStyle::Nothing || tuplet->brackStyle != details::TupletDef::BracketStyle::Nothing) {
            return false;
        }
    }

    // entries must have the same duration and actual duration.
    auto frame = getParent();
    MUSX_ASSERT_IF(startIndex >= frame->getEntries().size()) {
        throw std::logic_error("TupletInfo instance contains invalid start index.");
    }
    MUSX_ASSERT_IF(endIndex >= frame->getEntries().size()) {
        throw std::logic_error("TupletInfo instance contains invalid end index.");
    }

    EntryInfoPtr first(frame, startIndex);
    const auto targetActual = first->actualDuration;
    const auto targetNotated = first->getEntry()->duration;

    // actual duration must be >= 2x notated duration.
    if (targetActual < util::Fraction::fromEdu(targetNotated) * 2) {
        return false;
    }
    
    // all entries must have the same notated and actual durations
    for (size_t i = startIndex + 1; i <= endIndex; ++i) {
        EntryInfoPtr curr(frame, i);
        if (curr->actualDuration != targetActual) {
            return false;
        }
        if (curr->getEntry()->duration != targetNotated) {
            return false;
        }
    }

    // entries must form a contiguous beamed-together chain
    // i.e., each entry's "next in beam group" must be exactly the next tuplet entry
    EntryInfoPtr chain(frame, startIndex);
    for (size_t i = startIndex + 1; i <= endIndex; ++i) {
        auto nextInBeam = chain.getNextInBeamGroup();
        if (!nextInBeam.isSameEntry(EntryInfoPtr(frame, i))) {
            return false;
        }
        chain = nextInBeam; // advance along the beamed chain
    }

    // if the actual duration of the tuplet is less than a half, at least one beam must be detached.
    if (tuplet->calcReferenceDuration().calcEduDuration() < Edu(NoteType::Half)) {
        auto targetNoteType = std::get<0>(calcNoteInfoFromEdu(targetNotated)); // C++17 complains about structured bindings captured in a lamda.
        auto checkBeamExt = [&](const MusxInstance<details::BeamExtension>& beamExt) -> bool {
            return beamExt && (beamExt->mask >= unsigned(targetNoteType)) && beamExt->leftOffset >= 0 && beamExt->rightOffset <= 0;
        };
        if (!checkBeamExt(frame->getDocument()->getDetails()->get<details::BeamExtensionUpStem>(frame->getRequestedPartId(), first->getEntry()->getEntryNumber()))) {
            if (!checkBeamExt(frame->getDocument()->getDetails()->get<details::BeamExtensionDownStem>(frame->getRequestedPartId(), first->getEntry()->getEntryNumber()))) {
                return false;
            }
        }
    }
    return true;
}

bool EntryFrame::TupletInfo::calcCreatesSingleton(bool left) const
{
    MUSX_ASSERT_IF(!tuplet) {
        throw std::logic_error("TupletInfo contains no tuplet.");
    }
    // must have exactly 1 entry
    if (startIndex != endIndex) {
        return false;
    }
    // must be invisible
    if (!tuplet->hidden) {
        if (tuplet->numStyle != details::TupletDef::NumberStyle::Nothing || tuplet->brackStyle != details::TupletDef::BracketStyle::Nothing) {
            return false;
        }
    }
    // must have zero length
    if (tuplet->calcRatio() != 0) {
        return false;
    }
    // entries must have the same duration and actual duration.
    auto frame = getParent();
    MUSX_ASSERT_IF(startIndex >= frame->getEntries().size()) {
        throw std::logic_error("TupletInfo instance contains invalid start index.");
    }
    EntryInfoPtr entryInfo(frame, startIndex);
    if (!entryInfo.calcIsBeamStart()) {
        return false;
    }
    auto hiddenEntryInfo = left ? entryInfo : entryInfo.getNextInBeamGroup();
    if (!hiddenEntryInfo) {
        return false;
    }
    auto hiddenEntry = hiddenEntryInfo->getEntry();
    // must be a non-rest, not hidden as a whole, have suppressed ledger lines, a custom stem, and hidden noteheads.
    if (!hiddenEntry->isNote || hiddenEntry->isHidden || !hiddenEntry->noLeger || !hiddenEntry->stemDetail || !hiddenEntry->noteDetail) {
        return false;
    }
    // must have manual note positioning in the correct direction.
    if (left) {
        if (hiddenEntry->hOffset >= 0) {
            return false;
        }
    } else {
        if (hiddenEntry->hOffset <= 0) {
            return false;
        }
    }
    // if either direction has a custom stem, check it
    MusxInstance<details::CustomStem> customStem = frame->getDocument()->getDetails()->get<details::CustomDownStem>(frame->getRequestedPartId(), hiddenEntry->getEntryNumber());
    if (!customStem) {
        customStem = frame->getDocument()->getDetails()->get<details::CustomUpStem>(frame->getRequestedPartId(), hiddenEntry->getEntryNumber());
    }
    if (!customStem || !customStem->calcIsHiddenStem()) {
        return false;
    }
    // check that all noteheads are hidden.
    for (size_t x = 0; x < hiddenEntry->notes.size(); x++) {
        NoteInfoPtr noteInfo(hiddenEntryInfo, x);
        if (!noteInfo) {
            return false;
        }
        auto noteheadMods = frame->getDocument()->getDetails()->getForNote<details::NoteAlterations>(noteInfo, frame->getRequestedPartId());
        if (!noteheadMods) {
            return false;
        }
        if (noteheadMods->altNhead != ' ' && (!noteheadMods->customFont || !noteheadMods->customFont->hidden)) {
            return false;
        }
        /// Beam Over Barlines plugin does not do anything about accidentals, so do no check for them.
    }
    return true;
}

bool EntryFrame::TupletInfo::calcCreatesTimeStretch() const
{
    MUSX_ASSERT_IF(!tuplet) {
        throw std::logic_error("TupletInfo contains no tuplet.");
    }
    // must be invisible
    if (!tuplet->hidden) {
        if (tuplet->numStyle != details::TupletDef::NumberStyle::Nothing || tuplet->brackStyle != details::TupletDef::BracketStyle::Nothing) {
            return false;
        }
    }
    auto frame = getParent();
    // staff must be independent timesig
    const auto staff = frame->createCurrentStaff(0);
    if (staff && !staff->floatTime) {
        return false;
    }
    // durations must match
    const auto measure = frame->getMeasureInstance();
    if (staff && measure) {
        if (tuplet->calcReferenceDuration() == measure->calcDuration()) {
            auto dispTime = measure->createDisplayTimeSignature(staff->getCmper());
            return tuplet->calcDisplayDuration() == dispTime->calcTotalDuration();
        }
    } else {
        MUSX_INTEGRITY_ERROR("Unable to get data record for Staff " + std::to_string(frame->getStaff()) + " or Measure " + std::to_string(frame->getMeasure()));
    }
    return false;
}

// ************************
// ***** EntryInfoPtr *****
// ************************

EntryInfoPtr EntryInfoPtr::fromPositionOrNull(const DocumentPtr& document, Cmper partId, StaffCmper staffId, MeasCmper measureId, EntryNumber entryNumber)
{
    EntryInfoPtr result;
    if (auto gfhold = details::GFrameHoldContext(document, partId, staffId, measureId)) {
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) {
            if (entryInfo->getEntry()->getEntryNumber() == entryNumber) {
                result = entryInfo;
                return false; // stop iterating
            }
            return true;
        });
    }
    return result;
}

const std::shared_ptr<const EntryInfo> EntryInfoPtr::operator->() const
{
    MUSX_ASSERT_IF(!m_entryFrame) {
        throw std::logic_error("EntryInfoPtr has no frame.");
    }
    MUSX_ASSERT_IF(m_indexInFrame >= m_entryFrame->getEntries().size()) {
        throw std::logic_error("Entry index is too large for entries array.");
    }
    return m_entryFrame->getEntries()[m_indexInFrame];
}

EntryInfoPtr::operator bool() const
{
    return m_entryFrame && m_indexInFrame < m_entryFrame->getEntries().size();
}

bool EntryInfoPtr::isSameEntry(const EntryInfoPtr& src) const
{
    if (!*this || !src) {
        return false;
    }
    return (*this)->getEntry()->getEntryNumber() == src->getEntry()->getEntryNumber();
}

LayerIndex  EntryInfoPtr::getLayerIndex() const { return m_entryFrame->getLayerIndex(); }

StaffCmper EntryInfoPtr::getStaff() const { return m_entryFrame->getStaff(); }

MeasCmper EntryInfoPtr::getMeasure() const { return m_entryFrame->getMeasure(); }

MusxInstance<KeySignature> EntryInfoPtr::getKeySignature() const { return m_entryFrame->keySignature; }

MusxInstance<others::StaffComposite> EntryInfoPtr::createCurrentStaff(const std::optional<StaffCmper>& forStaffId) const
{
    if (!m_cachedStaff || m_cachedStaff->getCmper() != forStaffId.value_or(m_entryFrame->getStaff())) {
        m_cachedStaff = m_entryFrame->createCurrentStaff((*this)->elapsedDuration.calcEduDuration(), forStaffId);
    }
    return m_cachedStaff;
}

unsigned EntryInfoPtr::calcReverseGraceIndex() const
{
    unsigned result = (*this)->graceIndex;
    if (result) {
        for (auto next = getNextInFrame(); next && next->graceIndex; next = next.getNextInFrame()) {
            result++;
        }
        result = result - (*this)->graceIndex + 1;
    }
    return result;
}

std::optional<size_t> EntryInfoPtr::calcNextTupletIndex(std::optional<size_t> currentIndex) const
{
    size_t firstIndex = currentIndex ? *currentIndex + 1 : 0;
    for (size_t x = firstIndex; x < m_entryFrame->tupletInfo.size(); x++) {
        const auto& tuplInf = m_entryFrame->tupletInfo[x];
        if (tuplInf.startIndex == m_indexInFrame) {
            return x;
        }
        if (tuplInf.startIndex > m_indexInFrame) {
            break;
        }
    }
    return std::nullopt;
}

EntryInfoPtr EntryInfoPtr::getNextInLayer() const
{
    if (auto resultInFrame = getNextInFrame()) {
        return resultInFrame;
    }
    if (auto nextFrame = m_entryFrame->getNext()) {
        return EntryInfoPtr(nextFrame, 0);
    }
    return EntryInfoPtr();
}

EntryInfoPtr EntryInfoPtr::getNextInFrame() const
{
    if (m_entryFrame && m_indexInFrame < m_entryFrame->getEntries().size() - 1) {
        return EntryInfoPtr(m_entryFrame, m_indexInFrame + 1);
    }
    return EntryInfoPtr();
}

EntryInfoPtr EntryInfoPtr::getNextSameV() const
{
    auto next = getNextInFrame();
    auto entry = (*this)->getEntry();
    if (entry->voice2) {
        if (next && next->getEntry()->voice2) {
            return next;
        }
        return EntryInfoPtr();
    }
    if (entry->v2Launch) {
        while (next && next->getEntry()->voice2) {
            next = next.getNextInFrame();
        }
    }
    return next;
}

EntryInfoPtr EntryInfoPtr::getNextSameVNoGrace() const
{
    for (auto next = getNextSameV(); next; next = next.getNextSameV()) {
        if (!next->getEntry()->graceNote) {
            return next;
        }
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::getPreviousInLayer() const
{
    if (auto resultInFrame = getPreviousInFrame()) {
        return resultInFrame;
    }
    if (auto prevFrame = m_entryFrame->getPrevious()) {
        return EntryInfoPtr(prevFrame, prevFrame->getEntries().size() - 1);
    }
    return EntryInfoPtr();
}

EntryInfoPtr EntryInfoPtr::getPreviousInFrame() const
{
    if (m_entryFrame && m_indexInFrame <= m_entryFrame->getEntries().size() && m_indexInFrame > 0) {
        return EntryInfoPtr(m_entryFrame, m_indexInFrame - 1);
    }
    return EntryInfoPtr();
}

EntryInfoPtr EntryInfoPtr::getPreviousSameV() const
{
    auto prev = getPreviousInFrame();
    if ((*this)->getEntry()->voice2) {
        if (prev && prev->getEntry()->voice2) {
            return prev;
        }
        return EntryInfoPtr();
    }
    while (prev && prev->getEntry()->voice2) {
        prev = prev.getPreviousInFrame();
    }
    return prev;
}

EntryInfoPtr EntryInfoPtr::getPreviousSameVNoGrace() const
{
    for (auto prev = getPreviousSameV(); prev; prev = prev.getPreviousSameV()) {
        if (!prev->getEntry()->graceNote) {
            return prev;
        }
    }
    return {};
}
EntryInfoPtr EntryInfoPtr::getNextInVoice(int voice) const
{
    bool forV2 = voice == 2;
    auto next = getNextInFrame();
    while (next && next->getEntry()->voice2 != forV2) {
        next = next.getNextInFrame();
    }
    return next;
}

EntryInfoPtr EntryInfoPtr::getPreviousInVoice(int voice) const
{
    bool forV2 = voice == 2;
    auto prev = getPreviousInFrame();
    while (prev && prev->getEntry()->voice2 != forV2) {
        prev = prev.getPreviousInFrame();
    }
    return prev;
}

EntryInfoPtr EntryInfoPtr::getNextInBeamGroupAcrossBars(bool includeHiddenEntries) const
{
    if (auto nextBarCont = calcBeamContinuesRightOverBarline()) {
        return nextBarCont;
    }
    return getNextInBeamGroup(includeHiddenEntries);
}

EntryInfoPtr EntryInfoPtr::getPreviousInBeamGroupAcrossBars(bool includeHiddenEntries) const
{
    if (auto prevBarCont = calcBeamContinuesLeftOverBarline()) {
        return prevBarCont;
    }
    return getPreviousInBeamGroup(includeHiddenEntries);
}

bool EntryInfoPtr::calcDisplaysAsRest() const
{
    return !(*this)->getEntry()->isNote;
    /// @todo Add code to detect that the entry is converted to a rest by voiced part settings
}

std::pair<int, int> EntryInfoPtr::calcTopBottomStaffPositions() const
{
    const auto& entry = (*this)->getEntry();
    MUSX_ASSERT_IF(entry->notes.empty() || entry->floatRest) {
        throw std::logic_error("calcTopBottomStaffPositions cannot be called for a floating rest.");
    }
    int topLine = (std::numeric_limits<int>::min)();
    int botLine = (std::numeric_limits<int>::max)();

    auto updateLines = [&](const NoteInfoPtr note) {
        const int staffLine = std::get<3>(note.calcNotePropertiesInView(/*alwaysUseEntryStaff*/ true));
        if (staffLine > topLine) {
            topLine = staffLine;
        }
        if (staffLine < botLine) {
            botLine = staffLine;
        }
    };

    updateLines(NoteInfoPtr(*this, 0));
    updateLines(NoteInfoPtr(*this, entry->notes.size() - 1));

    return std::make_pair(topLine, botLine);
}

bool EntryInfoPtr::calcUpStemDefault() const
{
    //stem direction is determined by the beam a note or rest is part of, if any, so
	//we must always look for a beam to calculate direction.
    auto beamStart = findBeamStartOrCurrent();

  	int maxTopDiff = (std::numeric_limits<int>::min)();
	int minBotDiff = (std::numeric_limits<int>::max)();
	int numAbove = 0;
	int numBelow = 0;
    bool gotNonGrace = false;
    bool gotNonFloatRest = false;

    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        const auto& entry = next->getEntry();
        if (!entry->graceNote) {
            gotNonGrace = true;
            if (entry->notes.empty() || entry->floatRest) {
                continue;
            }
            gotNonFloatRest = true;
            auto staff = next.createCurrentStaff();
            auto [topLine, botLine] = next.calcTopBottomStaffPositions();
            const int currTopDiff = topLine - staff->stemReversal;
            if (maxTopDiff < currTopDiff) {
                maxTopDiff = currTopDiff;
            }
            const int currBotDiff = botLine - staff->stemReversal;
            if (minBotDiff > currBotDiff) {
                minBotDiff = currBotDiff;
            }
            const int valForStem = std::abs(currBotDiff) > std::abs(currTopDiff) ? currBotDiff : currTopDiff;
            if (valForStem > 0) {
                numAbove++;
            } else if (valForStem < 0) {
                numBelow++;
            }
        }
    }

    if (!gotNonGrace) {
        return true;
    }

    if (!gotNonFloatRest) {
        return (*this)->getEntry()->upStem; // Use whatever Finale last calculated.
    }

    // for beams, if the diffs don't determine it, then the number above & below do
    // (Function calcUnBeamed is false for beamed groups.)
    if (minBotDiff < 0 && minBotDiff == -maxTopDiff && !calcUnbeamed()) {
        return numBelow > numAbove;
    }

    const int valForBeam = std::abs(minBotDiff) > std::abs(maxTopDiff) ? minBotDiff : maxTopDiff;

    return valForBeam < 0;
}

bool EntryInfoPtr::calcUpStem() const
{
    /// @todo Add cross-staff support some day?

    //stem direction is determined by the beam a note or rest is part of, if any, so
    //we must always look for a beam to calculate direction.
    auto beamStart = findBeamStartOrCurrent();
    const auto frame = getFrame();

    // This is the precedence as tested by RGP in ~2001.
    // rhythmic slash notation
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        auto staff = next.createCurrentStaff(); // the staff is cached by createCurrentStaff after first retrieval.
        if (staff->altNotation == others::Staff::AlternateNotation::Rhythmic) {
            return staff->altRhythmStemsUp;
        }
    }
    // manual override of stem direction
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        const auto& entry = next->getEntry();
        if (next->getEntry()->freezeStem) {
            return entry->upStem;
        }
    }
    // layer override of stem direction
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        if (auto layerAtts = frame->getLayerAttributes()) {
            if (layerAtts->freezeLayer && calcIfLayerSettingsApply()) {
                return layerAtts->freezeStemsUp;
            }
        }
    }
    // staff override of stem direction
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        auto staff = next.createCurrentStaff();
        if (staff->stemDirection == others::Staff::StemDirection::AlwaysUp) {
            return true;
        } else if (staff->stemDirection == others::Staff::StemDirection::AlwaysDown) {
            return false;
        }
    }
    // v1/v2 direction
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        const auto& entry = next->getEntry();
        if (entry->v2Launch || entry->voice2) {
            return entry->upStem;
        }
    }
    // cross-staff direction was not part of the 2001 testing, but this seems the right place for it for now.
    const auto scrollViewStaves = frame->getDocument()->getOthers()->getArray<others::StaffUsed>(frame->getRequestedPartId(), BASE_SYSTEM_ID);
    int foundCrossDirection = 0;
    for (auto next = beamStart; next; next = next.getNextInBeamGroup()) {
        const int currDirection = next.calcCrossStaffDirectionForAll(scrollViewStaves);
        if (currDirection != 0) {
            if (foundCrossDirection == 0) {
                foundCrossDirection = currDirection;
            } else if (currDirection != foundCrossDirection) {
                foundCrossDirection = 0;
                break;
            }
        }
    }
    if (foundCrossDirection != 0) {
        return foundCrossDirection < 0; // stem direction is opposite the cross direction, so cross down means stem up.
    }

    return calcUpStemDefault();
}

bool EntryInfoPtr::calcUnbeamed() const
{
    if (!canBeBeamed()) return true;
    if ((*this)->getEntry()->isHidden) {
        return (!getNextInBeamGroup() || !getPreviousInBeamGroup());
    }
    return (!getNextInBeamGroup() && !getPreviousInBeamGroup());
}

bool EntryInfoPtr::canBeBeamed() const
{
    if ((*this)->getEntry()->duration >= Edu(NoteType::Quarter)) {
        return false;
    }
    if (auto staff = createCurrentStaff()) {
        if (staff->hideStems || staff->hideBeams) {
            return false;
        }
    }
    return true;
}

bool EntryInfoPtr::calcBeamMustStartHere() const
{
    return (*this)->getEntry()->beam || !canBeBeamed();
}

bool EntryInfoPtr::calcIsBeamStart() const
{
    if ((*this)->getEntry()->isHidden) return false;
    if (!canBeBeamed()) return false;
    return (!getPreviousInBeamGroup() && getNextInBeamGroup());
}

bool EntryInfoPtr::calcCreatesSingletonBeamLeft() const
{
    auto entryFrame = getFrame();
    for (const auto& tuplInfo : entryFrame->tupletInfo) {
        if (tuplInfo.startIndex == getIndexInFrame() && tuplInfo.calcCreatesSingletonBeamLeft()) {
            return true;
        }
    }
    return false;
}

bool EntryInfoPtr::calcCreatesSingletonBeamRight() const
{
    auto entryFrame = getFrame();
    for (const auto& tuplInfo : entryFrame->tupletInfo) {
        if (tuplInfo.startIndex == getIndexInFrame() && tuplInfo.calcCreatesSingletonBeamRight()) {
            return true;
        }
    }
    return false;
}

static bool checkBeamExtLeft(const MusxInstance<details::BeamExtension>& beamExt)
{
    return beamExt && beamExt->leftOffset < 0;
};

static bool checkBeamExtRight(const MusxInstance<details::BeamExtension>& beamExt) {
    return beamExt && beamExt->rightOffset > 0;
};

EntryInfoPtr EntryInfoPtr::findLeftBeamAnchorForBeamOverBarline() const
{
    auto entry = (*this)->getEntry();
    if (entry->graceNote) {
        return {};
    }
    int voice = static_cast<int>(entry->voice2) + 1;
    auto frame = getFrame();
    auto anchorEntryInfo = frame->getLastInVoice(voice);
    while (anchorEntryInfo && anchorEntryInfo->getEntry()->graceNote) {
        anchorEntryInfo = anchorEntryInfo.getPreviousSameV();
    }
    if (anchorEntryInfo && anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.canBeBeamed()) {
        for (auto entryInfo = anchorEntryInfo.getPreviousSameVNoGrace(); entryInfo; entryInfo = entryInfo.getPreviousSameVNoGrace()) {
            if (!entryInfo.calcDisplaysAsRest()) {
                anchorEntryInfo = entryInfo;
                break;
            }
            if (entryInfo.calcBeamMustStartHere()) {
                break;
            }
        }
    }
    if (anchorEntryInfo && !anchorEntryInfo->getEntry()->isHidden && !anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.canBeBeamed()) {
        auto beamStart = anchorEntryInfo.findBeamStartOrCurrent();
        if (beamStart.getIndexInFrame() <= getIndexInFrame()) {
            if (!beamStart.calcCreatesSingletonBeamRight()) {
                if (!checkBeamExtRight(frame->getDocument()->getDetails()->get<details::BeamExtensionUpStem>(frame->getRequestedPartId(), beamStart->getEntry()->getEntryNumber()))) {
                    if (!checkBeamExtRight(frame->getDocument()->getDetails()->get<details::BeamExtensionDownStem>(frame->getRequestedPartId(), beamStart->getEntry()->getEntryNumber()))) {
                        return {};
                    }
                }
            }
            return beamStart;
        }
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::findRightBeamAnchorForBeamOverBarline() const
{
   auto entry = (*this)->getEntry();
    if (entry->graceNote) {
        return {};
    }
    int voice = static_cast<int>(entry->voice2) + 1;
    auto frame = getFrame();
    auto anchorEntryInfo = frame->getFirstInVoice(voice);
    while (anchorEntryInfo && anchorEntryInfo->getEntry()->graceNote) {
        anchorEntryInfo = anchorEntryInfo.getNextSameV();
    }
    if (anchorEntryInfo && anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.canBeBeamed()) {
        for (auto entryInfo = anchorEntryInfo.getNextSameVNoGrace(); entryInfo; entryInfo = entryInfo.getNextSameVNoGrace()) {
            if (entryInfo.calcBeamMustStartHere()) {
                break;
            }
            if (!entryInfo.calcDisplaysAsRest()) {
                anchorEntryInfo = entryInfo;
                break;
            }
        }
    }
    if (anchorEntryInfo && !anchorEntryInfo->getEntry()->isHidden && !anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.canBeBeamed()) {
        auto beamStart = anchorEntryInfo.findBeamStartOrCurrent();
        if (beamStart.getIndexInFrame() >= getIndexInFrame()) {
            if (!beamStart.calcCreatesSingletonBeamLeft()) {
                if (!checkBeamExtLeft(frame->getDocument()->getDetails()->get<details::BeamExtensionUpStem>(frame->getRequestedPartId(), beamStart->getEntry()->getEntryNumber()))) {
                    if (!checkBeamExtLeft(frame->getDocument()->getDetails()->get<details::BeamExtensionDownStem>(frame->getRequestedPartId(), beamStart->getEntry()->getEntryNumber()))) {
                        return {};
                    }
                }
            }
            return beamStart;
        }
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::calcBeamContinuesLeftOverBarline() const
{
    const auto entry = (*this)->getEntry();
    if (entry->graceNote) {
        return {};
    }

    const auto frame = getFrame();
    int voice = static_cast<int>(entry->voice2) + 1;

    const auto leftBeamStart = findBeamStartOrCurrent();
    // must be the first item in the bar in that voice
    if (leftBeamStart.getPreviousInVoice(voice)) {
        return {};
    }
    if (leftBeamStart.calcUnbeamed()) {
        return {};
    }
    if (!leftBeamStart.calcCreatesSingletonBeamLeft()) {
        if (!checkBeamExtLeft(frame->getDocument()->getDetails()->get<details::BeamExtensionUpStem>(frame->getRequestedPartId(), leftBeamStart->getEntry()->getEntryNumber()))) {
            if (!checkBeamExtLeft(frame->getDocument()->getDetails()->get<details::BeamExtensionDownStem>(frame->getRequestedPartId(), leftBeamStart->getEntry()->getEntryNumber()))) {
                return {};
            }
        }
    }

    auto prevFrame = frame->getPrevious();
    if (!prevFrame) {
        return {};
    }
    if (auto prevEntryInfo = prevFrame->getLastInVoice(voice)) {
        if (!prevEntryInfo.calcUnbeamed()) {
            const auto rightBeamStart = prevEntryInfo.findBeamStartOrCurrent();
            if (rightBeamStart.calcCreatesSingletonBeamRight()) {
                return rightBeamStart;
            }
            if (!checkBeamExtRight(frame->getDocument()->getDetails()->get<details::BeamExtensionUpStem>(frame->getRequestedPartId(), rightBeamStart->getEntry()->getEntryNumber()))) {
                if (!checkBeamExtRight(frame->getDocument()->getDetails()->get<details::BeamExtensionDownStem>(frame->getRequestedPartId(), rightBeamStart->getEntry()->getEntryNumber()))) {
                    return {};
                }
            }
            return prevEntryInfo;
        }
    }
    return {};
};

EntryInfoPtr EntryInfoPtr::calcBeamContinuesRightOverBarline() const
{
    const auto entry = (*this)->getEntry();
    if (entry->graceNote) {
        return {};
    }

    const auto anchor = findLeftBeamAnchorForBeamOverBarline();
    if (!anchor) {
        return {};
    }

    auto nextInVoice = *this;
    if (calcCreatesSingletonBeamRight()) {
        nextInVoice = nextInVoice.getNextSameV();
    }
    MUSX_ASSERT_IF(!nextInVoice) {
        throw std::logic_error("getNextInVoice returned null when calcCreatesSingletonBeamRight was true.");
    }
    nextInVoice = nextInVoice.getNextSameVNoGrace();
    if (nextInVoice) {
        return nextInVoice;
    }

    auto nextFrame = getFrame()->getNext();
    if (!nextFrame) {
        return {};
    }
    int voice = static_cast<int>(entry->voice2) + 1;
    for (auto nextEntryInfo = nextFrame->getFirstInVoice(voice); nextEntryInfo; nextEntryInfo = nextEntryInfo.getNextSameV()) {
        if (nextEntryInfo->getEntry()->graceNote) {
            continue;
        }
        if (nextEntryInfo.calcIsBeamStart()) {
            auto rightAnchor = nextEntryInfo.findRightBeamAnchorForBeamOverBarline();
            if (!rightAnchor) {
                return {};
            }
            if (nextEntryInfo.calcCreatesSingletonBeamLeft()) {
                return nextEntryInfo.getNextInBeamGroup();
            }
            return nextEntryInfo;
        }
        break;
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::findBeamStartOrCurrent() const
{
    if (!canBeBeamed()) return *this;
    auto prev = getPreviousInBeamGroup();
    if (!prev) {
        return *this;
    }
    while (true) {
        if (auto tryPrev = prev.getPreviousInBeamGroup()) {
            prev = tryPrev;
        } else {
            break;
        }
    }
    return prev;
}

EntryInfoPtr EntryInfoPtr::findBeamEnd() const
{
    if (calcUnbeamed()) return EntryInfoPtr();
    auto next = getNextInBeamGroup();
    if (!next) {
        if (getPreviousInBeamGroup()) return *this;
        return EntryInfoPtr();
    }
    while (true) {
        if (auto tryNext = next.getNextInBeamGroup()) {
            next = tryNext;
        } else {
            break;
        }
    }
    return next;
}

unsigned calcNumberOfBeamsInEdu(Edu duration)
{
    unsigned result = 0;
    MUSX_ASSERT_IF(!duration)
    {
        throw std::logic_error("Edu duration value is zero.");
    }
    while (duration < Edu(NoteType::Quarter)) {
        result++;
        duration <<= 1;
    }
    return result;
}

unsigned EntryInfoPtr::calcNumberOfBeams() const
{
    return calcNumberOfBeamsInEdu((*this)->getEntry()->duration);
}

unsigned EntryInfoPtr::calcVisibleBeams() const
{
    if (calcDisplaysAsRest()) {
        if (auto opts = (*this)->getEntry()->getDocument()->getOptions()->get<options::BeamOptions>()) {
            if (!opts->extendSecBeamsOverRests) {
                return 1;
            }
        }
    }
    return calcNumberOfBeams();
}

template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
std::optional<unsigned> EntryInfoPtr::iterateFindRestsInSecondaryBeam(const EntryInfoPtr nextOrPrevInBeam) const
{
    auto entry = (*this)->getEntry();
    if (auto opts = entry->getDocument()->getOptions()->get<options::BeamOptions>()) {
        auto cutsBeam = [&](const EntryInfoPtr& entryInfo) -> bool {
            if (entryInfo->getEntry()->isHidden) {
                return true;
            }
            if (!opts->extendSecBeamsOverRests && calcDisplaysAsRest()) {
                return true;
            }
            return false;
            };
        if (cutsBeam(*this)) {
            return 0; // if *this* is a rest, it can't start or end a secondary beam
        }
        auto nextOrPrevInFrame = (this->*Iterator)();
        while (true) {
            assert(nextOrPrevInFrame); // should hit nextOrPrevInBeam before null.
            if (cutsBeam(nextOrPrevInFrame)) {
                if (calcNumberOfBeams() >= 2) {
                    return 2; // rests always cut to 8th beam, so any secondary beam starts or ends
                }
            }
            if (nextOrPrevInFrame->getEntry()->getEntryNumber() == nextOrPrevInBeam->getEntry()->getEntryNumber()) {
                break;
            }
            nextOrPrevInFrame = (nextOrPrevInFrame.*Iterator)();
        }
    }
    return std::nullopt;
}

unsigned EntryInfoPtr::calcLowestBeamStart() const
{
    if ((*this)->getEntry()->isHidden) return 0;
    if (!canBeBeamed()) return 0;
    auto prev = getPreviousInBeamGroup();
    if (!prev) {
        return getNextInBeamGroup() ? 1 : 0; // if this is the start of the beam, then the lowest is the primary (8th) beam.
    }
    if (auto restValue = iterateFindRestsInSecondaryBeam<&EntryInfoPtr::getPreviousSameV>(prev)) {
        return restValue.value();
    }
    auto entry = (*this)->getEntry();
    unsigned secondaryBreak = 0;
    if (entry->secBeam) {
        if (auto beamBreaks = m_entryFrame->getDocument()->getDetails()->get<details::SecondaryBeamBreak>(m_entryFrame->getRequestedPartId(), entry->getEntryNumber())) {
            secondaryBreak = beamBreaks->calcLowestBreak();
            if (secondaryBreak < 2) {
                secondaryBreak = 0;
            }
        }
    }
    unsigned prevNumBeams = prev.calcVisibleBeams();
    if (calcVisibleBeams() > prevNumBeams) {
        if (!secondaryBreak || secondaryBreak > prevNumBeams + 1) {
            secondaryBreak = prevNumBeams + 1;
        }
    }
    return secondaryBreak;
}

unsigned EntryInfoPtr::calcLowestBeamEnd() const
{
    if ((*this)->getEntry()->isHidden) return 0;
    if (!canBeBeamed()) return 0;
    auto next = getNextInBeamGroup();
    if (!next) {
        return getPreviousInBeamGroup() ? 1 : 0; // if this is the end of the beam, then the lowest is the primary (8th) beam.
    }
    if (auto restValue = iterateFindRestsInSecondaryBeam<&EntryInfoPtr::getNextSameV>(next)) {
        return restValue.value();
    }
    unsigned numBeams = calcVisibleBeams();
    unsigned nextSecondaryStart = next.calcLowestBeamStart();
    if (nextSecondaryStart && nextSecondaryStart <= numBeams) {
        return nextSecondaryStart;
    }
    unsigned nextNumBeams = next.calcVisibleBeams();
    if (numBeams > nextNumBeams) {
        return nextNumBeams + 1;
    }
    return 0;
}

unsigned EntryInfoPtr::calcLowestBeamEndAcrossBarlines() const
{
    if ((*this)->getEntry()->isHidden) return 0;
    if (auto next = calcBeamContinuesRightOverBarline()) {
        unsigned numBeams = calcVisibleBeams();
        unsigned nextNumBeams = next.calcVisibleBeams();
        if (numBeams > nextNumBeams) {
            return nextNumBeams + 1;
        }
        return 0;
    }
    return calcLowestBeamEnd();
}

unsigned EntryInfoPtr::calcLowestBeamStub() const
{
    if (unsigned lowestBeamStart = calcLowestBeamStart()) {
        if (unsigned lowestBeamEnd = calcLowestBeamEnd()) {
            return (std::max)(lowestBeamStart, lowestBeamEnd);
        }
    }
    return 0;
}

bool EntryInfoPtr::calcBeamStubIsLeft() const
{
    auto entry = (*this)->getEntry();
    if (entry->stemDetail) {
        if (auto manual = m_entryFrame->getDocument()->getDetails()->get<details::BeamStubDirection>(m_entryFrame->getRequestedPartId(), entry->getEntryNumber())) {
            return manual->isLeft();
        }
    }

    auto prev = getPreviousInBeamGroup();
    if (!prev) return false;  // beginning of beam points right
    auto next = getNextInBeamGroup();
    if (!next) return true;  // end of beam points left

    unsigned numBeams = calcNumberOfBeams();
    unsigned lowestStub = calcLowestBeamStub();
    if (numBeams >= lowestStub) {
        auto isSecondaryTerminator = [&](unsigned lowestTerminator, const EntryInfoPtr& neighbor) -> bool {
            if (lowestTerminator < numBeams) {
                return true;
            }
            if (lowestTerminator == numBeams && neighbor.calcNumberOfBeams() == numBeams) {
                if (!neighbor->getEntry()->isNote) {
                    // If we are not extending secondary beams over rests, then rests always cut to the 8th beam.
                    // That means for this purpose the neighbor rest has only a single beam and therefore cannot be compared to the current.
                    if (auto beamOpts = (*this)->getEntry()->getDocument()->getOptions()->get<options::BeamOptions>()) {
                        if (!beamOpts->extendSecBeamsOverRests) {
                            return false;
                        }
                    }
                }
                return true;
            }
            return false;
            };
        if (isSecondaryTerminator(calcLowestBeamStart(), prev)) return false;   // beginning of 2ndary beam points right
        if (isSecondaryTerminator(calcLowestBeamEnd(), next)) return true;      // end of 2ndary beam points left
    }

    auto prevDots = calcNoteInfoFromEdu(prev->actualDuration.calcEduDuration()).second;
    auto nextDots = calcNoteInfoFromEdu(next->actualDuration.calcEduDuration()).second;

    if (prevDots || nextDots) {
        return prevDots >= nextDots;
    }

    return false;
}

util::Fraction EntryInfoPtr::calcGlobalElapsedDuration() const
{
    return (*this)->elapsedDuration * m_entryFrame->getTimeStretch();
}

util::Fraction EntryInfoPtr::calcGlobalActualDuration() const
{
    return (*this)->actualDuration * m_entryFrame->getTimeStretch();
}

template<EntryInfoPtr(EntryInfoPtr::* Iterator)() const>
EntryInfoPtr EntryInfoPtr::iteratePotentialEntryInBeam() const
{
    EntryInfoPtr result = (this->*Iterator)();
    if (!result || !result.canBeBeamed()) {
        return EntryInfoPtr();
    }
    auto thisRawEntry = (*this)->getEntry();
    auto resultEntry = result->getEntry();
    // a grace can't beam past a non grace note
    if (thisRawEntry->graceNote && !resultEntry->graceNote) {
        return EntryInfoPtr();
    }
    // a non grace should skip grace notes
    if (!thisRawEntry->graceNote && resultEntry->graceNote) {
        do {
            if (result.calcBeamMustStartHere()) {
                return EntryInfoPtr();
            }
            result = (result.*Iterator)();
        } while (result && result->getEntry()->graceNote);
    }
    return result;
}

EntryInfoPtr EntryInfoPtr::nextPotentialInBeam(bool includeHiddenEntries) const
{
    auto next = iteratePotentialEntryInBeam<&EntryInfoPtr::getNextSameV>();
    if (!next || next->getEntry()->beam) {
        return EntryInfoPtr();
    }
    if (next && next->getEntry()->isHidden && !includeHiddenEntries) {
        return next.nextPotentialInBeam(includeHiddenEntries);
    }
    return next;
}

EntryInfoPtr EntryInfoPtr::previousPotentialInBeam(bool includeHiddenEntries) const
{
    if ((*this)->getEntry()->beam) {
        return EntryInfoPtr();
    }
    auto prev = iteratePotentialEntryInBeam<&EntryInfoPtr::getPreviousSameV>();
    if (prev && prev->getEntry()->isHidden && !includeHiddenEntries) {
        return prev.previousPotentialInBeam(includeHiddenEntries);
    }
    return prev;
}

template<EntryInfoPtr(EntryInfoPtr::* Iterator)(bool) const, EntryInfoPtr(EntryInfoPtr::* ReverseIterator)(bool) const>
EntryInfoPtr EntryInfoPtr::iterateBeamGroup(bool includeHiddenEntries) const
{
    if (!canBeBeamed()) {
        return EntryInfoPtr();
    }
    EntryInfoPtr result = (this->*Iterator)(includeHiddenEntries); // either nextPotentialInBeam or previousPotentialInBeam
    if (result) {
        auto thisRawEntry = (*this)->getEntry();
        auto resultEntry = result->getEntry();
        if (calcDisplaysAsRest() || result.calcDisplaysAsRest()) {
            auto beamOpts = getFrame()->getDocument()->getOptions()->get<options::BeamOptions>();
            MUSX_ASSERT_IF(!beamOpts) {
                throw std::logic_error("Document has no BeamOptions.");
            }
            auto searchForNoteFrom = [includeHiddenEntries](EntryInfoPtr from, EntryInfoPtr(EntryInfoPtr::* iterator)(bool) const) -> bool {
                for (auto next = from; next; next = (next.*iterator)(includeHiddenEntries)) {
                    if (!next.calcDisplaysAsRest()) {
                        return true;
                    }
                }
                return false;
            };
            bool noteFound = searchForNoteFrom(result, Iterator);
            if (!noteFound && !beamOpts->extendBeamsOverRests) {
                return EntryInfoPtr();
            }
            bool reverseNoteFound = searchForNoteFrom(*this, ReverseIterator);
            if (!reverseNoteFound && !beamOpts->extendBeamsOverRests) {
                return EntryInfoPtr();
            }
            if (beamOpts->extendBeamsOverRests && !noteFound && !reverseNoteFound) {
                return EntryInfoPtr();
            }
        }
    }
    return result;
}

bool EntryInfoPtr::calcIsFeatheredBeamStart(Evpu& outLeftY, Evpu& outRightY) const
{
    if (!(*this)->getEntry()->stemDetail || !calcIsBeamStart()) {
        return false;
    }

    Evpu leftYUp{}, rightYUp{}, leftYDown{}, rightYDown{};
    bool upFeathered = details::SecondaryBeamAlterationsUpStem::calcIsFeatheredBeam(*this, leftYUp, rightYUp);
    bool downFeathered = details::SecondaryBeamAlterationsDownStem::calcIsFeatheredBeam(*this, leftYDown, rightYDown);

    if (upFeathered && !downFeathered) {
        outLeftY = leftYUp;
        outRightY = rightYUp;
        return true;
    } else if (downFeathered && !upFeathered) {
        outLeftY = leftYDown;
        outRightY = rightYDown;
        return true;
    } else if (upFeathered && downFeathered) {
        // Use the greater wedge span as tie-breaker
        Evpu spanUp = std::abs(rightYUp - leftYUp);
        Evpu spanDown = std::abs(rightYDown - leftYDown);
        if (spanUp >= spanDown) {
            outLeftY = leftYUp;
            outRightY = rightYUp;
        } else {
            outLeftY = leftYDown;
            outRightY = rightYDown;
        }
        return true;
    }

    return false;
}

int EntryInfoPtr::calcEntrySize() const
{
    int result = 100;
    auto beamStart = findBeamStartOrCurrent();
    if (!beamStart || !beamStart->getEntry()->noteDetail) {
        return result;
    }
    auto doc = m_entryFrame->getDocument();
    if (auto entrySize = doc->getDetails()->get<details::EntrySize>(m_entryFrame->getRequestedPartId(), beamStart->getEntry()->getEntryNumber())) {
        result = entrySize->percent;
    }
    return result;
}

bool EntryInfoPtr::calcIsCue(bool includeVisibleInScore) const
{
    if (calcEntrySize() <= MAX_CUE_PERCENTAGE) {
        if (includeVisibleInScore) {
            return true;
        }
        auto doc = m_entryFrame->getDocument();
        if (auto scoreStaff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, getStaff(), getMeasure(), (*this)->elapsedDuration.calcEduDuration())) {
            const bool hidden = scoreStaff->calcAlternateNotationHidesEntries(getLayerIndex()) || scoreStaff->hideMode != others::Staff::HideMode::None;
            if (hidden) {
                auto parts = scoreStaff->getContainingParts(/*includeScore*/false);
                for (const auto& part : parts) {
                    if (auto partStaff = others::StaffComposite::createCurrent(doc, part->getCmper(), getStaff(), getMeasure(), (*this)->elapsedDuration.calcEduDuration())) {
                        if (partStaff->altNotation != others::Staff::AlternateNotation::BlankWithRests && partStaff->altNotation != others::Staff::AlternateNotation::Blank) {
                            return true;
                        }
                    }
                }
            }
        } else {
            MUSX_INTEGRITY_ERROR("Staff " + std::to_string(getStaff()) + " not found for measure " + std::to_string(getMeasure()));
        }
    }
    return false;
}

bool EntryInfoPtr::calcIsFullMeasureRest() const
{
    if ((*this)->getEntry()->isPossibleFullMeasureRest()) {
        return m_entryFrame->getEntries().size() == 1 && (*this)->elapsedDuration == 0;
    }
    return false;
}

bool EntryInfoPtr::calcIsBeamedRestWorkaroud() const
{
    auto entry = (*this)->getEntry();
    if (entry->isNote || calcNumberOfBeams() < 2) { // must be at least a 16th note
        return false;
    }
    if (entry->isHidden && entry->v2Launch) {
        // if this is a hidden v2 launch rest, check to see if there is an equivalent visible stand-alone v2 rest of the same type
        if (auto next = getNextInFrame()) {
            auto nextEntry = next->getEntry();
            if (!nextEntry->isNote && nextEntry->duration == entry->duration && !nextEntry->isHidden) {
                if (next = next.getNextInFrame(); !next || !next->getEntry()->voice2) {
                    return true;
                }
            }
        }
    } else if (!entry->isHidden && entry->voice2) {
        // if this is a visible stand-alone v2 rest, check to see if there is an equivalent invisible v2 launch rest preceding it
        if (auto next = getNextInFrame(); !next || !next->getEntry()->voice2) {
            if (auto prev = getPreviousInFrame(); prev && prev->getEntry()->v2Launch) {
                auto prevEntry = prev->getEntry();
                if (!prevEntry->isNote && prevEntry->isHidden && prevEntry->duration == entry->duration) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<size_t> EntryInfoPtr::findTupletInfo() const
{
    std::vector<size_t> result;
    if (auto entryFrame = this->getFrame()) {
        for (size_t x = 0; x < entryFrame->tupletInfo.size(); x++) {
            const auto& info = entryFrame->tupletInfo[x];
            if (getIndexInFrame() >= info.startIndex && getIndexInFrame() <= info.endIndex) {
                result.push_back(x);
            }
        }
    }
    return result;
}

bool EntryInfoPtr::calcIfLayerSettingsApply() const
{
    const auto frame = getFrame();
    const auto layerAtts = frame->getLayerAttributes();
    if (!layerAtts->onlyIfOtherLayersHaveNotes) {
        return true;
    }
    const auto context = frame->getContext();
    if (!context->calcIsMultiLayer()) {
        return false;
    }
    const LayerIndex layerIndex = frame->getLayerIndex();
    const auto startStaff = frame->getStartStaffInstance();
    for (size_t nextLayerIndex = 0; nextLayerIndex < context->frames.size(); nextLayerIndex++) {
        if (nextLayerIndex == layerIndex || context->frames[nextLayerIndex] == 0) {
            continue;
        }
        const auto nextLayerAtts = frame->getDocument()->getOthers()->get<others::LayerAttributes>(frame->getRequestedPartId(), static_cast<Cmper>(nextLayerIndex));
        MUSX_ASSERT_IF(!nextLayerAtts) {
            throw integrity_error("Layer attributes for layer " + std::to_string(nextLayerIndex) + " do not exist.");
        }
        if (layerAtts->ignoreHiddenLayers && nextLayerAtts->hideLayer) {
            continue;
        }
        if (layerAtts->ignoreHiddenNotesOnly) {
            if (startStaff->calcAlternateNotationHidesEntries(nextLayerIndex)) {
                continue; // layers are considered hidden if alternate notation at the beginning of the measure hides them, even if it doesn't go to the end.
            }
            if (auto layerFrame = context.createEntryFrame(nextLayerIndex)) {
                if (layerFrame->calcAreAllEntriesHiddenInFrame()) {
                    continue;
                }
            } else {
                continue; // do not count non-existent layer frames as having notes
            }
        }
        return true;
    }
    return false;
}

int EntryInfoPtr::calcCrossStaffDirectionForAll(DeferredReference<MusxInstanceList<others::StaffUsed>> staffList) const
{
    const auto frame = getFrame();

    if (!staffList) {
        staffList.emplace(frame->getDocument()->getOthers()->getArray<others::StaffUsed>(frame->getRequestedPartId(), BASE_SYSTEM_ID));
    }

    int crossStaffDirectionFound = 0;
    auto entry = (*this)->getEntry();
    for (size_t x = 0; x < entry->notes.size(); x++) {
        const auto noteInfo = NoteInfoPtr(*this, x);
        const int currDirection = noteInfo.calcCrossStaffDirection(staffList);
        if (currDirection != 0) {
            if (crossStaffDirectionFound == 0) {
                crossStaffDirectionFound = currDirection;
            } else if (currDirection != crossStaffDirectionFound) {
                return 0; // mixed directions, so short-circuit out
            }
        }
    }
    return crossStaffDirectionFound;
}

// *****************************
// ***** GFrameHoldContext *****
// *****************************

details::GFrameHoldContext::GFrameHoldContext(const DocumentPtr& document, Cmper partId, Cmper staffId, Cmper meas)
    : m_requestedPartId(partId)
{
    m_hold = document->getDetails()->get<details::GFrameHold>(partId, staffId, meas);
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
struct TupletState
{
    util::Fraction remainingSymbolicDuration;         // The remaining symbolic duration
    util::Fraction ratio;             // The remaining actual duration
    MusxInstance<details::TupletDef> tuplet; // The tuplet.
    size_t infoIndex;               // the index of this tuplet in EntryFrame::tupletInfo

    void accountFor(util::Fraction actual)
    {
        remainingSymbolicDuration -= (actual / ratio);
    }

    TupletState(const MusxInstance<details::TupletDef>& t, size_t i)
        : remainingSymbolicDuration(t->displayNumber* t->displayDuration, int(NoteType::Whole)),
        ratio(t->referenceNumber* t->referenceDuration, t->displayNumber* t->displayDuration),
        tuplet(t), infoIndex(i)
    {
    }
};
#endif // DOXYGEN_SHOULD_IGNORE_THIS

std::pair<MusxInstance<others::Frame>, Edu> details::GFrameHoldContext::findLayerFrame(LayerIndex layerIndex) const
{
    MusxInstance<others::Frame> layerFrame;
    Edu startEdu = 0;
    if (layerIndex < m_hold->frames.size() && m_hold->frames[layerIndex]) {
        auto frameIncis = m_hold->getDocument()->getOthers()->getArray<others::Frame>(getRequestedPartId(), m_hold->frames[layerIndex]);
        for (const auto& frame : frameIncis) {
            if (frame->startEntry) {
                if (layerFrame) {
                    MUSX_INTEGRITY_ERROR("More than one entry frame inci exists for frame " + std::to_string(m_hold->frames[layerIndex]));
                }
                layerFrame = frame;
            }
            if (frame->startTime > startEdu) {
                startEdu = frame->startTime;
            }
        }
    }
    return std::make_pair(layerFrame, startEdu);
}

std::shared_ptr<const EntryFrame> details::GFrameHoldContext::createEntryFrame(LayerIndex layerIndex) const
{
    if (!m_hold) return nullptr;
    if (layerIndex >= m_hold->frames.size()) { // note: layerIndex is unsigned
        throw std::invalid_argument("invalid layer index [" + std::to_string(layerIndex) + "]");
    }
    if (!m_hold->frames[layerIndex]) return nullptr; // nothing here
    auto document = m_hold->getDocument();
    auto [frame, startEdu] = findLayerFrame(layerIndex);
    std::shared_ptr<EntryFrame> entryFrame;
    if (frame) {
        const auto measure = m_hold->getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), m_hold->getMeasure());
        if (!measure) {
            throw std::invalid_argument("Measure instance for measure " + std::to_string(m_hold->getMeasure()) + " does not exist.");
        }
        const auto staff = others::StaffComposite::createCurrent(m_hold->getDocument(), getRequestedPartId(), m_hold->getStaff(), m_hold->getMeasure(), 0);
        if (!staff) {
            throw std::invalid_argument("Staff instance for staff " + std::to_string(m_hold->getStaff()) + " does not exist.");
        }
        const util::Fraction timeStretch = staff->floatTime
                                         ? measure->calcTimeStretch(staff->getCmper())
                                         : 1;
        entryFrame = std::make_shared<EntryFrame>(*this, layerIndex, timeStretch, staff);
        entryFrame->keySignature = measure->createKeySignature(m_hold->getStaff());
        auto entries = frame->getEntries();
        std::vector<TupletState> v1ActiveTuplets; // List of active tuplets for v1
        std::vector<TupletState> v2ActiveTuplets; // List of active tuplets for v2
        util::Fraction v1ActualElapsedDuration = util::Fraction::fromEdu(startEdu);
        util::Fraction v2ActualElapsedDuration = v1ActualElapsedDuration;
        int graceIndex = 0;
        for (size_t i = 0; i < entries.size(); i++) {
            const auto& entry = entries[i];
            auto entryInfo = std::shared_ptr<EntryInfo>(new EntryInfo(entry));
            if (entry->v2Launch) {
                // Note: v1 tuplets do not appear to affect v2 entries. If they did this would be correct:
                //      v2ActiveTuplets = v1ActiveTuplets;
                // But since they do not:
                v2ActiveTuplets.clear();
                v2ActualElapsedDuration = v1ActualElapsedDuration;
            }
            std::vector<TupletState>& activeTuplets = entry->voice2 ? v2ActiveTuplets : v1ActiveTuplets;
            util::Fraction& actualElapsedDuration = entry->voice2 ? v2ActualElapsedDuration : v1ActualElapsedDuration;
            entryInfo->elapsedDuration = actualElapsedDuration;
            entryInfo->clefIndexConcert = calcClefIndexAt(actualElapsedDuration);
            if (staff->transposition && staff->transposition->setToClef) {
                entryInfo->clefIndex = staff->transposedClef;
            } else {
                entryInfo->clefIndex = entryInfo->clefIndexConcert;
            }
            util::Fraction cumulativeRatio = 1;
            if (!entry->graceNote) {
                graceIndex = 0;
                if (entry->tupletStart) {
                    auto tuplets = document->getDetails()->getArray<details::TupletDef>(getRequestedPartId(), entry->getEntryNumber());
                    std::sort(tuplets.begin(), tuplets.end(), [](const auto& a, const auto& b) {
                        return a->calcReferenceDuration() > b->calcReferenceDuration(); // Sort descending by reference duration
                        });
                    for (const auto& tuplet : tuplets) {
                        size_t index = entryFrame->tupletInfo.size();
                        entryFrame->tupletInfo.emplace_back(entryFrame, tuplet, i, actualElapsedDuration, entry->voice2);
                        activeTuplets.emplace_back(tuplet, index);
                    }
                }

                // It appears that Finale allows exactly one entry per 0-length tuplet, no matter
                // what the symbolic duration of the tuplet is. This makes life *much* easier.
                bool zeroLengthTuplet = false;
                for (const auto& t : activeTuplets) {
                    if (t.ratio != 0) {
                        cumulativeRatio *= t.ratio;
                    } else {
                        zeroLengthTuplet = true;
                    }
                }
                util::Fraction actualDuration = zeroLengthTuplet ? 0 : entry->calcFraction() * cumulativeRatio;
                entryInfo->actualDuration = actualDuration;
            } else {
                entryInfo->graceIndex = ++graceIndex;
            }

            entryFrame->addEntry(entryInfo);

            actualElapsedDuration += entryInfo->actualDuration;
            if (!entry->graceNote) {
                for (auto it = activeTuplets.rbegin(); it != activeTuplets.rend(); ++it) {
                    if (it->ratio != 0) {
                        it->remainingSymbolicDuration -= entryInfo->actualDuration / cumulativeRatio;
                        cumulativeRatio /= it->ratio;
                    }
                }
                // always update all end positions, in case we run out of notes before the actual end
                // WARNING: Finale handles incomplete v2 tuplets in a different and buggy manner.
                //          It appears that Finale extends incomplete v2 tuplets to the end of the bar in many cases.
                //          This code only extends them to the end of the v2 sequence. This is by design.
                for (const auto& tuplet : activeTuplets) {
                    auto& tuplInf = entryFrame->tupletInfo[tuplet.infoIndex];
                    tuplInf.endIndex = i;
                    tuplInf.endDura = actualElapsedDuration;
                }
                activeTuplets.erase(
                    std::remove_if(activeTuplets.begin(), activeTuplets.end(),
                        [](const TupletState& t) { return t.remainingSymbolicDuration <= 0 || t.ratio == 0; }),
                    activeTuplets.end()
                );
            }
        }
    } else {
        MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(m_hold->getStaff()) + " and measure "
            + std::to_string(m_hold->getMeasure()) + " points to non-existent frame [" + std::to_string(m_hold->frames[layerIndex]) + "]");
    }
    return entryFrame;
}

bool details::GFrameHoldContext::iterateEntries(LayerIndex layerIndex, std::function<bool(const EntryInfoPtr&)> iterator) const
{
    auto entryFrame = createEntryFrame(layerIndex);
    if (entryFrame) {
        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            if (!iterator(EntryInfoPtr(entryFrame, x))) {
                return false;
            }
        }
    }
    return true;
}

bool details::GFrameHoldContext::iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator) const
{
    for (LayerIndex layerIndex = 0; layerIndex < m_hold->frames.size(); layerIndex++) {
        if (!iterateEntries(layerIndex, iterator)) {
            return false;
        }
    }
    return true;
}

std::map<LayerIndex, bool> details::GFrameHoldContext::calcVoices() const
{
    std::map<LayerIndex, bool> result;
    for (LayerIndex layerIndex = 0; layerIndex < m_hold->frames.size(); layerIndex++) {
        auto [frame, startEdu] = findLayerFrame(layerIndex);
        if (frame) {
            bool gotLayer = false;
            bool gotV2 = false;
            frame->iterateRawEntries([&](const MusxInstance<Entry>& entry) -> bool {
                gotLayer = true;
                if (entry->voice2) {
                    gotV2 = true;
                    return false;
                }
                return true;
            });
            if (gotLayer) {
                result.emplace(layerIndex, gotV2);
            }
        }
    }
    return result;
}

ClefIndex details::GFrameHoldContext::calcClefIndexAt(Edu position) const
{
    MUSX_ASSERT_IF(!m_hold) {
        return 0;
    }
    if (m_hold->clefId.has_value()) {
        return m_hold->clefId.value();
    }
    ClefIndex result = 0;
    auto clefList = m_hold->getDocument()->getOthers()->getArray<others::ClefList>(getRequestedPartId(), m_hold->clefListId);
    if (clefList.empty()) {
        MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(m_hold->getStaff()) + " and measure "
            + std::to_string(m_hold->getMeasure()) + " has non-existent clef list [" + std::to_string(m_hold->clefListId) + "]");
    } else {
        auto& lastClef = clefList[0];
        for (const auto& clef : clefList) {
            if (clef->xEduPos > position) {
                break; // Stop as soon as we pass the position
            }
            lastClef = clef;
        }
        result = lastClef->clefIndex;
    }
    return result;
}

bool details::GFrameHoldContext::calcIsCuesOnly(bool includeVisibleInScore) const
{
    bool foundCue = false;
    for (LayerIndex layerIndex = 0; layerIndex < m_hold->frames.size(); layerIndex++) {
        auto [frame, startEdu] = findLayerFrame(layerIndex);
        if (frame) {
            auto entries = frame->getEntries();
            if (startEdu == 0 && entries.size() == 1 && entries[0]->isPossibleFullMeasureRest()) {
                continue;
            }
            if (auto entryFrame = createEntryFrame(layerIndex)) {
                if (entryFrame->calcIsCueFrame(includeVisibleInScore)) {
                    foundCue = true;
                } else {
                    return false; // non-cue frame found, so this is not a cue frame
                }
            }
        }
    }
    return foundCue;
}

EntryInfoPtr details::GFrameHoldContext::calcNearestEntry(Edu eduPosition, bool findExact, std::optional<LayerIndex> matchLayer, std::optional<bool> matchVoice2) const
{
    EntryInfoPtr result;
    unsigned bestDiff = (std::numeric_limits<unsigned>::max)();

    auto iterator = [&](const EntryInfoPtr& entryInfo) {
        const auto entry = entryInfo->getEntry();
        if (entry->graceNote) {
            return true; // iterate past grace notes
        }
        if (matchVoice2.has_value() && entry->voice2 != *matchVoice2) {
            return true; // iterate past non-matching v1v2 values
        }
        unsigned eduDiff = static_cast<unsigned>(std::labs(eduPosition - entryInfo->elapsedDuration.calcEduDuration()));
        if (eduDiff <= 1) {
            result = entryInfo;
            return false; // stop iterating
        }
        if (!findExact && eduDiff < bestDiff) {
            bestDiff = eduDiff;
            result = entryInfo;
        }
        return true;
    };

    if (matchLayer.has_value()) {
        iterateEntries(*matchLayer, iterator);
    } else {
        iterateEntries(iterator);
    }

    return result;
}

// ****************
// ***** Note *****
// ****************

std::pair<int, int> Note::calcDefaultEnharmonic(const MusxInstance<KeySignature>& key) const
{
    auto transposer = key->createTransposer(harmLev, harmAlt);
    if (harmAlt) {
        transposer->enharmonicTranspose(music_theory::sign(harmAlt));
        if (std::abs(transposer->alteration()) > MAX_ALTERATIONS)
            return {harmLev, harmAlt};
        return {transposer->displacement(), transposer->alteration()};
    }

    transposer->enharmonicTranspose(1);
    int upDisp = transposer->displacement();
    int upAlt = transposer->alteration();

    // This is observed Finale behavior, relevant in the context of microtone custom key signatures.
    // A possibly more correct version would omit this hard-coded comparison to the number 2, but it
    // seems to be what Finale does.
    if (std::abs(upAlt) != 2) {
        if (std::abs(upAlt) > MAX_ALTERATIONS)
            return {harmLev, harmAlt};
        return {upDisp, upAlt};
    }

    auto down = key->createTransposer(harmLev, harmAlt);
    down->enharmonicTranspose(-1);
    int downAlt = down->alteration();

    if (std::abs(downAlt) > MAX_ALTERATIONS)
        return {harmLev, harmAlt};

    if (std::abs(downAlt) < std::abs(upAlt))
        return {down->displacement(), downAlt};
    return {upDisp, upAlt};
}

Note::NoteProperties Note::calcNoteProperties(const MusxInstance<KeySignature>& key, KeySignature::KeyContext ctx, ClefIndex clefIndex,
    const MusxInstance<others::PercussionNoteInfo>& percNoteInfo, const MusxInstance<others::Staff>& staff, bool respellEnharmonic) const
{
    auto [transposedLev, transposedAlt] = respellEnharmonic
                                        ? calcDefaultEnharmonic(key)
                                        : std::pair<int, int>{ harmLev, harmAlt };
    if (staff && staff->transposition && staff->transposition->chromatic) {
        const auto& chromatic = *staff->transposition->chromatic;
        auto transposer = key->createTransposer(transposedLev, transposedAlt);
        transposer->chromaticTranspose(chromatic.diatonic, chromatic.alteration);
        transposedLev = transposer->displacement();
        transposedAlt = transposer->alteration();
    }

    // Determine the base note and octave
    int keyAdjustedLev = key->calcTonalCenterIndex(ctx) + transposedLev + (key->getOctaveDisplacement(ctx) * music_theory::STANDARD_DIATONIC_STEPS);
    int octave = (keyAdjustedLev / music_theory::STANDARD_DIATONIC_STEPS) + 4; // Middle C (C4) is the reference
    int step = keyAdjustedLev % music_theory::STANDARD_DIATONIC_STEPS;
    if (step < 0) {
        step += music_theory::STANDARD_DIATONIC_STEPS;
        octave -= 1;
    }

    // Calculate the actual alteration
    const int actualAlteration = transposedAlt + key->calcAlterationOnNote(step, ctx);

    // Calculate the staff line
    const int staffLine = [&]() {
        if (percNoteInfo) {
            return percNoteInfo->staffPosition;
        }
        const auto& clefOptions = getDocument()->getOptions()->get<options::ClefOptions>();
        if (!clefOptions) {
            throw std::invalid_argument("Document contains no clef options!");
        }
        int middleCLine = clefOptions->getClefDef(clefIndex)->middleCPos;
        return keyAdjustedLev + middleCLine;
    }();

    return { music_theory::noteNames[step], octave, actualAlteration, staffLine };
}

// ***********************
// ***** NoteInfoPtr *****
// ***********************

NoteInfoPtr NoteInfoPtr::findEqualPitch(const EntryInfoPtr& entry) const
{
    if ((*this).getEntryInfo()->getEntry()->isNote && entry->getEntry()->isNote) {
        int srcOccurrence = 1;
        for (auto prev = getPrevious(); prev; prev = prev.getPrevious()) {
            if (!isSamePitchValues(prev)) break;
            srcOccurrence++;
        }
        for (auto note = NoteInfoPtr(entry, 0); note; note = note.getNext()) {
            if (note.isSamePitch(*this)) {
                for (int entryOccurrence = 1; entryOccurrence < srcOccurrence; entryOccurrence++) {
                    auto next = note.getNext();
                    if (!next.isSamePitchValues(note)) break;
                    note = next;
                }
                return note;
            }
        }
        // Finale does not allow ties to enharmonic equivalents, so we can safely ignore enharmonics.
    }
    return NoteInfoPtr();
}

NoteInfoPtr NoteInfoPtr::calcTieTo() const
{
    if (m_entry->getEntry()->isNote) {
        auto nextEntry = m_entry;
        while (nextEntry) {
            if (nextEntry->getEntry()->v2Launch) {
                nextEntry = nextEntry.getNextSameV();
                if (!nextEntry) {
                    if (auto nextFrame = m_entry.getFrame()->getNext()) {
                        nextEntry = nextFrame->getFirstInVoice(1); // v2Launch entries are always voice 1
                    }
                }
            } else if (m_entry->getEntry()->voice2) {
                auto tryEntry = nextEntry.getNextSameV();
                if (!tryEntry) { // if v2 sequence exhausted
                    auto nextDuration = m_entry->calcNextElapsedDuration();
                    nextEntry = nextEntry.getNextInLayer();
                    while (nextEntry.getMeasure() == m_entry.getMeasure() && nextEntry->elapsedDuration < nextDuration) {
                        nextEntry = nextEntry.getNextInLayer();
                    }
                } else {
                    nextEntry = tryEntry;
                }
            } else {
                nextEntry = nextEntry.getNextInLayer();
            }
            if (!nextEntry) {
                break;
            }
            if (nextEntry->getEntry()->graceNote) { // grace note tie to the next non grace entry, if there is a note there to tie to
                continue;
            }
            if (auto result = findEqualPitch(nextEntry)) {
                return result;
            }
            if (nextEntry->getEntry()->v2Launch) {
                nextEntry = nextEntry.getNextInLayer();
                return findEqualPitch(nextEntry);
            }
            break;
        }
    }
    return NoteInfoPtr();
}

NoteInfoPtr NoteInfoPtr::calcTieFrom(bool requireTie) const
{
    if (*this) {
        // grace notes cannot tie backwards; only forwards (see grace note comment above)
        auto thisRawEntry = m_entry->getEntry();
        if (thisRawEntry->isNote && !thisRawEntry->graceNote) {
            for (auto prev = m_entry.getPreviousInLayer(); prev; prev = prev.getPreviousInLayer()) {
                if (prev.getMeasure() < m_entry.getMeasure() - 1) {
                    // only search 1 measure previous
                    break;
                }
                auto prevEntry = prev->getEntry();
                if (!prevEntry->isNote) {
                    continue;
                }
                for (size_t noteIndex = 0; noteIndex < prevEntry->notes.size(); noteIndex++) {
                    NoteInfoPtr tryFrom(prev, noteIndex);
                    if (auto tryTiedTo = tryFrom.calcTieTo(); isSameNote(tryTiedTo)) {
                        if (!requireTie || tryFrom->tieStart) {
                            return tryFrom;
                        }
                        return NoteInfoPtr();
                    }
                }
            }
        }
    }
    return NoteInfoPtr();
}

StaffCmper NoteInfoPtr::calcStaff() const
{
    if ((*this)->crossStaff) {
        const auto entry = m_entry->getEntry();
        const auto noteRestOptions = entry->getDocument()->getOptions()->get<options::NoteRestOptions>();
        if (!noteRestOptions || noteRestOptions->doCrossStaffNotes) {
            if (auto crossStaff = entry->getDocument()->getDetails()->getForNote<details::CrossStaff>(*this)) {
                return crossStaff->staff;
            }
        }
    }
    return m_entry.getStaff();
}

Note::NoteProperties NoteInfoPtr::calcNoteProperties(const std::optional<bool>& enharmonicRespell, bool alwaysUseEntryStaff) const
{
    StaffCmper staffId = getEntryInfo().getStaff();
    ClefIndex clefIndex = getEntryInfo()->clefIndex;
    if (!alwaysUseEntryStaff) {
        staffId = calcStaff();
        if (staffId != m_entry.getStaff()) {
            if (auto staff = m_entry.createCurrentStaff(staffId)) {
                clefIndex = staff->calcClefIndexAt(m_entry.getMeasure(), m_entry->elapsedDuration.calcEduDuration(), /*forWrittenPitch*/ true);
            }
        }
    }
    return (*this)->calcNoteProperties(m_entry.getKeySignature(), KeySignature::KeyContext::Written, clefIndex, calcPercussionNoteInfo(), m_entry.createCurrentStaff(staffId),
            enharmonicRespell.value_or(calcIsEnharmonicRespell()));
}

Note::NoteProperties NoteInfoPtr::calcNotePropertiesConcert(bool alwaysUseEntryStaff) const
{
    const ClefIndex clefIndex = [&]() {
        if (!alwaysUseEntryStaff) {
            StaffCmper staffId = calcStaff();
            if (staffId != m_entry.getStaff()) {
                if (auto staff = m_entry.createCurrentStaff(staffId)) {
                    return staff->calcClefIndexAt(m_entry.getMeasure(), m_entry->elapsedDuration.calcEduDuration(), /*forWrittenPitch*/ false);
                }
            }
        }
        return m_entry->clefIndexConcert;
    }();
    return (*this)->calcNoteProperties(m_entry.getKeySignature(), KeySignature::KeyContext::Concert, clefIndex, calcPercussionNoteInfo(), nullptr, calcIsEnharmonicRespell());
}

Note::NoteProperties NoteInfoPtr::calcNotePropertiesInView(bool alwaysUseEntryStaff) const
{
    bool forWrittenPitch = false;
    auto entryFrame = m_entry.getFrame();
    if (auto partGlobals = entryFrame->getDocument()->getOthers()->get<others::PartGlobals>(entryFrame->getRequestedPartId(), MUSX_GLOBALS_CMPER)) {
        forWrittenPitch = partGlobals->showTransposed;
    }
    return forWrittenPitch ? calcNoteProperties(std::nullopt, alwaysUseEntryStaff) : calcNotePropertiesConcert(alwaysUseEntryStaff);
}

MusxInstance<others::PercussionNoteInfo> NoteInfoPtr::calcPercussionNoteInfo() const
{
    auto entry = getEntryInfo()->getEntry();
    if (entry->noteDetail) {
        if (auto currStaff = getEntryInfo().createCurrentStaff()) {
            if (currStaff->percussionMapId.has_value()) {
                const Cmper partId = getEntryInfo().getFrame()->getRequestedPartId();
                if (auto noteCode = entry->getDocument()->getDetails()->getForNote<details::PercussionNoteCode>(*this, partId)) {
                    auto percNoteInfoList = entry->getDocument()->getOthers()->getArray<others::PercussionNoteInfo>(partId, currStaff->percussionMapId.value());
                    for (const auto& percNoteInfo : percNoteInfoList) {
                        if (noteCode->noteCode == percNoteInfo->percNoteType) {
                            return percNoteInfo;
                        }
                    }
                }
            }
        }
    }
    return nullptr;
}

std::unique_ptr<music_theory::Transposer> NoteInfoPtr::createTransposer() const
{
    return m_entry.getKeySignature()->createTransposer((*this)->harmLev, (*this)->harmAlt);
}

bool NoteInfoPtr::calcIsEnharmonicRespell() const
{
    auto entry = m_entry->getEntry();
    if (entry->noteDetail) {
        if (auto noteAlts = entry->getDocument()->getDetails()->getForNote<details::NoteAlterations>(*this)) {
            return noteAlts->enharmonic;
        }
    }
    return false;
}

bool NoteInfoPtr::isSamePitch(const NoteInfoPtr& src) const
{
    if (!*this || !src) {
        return false;
    }
    auto thisPercInfo = calcPercussionNoteInfo();
    auto srcPercInfo = src.calcPercussionNoteInfo();
    if (thisPercInfo || srcPercInfo) {
        if (!thisPercInfo || !srcPercInfo) {
            return false; // can't compare perc note with non-perc-note
        }
        return thisPercInfo->percNoteType == srcPercInfo->percNoteType
            && thisPercInfo->staffPosition == srcPercInfo->staffPosition;
    }
    auto [thisPitch, thisOctave, thisAlter, thisStaffPos] = calcNotePropertiesConcert();
    auto [srcPitch, srcOctave, srcAlter, srcStaffPos] = src.calcNotePropertiesConcert();
    return srcPitch == thisPitch && srcOctave == thisOctave && srcAlter == thisAlter;
}

bool NoteInfoPtr::isSamePitchValues(const NoteInfoPtr& src) const
{
    if (!*this || !src) {
        return false;
    }
    return (*this)->harmLev == src->harmLev
        && (*this)->harmAlt == src->harmAlt;
}

int NoteInfoPtr::calcCrossStaffDirection(DeferredReference<MusxInstanceList<others::StaffUsed>> staffList) const
{
    const auto frame = getEntryInfo().getFrame();

    if (!staffList) {
        staffList.emplace(frame->getDocument()->getOthers()->getArray<others::StaffUsed>(frame->getRequestedPartId(), BASE_SYSTEM_ID));
    }

    const auto homeIndex = staffList->getIndexForStaff(frame->getStaff());
    MUSX_ASSERT_IF(!homeIndex) {
        throw std::logic_error("Input staffList does not contain the entry frame staff.");
    }
    const size_t noteIndex = staffList->getIndexForStaff(calcStaff()).value_or(*homeIndex);

    if (noteIndex < *homeIndex) {
        return 1;
    } else if (noteIndex > *homeIndex) {
        return -1;
    }

    return 0;
}

} // namespace dom
} // namespace musx