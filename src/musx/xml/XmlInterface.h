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

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <stdexcept>

namespace musx {

/**
 * @namespace musx::xml
 * @brief Provides interfaces and optional implementations for traversing XML.
 */
namespace xml {

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
/**
 * @brief Exception for load xml error.
 */
class load_error : public std::runtime_error
{
public:
    /**
     * @brief Constructor
     */
    load_error(const char *msg) : std::runtime_error(msg) {}
};
#endif // DOXYGEN_SHOULD_IGNORE_THIS

/**
 * @brief Interface for an XML attribute.
 */
class IXmlAttribute
{
public:
    virtual ~IXmlAttribute() = default;

    /**
     * @brief Gets the name of the attribute.
     * @return The name of the attribute.
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Gets the value of the attribute.
     * @return The value of the attribute.
     */
    virtual std::string getValue() const = 0;

    /**
     * @brief Gets the value of the attribute, converted to the specified type.
     * @tparam T The type to which the value should be converted.
     * @return The value converted to the specified type.
     * @throws std::invalid_argument if the conversion fails.
     */
    template <typename T>
    T getValueAs() const {
        std::istringstream iss(getValue());
        T value;
        if (!(iss >> value)) {
            throw std::invalid_argument("Failed to convert attribute value [" + iss.str() + "] to the specified type");
        }
        return value;
    }

    /**
     * @brief Advances to the next attribute.
     * @return A shared pointer to the next attribute, or nullptr if there are no more attributes.
     */
    virtual std::shared_ptr<IXmlAttribute> nextAttribute() const = 0;
};

/**
 * @brief Interface for an XML element.
 */
class IXmlElement
{
public:
    virtual ~IXmlElement() = default;

    /**
     * @brief Gets the tag name of the element.
     * @return The tag name of the element.
     */
    virtual std::string getTagName() const = 0;

    /**
     * @brief Gets the text content of the element.
     * @return The text content of the element.
     */
    virtual std::string getText() const = 0;

    /**
     * @brief Gets the text content of the element, converted to the specified type.
     * @tparam T The type to which the text content should be converted.
     * @param defaultValue The value to return if the text of the element is empty.
     * @return The text content converted to the specified type.
     * @throws std::invalid_argument if the conversion fails.
     */
    template <typename T>
    T getTextAs(T defaultValue = {}) const
    {
        std::istringstream iss(getText());
        if (iss.str().empty()) {
            return defaultValue;
        }

        using ValueType = std::conditional_t<std::is_same_v<T, char> || std::is_same_v<T, uint8_t>, int, T>;
        ValueType value = ValueType(defaultValue);

        if (!(iss >> value)) {
            throw std::invalid_argument("Failed to convert text content [" + iss.str() + "] to the specified type");
        }

        return T(value);
    }

    /**
     * @brief Gets the first attribute.
     * @return A shared pointer to the attribute, or nullptr if not found.
     */
    virtual std::shared_ptr<IXmlAttribute> getFirstAttribute() const = 0;

    /**
     * @brief Finds the first attribute.
     * @param name The name of the attribute to find.
     * @return A shared pointer to the attribute, or nullptr if not found.
     */
    virtual std::shared_ptr<IXmlAttribute> findAttribute(const std::string& name) const = 0;

    /**
     * @brief Finds the first child element.
     * @param tagName [optional] The tag name of the child elements to find. If omitted or empty, finds the first child with any name.
     * @return A shared pointer to the child element, or nullptr if not found.
     */
    virtual std::shared_ptr<IXmlElement> getFirstChildElement(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the next sibling element.
     * @param tagName [optional] The tag name of the sibling elements to find. If omitted or empty, finds the next sibling with any name.
     * @return A shared pointer to the next sibling element, or nullptr if not found.
     */
    virtual std::shared_ptr<IXmlElement> getNextSibling(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the previous sibling element.
     * @param tagName [optional] The tag name of the sibling elements to find. If omitted or empty, finds the previous sibling with any name.
     * @return A shared pointer to the previous sibling element, or nullptr if not found.
     */
    virtual std::shared_ptr<IXmlElement> getPreviousSibling(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the parent element.
     * @return A shared pointer to the parent element, or nullptr if not applicable.
     */
    virtual std::shared_ptr<IXmlElement> getParent() const = 0;
};

/**
 * @brief Interface for an XML document.
 */
class IXmlDocument
{
public:
    virtual ~IXmlDocument() = default;

    /**
     * @brief Loads XML content from a string.
     * @param xmlContent The XML content as a string.
     * @throws musx::xml::load_error if the load fails.
     */
    virtual void loadFromString(const std::string& xmlContent) = 0;

    /**
     * @brief Loads XML content from a vector of characters.
     * @param xmlContent The XML content as a vector of characters.
     * @throws musx::xml::load_error if the load fails.
     */
    virtual void loadFromString(const std::vector<char>& xmlContent) = 0;

    /**
     * @brief Gets the root element of the document.
     * @return A shared pointer to the root element, or nullptr if the document is empty.
     */
    virtual std::shared_ptr<IXmlElement> getRootElement() const = 0;
};

} // namespace xml
} // namespace musx
