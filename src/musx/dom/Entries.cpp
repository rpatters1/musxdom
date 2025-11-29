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

void Entry::calcLocations(const DocumentPtr& document)
{
    auto gfholds = document->getDetails()->getArray<details::GFrameHold>(SCORE_PARTID);
    for (const auto& gfhold : gfholds) {
        gfhold->iterateRawEntries([&](const MusxInstance<Entry>& entry, LayerIndex layerIndex) {
            Entry* mutableEntry = const_cast<Entry*>(entry.get());
            mutableEntry->locations.emplace_back(std::make_tuple(static_cast<StaffCmper>(gfhold->getCmper1()),
                static_cast<MeasCmper>(gfhold->getCmper2()), layerIndex));
            return true;
        });
    }
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

EntryInfoPtr EntryFrame::calcNearestEntry(util::Fraction position, bool findExact, std::optional<bool> matchVoice2, util::Fraction atGraceNoteDuration) const
{
    EntryInfoPtr result;
    util::Fraction bestDiff = (util::Fraction::max)();

    auto iterator = [&](const EntryInfoPtr& entryInfo) {
        if (entryInfo.calcGraceEllapsedDuration() != atGraceNoteDuration) {
            return true; // iterate past non-matching grace notes
        }
        if (matchVoice2.has_value() && entryInfo->getEntry()->voice2 != *matchVoice2) {
            return true; // iterate past non-matching v1v2 values
        }
        using std::abs;
        auto posDiff = abs(position - entryInfo->elapsedDuration);
        if (posDiff <= util::Fraction::fromEdu(1)) {
            result = entryInfo;
            return false; // stop iterating
        }
        if (!findExact && posDiff < bestDiff) {
            bestDiff = posDiff;
            result = entryInfo;
        }
        return true;
    };

    iterateEntries(iterator);
    return result;
}

bool EntryFrame::iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator) const
{
    for (size_t x = 0; x < m_entries.size(); x++) {
        if (!iterator(EntryInfoPtr(shared_from_this(), x))) {
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
        if (auto beamExt = details::BeamExtension::getForStem(first)) {
            return beamExt->mask >= unsigned(targetNoteType) && beamExt->leftOffset > 0 && beamExt->rightOffset < 0;
        } else {
            return false;
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
    const Evpu hiddenHOffset = hiddenEntryInfo.calcManuaOffset();
    if (left) {
        if (hiddenHOffset >= 0) {
            return false;
        }
    } else {
        if (hiddenHOffset <= 0) {
            return false;
        }
    }
    // entry must have a hidden stem
    auto customStem = details::CustomStem::getForStem(hiddenEntryInfo);
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

EntryInfoPtr EntryInfoPtr::fromPositionOrNull(const DocumentPtr& document, Cmper partId, StaffCmper staffId, MeasCmper measureId, EntryNumber entryNumber, util::Fraction timeOffset)
{
    EntryInfoPtr result;
    if (auto gfhold = details::GFrameHoldContext(document, partId, staffId, measureId, timeOffset)) {
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

EntryInfoPtr EntryInfoPtr::fromEntryNumber(const DocumentPtr& document, Cmper partId, EntryNumber entryNumber, util::Fraction timeOffset)
{
    if (const auto entry = document->getEntries()->get(entryNumber)) {
        if (!entry->locations.empty()) {
            auto [staffId, measureId,layerIndex] = entry->locations[0];
            if (auto gfhold = details::GFrameHoldContext(document, partId, staffId, measureId, timeOffset)) {
                EntryInfoPtr result;
                gfhold.iterateEntries(layerIndex, [&](const EntryInfoPtr& entryInfo) {
                    if (entryInfo->getEntry()->getEntryNumber() == entryNumber) {
                        result = entryInfo;
                        return false; // stop iterating
                    }
                    return true;
                });
                return result;
            }
        }
    }
    return {};
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

bool EntryInfoPtr::calcIsSameChordOrRest(const EntryInfoPtr& src, bool compareConcert) const
{
    if (!*this || !src) {
        return false;
    }
    if (isSameEntry(src)) {
        return true;
    }
    if (calcDisplaysAsRest() != src.calcDisplaysAsRest()) {
        return false;
    }
    const auto entryThis = (*this)->getEntry();
    const auto entrySrc = src->getEntry();
    if (entryThis->notes.size() != entrySrc->notes.size()) {
        return false;
    }
    // only need to compare raw key sigs, so skip KeySignature::isSame.
    const bool sameKeys = this->getFrame()->keySignature->key == src.getFrame()->keySignature->key;
    for (size_t index = 0; index < entryThis->notes.size(); index++) {
        const auto noteThis = NoteInfoPtr(*this, index);
        const auto& noteSrc = NoteInfoPtr(src, index);
        if (sameKeys || !compareConcert) {
            if (noteThis->harmLev != noteSrc->harmLev || noteThis->harmAlt != noteSrc->harmAlt) {
                return false;
            }
        } else {
            auto [noteTypeThis, octaveThis, alterThis, staffLineThis] = noteThis.calcNotePropertiesConcert(true);
            auto [noteTypeSrc, octaveSrc, alterSrc, staffLineSrc] = noteSrc.calcNotePropertiesConcert(true);
            if (noteTypeThis != noteTypeSrc || octaveThis != octaveSrc || alterThis != alterSrc) {
                return false;
            }
        }
    }
    return true;
}

LayerIndex  EntryInfoPtr::getLayerIndex() const { return m_entryFrame->getLayerIndex(); }

StaffCmper EntryInfoPtr::getStaff() const { return m_entryFrame->getStaff(); }

MeasCmper EntryInfoPtr::getMeasure() const { return m_entryFrame->getMeasure(); }

MusxInstance<KeySignature> EntryInfoPtr::getKeySignature() const { return m_entryFrame->keySignature; }

MusxInstance<details::EntryPartFieldDetail> EntryInfoPtr::getPartFieldData() const
{
    const auto frame = getFrame();
    if (frame->getRequestedPartId() != SCORE_PARTID) {
        const auto entry = (*this)->getEntry();
        if (const auto partData = frame->getDocument()->getDetails()->get<details::EntryPartFieldDetail>(frame->getRequestedPartId(), entry->getEntryNumber())) {
            // EntryPartFieldDetail is an outlier in that it is a partially shared entity that should be ignored if it comes from the score.
            if (partData->getSourcePartId() != SCORE_PARTID) {
                return partData;
            }
        }
    }
    return nullptr;
}

Evpu EntryInfoPtr::calcManuaOffset() const
{
    if (const auto partData = getPartFieldData()) {
        return partData->hOffset;
    }
    return (*this)->getEntry()->hOffsetScore;
}

std::pair<bool, bool> EntryInfoPtr::calcEntryStemSettings() const
{
    if (const auto partData = getPartFieldData()) {
        return std::make_pair(partData->freezeStem, partData->upStem);
    }
    const auto entry = (*this)->getEntry();
    return std::make_pair(entry->freezeStemScore, entry->upStemScore);
}

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

util::Fraction EntryInfoPtr::calcGraceEllapsedDuration() const
{
    util::Fraction result = 0;
    for (auto entryInfoPtr = *this; entryInfoPtr; entryInfoPtr = entryInfoPtr.getNextSameV()) {
        auto entry = entryInfoPtr->getEntry();
        if (!entryInfoPtr->getEntry()->graceNote) {
            break;
        }
        result -= util::Fraction::fromEdu(entry->duration);
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
    auto anchor = findRightBeamAnchorForBeamOverBarline();
    if (anchor && !anchor.findBeamEnd().isSameEntry(*this)) {
        auto result = getNextSameVNoGrace();
        if (result && result.calcCreatesSingletonBeamLeft()) {
            result = result.getNextInBeamGroup(includeHiddenEntries);
        }
        return result;
    }
    return getNextInBeamGroup(includeHiddenEntries);
}

EntryInfoPtr EntryInfoPtr::getPreviousInBeamGroupAcrossBars(bool includeHiddenEntries) const
{
    if (auto prevBarCont = calcBeamContinuesLeftOverBarline()) {
        return prevBarCont;
    }
    auto anchor = findLeftBeamAnchorForBeamOverBarline();
    if (anchor && !anchor.isSameEntry(*this)) {
        auto result = getPreviousSameVNoGrace();
        if (result && result.findBeamStartOrCurrent().calcCreatesSingletonBeamRight()) {
            result = result.getPreviousInBeamGroup(includeHiddenEntries);
        }
        return result;
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
        return std::get<1>(calcEntryStemSettings()); // Use whatever Finale last calculated.
    }

    // for beams, if the diffs don't determine it, then the number above & below do
    // (Function calcUnBeamed is false for beamed groups.)
    if (minBotDiff < 0 && minBotDiff == -maxTopDiff && !calcUnbeamed()) {
        return numBelow > numAbove;
    }

    const int valForBeam = std::abs(minBotDiff) > std::abs(maxTopDiff) ? minBotDiff : maxTopDiff;

    return valForBeam < 0;
}

bool EntryInfoPtr::calcUpStemImpl() const
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
        const auto [freezeStem, upStem] = next.calcEntryStemSettings();
        if (freezeStem) {
            return upStem;
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
            return std::get<1>(next.calcEntryStemSettings());
        }
    }
    // cross-staff direction was not part of the 2001 testing, but this seems the right place for it for now.
    const auto scrollViewStaves = frame->getDocument()->getScrollViewStaves(frame->getRequestedPartId());
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
    if (!calcCanBeBeamed()) return true;
    if ((*this)->getEntry()->isHidden) {
        return (!getNextInBeamGroup() || !getPreviousInBeamGroup());
    }
    return (!getNextInBeamGroup() && !getPreviousInBeamGroup());
}

bool EntryInfoPtr::calcCanBeBeamed() const
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
    return (*this)->getEntry()->beam || !calcCanBeBeamed();
}

bool EntryInfoPtr::calcIsBeamStart() const
{
    if ((*this)->getEntry()->isHidden) return false;
    if (!calcCanBeBeamed()) return false;
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
    return beamExt && beamExt->calcMaxExtension() && beamExt->leftOffset < 0;
};

static bool checkBeamExtRight(const MusxInstance<details::BeamExtension>& beamExt) {
    return beamExt && beamExt->calcMaxExtension() && beamExt->rightOffset > 0;
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
    if (anchorEntryInfo && anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.calcCanBeBeamed()) {
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
    if (anchorEntryInfo && !anchorEntryInfo->getEntry()->isHidden && !anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.calcCanBeBeamed()) {
        auto beamStart = anchorEntryInfo.findBeamStartOrCurrent();
        if (beamStart.getIndexInFrame() <= getIndexInFrame()) {
            if (!beamStart.calcCreatesSingletonBeamRight()) {
                if (!checkBeamExtRight(details::BeamExtension::getForStem(beamStart))) {
                    return {};
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
    if (anchorEntryInfo && anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.calcCanBeBeamed()) {
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
    if (anchorEntryInfo && !anchorEntryInfo->getEntry()->isHidden && !anchorEntryInfo.calcDisplaysAsRest() && anchorEntryInfo.calcCanBeBeamed()) {
        auto beamStart = anchorEntryInfo.findBeamStartOrCurrent();
        if (beamStart.calcCreatesSingletonBeamLeft()) {
            auto beamEnd = beamStart.findBeamEnd();
            if (beamEnd.getIndexInFrame() >= getIndexInFrame()) {
                return beamStart;
            }
        }
        if (beamStart.getIndexInFrame() >= getIndexInFrame()) {
            if (!checkBeamExtLeft(details::BeamExtension::getForStem(beamStart))) {
                return {};
            }
            return beamStart;
        }
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::findHiddenSourceForBeamOverBarline() const
{
    auto frame = getFrame();
    if ((*this)->elapsedDuration < frame->measureStaffDuration) {
        return {};
    }
    if ((*this)->elapsedDuration == frame->measureStaffDuration) {
        // Check for grace notes at the beginning of the next frame.
        if (auto nextFrame = frame->getNext()) {
            auto nextFirst = EntryInfoPtr(nextFrame, 0);
            const auto nextGraceDura = nextFirst.calcGraceEllapsedDuration();
            const auto thisGraceDura = calcGraceEllapsedDuration();
            if (thisGraceDura < nextGraceDura) {
                return {}; // out of sync can't be a match.
            }
        }
    }
    // Note that calcNearestEntry skips grace notes unless a grace note ellapsed dura is supplied.
    auto currEntry = frame->calcNearestEntry(frame->measureStaffDuration, /*findExact*/true);
    if (!currEntry) {
        return {};
    }
    util::Fraction normalizedStartDuration = (*this)->elapsedDuration - frame->measureStaffDuration;
    bool firstIteration = true;
    for (frame = frame->getNext(); frame && currEntry; frame = frame->getNext()) {
        auto searchEntry = EntryInfoPtr(frame, 0);
        if (firstIteration) {
            // find first non gracenote in frame
            searchEntry = frame->calcNearestEntry(0, /*findExact*/true);
            firstIteration = false;
            // backup for grace notes, if any
            while (searchEntry.getIndexInFrame() > 0) {
                searchEntry = searchEntry.getPreviousInFrame();
                currEntry = currEntry.getPreviousInFrame();
                if (!currEntry) {
                    return {};
                }
            }
        }
        for (; currEntry && searchEntry; currEntry = currEntry.getNextInFrame(), searchEntry = searchEntry.getNextInFrame()) {
            auto rawSearchEntry = searchEntry->getEntry();
            auto rawCurrEntry = currEntry->getEntry();
            if (!rawSearchEntry->isHidden || rawSearchEntry->voice2 != rawCurrEntry->voice2) {
                return {}; // if we encounter a non-hidden entry or not matching v1v2, no match
            }
            if (rawSearchEntry->duration != rawCurrEntry->duration) {
                return {}; // if we encounter non-matching symbolic durations, no match
            }
            if (searchEntry->actualDuration != currEntry->actualDuration) {
                return {}; // if we encounter non-matching actual durations, no match
            }
            if (searchEntry.calcGraceEllapsedDuration() != currEntry.calcGraceEllapsedDuration()) {
                return {}; // if we encounter non-matching grace note durations, no match
            }
            // Beam Over Barlines does not transpose entries if there is a key change, so concertCompare false is correct.
            if (!searchEntry.calcIsSameChordOrRest(currEntry, /*concertCompare*/false)) {
                return {}; // rest display must match.
            }
            const auto searchGraceDura = searchEntry.calcGraceEllapsedDuration();
            if (searchEntry->elapsedDuration == frame->measureStaffDuration) {
                // Beam Over Barline plugin crams hidden entries in the current frame for alls subsequent frames,
                // so we need an extra check for grace notes at the beginning of the next frame.
                if (auto nextFrame = frame->getNext()) {
                    auto nextFirst = EntryInfoPtr(nextFrame, 0);
                    const auto nextGraceDura = nextFirst.calcGraceEllapsedDuration();
                    if (nextGraceDura < searchGraceDura) {
                        return {}; // out of sync can't be a match.
                    } else if (nextGraceDura == searchGraceDura) {
                        break;
                    }
                }
            }
            if (searchEntry->elapsedDuration == normalizedStartDuration && searchGraceDura == this->calcGraceEllapsedDuration()) {
                return searchEntry; // got it!
            }
        }
        if (normalizedStartDuration < frame->measureStaffDuration) {
            break;
        }
        normalizedStartDuration -= frame->measureStaffDuration;
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::findDisplayEntryForBeamOverBarline() const
{
    if (!(*this)->getEntry()->isHidden || !calcCanBeBeamed()) {
        return {};
    }
    // search from beginning of measure.
    auto frame = getFrame();
    auto prevFrame = frame->getPrevious();
    if (!prevFrame) {
        return {};
    }
    auto currEntry = frame->calcNearestEntry(0, /*findExact*/true);
    if (!currEntry) {
        return {};
    }
    util::Fraction prevDurationOffset = prevFrame->measureStaffDuration;
    auto searchEntry = prevFrame->calcNearestEntry(prevDurationOffset, /*findExact*/true);

    // backup for grace notes, if any
    while (currEntry.getIndexInFrame() > 0) {
        currEntry = currEntry.getPreviousInFrame();
        searchEntry = searchEntry.getPreviousInFrame();
        if (!searchEntry) {
            return {};
        }
    }

    // search forward to exactly our index value.
    for (size_t x = 0; x <= getIndexInFrame(); x++) {
        auto rawSearchEntry = searchEntry->getEntry();
        auto rawCurrEntry = currEntry->getEntry();
        if (rawSearchEntry->voice2 != rawCurrEntry->voice2) {
            return {}; // if we encounter a non matching v1v2, no match
        }
        if (rawSearchEntry->duration != rawCurrEntry->duration) {
            return {}; // if we encounter non-matching symbolic durations, no match
        }
        if (searchEntry->actualDuration != currEntry->actualDuration) {
            return {}; // if we encounter non-matching actual durations, no match
        }
        if (searchEntry.calcGraceEllapsedDuration() != currEntry.calcGraceEllapsedDuration()) {
            return {}; // if we encounter non-matching grace note durations, no match
        }
        // Beam Over Barlines does not transpose entries if there is a key change, so concertCompare false is correct.
        if (!searchEntry.calcIsSameChordOrRest(currEntry, /*concertCompare*/false)) {
            return {}; // rest display must match.
        }
        if (searchEntry->elapsedDuration != currEntry->elapsedDuration + prevDurationOffset && searchEntry.calcGraceEllapsedDuration() == currEntry.calcGraceEllapsedDuration()) {
            return {};
        }
        if (x == getIndexInFrame()) {
            break;
        }
        currEntry = currEntry.getNextInFrame();
        MUSX_ASSERT_IF(!currEntry) {
            throw std::logic_error("Unable to advance enough times inside our own frame.");
        }
        searchEntry = searchEntry.getNextInFrame();
        if (!searchEntry) {
            return {};
        }
    }
    if (searchEntry && searchEntry->getEntry()->isHidden) {
        // if we get here, we need to back up another measure and search again.
        return searchEntry.findDisplayEntryForBeamOverBarline();
    }
    return searchEntry;
}

EntryInfoPtr EntryInfoPtr::findMainEntryForGraceNote(bool ignoreRests) const
{
    const auto entry = (*this)->getEntry();
    if (!entry->graceNote) {
        return {};
    }
    if (const auto nextNonGrace = getNextSameVNoGrace()) {
        if (ignoreRests && nextNonGrace.calcDisplaysAsRest()) {
            return {};
        }
        if (const auto nextNonGraceHiddenForBeamOverBarline = nextNonGrace.findHiddenSourceForBeamOverBarline()) {
            const auto hiddenForBeamOverBarline = findHiddenSourceForBeamOverBarline();
            if (!hiddenForBeamOverBarline) {
                return {};
            }
            if (hiddenForBeamOverBarline.getMeasure() != nextNonGraceHiddenForBeamOverBarline.getMeasure()) {
                return {};
            }
        }
        return nextNonGrace;
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::calcBeamContinuesLeftOverBarline() const
{
    const auto entry = (*this)->getEntry();
    if (entry->graceNote) {
        return {};
    }

    const auto anchor = findRightBeamAnchorForBeamOverBarline();
    if (!anchor) {
        return {};
    }

    auto prevInVoice = *this;
    if (findBeamStartOrCurrent().calcCreatesSingletonBeamLeft()) {
        prevInVoice = prevInVoice.getPreviousSameVNoGrace();
    }
    if (prevInVoice) {
        prevInVoice = prevInVoice.getPreviousSameVNoGrace();
    }
    if (prevInVoice) {
        return prevInVoice;
    }

    auto prevFrame = getFrame()->getPrevious();
    if (!prevFrame) {
        return {};
    }
    int voice = static_cast<int>(entry->voice2) + 1;
    for (auto prevEntryInfo = prevFrame->getLastInVoice(voice); prevEntryInfo; prevEntryInfo = prevEntryInfo.getPreviousSameV()) {
        if (prevEntryInfo->getEntry()->graceNote) {
            continue;
        }
        auto leftAnchor = prevEntryInfo.findLeftBeamAnchorForBeamOverBarline();
        if (!leftAnchor) {
            return {};
        }
        if (prevEntryInfo->actualDuration != 0 && prevEntryInfo.findBeamStartOrCurrent().calcCreatesSingletonBeamRight()) {
            prevEntryInfo = prevEntryInfo.getPreviousSameV();
        }
        return prevEntryInfo;
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
        auto rightAnchor = nextEntryInfo.findRightBeamAnchorForBeamOverBarline();
        if (!rightAnchor) {
            return {};
        }
        if (nextEntryInfo.calcCreatesSingletonBeamLeft()) {
            return nextEntryInfo.getNextInBeamGroup();
        }
        return nextEntryInfo;
    }
    return {};
}

EntryInfoPtr EntryInfoPtr::findBeamStartOrCurrent() const
{
    if (!calcCanBeBeamed()) return *this;
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

unsigned EntryInfoPtr::calcLowestBeamStart(bool considerBeamOverBarlines) const
{
    if ((*this)->getEntry()->isHidden) return 0;
    if (considerBeamOverBarlines) {
        if (auto anchor = findRightBeamAnchorForBeamOverBarline()) {
            if (auto beamExt = details::BeamExtension::getForStem(anchor)) {
                unsigned prevNumberOfBeams = beamExt->calcMaxExtension();
                if (calcVisibleBeams() > prevNumberOfBeams) {
                    return prevNumberOfBeams + 1;
                } else {
                    return 0;
                }
            }
        }
    }
    if (!calcCanBeBeamed()) return 0;
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
    if (!calcCanBeBeamed()) return 0;
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
    if (calcBeamContinuesRightOverBarline() && !getNextInBeamGroup()) {
        auto anchor = findLeftBeamAnchorForBeamOverBarline();
        MUSX_ASSERT_IF(!anchor) {
            throw std::logic_error("calcBeamContinuesRightOverBarline was true but no anchor exists.");
        }
        unsigned numBeams = calcVisibleBeams();
        unsigned nextNumBeams = numBeams;
        if (auto beamExt = details::BeamExtension::getForStem(anchor)) { // beamExt should always exist.
            nextNumBeams = beamExt->calcMaxExtension();
        }
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
    if (!result || !result.calcCanBeBeamed()) {
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
    if (!calcCanBeBeamed()) {
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
    const auto entry = (*this)->getEntry();
    if (entry->isPossibleFullMeasureRest() && (*this)->elapsedDuration == 0) {
        if (entry->voice2) {
            auto layerInfo = getFrame()->getContext().calcVoices();
            auto it = layerInfo.find(getLayerIndex());
            if (it != layerInfo.end()) {
                if (it->second == 1) { // exactly 1 v2 entry in this layer.
                    return calcManuaOffset() > 0; // it has to have been moved towards the center of the measure.
                }
            }
        }
        if (entry->v2Launch) {
            if (getNextSameV()) {
                return false;
            }
            return calcManuaOffset() > 0; // it has to have been moved towards the center of the measure.
        }
        return m_entryFrame->getEntries().size() == 1;
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
    for (LayerIndex nextLayerIndex = 0; nextLayerIndex < static_cast<LayerIndex>(context->frames.size()); nextLayerIndex++) {
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
        staffList.emplace(frame->getDocument()->getScrollViewStaves(frame->getRequestedPartId()));
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

bool EntryInfoPtr::calcIsSingletonGrace() const
{
    if ((*this)->getEntry()->graceNote) {
        if (auto prev = getPreviousSameV(); !prev || !prev->getEntry()->graceNote) {
            if (auto next = getNextSameV(); !next || !next->getEntry()->graceNote) {
                return true;
            }
        }
    }
    return false;
}

int EntryInfoPtr::calcIsAuxiliaryPitchMarker() const
{
    if (!calcIsSingletonGrace()) {
        return false;
    }
    if (auto customStem = details::CustomStem::getForStem(*this); !customStem || !customStem->calcIsHiddenStem()) {
        return false;
    }
    return true;
}

bool EntryInfoPtr::calcIsTrillToGraceEntry() const
{
    if (!calcIsAuxiliaryPitchMarker()) {
        return false;
    }
    auto mainEntry = getNextSameV();
    if (!mainEntry) {
        return false;
    }
    MUSX_ASSERT_IF(mainEntry->getEntry()->graceNote) {
        throw std::logic_error("Next entry after calcIsAuxiliaryPitchMarker entry is still a grace note.");
    }
    auto graceDistance = static_cast<Evpu>(EVPU_PER_SPACE);
    if (auto graceOptions = getFrame()->getDocument()->getOptions()->get<options::GraceNoteOptions>()) {
        graceDistance = graceOptions->entryOffset;
    }
    graceDistance = calcManuaOffset() - graceDistance;
    if (graceDistance < EVPU_PER_SPACE) {
        return false;
    }
    return true;
}

bool EntryInfoPtr::calcIsGlissToGraceEntry() const
{
    if (!calcIsAuxiliaryPitchMarker()) {
        return false;
    }
    const auto entry = (*this)->getEntry();
    if (!entry->smartShapeDetail) {
        return false;
    }
    const auto frame = getFrame();
    const auto smartShapeAssigns = frame->getDocument()->getDetails()->getArray<details::SmartShapeEntryAssign>(frame->getRequestedPartId(), entry->getEntryNumber());
    for (const auto& asgn : smartShapeAssigns) {
        if (const auto shape = frame->getDocument()->getOthers()->get<others::SmartShape>(frame->getRequestedPartId(), asgn->shapeNum)) {
            switch (shape->shapeType) {
            case others::SmartShape::ShapeType::Glissando:
            case others::SmartShape::ShapeType::TabSlide:
                if (shape->endTermSeg->endPoint->entryNumber == entry->getEntryNumber()) {
                    return true;
                }
            default:
                break;
            }
        }
    }
    return false;
}

// *****************************
// ***** GFrameHoldContext *****
// *****************************

details::GFrameHoldContext::GFrameHoldContext(const DocumentPtr& document, Cmper partId, Cmper staffId, Cmper measureId, util::Fraction timeOffset)
    : m_requestedPartId(partId), m_timeOffset(timeOffset)
{
    m_hold = document->getDetails()->get<details::GFrameHold>(partId, staffId, measureId);
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

std::shared_ptr<const EntryFrame> details::GFrameHoldContext::createEntryFrame(LayerIndex layerIndex) const
{
    if (!m_hold) return nullptr;
    if (layerIndex >= m_hold->frames.size()) { // note: layerIndex is unsigned
        throw std::invalid_argument("invalid layer index [" + std::to_string(layerIndex) + "]");
    }
    if (!m_hold->frames[layerIndex]) return nullptr; // nothing here
    auto document = m_hold->getDocument();
    auto [frame, startEdu] = m_hold->findLayerFrame(layerIndex);
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
        entryFrame->measureStaffDuration = measure->calcDuration(m_hold->getStaff());
        auto entries = frame->getEntries();
        std::vector<TupletState> v1ActiveTuplets; // List of active tuplets for v1
        std::vector<TupletState> v2ActiveTuplets; // List of active tuplets for v2
        util::Fraction v1ActualElapsedDuration = util::Fraction::fromEdu(startEdu) - m_timeOffset;
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
        entryFrame->maxElapsedStaffDuration = (std::max)(v1ActualElapsedDuration, v2ActualElapsedDuration);
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
        return entryFrame->iterateEntries(iterator);
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

std::map<LayerIndex, int> details::GFrameHoldContext::calcVoices() const
{
    std::map<LayerIndex, int> result;
    for (LayerIndex layerIndex = 0; layerIndex < m_hold->frames.size(); layerIndex++) {
        auto [frame, startEdu] = m_hold->findLayerFrame(layerIndex);
        if (frame) {
            bool gotLayer = false;
            int numV2 = 0;
            frame->iterateRawEntries([&](const MusxInstance<Entry>& entry) -> bool {
                gotLayer = true;
                if (entry->voice2) {
                    numV2++;
                }
                return true;
            });
            if (gotLayer) {
                result.emplace(layerIndex, numV2);
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
        auto [frame, startEdu] = m_hold->findLayerFrame(layerIndex);
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

EntryInfoPtr details::GFrameHoldContext::calcNearestEntry(util::Fraction position, bool findExact, std::optional<LayerIndex> matchLayer,
    std::optional<bool> matchVoice2, util::Fraction atGraceNoteDuration) const
{
    LayerIndex startLayer = matchLayer.value_or(0);
    for (LayerIndex layerIndex = startLayer; layerIndex < m_hold->frames.size(); layerIndex++) {
        if (auto entryFrame = createEntryFrame(layerIndex)) {
            if (auto result = entryFrame->calcNearestEntry(position, findExact, matchVoice2, atGraceNoteDuration)) {
                return result;
            }
        }
        if (matchLayer.has_value()) {
            break; // only iterate once if matchLayer specified.
        }
    }

    return {};
}

util::Fraction details::GFrameHoldContext::calcMinLegacyPickupSpacer() const
{
    Edu result = -1;
    for (LayerIndex layerIndex = 0; layerIndex < MAX_LAYERS; layerIndex++) {
        auto [frame, startEdu] = m_hold->findLayerFrame(layerIndex);
        if (frame) {
            if (result < 0 || startEdu < result) {
                result = startEdu;
            }
        }
    }
    if (result >= 0) {
        return util::Fraction::fromEdu(result);
    }
    return 0;
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
                    while (nextEntry && nextEntry.getMeasure() == m_entry.getMeasure() && nextEntry->elapsedDuration < nextDuration) {
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
        staffList.emplace(frame->getDocument()->getScrollViewStaves(frame->getRequestedPartId()));
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