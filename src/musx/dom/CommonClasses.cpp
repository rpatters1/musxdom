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

// ********************
// ***** FontInfo *****
// ********************

std::string FontInfo::getName() const
{
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(SCORE_PARTID, fontId)) {
        return fontDef->name;
    }
    throw std::invalid_argument("font definition not found for font id " + std::to_string(fontId));
}

void FontInfo::setFontIdByName(const std::string& name)
{
    auto fontDefs = getDocument()->getOthers()->getArray<others::FontDefinition>(SCORE_PARTID);
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
    if (auto fontDef = getDocument()->getOthers()->get<others::FontDefinition>(SCORE_PARTID, fontId)) {
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
        } else if (!envVariable.empty()) {
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
            } else {
                return {};
            }
        } else {
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

// ************************
// ***** KeySignature *****
// ************************

std::vector<unsigned> KeySignature::calcTonalCenterArrayForSharps() const
{
    if (isMinor()) {
        return { 5, 2, 6, 3, 0, 4, 1, 5 };
    }
    if (!isBuiltIn()) {
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterSharps>(SCORE_PARTID, getKeyMode())) {
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
        if (auto centers = getDocument()->getOthers()->get<others::TonalCenterFlats>(SCORE_PARTID, getKeyMode())) {
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
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountSharps>(SCORE_PARTID, getKeyMode())) {
                return amounts->values;
            }
        } else {
            if (auto amounts = getDocument()->getOthers()->get<others::AcciAmountFlats>(SCORE_PARTID, getKeyMode())) {
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
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderSharps>(SCORE_PARTID, getKeyMode())) {
                return order->values;
            }
        } else {
            if (auto order = getDocument()->getOthers()->get<others::AcciOrderFlats>(SCORE_PARTID, getKeyMode())) {
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

int KeySignature::calcScaleDegree(int displacement) const
{
    const int diatonicSteps = [&]() {
        if (const auto keyMap = calcKeyMap()) {
            return int(keyMap.value().size());
        }
        return music_theory::STANDARD_DIATONIC_STEPS;
    }();
    return music_theory::positiveModulus(displacement, diatonicSteps);
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
    auto keyMap = getDocument()->getOthers()->get<others::KeyMapArray>(SCORE_PARTID, getKeyMode());
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
    if (auto keyFormat = getDocument()->getOthers()->get<others::KeyFormat>(SCORE_PARTID, getKeyMode())) {
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

// **********************
// ***** MusicRange *****
// **********************

std::optional<std::pair<MeasCmper, Edu>> MusicRange::nextLocation(const std::optional<StaffCmper>& forStaff) const
{
    std::optional<std::pair<MeasCmper, Edu>> result;
    const Edu endEdu = endPosition.calcEduDuration();
    if (auto currMeasure = getDocument()->getOthers()->get<others::Measure>(SCORE_PARTID, endMeasureId)) {
        MeasCmper nextMeas = endMeasureId;
        const Edu maxEdu = currMeasure->calcDuration(forStaff).calcEduDuration() - 1;
        Edu nextEdu = 0;
        if (endEdu < maxEdu) {
            nextEdu = endEdu + 1;
        } else {
            nextMeas++;
            if (!getDocument()->getOthers()->get<others::Measure>(SCORE_PARTID, nextMeas)) {
                return std::nullopt;
            }
        }
        result = std::make_pair(nextMeas, nextEdu);
    } else {
        MUSX_INTEGRITY_ERROR("MusicRange has invalid end measure " + std::to_string(endMeasureId));
    }
    return result;
}

bool MusicRange::contains(const EntryInfoPtr& entryInfo) const
{
    return contains(entryInfo.getMeasure(), entryInfo->elapsedDuration);
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

bool TimeSignature::isSame(const TimeSignature& src) const
{
    if (components != src.components) {
        return false;
    }
    if (isCutTime() || isCommonTime()) {
        return getAbbreviatedSymbol() == src.getAbbreviatedSymbol();
    }
    return true;
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
    if (!finalUnit) {
        throw std::logic_error("The beat size is zero.");
    }

    // Compute final beats relative to finalUnit
    auto totalBeats = std::accumulate(summedUnits.begin(), summedUnits.end(), util::Fraction{}, [finalUnit](auto acc, const auto& p) {
        return acc + p.first * (p.second / finalUnit);
    });

    int power2 = 0;
    int otherPrimes = finalUnit;
    while ((otherPrimes & 0x01) == 0) {
        otherPrimes >>= 1;
        power2++;
    }

    return { totalBeats * otherPrimes, NoteType(1 << power2) };
}

util::Fraction TimeSignature::calcBeatValueAt(Edu eduPosition) const
{
    if (components.empty()) {
        return {};
    }

    const util::Fraction pos = util::Fraction::fromEdu(eduPosition); // fraction of whole note
    util::Fraction cursor{};
    util::Fraction lastBeatValue{};

    for (const TimeSigComponent& comp : components) {
        MUSX_ASSERT_IF(comp.units.empty() || comp.counts.empty()) {
            // detect this in debug mode, but handle without throw in production.
            continue;
        }

        const Edu beatEdu = comp.sumUnits();
        const util::Fraction beatCount = comp.sumCounts();

        MUSX_ASSERT_IF(beatEdu <= 0 || beatCount <= util::Fraction{}) {
            // detect this in debug mode, but handle without throw in production.
            continue;
        }

        lastBeatValue = util::Fraction::fromEdu(beatEdu);
        const util::Fraction compDur = beatCount * lastBeatValue;

        if (pos >= cursor && pos < (cursor + compDur)) {
            return lastBeatValue;
        }

        cursor += compDur;
    }

    return lastBeatValue;
}

} // namespace dom
} // namespace musx
