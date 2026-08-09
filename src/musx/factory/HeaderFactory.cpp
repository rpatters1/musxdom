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

#include "musx/factory/HeaderFactory.h"

#include <stdexcept>

#include "musx/factory/FactoryBase.h"

namespace musx {
namespace factory {
namespace {

class HeaderParser : public FactoryBase
{
public:
    static dom::header::Platform parsePlatform(const xml::XmlElementPtr& element)
    {
        const auto value = element->getText();
        if (value == "MAC") return dom::header::Platform::Mac;
        if (value == "WIN") return dom::header::Platform::Windows;
        return dom::header::Platform::Other;
    }

    static dom::header::FinaleVersion parseVersion(const xml::XmlElementPtr& element)
    {
        if (!element) throw std::runtime_error("Missing version element.");
        dom::header::FinaleVersion version;
        getFieldFromXml(element, "major", version.major, [](auto e) { return e->template getTextAs<int>(); });
        getFieldFromXml(element, "minor", version.minor, [](auto e) { return e->template getTextAs<int>(); });
        version.maint = getOptionalChildTextAs<int>(element, "maint");
        getFieldFromXml(element, "devStatus", version.devStatus, [](auto e) { return e->getText(); });
        version.build = getOptionalChildTextAs<int>(element, "build");
        return version;
    }

    static dom::header::FileInfo parseFileInfo(const xml::XmlElementPtr& element)
    {
        dom::header::FileInfo info;
        getFieldFromXml(element, "year", info.year, [](auto e) { return e->template getTextAs<int>(); });
        getFieldFromXml(element, "month", info.month, [](auto e) { return e->template getTextAs<int>(); });
        getFieldFromXml(element, "day", info.day, [](auto e) { return e->template getTextAs<int>(); });
        getFieldFromXml(element, "modifiedBy", info.modifiedBy, [](auto e) { return e->getText(); });
        getFieldFromXml(element, "enigmaVersion", info.finaleVersion, parseVersion);
        getFieldFromXml(element, "application", info.application, [](auto e) { return e->getText(); });
        getFieldFromXml(element, "platform", info.platform, parsePlatform);
        getFieldFromXml(element, "appVersion", info.appVersion, parseVersion);
        getFieldFromXml(element, "fileVersion", info.fileVersion, parseVersion);
        getFieldFromXml(element, "appRegion", info.appRegion, [](auto e) { return e->getText(); });
        return info;
    }

    static dom::header::HeaderPtr parse(const xml::XmlElementPtr& element)
    {
        auto data = getFirstChildElement(element, "headerData");
        auto header = std::make_shared<dom::header::Header>();
        getFieldFromXml(data, "wordOrder", header->wordOrder, [](auto e) {
            const auto value = e->getText();
            if (value == "lo-endian") return dom::header::WordOrder::LittleEndian;
            if (value == "hi-endian") return dom::header::WordOrder::BigEndian;
            throw std::invalid_argument("Invalid word order value: " + value);
        });
        getFieldFromXml(data, "textEncoding", header->textEncoding, [](auto e) {
            const auto value = e->getText();
            if (value == "Mac") return dom::header::TextEncoding::Mac;
            if (value == "Windows") return dom::header::TextEncoding::Windows;
            return dom::header::TextEncoding::Other;
        });
        getFieldFromXml(data, "created", header->created, parseFileInfo);
        getFieldFromXml(data, "modified", header->modified, parseFileInfo);
        return header;
    }
};

} // namespace

dom::header::HeaderPtr HeaderFactory::create(const xml::XmlElementPtr& element)
{
    return HeaderParser::parse(element);
}

} // namespace factory
} // namespace musx
