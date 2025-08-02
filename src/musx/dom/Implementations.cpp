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
#include <cstdlib>
#include <exception>
#include <filesystem>
#include <sstream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <cmath>
#include <cwctype>
#include <string_view>
#include <unordered_set>

 // This header includes method implementations that need to see all the classes in the dom

#include "musx/musx.h"

#if ! defined(MUSX_RUNNING_ON_WINDOWS)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace musx {
namespace dom {

// ***************************
// ***** BeamAlterations *****
// ***************************

template <typename T,
          std::enable_if_t<std::is_base_of_v<details::BeamAlterations, T>, int>>
void details::BeamAlterations::calcAllActiveFlags(const DocumentPtr& document)
{
    if (const auto beamOptions = document->getOptions()->get<options::BeamOptions>()) {
        const auto values = document->getDetails()->getArray<T>(SCORE_PARTID);
#ifdef MUSX_DISPLAY_NODE_NAMES
        util::Logger::log(util::Logger::LogLevel::Verbose, std::string(T::XmlNodeName) + " has " + std::to_string(values.size()) + " elements.");
#endif
        for (const auto& value : values) {
            T* mutableValue = const_cast<T*>(value.get());
            mutableValue->m_active = (value->flattenStyle == beamOptions->beamingStyle);
        }
    } else {
        MUSX_INTEGRITY_ERROR("Unable to retrieve beaming options. Active indicators for beam alterations were not set.");
    }
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template void details::BeamAlterations::calcAllActiveFlags<details::BeamAlterationsUpStem>(const DocumentPtr&);
template void details::BeamAlterations::calcAllActiveFlags<details::BeamAlterationsDownStem>(const DocumentPtr&);
template void details::BeamAlterations::calcAllActiveFlags<details::SecondaryBeamAlterationsUpStem>(const DocumentPtr&);
template void details::BeamAlterations::calcAllActiveFlags<details::SecondaryBeamAlterationsDownStem>(const DocumentPtr&);
#endif // DOXYGEN_SHOULD_IGNORE_THIS

Efix details::BeamAlterations::calcEffectiveBeamWidth() const
{
    if (getInci().has_value()) { // secondary beams have incis; primary beams do not
        MusxInstance<BeamAlterations> primary;
        if (dynamic_cast<const SecondaryBeamAlterationsDownStem*>(this)) {
            primary = getDocument()->getDetails()->get<BeamAlterationsDownStem>(getPartId(), getEntryNumber());
        } else {
            primary = getDocument()->getDetails()->get<BeamAlterationsUpStem>(getPartId(), getEntryNumber());
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
bool details::BeamAlterations::calcIsFeatheredBeamImpl(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY)
{
    static_assert(std::is_same_v<SecondaryBeamType, details::SecondaryBeamAlterationsDownStem>
               || std::is_same_v<SecondaryBeamType, details::SecondaryBeamAlterationsUpStem>,
        "SecondaryBeamType must be a secondary beam type.");
    constexpr bool isDownstem = std::is_same_v<SecondaryBeamType, details::SecondaryBeamAlterationsDownStem>;
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

// ***********************
// ***** ClefOptions *****
// ***********************

bool options::ClefOptions::ClefDef::isBlank() const
{
    if (isShape) {
        if (const auto shape = shapeId ? getDocument()->getOthers()->get<others::ShapeDef>(getPartId(), shapeId) : nullptr) {
            return shape->isBlank();
        }
        return true;
    }
    return !clefChar || (clefChar <= 0xffff && std::iswspace(static_cast<wint_t>(clefChar)));
}

options::ClefOptions::ClefInfo options::ClefOptions::ClefDef::calcInfo(const MusxInstance<others::Staff>& currStaff) const
{
    auto calcPercType = [&]() -> music_theory::ClefType {
        if (auto clefFont = calcFont()) {
            if (clefFont->calcIsSMuFL()) {
                if (clefChar == 0xE06A) { // SMuFL `unpitchedPercussionClef2`
                    return music_theory::ClefType::Percussion2;
                }
            } else if (clefFont->calcIsSymbolFont()) {
                if (clefChar == 214) {
                    return music_theory::ClefType::Percussion2;
                }
            }
        }
        return music_theory::ClefType::Percussion1;
    };

    auto calcTabType = [&]() -> music_theory::ClefType {
        music_theory::ClefType result = music_theory::ClefType::Tab;
        if (isShape) {
            if (auto shape = getDocument()->getOthers()->get<others::ShapeDef>(getPartId(), shapeId)) {
                shape->iterateInstructions([&](others::ShapeDef::InstructionType instructionType, std::vector<int> data) -> bool {
                    if (std::optional<FontInfo> fontInfo = others::ShapeInstruction::parseSetFont(getDocument(), instructionType, data)) {
                        if (fontInfo->getName().find("Times") != std::string::npos) { // Finale default file uses "Times" or "Times New Roman"
                            result = music_theory::ClefType::TabSerif;
                        }
                        return false;
                    }
                    return true;
                });
            }
            return result;
        }
        // 0xF40D is "4stringTabClefSerif" and 0xF40B is "6stringTabClefSerif"
        // They are both optional glyphs from the MakeMusic extended glyph set defined in glyphnamesFinale.json.
        if (calcFont()->calcIsSMuFL() && (clefChar == 0xF40D || clefChar == 0xF40B)) {
            result = music_theory::ClefType::TabSerif;
        }
        return result;
    };

    if (currStaff) {
        switch (currStaff->notationStyle) {
            case others::Staff::NotationStyle::Tablature: return std::make_pair(calcTabType(), 0);
            case others::Staff::NotationStyle::Percussion: return std::make_pair(calcPercType(), 0);
            default: break;
        }
    }
    if (!currStaff && staffPosition == 0 && middleCPos == -10 && isShape) { // identify tab staves based on Finale default file settings
        return std::make_pair(calcTabType(), 0);
    }
    music_theory::ClefType clefType = music_theory::ClefType::Unknown;
    int octave = 0;
    auto clefPitch = music_theory::NoteName(music_theory::positiveModulus(staffPosition - middleCPos, music_theory::STANDARD_DIATONIC_STEPS, &octave));
    switch (clefPitch) {
        case music_theory::NoteName::C: clefType = music_theory::ClefType::C; break;
        case music_theory::NoteName::F: clefType = music_theory::ClefType::F; break;
        case music_theory::NoteName::G: clefType = music_theory::ClefType::G; break;
        case music_theory::NoteName::B: clefType = calcPercType(); break; // Finale SMuFL default file settings
        default: break;
    }
    if (clefType == music_theory::ClefType::F) {
        octave++; // The F clef's non-transposing position is below middle C, so compensate.
    }
    return std::make_pair(clefType, octave);
}

MusxInstance<FontInfo> options::ClefOptions::ClefDef::calcFont() const
{
    MusxInstance<FontInfo> result;
    if (useOwnFont && font) {
        result = font;
    } else if (auto fontOptions = getDocument()->getOptions()->get<options::FontOptions>()) {
        result = fontOptions->getFontInfo(options::FontOptions::FontType::Clef);
    }
    if (!result) {
        throw std::invalid_argument("Unable to determine clef font due to missing font definitions.");
    }
    return result;
}

// ********************
// ***** Document *****
// ********************

MusxInstance<others::Page> Document::calculatePageFromMeasure(Cmper partId, MeasCmper measureId) const
{
    MusxInstance<others::Page> result;
    auto pages = getOthers()->getArray<others::Page>(partId);
    for (const auto& page : pages) {
        if (page->firstMeasureId && page->lastMeasureId) {
            if (measureId >= page->firstMeasureId.value() && measureId <= page->lastMeasureId.value()) {
                result = page;
                break;
            }
        }
    }
    if (!result) {
        MUSX_INTEGRITY_ERROR("Unable to find page for measure ID " + std::to_string(measureId));
    }
    return result;
}

MusxInstance<others::StaffSystem> Document::calculateSystemFromMeasure(Cmper partId, MeasCmper measureId) const
{
    MusxInstance<others::StaffSystem> result;
    auto systems = getOthers()->getArray<others::StaffSystem>(partId);
    for (const auto& system : systems) {
        // endMeas is 1 measure past the end of the system
        if (measureId >= system->startMeas && measureId < system->endMeas) {
            result = system;
            break;
        }
    }
    if (!result) {
        MUSX_INTEGRITY_ERROR("Unable to find system for measure ID " + std::to_string(measureId));
    }
    return result;
}

InstrumentMap Document::createInstrumentMap(Cmper forPartId) const
{
    InstrumentMap result;

    const auto scrollView = getOthers()->getArray<others::StaffUsed>(forPartId, BASE_SYSTEM_ID);
    if (scrollView.empty()) {
        return result;
    }
    std::unordered_set<Cmper> multiStaffInstsFound;
    std::unordered_set<StaffCmper> mappedStaves;
    for (const auto& staffItem : scrollView) {
        if (auto rawStaff = getOthers()->get<others::Staff>(forPartId, staffItem->staffId)) { // do not use staffItem->getStaffInstance() because we want no throw here
            if (rawStaff->multiStaffInstId != 0) {
                if (multiStaffInstsFound.find(rawStaff->multiStaffInstId) == multiStaffInstsFound.end()) {
                    if (auto multiStaffInst = getOthers()->get<others::MultiStaffInstrumentGroup>(forPartId, rawStaff->multiStaffInstId)) {
                        if (auto multiStaffGroupId = getOthers()->get<others::MultiStaffGroupId>(forPartId, rawStaff->multiStaffInstId)) {
                            multiStaffInstsFound.emplace(rawStaff->multiStaffInstId);
                            const auto [it, created] = result.emplace(rawStaff->getCmper(), InstrumentInfo());
                            MUSX_ASSERT_IF(!created) {
                                throw std::logic_error("Attempted to insert multi-instrument id " + std::to_string(rawStaff->multiStaffInstId) + " more than once.");
                            }
                            it->second.staffGroupId = multiStaffGroupId->staffGroupId;
                            it->second.multistaffGroupId = rawStaff->multiStaffInstId;
                            std::optional<size_t> topIndex = scrollView.getIndexForStaff(rawStaff->getCmper());
                            MUSX_ASSERT_IF(!topIndex.has_value()) {
                                throw std::logic_error("Unable to find " + std::to_string(rawStaff->getCmper()) + " in scrollView.");
                            }
                            for (StaffCmper staffId : multiStaffInst->staffNums) {
                                std::optional<size_t> staffIndex = scrollView.getIndexForStaff(staffId);
                                MUSX_ASSERT_IF(!staffIndex.has_value()) {
                                    throw std::logic_error("Unable to find staff " + std::to_string(staffId) + " from multistaff instrument group in scrollView.");
                                }
                                it->second.staves.emplace(staffId, staffIndex.value() - topIndex.value());
                                mappedStaves.emplace(staffId);
                            }
                        }
                    }
                }
            }
        }
    }
    auto staffGroups = details::StaffGroupInfo::getGroupsAtMeasure(1, forPartId, scrollView);
    for (const auto& staffGroup : staffGroups) {
        const auto& group = staffGroup.group;
        // for now, only identify piano braces as visual staff groups
        if (group->bracket && group->bracket->style == details::StaffGroup::BracketStyle::PianoBrace) {
            if (const auto topStaff = getOthers()->get<others::Staff>(forPartId, group->startInst)) {
                std::unordered_map<StaffCmper, size_t> candidateStaves;
                size_t sequenceIndex = 0;
                staffGroup.iterateStaves(1, 0, [&](const MusxInstance<others::StaffComposite>& nextStaff) {
                    if (nextStaff->multiStaffInstId == topStaff->multiStaffInstId || mappedStaves.find(nextStaff->getCmper()) == mappedStaves.end()) {
                        if (nextStaff->instUuid == topStaff->instUuid || !nextStaff->hasInstrumentAssigned()) {
                            if (nextStaff->hideNameInScore || nextStaff->getFullName().empty()) {
                                const auto [it, created] = candidateStaves.emplace(nextStaff->getCmper(), sequenceIndex++);
                                MUSX_ASSERT_IF (!created) {
                                    throw std::logic_error("Attempted to insert staff " + std::to_string(nextStaff->getCmper()) + " twice in the same multistaff instrument.");
                                }
                                return true;
                            }
                        }
                    }
                    candidateStaves.clear();
                    return false;
                });
                if (!candidateStaves.empty()) {
                    auto [instIt, created] = result.emplace(topStaff->getCmper(), InstrumentInfo());
                    auto& [top, instInfo] = *instIt;
                    if (created || instInfo.staffGroupId == 0 || group->getCmper2() == instInfo.staffGroupId) {
                        if (instInfo.staffGroupId == 0) {
                            util::Logger::log(util::Logger::LogLevel::Info, "Treating piano brace " + std::to_string(group->getCmper2())
                                + " [" + group->getFullName() + "] on staff " + std::to_string(group->startInst) + " as a multistaff instrument.");
                        }
                        instInfo.staffGroupId = group->getCmper2();
                        for (const auto& [cmper, index] : candidateStaves) {
                            instInfo.staves.insert_or_assign(cmper, index);
                            mappedStaves.emplace(cmper);
                        }
                    }
                }
            }
        }
    }
    for (const auto& staffItem : scrollView) {
        if (mappedStaves.find(staffItem->staffId) == mappedStaves.end()) {
            const auto [it, created] = result.emplace(staffItem->staffId, InstrumentInfo());
            MUSX_ASSERT_IF(!created) {
                throw std::logic_error("Attempted to insert single-instrument id " + std::to_string(staffItem->staffId) + " that was already mapped.");
            }
            it->second.staves.emplace(staffItem->staffId, 0);
        }
    }
    for (const auto& [instId, info] : result) {
        std::unordered_set<size_t> indices;
        for (const auto& [staffId, index] : info.staves) {
            indices.insert(index);
        }
        if (indices.size() != info.staves.size()) {
            MUSX_INTEGRITY_ERROR("Instrument " + std::to_string(instId) + " has duplicate or missing staff indices.");
        } else {
            for (size_t i = 0; i < info.staves.size(); ++i) {
                bool breakOut = false; // avoid compiler warning if MUSX_INTEGRITY_ERROR throws
                if (!indices.count(i)) {
                    breakOut = true;
                    MUSX_INTEGRITY_ERROR("Instrument " + std::to_string(instId) + " is missing index " + std::to_string(i));
                }
                if (breakOut) break;
            }
        }
    }
    return result;
}

const InstrumentInfo& Document::getInstrumentForStaff(StaffCmper staffId) const
{
    const auto& instIt = m_instruments.find(staffId);
    if (instIt != m_instruments.end()) {
        return instIt->second;
    } else {
        for (const auto& [top, info] : m_instruments) {
            if (info.staves.find(staffId) != info.staves.end()) {
                return info;
            }
        }
    }
    assert(false); // flag this as early as possible, because getting here is a program bug.
    throw std::logic_error("Staff " + std::to_string(staffId) + " was not mapped to an instrument.");
}

bool Document::calcHasVaryingSystemStaves(Cmper forPartId) const
{
    auto staffSystems = getOthers()->getArray<others::StaffSystem>(forPartId);
    auto scrollView = getOthers()->getArray<others::StaffUsed>(forPartId, BASE_SYSTEM_ID);
    for (const auto& staffSystem : staffSystems) {
        auto nextSystem = getOthers()->getArray<others::StaffUsed>(forPartId, staffSystem->getCmper());
        if (nextSystem.size() != scrollView.size()) {
            return true;
        }
        for (size_t staffIndex = 0; staffIndex < nextSystem.size(); staffIndex++) {
            if (nextSystem[staffIndex]->staffId != scrollView[staffIndex]->staffId) {
                return true;
            }
        }
    }
    return false;
}

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
    }
    else if (forV2) {
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
    return others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaffId.value_or(getStaff()),
        getMeasure(), eduPosition);
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

bool EntryFrame::TupletInfo::calcIsTremolo() const
{
    MUSX_ASSERT_IF(!tuplet) {
        throw std::logic_error("TupletInfo contains no tuplet.");
    }
    // must have exactly 2 entries
    if (endIndex != startIndex + 1) {
        return false;
    }
    // must be invisible
    if (!tuplet->hidden) {
        if (tuplet->numStyle != details::TupletDef::NumberStyle::Nothing || tuplet->brackStyle != details::TupletDef::BracketStyle::Nothing) {
            return false;
        }
    }
    // must have a ratio that is a positive integer power of 2 (i.e., >= 2)
    util::Fraction ratio = tuplet->calcRatio();
    if (ratio.denominator() != 1 || ratio.numerator() < 2 || ((ratio.numerator() & (ratio.numerator() - 1)) != 0)) {
        return false;
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
    EntryInfoPtr second(frame, endIndex);
    if (first->actualDuration != second->actualDuration) {
        return false;
    }
    if (first->getEntry()->duration != second->getEntry()->duration) {
        return false;
    }
    // entries must be beamed-together neighbors
    return second.isSameEntry(first.getNextInBeamGroup());
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

bool EntryFrame::TupletInfo::calcCreatesBeamContinuationRight() const
{
    if (!calcCreatesSingletonRight()) {
        return false;
    }
    auto frame = getParent();
    int voice = int(voice2) + 1;
    EntryInfoPtr entryInfo = EntryInfoPtr(frame, startIndex);
    auto nextInBeam = entryInfo.getNextInBeamGroup();
    // must be followed by exactly one beam
    if (!nextInBeam) {
        return false;
    }
    // the next item must be the last item
    if (nextInBeam.getNextInVoice(voice)) {
        return false;
    }
    auto nextFrame = frame->getNext();
    if (!nextFrame) {
        return false;
    }
    if (auto nextEntryInfo = nextFrame->getFirstInVoice(voice)) {
        return !nextEntryInfo.calcUnbeamed();
    }
    return false;
}

bool EntryFrame::TupletInfo::calcCreatesBeamContinuationLeft() const
{
    if (!calcCreatesSingletonLeft()) {
        return false;
    }
    auto frame = getParent();
    int voice = int(voice2) + 1;
    EntryInfoPtr entryInfo = EntryInfoPtr(frame, startIndex);
    if (entryInfo.getPreviousInVoice(voice)) {
        return false;
    }
    auto prevFrame = frame->getPrevious();
    if (!prevFrame) {
        return false;
    }
    if (auto prevEntryInfo = prevFrame->getLastInVoice(voice)) {
        return !prevEntryInfo.calcUnbeamed();
    }
    return false;
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
    return m_entryFrame->createCurrentStaff((*this)->elapsedDuration.calcEduDuration(), forStaffId);
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
    if ((*this)->getEntry()->voice2) {
        if (next && next->getEntry()->voice2) {
            return next;
        }
        return EntryInfoPtr();
    }
    if ((*this)->v2Launch) {
        while (next && next->getEntry()->voice2) {
            next = next.getNextInFrame();
        }
    }
    return next;
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

bool EntryInfoPtr::calcDisplaysAsRest() const
{
    return !(*this)->getEntry()->isNote;
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

bool EntryInfoPtr::calcIsBeamStart() const
{
    if ((*this)->getEntry()->isHidden) return false;
    if (!canBeBeamed()) return false;
    return (!getPreviousInBeamGroup() && getNextInBeamGroup());
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
            if (result->getEntry()->beam || !result.canBeBeamed()) {
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
            MUSX_ASSERT_IF(!beamOpts)
            {
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
        if (auto scoreStaff = others::StaffComposite::createCurrent(doc, SCORE_PARTID, getStaff(), getMeasure(), calcGlobalElapsedDuration().calcEduDuration())) {
            bool hidden = (scoreStaff->altNotation == others::Staff::AlternateNotation::BlankWithRests || scoreStaff->altNotation == others::Staff::AlternateNotation::Blank)
                && (scoreStaff->altLayer == getLayerIndex() || scoreStaff->altHideOtherNotes);
            if (!hidden) {
                hidden = scoreStaff->hideMode != others::Staff::HideMode::None;
            }
            if (hidden) {
                auto parts = scoreStaff->getContainingParts(/*includeScore*/false);
                for (const auto& part : parts) {
                    if (auto partStaff = others::StaffComposite::createCurrent(doc, part->getCmper(), getStaff(), getMeasure(), calcGlobalElapsedDuration().calcEduDuration())) {
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
    if (entry->isHidden && (*this)->v2Launch) {
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
            if (auto prev = getPreviousInFrame(); prev && prev->v2Launch) {
                auto prevEntry = prev->getEntry();
                if (!prevEntry->isNote && prevEntry->isHidden && prevEntry->duration == entry->duration) {
                    return true;
                }
            }
        }
    }
    return false;
}

// ***********************
// ***** FontOptions *****
// ***********************

MusxInstance<FontInfo> options::FontOptions::getFontInfo(options::FontOptions::FontType type) const
{
    auto it = fontOptions.find(type);
    if (it == fontOptions.end()) {
        throw std::invalid_argument("Font type " + std::to_string(int(type)) + " not found in document");
    }
    return it->second;
}

MusxInstance<FontInfo> options::FontOptions::getFontInfo(const DocumentPtr& document, options::FontOptions::FontType type)
{
    auto options = document->getOptions();
    if (!options) {
        throw std::invalid_argument("No options found in document");
    }
    auto fontOptions = options->get<options::FontOptions>();
    if (!fontOptions) {
        throw std::invalid_argument("Default fonts not found in document");
    }
    return fontOptions->getFontInfo(type);
}

// ********************
// ***** FontInfo *****
// ********************

std::string FontInfo::getName() const
{
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(getPartId(), fontId)) {
        return fontDef->name;
    }
    throw std::invalid_argument("font definition not found for font id " + std::to_string(fontId));
}

void FontInfo::setFontIdByName(const std::string& name)
{
    auto fontDefs = getDocument()->getOthers()->getArray<others::FontDefinition>(getPartId());
    for (auto fontDef : fontDefs) {
        if (fontDef->name == name) {
            fontId = fontDef->getCmper();
            return;
        }
    }
    throw std::invalid_argument("font definition not found for font \"" + name + "\"");
}

std::optional<std::filesystem::path> FontInfo::calcSMuFLMetaDataPath(const std::string& fontName)
{
    auto standardFontPaths = calcSMuFLPaths();
    for (const auto& path : standardFontPaths) {
        if (!path.empty()) {
            std::filesystem::path metaFilePath(path / fontName / fontName);
            metaFilePath.replace_extension(".json");
            if (std::filesystem::is_regular_file(metaFilePath)) {
                return metaFilePath;
            }
        }
    }
    return std::nullopt;
}

bool FontInfo::calcIsSMuFL() const
{
   static const std::set<std::string_view> knownSmuflFontNames =
    {
        "Bravura",
        "Leland",
        "Emmentaler",
        "Gonville",
        "MuseJazz",
        "Petaluma",
        "Finale Maestro",
        "Finale Broadway"
    };

    if (calcSMuFLMetaDataPath().has_value()) {
        return true;
    }

    auto it = knownSmuflFontNames.find(getName());
    return it != knownSmuflFontNames.end();
}

bool FontInfo::calcIsSymbolFont() const
{
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(getPartId(), fontId)) {
        return fontDef->calcIsSymbolFont();
    }
    throw std::invalid_argument("font definition not found for font id " + std::to_string(fontId));
}

std::vector<std::filesystem::path> FontInfo::calcSMuFLPaths()
{
    if (const auto& testPath = util::TestConfiguration::getTestDataPath()) {
        return { std::filesystem::path(testPath.value()) / "font_metadata" };
    }
#if defined(MUSX_RUNNING_ON_WINDOWS)
    auto systemEnv = "COMMONPROGRAMFILES";
    auto userEnv = "LOCALAPPDATA";
#elif defined(MUSX_RUNNING_ON_MACOS)
    auto systemEnv = "";
    auto userEnv = "HOME";
#elif defined(MUSX_RUNNING_ON_LINUX_UNIX)
    auto systemEnv = "XDG_DATA_DIRS";
    auto userEnv = "XDG_DATA_HOME";
#else
    static_assert(false, "Unsupported OS for FontInfo::calcSMuFLPaths");
#endif

#if ! defined(MUSX_RUNNING_ON_WINDOWS)    
    auto getHomePath = []() -> std::string {
        auto homeEnv = getenv("HOME");
        if (homeEnv) {
            return homeEnv;
        }
        uid_t uid = getuid(); // Get the current user's UID
        struct passwd* pw = getpwuid(uid); // Fetch the password entry for the UID
        if (pw) {
            return pw->pw_dir;
        }
        return "";
        };
#else
    auto getHomePath = []() -> void {};
#endif

    auto getBasePaths = [getHomePath](const std::string& envVariable) -> std::vector<std::string> {
        std::vector<std::string> paths;
#if defined(MUSX_RUNNING_ON_WINDOWS)
        char* buffer = nullptr;
        size_t bufferSize = 0;
        if (_dupenv_s(&buffer, &bufferSize, envVariable.c_str()) == 0 && buffer != nullptr) {
            paths.emplace_back(buffer);
            free(buffer);
        }
        else {
            return {};
        }
#else
        if (envVariable == "HOME") {
            paths.emplace_back(getHomePath());
        }
        else if (!envVariable.empty()) {
            if (auto envValue = getenv(envVariable.c_str())) {
                std::stringstream ss(envValue);
                std::string path;
                while (std::getline(ss, path, ':')) {
                    paths.push_back(path);
                }
#if defined(MUSX_RUNNING_ON_LINUX_UNIX)
            }
            else if (envVariable == "XDG_DATA_HOME") {
                paths.emplace_back(getHomePath() + "/.local/share");
            }
            else if (envVariable == "XDG_DATA_DIRS") {
                paths.emplace_back("/usr/local/share");
                paths.emplace_back("/usr/share");
#endif         
            }
            else {
                return {};
            }
        }
        else {
            paths.emplace_back("/");
        }
#endif
        return paths;
        };
    auto paths = getBasePaths(userEnv);
    auto temp = getBasePaths(systemEnv);
    paths.insert(paths.end(),
        std::make_move_iterator(temp.begin()),
        std::make_move_iterator(temp.end()));
    std::vector<std::filesystem::path> retval;
    for (const auto& next : paths) {
        std::filesystem::path path = next;
#if defined(MUSX_RUNNING_ON_MACOS)
        path = path / "Library" / "Application Support";
#endif
        path = path / "SMuFL" / "Fonts";
        retval.emplace_back(std::move(path));
    }
    return retval;
}

// *****************
// ***** Frame *****
// *****************

void others::Frame::iterateRawEntries(std::function<bool(const MusxInstance<Entry>& entry)> iterator) const
{
    auto firstEntry = startEntry ? getDocument()->getEntries()->get(startEntry) : nullptr;
    if (!firstEntry) {
        MUSX_INTEGRITY_ERROR("Frame " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value_or(-1)) + " is not iterable.");
    }
    for (auto entry = firstEntry; entry; entry = entry->getNext()) {
        if (!iterator(entry) || entry->getEntryNumber() == endEntry) {
            break;
        }
    }
}

MusxInstanceList<Entry> others::Frame::getEntries() const
{
    MusxInstanceList<Entry> retval(getDocument(), getPartId());
    iterateRawEntries([&](const MusxInstance<Entry>& entry) -> bool {
        retval.emplace_back(entry);
        return true;
    });
    return retval;
}
 
// **********************
// ***** CustomStem *****
// **********************

bool details::CustomStem::calcIsHiddenStem() const
{
    if (shapeDef != 0) {
        if (const auto shape = getDocument()->getOthers()->get<others::ShapeDef>(getPartId(), shapeDef)) {
            return shape->isBlank();
        }
    }
    return true;
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
        entryFrame = std::make_shared<EntryFrame>(*this, layerIndex, timeStretch);
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
            if (!entry->voice2 && (i + 1) < entries.size() && entries[i + 1]->voice2) {
                entryInfo->v2Launch = true;
            }
            if (entryInfo->v2Launch) {
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

bool details::GFrameHoldContext::iterateEntries(LayerIndex layerIndex, std::function<bool(const EntryInfoPtr&)> iterator)
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

bool details::GFrameHoldContext::iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator)
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

// ***********************************
// ***** IndependentStaffDetails *****
// ***********************************

MusxInstance<TimeSignature> details::IndependentStaffDetails::createTimeSignature() const
{
   return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, altNumTsig, altDenTsig));
}

MusxInstance<TimeSignature> details::IndependentStaffDetails::createDisplayTimeSignature() const
{
    if (!displayAbbrvTime) {
        return createTimeSignature();
    }
    return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivBeat, displayAltNumTsig, displayAltDenTsig, displayAbbrvTime));
}

// **************************
// ***** InstrumentInfo *****
// **************************

std::vector<StaffCmper> InstrumentInfo::getSequentialStaves() const
{
    std::vector<std::pair<StaffCmper, size_t>> sorted(staves.begin(), staves.end());
    std::sort(sorted.begin(), sorted.end(),
              [](const auto& a, const auto& b) { return a.second < b.second; });

    std::vector<StaffCmper> result;
    result.reserve(sorted.size());
    for (const auto& [staffId, _] : sorted) {
        result.push_back(staffId);
    }
    return result;
}

// *********************
// ***** StaffUsed *****
// *********************

util::Fraction others::StaffUsed::calcEffectiveScaling() const
{
    util::Fraction result(1);
    if (SystemCmper(getCmper()) > 0) { // if this is a page-view system
        if (auto system = getDocument()->getOthers()->get<others::StaffSystem>(getPartId(), getCmper())) {
            result = system->calcSystemScaling();
            if (auto page = system->getPage()) {
                result *= page->calcPageScaling();
            }
            if (system->hasStaffScaling) {
                if (auto staffSize = getDocument()->getDetails()->get<details::StaffSize>(getPartId(), getCmper(), staffId)) {
                    result *= staffSize->calcStaffScaling();
                }
            }
        }
    }
    return result;
}

MusxInstance<others::Staff> others::StaffUsed::getStaffInstance() const
{
    auto retval = getDocument()->getOthers()->get<others::Staff>(getPartId(), staffId);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffId) + " not found for StaffUsed list " + std::to_string(getCmper()));
    }
    return retval;
}

MusxInstance<others::Staff> others::StaffUsed::getStaffInstance(MeasCmper measureId, Edu eduPosition) const
{
    auto retval = others::StaffComposite::createCurrent(getDocument(), getPartId(), staffId, measureId, eduPosition);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Composite staff " + std::to_string(staffId) + " not found for StaffUsed list " + std::to_string(getCmper())
            + " at measure " + std::to_string(measureId) + " eduPosition " + std::to_string(eduPosition));
    }
    return retval;
}

// ************************
// ***** KeySignature *****
// ************************

std::vector<unsigned> KeySignature::calcTonalCenterArrayForSharps() const
{
    if (isMinor()) {
        return { 5, 2, 6, 3, 0, 4, 1, 5 };
    }
    if (!isBuiltIn()) {
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterSharps>(getPartId(), getKeyMode())) {
            return centers->values;
        }
    }
    // Major or default
    return { 0, 4, 1, 5, 2, 6, 3, 0 };
}

std::vector<unsigned> KeySignature::calcTonalCenterArrayForFlats() const
{
    if (isMinor()) {
        return { 5, 1, 4, 0, 3, 6, 2, 5 };
    }
    if (!isBuiltIn()) {
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterFlats>(getPartId(), getKeyMode())) {
            return centers->values;
        }
    }
    // Major or default
    return { 0, 3, 6, 2, 5, 1, 4, 0 };
}

std::vector<unsigned> KeySignature::calcTonalCenterArray(KeyContext ctx) const
{
    int alter = getAlteration(ctx);
    if (alter >= 0) {
        return calcTonalCenterArrayForSharps();
    } else {
        return calcTonalCenterArrayForFlats();
    }
}

std::vector<int> KeySignature::calcAcciAmountsArray(KeyContext ctx) const
{
    int alter = getAlteration(ctx);

    if (!isBuiltIn()) {
        if (alter >= 0) {
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountSharps>(getPartId(), getKeyMode())) {
                return amounts->values;
            }
        } else {
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountFlats>(getPartId(), getKeyMode())) {
                return amounts->values;
            }
        }
    }

    // Major/minor or default
    if (alter >= 0) {
        return std::vector<int>(music_theory::STANDARD_DIATONIC_STEPS, 1);
    } else {
        return std::vector<int>(music_theory::STANDARD_DIATONIC_STEPS, -1);
    }
}

