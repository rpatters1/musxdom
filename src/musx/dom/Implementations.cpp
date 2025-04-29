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
#include <filesystem>
#include <sstream>
#include <functional>
#include <numeric>
#include <algorithm>
#include <cmath>

 // This header includes method implementations that need to see all the classes in the dom

#include "musx/musx.h"
#include "music_theory/music_theory.hpp"

#if ! defined(MUSX_RUNNING_ON_WINDOWS)
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif

namespace musx {
namespace dom {

// *****************
// ***** Base *****
// *****************

std::shared_ptr<others::PartDefinition> Base::getPartDefinition() const
{
    if (auto retval = getDocument()->getOthers()->get<others::PartDefinition>(SCORE_PARTID, getPartId())) {
        return retval;
    }
    MUSX_INTEGRITY_ERROR("PartDefinition for part id " + std::to_string(getPartId()) + " does not exist.");
    return nullptr;
}

// *****************
// ***** Entry *****
// *****************

std::shared_ptr<Entry> Entry::getNext() const
{
    if (!m_next) return nullptr;
    auto retval = getDocument()->getEntries()->get<Entry>(m_next);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Entry " + std::to_string(m_entnum) + " has next entry " + std::to_string(m_next) + " that does not exist.");
    }
    return retval;
}

std::shared_ptr<Entry> Entry::getPrevious() const
{
    if (!m_prev) return nullptr;
    auto retval = getDocument()->getEntries()->get<Entry>(m_prev);
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

EntryFrame::EntryFrame(const details::GFrameHold& gfhold, InstCmper staff, MeasCmper measure, LayerIndex layerIndex, bool forWrittenPitch) :
    Base(gfhold.getDocument(), gfhold.getPartId(), gfhold.getShareMode()),
    m_staff(staff),
    m_measure(measure),
    m_layerIndex(layerIndex),
    m_forWrittenPitch(forWrittenPitch)
{
}

EntryInfoPtr EntryFrame::getFirstInVoice(int voice) const
{
    bool forV2 = voice == 2;
    auto firstEntry = EntryInfoPtr(shared_from_this(), 0);
    if (firstEntry->getEntry()->voice2) {
        MUSX_INTEGRITY_ERROR("Entry frame for staff " + std::to_string(m_staff) + " measure " + std::to_string(m_measure)
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

std::shared_ptr<const EntryFrame> EntryFrame::getNext() const
{
    if (auto gfhold = getDocument()->getDetails()->get<details::GFrameHold>(getPartId(), m_staff, m_measure + 1)) {
        return gfhold->createEntryFrame(m_layerIndex, m_forWrittenPitch);
    }
    return nullptr;
}

std::shared_ptr<const EntryFrame> EntryFrame::getPrevious() const
{
    if (m_measure > 1) {
        if (auto gfhold = getDocument()->getDetails()->get<details::GFrameHold>(getPartId(), m_staff, m_measure - 1)) {
            return gfhold->createEntryFrame(m_layerIndex, m_forWrittenPitch);
        }
    }
    return nullptr;
}

// ************************
// ***** EntryInfoPtr *****
// ************************

const std::shared_ptr<const EntryInfo> EntryInfoPtr::operator->() const
{
    MUSX_ASSERT_IF(!m_entryFrame)
    {
        throw std::logic_error("EntryInfoPtr has no frame.");
    }
    MUSX_ASSERT_IF(m_indexInFrame >= m_entryFrame->getEntries().size())
    {
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

InstCmper EntryInfoPtr::getStaff() const { return m_entryFrame->getStaff(); }

MeasCmper EntryInfoPtr::getMeasure() const { return m_entryFrame->getMeasure(); }

std::shared_ptr<KeySignature> EntryInfoPtr::getKeySignature() const { return m_entryFrame->keySignature; }

std::shared_ptr<others::StaffComposite> EntryInfoPtr::createCurrentStaff(const std::optional<InstCmper>& forStaffId) const
{
    auto entry = (*this)->getEntry();
    return others::StaffComposite::createCurrent(entry->getDocument(), entry->getPartId(), forStaffId.value_or(getStaff()),
        getMeasure(), (*this)->elapsedDuration.calcEduDuration());
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
        }
        else {
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
        if (auto beamBreaks = entry->getDocument()->getDetails()->get<details::SecondaryBeamBreak>(entry->getPartId(), entry->getEntryNumber())) {
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
            return std::max(lowestBeamStart, lowestBeamEnd);
        }
    }
    return 0;
}

bool EntryInfoPtr::calcBeamStubIsLeft() const
{
    auto entry = (*this)->getEntry();
    if (entry->stemDetail) {
        if (auto manual = entry->getDocument()->getDetails()->get<details::BeamStubDirection>(entry->getPartId(), entry->getEntryNumber())) {
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

// ***********************
// ***** FontOptions *****
// ***********************

std::shared_ptr<FontInfo> options::FontOptions::getFontInfo(options::FontOptions::FontType type) const
{
    auto it = fontOptions.find(type);
    if (it == fontOptions.end()) {
        throw std::invalid_argument("Font type " + std::to_string(int(type)) + " not found in document");
    }
    return it->second;
}

std::shared_ptr<FontInfo> options::FontOptions::getFontInfo(const DocumentPtr& document, options::FontOptions::FontType type)
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

std::optional<std::filesystem::path> FontInfo::calcSMuFLMetaDataPath() const
{
    auto name = getName();
    auto standardFontPaths = calcSMuFLPaths();
    for (const auto& path : standardFontPaths) {
        if (!path.empty()) {
            std::filesystem::path metaFilePath(path / name / name);
            metaFilePath.replace_extension(".json");
            if (std::filesystem::is_regular_file(metaFilePath)) {
                return metaFilePath;
            }
        }
    }
    return std::nullopt;
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

std::vector<std::shared_ptr<const Entry>> others::Frame::getEntries()
{
    std::vector<std::shared_ptr<const Entry>> retval;
    auto firstEntry = startEntry ? getDocument()->getEntries()->get<Entry>(startEntry) : nullptr;
    if (!firstEntry) {
        MUSX_INTEGRITY_ERROR("Frame " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value_or(-1)) + " is not iterable.");
        return retval;
    }
    for (auto entry = firstEntry; entry; entry = entry->getNext()) {
        retval.emplace_back(entry);
        if (entry->getEntryNumber() == endEntry) {
            break;
        }
    }
    return retval;
}

// **********************
// ***** GFrameHold *****
// **********************

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
struct TupletState
{
    util::Fraction remainingSymbolicDuration;         // The remaining symbolic duration
    util::Fraction ratio;             // The remaining actual duration
    std::shared_ptr<const details::TupletDef> tuplet; // The tuplet.
    size_t infoIndex;               // the index of this tuplet in EntryFrame::tupletInfo

    void accountFor(util::Fraction actual)
    {
        remainingSymbolicDuration -= (actual / ratio);
    }

    TupletState(const std::shared_ptr<details::TupletDef>& t, size_t i)
        : remainingSymbolicDuration(t->displayNumber* t->displayDuration, int(NoteType::Whole)),
        ratio(t->referenceNumber* t->referenceDuration, t->displayNumber* t->displayDuration),
        tuplet(t), infoIndex(i)
    {
    }
};
#endif // DOXYGEN_SHOULD_IGNORE_THIS

std::shared_ptr<const EntryFrame> details::GFrameHold::createEntryFrame(LayerIndex layerIndex, bool forWrittenPitch) const
{
    if (layerIndex >= frames.size()) { // note: layerIndex is unsigned
        throw std::invalid_argument("invalid layer index [" + std::to_string(layerIndex) + "]");
    }
    if (!frames[layerIndex]) return nullptr; // nothing here
    auto document = getDocument();
    auto frameIncis = document->getOthers()->getArray<others::Frame>(getPartId(), frames[layerIndex]);
    auto frame = [frameIncis]() -> std::shared_ptr<others::Frame> {
        for (const auto& frame : frameIncis) {
            if (frame->startEntry) {
                return frame;
            }
        }
        return nullptr;
    }();
    std::shared_ptr<EntryFrame> retval;
    if (frame) {
        retval = std::make_shared<EntryFrame>(*this, getStaff(), getMeasure(), layerIndex, forWrittenPitch);
        const auto& measure = getDocument()->getOthers()->get<others::Measure>(getPartId(), getMeasure());
        if (!measure) {
            throw std::invalid_argument("Measure instance for measure " + std::to_string(getMeasure()) + " does not exist.");
        }
        const auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), getStaff(), getMeasure(), 0);
        if (!staff) {
            throw std::invalid_argument("Staff instance for staff " + std::to_string(getStaff()) + " does not exist.");
        }
        retval->keySignature = measure->createKeySignature(getStaff());
        if (forWrittenPitch && staff->transposition && staff->transposition->keysig) {
            retval->keySignature->setTransposition(staff->transposition->keysig->interval, staff->transposition->keysig->adjust, !staff->transposition->noSimplifyKey);
        }
        auto entries = frame->getEntries();
        std::vector<TupletState> v1ActiveTuplets; // List of active tuplets for v1
        std::vector<TupletState> v2ActiveTuplets; // List of active tuplets for v2
        util::Fraction v1ActualElapsedDuration = 0;
        for (const auto& f : frameIncis) {
            v1ActualElapsedDuration += util::Fraction::fromEdu(f->startTime); // if there is an old-skool pickup, this accounts for it
        }
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
            if (forWrittenPitch && staff->transposition && staff->transposition->setToClef) {
                entryInfo->clefIndex = staff->transposedClef;
            } else {
                entryInfo->clefIndex = calcClefIndexAt(actualElapsedDuration);
            }
            util::Fraction cumulativeRatio = 1;
            if (!entry->graceNote) {
                graceIndex = 0;
                auto tuplets = document->getDetails()->getArray<details::TupletDef>(SCORE_PARTID, entry->getEntryNumber());
                std::sort(tuplets.begin(), tuplets.end(), [](const auto& a, const auto& b) {
                    return a->calcReferenceDuration() > b->calcReferenceDuration(); // Sort descending by reference duration
                    });
                for (const auto& tuplet : tuplets) {
                    size_t index = retval->tupletInfo.size();
                    retval->tupletInfo.emplace_back(tuplet, i, actualElapsedDuration);
                    activeTuplets.emplace_back(tuplet, index);
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

            retval->addEntry(entryInfo);

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
                    auto& tuplInf = retval->tupletInfo[tuplet.infoIndex];
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
        MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getStaff()) + " and measure "
            + std::to_string(getMeasure()) + " points to non-existent frame [" + std::to_string(frames[layerIndex]) + "]");
    }
    return retval;
}

bool details::GFrameHold::iterateEntries(LayerIndex layerIndex, std::function<bool(const EntryInfoPtr&)> iterator)
{
    bool forWrittenPitch = false;
    if (auto partGlobals = getDocument()->getOthers()->get<others::PartGlobals>(getPartId(), MUSX_GLOBALS_CMPER)) {
        forWrittenPitch = partGlobals->showTransposed;
    }
    auto entryFrame = createEntryFrame(layerIndex, forWrittenPitch);
    if (entryFrame) {
        for (size_t x = 0; x < entryFrame->getEntries().size(); x++) {
            if (!iterator(EntryInfoPtr(entryFrame, x))) {
                return false;
            }
        }
    }
    return true;
}

bool details::GFrameHold::iterateEntries(std::function<bool(const EntryInfoPtr&)> iterator)
{
    for (LayerIndex layerIndex = 0; layerIndex < frames.size(); layerIndex++) {
        if (!iterateEntries(layerIndex, iterator)) {
            return false;
        }
    }
    return true;
}

ClefIndex details::GFrameHold::calcClefIndexAt(Edu position) const
{
    if (clefId.has_value()) {
        return clefId.value();
    }
    ClefIndex result = 0;
    auto clefList = getDocument()->getOthers()->getArray<others::ClefList>(getPartId(), clefListId);
    if (clefList.empty()) {
        MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getStaff()) + " and measure "
            + std::to_string(getMeasure()) + " has non-existent clef list [" + std::to_string(clefListId) + "]");
    }
    else {
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

// ***********************************
// ***** IndependentStaffDetails *****
// ***********************************

std::shared_ptr<TimeSignature> details::IndependentStaffDetails::createTimeSignature() const
{
   return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, altNumTsig, altDenTsig));
}

std::shared_ptr<TimeSignature> details::IndependentStaffDetails::createDisplayTimeSignature() const
{
    if (!displayAbbrvTime) {
        return createTimeSignature();
    }
    return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivBeat, displayAltNumTsig, displayAltDenTsig, displayAbbrvTime));
}

// **************************
// ***** InstrumentUsed *****
// **************************

std::shared_ptr<others::Staff> others::InstrumentUsed::getStaff() const
{
    auto retval = getDocument()->getOthers()->get<others::Staff>(getPartId(), staffId);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffId) + " not found for InstrumentUsed list " + std::to_string(getCmper()));
    }
    return retval;
}

