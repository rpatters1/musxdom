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
#include <type_traits>

#include "musx/musx.h"

namespace musx {
namespace dom {

// ****************************
// ***** EntryDetailsBase *****
// ****************************

template <typename EDUP, typename EDDOWN, typename EDBASE>
MusxInstance<EDBASE> EntryDetailsBase::getStemDependentDetail(const EntryInfoPtr& entryInfo, StemSelection stemSelection)
{
    static_assert(std::is_base_of_v<EntryDetailsBase, EDBASE>, "EDBASE must be derived from EntryDetailsBase.");
    static_assert(std::is_base_of_v<EDBASE, EDUP>, "EDUP must be derived from EDBASE");
    static_assert(std::is_base_of_v<EDBASE, EDDOWN>, "EDDOWN must be derived from EDBASE");

    const auto frame = entryInfo.getFrame();
    const auto entry = entryInfo->getEntry();

    switch (stemSelection) {
    case StemSelection::UpStem:
        return frame->getDocument()->getDetails()->get<EDUP>(frame->getRequestedPartId(), entry->getEntryNumber());
    case StemSelection::DownStem:
        return frame->getDocument()->getDetails()->get<EDDOWN>(frame->getRequestedPartId(), entry->getEntryNumber());
    case StemSelection::MatchEntry:
        if (entryInfo.calcUpStem()) {
            return frame->getDocument()->getDetails()->get<EDUP>(frame->getRequestedPartId(), entry->getEntryNumber());
        } else {
            return frame->getDocument()->getDetails()->get<EDDOWN>(frame->getRequestedPartId(), entry->getEntryNumber());
        }
    case StemSelection::Any:
        if (auto upStem = frame->getDocument()->getDetails()->get<EDUP>(frame->getRequestedPartId(), entry->getEntryNumber())) {
            return upStem;
        } else {
            return frame->getDocument()->getDetails()->get<EDDOWN>(frame->getRequestedPartId(), entry->getEntryNumber());
        }
    }
    return nullptr;
}

namespace details {

// ***************************
// ***** BeamAlterations *****
// ***************************

template <typename T,
          std::enable_if_t<std::is_base_of_v<BeamAlterations, T>, int>>
void BeamAlterations::calcAllActiveFlags(const DocumentPtr& document)
{
    if (const auto beamOptions = document->getOptions()->get<options::BeamOptions>()) {
        const auto values = document->getDetails()->getArray<T>(SCORE_PARTID);
        for (const auto& value : values) {
            T* mutableValue = const_cast<T*>(value.get());
            mutableValue->m_active = (value->flattenStyle == beamOptions->beamingStyle);
        }
    } else {
        MUSX_INTEGRITY_ERROR("Unable to retrieve beaming options. Active indicators for beam alterations were not set.");
    }
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template void BeamAlterations::calcAllActiveFlags<BeamAlterationsUpStem>(const DocumentPtr&);
template void BeamAlterations::calcAllActiveFlags<BeamAlterationsDownStem>(const DocumentPtr&);
template void BeamAlterations::calcAllActiveFlags<SecondaryBeamAlterationsUpStem>(const DocumentPtr&);
template void BeamAlterations::calcAllActiveFlags<SecondaryBeamAlterationsDownStem>(const DocumentPtr&);
#endif // DOXYGEN_SHOULD_IGNORE_THIS

Efix BeamAlterations::calcEffectiveBeamWidth() const
{
    if (getInci().has_value()) { // secondary beams have incis; primary beams do not
        MusxInstance<BeamAlterations> primary;
        if (dynamic_cast<const SecondaryBeamAlterationsDownStem*>(this)) {
            primary = getDocument()->getDetails()->get<BeamAlterationsDownStem>(getRequestedPartId(), getEntryNumber());
        } else {
            primary = getDocument()->getDetails()->get<BeamAlterationsUpStem>(getRequestedPartId(), getEntryNumber());
        }
        if (primary) {
            return primary->calcEffectiveBeamWidth();
        }
    } else if (m_active) {
        if (beamWidth >= 0) {
            return beamWidth;
        }
    }
    Efix result = 0;
    if (const auto beamOptions = getDocument()->getOptions()->get<options::BeamOptions>()) {
        result = beamOptions->beamWidth;
    } else {
        MUSX_INTEGRITY_ERROR("Unable to retrieve beaming options. Beam width value returned is zero.");
    }
    return result;
}

template <typename SecondaryBeamType>
bool BeamAlterations::calcIsFeatheredBeamImpl(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY)
{
    static_assert(std::is_same_v<SecondaryBeamType, SecondaryBeamAlterationsDownStem>
               || std::is_same_v<SecondaryBeamType, SecondaryBeamAlterationsUpStem>,
        "SecondaryBeamType must be a secondary beam type.");
    constexpr bool isDownstem = std::is_same_v<SecondaryBeamType, SecondaryBeamAlterationsDownStem>;
    constexpr int direction = isDownstem ? 1 : -1;

    const auto& frame = entryInfo.getFrame();
    const auto& doc = frame->getDocument();
    const auto& secondaries = doc->getDetails()->getArray<SecondaryBeamType>(
        frame->getRequestedPartId(), entryInfo->getEntry()->getEntryNumber());

    const auto beamOptions = doc->getOptions()->get<options::BeamOptions>();
    if (!beamOptions) {
        MUSX_INTEGRITY_ERROR("Unable to retrieve BeamOptions for determining feathered beaming.");
    }

    const Evpu beamSpacing = beamOptions ? beamOptions->beamSepar : 18;
    const Evpu beamWidth   = beamOptions ? Evpu(std::round(beamOptions->beamWidth / EFIX_PER_EVPU)) : 12;

    Evpu leftY = direction * beamWidth;
    Evpu rightY = leftY;
    Evpu extremeLeft = leftY;
    Evpu extremeRight = rightY;

    for (const auto& sec : secondaries) {
        if (!sec->isActive()) continue;

        const Evpu dyL = sec->leftOffsetY + direction * beamSpacing;
        const Evpu dyR = (sec->leftOffsetY + sec->rightOffsetY) + direction * beamSpacing;

        leftY += dyL;
        rightY += dyR;

        if constexpr (isDownstem) {
            extremeLeft  = (std::max)(extremeLeft, leftY);
            extremeRight = (std::max)(extremeRight, rightY);
        } else {
            extremeLeft  = (std::min)(extremeLeft, leftY);
            extremeRight = (std::min)(extremeRight, rightY);
        }
    }

    const Evpu spanLeft  = direction * extremeLeft;
    const Evpu spanRight = direction * extremeRight;

    if (spanLeft != spanRight) {
        outLeftY = spanLeft;
        outRightY = spanRight;
        return true;
    }

    return false;
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template bool BeamAlterations::calcIsFeatheredBeamImpl<SecondaryBeamAlterationsUpStem>(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY);
template bool BeamAlterations::calcIsFeatheredBeamImpl<SecondaryBeamAlterationsDownStem>(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY);
#endif // DOXYGEN_SHOULD_IGNORE_THIS

template <typename SecondaryBeamType>
MusxInstanceList<SecondaryBeamType> BeamAlterations::getSecondaryBeamArray(const EntryInfoPtr& entryInfo)
{
    static_assert(std::is_same_v<SecondaryBeamType, SecondaryBeamAlterationsDownStem>
               || std::is_same_v<SecondaryBeamType, SecondaryBeamAlterationsUpStem>,
        "SecondaryBeamType must be a secondary beam type.");

    auto frame = entryInfo.getFrame();
    return frame->getDocument()->getDetails()->getArray<SecondaryBeamType>(frame->getRequestedPartId(), entryInfo->getEntry()->getEntryNumber());
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template MusxInstanceList<SecondaryBeamAlterationsUpStem> BeamAlterations::getSecondaryBeamArray<SecondaryBeamAlterationsUpStem>(const EntryInfoPtr& entryInfo);
template MusxInstanceList<SecondaryBeamAlterationsDownStem> BeamAlterations::getSecondaryBeamArray<SecondaryBeamAlterationsDownStem>(const EntryInfoPtr& entryInfo);
#endif // DOXYGEN_SHOULD_IGNORE_THIS

MusxInstance<BeamAlterations> BeamAlterations::getPrimaryForStem(const EntryInfoPtr& entryInfo, StemSelection stemSelection)
{
    if (entryInfo->getEntry()->stemDetail) {
        return getStemDependentDetail<BeamAlterationsUpStem, BeamAlterationsDownStem, BeamAlterations>(entryInfo, stemSelection);
    }
    return nullptr;
}

MusxInstance<BeamAlterations> BeamAlterations::getSecondaryForStem(const EntryInfoPtr& entryInfo, Edu dura, StemSelection stemSelection)
{
    auto searchArray = [&](auto beamAlts) -> MusxInstance<BeamAlterations> {
        auto it = std::find_if(beamAlts.begin(), beamAlts.end(), [&](const auto& beamAlt) {
            return beamAlt->dura == dura;
        });
        if (it != beamAlts.end()) {
            return *it;
        }
        return nullptr;
    };
    if (entryInfo->getEntry()->stemDetail) {
        switch (stemSelection) {
        case StemSelection::UpStem:
            return searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsUpStem>(entryInfo));
        case StemSelection::DownStem:
            return searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsDownStem>(entryInfo));
        case StemSelection::MatchEntry:
            if (entryInfo.calcUpStem()) {
                return searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsUpStem>(entryInfo));
            } else {
                return searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsDownStem>(entryInfo));
            }
        case StemSelection::Any:
            if (auto upStem = searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsUpStem>(entryInfo))) {
                return upStem;
            } else {
                return searchArray(getSecondaryBeamArray<SecondaryBeamAlterationsDownStem>(entryInfo));
            }
        }
    }
    return {};
}

// *************************
// ***** BeamExtension *****
// *************************

unsigned BeamExtension::calcMaxExtension() const
{
    // Legacy behavior: only the 8th-note beam is extended.
    // Regardless of mask contents, Finale would extend only the first beam.
    if (!extBeyond8th) {
        return 1U;
    }

    if (mask) {
        unsigned maxExtension = 10;      // 1 = 8th, ..., 10 = 4096th
        unsigned workingMask  = mask;
        while ((workingMask & 0x01U) == 0U && maxExtension > 0U) {
            workingMask >>= 1U;
            maxExtension--;
        }
        return maxExtension;             // always 1–10 for nonzero mask
    }

    // Fallback: treat as 8th.
    return 0U;
}

MusxInstance<BeamExtension> BeamExtension::getForStem(const EntryInfoPtr& entryInfo, StemSelection stemSelection) {
    if (entryInfo->getEntry()->beamExt) {
        return getStemDependentDetail<BeamExtensionUpStem, BeamExtensionDownStem, BeamExtension>(entryInfo, stemSelection);
    }
    return nullptr;
}

// ***********************
// ***** ChordAssign *****
// ***********************

Cmper ChordAssign::calcFretboardGroupCmper() const
{
    if (suffixId != 0) {
        return suffixId;
    } else if (rootLowerCase) {
        return 65533; // hard-coded minor triad fretboard group
    }
    return 65534; // hard-coded major triad fretboard group
}

MusxInstanceList<others::ChordSuffixElement> ChordAssign::getChordSuffix() const
{
    return getDocument()->getOthers()->getArray<others::ChordSuffixElement>(getRequestedPartId(), suffixId);
}

MusxInstance<others::FretboardGroup> ChordAssign::getFretboardGroup() const
{
    const Cmper groupCmper = calcFretboardGroupCmper();
    if (!useFretboardFont && groupCmper != 0) {
        return getDocument()->getOthers()->get<others::FretboardGroup>(getRequestedPartId(), groupCmper, fretboardGroupInci);
    }
    return nullptr;
}

MusxInstance<others::FretboardStyle> ChordAssign::getFretboardStyle() const
{
    if (!useFretboardFont && fbStyleId != 0) {
        return getDocument()->getOthers()->get<others::FretboardStyle>(getRequestedPartId(), fbStyleId);
    }
    return nullptr;
}

// **********************
// ***** CustomStem *****
// **********************

bool CustomStem::calcIsHiddenStem() const
{
    if (shapeDef != 0) {
        if (const auto shape = getDocument()->getOthers()->get<others::ShapeDef>(getRequestedPartId(), shapeDef)) {
            return shape->isBlank();
        }
    }
    return true;
}

MusxInstance<CustomStem> CustomStem::getForStem(const EntryInfoPtr& entryInfo, StemSelection stemSelection)
{
    if (entryInfo->getEntry()->stemDetail) {
        return getStemDependentDetail<CustomUpStem, CustomDownStem, CustomStem>(entryInfo, stemSelection);
    }
    return nullptr;
}

// ***********************************
// ***** IndependentStaffDetails *****
// ***********************************

MusxInstance<TimeSignature> IndependentStaffDetails::createTimeSignature() const
{
   return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, altNumTsig, altDenTsig));
}

