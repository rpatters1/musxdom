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
#include <cstdlib>
#include <exception>
#include <limits>
#include <string>
#include <vector>

#include "musx/musx.h"

namespace musx {
namespace dom {

// ********************
// ***** Document *****
// ********************

MusxInstanceList<others::StaffUsed> Document::getScrollViewStaves(Cmper partId) const
{
    return getOthers()->getArray<others::StaffUsed>(partId, calcScrollViewCmper(partId));
}

MusxInstance<others::Page> Document::calcPageFromMeasure(Cmper partId, MeasCmper measureId) const
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

MusxInstance<others::StaffSystem> Document::calcSystemFromMeasure(Cmper partId, MeasCmper measureId) const
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

    const auto scrollView = getScrollViewStaves(forPartId);
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
                        multiStaffInstsFound.emplace(rawStaff->multiStaffInstId);
                        const auto [it, created] = result.emplace(rawStaff->getCmper(), InstrumentInfo());
                        MUSX_ASSERT_IF(!created) {
                            throw std::logic_error("Attempted to insert multi-instrument id " + std::to_string(rawStaff->multiStaffInstId) + " more than once.");
                        }
                        if (auto multiStaffGroupId = getOthers()->get<others::MultiStaffGroupId>(forPartId, rawStaff->multiStaffInstId)) {
                            it->second.staffGroupId = multiStaffGroupId->staffGroupId;
                        }
                        it->second.multistaffGroupId = rawStaff->multiStaffInstId;
                        std::optional<size_t> topIndex = scrollView.getIndexForStaff(rawStaff->getCmper());
                        MUSX_ASSERT_IF(!topIndex.has_value()) {
                            throw std::logic_error("Unable to find " + std::to_string(rawStaff->getCmper()) + " in scrollView.");
                        }
                        for (StaffCmper staffId : multiStaffInst->staffNums) {
                            std::optional<size_t> staffIndex = scrollView.getIndexForStaff(staffId);
                            if (staffIndex.has_value()) {
                                it->second.staves.emplace(staffId, staffIndex.value() - topIndex.value());
                                mappedStaves.emplace(staffId);
                            } else if (forPartId == SCORE_PARTID) {
                                MUSX_INTEGRITY_ERROR("Unable to find staff " + std::to_string(staffId) + " from multistaff instrument group in scrollView.");
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
        if (group->bracket && group->bracket->style == details::Bracket::BracketStyle::PianoBrace) {
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
                            util::Logger::log(util::Logger::LogLevel::Verbose, "Treating piano brace " + std::to_string(group->getCmper2())
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
    auto result = InstrumentInfo::getInstrumentForStaff(m_instruments, staffId);
    if (!result) {
        assert(false); // flag this as early as possible, because getting here is a program bug.
        throw std::logic_error("Staff " + std::to_string(staffId) + " was not mapped to an instrument.");
    }
    return *result;
}

MusicRange Document::calcEntireDocument() const
{
    auto measures = getOthers()->getArray<others::Measure>(SCORE_PARTID);
    return MusicRange(m_self, 1, 0, static_cast<MeasCmper>(measures.size()), (std::numeric_limits<util::Fraction>::max)());
}

bool Document::calcHasVaryingSystemStaves(Cmper forPartId) const
{
    auto staffSystems = getOthers()->getArray<others::StaffSystem>(forPartId);
    auto scrollView = getScrollViewStaves(forPartId);
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

bool Document::iterateEntries(Cmper partId, std::function<bool(const EntryInfoPtr&)> iterator) const
{
    auto scrollView = getScrollViewStaves(partId);
    if (scrollView.empty()) {
        return true;
    }
    return scrollView.iterateEntries(0, scrollView.size() - 1, calcEntireDocument(), iterator);
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

const InstrumentInfo* InstrumentInfo::getInstrumentForStaff(const InstrumentMap& map, StaffCmper staffId)
{
    const auto& instIt = map.find(staffId);
    if (instIt != map.end()) {
        return &instIt->second;
    } else {
        for (const auto& [top, info] : map) {
            if (info.staves.find(staffId) != info.staves.end()) {
                return &info;
            }
        }
    }
    return nullptr;
}

} // namespace dom
} // namespace musx