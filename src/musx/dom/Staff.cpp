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
namespace others {

// *****************
// ***** Staff *****
// *****************

void Staff::calcAllAutoNumberValues(const DocumentPtr& document)
{
    auto scrollViewList = document->getOthers()->getArray<InstrumentUsed>(SCORE_PARTID, BASE_SYSTEM_ID);

    // Map to track counts for instUuid
    std::unordered_map<std::string, int> instUuidCounts;
    std::unordered_set<Cmper> countedMultistaffGroups;

    // Pass 1: Check if any instUuid has auto-numbering disabled
    std::unordered_set<std::string> disabledInstUuids;
    for (const auto& instrumentUsed : scrollViewList) {
        auto staff = instrumentUsed->getStaffInstance();
        if (staff && !staff->useAutoNumbering) {
            disabledInstUuids.insert(staff->instUuid);
        }
    }

    // Pass 2: Count occurrences of instUuid, considering multistaff instruments
    for (const auto& instrumentUsed : scrollViewList) {
        auto staff = instrumentUsed->getStaffInstance();
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
        auto staff = instrumentUsed->getStaffInstance();
        Staff* mutableStaff = const_cast<Staff*>(staff.get());
        if (!staff) continue;
        if (staff->instUuid.empty() || disabledInstUuids.count(staff->instUuid)) {
            mutableStaff->autoNumberValue = std::nullopt; // No numbering for disabled or empty instUuid
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
                auto groupStaff = multiStaffGroup->getStaffInstanceAtIndex(x);
                if (groupStaff) {
                    Staff* mutableGroupStaff = const_cast<Staff*>(groupStaff.get());
                    mutableGroupStaff->autoNumberValue = groupNumber;
                }
            }
            continue; // Skip further processing for the current staff
        }

        // Assign a number for single staves
        instUuidNumbers[staff->instUuid]++;
        mutableStaff->autoNumberValue = instUuidNumbers[staff->instUuid];
    }
}
template <typename SubType>
void Staff::calcAllRuntimeValues(const DocumentPtr& document)
{
    static_assert(std::is_same_v<SubType, Staff>
               || std::is_same_v<SubType, StaffStyle>,
        "SubType template parameter must be Staff or StaffStyle.");
    using DrumStaffType = std::conditional_t<std::is_same_v<SubType, Staff>, DrumStaff, DrumStaffStyle>;
    using NamePositionFullType = std::conditional_t<std::is_same_v<SubType, Staff>, NamePositionFull, NamePositionStyleFull>;
    using NamePositionAbrvType = std::conditional_t<std::is_same_v<SubType, Staff>, NamePositionAbbreviated, NamePositionStyleAbbreviated>;
    constexpr bool isForStyle = std::is_same_v<SubType, StaffStyle>;

    auto list = document->getOthers()->getArray<SubType>(SCORE_PARTID);
    for (const auto& item : list) {
        SubType* mutableItem = const_cast<SubType*>(item.get());
        if (item->notationStyle == Staff::NotationStyle::Percussion) {
            if (auto drumStaff = document->getOthers()->get<DrumStaffType>(SCORE_PARTID, item->getCmper())) {
                mutableItem->percussionMapId = drumStaff->whichDrumLib;
            } else {
                mutableItem->percussionMapId = Cmper(0);
                MUSX_INTEGRITY_ERROR("Staff or StaffStyle " + std::to_string(item->getCmper()) + " is percussion style but has no DrumStaff record.");
            }
        } else {
            mutableItem->percussionMapId = std::nullopt;
        }
        bool checkFullNeeded = true;
        if constexpr (isForStyle) {
            checkFullNeeded = item->masks->fullNamePos;
        }
        if (checkFullNeeded) {
            if (auto full = document->getOthers()->get<NamePositionFullType>(SCORE_PARTID, item->getCmper())) {
                mutableItem->fullNamePosId = item->getCmper();
            } else {
                mutableItem->fullNamePosId = 0;
            }
            mutableItem->fullNamePosFromStyle = isForStyle;
        }
        bool checkAbbrvNeeded = true;
        if constexpr (isForStyle) {
            checkAbbrvNeeded = item->masks->abrvNamePos;
        }
        if (checkAbbrvNeeded) {
            if (auto abrv = document->getOthers()->get<NamePositionAbrvType>(SCORE_PARTID, item->getCmper())) {
                mutableItem->abrvNamePosId = item->getCmper();
            } else {
                mutableItem->abrvNamePosId = 0;
            }
            mutableItem->abrvNamePosFromStyle = isForStyle;
        }
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

std::pair<std::string, bool> Staff::calcAutoNumberingAffix() const
{
    if (!autoNumberValue.has_value()) {
        return std::make_pair(std::string(), false); // No numbering needed
    }

    int number = *autoNumberValue;
    switch (autoNumbering) {
        default:
        case AutoNumberingStyle::ArabicSuffix:
            return std::make_pair(" " + std::to_string(number), false);
        case AutoNumberingStyle::RomanSuffix:
            return std::make_pair(" " + intToRoman(number), false);
        case AutoNumberingStyle::OrdinalPrefix:
            return std::make_pair(ordinalPrefix(number) + " ", true);
        case AutoNumberingStyle::AlphaSuffix:
            return std::make_pair(" " + intToAlphabetic(number), false);
        case AutoNumberingStyle::ArabicPrefix:
            return std::make_pair(std::to_string(number) + ". ", true);
    }
}

std::string Staff::addAutoNumbering(const std::string& plainName) const
{
    auto [affix, isPrefix] = calcAutoNumberingAffix();
    if (affix.empty()) {
        return plainName;
    }
    if (isPrefix) {
        return affix + plainName;
    }
    return plainName + affix;
}

std::string Staff::getFullName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return TextBlock::getText(getDocument(), fullNameTextId, SCORE_PARTID, true, accidentalStyle); // true: strip enigma tags
}

std::string Staff::getAbbreviatedName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    return TextBlock::getText(getDocument(), abbrvNameTextId, SCORE_PARTID, true, accidentalStyle); // true: strip enigma tags
}

