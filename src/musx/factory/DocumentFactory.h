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

#include "musx/dom/Document.h"
#include "musx/factory/HeaderFactory.h"
#include "musx/factory/OthersFactory.h"
#include "musx/xml/XmlInterface.h"

namespace musx {
namespace factory {

/**
 * @brief Factory class for creating `Document` objects from XML.
 */
class DocumentFactory : FactoryBase
{
    using Document = musx::dom::Document;
    using DocumentPtr = std::shared_ptr<Document>;

public:
    /**
     * @brief Creates a `Document` object from an XML element.
     * 
     * @param xmlBuffer Buffer containing EnigmaXML for a musx file.
     * @return A fully populated `Document` object.
     * @throws std::invalid_argument If required nodes or attributes are missing or invalid.
     */
    template <typename XmlDocumentType>
    static DocumentPtr create(const std::vector<char>& xmlBuffer)
    {
        static_assert(std::is_base_of<musx::xml::IXmlDocument, XmlDocumentType>::value, 
                      "XmlReaderType must derive from IXmlDocument.");

        std::unique_ptr<musx::xml::IXmlDocument> document = std::make_unique<XmlDocumentType>();
        document->loadFromString(xmlBuffer);

        auto rootElement = document->getRootElement();
        if (!rootElement || rootElement->getTagName() != "finale") {
            throw std::invalid_argument("Missing <finale> element.");
        }

        auto headerElement = getFirstChildElement(rootElement, "header");
        auto header = musx::factory::HeaderFactory::create(headerElement);

        auto othersElement = getFirstChildElement(rootElement, "others");
        auto othersPool = musx::factory::OthersFactory::create(othersElement);

        return DocumentPtr(new Document(std::move(header), othersPool));
    }
};

} // namespace factory
} // namespace musx
