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

#include <cstddef>
#include <memory>
#include <optional>
#include <type_traits>
#include <utility>
#include <vector>

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
    template <typename Container>
    using IsCharContainer = std::enable_if_t<
        (sizeof(std::remove_cv_t<typename Container::value_type>) == 1)
        && !std::is_same_v<std::remove_cv_t<typename Container::value_type>, bool>
    >;

    /// @brief Optional arguments for document creation.
    struct CreateOptions
    {
        CreateOptions() = default;
        CreateOptions(dom::PartVoicingPolicy policy) : partVoicingPolicy(policy) {}
        CreateOptions(std::vector<char>&& notationMetadata)
            : m_notationMetadata(std::move(notationMetadata)) {}

        template <typename Container, typename = IsCharContainer<Container>>
        CreateOptions(const Container& notationMetadata)
        {
            setNotationMetadata(notationMetadata);
        }

        dom::PartVoicingPolicy partVoicingPolicy = dom::PartVoicingPolicy::Ignore; ///< Part voicing behavior for this document.

        /// @brief Optional NotationMetadata.xml content.
        [[nodiscard]]
        const std::vector<char>& getNotationMetadata() const { return m_notationMetadata; }

        template <typename Container, typename = IsCharContainer<Container>>
        void setNotationMetadata(const Container& notationMetadata)
        {
            const auto* data = reinterpret_cast<const char*>(notationMetadata.data());
            m_notationMetadata.assign(data, data + notationMetadata.size());
        }

        void setNotationMetadata(std::vector<char>&& notationMetadata)
        {
            m_notationMetadata = std::move(notationMetadata);
        }

    private:
        std::vector<char> m_notationMetadata;
    };

    /**
     * @brief Creates a `Document` object from an XML buffer.
     *
     * @param data Pointer to a buffer containing EnigmaXML for a musx file.
     * @param size The size of the buffer.
     * @param createOptions Optional creation options, including part voicing policy and NotationMetadata.xml data.
     * @return A fully populated `Document` object.
     * @throws std::invalid_argument If required nodes or attributes are missing or invalid.
     */
    template <typename XmlDocumentType>
    [[nodiscard]]
    static DocumentPtr create(const char* data, size_t size)
    {
        return create<XmlDocumentType>(data, size, CreateOptions{});
    }

    /// @brief Creates a `Document` object from a char container (e.g. std::string, std::vector<char>).
    template <typename XmlDocumentType, typename Container, typename = IsCharContainer<Container>>
    [[nodiscard]]
    static DocumentPtr create(const Container& xmlBuffer)
    {
        return create<XmlDocumentType>(asCharData(xmlBuffer), xmlBuffer.size(), CreateOptions{});
    }

    /**
     * @brief Creates a `Document` object from an XML buffer.
     *
     * @param data Pointer to a buffer containing EnigmaXML for a musx file.
     * @param size The size of the buffer.
     * @param createOptions Optional creation options, including part voicing policy and NotationMetadata.xml data.
     * @return A fully populated `Document` object.
     * @throws std::invalid_argument If required nodes or attributes are missing or invalid.
     */
    template <typename XmlDocumentType>
    [[nodiscard]]
    static DocumentPtr create(const char* data, size_t size, const CreateOptions& createOptions)
    {
        static_assert(std::is_base_of<musx::xml::IXmlDocument, XmlDocumentType>::value,
                      "XmlReaderType must derive from IXmlDocument.");

        std::unique_ptr<musx::xml::IXmlDocument> xmlDocument = std::make_unique<XmlDocumentType>();
        xmlDocument->loadFromBuffer(data, size);

        auto rootElement = xmlDocument->getRootElement();
        if (!rootElement || rootElement->getTagName() != "finale") {
            throw std::invalid_argument("Missing <finale> element.");
        }

        DocumentPtr document(new Document);
        document->m_self = document;
        document->m_partVoicingPolicy = createOptions.partVoicingPolicy;
        document->m_scoreDurationSeconds = parseScoreDurationSeconds<XmlDocumentType>(createOptions.getNotationMetadata());

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
        if (!document->getOptions()) document->getOptions() = std::make_shared<musx::dom::OptionsPool>(document);
        if (!document->getOthers()) document->getOthers() = std::make_shared<musx::dom::OthersPool>(document);
        if (!document->getDetails()) document->getDetails() = std::make_shared<musx::dom::DetailsPool>(document);
        if (!document->getEntries()) document->getEntries() = std::make_shared<musx::dom::EntryPool>(document);
        if (!document->getTexts()) document->getTexts() = std::make_shared<musx::dom::TextsPool>(document);

#ifdef MUSX_DISPLAY_NODE_NAMES
        util::Logger::log(util::Logger::LogLevel::Verbose, "============");
#endif
        elementLinker.resolveAll(document);
        document->m_instruments = document->createInstrumentMap(SCORE_PARTID);

        document->m_maxBlankPages = 0;
        auto linkedParts = document->getOthers()->getArray<PartDefinition>(SCORE_PARTID);
        for (const auto& part : linkedParts) {
            auto mutablePart = const_cast<PartDefinition*>(part.get());
            mutablePart->numberOfLeadingBlankPages = 0;
            auto pages = document->getOthers()->getArray<Page>(part->getCmper());
            mutablePart->numberOfPages = int(pages.size());
            for (const auto& page : pages) {
                if (!page->isBlank()) {
                    break;
                }
                mutablePart->numberOfLeadingBlankPages++;
            }
            if (mutablePart->numberOfLeadingBlankPages > document->m_maxBlankPages) {
                document->m_maxBlankPages = part->numberOfLeadingBlankPages;
            }
        }

        return document;
    }

    /// @brief Creates a `Document` object from a char container and explicit create options.
    template <typename XmlDocumentType, typename Container, typename = IsCharContainer<Container>>
    [[nodiscard]]
    static DocumentPtr create(const Container& xmlBuffer, const CreateOptions& createOptions)
    {
        return create<XmlDocumentType>(asCharData(xmlBuffer), xmlBuffer.size(), createOptions);
    }

private:
    template <typename Container>
    static const char* asCharData(const Container& buffer)
    {
        return reinterpret_cast<const char*>(buffer.data());
    }

    template <typename XmlDocumentType>
    static std::optional<double> parseScoreDurationSeconds(const std::vector<char>& notationMetadata)
    {
        if (notationMetadata.empty()) {
            return std::nullopt;
        }

        std::unique_ptr<musx::xml::IXmlDocument> xmlDocument = std::make_unique<XmlDocumentType>();
        try {
            xmlDocument->loadFromBuffer(notationMetadata.data(), notationMetadata.size());
        } catch (...) {
            return std::nullopt;
        }

        auto rootElement = xmlDocument->getRootElement();
        if (!rootElement || rootElement->getTagName() != "metadata") {
            return std::nullopt;
        }
        if (auto fileInfo = rootElement->getFirstChildElement("fileInfo")) {
            auto creatorString = fileInfo->getFirstChildElement("creatorString");
            if (!creatorString || creatorString->getTextTrimmed().rfind("Finale", 0) != 0) {
                return std::nullopt;
            }
            if (auto scoreDuration = fileInfo->getFirstChildElement("scoreDuration")) {
                try {
                    return scoreDuration->getTextAs<double>();
                } catch (...) {
                    return std::nullopt;
                }
            }
        }
        return std::nullopt;
    }
};

} // namespace factory
} // namespace musx
