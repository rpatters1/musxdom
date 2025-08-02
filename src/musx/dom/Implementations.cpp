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

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template bool details::BeamAlterations::calcIsFeatheredBeamImpl<details::SecondaryBeamAlterationsUpStem>(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY);
template bool details::BeamAlterations::calcIsFeatheredBeamImpl<details::SecondaryBeamAlterationsDownStem>(const EntryInfoPtr& entryInfo, Evpu& outLeftY, Evpu& outRightY);
#endif // DOXYGEN_SHOULD_IGNORE_THIS

// ***********************
// ***** ClefOptions *****
// ***********************

bool options::ClefOptions::ClefDef::isBlank() const
{
    if (isShape) {
        if (const auto shape = shapeId ? getDocument()->getOthers()->get<others::ShapeDef>(getSourcePartId(), shapeId) : nullptr) {
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
            if (auto shape = getDocument()->getOthers()->get<others::ShapeDef>(getSourcePartId(), shapeId)) {
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
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(getSourcePartId(), fontId)) {
        return fontDef->name;
    }
    throw std::invalid_argument("font definition not found for font id " + std::to_string(fontId));
}

void FontInfo::setFontIdByName(const std::string& name)
{
    auto fontDefs = getDocument()->getOthers()->getArray<others::FontDefinition>(getSourcePartId());
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
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(getSourcePartId(), fontId)) {
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
    MusxInstanceList<Entry> retval(getDocument(), getRequestedPartId());
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
        if (const auto shape = getDocument()->getOthers()->get<others::ShapeDef>(getRequestedPartId(), shapeDef)) {
            return shape->isBlank();
        }
    }
    return true;
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
        if (auto system = getDocument()->getOthers()->get<others::StaffSystem>(getRequestedPartId(), getCmper())) {
            result = system->calcSystemScaling();
            if (auto page = system->getPage()) {
                result *= page->calcPageScaling();
            }
            if (system->hasStaffScaling) {
                if (auto staffSize = getDocument()->getDetails()->get<details::StaffSize>(getRequestedPartId(), getCmper(), staffId)) {
                    result *= staffSize->calcStaffScaling();
                }
            }
        }
    }
    return result;
}

MusxInstance<others::Staff> others::StaffUsed::getStaffInstance() const
{
    auto retval = getDocument()->getOthers()->get<others::Staff>(getRequestedPartId(), staffId);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffId) + " not found for StaffUsed list " + std::to_string(getCmper()));
    }
    return retval;
}

MusxInstance<others::Staff> others::StaffUsed::getStaffInstance(MeasCmper measureId, Edu eduPosition) const
{
    auto retval = others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), staffId, measureId, eduPosition);
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
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterSharps>(getSourcePartId(), getKeyMode())) {
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
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterFlats>(getSourcePartId(), getKeyMode())) {
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
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountSharps>(getSourcePartId(), getKeyMode())) {
                return amounts->values;
            }
        } else {
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountFlats>(getSourcePartId(), getKeyMode())) {
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
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderSharps>(getSourcePartId(), getKeyMode())) {
                return order->values;
            }
        } else {
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderFlats>(getSourcePartId(), getKeyMode())) {
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
    auto keyMap = getDocument()->getOthers()->get<others::KeyMapArray>(getSourcePartId(), getKeyMode());
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
    if (auto keyFormat = getDocument()->getOthers()->get<others::KeyFormat>(getSourcePartId(), getKeyMode())) {
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
        return rawText->getRawTextCtx(rawText, SCORE_PARTID);
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
        syllables.push_back(MusxInstance<LyricsSyllableInfo>(new  LyricsSyllableInfo(getDocument(), current, lastSeparatorHadHyphen, currSeparatorHasHyphen)));
    }
}

// ****************************
// ***** MarkingCategiory *****
// ****************************