MusxInstance<TimeSignature> IndependentStaffDetails::createDisplayTimeSignature() const
{
    if (!displayAbbrvTime) {
        return createTimeSignature();
    }
    return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivBeat, displayAltNumTsig, displayAltDenTsig, displayAbbrvTime));
}

// ************************
// ***** LyricsAssign *****
// ************************

MusxInstance<texts::LyricsTextBase> LyricAssignVerse::getLyricText() const
{
    return getDocument()->getTexts()->get<TextType>(lyricNumber);
}

MusxInstance<texts::LyricsTextBase> LyricAssignChorus::getLyricText() const
{
    return getDocument()->getTexts()->get<TextType>(lyricNumber);
}

MusxInstance<texts::LyricsTextBase> LyricAssignSection::getLyricText() const
{
    return getDocument()->getTexts()->get<TextType>(lyricNumber);
}

util::EnigmaParsingContext LyricAssign::getRawTextCtx() const
{
    // note that lyrics do not have text inserts. The UI doesn't permit them.
    if (auto rawText = getLyricText()) {
        return rawText->getRawTextCtx(rawText, SCORE_PARTID);
    }
    return {};
}

// *****************************
// ***** MeasureTextAssign *****
// *****************************

MusxInstance<others::TextBlock> MeasureTextAssign::getTextBlock() const
{
    return getDocument()->getOthers()->get<others::TextBlock>(getRequestedPartId(), block);
}