std::shared_ptr<others::Staff> others::InstrumentUsed::getStaffAtIndex(const std::vector<std::shared_ptr<others::InstrumentUsed>>& iuArray, Cmper index)
{
    if (index >= iuArray.size()) return nullptr;
    auto iuItem = iuArray[index];
    return iuItem->getStaff();
}

std::optional<size_t> others::InstrumentUsed::getIndexForStaff(const std::vector<std::shared_ptr<InstrumentUsed>>& iuArray, InstCmper staffId)
{
    for (size_t x = 0; x < iuArray.size(); x++) {
        if (iuArray[x]->staffId == staffId) {
            return x;
        }
    }
    return std::nullopt;
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

std::vector<unsigned> KeySignature::calcTonalCenterArray() const
{
    int alter = getAlteration();
    if (alter >= 0) {
        return calcTonalCenterArrayForSharps();
    } else {
        return calcTonalCenterArrayForFlats();
    }
}

std::vector<int> KeySignature::calcAcciAmountsArray() const
{
    int alter = getAlteration();

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

std::vector<unsigned> KeySignature::calcAcciOrderArray() const
{
    int alter = getAlteration();

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

int KeySignature::calcTonalCenterIndex() const
{
    if (!isNonLinear() && !isLinear()) {
        MUSX_INTEGRITY_ERROR("Key signature mode " + std::to_string(getKeyMode()) + " is neither linear nor non-linear. It is invalid.");
    }

    int alter = getAlteration();
    auto centers = calcTonalCenterArray();
    return int(centers[std::abs(alter) % centers.size()]);
}

int KeySignature::calcAlterationOnNote(unsigned noteIndex) const
{
    if (!isNonLinear() && !isLinear()) {
        MUSX_INTEGRITY_ERROR("Key signature mode " + std::to_string(getKeyMode()) + " is neither linear nor non-linear. It is invalid.");
    }

    auto amounts = calcAcciAmountsArray();
    auto order = calcAcciOrderArray();

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
        unsigned keyFifths = std::abs(getAlteration());
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
    int concertAlteration = getAlteration();
    int concertTonalCenterIndex = calcTonalCenterIndex();
    int tonalCenterOffset = interval % music_theory::STANDARD_DIATONIC_STEPS;
    
    int alteration = concertAlteration + keyAdjustment;
    if (simplify && keyAdjustment) {
        // Finale does not simplify microtonal key sigs correctly.
        // This simplification *does* work correctly, provided the custom key sig
        // is set up to increment each key signature accidental by the number of steps
        // in a chromatic half-step.
        int direction = music_theory::sign(alteration);
        while (std::abs(alteration) >= 7) {
            alteration -= direction * music_theory::STANDARD_12EDO_STEPS;
            tonalCenterOffset += direction;
        }
    }
    m_alterationOffset = alteration - concertAlteration;
    m_octaveDisplacement += (concertTonalCenterIndex + tonalCenterOffset) / music_theory::STANDARD_DIATONIC_STEPS;
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
                    syllables.push_back(std::shared_ptr<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen)));
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
        syllables.push_back(std::shared_ptr<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen)));
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