std::vector<unsigned> KeySignature::calcAcciOrderArray(KeyContext ctx) const
{
    int alter = getAlteration(ctx);

    if (!isBuiltIn()) {
        if (alter >= 0) {
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderSharps>(getPartId(), getKeyMode())) {
                return order->values;
            }
        } else {
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderFlats>(getPartId(), getKeyMode())) {
                return order->values;
            }
        }
    }

    // Major/minor or default
    if (alter >= 0) {
        return { 3, 0, 4, 1, 5, 2, 6 };
    } else {
        return { 6, 2, 5, 1, 4, 0, 3 };
    }
}

int KeySignature::calcTonalCenterIndex(KeyContext ctx) const
{
    if (!isNonLinear() && !isLinear()) {
        MUSX_INTEGRITY_ERROR("Key signature mode " + std::to_string(getKeyMode()) + " is neither linear nor non-linear. It is invalid.");
    }

    int alter = getAlteration(ctx);
    auto centers = calcTonalCenterArray(ctx);
    return int(centers[std::abs(alter) % centers.size()]);
}

int KeySignature::calcAlterationOnNote(unsigned noteIndex, KeyContext ctx) const
{
    if (!isNonLinear() && !isLinear()) {
        MUSX_INTEGRITY_ERROR("Key signature mode " + std::to_string(getKeyMode()) + " is neither linear nor non-linear. It is invalid.");
    }

    auto amounts = calcAcciAmountsArray(ctx);
    auto order = calcAcciOrderArray(ctx);

    int keySigAlteration = 0;

    if (isNonLinear()) {
        for (size_t i = 0; i < amounts.size() && i < order.size(); i++) {
            if (amounts[i] == 0) {
                break;
            }
            if (noteIndex == order[i]) {
                keySigAlteration += amounts[i];
            }
        }
    } else {
        unsigned keyFifths = std::abs(getAlteration(ctx));
        for (size_t i = 0; i < keyFifths && i < amounts.size() && i < order.size(); ++i) {
            if (noteIndex == order[i % order.size()]) {
                keySigAlteration += amounts[i];
            }
        }
    }

    return keySigAlteration;
}