MusxInstance<MultiStaffInstrumentGroup> Staff::getMultiStaffInstGroup() const
{
    if (multiStaffInstId) {
        if (auto retval = getDocument()->getOthers()->get<MultiStaffInstrumentGroup>(SCORE_PARTID, multiStaffInstId)) {
            return retval;
        }
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(getCmper()) + " points to non-existent MultiStaffInstrumentGroup " + std::to_string(multiStaffInstId));
    }
    return nullptr;
}

MusxInstance<details::StaffGroup> Staff::getMultiStaffInstVisualGroup() const
{
    Cmper groupId = getDocument()->getInstrumentForStaff(getCmper()).staffGroupId;
    if (groupId != 0) {
        if (auto retval = getDocument()->getDetails()->get<details::StaffGroup>(SCORE_PARTID, BASE_SYSTEM_ID, groupId)) {
            return retval;
        } else {
            MUSX_INTEGRITY_ERROR("Instrument map " + std::to_string(getCmper()) + " points to non-existent StaffGroup " + std::to_string(groupId)
                + " for staff " + std::to_string(getCmper()));
        }
    }
    return nullptr;
}

util::EnigmaParsingContext Staff::getFullInstrumentNameCtx(Cmper forPartId, bool preferStaffName) const
{
    auto block = [&]() -> MusxInstance<TextBlock> {
        if (!preferStaffName || !fullNameTextId) {
            if (auto group = getMultiStaffInstVisualGroup()) {
                return getDocument()->getOthers()->get<TextBlock>(forPartId, group->fullNameId);
            }
        }
        return getDocument()->getOthers()->get<TextBlock>(forPartId, fullNameTextId);
    }();
    if (!block) {
        return {};
    }
    auto parsingContext = block->getRawTextCtx(forPartId);
    auto [affix, isPrefix] = calcAutoNumberingAffix();
    parsingContext.affixText = affix;
    parsingContext.affixIsPrefix = isPrefix;
    return parsingContext;
}

std::string Staff::getFullInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle, bool preferStaffName) const
{
    if (auto ctx = getFullInstrumentNameCtx(SCORE_PARTID, preferStaffName)) {
        return ctx.getText(true, accidentalStyle);
    }
    return {};
}

