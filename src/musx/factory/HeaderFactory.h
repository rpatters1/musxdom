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

#include <memory>
#include <stdexcept>

#include "musx/factory/FactoryBase.h"
#include "musx/dom/Header.h"
#include "musx/xml/XmlInterface.h"

namespace musx {
namespace factory {

/**
 * @brief Factory class for creating `Header` objects from XML.
 */
class HeaderFactory : public FactoryBase
{
public:
    /**
     * @brief Creates a `Header` object from an XML element.
     * 
     * @param element The XML element representing the `<header>` node.
     * @return A fully populated `Header` object.
     * @throws std::invalid_argument If required nodes or attributes are missing or invalid.
     */
    static musx::dom::header::HeaderPtr create(const std::shared_ptr<xml::IXmlElement>& element)
    {
        auto headerDataElement = getFirstChildElement(element, "headerData");
        if (!headerDataElement) {
            throw std::invalid_argument("Missing <headerData> element.");
        }

        auto header = std::make_shared<dom::header::Header>();
        header->wordOrder = parseWordOrder(getFirstChildElement(headerDataElement, "wordOrder")->getText());
        header->textEncoding = parseTextEncoding(getFirstChildElement(headerDataElement, "textEncoding")->getText());
        header->created = parseFileInfo(getFirstChildElement(headerDataElement, "created"));
        header->modified = parseFileInfo(getFirstChildElement(headerDataElement, "modified"));

        return header;
    }

private:
    /**
     * @brief Parses the `WordOrder` enum from a string.
     * 
     * @param value The string value to parse.
     * @return The corresponding `WordOrder` enum value.
     * @throws std::invalid_argument if the word order value is invalid.
     */
    static musx::dom::header::WordOrder parseWordOrder(const std::string& value)
    {
        if (value == "lo-endian") return musx::dom::header::WordOrder::LittleEndian;
        if (value == "hi-endian") return musx::dom::header::WordOrder::BigEndian;
        throw std::invalid_argument("Invalid word order value: " + value);
    }

    /**
     * @brief Parses the `TextEncoding` enum from a string.
     * 
     * @param value The string value to parse.
     * @return The corresponding `TextEncoding` enum value.
     */
    static musx::dom::header::TextEncoding parseTextEncoding(const std::string& value)
    {
        if (value == "Mac") return musx::dom::header::TextEncoding::Mac;
        if (value == "Windows") return musx::dom::header::TextEncoding::Windows;
        return musx::dom::header::TextEncoding::Other;
    }

    /**
     * @brief Parses the `Platform` enum from a string.
     * 
     * @param value The string value to parse.
     * @return The corresponding `Platform` enum value.
     */
    static musx::dom::header::Platform parsePlatform(const std::string& value)
    {
        if (value == "MAC") return musx::dom::header::Platform::Mac;
        if (value == "WIN") return musx::dom::header::Platform::Windows;
        return musx::dom::header::Platform::Other;
    }

    /**
     * @brief Parses a `FileInfo` object from an XML element.
     * 
     * @param element The XML element representing a `<created>` or `<modified>` node.
     * @return A fully populated `FileInfo` object.
     */
    static musx::dom::header::FileInfo parseFileInfo(const std::shared_ptr<xml::IXmlElement>& element)
    {
        musx::dom::header::FileInfo date;
        date.year = getFirstChildElement(element, "year")->getTextAs<int>();
        date.month = getFirstChildElement(element, "month")->getTextAs<int>();
        date.day = getFirstChildElement(element, "day")->getTextAs<int>();
        date.modifiedBy = getFirstChildElement(element, "modifiedBy")->getText();
        date.finaleVersion = parseFinaleVersion(getFirstChildElement(element, "enigmaVersion"));
        date.application = getFirstChildElement(element, "application")->getText();
        date.platform = parsePlatform(getFirstChildElement(element, "platform")->getText());
        date.appVersion = parseFinaleVersion(getFirstChildElement(element, "appVersion"));
        date.fileVersion = parseFinaleVersion(getFirstChildElement(element, "fileVersion"));
        date.appRegion = getFirstChildElement(element, "appRegion")->getText();

        return date;
    }

    /**
     * @brief Parses a `FinaleVersion` object from an XML element.
     * 
     * @param element The XML element representing a `<version>` node.
     * @return A fully populated `FinaleVersion` object.
     * @throws std::runtime_error If required nodes or attributes are missing or invalid.
     */
    static musx::dom::header::FinaleVersion parseFinaleVersion(const std::shared_ptr<xml::IXmlElement>& element) {
        if (!element) {
            throw std::runtime_error("Missing version element.");
        }

        musx::dom::header::FinaleVersion version;
        version.major = getFirstChildElement(element, "major")->getTextAs<int>();
        version.minor = getFirstChildElement(element, "minor")->getTextAs<int>();
        version.maint = getOptionalChildTextAs<int>(element, "maint");
        version.devStatus = getFirstChildElement(element, "devStatus")->getText();
        version.build = getOptionalChildTextAs<int>(element, "build");

        return version;
    }
};

} // namespace factory
} // namespace musx