std::string others::MarkingCategory::getName() const
{
    auto catName = getDocument()->getOthers()->get<others::MarkingCategoryName>(getRequestedPartId(), getCmper());
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
        staff = others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0);
        if (staff && staff->floatKeys) {
            if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getRequestedPartId(), forStaff.value(), getCmper())) {
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
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatTime) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getRequestedPartId(), forStaff.value(), getCmper())) {
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
        if (auto staff = others::StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0)) {
            if (staff->floatTime) {
                if (auto floats = getDocument()->getDetails()->get<details::IndependentStaffDetails>(getRequestedPartId(), forStaff.value(), getCmper())) {
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
    return getDocument()->getOthers()->get<others::TextExpressionDef>(getRequestedPartId(), textExprId);
}

MusxInstance<others::ShapeExpressionDef> others::MeasureExprAssign::getShapeExpression() const
{
    if (!shapeExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<others::ShapeExpressionDef>(getRequestedPartId(), shapeExprId);
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
        if (auto measure = getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), next)) {
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
    return getDocument()->getOthers()->get<others::TextBlock>(getRequestedPartId(), block);
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
    auto retval = getDocument()->getOthers()->get<others::Staff>(getRequestedPartId(), staffNums[x]);
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
    if (auto currMeasure = getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), endMeas)) {
        MeasCmper nextMeas = endMeas;
        Edu maxEdu = currMeasure->calcDuration(forStaff).calcEduDuration() - 1;
        Edu nextEdu = 0;
        if (endEdu < maxEdu) {
            nextEdu = endEdu + 1;
        } else {
            nextMeas++;
            if (!getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), nextMeas)) {
                return std::nullopt;
            }
        }
        result = std::make_pair(nextMeas, nextEdu);
    } else {
        MUSX_INTEGRITY_ERROR("MusicRange has invalid end measure " + std::to_string(endMeas));
    }
    return result;
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

MusxInstance<options::PageFormatOptions::PageFormat> options::PageFormatOptions::calcPageFormatForPart(Cmper partId) const
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
    return getDocument()->getOthers()->get<others::TextBlock>(getRequestedPartId(), block);
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
        if (auto textBlock = getDocument()->getOthers()->get<others::TextBlock>(getRequestedPartId(), nameId)) {
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
        auto measure = getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), x);
        if (!measure) {
            return 1;
        }
        if (measure->hasEnding && getDocument()->getOthers()->get<others::RepeatEndingStart>(getRequestedPartId(), x)) {
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
        auto measure = getDocument()->getOthers()->get<others::Measure>(getRequestedPartId(), x);
        if (!measure) {
            break;
        }
        if (measure->backwardsRepeatBar) {
            if (auto backRepeat = getDocument()->getOthers()->get<others::RepeatBack>(getRequestedPartId(), x)) {
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
    return others::TextBlock::getText(getDocument(), fullNameId, getRequestedPartId(), true, accidentalStyle); // true: strip enigma tags
}

std::string details::StaffGroup::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return others::TextBlock::getText(getDocument(), abbrvNameId, getRequestedPartId(), true, accidentalStyle); // true: strip enigma tags
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
        if (auto staff = others::StaffComposite::createCurrent(iUsed->getDocument(), iUsed->getRequestedPartId(), iUsed->staffId, measId, eduPosition)) {
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
    auto result = getDocument()->getOthers()->get<others::StaffStyle>(getRequestedPartId(), styleId);
    if (!result) {
        MUSX_INTEGRITY_ERROR("Staff style assignment has invalid staff style ID " + std::to_string(styleId)
            + ": Part " + std::to_string(getRequestedPartId())
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
    return getDocument()->getOthers()->get<others::Page>(getRequestedPartId(), pageId);
}

std::pair<util::Fraction, util::Fraction> others::StaffSystem::calcMinMaxStaffSizes() const
{
    if (hasStaffScaling) {
        auto systemStaves = getDocument()->getOthers()->getArray<others::StaffUsed>(getRequestedPartId(), getCmper());
        if (!systemStaves.empty()) {
            std::pair<util::Fraction, util::Fraction> result = std::make_pair((std::numeric_limits<util::Fraction>::max)(), (std::numeric_limits<util::Fraction>::min)());
            for (const auto& systemStaff : systemStaves) {
                auto staffSize = getDocument()->getDetails()->get<details::StaffSize>(getRequestedPartId(), getCmper(), systemStaff->getCmper());
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
        return rawText->getRawTextCtx(rawText, forPartId, forPageId, defaultInsertFunc);
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
    return getDocument()->getOthers()->get<others::TextBlock>(getRequestedPartId(), textIdKey);
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
    return getDocument()->getOthers()->get<others::TextExpressionEnclosure>(getRequestedPartId(), getCmper());
}

// *********************
// ***** TextsBase *****
// *********************

util::EnigmaParsingContext TextsBase::getRawTextCtx(const MusxInstance<TextsBase>& ptrToThis, Cmper forPartId, std::optional<Cmper> forPageId,
    util::EnigmaString::TextInsertCallback defaultInsertFunc) const
{
    return util::EnigmaParsingContext(ptrToThis, forPartId, forPageId, defaultInsertFunc);
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
