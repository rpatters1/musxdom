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
#include "musx/dom/Document.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::details;

template <>
struct FieldPopulator<TieAlterEnd> : private FieldPopulator<TieAlterBase>
{
    using FieldPopulator<TieAlterBase>::populate;
};

template <>
struct FieldPopulator<TieAlterStart> : private FieldPopulator<TieAlterBase>
{
    using FieldPopulator<TieAlterBase>::populate;
};

template <>
inline StaffGroup::BracketStyle toEnum<StaffGroup::BracketStyle>(const int& value)
{
    if (value >= static_cast<int>(StaffGroup::BracketStyle::None) &&
        value <= static_cast<int>(StaffGroup::BracketStyle::DeskBracket)) {
        return static_cast<StaffGroup::BracketStyle>(value);
    }
    MUSX_UNKNOWN_XML("Invalid <bracket><id> value in XML for StaffGroup: " + std::to_string(value));
    return {};
}

MUSX_RESOLVER_ENTRY(StaffGroup, {
    [](const dom::DocumentPtr& document) {
        auto parts = document->getOthers()->getArray<others::PartDefinition>(SCORE_PARTID);
        for (const auto& part : parts) {
            auto groups = document->getDetails()->getArray<StaffGroup>(part->getCmper(), BASE_SYSTEM_ID);
            auto baseList = document->getOthers()->getArray<others::InstrumentUsed>(part->getCmper(), BASE_SYSTEM_ID);
            for (const auto& instance : groups) {
                auto startIndex = others::InstrumentUsed::getIndexForStaff(baseList, instance->startInst);
                auto endIndex = others::InstrumentUsed::getIndexForStaff(baseList, instance->endInst);
                if (!startIndex || !endIndex) {
                    // this situation arises fairly commonly in Finale files, so the message is demoted to a verbose logging message
                    // from an integrity error.
                    util::Logger::log(util::Logger::LogLevel::Verbose, "Group " + std::to_string(instance->getCmper2()) + " in part " + part->getName()
                        + " [" + std::to_string(part->getCmper()) + "] has non-existent start or end staff cmpers");
                    continue;
                }
                for (size_t x = *startIndex; x <= *endIndex && x < baseList.size(); x++) {
                    instance->staves.insert(baseList[x]->staffId);
                }
            }
        }
    }
});

} // namespace factory
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE THIS