std::shared_ptr<KeySignature> others::Measure::createKeySignature(const std::optional<InstCmper>& forStaff) const
{
    std::shared_ptr<KeySignature> result;
    if (forStaff) {
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatKeys) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getPartId(), forStaff.value(), getCmper())) {
                    if (floats->hasKey) {
                        result = std::make_shared<KeySignature>(*floats->keySig);
                    }
                }
            }
        }
    }
    if (!result) {
        result = std::make_shared<KeySignature>(*globalKeySig);
    }
    return result;
}

std::shared_ptr<TimeSignature> others::Measure::createTimeSignature(const std::optional<InstCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatKeys) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getPartId(), forStaff.value(), getCmper())) {
                    if (floats->hasTime) {
                        return floats->createTimeSignature();
                    }
                }
            }
        }
    }
   return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), beats, divBeat, compositeNumerator, compositeDenominator));
}

std::shared_ptr<TimeSignature> others::Measure::createDisplayTimeSignature(const std::optional<InstCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatKeys) {
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
    return std::shared_ptr<TimeSignature>(new TimeSignature(getDocument(), dispBeats, dispDivbeat, compositeDispNumerator, compositeDispDenominator, abbrvTime));
}

// *****************************
// ***** MeasureExprAssign *****
// *****************************

std::shared_ptr<others::TextExpressionDef> others::MeasureExprAssign::getTextExpression() const
{
    if (!textExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<others::TextExpressionDef>(getPartId(), textExprId);
}

std::shared_ptr<others::ShapeExpressionDef> others::MeasureExprAssign::getShapeExpression() const
{
    if (!shapeExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<others::ShapeExpressionDef>(getPartId(), shapeExprId);
}

// *******************************
// ***** MeasureNumberRegion *****
// *******************************

std::shared_ptr<others::MeasureNumberRegion> others::MeasureNumberRegion::findMeasure(const DocumentPtr& document, MeasCmper measureId)
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

// *************************************
// ***** MultiStaffInstrumentGroup *****
// *************************************

std::shared_ptr<others::Staff> others::MultiStaffInstrumentGroup::getStaffAtIndex(size_t x) const
{
    if (x >= staffNums.size()) return nullptr;
    auto retval = getDocument()->getOthers()->get<others::Staff>(getPartId(), staffNums[x]);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffNums[x])
            + " not found for multiple staff instrument " + std::to_string(getCmper()));
    }
    return retval;
}

