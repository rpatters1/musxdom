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

#include <string>
#include <memory>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <functional>

// Do not add header dependencies from musx.

namespace musx {

/**
 * @namespace musx::xml
 * @brief Provides interfaces and optional implementations for traversing XML.
 */
namespace xml {

/**
 * @brief Exception for load xml error.
 */
class load_error : public std::runtime_error
{
public:
    using std::runtime_error::runtime_error;
};

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
/** @brief trims whitespace from a string */
inline std::string trim(const std::string& str)
{
    auto start = std::find_if_not(str.begin(), str.end(), ::isspace);
    auto end = std::find_if_not(str.rbegin(), str.rend(), ::isspace).base();
    return (start < end) ? std::string(start, end) : std::string();
}
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
     * @brief Gets the text content of the attribute with whitespace trimmed.
     */
    std::string getValueTrimmed() const { return trim(getValue()); }

    /**
     * @brief Gets the value of the attribute, converted to the specified type.
     * @tparam T The type to which the value should be converted.
     * @return The value converted to the specified type.
     * @throws std::invalid_argument if the conversion fails.
     */
    template <typename T>
    T getValueAs() const {
        std::istringstream iss(getValueTrimmed());
        T value;
        if constexpr (std::is_same_v<T, bool>) {
            std::string str;
            iss >> str;
            std::transform(str.begin(), str.end(), str.begin(), ::tolower); // Ensure case insensitivity
            if (str == "true") {
                return true;
            }
            else if (str == "false") {
                return false;
            }
        }
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

class IXmlElement;
using XmlElementPtr = std::shared_ptr<IXmlElement>; ///< shared pointer to @ref IXmlElement
template <typename T>
using XmlElementPopulator = std::function<void(const XmlElementPtr&, const std::shared_ptr<T>&)>; ///< function type for populating a field from an @ref IXmlElement
template <typename T>
using XmlElementDescriptor = std::tuple<const std::string_view, XmlElementPopulator<T>>; ///< associates an xml node name with and @ref XmlElementPopulator
template <typename T>
using XmlElementArray = std::vector<XmlElementDescriptor<T>>; ///< an array type for @ref XmlElementDescriptor instances.

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
     * @brief Gets the text content of the element with whitespace trimmed.
     */
    std::string getTextTrimmed() const { return trim(getText()); }

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
        static_assert(!std::is_same_v<T, bool>, "Do not use getTextAs with bool type. Simply assign true. (The presence of the node means true.)");

        std::istringstream iss(getTextTrimmed());
        if (iss.str().empty()) {
            return defaultValue;
        }

        using ValueType = std::conditional_t<
                            std::is_same_v<T, char>
                            || std::is_same_v<T, uint8_t>
                            || std::is_same_v<T, char16_t>
                            || std::is_same_v<T, char32_t>, int, T>;
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
    virtual XmlElementPtr getFirstChildElement(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the next sibling element.
     * @param tagName [optional] The tag name of the sibling elements to find. If omitted or empty, finds the next sibling with any name.
     * @return A shared pointer to the next sibling element, or nullptr if not found.
     */
    virtual XmlElementPtr getNextSibling(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the previous sibling element.
     * @param tagName [optional] The tag name of the sibling elements to find. If omitted or empty, finds the previous sibling with any name.
     * @return A shared pointer to the previous sibling element, or nullptr if not found.
     */
    virtual XmlElementPtr getPreviousSibling(const std::string& tagName = {}) const = 0;

    /**
     * @brief Gets the parent element.
     * @return A shared pointer to the parent element, or nullptr if not applicable.
     */
    virtual XmlElementPtr getParent() const = 0;
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
