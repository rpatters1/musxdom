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
#include "musx/factory/PoolFactory.h"
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

        std::unique_ptr<musx::xml::IXmlDocument> xmlDocument = std::make_unique<XmlDocumentType>();
        xmlDocument->loadFromString(xmlBuffer);

        auto rootElement = xmlDocument->getRootElement();
        if (!rootElement || rootElement->getTagName() != "finale") {
            throw std::invalid_argument("Missing <finale> element.");
        }

        DocumentPtr document(new Document);

        ElementLinker elementLinker;
        for (auto element = rootElement->getFirstChildElement(); element; element = element->getNextSibling()) {
            if (element->getTagName() == "header") {
                document->getHeader() = musx::factory::HeaderFactory::create(element);
            } else if (element->getTagName() == "options") {
                document->getOptions() = musx::factory::OptionsFactory::create(element, document, elementLinker);
            } else if (element->getTagName() == "others") {
                document->getOthers() = musx::factory::OthersFactory::create(element, document, elementLinker);
            } else if (element->getTagName() == "details") {
                document->getDetails() = musx::factory::DetailsFactory::create(element, document, elementLinker);
            } else if (element->getTagName() == "entries") {
                document->getEntries() = musx::factory::EntryFactory::create(element, document, elementLinker);
            } else if (element->getTagName() == "texts") {
                document->getTexts() = musx::factory::TextsFactory::create(element, document, elementLinker);
            }
        }
        if (!document->getHeader()) document->getHeader() = std::make_shared<musx::dom::Header>();
        if (!document->getOptions()) document->getOptions() = std::make_shared<musx::dom::OptionsPool>();
        if (!document->getOthers()) document->getOthers() = std::make_shared<musx::dom::OthersPool>();
        if (!document->getDetails()) document->getDetails() = std::make_shared<musx::dom::DetailsPool>();
        if (!document->getEntries()) document->getEntries() = std::make_shared<musx::dom::EntryPool>();
        if (!document->getTexts()) document->getTexts() = std::make_shared<musx::dom::TextsPool>();

        elementLinker.resolveAll(document);
#ifdef MUSX_DISPLAY_NODE_NAMES
        util::Logger::log(util::Logger::LogLevel::Verbose, "============");
#endif        
        return document;
    }
};

} // namespace factory
} // namespace musx