void KeySignature::setTransposition(int interval, int keyAdjustment, bool simplify)
{
    if (!isLinear()) {
        return;
    }
    m_octaveDisplacement = interval / music_theory::STANDARD_DIATONIC_STEPS;
    m_alterationOffset = 0; // suppresses transposed tone center and alteration calc
    int concertAlteration = getAlteration(KeyContext::Concert);
    int concertTonalCenterIndex = calcTonalCenterIndex(KeyContext::Concert);
    int tonalCenterOffset = interval % music_theory::STANDARD_DIATONIC_STEPS;
    
    int alteration = concertAlteration + keyAdjustment;
    if (simplify && keyAdjustment) {
        // Finale does not simplify microtonal key sigs correctly.
        // This simplification *does* work correctly with microtonal key sigs, provided the custom key sig
        // is set up to increment each key signature accidental by the number of EDO divisions
        // in a chromatic half-step.
        int direction = music_theory::sign(alteration);
        while (std::abs(alteration) >= music_theory::STANDARD_DIATONIC_STEPS) {
            alteration -= direction * music_theory::STANDARD_12EDO_STEPS;
            tonalCenterOffset += direction;
        }
    }
    m_alterationOffset = alteration - concertAlteration;
    m_octaveDisplacement += (concertTonalCenterIndex + tonalCenterOffset) / music_theory::STANDARD_DIATONIC_STEPS;
}

