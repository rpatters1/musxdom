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

namespace musx {
namespace factory {

using namespace dom;
using namespace dom::others;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

template <>
struct FieldPopulator<Enclosure> : public FactoryBase
{
private:
    static Enclosure::Shape toShape(int value)
    {
        if (value >= static_cast<int>(Enclosure::Shape::NoEnclosure) &&
            value <= static_cast<int>(Enclosure::Shape::Octogon)) {
            return static_cast<Enclosure::Shape>(value);
        }
        throw std::invalid_argument("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
    }

public:
    static void populate(Enclosure& instance, const std::shared_ptr<xml::IXmlElement>& element)
    {
        getFieldFromXml(element, "xAdd", instance.xAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yAdd", instance.yAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "xMargin", instance.xMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yMargin", instance.yMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "lineWidth", instance.lineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "sides", instance.shape, [](auto element) {  return toShape(element->template getTextAs<uint8_t>()); });
        getFieldFromXml(element, "cornerRadius", instance.cornerRadius, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "fixedSize", instance.fixedSize, [](auto element) { return true; }, false);
        getFieldFromXml(element, "notTall", instance.notTall, [](auto element) { return true; }, false);
        getFieldFromXml(element, "opaque", instance.opaque, [](auto element) { return true; }, false);
        getFieldFromXml(element, "roundCorners", instance.roundCorners, [](auto element) { return true; }, false);
    }
};

template <>
struct FieldPopulator<FontDefinition> : public FactoryBase
{
    static void populate(FontDefinition& instance, const std::shared_ptr<xml::IXmlElement>& element)
    {
        getFieldFromXml(element, "charsetBank", instance.charsetBank, [](auto element) { return element->getText(); });
        getFieldFromXml(element, "charsetVal", instance.charsetVal, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "pitch", instance.pitch, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "family", instance.family, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "name", instance.name, [](auto element) { return element->getText(); });
    }
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

// Repeat for other types...

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