std::shared_ptr<others::Staff> others::MultiStaffInstrumentGroup::getFirstStaff() const
{
    if (staffNums.empty()) {
        MUSX_INTEGRITY_ERROR("MultiStaffInstrumentGroup " + std::to_string(getCmper()) + " contains no staves.");
        return nullptr;
    }
    return getStaffAtIndex(0);
}

std::shared_ptr<details::StaffGroup> others::MultiStaffInstrumentGroup::getStaffGroup() const
{
    auto document = getDocument();
    auto groupIdRecord = document->getOthers()->get<others::MultiStaffGroupId>(getPartId(), getCmper());
    if (!groupIdRecord) return nullptr;
    auto retval = document->getDetails()->get<details::StaffGroup>(getPartId(), BASE_SYSTEM_ID, groupIdRecord->staffGroupId);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("StaffGroup " + std::to_string(groupIdRecord->staffGroupId)
            + " not found for MultiStaffInstrumentGroup " + std::to_string(getCmper()));
    }
    return retval;
}

// ****************
// ***** Note *****
// ****************

std::tuple<Note::NoteName, int, int, int> Note::calcNoteProperties(const std::shared_ptr<KeySignature>& key, ClefIndex clefIndex,
    const std::shared_ptr<const others::Staff>& staff) const
{
    static constexpr std::array<Note::NoteName, music_theory::STANDARD_DIATONIC_STEPS> noteNames = {
        Note::NoteName::C, Note::NoteName::D, Note::NoteName::E, Note::NoteName::F, Note::NoteName::G, Note::NoteName::A, Note::NoteName::B
    };

    int transposedLev = harmLev;
    int transposedAlt = harmAlt;
    if (staff && staff->transposition && staff->transposition->chromatic) {
        const auto& chromatic = *staff->transposition->chromatic;
        auto transposer = key->createTransposer(transposedLev, transposedAlt);
        transposer->chromaticTranspose(chromatic.diatonic, chromatic.alteration);
        transposedLev = transposer->displacement();
        transposedAlt = transposer->alteration();
    }

    // Determine the base note and octave
    int keyAdjustedLev = key->calcTonalCenterIndex() + transposedLev + (key->getOctaveDisplacement() * music_theory::STANDARD_DIATONIC_STEPS);
    int octave = (keyAdjustedLev / music_theory::STANDARD_DIATONIC_STEPS) + 4; // Middle C (C4) is the reference
    int step = keyAdjustedLev % music_theory::STANDARD_DIATONIC_STEPS;
    if (step < 0) {
        step += music_theory::STANDARD_DIATONIC_STEPS;
        octave -= 1;
    }

    // Calculate the actual alteration
    int actualAlteration = transposedAlt + key->calcAlterationOnNote(step);

    // Calculate the staff line
    const auto& clefOptions = getDocument()->getOptions()->get<options::ClefOptions>();
    if (!clefOptions) {
        throw std::invalid_argument("Document contains no clef options!");
    }
    if (clefIndex >= clefOptions->clefDefs.size()) {
        throw std::invalid_argument("Clef index " + std::to_string(clefIndex) + " does not exist in document.`");
    }
    int middleCLine = clefOptions->clefDefs[clefIndex]->middleCPos;

    return { noteNames[step], octave, actualAlteration, keyAdjustedLev + middleCLine };
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
        auto [srcPitch, srcOctave, srcAlter, srcStaffPos] = calcNoteProperties();
        for (auto note = NoteInfoPtr(entry, 0); note; note = note.getNext()) {
            auto [pitch, octave, alter, staffPos] = note.calcNoteProperties();
            if (srcPitch == pitch && srcOctave == octave && srcAlter == alter) {
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

NoteInfoPtr NoteInfoPtr::calcTieFrom() const
{
    // grace notes cannot tie backwards; only forwards (see grace note comment above)
    auto thisRawEntry = m_entry->getEntry();
    if (thisRawEntry->isNote && !thisRawEntry->graceNote) {
        for (auto currEntry = m_entry.getPreviousInLayer(); currEntry; currEntry = currEntry.getPreviousInLayer()) {
            if (currEntry->v2Launch && m_entry.isSameEntry(currEntry.getNextInFrame())) {
                continue;
            }
            auto currRawEntry = currEntry->getEntry();
            if (auto result = findEqualPitch(currEntry)) {
                return result;
            }
            if (currRawEntry->graceNote) {
                continue;
            }
            bool skipBackToV1 = !thisRawEntry->voice2
                              || (currRawEntry->voice2 && currEntry.getPreviousInFrame()->v2Launch);
            if (skipBackToV1 && currRawEntry->voice2) {
                while (currEntry) {
                    auto testEntry = currEntry.getPreviousInLayer();
                    if (!testEntry || !testEntry->getEntry()->voice2) {
                        break;
                    }
                    currEntry = testEntry;
                }
            } else {
                break;
            }
        }
    }
    return NoteInfoPtr();
}

InstCmper NoteInfoPtr::calcStaff() const
{
    if ((*this)->crossStaff) {
        if (auto crossStaff = m_entry->getEntry()->getDocument()->getDetails()->getForNote<details::CrossStaff>(*this)) {
            return crossStaff->staff;
        }
    }
    return m_entry.getStaff();
}

std::tuple<Note::NoteName, int, int, int> NoteInfoPtr::calcNoteProperties() const
{
    ClefIndex clefIndex = [&]() {
        if ((*this)->crossStaff) {
            InstCmper staffId = calcStaff();
            if (staffId != m_entry.getStaff()) {
                if (auto staff = m_entry.createCurrentStaff(staffId)) {
                    return staff->calcClefIndexAt(m_entry.getMeasure(), m_entry->elapsedDuration.calcEduDuration());
                }
            }
        }
        return m_entry->clefIndex;
    }();

    if (getEntryInfo().getFrame()->isForWrittenPitch()) {
        return (*this)->calcNoteProperties(m_entry.getKeySignature(), clefIndex, m_entry.createCurrentStaff());
    }

    return (*this)->calcNoteProperties(m_entry.getKeySignature(), clefIndex);
}

std::unique_ptr<music_theory::Transposer> NoteInfoPtr::createTransposer() const
{
    return m_entry.getKeySignature()->createTransposer((*this)->harmLev, (*this)->harmAlt);
}

// *****************************
// ***** PageFormatOptions *****
// *****************************

std::shared_ptr<options::PageFormatOptions::PageFormat> options::PageFormatOptions::calcPageFormatForPart(Cmper partId) const
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
// ***** PartDefinition *****
// **************************

std::string others::PartDefinition::getName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (nameId) {
        return TextBlock::getText(getDocument(), nameId, true, accidentalStyle); // true: trim tags
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

std::shared_ptr<others::PartDefinition> others::PartDefinition::getScore(const DocumentPtr& document)
{
    if (auto score = document->getOthers()->get<others::PartDefinition>(SCORE_PARTID, SCORE_PARTID)) {
        return score;
    }
    MUSX_INTEGRITY_ERROR("The document contains no instance of PartDefinition for the score.");
    return nullptr;
}

std::vector<std::shared_ptr<others::PartDefinition>> others::PartDefinition::getInUserOrder(const DocumentPtr& document)
{
    auto result = document->getOthers()->getArray<others::PartDefinition>(SCORE_PARTID);
    std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->partOrder < rhs->partOrder;
    });
    return result;
}

// *****************************
// ***** RepeatEndingStart *****
// *****************************

int others::RepeatEndingStart::calcEndingLength() const
{
    int maxLength = std::numeric_limits<int>::max();

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

// ********************
// ***** ShapeDef *****
// ********************

void others::ShapeDef::iterateInstructions(std::function<bool(others::ShapeDef::InstructionType, std::vector<int>)> callback) const
{
    auto insts = getDocument()->getOthers()->get<others::ShapeInstructionList>(getPartId(), instructionList);
    auto data = getDocument()->getOthers()->get<others::ShapeData>(getPartId(), dataList);
    if (insts && data) {
        size_t currentDataIndex = 0;
        for (const auto& inst : insts->instructions) {
            if (currentDataIndex + inst->numData > data->values.size()) {
                throw std::invalid_argument("ShapeDef " + std::to_string(getCmper()) + " does not have enough data for instructions.");
            }
            if (!callback(inst->type, { data->values.begin() + currentDataIndex, data->values.begin() + currentDataIndex + inst->numData })) {
                break;
            }
            currentDataIndex += inst->numData;
        }
    } else {
        MUSX_INTEGRITY_ERROR("ShapeDef " + std::to_string(getCmper()) + " is missing instructions and/or data.");
    }
}

// **********************
// ***** SmartShape *****
// **********************

EntryInfoPtr others::SmartShape::EndPoint::calcAssociatedEntry() const
{
    EntryInfoPtr result;
    if (auto gfhold = getDocument()->getDetails()->get<details::GFrameHold>(getPartId(), staffId, measId)) {
        gfhold->iterateEntries([&](const EntryInfoPtr& entryInfo) {
            if (!entryNumber) {
                unsigned eduDiff = static_cast<unsigned>(std::labs(eduPosition - entryInfo->elapsedDuration.calcEduDuration()));
                if (eduDiff <= 1) {
                    result = entryInfo;
                    return false; // stop iterating
                }
            } else if (entryInfo->getEntry()->getEntryNumber() == entryNumber) {
                result = entryInfo;
                return false; // stop iterating
            }
            return true;
        });
    }
    if (!result && entryNumber != 0) {
        MUSX_INTEGRITY_ERROR("SmartShape at Staff " + std::to_string(staffId) + " Measure " + std::to_string(measId)
            + " contains endpoint with invalid entry number " + std::to_string(entryNumber));
    }
    return result;
}

Edu others::SmartShape::EndPoint::calcEduPosition() const
{
    if (!entryNumber) return eduPosition;
    if (auto entryInfo = calcAssociatedEntry()) {
        return entryInfo->elapsedDuration.calcEduDuration();
    }
    return 0;
}

bool others::SmartShape::calcAppliesTo(const EntryInfoPtr& entryInfo) const
{
    auto entry = entryInfo->getEntry();
    if (entryBased) {
        if (entry->getEntryNumber() == startTermSeg->endPoint->entryNumber) return true;
        if (entry->getEntryNumber() == endTermSeg->endPoint->entryNumber) return true;
    }
    if (entryInfo.getStaff() != startTermSeg->endPoint->staffId && entryInfo.getStaff() != endTermSeg->endPoint->staffId) {
        return false;
    }
    if (auto meas = entry->getDocument()->getOthers()->get<others::Measure>(entry->getPartId(), entryInfo.getMeasure())) {
        if (meas->hasSmartShape) {
            auto shapeAssigns = entry->getDocument()->getOthers()->getArray<others::SmartShapeMeasureAssign>(entry->getPartId(), entryInfo.getMeasure());
            for (const auto& asgn : shapeAssigns) {
                if (asgn->shapeNum == getCmper()) {
                    if (entryInfo.getMeasure() > startTermSeg->endPoint->measId && entryInfo.getMeasure() < endTermSeg->endPoint->measId) {
                        return true;
                    } else if (entryInfo.getMeasure() == startTermSeg->endPoint->measId && entryInfo.getMeasure() == endTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration.calcEduDuration() >= startTermSeg->endPoint->calcEduPosition()
                               && entryInfo->elapsedDuration.calcEduDuration() <= endTermSeg->endPoint->calcEduPosition();
                    } else if (entryInfo.getMeasure() == startTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration.calcEduDuration() >= startTermSeg->endPoint->calcEduPosition();
                    } else if (entryInfo.getMeasure() == endTermSeg->endPoint->measId) {
                        return entryInfo->elapsedDuration.calcEduDuration() <= endTermSeg->endPoint->calcEduPosition();
                    }
                }
            }
        }
    }
    return false;
}

// *****************
// ***** Staff *****
// *****************

void others::Staff::calcAutoNumberValues(const DocumentPtr& document)
{
    auto scrollViewList = document->getOthers()->getArray<others::InstrumentUsed>(SCORE_PARTID, BASE_SYSTEM_ID);

    // Map to track counts for instUuid
    std::unordered_map<std::string, int> instUuidCounts;
    std::unordered_set<Cmper> countedMultistaffGroups;

    // Pass 1: Check if any instUuid has auto-numbering disabled
    std::unordered_set<std::string> disabledInstUuids;
    for (const auto& instrumentUsed : scrollViewList) {
        auto staff = instrumentUsed->getStaff();
        if (staff && !staff->useAutoNumbering) {
            disabledInstUuids.insert(staff->instUuid);
        }
    }

    // Pass 2: Count occurrences of instUuid, considering multistaff instruments
    for (const auto& instrumentUsed : scrollViewList) {
        auto staff = instrumentUsed->getStaff();
        if (!staff || staff->instUuid.empty() || disabledInstUuids.count(staff->instUuid)) {
            continue;
        }

        // Check if the staff is part of a multistaff instrument
        auto multiStaffGroup = staff->getMultiStaffInstGroup();
        if (multiStaffGroup) {
            if (countedMultistaffGroups.find(multiStaffGroup->getCmper()) != countedMultistaffGroups.end()) {
                continue; // Skip already-counted multistaff groups
            }
            countedMultistaffGroups.insert(multiStaffGroup->getCmper());
        }
        instUuidCounts[staff->instUuid]++;
    }

    // Pass 2.1: Remove singleton or empty instUuid counts, including single multistaff instruments
    for (const auto& count : instUuidCounts) {
        if (count.second <= 1) {
            disabledInstUuids.insert(count.first);
        }
    }

    // Pass 3: Assign auto-numbering values
    std::unordered_map<std::string, int> instUuidNumbers;
    countedMultistaffGroups.clear(); // Reset for numbering

    for (const auto& instrumentUsed : scrollViewList) {
        auto staff = instrumentUsed->getStaff();
        if (!staff) continue;
        if (staff->instUuid.empty() || disabledInstUuids.count(staff->instUuid)) {
            staff->autoNumberValue = std::nullopt; // No numbering for disabled or empty instUuid
            continue;
        }

        // Check if the staff is part of a multistaff instrument
        auto multiStaffGroup = staff->getMultiStaffInstGroup();
        if (multiStaffGroup) {
            if (countedMultistaffGroups.find(multiStaffGroup->getCmper()) == countedMultistaffGroups.end()) {
                // Assign a number for this multistaff group
                countedMultistaffGroups.insert(multiStaffGroup->getCmper());
                instUuidNumbers[staff->instUuid]++;
            }
            // Assign the same number to all staves in the group
            auto groupNumber = instUuidNumbers[staff->instUuid];
            for (size_t x = 0; x < multiStaffGroup->staffNums.size(); x++) {
                auto groupStaff = multiStaffGroup->getStaffAtIndex(x);
                if (groupStaff) {
                    groupStaff->autoNumberValue = groupNumber;
                }
            }
            continue; // Skip further processing for the current staff
        }

        // Assign a number for single staves
        instUuidNumbers[staff->instUuid]++;
        staff->autoNumberValue = instUuidNumbers[staff->instUuid];
    }
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
static std::string intToRoman(int num)
{
    if (num <= 0 || num > 3999) {
        throw std::out_of_range("Number out of range for Roman numerals (1-3999)");
    }

    const std::vector<std::pair<int, std::string>> romanMap = {
        {1000, "M"}, {900, "CM"}, {500, "D"}, {400, "CD"},
        {100, "C"}, {90, "XC"}, {50, "L"}, {40, "XL"},
        {10, "X"}, {9, "IX"}, {5, "V"}, {4, "IV"}, {1, "I"}
    };

    std::string result;
    for (const auto& [value, symbol] : romanMap) {
        while (num >= value) {
            result += symbol;
            num -= value;
        }
    }
    return result;
}

std::string intToAlphabetic(int num) {
    if (num <= 0) {
        throw std::out_of_range("Number must be greater than 0");
    }

    std::string result;
    while (num > 0) {
        --num; // Convert to 0-based index
        result.insert(result.begin(), 'A' + (num % 26));
        num /= 26;
    }

    return result;
}

std::string ordinalPrefix(int num)
{
    if (num <= 0) {
        throw std::out_of_range("Number must be greater than 0");
    }

    int lastTwoDigits = num % 100;
    int lastDigit = num % 10;

    if (lastTwoDigits >= 11 && lastTwoDigits <= 13) {
        return std::to_string(num) + "th";
    }

    switch (lastDigit) {
        case 1: return std::to_string(num) + "st";
        case 2: return std::to_string(num) + "nd";
        case 3: return std::to_string(num) + "rd";
        default: return std::to_string(num) + "th";
    }
}
#endif // DOXYGEN_SHOULD_IGNORE_THIS

std::string others::Staff::addAutoNumbering(const std::string& plainName) const
{
    if (!autoNumberValue.has_value()) {
        return plainName; // No numbering needed
    }

    int number = *autoNumberValue;
    switch (autoNumbering) {
        default:
        case AutoNumberingStyle::ArabicSuffix:
            return plainName + " " + std::to_string(number);
        case AutoNumberingStyle::RomanSuffix:
            return plainName + " " + intToRoman(number);
        case AutoNumberingStyle::OrdinalPrefix:
            return ordinalPrefix(number) + " " + plainName;
        case AutoNumberingStyle::AlphaSuffix:
            return plainName + " " + intToAlphabetic(number);
        case AutoNumberingStyle::ArabicPrefix:
            return std::to_string(number) + ". " + plainName;
    }
}

std::string others::Staff::getFullName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), fullNameTextId, true, accidentalStyle); // true: strip enigma tags
}

std::string others::Staff::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), abbrvNameTextId, true, accidentalStyle); // true: strip enigma tags
}