void KeySignature::setTransposition(const MusxInstance<others::Staff>& staff)
{
    if (staff && staff->transposition && staff->transposition->keysig) {
        const auto& keysig = *staff->transposition->keysig;
        setTransposition(keysig.interval, keysig.adjust, !staff->transposition->noSimplifyKey);
    }
}

std::optional<std::vector<int>> KeySignature::calcKeyMap() const
{
    size_t tonalCenter = static_cast<size_t>(calcTonalCenterArrayForSharps()[0]);
    auto keyMap = getDocument()->getOthers()->get<others::KeyMapArray>(getPartId(), getKeyMode());
    if (!keyMap || keyMap->steps.empty()) {
        return std::nullopt;
    }
    unsigned numDiatonicSteps = keyMap->countDiatonicSteps();

    // Find the index of the first step whose diatonic step matches the tonal center
    auto centerIt = std::find_if(keyMap->steps.begin(), keyMap->steps.end(), [&](const auto& s) {
        return s->diatonic && s->hlevel == tonalCenter;
    });
    if (centerIt == keyMap->steps.end()) {
        return std::nullopt;
    }
    size_t indexOfTonalCenter = std::distance(keyMap->steps.begin(), centerIt);

    // Initialize result
    std::vector<int> result(numDiatonicSteps, 0);
    int currDiatonicStep = -1;
    const size_t stepCount = keyMap->steps.size();
    for (size_t i = 0; i < stepCount; ++i) {
        size_t wrappedIndex = (indexOfTonalCenter + i) % stepCount;
        const auto& step = keyMap->steps[wrappedIndex];
        if (step->diatonic) {
            ++currDiatonicStep;
        }
        for (size_t diatonicStep = 1; diatonicStep < numDiatonicSteps; ++diatonicStep) {
            if (int(diatonicStep) > currDiatonicStep) {
                ++result[diatonicStep];
            }
        }
    }

    return result;
}

int KeySignature::calcEDODivisions() const
{
    if (auto keyFormat = getDocument()->getOthers()->get<others::KeyFormat>(getPartId(), getKeyMode())) {
        return static_cast<int>(keyFormat->semitones);
    }
    return music_theory::STANDARD_12EDO_STEPS;
}

std::unique_ptr<music_theory::Transposer> KeySignature::createTransposer(int displacement, int alteration) const
{
    return std::make_unique<music_theory::Transposer>(displacement, alteration, isMinor(), calcEDODivisions(), calcKeyMap());
}

std::optional<music_theory::DiatonicMode> KeySignature::calcDiatonicMode() const
{
    if (isLinear()) {
        const auto centers = calcTonalCenterArrayForSharps();
        const unsigned index = centers[0];
        if (index < music_theory::STANDARD_DIATONIC_STEPS) {
            return static_cast<music_theory::DiatonicMode>(index);
        } else {
            MUSX_INTEGRITY_ERROR("KeyMode " + std::to_string(getKeyMode()) + " returned invalid tonal center at index 0: " + std::to_string(index));
        }
    }
    return std::nullopt;
}

// ************************
// ***** LyricsAssign *****
// ************************

template <typename TextType>
util::EnigmaParsingContext details::LyricAssign::getRawTextCtx() const
{
    static_assert(std::is_base_of_v<texts::LyricsTextBase, TextType>, "TextType must be a subclass of texts::LyricsTextBase.");
    // note that lyrics do not have text inserts. The UI doesn't permit them.
    if (auto rawText = getDocument()->getTexts()->get<TextType>(lyricNumber)) {
        return rawText->getRawTextCtx(SCORE_PARTID);
    }
    return {};
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template util::EnigmaParsingContext details::LyricAssign::getRawTextCtx<texts::LyricsVerse>() const;
template util::EnigmaParsingContext details::LyricAssign::getRawTextCtx<texts::LyricsChorus>() const;
template util::EnigmaParsingContext details::LyricAssign::getRawTextCtx<texts::LyricsSection>() const;
#endif // DOXYGEN_SHOULD_IGNORE_THIS

// **************************
// ***** LyricsTextBase *****
// **************************

void texts::LyricsTextBase::createSyllableInfo()
{
    std::string current;
    bool inSeparator = false;
    bool currSeparatorHasHyphen = false;
    bool lastSeparatorHadHyphen = false;

    auto plainText = util::EnigmaString::trimTags(text);
    syllables.clear();
    for (auto c : plainText) {
        if (c == '-' || isspace(static_cast<unsigned char>(c))) {
            if (c == '-') {
                currSeparatorHasHyphen = true;
            }
            inSeparator = true;
        } else {
            if (inSeparator) {
                if (!current.empty()) {
                    syllables.push_back(std::shared_ptr<const LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen)));
                    current.clear();
                }
                lastSeparatorHadHyphen = currSeparatorHasHyphen;
                currSeparatorHasHyphen = false;
                inSeparator = false;
            }
            current += c;
        }
    }

    if (!current.empty()) {
        syllables.push_back(std::shared_ptr<const LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen)));
    }
}

