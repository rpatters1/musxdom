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

#ifdef MUSX_USE_RAPIDXML

#include <string>
#include <memory>
#include <vector>
#include <exception>

#include "rapidxml.hpp"
#include "XmlInterface.h"

namespace musx {
namespace xml {

/**
 * @namespace musx::xml::rapidxml
 * @brief Provides an implementation of #musx::xml::IXmlAttribute, #musx::xml::IXmlElement, and #musx::xml::IXmlDocument using the rapidxml library.
 *
 * This namespace contains optional implementations of the musx XML interfaces based on the rapidxml library.
 * Users can replace this implementation with their own by adhering to the `musx::xml::IXml*` interfaces.
 * This design allows flexibility to use any XML parsing library without modifying the core musx interfaces.
 */
namespace rapidxml {

/**
 * @brief Implementation of IXmlAttribute using rapidxml.
 */
class Attribute : public musx::xml::IXmlAttribute
{
    ::rapidxml::xml_attribute<>* attribute; ///< Pointer to rapidxml::xml_attribute.

public:
    /**
     * @brief Constructor
     */
    explicit Attribute(::rapidxml::xml_attribute<>* attr) : attribute(attr) {}

    std::string getName() const override { return attribute->name(); }
    std::string getValue() const override { return attribute->value(); }

    std::shared_ptr<IXmlAttribute> nextAttribute() const override {
        ::rapidxml::xml_attribute<>* next = attribute->next_attribute();
        return next ? std::make_shared<Attribute>(next) : nullptr;
    }
};

/**
 * @brief Implementation of IXmlElement using rapidxml.
 */
class Element : public musx::xml::IXmlElement
{
    ::rapidxml::xml_node<>* element; ///< Pointer to rapidxml::xml_node.

    static const char* tagPtr(const std::string& tagName) {
        return tagName.empty() ? nullptr : tagName.c_str();
    }

public:
    /**
     * @brief Constructor
     */
    explicit Element(::rapidxml::xml_node<>* elem) : element(elem) {}

    std::string getTagName() const override { return element->name(); }

    std::string getText() const override {
        return element->value() ? element->value() : "";
    }

    std::shared_ptr<IXmlAttribute> getFirstAttribute() const override {
        ::rapidxml::xml_attribute<>* attr = element->first_attribute();
        return attr ? std::make_shared<Attribute>(attr) : nullptr;
    }

    std::shared_ptr<IXmlAttribute> findAttribute(const std::string& tagName) const override {
        ::rapidxml::xml_attribute<>* attr = element->first_attribute(tagName.c_str());
        return attr ? std::make_shared<Attribute>(attr) : nullptr;
    }

    std::shared_ptr<IXmlElement> getFirstChildElement(const std::string& tagName = {}) const override {
        ::rapidxml::xml_node<>* child = element->first_node(tagPtr(tagName));
        return child ? std::make_shared<Element>(child) : nullptr;
    }

    std::shared_ptr<IXmlElement> getNextSibling(const std::string& tagName = {}) const override {
        ::rapidxml::xml_node<>* sibling = element->next_sibling(tagPtr(tagName));
        return sibling ? std::make_shared<Element>(sibling) : nullptr;
    }

    std::shared_ptr<IXmlElement> getPreviousSibling(const std::string& tagName = {}) const override {
        // rapidxml does not directly support previous sibling, we need to iterate from the parent.
        if (!element->parent()) return nullptr;

        ::rapidxml::xml_node<>* prev = nullptr;
        for (::rapidxml::xml_node<>* sibling = element->parent()->first_node(); sibling; sibling = sibling->next_sibling()) {
            if (sibling == element) break;
            if (tagName.empty() || sibling->name() == tagName) prev = sibling;
        }
        return prev ? std::make_shared<Element>(prev) : nullptr;
    }

    std::shared_ptr<IXmlElement> getParent() const override {
        ::rapidxml::xml_node<>* parent = element->parent();
        return parent && parent->type() == ::rapidxml::node_element ? std::make_shared<Element>(parent) : nullptr;
    }
};

/**
 * @brief Implementation of IXmlDocument using rapidxml.
 */
class Document : public musx::xml::IXmlDocument
{
    ::rapidxml::xml_document<> doc; ///< The rapidxml document.
    std::vector<char> buffer; ///< Buffer for the document content.

public:
    void loadFromString(const std::string& xmlContent) override {
        buffer.assign(xmlContent.begin(), xmlContent.end());
        buffer.push_back('\0'); // Null-terminate the buffer.

        try {
            doc.parse<0>(buffer.data());
        } catch (const ::rapidxml::parse_error& e) {
            throw musx::xml::load_error(e.what());
        }
    }

    void loadFromString(const std::vector<char>& xmlContent) override {
        buffer = xmlContent;
        buffer.push_back('\0'); // Null-terminate the buffer.

        try {
            doc.parse<0>(buffer.data());
        } catch (const ::rapidxml::parse_error& e) {
            throw musx::xml::load_error(e.what());
        }
    }

    std::shared_ptr<IXmlElement> getRootElement() const override {
        ::rapidxml::xml_node<>* root = doc.first_node();
        return root ? std::make_shared<Element>(root) : nullptr;
    }
};

} // namespace rapidxml
} // namespace xml
} // namespace musx

#endif // MUSX_USE_RAPIDXML
