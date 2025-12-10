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

#ifdef MUSX_USE_QTXML

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QDomAttr>
#include <QByteArray>
#include <memory>
#include "XmlInterface.h"

namespace musx {
namespace xml {

/**
 * @namespace musx::xml::qt
 * @brief Provides an implementation of #musx::xml::IXmlAttribute, #musx::xml::IXmlElement, and #musx::xml::IXmlDocument using Qt's QDom classes.
 */
namespace qt {

/**
 * @brief Implementation of IXmlAttribute using Qt's QDomAttr.
 */
class Attribute : public musx::xml::IXmlAttribute {
    QDomAttr m_attr;
    int m_index;

public:
    Attribute(const QDomAttr& attr, int index)
        : m_attr(attr), m_index(index) {}

    std::string getName() const override {
        return m_attr.name().toStdString();
    }

    std::string getValue() const override {
        return m_attr.value().toStdString();
    }

    std::shared_ptr<IXmlAttribute> nextAttribute() const override {
        QDomNamedNodeMap attributes = m_attr.parentNode().attributes();
        if (m_index + 1 >= attributes.count()) {
            return nullptr;
        }
        QDomAttr nextAttr = attributes.item(m_index + 1).toAttr();
        if (nextAttr.isNull()) { // [[unlikely]]
            return nullptr;
        }
        return std::make_shared<Attribute>(nextAttr, m_index + 1);
    }
};

/**
 * @brief Implementation of IXmlElement using Qt's QDomElement.
 */
class Element : public musx::xml::IXmlElement {
    const QDomElement m_element;

public:
    explicit Element(const QDomElement& elem) : m_element(elem) {}

    std::string getTagName() const override { return m_element.tagName().toStdString(); }

    std::string getText() const override { return m_element.text().toStdString(); }

    std::shared_ptr<IXmlAttribute> getFirstAttribute() const override {
        QDomNamedNodeMap attributes = m_element.attributes();
        if (attributes.isEmpty()) {
            return nullptr;
        }
        QDomAttr attr = attributes.item(0).toAttr();
        if (attr.isNull()) { // [[unlikely]]
            return nullptr;
        }
        return std::make_shared<Attribute>(attr, 0);
    }

    std::shared_ptr<IXmlAttribute> findAttribute(const std::string& name) const override {
        // work around Qt bug that attributeNode is not marked const
        QDomElement& nonConstElement = const_cast<QDomElement&>(m_element);
        QDomAttr attr = nonConstElement.attributeNode(QString::fromStdString(name));
        return attr.isNull() ? nullptr : std::make_shared<Attribute>(attr, 0);
    }

    std::shared_ptr<IXmlElement> getFirstChildElement(const std::string& tagName = {}) const override {
        QDomNode child = tagName.empty()
            ? m_element.firstChildElement()
            : m_element.firstChildElement(QString::fromStdString(tagName));
        return child.isNull() ? nullptr : std::make_shared<Element>(child.toElement());
    }

    std::shared_ptr<IXmlElement> getNextSibling(const std::string& tagName = {}) const override {
        QDomNode sibling = m_element.nextSiblingElement(QString::fromStdString(tagName));
        if (tagName.empty() && sibling.isNull()) {
            sibling = m_element.nextSibling();
            while (!sibling.isNull() && !sibling.isElement()) {
                sibling = sibling.nextSibling();
            }
        }
        return sibling.isNull() ? nullptr : std::make_shared<Element>(sibling.toElement());
    }

    std::shared_ptr<IXmlElement> getPreviousSibling(const std::string& tagName = {}) const override {
        QDomNode sibling = m_element.previousSiblingElement(QString::fromStdString(tagName));
        if (tagName.empty() && sibling.isNull()) {
            sibling = m_element.previousSibling();
            while (!sibling.isNull() && !sibling.isElement()) {
                sibling = sibling.previousSibling();
            }
        }
        return sibling.isNull() ? nullptr : std::make_shared<Element>(sibling.toElement());
    }

    std::shared_ptr<IXmlElement> getParent() const override {
        QDomNode parent = m_element.parentNode();
        return parent.isNull() || !parent.isElement() ? nullptr : std::make_shared<Element>(parent.toElement());
    }
};

/**
 * @brief Implementation of IXmlDocument using Qt's QDomDocument.
 */
class Document : public musx::xml::IXmlDocument {
    QDomDocument m_document;

public:
    void loadFromString(const std::string& xmlContent) override {
        QString errorMsg;
        int errorLine = 0, errorColumn = 0;
        if (!m_document.setContent(QString::fromUtf8(xmlContent.c_str()), &errorMsg, &errorLine, &errorColumn)) {
            throw musx::xml::load_error("Error parsing XML at line " + std::to_string(errorLine) +
                                        ", column " + std::to_string(errorColumn) + ": " + errorMsg.toStdString());
        }
    }

    void loadFromBuffer(const char * data, size_t size) override {
        QString errorMsg;
        int errorLine = 0, errorColumn = 0;
        if (!m_document.setContent(QByteArray(data, size), &errorMsg, &errorLine, &errorColumn)) {
            throw musx::xml::load_error("Error parsing XML at line " + std::to_string(errorLine) +
                                        ", column " + std::to_string(errorColumn) + ": " + errorMsg.toStdString());
        }
    }

    std::shared_ptr<IXmlElement> getRootElement() const override {
        QDomElement root = m_document.documentElement();
        return root.isNull() ? nullptr : std::make_shared<Element>(root);
    }
};

} // namespace qt
} // namespace xml
} // namespace musx

#endif // MUSX_USE_QTXML