util::EnigmaParsingContext MeasureTextAssign::getRawTextCtx(Cmper forPartId) const
{
    if (auto textBlock = getTextBlock()) {
        if (const auto page = getDocument()->calculatePageFromMeasure(forPartId, getCmper2())) {
            return textBlock->getRawTextCtx(forPartId, page->getCmper());
        }
    }
    return {};
}

// **********************
// ***** StaffGroup *****
// **********************

std::string StaffGroup::getFullName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), fullNameId, getRequestedPartId(), true, accidentalStyle); // true: strip enigma tags
}

std::string StaffGroup::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), abbrvNameId, getRequestedPartId(), true, accidentalStyle); // true: strip enigma tags
}

MusxInstance<others::MultiStaffInstrumentGroup> StaffGroup::getMultiStaffInstGroup() const
{
    if (multiStaffGroupId) {
        if (auto retval = getDocument()->getOthers()->get<others::MultiStaffInstrumentGroup>(SCORE_PARTID, multiStaffGroupId)) {
            return retval;
        }
        MUSX_INTEGRITY_ERROR("StaffGroup " + std::to_string(getCmper2()) + " points to non-existent MultiStaffInstrumentGroup " + std::to_string(multiStaffGroupId));
    }
    return nullptr;
}

std::string StaffGroup::getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto multiStaffGroup = getMultiStaffInstGroup()) {
        if (auto staff = multiStaffGroup->getFirstStaffInstance()) {
            return staff->getFullInstrumentName(accidentalStyle);
        }
    }
    return getFullName(accidentalStyle);
}

