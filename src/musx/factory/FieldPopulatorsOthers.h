/*
 * Copyright (C) 2024, Robert Patterson
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
struct FieldPopulator<TextExpressionEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

template <>
struct FieldPopulator<TextRepeatEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

MUSX_RESOLVER_ARRAY(LayerAttributes, {
    [](const dom::DocumentPtr& document) {
        auto layers = document->getOthers()->getArray<LayerAttributes>(SCORE_PARTID);
        if (layers.size() != 4) {
            throw std::invalid_argument("Expected exactly 4 <layerAtts> elements.");
        }
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->getCmper() != i) {
                throw std::invalid_argument("Expected <layerAtts> elements to have cmper values 0, 1, 2, 3 in order.");
            }
        }
    }
});

MUSX_RESOLVER_ARRAY(MarkingCategory, {
    [](const dom::DocumentPtr& document) {
        auto cats = document->getOthers()->getArray<MarkingCategory>(SCORE_PARTID);
        for (const auto& cat : cats) {
            if (cat->categoryType == MarkingCategory::CategoryType::Invalid) {
                throw std::invalid_argument("Encountered <markingsCategory> node (cmper " + std::to_string(cat->getCmper()) + ") with no categoryType");
            }
        }
    }
});

MUSX_RESOLVER_ARRAY(MultiStaffInstrumentGroup, {
    [](const dom::DocumentPtr& document) {
        auto instGroups = document->getOthers()->getArray<MultiStaffInstrumentGroup>(SCORE_PARTID);
        for (const auto& instance : instGroups) {
            for (size_t x = 0; x < instance->staffNums.size(); x++) {
                auto staff = instance->getStaffAtIndex(x);
                if (staff) {
                    if (staff->multiStaffInstId) {
                        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(staff->getCmper()) + " appears in more than one instance of MultiStaffInstrumentGroup.");
                    } else {
                        staff->multiStaffInstId = instance->getCmper();
                    }
                }
            }
        }
    }
});

MUSX_RESOLVER_ARRAY(TextExpressionDef, {
    [](const dom::DocumentPtr& document) {
        auto exps = document->getOthers()->getArray<TextExpressionDef>(SCORE_PARTID);
        for (const auto& instance : exps) {
            if (instance->categoryId) {
                auto markingCat = document->getOthers()->get<MarkingCategory>(instance->getPartId(), instance->categoryId);
                if (!markingCat) {
                    throw std::invalid_argument("Marking category for text expression " + std::to_string(instance->getCmper()) + " does not exist.");
                }
                markingCat->textExpressions.emplace(instance->getCmper(), instance);
            }
        }
    }
});

} // namespace factory
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE THIS
