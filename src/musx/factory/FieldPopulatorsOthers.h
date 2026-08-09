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
    Enclosure::Shape result{};
    if (value >= static_cast<uint8_t>(Enclosure::Shape::NoEnclosure) &&
        value <= static_cast<uint8_t>(Enclosure::Shape::Octogon)) {
        result = static_cast<Enclosure::Shape>(value);
    } else {
        MUSX_UNKNOWN_XML("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
    }
    return result;
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

} // namespace factory
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE THIS