util::EnigmaParsingContext Staff::getAbbreviatedInstrumentNameCtx(Cmper forPartId, bool preferStaffName) const
{
    auto block = [&]() -> MusxInstance<TextBlock> {
        if (!preferStaffName || !abbrvNameTextId) {
            if (auto group = getMultiStaffInstVisualGroup()) {
                return getDocument()->getOthers()->get<TextBlock>(forPartId, group->abbrvNameId);
            }
        }
        return getDocument()->getOthers()->get<TextBlock>(forPartId, abbrvNameTextId);
    }();
    if (!block) {
        return {};
    }
    auto parsingContext = block->getRawTextCtx(forPartId);
    auto [affix, isPrefix] = calcAutoNumberingAffix();
    parsingContext.affixText = affix;
    parsingContext.affixIsPrefix = isPrefix;
    return parsingContext;
}

std::string Staff::getAbbreviatedInstrumentName(util::EnigmaString::AccidentalStyle accidentalStyle, bool preferStaffName) const
{
    if (auto ctx = getAbbreviatedInstrumentNameCtx(SCORE_PARTID, preferStaffName)) {
        return ctx.getText(true, accidentalStyle);
    }
    return {};
}

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template <typename NamePositionType>
MusxInstance<NamePositioning> Staff::getNamePosition() const
{
    static_assert(std::is_same_v<NamePositionType, NamePositionAbbreviated>
               || std::is_same_v<NamePositionType, NamePositionStyleAbbreviated>
               || std::is_same_v<NamePositionType, NamePositionFull>
               || std::is_same_v<NamePositionType, NamePositionStyleFull>,
        "NamePositionType must be a name positioning type.");
    constexpr bool isForFull = std::is_same_v<NamePositionType, NamePositionFull> || std::is_same_v<NamePositionType, NamePositionStyleFull>;

    const Cmper posCmper = isForFull ? fullNamePosId : abrvNamePosId;
    if (posCmper) {
        if (auto pos = getDocument()->getOthers()->get<NamePositionType>(getPartId(), posCmper)) {
            return pos;
        }
    }

    MusxInstance<NamePositioning> defaultValue;
    if (auto staffOptions = getDocument()->getOptions()->get<options::StaffOptions>()) {
        if constexpr (isForFull) {
            defaultValue = staffOptions->namePos;
        } else {
            defaultValue = staffOptions->namePosAbbrv;
        }
    } else {
        MUSX_INTEGRITY_ERROR("Unable to retrieve staff options for returning default name positioning.");
    }
    return defaultValue;
}
#endif // DOXYGEN_SHOULD_IGNORE_THIS

MusxInstance<NamePositioning> Staff::getFullNamePosition() const
{
    if (fullNamePosFromStyle) {
        return getNamePosition<NamePositionStyleFull>();
    }
    return getNamePosition<NamePositionFull>();
}

MusxInstance<NamePositioning> Staff::getAbbreviatedNamePosition() const
{
    if (abrvNamePosFromStyle) {
        return getNamePosition<NamePositionStyleAbbreviated>();
    }
    return getNamePosition<NamePositionAbbreviated>();
}

ClefIndex Staff::calcClefIndexAt(MeasCmper measureId, Edu position, bool forWrittenPitch) const
{
    if (forWrittenPitch && transposition && transposition->setToClef) {
        return transposedClef;
    }
    for (MeasCmper tryMeasure = measureId; tryMeasure > 0; tryMeasure--) {
        if (auto gfhold = details::GFrameHoldContext(getDocument(), getPartId(), getCmper(), tryMeasure)) {
            return gfhold.calcClefIndexAt(position);
        }
        // after the first iteration, we are looking for the clef at the end of the measure
        position = (std::numeric_limits<Edu>::max)();
    }
    return defaultClef;
}

ClefIndex Staff::calcFirstClefIndex(const DocumentPtr& document, Cmper partId, InstCmper staffCmper)
{
    if (auto staff = StaffComposite::createCurrent(document, partId, staffCmper, 1, 0)) {
        return staff->calcFirstClefIndex();
    } else {
        throw std::logic_error("Unable to find StaffComposite instance for staff " + std::to_string(staffCmper));
    }
}