std::string StaffGroup::getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto multiStaffGroup = getMultiStaffInstGroup()) {
        if (auto staff = multiStaffGroup->getFirstStaffInstance()) {
            return staff->getAbbreviatedInstrumentName(accidentalStyle);
        }
    }
    return getAbbreviatedName(accidentalStyle);
}

// **************************
// ***** StaffGroupInfo *****
// **************************

StaffGroupInfo::StaffGroupInfo(const MusxInstance<StaffGroup>& staffGroup,
    const MusxInstanceList<others::StaffUsed>& inpSysStaves) : group(staffGroup), systemStaves(inpSysStaves)
{
    if (inpSysStaves.empty()) {
        throw std::logic_error("Attempt to create StaffGroupInfo with no system staves (StaffGroup " + std::to_string(staffGroup->getCmper2()) + ")");
    }
    auto it = std::find_if(inpSysStaves.begin(), inpSysStaves.end(), [&](const auto& item) {
        return item->staffId == group->startInst;
    });
    if (it != inpSysStaves.end()) {
        startSlot = std::distance(inpSysStaves.begin(), it);
    }
    it = std::find_if(inpSysStaves.begin(), inpSysStaves.end(), [&](const auto& item) {
        return item->staffId == group->endInst;
    });
    if (it != inpSysStaves.end()) {
        endSlot = std::distance(inpSysStaves.begin(), it);
    }
}

