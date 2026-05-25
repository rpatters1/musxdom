/*
 * Copyright (C) 2026, Robert Patterson
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

#include "musx/dom/Instrument.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <set>
#include <utility>

#include "musx/musx.h"

namespace musx {
namespace dom {

namespace {

struct StaffStyleInstrumentSpan {
    std::string instUuid;
    std::optional<MusicPoint> revertPosition;

    bool operator==(const StaffStyleInstrumentSpan& other) const
    { return instUuid == other.instUuid && revertPosition == other.revertPosition; }

    bool operator!=(const StaffStyleInstrumentSpan& other) const
    { return !(*this == other); }
};

using StaffStyleInstrumentSpans = std::map<MusicPoint, StaffStyleInstrumentSpan>;

StaffStyleInstrumentSpans collectStaffStyleInstrumentSpans(const DocumentPtr& document, Cmper partId, StaffCmper staffId)
{
    StaffStyleInstrumentSpans result;
    const auto assignments = document->getOthers()->getArray<others::StaffStyleAssign>(partId, staffId);
    for (const auto& assignment : assignments) {
        const auto style = assignment->getStaffStyle();
        if (style && style->containsInstrumentChange()) {
            const auto start = MusicPoint(assignment->startMeas, util::Fraction::fromEdu(assignment->startEdu));
            result.emplace(start, StaffStyleInstrumentSpan{ style->instUuid, assignment->nextLocation(staffId) });
        }
    }
    return result;
}

std::string formatMusicPoint(const MusicPoint& point)
{
    return "measure " + std::to_string(point.measureId)
        + ", edu " + std::to_string(point.position.calcEduDuration());
}

} // namespace

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

InstrumentInfo::InstrumentChangeEvents InstrumentInfo::getChanges() const
{
    const auto sequentialStaves = getSequentialStaves();
    if (sequentialStaves.empty()) {
        return {};
    }

    const auto document = getDocument();
    const auto partId = getPartId();
    const auto topStaffId = sequentialStaves.front();
    const auto topStaffEvents = collectStaffStyleInstrumentSpans(document, partId, topStaffId);
    StaffStyleInstrumentSpans acceptedEvents = topStaffEvents;

    for (auto staffIt = std::next(sequentialStaves.begin()); staffIt != sequentialStaves.end(); ++staffIt) {
        const auto staffId = *staffIt;
        const auto staffEvents = collectStaffStyleInstrumentSpans(document, partId, staffId);
        for (auto eventIt = acceptedEvents.begin(); eventIt != acceptedEvents.end();) {
            const auto staffEventIt = staffEvents.find(eventIt->first);
            if (staffEventIt == staffEvents.end() || staffEventIt->second != eventIt->second) {
                util::Logger::log(util::Logger::LogLevel::Warning,
                    "Ignoring instrument change for top staff " + std::to_string(topStaffId)
                    + " at " + formatMusicPoint(eventIt->first)
                    + " because staff " + std::to_string(staffId) + " has no aligned instrument change.");
                eventIt = acceptedEvents.erase(eventIt);
            } else {
                ++eventIt;
            }
        }
        for (const auto& [staffChangeStart, staffRevert] : staffEvents) {
            const auto topEventIt = topStaffEvents.find(staffChangeStart);
            if (topEventIt == topStaffEvents.end() || topEventIt->second != staffRevert) {
                util::Logger::log(util::Logger::LogLevel::Warning,
                    "Ignoring instrument change for staff " + std::to_string(staffId)
                    + " at " + formatMusicPoint(staffChangeStart)
                    + " because top staff " + std::to_string(topStaffId) + " has no aligned instrument change.");
            }
        }
    }

    std::set<MusicPoint> changePositions;
    changePositions.emplace(MusicPoint{});
    for (const auto& [start, span] : acceptedEvents) {
        changePositions.emplace(start);
        if (span.revertPosition) {
            changePositions.emplace(*span.revertPosition);
        }
    }

    InstrumentChangeEvents result;
    for (const auto& position : changePositions) {
        auto topStaffComposite = others::StaffComposite::createCurrent(document, partId, topStaffId, position.measureId, position.position.calcEduDuration());
        if (!topStaffComposite) {
            util::Logger::log(util::Logger::LogLevel::Warning,
                "Unable to create top staff composite for instrument change on staff " + std::to_string(topStaffId)
                + " at " + formatMusicPoint(position) + ".");
            continue;
        }
        const auto identity = InstrumentIdentity{ topStaffComposite->instUuid };
        result.emplace(position, InstrumentChange{ identity, topStaffComposite });
    }
    return result;
}

std::vector<InstrumentInfo::InstrumentIdentity> InstrumentInfo::getInstrumentIdentities() const
{
    std::vector<InstrumentIdentity> result;
    std::set<InstrumentIdentity> seen;
    for (const auto& [_, change] : getChanges()) {
        if (seen.insert(change.identity).second) {
            result.emplace_back(change.identity);
        }
    }
    return result;
}

InstrumentInfo::InstrumentIdentity InstrumentInfo::getInstrumentIdentityAt(MusicPoint point) const
{
    const auto changes = getChanges();
    const auto it = changes.upper_bound(point);
    MUSX_ASSERT_IF(it == changes.begin()) {
        throw std::logic_error("No instrument identity found at " + formatMusicPoint(point) + ".");
    }
    return std::prev(it)->second.identity;
}

const InstrumentInfo* InstrumentMap::getInstrumentForStaff(StaffCmper staffId) const
{
    const auto& instIt = this->find(staffId);
    if (instIt != this->end()) {
        return &instIt->second;
    } else {
        for (const auto& [top, info] : *this) {
            if (info.staves.find(staffId) != info.staves.end()) {
                return &info;
            }
        }
    }
    return nullptr;
}

} // namespace dom
} // namespace musx