int Staff::calcNumberOfStafflines() const
{
    if (staffLines.has_value()) {
        return staffLines.value();
    } else if (customStaff.has_value()) {
        return int(customStaff.value().size());
    }
    return music_theory::STANDARD_NUMBER_OF_STAFFLINES;
}

int Staff::calcMiddleStaffPosition() const
{
    if (staffLines.has_value()) {
        return -(staffLines.value() - 1);
    } else if (customStaff.has_value()) {
        const auto& lines = customStaff.value();
        if (!lines.empty()) {
            int numLines = lines[lines.size() - 1] - lines[0] + 1;
            int referenceOffset = 2 * (11 - lines[0]);
            return referenceOffset - (numLines - 1);
        }
    }
    return 0;
}

int Staff::calcToplinePosition() const
{
    if (staffLines.has_value()) {
        return 0;
    }
    if (customStaff.has_value()) {
        const auto& lines = customStaff.value();
        if (!lines.empty()) {
            return 2 * (11 - lines[0]);
        }
    }
    return 0;
}


bool Staff::hasInstrumentAssigned() const
{
    if (instUuid.empty() || instUuid == uuid::BlankStaff || instUuid == uuid::Unknown) {
        return false;
    }
    return true;
}

std::pair<int, int> Staff::calcTranspositionInterval() const
{
    if (transposition) {
        if (transposition->chromatic) {
            return std::make_pair(transposition->chromatic->diatonic, transposition->chromatic->alteration);
        } else if (transposition->keysig) {
            const int alteration = music_theory::calcAlterationFromKeySigChange(transposition->keysig->interval, transposition->keysig->adjust);
            return std::make_pair(transposition->keysig->interval, alteration);
        }
    }
    return std::make_pair(0, 0);
}

MusxInstanceList<PartDefinition> Staff::getContainingParts(bool includeScore) const
{
    MusxInstanceList<PartDefinition> result(getDocument(), SCORE_PARTID);
    auto parts = getDocument()->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
    for (const auto& part : parts) {
        if (!includeScore && part->getCmper() == SCORE_PARTID) {
            continue;
        }
        auto scrollView = getDocument()->getOthers()->getArray<InstrumentUsed>(part->getCmper(), BASE_SYSTEM_ID);
        for (const auto& next : scrollView) {
            if (next->staffId == this->getCmper()) {
                result.push_back(part);
                break;
            }
        }
    }
    return result;
}

MusxInstance<PartDefinition> Staff::firstContainingPart() const
{
    auto parts = getDocument()->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
    for (const auto& part : parts) {
        if (part->getCmper() != SCORE_PARTID) {
            auto scrollView = getDocument()->getOthers()->getArray<InstrumentUsed>(part->getCmper(), BASE_SYSTEM_ID);
            for (const auto& next : scrollView) {
                if (next->staffId == this->getCmper()) {
                    return part;
                }
            }
        }
    }
    return nullptr;
}

// **************************
// ***** StaffComposite *****
// **************************