std::shared_ptr<others::MultiStaffInstrumentGroup> others::Staff::getMultiStaffInstGroup() const
{
    if (multiStaffInstId) {
        if (auto retval = getDocument()->getOthers()->get<others::MultiStaffInstrumentGroup>(SCORE_PARTID, multiStaffInstId)) {
            return retval;
        }
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(getCmper()) + " points to non-existent MultiStaffInstrumentGroup " + std::to_string(multiStaffInstId));
    }
    return nullptr;
}

std::string others::Staff::getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle, bool preferStaffName) const
{
    auto name = [&]() -> std::string {
        if (!preferStaffName || !fullNameTextId) {
            if (auto multiInstGroup = getMultiStaffInstGroup()) {
                if (auto group = multiInstGroup->getStaffGroup()) {
                    return group->getFullName(accidentalStyle);
                }
            }
        }
        return getFullName(accidentalStyle);
    }();
    if (name.empty()) return name;
    return addAutoNumbering(name);
}

std::string others::Staff::getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle, bool preferStaffName) const
{
    auto name = [&]() -> std::string {
        if (!preferStaffName || !abbrvNameTextId) {
            if (auto multiInstGroup = getMultiStaffInstGroup()) {
                if (auto group = multiInstGroup->getStaffGroup()) {
                    return group->getAbbreviatedName(accidentalStyle);
                }
            }
        }
        return getAbbreviatedName(accidentalStyle);
    }();
    if (name.empty()) return name;
    return addAutoNumbering(name);
}

