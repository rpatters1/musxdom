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

#ifdef MUSX_USE_PUGIXML

#include <string>
#include <memory>
#include <exception>
#include "pugixml.hpp"
#include "XmlInterface.h"

namespace musx {
namespace xml {

/**
 * @namespace musx::xml::pugi
 * @brief Provides an implementation of #musx::xml::IXmlAttribute, #musx::xml::IXmlElement, and #musx::xml::IXmlDocument using the PugiXML library.
 *
 * This namespace contains optional implementations of the musx XML interfaces based on the PugiXML library.
 * Users can replace this implementation with their own by adhering to the `musx::xml::IXml*` interfaces.
 * This design allows flexibility to use any XML parsing library without modifying the core musx interfaces.
 */
namespace pugi {

/**
 * @brief Implementation of IXmlAttribute using PugiXML.
 */
class Attribute : public musx::xml::IXmlAttribute {
    const ::pugi::xml_attribute m_attribute; ///< Read-only PugiXML attribute.

public:
    /**
     * @brief Constructor
     */
    explicit Attribute(::pugi::xml_attribute attr) : m_attribute(attr) {}

    std::string getName() const override { return m_attribute.name(); }
    std::string getValue() const override { return m_attribute.value(); }

    std::shared_ptr<IXmlAttribute> nextAttribute() const override {
        ::pugi::xml_attribute next = m_attribute.next_attribute();
        return next ? std::make_shared<Attribute>(next) : nullptr;
    }
};

/**
 * @brief Implementation of IXmlElement using PugiXML.
 */
class Element : public musx::xml::IXmlElement {
    const ::pugi::xml_node m_element; ///< Read-only PugiXML node.

    static const char* tagPtr(const std::string& tagName) {
        return tagName.empty() ? nullptr : tagName.c_str();
    }

public:
    /**
     * @brief Constructor
     */
    explicit Element(::pugi::xml_node elem) : m_element(elem) {}

    std::string getTagName() const override { return m_element.name(); }

    std::string getText() const override {
        return m_element.child_value();
    }

    std::shared_ptr<IXmlAttribute> getFirstAttribute() const override {
        ::pugi::xml_attribute attr = m_element.first_attribute();
        return attr ? std::make_shared<Attribute>(attr) : nullptr;
    }

    std::shared_ptr<IXmlAttribute> findAttribute(const std::string& name) const override {
        ::pugi::xml_attribute attr = m_element.attribute(name.c_str());
        return attr ? std::make_shared<Attribute>(attr) : nullptr;
    }

    std::shared_ptr<IXmlElement> getFirstChildElement(const std::string& tagName = {}) const override {
        ::pugi::xml_node child = tagPtr(tagName) ? m_element.child(tagPtr(tagName)) : m_element.first_child();
        return child ? std::make_shared<Element>(child) : nullptr;
    }

    std::shared_ptr<IXmlElement> getNextSibling(const std::string& tagName = {}) const override {
        ::pugi::xml_node sibling = tagPtr(tagName) ? m_element.next_sibling(tagPtr(tagName)) : m_element.next_sibling();
        return sibling ? std::make_shared<Element>(sibling) : nullptr;
    }

    std::shared_ptr<IXmlElement> getPreviousSibling(const std::string& tagName = {}) const override {
        ::pugi::xml_node sibling = tagPtr(tagName) ? m_element.previous_sibling(tagPtr(tagName)) : m_element.previous_sibling();
        return sibling ? std::make_shared<Element>(sibling) : nullptr;
    }

    std::shared_ptr<IXmlElement> getParent() const override {
        ::pugi::xml_node parent = m_element.parent();
        return parent && parent.type() == ::pugi::node_element ? std::make_shared<Element>(parent) : nullptr;
    }
};

/**
 * @brief Implementation of IXmlDocument using PugiXML.
 */
class Document : public musx::xml::IXmlDocument {
    ::pugi::xml_document m_document; ///< PugiXML document.

public:
    void loadFromString(const std::string& xmlContent) override {
        ::pugi::xml_parse_result result = m_document.load_string(xmlContent.c_str());
        if (!result) {
            throw musx::xml::load_error(result.description());
        }
    }

    void loadFromString(const std::vector<char>& xmlContent) override {
        ::pugi::xml_parse_result result = m_document.load_buffer(xmlContent.data(), xmlContent.size());
        if (!result) {
            throw musx::xml::load_error(result.description());
        }
    }

    std::shared_ptr<IXmlElement> getRootElement() const override {
        ::pugi::xml_node root = m_document.document_element();
        return root ? std::make_shared<Element>(root) : nullptr;
    }
};

} // namespace pugi
} // namespace xml
} // namespace musx

#endif // MUSX_USE_PUGIXML