// ****************************
// ***** MarkingCategiory *****
// ****************************

std::string others::MarkingCategory::getName() const
{
    auto catName = getDocument()->getOthers()->get<others::MarkingCategoryName>(getPartId(), getCmper());
    if (catName) {
        return catName->name;
    }
    return {};
}

// *******************
// ***** Measure *****
// *******************

int others::Measure::calcDisplayNumber() const
{
    if (noMeasNum) {
        return getCmper();
    }
    if (const auto region = others::MeasureNumberRegion::findMeasure(getDocument(), getCmper())) {
        return region->calcDisplayNumberFor(getCmper());
    }
    return getCmper();
}

MusxInstance<KeySignature> others::Measure::createKeySignature(const std::optional<StaffCmper>& forStaff) const
{
    std::shared_ptr<KeySignature> result;
    MusxInstance<others::Staff> staff;
    if (forStaff) {
        staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0);
        if (staff && staff->floatKeys) {
            if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getPartId(), forStaff.value(), getCmper())) {
                if (floats->hasKey) {
                    result = std::make_shared<KeySignature>(*floats->keySig);
                }
            }
        }
    }
    if (!result) {
        result = std::make_shared<KeySignature>(*globalKeySig);
    }
    if (result && staff) {
        result->setTransposition(staff);
    }
    return result;
}

MusxInstance<TimeSignature> others::Measure::createTimeSignature(const std::optional<StaffCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatTime) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getPartId(), forStaff.value(), getCmper())) {
                    if (floats->hasTime) {
                        return floats->createTimeSignature();
                    }
                }
            }
        }
    }
   return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, compositeNumerator, compositeDenominator));
}

MusxInstance<TimeSignature> others::Measure::createDisplayTimeSignature(const std::optional<StaffCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatTime) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getPartId(), forStaff.value(), getCmper())) {
                    if (floats->hasTime) {
                        return floats->createDisplayTimeSignature();
                    }
                }
            }
        }
    }
    if (!useDisplayTimesig) {
        return createTimeSignature(forStaff);
    }
    return MusxInstance<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivbeat, compositeDispNumerator, compositeDispDenominator, abbrvTime));
}

util::Fraction others::Measure::calcDuration(const std::optional<StaffCmper>& forStaff) const
{
    auto timeSig = createTimeSignature(forStaff);
    return timeSig->calcTotalDuration();
}

// *****************************
// ***** MeasureExprAssign *****
// *****************************

MusxInstance<others::TextExpressionDef> others::MeasureExprAssign::getTextExpression() const
{
    if (!textExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<others::TextExpressionDef>(getPartId(), textExprId);
}

MusxInstance<others::ShapeExpressionDef> others::MeasureExprAssign::getShapeExpression() const
{
    if (!shapeExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<others::ShapeExpressionDef>(getPartId(), shapeExprId);
}

// *******************************
// ***** MeasureNumberRegion *****
// *******************************

MusxInstance<others::MeasureNumberRegion> others::MeasureNumberRegion::findMeasure(const DocumentPtr& document, MeasCmper measureId)
{
    auto regions = document->getOthers()->getArray<others::MeasureNumberRegion>(SCORE_PARTID);
    for (const auto& region : regions) {
        if (region->calcIncludesMeasure(measureId)) {
            return region;
        }
    }
    return nullptr;
}

int others::MeasureNumberRegion::calcDisplayNumberFor(MeasCmper measureId) const
{
    if (!calcIncludesMeasure(measureId)) {
        throw std::logic_error("Measure id " + std::to_string(measureId) + " is not contained in measure number region " + std::to_string(getCmper()));
    }
    int result = int(measureId) - int(startMeas) + getStartNumber();
    for (MeasCmper next = startMeas; next <= measureId; next++) {
        if (auto measure = getDocument()->getOthers()->get<others::Measure>(getPartId(), next)) {
            if (measure->noMeasNum) {
                if (measure->getCmper() == measureId) {
                    return measureId;
                }
                result--;
            }
        }
    }
    return result;
}

// *****************************
// ***** MeasureTextAssign *****
// *****************************

MusxInstance<others::TextBlock> details::MeasureTextAssign::getTextBlock() const
{
    return getDocument()->getOthers()->get<others::TextBlock>(getPartId(), block);
}

util::EnigmaParsingContext details::MeasureTextAssign::getRawTextCtx(Cmper forPartId) const
{
    if (auto textBlock = getTextBlock()) {
        if (const auto page = getDocument()->calculatePageFromMeasure(forPartId, getCmper2())) {
            return textBlock->getRawTextCtx(forPartId, page->getCmper());
        }
    }
    return {};
}

// *************************************
// ***** MultiStaffInstrumentGroup *****
// *************************************

MusxInstance<others::Staff> others::MultiStaffInstrumentGroup::getStaffInstanceAtIndex(size_t x) const
{
    if (x >= staffNums.size()) return nullptr;
    auto retval = getDocument()->getOthers()->get<others::Staff>(getPartId(), staffNums[x]);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffNums[x])
            + " not found for multiple staff instrument " + std::to_string(getCmper()));
    }
    return retval;
}

MusxInstance<others::Staff> others::MultiStaffInstrumentGroup::getFirstStaffInstance() const
{
    if (staffNums.empty()) {
        MUSX_INTEGRITY_ERROR("MultiStaffInstrumentGroup " + std::to_string(getCmper()) + " contains no staves.");
        return nullptr;
    }
    return getStaffInstanceAtIndex(0);
}

MusxInstance<details::StaffGroup> others::MultiStaffInstrumentGroup::getStaffGroup(Cmper forPartId) const
{
    auto document = getDocument();
    auto groupIdRecord = document->getOthers()->get<others::MultiStaffGroupId>(forPartId, getCmper());
    if (!groupIdRecord || groupIdRecord->staffGroupId == 0) {
        // staffGroupId can be 0 in upgraded files where there is another StaffGroup showing the instrument name.
        return nullptr;
    }
    auto result = document->getDetails()->get<details::StaffGroup>(forPartId, BASE_SYSTEM_ID, groupIdRecord->staffGroupId);
    if (!result) {
        MUSX_INTEGRITY_ERROR("StaffGroup " + std::to_string(groupIdRecord->staffGroupId)
            + " not found for MultiStaffInstrumentGroup " + std::to_string(getCmper()));
    }
    return result;
}

void others::MultiStaffInstrumentGroup::calcAllMultiStaffGroupIds(const DocumentPtr& document)
{
    auto instGroups = document->getOthers()->getArray<others::MultiStaffInstrumentGroup>(SCORE_PARTID);
    for (const auto& instance : instGroups) {
        for (size_t x = 0; x < instance->staffNums.size(); x++) {
            auto staff = instance->getStaffInstanceAtIndex(x);
            if (staff) {
                if (staff->multiStaffInstId != instance->getCmper()) {
                    if (staff->multiStaffInstId) {
                        musx::util::Logger::log(musx::util::Logger::LogLevel::Verbose,
                            "Staff " + std::to_string(staff->getCmper()) + " appears in more than one instance of MultiStaffInstrumentGroup.");
                    } else {
                        others::Staff* staffMutable = const_cast<others::Staff*>(staff.get());
                        staffMutable->multiStaffInstId = instance->getCmper();
                    }
                }
            }
        }
    }
    // guarantee calcAllAutoNumberValues is called after calcAllMultiStaffGroupIds.
    others::Staff::calcAllAutoNumberValues(document);
}

// **********************
// ***** MusicRange *****
// **********************