ClefIndex others::Staff::calcClefIndexAt(MeasCmper measureId, Edu position) const
{
    for (MeasCmper tryMeasure = measureId; tryMeasure > 0; tryMeasure--) {
        if (auto gfhold = getDocument()->getDetails()->get<details::GFrameHold>(getPartId(), getCmper(), tryMeasure)) {
            return gfhold->calcClefIndexAt(position);
        }
        // after the first iteration, we are looking for the clef at the end of the measure
        position = std::numeric_limits<Edu>::max();
    }
    return defaultClef;
}

ClefIndex others::Staff::calcFirstClefIndex(const DocumentPtr& document, Cmper partId, InstCmper staffCmper)
{
    if (auto staff = others::StaffComposite::createCurrent(document, partId, staffCmper, 1, 0)) {
        return staff->calcFirstClefIndex();
    } else {
        throw std::logic_error("Unable to find StaffComposite instance for staff " + std::to_string(staffCmper));
    }
}

int others::Staff::calcMiddleStaffPosition() const
{
    if (staffLines.has_value()) {
        return -(staffLines.value() - 1);
    } else if (customStaff.has_value()) {
        const auto& lines = customStaff.value();
        if (!lines.empty()) {
            int numStaves = lines[lines.size() - 1] - lines[0] + 1;
            int referenceOffset = 2 * (11 - lines[0]);
            return referenceOffset - (numStaves - 1);
        }
    }
    return 0;
}

