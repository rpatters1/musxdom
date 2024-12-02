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

#include <stdexcept>
#include <string>
#include <optional>

#include "musx/dom/ObjectPool.h"
#include "musx/xml/XmlInterface.h"

namespace musx {

/**
 * @namespace musx::factory
 * @brief Class factories are in this namespace
 */
namespace factory {

/**
 * @brief Factory base class.
 */
class FactoryBase
{
protected:
    /** @brief Helper function to check if a child exists and populate it if so.
     *
     * EnigmaXml generally omits fields when they have the default value (usually 0). This helper
     * function handles that without causing an exception. However, fields retrieved with this function
     * are expected to exist unless the `expected` parameter is false. If they are missing but expected,
     * this fact may be logged.
     *
     * @tparam DataType the type of data to be populated (usually inferred from the call)
     * @tparam ParserFunc the parser function (usually inferred from the call)
     */
    template<typename DataType, typename ParserFunc>
    static void getFieldFromXml(const std::shared_ptr<xml::IXmlElement>& element, const std::string& nodeName, DataType& dataField, ParserFunc parserFunc, bool expected = true)
    {
        if (auto childElement = element->getFirstChildElement(nodeName)) {
            dataField = parserFunc(childElement);
        } else if (expected) {
#ifdef MUSX_LOG_MISSING_FIELDS
            //stdout in lieu of logging for now
            std::cout << "Expected field <" << element->getTagName() << "><" << nodeName << "> not found." << std::endl;
#endif
        }
    }

    /** @brief Helper function to throw when child element does not exist.
     *
     * @throws std::invalid_argument when child element does not exist.
     */
    static std::shared_ptr<xml::IXmlElement> getFirstChildElement(const std::shared_ptr<xml::IXmlElement>& element, const std::string& childElementName)
    {
        auto childElement = element->getFirstChildElement(childElementName);
        if (!childElement) {
            throw std::invalid_argument("Missing <" + childElementName + "> element.");
        }
        return childElement;
    }

    /** @brief Helper function to return std::nullopt when child element does not exist. */
    static std::optional<std::string> getOptionalChildText(const std::shared_ptr<xml::IXmlElement>& element, const std::string& childElementName)
    {
        auto childElement = element->getFirstChildElement(childElementName);
        if (!childElement) {
            return std::nullopt;
        }
        return childElement->getText();
    }

    /** @brief Helper function to return std::nullopt when child element does not exist. */
    template<typename T>
    static std::optional<T> getOptionalChildTextAs(const std::shared_ptr<xml::IXmlElement>& element, const std::string& childElementName, T defaultValue = {})
    {
        auto childElement = element->getFirstChildElement(childElementName);
        if (!childElement) {
            return std::nullopt;
        }
        return childElement->getTextAs<T>(defaultValue);
    }

public:
    virtual ~FactoryBase() {}
};

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
template <typename T>
struct FieldPopulator : public FactoryBase {};
#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