std::optional<std::pair<MeasCmper, Edu>> others::MusicRange::nextLocation(const std::optional<StaffCmper>& forStaff) const
{
    std::optional<std::pair<MeasCmper, Edu>> result;
    if (auto currMeasure = getDocument()->getOthers()->get<others::Measure>(getPartId(), endMeas)) {
        MeasCmper nextMeas = endMeas;
        Edu maxEdu = currMeasure->calcDuration(forStaff).calcEduDuration() - 1;
        Edu nextEdu = 0;
        if (endEdu < maxEdu) {
            nextEdu = endEdu + 1;
        } else {
            nextMeas++;
            if (!getDocument()->getOthers()->get<others::Measure>(getPartId(), nextMeas)) {
                return std::nullopt;
            }
        }
        result = std::make_pair(nextMeas, nextEdu);
    } else {
        MUSX_INTEGRITY_ERROR("MusicRange has invalid end measure " + std::to_string(endMeas));
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
    const MusxInstance<others::Staff>& staff, bool respellEnharmonic) const
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
    int actualAlteration = transposedAlt + key->calcAlterationOnNote(step, ctx);

    // Calculate the staff line
    const auto& clefOptions = getDocument()->getOptions()->get<options::ClefOptions>();
    if (!clefOptions) {
        throw std::invalid_argument("Document contains no clef options!");
    }
    int middleCLine = clefOptions->getClefDef(clefIndex)->middleCPos;

    return { music_theory::noteNames[step], octave, actualAlteration, keyAdjustedLev + middleCLine };
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
            if (nextEntry->v2Launch) {
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
            if (nextEntry->v2Launch) {
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
        if (auto crossStaff = m_entry->getEntry()->getDocument()->getDetails()->getForNote<details::CrossStaff>(*this)) {
            return crossStaff->staff;
        }
    }
    return m_entry.getStaff();
}

Note::NoteProperties NoteInfoPtr::calcNoteProperties(const std::optional<bool>& enharmonicRespell) const
{
    StaffCmper staffId = calcStaff();
    const ClefIndex clefIndex = [&]() {
        if (staffId != m_entry.getStaff()) {
            if (auto staff = m_entry.createCurrentStaff(staffId)) {
                return staff->calcClefIndexAt(m_entry.getMeasure(), m_entry->elapsedDuration.calcEduDuration(), /*forWrittenPitch*/ true);
            }
        }
        return m_entry->clefIndex;
    }();
    return (*this)->calcNoteProperties(m_entry.getKeySignature(), KeySignature::KeyContext::Written, clefIndex, m_entry.createCurrentStaff(staffId),
            enharmonicRespell.value_or(calcIsEnharmonicRespell()));
}

Note::NoteProperties NoteInfoPtr::calcNotePropertiesConcert() const
{
    const ClefIndex clefIndex = [&]() {
        StaffCmper staffId = calcStaff();
        if (staffId != m_entry.getStaff()) {
            if (auto staff = m_entry.createCurrentStaff(staffId)) {
                return staff->calcClefIndexAt(m_entry.getMeasure(), m_entry->elapsedDuration.calcEduDuration(), /*forWrittenPitch*/ false);
            }
        }
        return m_entry->clefIndexConcert;
    }();
    return (*this)->calcNoteProperties(m_entry.getKeySignature(), KeySignature::KeyContext::Concert, clefIndex, nullptr, calcIsEnharmonicRespell());
}

Note::NoteProperties NoteInfoPtr::calcNotePropertiesInView() const
{
    bool forWrittenPitch = false;
    auto entryFrame = m_entry.getFrame();
    if (auto partGlobals = entryFrame->getDocument()->getOthers()->get<others::PartGlobals>(entryFrame->getRequestedPartId(), MUSX_GLOBALS_CMPER)) {
        forWrittenPitch = partGlobals->showTransposed;
    }
    return forWrittenPitch ? calcNoteProperties() : calcNotePropertiesConcert();
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

// ***********************************************
// ***** MusxInstanceList<others::StaffUsed> *****
// ***********************************************

MusxInstance<others::Staff> MusxInstanceList<others::StaffUsed>::getStaffInstanceAtIndex(Cmper index) const
{
    const auto& iuArray = *this;
    if (index >= iuArray.size()) return nullptr;
    auto iuItem = iuArray[index];
    return iuItem->getStaffInstance();
}

std::optional<size_t> MusxInstanceList<others::StaffUsed>::getIndexForStaff(StaffCmper staffId) const
{
    const auto& iuArray = *this;
    for (size_t x = 0; x < iuArray.size(); x++) {
        if (iuArray[x]->staffId == staffId) {
            return x;
        }
    }
    return std::nullopt;
}

// ****************
// ***** Page *****
// ****************

void others::Page::calcSystemInfo(const DocumentPtr& document)
{
    auto linkedParts = document->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
    for (const auto& part : linkedParts) {
        auto pages = document->getOthers()->getArray<Page>(part->getCmper());
        auto systems = document->getOthers()->getArray<StaffSystem>(part->getCmper());
        for (const auto& system : systems) {
            StaffSystem* mutableSystem = const_cast<StaffSystem*>(system.get());
            mutableSystem->pageId = 0; // initialize
        }
        for (size_t x = 0; x < pages.size(); x++) {
            auto page = pages[x];
            Page* mutablePage = const_cast<Page*>(page.get());
            mutablePage->lastSystemId = std::nullopt;
            if (!page->isBlank()) {
                if (page->firstSystemId > 0) {
                    mutablePage->lastSystemId = [&]() -> SystemCmper {
                        size_t nextIndex = x + 1;
                        while (nextIndex < pages.size()) {
                            auto nextPage = pages[nextIndex++];
                            if (!nextPage->isBlank()) {
                                if (nextPage->firstSystemId > 0) {
                                    return nextPage->firstSystemId - 1;
                                } else {
                                    return 0;
                                }                            
                            }
                        }
                        return SystemCmper(systems.size());
                    }();
                    if (page->lastSystemId.value() >= page->firstSystemId) {
                        if (auto sys = document->getOthers()->get<others::StaffSystem>(part->getCmper(), page->firstSystemId)) {
                            mutablePage->firstMeasureId = sys->startMeas;
                        } else {
                            MUSX_INTEGRITY_ERROR("Page " + std::to_string(page->getCmper()) + " of part " + part->getName()
                                + " has a no system instance for its first system.");                        
                        }
                        if (auto sys = document->getOthers()->get<others::StaffSystem>(part->getCmper(), page->lastSystemId.value())) {
                            mutablePage->lastMeasureId = sys->getLastMeasure();
                        } else {
                            MUSX_INTEGRITY_ERROR("Page " + std::to_string(page->getCmper()) + " of part " + part->getName()
                                + " has a no system instance for its last system.");                        
                        }
                        for (size_t x = size_t(page->firstSystemId - 1); x < size_t(page->lastSystemId.value()); x++) {
                            StaffSystem* mutableSystem = const_cast<StaffSystem*>(systems[x].get());
                            mutableSystem->pageId = PageCmper(page->getCmper());
                        }
                    } else {
                        MUSX_INTEGRITY_ERROR("The systems on page " + std::to_string(page->getCmper()) + " of part " + part->getName()
                            + " cannot be determined.");
                    }
                } else {
                    MUSX_INTEGRITY_ERROR("Layout for page " + std::to_string(page->getCmper())
                        + " of part " + std::to_string(part->getCmper()) + " is in an unknown state.");
                }
            }
        }
    }
}

// *****************************
// ***** PageFormatOptions *****
// *****************************

std::shared_ptr<const options::PageFormatOptions::PageFormat> options::PageFormatOptions::calcPageFormatForPart(Cmper partId) const
{
    const auto& baseOptions = (partId == SCORE_PARTID) ? pageFormatScore : pageFormatParts;
    auto retval = std::make_shared<options::PageFormatOptions::PageFormat>(*baseOptions);
    auto pages = getDocument()->getOthers()->getArray<others::Page>(partId);
    auto page1 = pages.size() >= 1 ? pages[0] : nullptr;
    auto page2 = pages.size() >= 2 ? pages[1] : page1; // left page
    auto page3 = pages.size() >= 3 ? pages[2] : page1; // right page that isn't page 1
    if (page2) {
        retval->pageHeight = page2->height;
        retval->pageWidth = page2->width;
        retval->pagePercent = page2->percent;
        retval->leftPageMarginTop = page2->margTop;
        retval->leftPageMarginLeft = page2->margLeft;
        retval->leftPageMarginBottom = page2->margBottom;
        retval->leftPageMarginRight = page2->margRight;
    }
    if (page1) {
        if (retval->differentFirstPageMargin || page1->margTop != page2->margTop) {
            retval->firstPageMarginTop = page1->margTop;
            retval->differentFirstPageMargin = true;
        }
    }
    if (page3) {
        if (retval->facingPages || page3->margTop != page2->margTop || page3->margLeft != page2->margLeft
            || page3->margBottom != page2->margBottom || page3->margRight != page2->margRight) {
            retval->facingPages = true;
            retval->rightPageMarginTop = page3->margTop;
            retval->rightPageMarginLeft = page3->margLeft;
            retval->rightPageMarginBottom = page3->margBottom;
            retval->rightPageMarginRight = page3->margRight;
        }
    }
    auto systems = getDocument()->getOthers()->getArray<others::StaffSystem>(partId);
    auto system1 = systems.size() >= 1 ? systems[0] : nullptr;
    auto system2 = systems.size() >= 2 ? systems[1] : system1;
    if (system2) {
        retval->sysPercent = system2->ssysPercent;
        retval->rawStaffHeight = system2->staffHeight >> 2; // divide by 4 to convert Efix (1/64 Evpu) to Evpu16ths
        retval->sysMarginTop = system2->top;
        retval->sysMarginLeft = system2->left;
        retval->sysMarginBottom = system2->bottom;
        retval->sysMarginRight = system2->right;
        // do not copy system2->distanceToPrev because it varies from the default quite often
    }
    if (system1) {
        if (retval->differentFirstSysMargin || system1->top != system2->top || system1->left != system2->left) {
            retval->differentFirstSysMargin = true;
            retval->firstSysMarginTop = system1->top;
            retval->firstSysMarginLeft = system1->left;
            // do not change retval->firstSysMarginDistance because it varies so much depending on context
        }
    }
    return retval;
}

// **************************
// ***** PageTextAssign *****
// **************************

MusxInstance<others::TextBlock> others::PageTextAssign::getTextBlock() const
{
    return getDocument()->getOthers()->get<others::TextBlock>(getPartId(), block);
}

util::EnigmaParsingContext others::PageTextAssign::getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId) const
{
    if (auto textBlock = getTextBlock()) {
        if (getCmper() > 0) {
            forPageId = calcStartPageNumber(forPartId);
        }
        return textBlock->getRawTextCtx(forPartId, forPageId);
    }
    return {};
}

MusxInstance<others::PageTextAssign> others::PageTextAssign::getForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId, Inci inci)
{
    if (auto part = document->getOthers()->get<others::PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->get<others::PageTextAssign>(partId, pageAssignmentId, inci);
    }
    return nullptr;
}

std::vector<MusxInstance<others::PageTextAssign>> others::PageTextAssign::getArrayForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId)
{
    if (auto part = document->getOthers()->get<others::PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->getArray<others::PageTextAssign>(partId, pageAssignmentId);
    }
    return {};
}

std::optional<PageCmper> others::PageTextAssign::calcStartPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<others::PartDefinition>(SCORE_PARTID, forPartId)) {
        if (auto calcValue = part->calcPageNumberFromAssignmentId(getCmper() ? getCmper() : startPage)) {
            if (calcValue.value() <= part->numberOfPages) {
                return calcValue;
            }
        } else if (isMultiPage()) {
            if (auto endPageNum = calcEndPageNumber(forPartId)) {
                if (part->numberOfLeadingBlankPages < endPageNum.value()) {
                    return PageCmper(part->numberOfLeadingBlankPages) + 1;
                }
            }
        }
    }
    return std::nullopt;
}