bool others::Staff::hasInstrumentAssigned() const
{
    if (instUuid.empty() || instUuid == uuid::BlankStaff || instUuid == uuid::Unknown) {
        return false;
    }
    return true;
}

// **************************
// ***** StaffComposite *****
// **************************

void others::StaffComposite::applyStyle(const std::shared_ptr<others::StaffStyle>& staffStyle)
{
    auto srcMasks = staffStyle->masks;

    if (staffStyle->hasInstrumentAssigned()) {
        instUuid = staffStyle->instUuid;
        // there appears to be no mask for instUuid.
    }

    /// @todo the rest of the masks as we discover/create them
    if (srcMasks->defaultClef) {
        defaultClef = staffStyle->defaultClef;
        masks->defaultClef = true;
    }
    if (srcMasks->floatKeys) {
        floatKeys = staffStyle->floatKeys;
        masks->floatKeys = true;
    }
    if (srcMasks->floatTime) {
        floatTime = staffStyle->floatTime;
        masks->floatTime = true;
    }
    if (srcMasks->staffType) {
        staffLines = staffStyle->staffLines;
        customStaff = staffStyle->customStaff;
        lineSpace = staffStyle->lineSpace;
        topBarlineOffset = staffStyle->topBarlineOffset;
        botBarlineOffset = staffStyle->botBarlineOffset;
        dwRestOffset = staffStyle->dwRestOffset;
        wRestOffset = staffStyle->wRestOffset;
        hRestOffset = staffStyle->hRestOffset;
        otherRestOffset = staffStyle->otherRestOffset;
        botRepeatDotOff = staffStyle->botRepeatDotOff;
        topRepeatDotOff = staffStyle->topRepeatDotOff;
        stemReversal = staffStyle->stemReversal;
        // hideTopRepeatDot
        // hideBotRepeatDot
        masks->staffType = true;
    }
    if (srcMasks->transposition) {
        transposedClef = staffStyle->transposedClef;
        transposition = staffStyle->transposition;
        masks->transposition = true;
    }
    if (srcMasks->hideKeySigsShowAccis) {
        hideKeySigsShowAccis = staffStyle->hideKeySigsShowAccis;
        masks->hideKeySigsShowAccis = true;
    }
    if (srcMasks->negNameScore) {
        hideNameInScore = staffStyle->hideNameInScore;
        masks->negNameScore = true;
    }
    if (srcMasks->fullName) {
        fullNameTextId = staffStyle->fullNameTextId;
        masks->fullName = true;
    }
    if (srcMasks->abrvName) {
        abbrvNameTextId = staffStyle->abbrvNameTextId;
        masks->abrvName = true;
    }
    if (srcMasks->showStems) {
        hideStems = staffStyle->hideStems;
        hideBeams = staffStyle->hideBeams;
        stemDirection = staffStyle->stemDirection;
        // stemsFixedStart
        // stemdFixedEnd
        // stemStartFromStaff
        // horzStemOffUp
        // horzStemOffDown
        // vertStemStartOffUp
        // vertStemStartOffDown
        // vertStemEndOffUp
        // vertStemEndOffDown
        masks->showStems = true;
    }
    if (srcMasks->showNameParts) {
        showNameInParts = staffStyle->showNameInParts;
        masks->showNameParts = true;
    }
}

std::shared_ptr<others::StaffComposite> others::StaffComposite::createCurrent(const DocumentPtr& document, Cmper partId,
    InstCmper staffId, MeasCmper measId, Edu eduPosition)
{
    auto rawStaff = document->getOthers()->get<others::Staff>(partId, staffId);
    if (!rawStaff) return nullptr;

    std::shared_ptr<others::StaffComposite> result(new others::StaffComposite(rawStaff));
    if (result->hasStyles) {
        auto styles = others::StaffStyle::findAllOverlappingStyles(document, partId, staffId, measId, eduPosition);
        for (const auto& style : styles) {
            result->applyStyle(style);
        }
    }

    return result;
}

// **********************
// ***** StaffGroup *****
// **********************

std::string details::StaffGroup::getFullName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), fullNameId, true, accidentalStyle); // true: strip enigma tags
}

std::string details::StaffGroup::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), abbrvNameId, true, accidentalStyle); // true: strip enigma tags
}