void StaffGroupInfo::iterateStaves(MeasCmper measId, Edu eduPosition, std::function<bool(const MusxInstance<others::StaffComposite>&)> iterator) const
{
    MUSX_ASSERT_IF (!startSlot || !endSlot) {
        throw std::logic_error("StaffGroupInfo::iterateStaves called with invalid start or end slot for system staves.");
    }
    for (size_t slot = startSlot.value(); slot <= endSlot.value(); slot++) {
        MUSX_ASSERT_IF (slot >= systemStaves.size()) {
            throw std::logic_error("StaffGroupInfo::iterateStaves encounted invalid slot (" + std::to_string(slot) + ") for system staves.");
        }
        const auto& iUsed = systemStaves[slot];
        if (auto staff = others::StaffComposite::createCurrent(iUsed->getDocument(), iUsed->getRequestedPartId(), iUsed->staffId, measId, eduPosition)) {
            if (!iterator(staff)) {
                break;
            }
        } else {
            MUSX_INTEGRITY_ERROR("StaffGroupInfo::iterateStaves could not find staff " + std::to_string(iUsed->staffId) + " in slot " + std::to_string(slot));
        }
    }
}

std::vector<StaffGroupInfo> StaffGroupInfo::getGroupsAtMeasure(MeasCmper measureId, Cmper linkedPartId,
    const MusxInstanceList<others::StaffUsed>& systemStaves)
{
    if (systemStaves.empty()) {
        util::Logger::log(util::Logger::LogLevel::Info, "Attempted to find groups for empty system staves. [measure " + std::to_string(measureId)
            + ", part " + std::to_string(linkedPartId) +"] Returning an empty vector.");
        return {};
    }
    auto rawGroups = systemStaves.getDocument()->getDetails()->getArray<StaffGroup>(linkedPartId, BASE_SYSTEM_ID);
    std::vector<StaffGroupInfo> retval;
    for (const auto& rawGroup : rawGroups) {
        if (rawGroup->startMeas <= measureId && rawGroup->endMeas >= measureId) {
            StaffGroupInfo group(rawGroup, systemStaves);
            if (group.startSlot && group.endSlot) {
                retval.emplace_back(std::move(group));
            }
        }
    }
    return retval;
}

} // namespace details
} // namespace dom
} // namespace musx