std::optional<PageCmper> others::PageTextAssign::calcEndPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<others::PartDefinition>(SCORE_PARTID, forPartId)) {
        if (isMultiAssignedThruLastPage()) {
            return PageCmper(part->numberOfPages);
        }
        if (auto calcValue = part->calcPageNumberFromAssignmentId(getCmper() ? getCmper() : endPage)) {
            if (calcValue.value() > part->numberOfPages) {
                calcValue = PageCmper(part->numberOfPages);
            }
            return calcValue;
        }
    }
    return std::nullopt;
}

// **************************
// ***** PartDefinition *****
// **************************

util::EnigmaParsingContext others::PartDefinition::getNameRawTextCtx() const
{
    /// @todo perhaps additional logic as in getName, but not until something is broken.
    if (nameId) {
        if (auto textBlock = getDocument()->getOthers()->get<others::TextBlock>(getPartId(), nameId)) {
            return textBlock->getRawTextCtx(getCmper());
        }
    }
    return {};
}

std::string others::PartDefinition::getName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto nameRawText = getNameRawTextCtx()) {
        // Although the Finale U.I. prevents ^partname inserts in partname enigma strings, one might have crept in.
        std::unordered_set<std::string_view> ignoreTags = { "partname" }; // do not parse ^partname inserts
        return nameRawText.getText(true, accidentalStyle, ignoreTags);
    }
    if (defaultNameStaff) {
        if (auto staff = getDocument()->getOthers()->get<others::Staff>(SCORE_PARTID, defaultNameStaff)) {
            return staff->getFullInstrumentName(accidentalStyle, true); // true: prefer staff name
        } else {
            MUSX_INTEGRITY_ERROR("Part " + std::to_string(getCmper()) + " uses nonexistent Staff " + std::to_string(defaultNameStaff) + " for part name.");
        }
    }
    if (defaultNameGroup) {
        if (auto group = getDocument()->getDetails()->get<details::StaffGroup>(SCORE_PARTID, BASE_SYSTEM_ID, defaultNameGroup)) {
            return group->getFullInstrumentName(accidentalStyle);
        } else {
            MUSX_INTEGRITY_ERROR("Part " + std::to_string(getCmper()) + " uses nonexistent StaffGroup " + std::to_string(defaultNameGroup) + " for part name.");
        }
    }
    return {};
}

Cmper others::PartDefinition::calcSystemIuList(Cmper systemId) const
{
    if (auto partGlobs = getDocument()->getOthers()->get<others::PartGlobals>(getCmper(), MUSX_GLOBALS_CMPER)) {
        if (partGlobs->specialPartExtractionIUList) {
            return partGlobs->specialPartExtractionIUList;
        }
    }
    return systemId;
}

MusxInstance<others::PartDefinition> others::PartDefinition::getScore(const DocumentPtr& document)
{
    if (auto score = document->getOthers()->get<others::PartDefinition>(SCORE_PARTID, SCORE_PARTID)) {
        return score;
    }
    MUSX_INTEGRITY_ERROR("The document contains no instance of PartDefinition for the score.");
    return nullptr;
}

MusxInstanceList<others::PartDefinition> others::PartDefinition::getInUserOrder(const DocumentPtr& document)
{
    auto result = document->getOthers()->getArray<others::PartDefinition>(SCORE_PARTID);
    std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->partOrder < rhs->partOrder;
    });
    return result;
}

std::optional<PageCmper> others::PartDefinition::calcPageNumberFromAssignmentId(PageCmper pageAssignmentId) const
{
    MUSX_ASSERT_IF(pageAssignmentId < 0) {
        throw std::logic_error("Attempt to convert negative page assignment " + std::to_string(pageAssignmentId) + " to a page number.");
    }
    std::optional<PageCmper> result = pageAssignmentId;
    if (result.value() > numberOfLeadingBlankPages) {
        const int calcValue = int(result.value()) - getDocument()->getMaxBlankPages() + numberOfLeadingBlankPages;
        if (calcValue > numberOfLeadingBlankPages) {
            result = PageCmper(calcValue);
        } else {
            return std::nullopt;
        }
    }
    return result;
}

PageCmper others::PartDefinition::calcAssignmentIdFromPageNumber(PageCmper pageId) const
{
    if (pageId != 0) {
        if (pageId > numberOfLeadingBlankPages) {
            const int calcValue = int(pageId) + getDocument()->getMaxBlankPages() - numberOfLeadingBlankPages;
            return PageCmper(calcValue);
        }
    }
    return pageId;
}

// ******************************
// ***** PercussionNoteInfo *****
// ******************************

const percussion::PercussionNoteType& others::PercussionNoteInfo::getNoteType() const
{
    return percussion::getPercussionNoteTypeFromId(getBaseNoteTypeId());
}

// *****************************
// ***** RepeatEndingStart *****
// *****************************

int others::RepeatEndingStart::calcEndingLength() const
{
    int maxLength = (std::numeric_limits<int>::max)();

    switch (jumpAction) {
        case RepeatActionType::JumpAuto:
            break;
        case RepeatActionType::JumpAbsolute:
            maxLength = targetValue - getCmper();
            break;
        case RepeatActionType::JumpRelative:
            maxLength = targetValue;
            break;
        default:
            return 1;
    }
    if (maxLength <= 0) {
        return 1;
    }
    Cmper x = getCmper() + 1;
    while (true) {
        auto measure = getDocument()->getOthers()->get<others::Measure>(getPartId(), x);
        if (!measure) {
            return 1;
        }
        if (measure->hasEnding && getDocument()->getOthers()->get<others::RepeatEndingStart>(getPartId(), x)) {
            break;
        }
        if (--maxLength <= 0) {
            break;
        }
        x++;
    }
    return x - getCmper();
}

bool others::RepeatEndingStart::calcIsOpen() const
{
    if (endLineVPos < 0) {
        return false;
    }
    if (jumpAction == others::RepeatActionType::NoJump) {
        return true;
    }
    for (Cmper x = getCmper(); true; x++) {
        auto measure = getDocument()->getOthers()->get<others::Measure>(getPartId(), x);
        if (!measure) {
            break;
        }
        if (measure->backwardsRepeatBar) {
            if (auto backRepeat = getDocument()->getOthers()->get<others::RepeatBack>(getPartId(), x)) {
                if (auto repeatOptions = getDocument()->getOptions()->get<options::RepeatOptions>()) {
                    return (backRepeat->leftVPos - backRepeat->rightVPos) == repeatOptions->bracketHookLen;
                }
                return true;
            }
        }
    }
    return false;
}

// **********************
// ***** StaffGroup *****
// **********************

std::string details::StaffGroup::getFullName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    // StaffGroup instances are always part-specific, so we can use getPartId here.
    return others::TextBlock::getText(getDocument(), fullNameId, getPartId(), true, accidentalStyle); // true: strip enigma tags
}

std::string details::StaffGroup::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    // StaffGroup instances are always part-specific, so we can use getPartId here.
    return others::TextBlock::getText(getDocument(), abbrvNameId, getPartId(), true, accidentalStyle); // true: strip enigma tags
}

MusxInstance<others::MultiStaffInstrumentGroup> details::StaffGroup::getMultiStaffInstGroup() const
{
    if (multiStaffGroupId) {
        if (auto retval = getDocument()->getOthers()->get<others::MultiStaffInstrumentGroup>(SCORE_PARTID, multiStaffGroupId)) {
            return retval;
        }
        MUSX_INTEGRITY_ERROR("StaffGroup " + std::to_string(getCmper2()) + " points to non-existent MultiStaffInstrumentGroup " + std::to_string(multiStaffGroupId));
    }
    return nullptr;
}

std::string details::StaffGroup::getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto multiStaffGroup = getMultiStaffInstGroup()) {
        if (auto staff = multiStaffGroup->getFirstStaffInstance()) {
            return staff->getFullInstrumentName(accidentalStyle);
        }
    }
    return getFullName(accidentalStyle);
}

std::string details::StaffGroup::getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle) const
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