std::shared_ptr<others::MultiStaffInstrumentGroup> details::StaffGroup::getMultiStaffInstGroup() const
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
        if (auto staff = multiStaffGroup->getFirstStaff()) {
            return staff->getFullInstrumentName(accidentalStyle);
        }
    }
    return getFullName(accidentalStyle);
}

std::string details::StaffGroup::getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto multiStaffGroup = getMultiStaffInstGroup()) {
        if (auto staff = multiStaffGroup->getFirstStaff()) {
            return staff->getAbbreviatedInstrumentName(accidentalStyle);
        }
    }
    return getAbbreviatedName(accidentalStyle);
}

// **************************
// ***** StaffGroupInfo *****
// **************************

details::StaffGroupInfo::StaffGroupInfo(const std::shared_ptr<StaffGroup>& inp,
    const std::vector<std::shared_ptr<others::InstrumentUsed>>& systemStaves) : group(inp)
{
    if (systemStaves.empty()) {
        throw std::logic_error("Attempt to create StaffGroupInfo with no system staves (StaffGroup " + std::to_string(inp->getCmper2()) + ")");
    }
    for (size_t x = 0; x < systemStaves.size(); x++) {
        if (inp->staves.find(systemStaves[x]->staffId) != inp->staves.end()) {
            startSlot = x;
            break;
        }
    }
    if (startSlot) {
        for (size_t x = systemStaves.size() - 1; x >= *startSlot; x--) {
            if (inp->staves.find(systemStaves[x]->staffId) != inp->staves.end()) {
                endSlot = x;
                break;
            }
        }
    }
}

std::vector<details::StaffGroupInfo> details::StaffGroupInfo::getGroupsAtMeasure(MeasCmper measureId,
    const std::shared_ptr<others::PartDefinition>& linkedPart,
    const std::vector<std::shared_ptr<others::InstrumentUsed>>& systemStaves)
{
    auto rawGroups = linkedPart->getDocument()->getDetails()->getArray<details::StaffGroup>(linkedPart->getCmper(), BASE_SYSTEM_ID);
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

 std::vector<std::shared_ptr<others::StaffStyle>> others::StaffStyle::findAllOverlappingStyles(const DocumentPtr& document,
        Cmper partId, InstCmper staffId, MeasCmper measId, Edu eduPosition)
{
    auto staffStyleAssignments = document->getOthers()->getArray<others::StaffStyleAssign>(partId, staffId);
    std::vector<std::shared_ptr<others::StaffStyleAssign>> applicableAssignments;
    std::copy_if(staffStyleAssignments.begin(), staffStyleAssignments.end(), std::back_inserter(applicableAssignments),
        [measId, eduPosition](const std::shared_ptr<others::StaffStyleAssign>& range) {
            return range->contains(measId, eduPosition);
        });

    std::vector<std::shared_ptr<others::StaffStyle>> result;
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

std::shared_ptr<others::StaffStyle> others::StaffStyleAssign::getStaffStyle() const
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

// ********************
// ***** TextBase *****
// ********************

std::shared_ptr<FontInfo> TextsBase::parseFirstFontInfo() const
{
    std::string searchText = this->text;
    auto fontInfo = std::make_shared<FontInfo>(this->getDocument());
    bool foundTag = false;

    while (true) {
        if (!musx::util::EnigmaString::startsWithFontCommand(searchText)) {
            break;
        }

        size_t endOfTag = searchText.find_first_of(')');
        if (endOfTag == std::string::npos) {
            break;
        }

        std::string fontTag = searchText.substr(0, endOfTag + 1);
        if (!musx::util::EnigmaString::parseFontCommand(fontTag, *fontInfo.get())) {
            return nullptr;
        }

        searchText.erase(0, endOfTag + 1);
        foundTag = true;
    }

    if (foundTag) {
        return fontInfo;
    }

    return nullptr;
}

// *********************
// ***** TextBlock *****
// *********************

std::shared_ptr<TextsBase> others::TextBlock::getRawTextBlock() const
{
    switch (textType) {
        default:
        case TextType::Block:
            return getDocument()->getTexts()->get<texts::BlockText>(textId);
        case TextType::Expression:
            return getDocument()->getTexts()->get<texts::ExpressionText>(textId);        
    }    
}

std::string others::TextBlock::getText(bool trimTags, util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    auto block = getRawTextBlock();
    if (!block) return {};
    auto retval = musx::util::EnigmaString::replaceAccidentalTags(block->text, accidentalStyle);
    if (trimTags) {
        return musx::util::EnigmaString::trimTags(retval);
    }
    return retval;
}

std::string others::TextBlock::getText(const DocumentPtr& document, const Cmper textId, bool trimTags, util::EnigmaString::AccidentalStyle accidentalStyle)
{
    auto textBlock = document->getOthers()->get<others::TextBlock>(SCORE_PARTID, textId);
    if (textBlock) {
        return textBlock->getText(trimTags, accidentalStyle);
    }
    return {};
}

// *****************************
// ***** TextExpressionDef *****
// *****************************

std::shared_ptr<others::TextBlock> others::TextExpressionDef::getTextBlock() const
{
    return getDocument()->getOthers()->get<others::TextBlock>(getPartId(), textIdKey);
}

std::shared_ptr<others::Enclosure> others::TextExpressionDef::getEnclosure() const
{
    if (!hasEnclosure) return nullptr;
    return getDocument()->getOthers()->get<others::TextExpressionEnclosure>(getPartId(), getCmper());
}

// *************************
// ***** TimeSignature *****
// *************************

TimeSignature::TimeSignature(const DocumentWeakPtr& document, int beats, Edu unit, bool hasCompositeTop, bool hasCompositeBottom, std::optional<bool> abbreviate)
    : CommonClassBase(document), m_abbreviate(abbreviate)
{
    auto tops = [&]() -> std::vector<std::vector<util::Fraction>> {
        if (hasCompositeTop) {
            if (auto comps = getDocument()->getOthers()->get<others::TimeCompositeUpper>(SCORE_PARTID, beats)) {
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
            if (auto comps = getDocument()->getOthers()->get<others::TimeCompositeLower>(SCORE_PARTID, unit)) {
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
    for (size_t x = 0; x < std::min(tops.size(), bots.size()); x++) {
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
