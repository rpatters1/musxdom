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
#pragma once

#include "musx/dom/BaseClasses.h"
#include "musx/dom/Others.h"
#include "musx/dom/Details.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::others;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

template <>
inline Enclosure::Shape toEnum<Enclosure::Shape>(const uint8_t& value)
{
    if (value >= static_cast<uint8_t>(Enclosure::Shape::NoEnclosure) &&
        value <= static_cast<uint8_t>(Enclosure::Shape::Octogon)) {
        return static_cast<Enclosure::Shape>(value);
    }
    MUSX_UNKNOWN_XML("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
    return {};
}

template <>
struct FieldPopulator<DrumStaffStyle> : private FieldPopulator<DrumStaff>
{
    using FieldPopulator<DrumStaff>::populate;
};

template <>
struct FieldPopulator<MarkingCategoryName> : private FieldPopulator<OthersName>
{
    using FieldPopulator<OthersName>::populate;
};

template <>
struct FieldPopulator<NamePositionAbbreviated> : private FieldPopulator<NamePositioning>
{
    using FieldPopulator<NamePositioning>::populate;
};

template <>
struct FieldPopulator<NamePositionStyleAbbreviated> : private FieldPopulator<NamePositioning>
{
    using FieldPopulator<NamePositioning>::populate;
};

template <>
struct FieldPopulator<NamePositionFull> : private FieldPopulator<NamePositioning>
{
    using FieldPopulator<NamePositioning>::populate;
};

template <>
struct FieldPopulator<NamePositionStyleFull> : private FieldPopulator<NamePositioning>
{
    using FieldPopulator<NamePositioning>::populate;
};

template <>
struct FieldPopulator<RepeatBackIndividualPositioning> : private FieldPopulator<RepeatIndividualPositioning>
{
    using FieldPopulator<RepeatIndividualPositioning>::populate;
};

template <>
struct FieldPopulator<RepeatEndingStartIndividualPositioning> : private FieldPopulator<RepeatIndividualPositioning>
{
    using FieldPopulator<RepeatIndividualPositioning>::populate;
};

template <>
struct FieldPopulator<RepeatEndingTextIndividualPositioning> : private FieldPopulator<RepeatIndividualPositioning>
{
    using FieldPopulator<RepeatIndividualPositioning>::populate;
};

template <>
struct FieldPopulator<TextRepeatIndividualPositioning> : private FieldPopulator<RepeatIndividualPositioning>
{
    using FieldPopulator<RepeatIndividualPositioning>::populate;
};

template <>
struct FieldPopulator<StaffListCategoryName> : private FieldPopulator<OthersName>
{
    using FieldPopulator<OthersName>::populate;
};

template <>
struct FieldPopulator<StaffListCategoryParts> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<StaffListCategoryScore> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<StaffListRepeatName> : private FieldPopulator<OthersName>
{
    using FieldPopulator<OthersName>::populate;
};

template <>
struct FieldPopulator<StaffListRepeatParts> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<StaffListRepeatPartsForced> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<StaffListRepeatScore> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<StaffListRepeatScoreForced> : private FieldPopulator<StaffList>
{
    using FieldPopulator<StaffList>::populate;
};

template <>
struct FieldPopulator<TextExpressionEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

template <>
struct FieldPopulator<TextRepeatEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

MUSX_RESOLVER_ENTRY(KeyMapArray, {
    [](const dom::DocumentPtr& document) {
        auto arrays = document->getOthers()->getArray<KeyMapArray>(SCORE_PARTID);
        for (const auto& array : arrays) {
            KeyMapArray* mutableArray = const_cast<KeyMapArray*>(array.get());
            auto trimSteps = [&](size_t newSize) {
                while (array->steps.size() > newSize) {
                    const auto& elt = array->steps[array->steps.size() - 1];
                    if (elt->diatonic || elt->hlevel != 0) {
                        break; // itegrity check below will catch this error
                    }
                    mutableArray->steps.pop_back();
                }
            };
            if (auto keyFormat = document->getOthers()->get<others::KeyFormat>(SCORE_PARTID, array->getCmper())) {
                trimSteps(keyFormat->semitones);
                if (keyFormat->scaleTones != array->countDiatonicSteps() || keyFormat->semitones != array->steps.size()) {
                    MUSX_INTEGRITY_ERROR("KeyMapArray " + std::to_string(array->getCmper()) + " does not match KeyFormat.");
                }
            } else {
                trimSteps(12);
                if (array->countDiatonicSteps() != 7 || array->steps.size() != 12) { // default diatonic
                    MUSX_INTEGRITY_ERROR("KeyMapArray " + std::to_string(array->getCmper()) + " has no KeyFormat but does not match default values.");
                }
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(LayerAttributes, {
    [](const dom::DocumentPtr& document) {
        auto layers = document->getOthers()->getArray<LayerAttributes>(SCORE_PARTID);
        if (layers.size() != 4) {
            MUSX_INTEGRITY_ERROR("Expected exactly 4 <layerAtts> elements.");
        }
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->getCmper() != i) {
                MUSX_INTEGRITY_ERROR("Expected <layerAtts> elements to have cmper values 0, 1, 2, 3 in order.");
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(MarkingCategory, {
    [](const dom::DocumentPtr& document) {
        auto cats = document->getOthers()->getArray<MarkingCategory>(SCORE_PARTID);
        for (const auto& cat : cats) {
            if (cat->categoryType == MarkingCategory::CategoryType::Invalid) {
                MUSX_INTEGRITY_ERROR("Encountered <markingsCategory> node (cmper " + std::to_string(cat->getCmper()) + ") with no categoryType");
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(MultiStaffGroupId, {
    [](const dom::DocumentPtr& document) {
        auto parts = document->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
        for (const auto& part : parts) {
            auto instGroups = document->getOthers()->getArray<MultiStaffGroupId>(part->getCmper());
            for (const auto& instance : instGroups) {
                if (auto group = document->getDetails()->get<details::StaffGroup>(part->getCmper(), BASE_SYSTEM_ID, instance->staffGroupId)) {
                    details::StaffGroup* mutableGroup = const_cast<details::StaffGroup*>(group.get());
                    mutableGroup->multiStaffGroupId = instance->getCmper();
                } else if (instance->staffGroupId != 0) {
                    MUSX_INTEGRITY_ERROR("Group " + std::to_string(instance->staffGroupId) + " appears in MultiStaffGroupId "
                        + std::to_string(instance->getCmper()) + " but does not exist.");
                }
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(MultiStaffInstrumentGroup, MultiStaffInstrumentGroup::calcAllMultiStaffGroupIds);

MUSX_RESOLVER_ENTRY(Page, Page::calcSystemInfo);

MUSX_RESOLVER_ENTRY(PartDefinition, {
    [](const dom::DocumentPtr& document) {
        auto parts = document->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
        for (const auto& part : parts) {
            auto partGlobals = document->getOthers()->get<others::PartGlobals>(part->getCmper(), MUSX_GLOBALS_CMPER);
            if (!partGlobals) {
                MUSX_INTEGRITY_ERROR("Part " + std::to_string(part->getCmper()) + " has no PartGlobals.");
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(ShapeExpressionDef, {
    [](const dom::DocumentPtr& document) {
        auto exps = document->getOthers()->getArray<ShapeExpressionDef>(SCORE_PARTID);
        for (const auto& instance : exps) {
            if (instance->categoryId) {
                auto markingCat = document->getOthers()->get<MarkingCategory>(instance->getSourcePartId(), instance->categoryId);
                if (!markingCat) {
                    MUSX_INTEGRITY_ERROR("Marking category for shape expression " + std::to_string(instance->getCmper()) + " does not exist.");
                }
                auto mutableMarkingCat = const_cast<MarkingCategory*>(markingCat.get());
                mutableMarkingCat->shapeExpressions.emplace(instance->getCmper(), instance);
            }
        }
    }
});

MUSX_RESOLVER_ENTRY(Staff, {
    [](const dom::DocumentPtr& document) {
        others::Staff::calcAllRuntimeValues<others::Staff>(document);
        auto instGroups = document->getOthers()->getArray<MultiStaffInstrumentGroup>(SCORE_PARTID);
        // If no MultiStaffInstrumentGroup records exist, then we need to do this here.
        if (instGroups.empty()) {
            others::Staff::calcAllAutoNumberValues(document);
        }
    }
});

MUSX_RESOLVER_ENTRY(StaffStyle, {
    [](const dom::DocumentPtr& document) {
        others::Staff::calcAllRuntimeValues<others::StaffStyle>(document);
    }
});

MUSX_RESOLVER_ENTRY(TextExpressionDef, {
    [](const dom::DocumentPtr& document) {
        auto exps = document->getOthers()->getArray<TextExpressionDef>(SCORE_PARTID);
        for (const auto& instance : exps) {
            if (instance->categoryId) {
                auto markingCat = document->getOthers()->get<MarkingCategory>(instance->getSourcePartId(), instance->categoryId);
                if (!markingCat) {
                    MUSX_INTEGRITY_ERROR("Marking category for text expression " + std::to_string(instance->getCmper()) + " does not exist.");
                }
                auto mutableMarkingCat = const_cast<MarkingCategory*>(markingCat.get());
                mutableMarkingCat->textExpressions.emplace(instance->getCmper(), instance);
            }
        }
    }
});

} // namespace factory
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE THIS