details::StaffGroupInfo::StaffGroupInfo(const MusxInstance<StaffGroup>& staffGroup,
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

void details::StaffGroupInfo::iterateStaves(MeasCmper measId, Edu eduPosition, std::function<bool(const MusxInstance<others::StaffComposite>&)> iterator) const
{
    MUSX_ASSERT_IF (!startSlot || !endSlot) {
        throw std::logic_error("StaffGroupInfo::iterateStaves called with invalid start or end slot for system staves.");
    }
    for (size_t slot = startSlot.value(); slot <= endSlot.value(); slot++) {
        MUSX_ASSERT_IF (slot >= systemStaves.size()) {
            throw std::logic_error("StaffGroupInfo::iterateStaves encounted invalid slot (" + std::to_string(slot) + ") for system staves.");
        }
        const auto& iUsed = systemStaves[slot];
        if (auto staff = others::StaffComposite::createCurrent(iUsed->getDocument(), iUsed->getPartId(), iUsed->staffId, measId, eduPosition)) {
            if (!iterator(staff)) {
                break;
            }
        } else {
            MUSX_INTEGRITY_ERROR("StaffGroupInfo::iterateStaves could not find staff " + std::to_string(iUsed->staffId) + " in slot " + std::to_string(slot));
        }
    }
}

std::vector<details::StaffGroupInfo> details::StaffGroupInfo::getGroupsAtMeasure(MeasCmper measureId, Cmper linkedPartId,
    const MusxInstanceList<others::StaffUsed>& systemStaves)
{
    if (systemStaves.empty()) {
        util::Logger::log(util::Logger::LogLevel::Info, "Attempted to find groups for empty system staves. [measure " + std::to_string(measureId)
            + ", part " + std::to_string(linkedPartId) +"] Returning an empty vector.");
        return {};
    }
    auto rawGroups = systemStaves.getDocument()->getDetails()->getArray<details::StaffGroup>(linkedPartId, BASE_SYSTEM_ID);
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

// **********************
// ***** StaffStyle *****
// **********************

MusxInstanceList<others::StaffStyle> others::StaffStyle::findAllOverlappingStyles(const DocumentPtr& document,
        Cmper partId, StaffCmper staffId, MeasCmper measId, Edu eduPosition)
{
    auto staffStyleAssignments = document->getOthers()->getArray<others::StaffStyleAssign>(partId, staffId);
    std::vector<MusxInstance<others::StaffStyleAssign>> applicableAssignments;
    std::copy_if(staffStyleAssignments.begin(), staffStyleAssignments.end(), std::back_inserter(applicableAssignments),
        [measId, eduPosition](const MusxInstance<others::StaffStyleAssign>& range) {
            return range->contains(measId, eduPosition);
        });

    MusxInstanceList<others::StaffStyle> result(document, partId);
    result.reserve(applicableAssignments.size());
    for (const auto& assign : applicableAssignments) {
        if (auto style = assign->getStaffStyle()) {
            result.emplace_back(style);
        }
    }
    return result;
 }

// ****************************
// ***** StaffStyleAssign *****
// ****************************

MusxInstance<others::StaffStyle> others::StaffStyleAssign::getStaffStyle() const
{
    auto result = getDocument()->getOthers()->get<others::StaffStyle>(getPartId(), styleId);
    if (!result) {
        MUSX_INTEGRITY_ERROR("Staff style assignment has invalid staff style ID " + std::to_string(styleId)
            + ": Part " + std::to_string(getPartId())
            + " Staff " + std::to_string(getCmper())
        );
    }
    return result;
}

// ***********************
// ***** StaffSystem *****
// ***********************

MusxInstance<others::Page> others::StaffSystem::getPage() const
{
    return getDocument()->getOthers()->get<others::Page>(getPartId(), pageId);
}

std::pair<util::Fraction, util::Fraction> others::StaffSystem::calcMinMaxStaffSizes() const
{
    if (hasStaffScaling) {
        auto systemStaves = getDocument()->getOthers()->getArray<others::StaffUsed>(getPartId(), getCmper());
        if (!systemStaves.empty()) {
            std::pair<util::Fraction, util::Fraction> result = std::make_pair((std::numeric_limits<util::Fraction>::max)(), (std::numeric_limits<util::Fraction>::min)());
            for (const auto& systemStaff : systemStaves) {
                auto staffSize = getDocument()->getDetails()->get<details::StaffSize>(getPartId(), getCmper(), systemStaff->getCmper());
                const util::Fraction val = staffSize ? double(staffSize->staffPercent) / 100.0 : 1.0;
                if (val < result.first) result.first = val;
                if (val > result.second) result.second = val;
            }
            return result;
        }
    }
    return std::make_pair(util::Fraction(1), util::Fraction(1));;
}

// ***********************
// ***** TempoChange *****
// ***********************

int others::TempoChange::getAbsoluteTempo(NoteType noteType) const
{
    if (isRelative) {
        throw std::logic_error("Tempo change at measure " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value())
            + " is a relative tempo change.");
    }
    double result = (ratio * unit) / 65536.0; 
    /* The value here is relative to 60 BPM == 1024 */
    result *= 60.0;            
    result /= double(noteType);            
    return int(std::lround(result));
}

// *********************
// ***** TextBlock *****
// *********************

util::EnigmaParsingContext others::TextBlock::getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId, util::EnigmaString::TextInsertCallback defaultInsertFunc) const
{
    MusxInstance<TextsBase> rawText;
    switch (textType) {
        default:
            break;
        case TextType::Block:
            rawText = getDocument()->getTexts()->get<texts::BlockText>(textId);
            break;
        case TextType::Expression:
            rawText = getDocument()->getTexts()->get<texts::ExpressionText>(textId);
            break;
    }
    if (rawText) {
        return rawText->getRawTextCtx(forPartId, forPageId, defaultInsertFunc);
    }
    return {};
}

std::string others::TextBlock::getText(const DocumentPtr& document, const Cmper textId, Cmper forPartId, bool trimTags, util::EnigmaString::AccidentalStyle accidentalStyle)
{
    auto textBlock = document->getOthers()->get<others::TextBlock>(forPartId, textId);
    if (textBlock) {
        return textBlock->getRawTextCtx(forPartId).getText(trimTags, accidentalStyle);
    }
    return {};
}

// *****************************
// ***** TextExpressionDef *****
// *****************************

MusxInstance<others::TextBlock> others::TextExpressionDef::getTextBlock() const
{
    return getDocument()->getOthers()->get<others::TextBlock>(getPartId(), textIdKey);
}

util::EnigmaParsingContext others::TextExpressionDef::getRawTextCtx(Cmper forPartId) const
{
    if (auto textBlock = getTextBlock()) {
        return textBlock->getRawTextCtx(forPartId, std::nullopt, [&](const std::vector<std::string>& components) -> std::optional<std::string> {
            if (components[0] == "value") {
                return std::to_string(value);
            } else if (components[0] == "control") {
                return std::to_string(auxData1);
            } else if (components[0] == "pass") {
                return std::to_string(playPass);
            }
            return std::nullopt;
        });
    }
    return {};
}

MusxInstance<others::Enclosure> others::TextExpressionDef::getEnclosure() const
{
    if (!hasEnclosure) return nullptr;
    return getDocument()->getOthers()->get<others::TextExpressionEnclosure>(getPartId(), getCmper());
}

// *********************
// ***** TextsBase *****
// *********************

util::EnigmaParsingContext TextsBase::getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId,
    util::EnigmaString::TextInsertCallback defaultInsertFunc) const
{
    return util::EnigmaParsingContext(shared_from_this(), forPartId, forPageId, defaultInsertFunc);
}

// *************************
// ***** TimeSignature *****
// *************************

TimeSignature::TimeSignature(const DocumentWeakPtr& document, int beats, Edu unit, bool hasCompositeTop, bool hasCompositeBottom, std::optional<bool> abbreviate)
    : CommonClassBase(document), m_abbreviate(abbreviate)
{
    auto tops = [&]() -> std::vector<std::vector<util::Fraction>> {
        if (hasCompositeTop) {
            if (auto comps = getDocument()->getOthers()->get<others::TimeCompositeUpper>(SCORE_PARTID, Cmper(beats))) {
                std::vector<std::vector<util::Fraction>> result;
                for (const auto& nextItem : comps->items) {
                    if (nextItem->startGroup || result.empty()) {
                        result.push_back({});
                    }
                    result[result.size() - 1].push_back(nextItem->fullFraction());
                }
                return result;
            } else {
                return {};
            }
        }
        return { {beats} };
    }();
    auto bots = [&]() -> std::vector<std::vector<Edu>> {
        if (hasCompositeBottom) {
            if (auto comps = getDocument()->getOthers()->get<others::TimeCompositeLower>(SCORE_PARTID, Cmper(unit))) {
                std::vector<std::vector<Edu>>result;
                for (const auto& nextItem : comps->items) {
                    if (nextItem->startGroup || result.empty()) {
                        result.push_back({});
                    }
                    result[result.size() - 1].push_back(nextItem->unit);
                }
                return result;
            } else {
                return {};
            }
        }
        return { {unit} };
    }();

    if (tops.empty() || bots.empty()) {
        throw std::invalid_argument("Time signature is missing composite top array or composite bottom array.");
    }
    if (tops.size() != bots.size()) {
        MUSX_INTEGRITY_ERROR("Composite top group for time signature does not match composite bottom group.");
    }
    for (size_t x = 0; x < (std::min)(tops.size(), bots.size()); x++) {
        components.push_back({ tops[x], bots[x] });
    }
}

std::optional<char32_t> TimeSignature::getAbbreviatedSymbol() const
{
    auto musicChars = getDocument()->getOptions()->get<options::MusicSymbolOptions>();
    const char32_t commonTimeSymbol = musicChars ? musicChars->timeSigAbrvCommon : U'\U0000E08A';   // SMuFL common time symbol default
    const char32_t cutTimeSymbol = musicChars ? musicChars->timeSigAbrvCut : U'\U0000E08B';         // SMuFL cut time symbol default
    if (m_abbreviate.has_value()) {
        if (m_abbreviate.value()) {
            if (isCutTime()) {
                return cutTimeSymbol;
            } else if (isCommonTime()) {
                return commonTimeSymbol;
            }
        }
    } else if (auto options = getDocument()->getOptions()->get<options::TimeSignatureOptions>()) {
        if (options->timeSigDoAbrvCut && isCutTime()) {
            return cutTimeSymbol;
        } else if (options->timeSigDoAbrvCommon && isCommonTime()) {
            return commonTimeSymbol;
        }
    }
    return std::nullopt;
}

bool TimeSignature::isCommonTime() const
{
    if (components.size() != 1 || components[0].counts.size() != 1 || components[0].units.size() != 1) {
        return false;
    }
    return components[0].counts[0] == 4 && components[0].units[0] == Edu(NoteType::Quarter);
}

bool TimeSignature::isCutTime() const
{
    if (components.size() != 1 || components[0].counts.size() != 1 || components[0].units.size() != 1) {
        return false;
    }
    return components[0].counts[0] == 2 && components[0].units[0] == Edu(NoteType::Half);
}

std::pair<util::Fraction, NoteType> TimeSignature::calcSimplified() const
{
    // Lambda to compute GCD of a vector
    auto computeGCD = [](const std::vector<Edu>& values) {
        return values.empty() ? 1 : std::reduce(values.begin() + 1, values.end(), values[0], std::gcd<Edu, Edu>);
    };
    
    std::vector<Edu> allUnits;
    std::vector<std::pair<util::Fraction, Edu>> summedUnits;

    for (const auto& ts : components) {
        Edu totalUnit = ts.sumUnits();
        summedUnits.emplace_back(ts.sumCounts(), totalUnit);
        allUnits.push_back(totalUnit);
    }

    Edu finalUnit = computeGCD(allUnits); // The final unit size

    // Compute final beats relative to finalUnit
    auto totalBeats = std::accumulate(summedUnits.begin(), summedUnits.end(), util::Fraction{}, [finalUnit](auto acc, const auto& p) {
        return acc + p.first * (p.second / finalUnit);
    });

    if (!finalUnit) {
        throw std::logic_error("The beat size is zero.");
    }

    int power2 = 0;
    int otherPrimes = finalUnit;
    while ((otherPrimes & 0x01) == 0) {
        otherPrimes >>= 1;
        power2++;
    }

    return { totalBeats * otherPrimes, NoteType(1 << power2) };
}

} // namespace dom    
} // namespace musx