void StaffComposite::applyStyle(const MusxInstance<StaffStyle>& staffStyle)
{
    auto srcMasks = staffStyle->masks;

    if (staffStyle->hasInstrumentAssigned()) {
        instUuid = staffStyle->instUuid;
        // there appears to be no mask for instUuid.
    }

    /// @todo the rest of the masks as we discover/create them
    if (srcMasks->floatNoteheadFont) {
        noteFont = staffStyle->noteFont;
        useNoteFont = staffStyle->useNoteFont;
        masks->floatNoteheadFont = true;
    }
    if (srcMasks->flatBeams) {
        flatBeams = staffStyle->flatBeams;
        masks->flatBeams = true;
    }
    if (srcMasks->blankMeasureRest) {
        blankMeasure = staffStyle->blankMeasure;
        masks->blankMeasureRest = true;
    }
    if (srcMasks->noOptimize) {
        noOptimize = staffStyle->noOptimize;
        masks->noOptimize = true;
    }
    if (srcMasks->notationStyle) {
        notationStyle = staffStyle->notationStyle;
        capoPos = staffStyle->capoPos;
        lowestFret = staffStyle->lowestFret;
        vertTabNumOff = staffStyle->vertTabNumOff;
        showTabClefAllSys = staffStyle->showTabClefAllSys;
        useTabLetters = staffStyle->useTabLetters;
        breakTabLinesAtNotes = staffStyle->breakTabLinesAtNotes;
        hideTuplets = staffStyle->hideTuplets;
        fretInstrumentId = staffStyle->fretInstrumentId;
        percussionMapId = staffStyle->percussionMapId;
        masks->notationStyle = true;
    }
    if (srcMasks->defaultClef) {
        defaultClef = staffStyle->defaultClef;
        masks->defaultClef = true;
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
        hideRepeatBottomDot = staffStyle->hideRepeatBottomDot;
        hideRepeatTopDot = staffStyle->hideRepeatTopDot;
        masks->staffType = true;
    }
    if (srcMasks->transposition) {
        transposedClef = staffStyle->transposedClef;
        transposition = staffStyle->transposition;
        masks->transposition = true;
    }
    if (srcMasks->blineBreak) {
        blineBreak = staffStyle->blineBreak;
        masks->blineBreak = true;
    }
    if (srcMasks->rbarBreak) {
        rbarBreak = staffStyle->rbarBreak;
        masks->rbarBreak = true;
    }
    if (srcMasks->negMnumb) {
        hideMeasNums = staffStyle->hideMeasNums;
        masks->negMnumb = true;
    }
    if (srcMasks->negRepeat) {
        hideRepeats= staffStyle->hideRepeats;
        masks->negRepeat = true;
    }
    if (srcMasks->negNameScore) {
        hideNameInScore = staffStyle->hideNameInScore;
        masks->negNameScore = true;
    }
    if (srcMasks->hideBarlines) {
        hideBarlines = staffStyle->hideBarlines;
        masks->hideBarlines = true;
    }
    if (srcMasks->fullName) {
        fullNameTextId = staffStyle->fullNameTextId;
        masks->fullName = true;
    }
    if (srcMasks->abrvName) {
        abbrvNameTextId = staffStyle->abbrvNameTextId;
        masks->abrvName = true;
    }
    if (srcMasks->floatKeys) {
        floatKeys = staffStyle->floatKeys;
        masks->floatKeys = true;
    }
    if (srcMasks->floatTime) {
        floatTime = staffStyle->floatTime;
        masks->floatTime = true;
    }
    if (srcMasks->hideRptBars) {
        hideRptBars = staffStyle->hideRptBars;
        masks->hideRptBars = true;
    }
    if (srcMasks->fullNamePos) {
        fullNamePosId = staffStyle->fullNamePosId;
        fullNamePosFromStyle = true;
        masks->fullNamePos = true;
    }
    if (srcMasks->abrvNamePos) {
        abrvNamePosId = staffStyle->abrvNamePosId;
        abrvNamePosFromStyle = true;
        masks->abrvNamePos = true;
    }
    if (srcMasks->altNotation) {
        altNotation = staffStyle->altNotation;
        altLayer = staffStyle->altLayer;
        altHideArtics = staffStyle->altHideArtics;
        altHideLyrics = staffStyle->altHideLyrics;
        altHideSmartShapes = staffStyle->altHideSmartShapes;
        altRhythmStemsUp = staffStyle->altRhythmStemsUp;
        altSlashDots = staffStyle->altSlashDots;
        altHideOtherNotes = staffStyle->altHideOtherNotes;
        altHideOtherArtics = staffStyle->altHideOtherArtics;
        altHideExpressions = staffStyle->altHideExpressions;
        altHideOtherLyrics = staffStyle->altHideOtherLyrics;
        altHideOtherSmartShapes = staffStyle->altHideOtherSmartShapes;
        altHideOtherExpressions = staffStyle->altHideOtherExpressions;
        masks->altNotation = true;
    }
    if (srcMasks->negKey) {
        hideKeySigs = staffStyle->hideKeySigs;
        masks->negKey = true;
    }
    if (srcMasks->negTime) {
        hideTimeSigs = staffStyle->hideTimeSigs;
        masks->negTime = true;
    }
    if (srcMasks->negClef) {
        hideClefs = staffStyle->hideClefs;
        masks->negClef = true;
    }
    if (srcMasks->hideStaff) {
        hideMode = staffStyle->hideMode;
        masks->hideStaff = true;
    }
    if (srcMasks->noKey) {
        noKey = staffStyle->noKey;
        masks->noKey = true;
    }
    if (srcMasks->showTies) {
        hideTies = staffStyle->hideTies;
        masks->showTies = true;
    }
    if (srcMasks->showDots) {
        hideDots = staffStyle->hideDots;
        masks->showDots = true;
    }
    if (srcMasks->showRests) {
        hideRests = staffStyle->hideRests;
        masks->showRests = true;
    }
    if (srcMasks->showStems) {
        hideStems = staffStyle->hideStems;
        hideBeams = staffStyle->hideBeams;
        stemDirection = staffStyle->stemDirection;
        stemStartFromStaff = staffStyle->stemStartFromStaff;
        stemsFixedEnd = staffStyle->stemsFixedEnd;
        stemsFixedStart = staffStyle->stemsFixedStart;
        horzStemOffUp = staffStyle->horzStemOffUp;
        horzStemOffDown = staffStyle->horzStemOffDown;
        vertStemStartOffUp = staffStyle->vertStemStartOffUp;
        vertStemStartOffDown = staffStyle->vertStemStartOffDown;
        vertStemEndOffUp = staffStyle->vertStemEndOffUp;
        vertStemEndOffDown = staffStyle->vertStemEndOffDown;
        masks->showStems = true;
    }
    if (srcMasks->hideChords) {
        hideChords = staffStyle->hideChords;
        masks->hideChords = true;
    }
    if (srcMasks->hideFretboards) {
        hideFretboards = staffStyle->hideFretboards;
        masks->hideFretboards = true;
    }
    if (srcMasks->hideLyrics) {
        hideLyrics = staffStyle->hideLyrics;
        masks->hideLyrics = true;
    }
    if (srcMasks->showNameParts) {
        showNameInParts = staffStyle->showNameInParts;
        masks->showNameParts = true;
    }
    if (srcMasks->showNoteColors) {
        showNoteColors = staffStyle->showNoteColors;
        masks->showNoteColors = true;
    }
    if (srcMasks->hideStaffLines) {
        hideStaffLines = staffStyle->hideStaffLines;
        masks->hideStaffLines = true;
    }
    if (srcMasks->redisplayLayerAccis) {
        redisplayLayerAccis = staffStyle->redisplayLayerAccis;
        masks->redisplayLayerAccis = true;
    }
    if (srcMasks->negTimeParts) {
        hideTimeSigsInParts = staffStyle->hideTimeSigsInParts;
        masks->negTimeParts = true;
    }
    if (srcMasks->hideKeySigsShowAccis) {
        hideKeySigsShowAccis = staffStyle->hideKeySigsShowAccis;
        masks->hideKeySigsShowAccis = true;
    }
}

MusxInstance<Staff> StaffComposite::getRawStaff() const
{
    auto result = getDocument()->getOthers()->get<Staff>(getPartId(), getCmper());
    if (!result) {
        MUSX_INTEGRITY_ERROR("Unable to load staff " + std::to_string(getCmper()) + " from StaffComposite.");        
    }
    return result;
}

MusxInstance<StaffComposite> StaffComposite::createCurrent(const DocumentPtr& document, Cmper partId,
    InstCmper staffId, MeasCmper measId, Edu eduPosition)
{
    auto rawStaff = document->getOthers()->get<Staff>(partId, staffId);
    if (!rawStaff) return nullptr;

    std::shared_ptr<StaffComposite> result(new StaffComposite(rawStaff, partId, measId, eduPosition));
    if (result->hasStyles) {
        auto styles = StaffStyle::findAllOverlappingStyles(document, partId, staffId, measId, eduPosition);
        for (const auto& style : styles) {
            result->applyStyle(style);
        }
    }

    return result;
}

} // namespace others
} // namespace dom
} // namespace musx