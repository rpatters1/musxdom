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

#ifdef MUSX_USE_TINYXML2

#include <string>
#include <memory>

#include "tinyxml2.h"
#include "xml_interface.h"

namespace musx {

/**
 * @namespace musx::tinyxml2
 * @brief Provides an implementation of #musx::IXmlAttribute, #musx::IXmlElement, and #musx::IXmlDocument using the tinyxml2 library.
 *
 * This namespace contains optional implementations of the musx XML interfaces based on the tinyxml2 library.
 * Users can replace this implementation with their own by adhering to the `musx::IXml*` interfaces.
 * This design allows flexibility to use any XML parsing library without modifying the core musx interfaces.
 */
namespace tinyxml2 {

/**
 * @brief Implementation of IXmlAttribute using tinyxml2.
 */
class Attribute : public musx::IXmlAttribute {
    const ::tinyxml2::XMLAttribute* const attribute; ///< Read-only pointer to tinyxml2::XMLAttribute.

public:
    /**
     * @brief Constructor
     */
    explicit Attribute(const ::tinyxml2::XMLAttribute* attr) : attribute(attr) {}

    std::string getName() const override { return attribute->Name(); }
    std::string getValue() const override { return attribute->Value(); }
};

/**
 * @brief Implementation of IXmlElement using tinyxml2.
 */
class Element : public musx::IXmlElement {
    const ::tinyxml2::XMLElement* const element; ///< Read-only pointer to tinyxml2::XMLElement.

public:
    /**
     * @brief Constructor
     */
    explicit Element(const ::tinyxml2::XMLElement* elem) : element(elem) {}

    std::string getTagName() const override { return element->Name(); }

    std::string getText() const override {
        return element->GetText() ? element->GetText() : "";
    }

    std::shared_ptr<IXmlAttribute> getFirstAttribute(const std::string& name = {}) const override {
        const ::tinyxml2::XMLAttribute* attr = name.empty() ? element->FirstAttribute() : element->FindAttribute(name.c_str());
        return attr ? std::make_shared<Attribute>(attr) : nullptr;
    }

    std::shared_ptr<IXmlElement> getFirstChildElement(const std::string& tagName = {}) const override {
        const ::tinyxml2::XMLElement* child = tagName.empty() ? element->FirstChildElement() : element->FirstChildElement(tagName.c_str());
        return child ? std::make_shared<Element>(child) : nullptr;
    }

    std::shared_ptr<IXmlElement> getNextSibling(const std::string& tagName = {}) const override {
        const ::tinyxml2::XMLElement* sibling = tagName.empty() ? element->NextSiblingElement() : element->NextSiblingElement(tagName.c_str());
        return sibling ? std::make_shared<Element>(sibling) : nullptr;
    }

    std::shared_ptr<IXmlElement> getPreviousSibling(const std::string& tagName = {}) const override {
        if (!element->Parent() || !element->Parent()->ToElement()) return nullptr;
        const ::tinyxml2::XMLElement* parent = element->Parent()->ToElement()->FirstChildElement();
        const ::tinyxml2::XMLElement* previous = nullptr;

        while (parent && parent != element) {
            if (tagName.empty() || tagName == parent->Name()) {
                previous = parent;
            }
            parent = parent->NextSiblingElement();
        }
        return previous ? std::make_shared<Element>(previous) : nullptr;
    }

    std::shared_ptr<IXmlElement> getParent() const override {
        if (!element->Parent() || !element->Parent()->ToElement()) return nullptr;
        return std::make_shared<Element>(element->Parent()->ToElement());
    }
};

/**
 * @brief Implementation of IXmlDocument using tinyxml2.
 */
class Document : public musx::IXmlDocument {
    ::tinyxml2::XMLDocument doc; ///< Non-const since the document itself may be parsed or reset.

public:
    void loadFromString(const std::string& xmlContent) override {
        doc.Parse(xmlContent.c_str());
    }

    std::shared_ptr<IXmlElement> getRootElement() const override {
        const ::tinyxml2::XMLElement* root = doc.RootElement();
        return root ? std::make_shared<Element>(root) : nullptr;
    }
};

} // namespace tinyxml2
} // namespace musx

#endif // MUSX_USE_TINYXML2