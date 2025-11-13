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
namespace others {

// *****************
// ***** Frame *****
// *****************

void Frame::iterateRawEntries(std::function<bool(const MusxInstance<Entry>& entry)> iterator) const
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

MusxInstanceList<Entry> Frame::getEntries() const
{
    MusxInstanceList<Entry> retval(getDocument(), getRequestedPartId());
    iterateRawEntries([&](const MusxInstance<Entry>& entry) -> bool {
        retval.emplace_back(entry);
        return true;
    });
    return retval;
}

// **************************
// ***** FretboardGroup *****
// **************************

MusxInstance<FretInstrument> FretboardGroup::getFretInstrument() const
{
    return getDocument()->getOthers()->get<FretInstrument>(getRequestedPartId(), fretInstId);
}

MusxInstanceList<details::FretboardDiagram> FretboardGroup::getFretboardDiagrams() const
{
    MusxInstanceList<details::FretboardDiagram> result(getDocument(), getRequestedPartId());
    MUSX_ASSERT_IF(!getInci().has_value()) {
        throw std::logic_error("FretboardGroup " + std::to_string(getCmper()) + " has no inci.");
    }
    Cmper cmper2Base = Cmper(getInci().value()) * 16;
    for (Cmper cmper2Offset = 0; cmper2Offset < music_theory::STANDARD_12EDO_STEPS; cmper2Offset++) {
        if (auto nextDiagram = getDocument()->getDetails()->get<details::FretboardDiagram>(getRequestedPartId(), getCmper(), cmper2Base + cmper2Offset)) {
            result.push_back(nextDiagram);
        } else {
            result.clear();
            MUSX_INTEGRITY_ERROR("FretboardGroup " + std::to_string(getCmper()) + " inci " + std::to_string(getInci().value())
                + " is missing a fretboard diagram for pitch class " + std::to_string(cmper2Offset));
#ifndef MUSX_THROW_ON_INTEGRITY_CHECK_FAIL
            break;
#endif        
        }
    }
    return result;
}

// ****************************
// ***** MarkingCategiory *****
// ****************************

std::string MarkingCategory::getName() const
{
    auto catName = getDocument()->getOthers()->get<MarkingCategoryName>(getRequestedPartId(), getCmper());
    if (catName) {
        return catName->name;
    }
    return {};
}

CategoryStaffListSet MarkingCategory::createStaffListSet() const
{
    return CategoryStaffListSet(getDocument(), getRequestedPartId(), staffList);
}

// *******************
// ***** Measure *****
// *******************

int Measure::calcDisplayNumber() const
{
    if (noMeasNum) {
        return getCmper();
    }
    if (const auto region = MeasureNumberRegion::findMeasure(getDocument(), getCmper())) {
        return region->calcDisplayNumberFor(getCmper());
    }
    return getCmper();
}

MusxInstance<KeySignature> Measure::createKeySignature(const std::optional<StaffCmper>& forStaff) const
{
    std::shared_ptr<KeySignature> result;
    MusxInstance<Staff> staff;
    if (forStaff) {
        staff = StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0);
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

MusxInstance<TimeSignature> Measure::createTimeSignature(const std::optional<StaffCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0)) {
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

MusxInstance<TimeSignature> Measure::createDisplayTimeSignature(const std::optional<StaffCmper>& forStaff) const
{
    if (forStaff) {
        if (auto staff = StaffComposite::createCurrent(getDocument(), getRequestedPartId(), forStaff.value(), getCmper(), 0)) {
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

util::Fraction Measure::calcDuration(const std::optional<StaffCmper>& forStaff) const
{
    auto timeSig = createTimeSignature(forStaff);
    return timeSig->calcTotalDuration();
}

// *****************************
// ***** MeasureExprAssign *****
// *****************************

MusxInstance<TextExpressionDef> MeasureExprAssign::getTextExpression() const
{
    if (!textExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<TextExpressionDef>(getRequestedPartId(), textExprId);
}

MusxInstance<ShapeExpressionDef> MeasureExprAssign::getShapeExpression() const
{
    if (!shapeExprId) {
        return nullptr;
    }
    return getDocument()->getOthers()->get<ShapeExpressionDef>(getRequestedPartId(), shapeExprId);
}

CategoryStaffListSet MeasureExprAssign::createStaffListSet() const
{
    return CategoryStaffListSet(getDocument(), getRequestedPartId(), staffList);
}

EntryInfoPtr MeasureExprAssign::calcAssociatedEntry(bool findExact) const
{
    if (staffAssign > 0) {
        if (auto gfHold = details::GFrameHoldContext(getDocument(), getRequestedPartId(), staffAssign, getCmper())) {
            const auto matchLayer = layer ? std::make_optional(LayerIndex(layer - 1)) : std::nullopt;
            return gfHold.calcNearestEntry(eduPosition, findExact, matchLayer, voice2);
        }
    }
    return {};
}

bool MeasureExprAssign::calcIsAssignedInRequestedPart() const
{
    if (getRequestedPartId() != SCORE_PARTID) {
        return showStaffList != ShowStaffList::ScoreOnly;
    }
    return showStaffList != ShowStaffList::PartOnly;
}

bool MeasureExprAssign::calcAppliesToLayer(LayerIndex layerIndex) const
{
    return (layer == 0 || layer - 1 == layerIndex);
}

StaffCmper MeasureExprAssign::calcAssignedStaffId(bool forPageView) const
{
    if (staffAssign >= 0) {
        return staffAssign;
    }
    const auto systemStaves = [&]() -> std::optional<MusxInstanceList<StaffUsed>> {
        if (forPageView) {
            if (auto system = getDocument()->calculateSystemFromMeasure(getRequestedPartId(), getCmper())) {
                return getDocument()->getOthers()->getArray<StaffUsed>(getRequestedPartId(), system->getCmper());
            }
        }
        return getDocument()->getOthers()->getArray<StaffUsed>(getRequestedPartId(), BASE_SYSTEM_ID);
    }();
    switch (static_cast<StaffList::FloatingValues>(staffAssign)) {
        case StaffList::FloatingValues::TopStaff: return systemStaves->getTopStaffId();
        case StaffList::FloatingValues::BottomStaff: return systemStaves->getBottomStaffId();
        default: break;
    }
    return 0;
}

MusxInstance<StaffComposite> MeasureExprAssign::createCurrentStaff(bool forPageView) const
{
    return StaffComposite::createCurrent(getDocument(), getRequestedPartId(), calcAssignedStaffId(forPageView), getCmper(), eduPosition);
}

bool MeasureExprAssign::calcIsHiddenByAlternateNotation() const
{
    if (staffList != 0) {
        return false; // assignments with staff lists are never hidden by alternate notation: observed behavior
    }
    auto staff = createCurrentStaff();
    MUSX_ASSERT_IF(!staff) {
        throw std::logic_error("Unable to create current staff for expression assignment.");
    }
    if (calcAppliesToLayer(staff->altLayer)) {
        return staff->altHideExpressions;
    } else {
        return staff->altHideOtherExpressions;
    }
}

// *******************************
// ***** MeasureNumberRegion *****
// *******************************

MusxInstance<MeasureNumberRegion> MeasureNumberRegion::findMeasure(const DocumentPtr& document, MeasCmper measureId)
{
    auto regions = document->getOthers()->getArray<MeasureNumberRegion>(SCORE_PARTID);
    for (const auto& region : regions) {
        if (region->calcIncludesMeasure(measureId)) {
            return region;
        }
    }
    return nullptr;
}

int MeasureNumberRegion::calcDisplayNumberFor(MeasCmper measureId) const
{
    if (!calcIncludesMeasure(measureId)) {
        throw std::logic_error("Measure id " + std::to_string(measureId) + " is not contained in measure number region " + std::to_string(getCmper()));
    }
    int result = int(measureId) - int(startMeas) + getStartNumber();
    for (MeasCmper next = startMeas; next <= measureId; next++) {
        if (auto measure = getDocument()->getOthers()->get<Measure>(getRequestedPartId(), next)) {
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

MusxInstance<Staff> MultiStaffInstrumentGroup::getStaffInstanceAtIndex(size_t x) const
{
    if (x >= staffNums.size()) return nullptr;
    auto retval = getDocument()->getOthers()->get<Staff>(getRequestedPartId(), staffNums[x]);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffNums[x])
            + " not found for multiple staff instrument " + std::to_string(getCmper()));
    }
    return retval;
}

MusxInstance<Staff> MultiStaffInstrumentGroup::getFirstStaffInstance() const
{
    if (staffNums.empty()) {
        MUSX_INTEGRITY_ERROR("MultiStaffInstrumentGroup " + std::to_string(getCmper()) + " contains no staves.");
        return nullptr;
    }
    return getStaffInstanceAtIndex(0);
}

MusxInstance<details::StaffGroup> MultiStaffInstrumentGroup::getStaffGroup(Cmper forPartId) const
{
    auto document = getDocument();
    auto groupIdRecord = document->getOthers()->get<MultiStaffGroupId>(forPartId, getCmper());
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

void MultiStaffInstrumentGroup::calcAllMultiStaffGroupIds(const DocumentPtr& document)
{
    auto instGroups = document->getOthers()->getArray<MultiStaffInstrumentGroup>(SCORE_PARTID);
    for (const auto& instance : instGroups) {
        for (size_t x = 0; x < instance->staffNums.size(); x++) {
            auto staff = instance->getStaffInstanceAtIndex(x);
            if (staff) {
                if (staff->multiStaffInstId != instance->getCmper()) {
                    if (staff->multiStaffInstId) {
                        musx::util::Logger::log(musx::util::Logger::LogLevel::Verbose,
                            "Staff " + std::to_string(staff->getCmper()) + " (" + staff->getFullName()
                                + ") appears in more than one instance of MultiStaffInstrumentGroup.");
                    } else {
                        Staff* staffMutable = const_cast<Staff*>(staff.get());
                        staffMutable->multiStaffInstId = instance->getCmper();
                    }
                }
            }
        }
    }
    // guarantee calcAllAutoNumberValues is called after calcAllMultiStaffGroupIds.
    Staff::calcAllAutoNumberValues(document);
}

// ****************
// ***** Page *****
// ****************

void Page::calcSystemInfo(const DocumentPtr& document)
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
                        if (auto sys = document->getOthers()->get<StaffSystem>(part->getCmper(), page->firstSystemId)) {
                            mutablePage->firstMeasureId = sys->startMeas;
                        } else {
                            MUSX_INTEGRITY_ERROR("Page " + std::to_string(page->getCmper()) + " of part " + part->getName()
                                + " has a no system instance for its first system.");                        
                        }
                        if (auto sys = document->getOthers()->get<StaffSystem>(part->getCmper(), page->lastSystemId.value())) {
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

// **************************
// ***** PageTextAssign *****
// **************************

MusxInstance<TextBlock> PageTextAssign::getTextBlock() const
{
    return getDocument()->getOthers()->get<TextBlock>(getRequestedPartId(), block);
}

util::EnigmaParsingContext PageTextAssign::getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId) const
{
    if (auto textBlock = getTextBlock()) {
        if (getCmper() > 0) {
            forPageId = calcStartPageNumber(forPartId);
        }
        return textBlock->getRawTextCtx(forPartId, forPageId);
    }
    return {};
}

MusxInstance<PageTextAssign> PageTextAssign::getForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId, Inci inci)
{
    if (auto part = document->getOthers()->get<PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->get<PageTextAssign>(partId, pageAssignmentId, inci);
    }
    return nullptr;
}

MusxInstanceList<PageTextAssign> PageTextAssign::getArrayForPageId(const DocumentPtr& document, Cmper partId, PageCmper pageId)
{
    if (auto part = document->getOthers()->get<PartDefinition>(SCORE_PARTID, partId)) {
        const PageCmper pageAssignmentId = part->calcAssignmentIdFromPageNumber(pageId);
        return document->getOthers()->getArray<PageTextAssign>(partId, pageAssignmentId);
    }
    return MusxInstanceList<PageTextAssign>(document, partId);
}

std::optional<PageCmper> PageTextAssign::calcStartPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<PartDefinition>(SCORE_PARTID, forPartId)) {
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

std::optional<PageCmper> PageTextAssign::calcEndPageNumber(Cmper forPartId) const
{
    if (auto part = getDocument()->getOthers()->get<PartDefinition>(SCORE_PARTID, forPartId)) {
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

util::EnigmaParsingContext PartDefinition::getNameRawTextCtx() const
{
    /// @todo perhaps additional logic as in getName, but not until something is broken.
    if (nameId) {
        if (auto textBlock = getDocument()->getOthers()->get<TextBlock>(getRequestedPartId(), nameId)) {
            return textBlock->getRawTextCtx(getCmper());
        }
    }
    return {};
}

std::string PartDefinition::getName(util::EnigmaString::AccidentalStyle accidentalStyle) const
{
    if (auto nameRawText = getNameRawTextCtx()) {
        // Although the Finale U.I. prevents ^partname inserts in partname enigma strings, one might have crept in.
        std::unordered_set<std::string_view> ignoreTags = { "partname" }; // do not parse ^partname inserts
        return nameRawText.getText(true, accidentalStyle, ignoreTags);
    }
    if (defaultNameStaff) {
        if (auto staff = getDocument()->getOthers()->get<Staff>(SCORE_PARTID, defaultNameStaff)) {
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

Cmper PartDefinition::calcSystemIuList(Cmper systemId) const
{
    if (auto partGlobs = getDocument()->getOthers()->get<PartGlobals>(getCmper(), MUSX_GLOBALS_CMPER)) {
        if (partGlobs->specialPartExtractionIUList) {
            return partGlobs->specialPartExtractionIUList;
        }
    }
    return systemId;
}

MusxInstance<PartDefinition> PartDefinition::getScore(const DocumentPtr& document)
{
    if (auto score = document->getOthers()->get<PartDefinition>(SCORE_PARTID, SCORE_PARTID)) {
        return score;
    }
    MUSX_INTEGRITY_ERROR("The document contains no instance of PartDefinition for the score.");
    return nullptr;
}

MusxInstanceList<PartDefinition> PartDefinition::getInUserOrder(const DocumentPtr& document)
{
    auto result = document->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
    std::sort(result.begin(), result.end(), [](const auto& lhs, const auto& rhs) {
        return lhs->partOrder < rhs->partOrder;
    });
    return result;
}

std::optional<PageCmper> PartDefinition::calcPageNumberFromAssignmentId(PageCmper pageAssignmentId) const
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

PageCmper PartDefinition::calcAssignmentIdFromPageNumber(PageCmper pageId) const
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

const percussion::PercussionNoteType& PercussionNoteInfo::getNoteType() const
{
    return percussion::getPercussionNoteTypeFromId(getBaseNoteTypeId());
}

// **********************
// ***** RepeatBack *****
// **********************

template<typename T>
static MusxInstance<T> getIndividualPositioningImpl(const MusxInstanceList<T> indivPos, StaffCmper staffId, std::optional<MeasCmper> measureId = std::nullopt)
{
    static_assert(std::is_base_of<others::RepeatIndividualPositioning, T>::value, "T must derive from RepeatIndividualPositioning");

    const auto it = std::find_if(indivPos.begin(), indivPos.end(),
        [&](const auto& ptr)
        {
            MUSX_ASSERT_IF(!ptr) {
                throw std::logic_error("Individual position array should not contain nulls!");
            }
            if (ptr->staffId != staffId) {
                return false;
            }
            return !measureId || ptr->measureId == measureId.value();
        });
    return (it != indivPos.end()) ? *it : nullptr;
}

MusxInstance<RepeatIndividualPositioning> RepeatBack::getIndividualPositioning(StaffCmper staffId) const
{
    return getIndividualPositioningImpl(getDocument()->getOthers()->getArray<RepeatBackIndividualPositioning>(getRequestedPartId(), getCmper()), staffId);
}

RepeatStaffListSet RepeatBack::createStaffListSet() const
{
    return RepeatStaffListSet(getDocument(), getRequestedPartId(), staffList);
}

// *****************************
// ***** RepeatEndingStart *****
// *****************************

int RepeatEndingStart::calcEndingLength() const
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
        auto measure = getDocument()->getOthers()->get<Measure>(getRequestedPartId(), x);
        if (!measure) {
            return 1;
        }
        if (measure->hasEnding && getDocument()->getOthers()->get<RepeatEndingStart>(getRequestedPartId(), x)) {
            break;
        }
        if (--maxLength <= 0) {
            break;
        }
        x++;
    }
    return x - getCmper();
}

bool RepeatEndingStart::calcIsOpen() const
{
    if (endLineVPos < 0) {
        return false;
    }
    if (jumpAction == RepeatActionType::NoJump) {
        return true;
    }
    for (Cmper x = getCmper(); true; x++) {
        auto measure = getDocument()->getOthers()->get<Measure>(getRequestedPartId(), x);
        if (!measure) {
            break;
        }
        if (measure->backwardsRepeatBar) {
            if (auto backRepeat = getDocument()->getOthers()->get<RepeatBack>(getRequestedPartId(), x)) {
                if (auto repeatOptions = getDocument()->getOptions()->get<options::RepeatOptions>()) {
                    return (backRepeat->leftVPos - backRepeat->rightVPos) == repeatOptions->bracketHookLen;
                }
                return true;
            }
        }
    }
    return false;
}

MusxInstance<RepeatIndividualPositioning> RepeatEndingStart::getIndividualPositioning(StaffCmper staffId) const
{
    return getIndividualPositioningImpl(getDocument()->getOthers()->getArray<RepeatEndingStartIndividualPositioning>(getRequestedPartId(), getCmper()), staffId);
}

MusxInstance<RepeatIndividualPositioning> RepeatEndingStart::getTextIndividualPositioning(StaffCmper staffId) const
{
    return getIndividualPositioningImpl(getDocument()->getOthers()->getArray<RepeatEndingTextIndividualPositioning>(getRequestedPartId(), getCmper()), staffId);
}

std::string RepeatEndingStart::createEndingText() const
{
    if (auto userText = getDocument()->getOthers()->get<RepeatEndingText>(getRequestedPartId(), getCmper())) {
        return userText->text;
    }
    std::string result;
    if (auto passList = getDocument()->getOthers()->get<RepeatPassList>(getRequestedPartId(), getCmper())) {
        for (int pass : passList->values) {
            if (!result.empty()) {
                result += ',';
                result += ' ';
            }
            result += std::to_string(pass);
        }
        if (auto repeatOptions = getDocument()->getOptions()->get<options::RepeatOptions>()) {
            if (repeatOptions->addPeriod) {
                result += '.';
            }
        }
    }
    return result;
}

RepeatStaffListSet RepeatEndingStart::createStaffListSet() const
{
    return RepeatStaffListSet(getDocument(), getRequestedPartId(), staffList);
}

// ************************
// ***** StaffListSet *****
// ************************

template <class ScoreList, class PartsList, class ScoreForcedList, class PartsForcedList>
StaffListSet<ScoreList, PartsList, ScoreForcedList, PartsForcedList>::StaffListSet(
    const DocumentPtr& document, Cmper partId, Cmper staffListId) noexcept
{
    if (partId == SCORE_PARTID) {
        m_staffList = document->getOthers()->get<ScoreList>(partId, staffListId);
        if constexpr (!std::is_same_v<ScoreForcedList, void>) {
            m_forcedStaffList = document->getOthers()->get<ScoreForcedList>(partId, staffListId);
        }
    } else {
        m_staffList = document->getOthers()->get<PartsList>(partId, staffListId);
        if constexpr (!std::is_same_v<PartsForcedList, void>) {
            m_forcedStaffList = document->getOthers()->get<PartsForcedList>(partId, staffListId);
        }
    }
}

template <class ScoreList, class PartsList, class ScoreForcedList, class PartsForcedList>
bool StaffListSet<ScoreList, PartsList, ScoreForcedList, PartsForcedList>::contains(
    StaffCmper staffId, const MusxInstanceList<StaffUsed>& systemStaves, bool isHidden) const noexcept
{
    const StaffCmper topStaffId = systemStaves.getTopStaffId();
    const StaffCmper botStaffId = systemStaves.getBottomStaffId();

    auto staffListContainsStaff = [&](const MusxInstance<StaffList>& staffList) -> bool {
        if (!staffList) {
            return false;
        }
        MUSX_ASSERT_IF(staffList->getRequestedPartId() != systemStaves.getRequestedPartId()) {
            throw std::logic_error("Staff system part id [" + std::to_string(systemStaves.getRequestedPartId())
                + "] does not match part id for staff list set [" + std::to_string(staffList->getRequestedPartId()) + "].");
        }
        if (staffId == topStaffId && staffList->containsValue(static_cast<StaffCmper>(StaffList::FloatingValues::TopStaff))) {
            return true;
        }
        if (staffId == botStaffId && staffList->containsValue(static_cast<StaffCmper>(StaffList::FloatingValues::BottomStaff))) {
            return true;
        }
        /// @todo It may be necessary to add some fuzzy logic around StaffGroup here, at least for category staff lists.
        /// Finale includes a staff if it is the top staff in a group, when the top staff of a group is included. We defer this
        /// to another day.
        return staffList->containsValue(staffId);
    };

    if (!isHidden && staffListContainsStaff(m_staffList)) {
        return true;
    }
    
    return staffListContainsStaff(m_forcedStaffList);
}

template class StaffListSet< StaffListCategoryScore, StaffListCategoryParts>;
template class StaffListSet<StaffListRepeatScore, StaffListRepeatParts, StaffListRepeatScoreForced, StaffListRepeatPartsForced>;

// ***********************
// ***** StaffSystem *****
// ***********************

MusxInstance<Page> StaffSystem::getPage() const
{
    return getDocument()->getOthers()->get<Page>(getRequestedPartId(), pageId);
}

std::pair<util::Fraction, util::Fraction> StaffSystem::calcMinMaxStaffSizes() const
{
    if (hasStaffScaling) {
        auto systemStaves = getDocument()->getOthers()->getArray<StaffUsed>(getRequestedPartId(), getCmper());
        if (!systemStaves.empty()) {
            std::pair<util::Fraction, util::Fraction> result = std::make_pair((std::numeric_limits<util::Fraction>::max)(), (std::numeric_limits<util::Fraction>::min)());
            for (const auto& systemStaff : systemStaves) {
                auto staffSize = getDocument()->getDetails()->get<details::StaffSize>(getRequestedPartId(), getCmper(), systemStaff->getCmper());
                const util::Fraction val = staffSize ? util::Fraction(staffSize->staffPercent / 100) : 1;
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

int TempoChange::getAbsoluteTempo(NoteType noteType) const
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

util::EnigmaParsingContext TextBlock::getRawTextCtx(Cmper forPartId, std::optional<Cmper> forPageId, util::EnigmaString::TextInsertCallback defaultInsertFunc) const
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

std::string TextBlock::getText(const DocumentPtr& document, const Cmper textId, Cmper forPartId, bool trimTags, util::EnigmaString::AccidentalStyle accidentalStyle)
{
    auto textBlock = document->getOthers()->get<TextBlock>(forPartId, textId);
    if (textBlock) {
        return textBlock->getRawTextCtx(forPartId).getText(trimTags, accidentalStyle);
    }
    return {};
}

// *****************************
// ***** TextExpressionDef *****
// *****************************

MusxInstance<TextBlock> TextExpressionDef::getTextBlock() const
{
    return getDocument()->getOthers()->get<TextBlock>(getRequestedPartId(), textIdKey);
}

util::EnigmaParsingContext TextExpressionDef::getRawTextCtx(Cmper forPartId) const
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

MusxInstance<Enclosure> TextExpressionDef::getEnclosure() const
{
    if (!hasEnclosure) return nullptr;
    return getDocument()->getOthers()->get<TextExpressionEnclosure>(getRequestedPartId(), getCmper());
}

// *********************
// ***** StaffUsed *****
// *********************

util::Fraction StaffUsed::calcEffectiveScaling() const
{
    util::Fraction result(1);
    if (SystemCmper(getCmper()) > 0) { // if this is a page-view system
        if (auto system = getDocument()->getOthers()->get<StaffSystem>(getRequestedPartId(), getCmper())) {
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

MusxInstance<Staff> StaffUsed::getStaffInstance() const
{
    auto retval = getDocument()->getOthers()->get<Staff>(getRequestedPartId(), staffId);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staffId) + " not found for StaffUsed list " + std::to_string(getCmper()));
    }
    return retval;
}

MusxInstance<StaffComposite> StaffUsed::getStaffInstance(MeasCmper measureId, Edu eduPosition) const
{
    auto retval = StaffComposite::createCurrent(getDocument(), getRequestedPartId(), staffId, measureId, eduPosition);
    if (!retval) {
        MUSX_INTEGRITY_ERROR("Composite staff " + std::to_string(staffId) + " not found for StaffUsed list " + std::to_string(getCmper())
            + " at measure " + std::to_string(measureId) + " eduPosition " + std::to_string(eduPosition));
    }
    return retval;
}

// ****************************
// ***** TextRepeatAssign *****
// ****************************

MusxInstance<RepeatIndividualPositioning> TextRepeatAssign::getIndividualPositioning(StaffCmper staffId) const
{
    return getIndividualPositioningImpl(getDocument()->getOthers()->getArray<TextRepeatIndividualPositioning>(getRequestedPartId(), textRepeatId),
        staffId, static_cast<MeasCmper>(getCmper()));
}

RepeatStaffListSet TextRepeatAssign::createStaffListSet() const
{
    return RepeatStaffListSet(getDocument(), getRequestedPartId(), staffList);
}

} // namespace others

} // namespace dom
} // namespace musx