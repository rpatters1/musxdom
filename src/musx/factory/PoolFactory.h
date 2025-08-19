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

#include "musx/xml/XmlInterface.h"
#include "musx/dom/BaseClasses.h"
#include "musx/dom/ObjectPool.h"
#include "musx/dom/Document.h"
#include "FactoryBase.h"
#include "TypeRegistry.h"

#ifdef MUSX_DISPLAY_NODE_NAMES
#include <set>
#endif

namespace musx {
namespace factory {

/**
 * @brief Factory class for creating a `dom::ObjectPool` from XML.
 *
 * This factory class uses CRTP (Curiously Recurring Template Pattern) to implement a polymorphic factory for creating 
 * `ObjectPool` objects from an XML representation. The derived classes are responsible for specifying their own 
 * creation mechanisms by providing appropriate specializations.
 *
 * @tparam DerivedType The derived type for CRTP, which will implement specific extraction behavior from XML.
 * @tparam ObjectBase The base type of the objects that are stored in the pool, providing a common interface.
 * @tparam PoolType The type of the pool that stores the objects, such as a collection of `ObjectBase` derived objects.
 */
template<typename DerivedType, typename ObjectBase, typename PoolType>
class PoolFactory : public FactoryBase
{
//    static_assert(std::is_base_of<PoolFactory<DerivedType, DerivedType>::value, 
//                "ObjectBase must derive from PoolFactory.");

public:    
    /**
     * @brief Creates a `OthersPool` object from an XML element.
     *
     * This function creates an object pool by parsing the XML element representing the `<others>` node.
     * It iterates over each child element of the XML element, extracts relevant information using 
     * `DerivedType::extractFromXml`, and adds the created objects to the pool.
     *
     * @param element The XML element representing the `<others>` node.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker A class for storing deferred linkage commands.
     * @return A fully populated `ObjectPoolType` object.
     */
    static std::shared_ptr<PoolType> create(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker)
    {
        auto pool = std::make_shared<PoolType>(document);

#ifdef MUSX_DISPLAY_NODE_NAMES
        std::string currentTag;
        size_t currentTagCount = 0;
        util::Logger::log(util::Logger::LogLevel::Verbose, "============");
        util::Logger::log(util::Logger::LogLevel::Verbose, element->getTagName());
        util::Logger::log(util::Logger::LogLevel::Verbose, "============");
#endif

        for (auto childElement = element->getFirstChildElement(); childElement; childElement = childElement->getNextSibling()) {            
            if (auto instanceInfo = DerivedType::extractFromXml(childElement, document, elementLinker, pool)) {
#ifdef MUSX_DISPLAY_NODE_NAMES
                if (currentTag != childElement->getTagName()) {
                    if (!currentTag.empty()) {
                        util::Logger::log(util::Logger::LogLevel::Verbose, "  " + currentTag + " [" + std::to_string(currentTagCount) + "]");
                    }
                    currentTag = childElement->getTagName();
                    currentTagCount = 0;
                }
                currentTagCount++;
#endif
                MUSX_ASSERT_IF(childElement->getTagName() != instanceInfo->xmlNodeName) {
                    throw std::logic_error("Instance of " + std::string(instanceInfo->xmlNodeName) + " does not match xml tag " + element->getTagName());
                }
                auto typedPtr = std::dynamic_pointer_cast<ObjectBase>(instanceInfo->instance);
                MUSX_ASSERT_IF(!typedPtr) {
                    throw std::logic_error("Unable to cast instance to correct type for " + std::string(instanceInfo->xmlNodeName));
                }
                if constexpr (std::is_same_v<PoolType, EntryPool>) {
                    pool->add(typedPtr->getEntryNumber(), typedPtr);
                } else {
                    pool->add(instanceInfo->xmlNodeName, typedPtr);
                }
            }
        }

#ifdef MUSX_DISPLAY_NODE_NAMES
        if (!currentTag.empty() && currentTagCount != 0) {
            util::Logger::log(util::Logger::LogLevel::Verbose, "  " + currentTag + " [" + std::to_string(currentTagCount) + "]");
        }
#endif

        return pool;
    }
};

/**
 * @brief Factory class for creating `Options` objects from XML.
 *
 * This class specializes `PoolFactory` to handle the creation of `Options` objects, which 
 * represent various document options stored in a `ScalarPool`. It includes functionality 
 * for extracting and creating these objects from XML elements.
 */
class OptionsFactory : public PoolFactory<OptionsFactory, dom::OptionsBase, dom::OptionsPool>
{
public:
    using PoolFactory::create;

    /**
     * @brief Extracts an `OptionsBase` object from an XML element.
     *
     * Extracts an `OptionsBase` derived object from the given XML element by delegating 
     * to the `RegisteredOptions` class. This allows the creation of `Options` objects from XML.
     *
     * @param element The XML element from which to extract the object.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param pool The pool we are constructiong. (It hasn't been assigned to @p document yet.)
     * @return A shared pointer to the created object.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker, const OptionsPoolPtr& pool)
    {
        return RegisteredOptions::createInstance(pool, element, elementLinker, document);
    }
};

/**
 * @brief Factory class for creating `Others` objects from XML.
 *
 * This class specializes `PoolFactory` to handle the creation of `Others` objects, representing 
 * various attributes stored in an `OthersPool`. It includes an XML parsing mechanism to 
 * extract and create these objects, which are used in the document model.
 */
class OthersFactory : public PoolFactory<OthersFactory, dom::OthersBase, dom::OthersPool>
{
public:
    using PoolFactory::create;

    /**
     * @brief Extracts an `OthersBase` object from an XML element.
     *
     * Extracts an `OthersBase` derived object from the given XML element using the specified 
     * attributes `cmper` and `inci`. Throws an exception if a required attribute is missing.
     *
     * @param element The XML element from which to extract the object.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param pool The pool we are constructiong. (It hasn't been assigned to @p document yet.)
     * @return A shared pointer to the created object.
     * @throws std::invalid_argument if required attributes are missing.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker, const OthersPoolPtr& pool)
    {
        auto cmperAttribute = element->findAttribute("cmper");
        if (!cmperAttribute) {
            throw std::invalid_argument("missing cmper for others element " + element->getTagName());
        }
        auto inciAttribute = element->findAttribute("inci");
        if (inciAttribute) {
            return RegisteredOthers::createInstance(pool, element, elementLinker,
                document, cmperAttribute->getValueAs<dom::Cmper>(), inciAttribute->getValueAs<dom::Inci>());
        }
        else {
            return RegisteredOthers::createInstance(pool, element, elementLinker,
                document, cmperAttribute->getValueAs<dom::Cmper>());
        }
    }
};

/**
 * @brief Factory class for creating `Details` objects from XML.
 *
 * This class specializes `PoolFactory` to handle the creation of `Others` objects, representing 
 * various attributes stored in an `OthersPool`. It includes an XML parsing mechanism to 
 * extract and create these objects, which are used in the document model.
 */
class DetailsFactory : public PoolFactory<DetailsFactory, dom::DetailsBase, dom::DetailsPool>
{
public:
    using PoolFactory::create;

    /**
     * @brief Extracts an `OthersBase` object from an XML element.
     *
     * Extracts an `OthersBase` derived object from the given XML element using the specified 
     * attributes `cmper` and `inci`. Throws an exception if a required attribute is missing.
     *
     * @param element The XML element from which to extract the object.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param pool The pool we are constructiong. (It hasn't been assigned to @p document yet.)
     * @return A shared pointer to the created object.
     * @throws std::invalid_argument if required attributes are missing.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker, const DetailsPoolPtr& pool)
    {
        std::optional<dom::Inci> inci;
        if (auto inciAttribute = element->findAttribute("inci")) {
            inci = inciAttribute->getValueAs<dom::Inci>();
        }
        if (auto entnumAttribute = element->findAttribute("entnum")) {
            if (inci.has_value()) {
                return RegisteredDetails::createInstance(pool, element, elementLinker,
                    document, entnumAttribute->getValueAs<dom::EntryNumber>(), inci.value());
            } else {
                return RegisteredDetails::createInstance(pool, element, elementLinker,
                    document, entnumAttribute->getValueAs<dom::EntryNumber>());
            }
        }
        auto cmper1Attribute = element->findAttribute("cmper1");
        if (!cmper1Attribute) {
            throw std::invalid_argument("missing cmper1 for details element " + element->getTagName());
        }
        auto cmper2Attribute = element->findAttribute("cmper2");
        if (!cmper2Attribute) {
            throw std::invalid_argument("missing cmper2 for details element " + element->getTagName());
        }
        if (inci.has_value()) {
            return RegisteredDetails::createInstance(pool, element, elementLinker,
                document, cmper1Attribute->getValueAs<dom::Cmper>(), cmper2Attribute->getValueAs<dom::Cmper>(), inci.value());
        } else {
            return RegisteredDetails::createInstance(pool, element, elementLinker,
                document, cmper1Attribute->getValueAs<dom::Cmper>(), cmper2Attribute->getValueAs<dom::Cmper>());
        }
    }
};

/**
 * @brief Factory class for creating `Entry` objects from XML.
 *
 * This class specializes `PoolFactory` to handle the creation of `Entry` objects, which 
 * represent chords and notes.
 */
class EntryFactory : public PoolFactory<EntryFactory, dom::Entry, dom::EntryPool>
{
public:
    using PoolFactory::create;

    /**
     * @brief Extracts an `Entry` object from an XML element.
     *
     * Extracts an `EntryFactory` derived object from the given XML element by delegating 
     * to the `RegisteredEntries` class. This allows the creation of `Options` objects from XML.
     *
     * @param element The XML element from which to extract the object.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param pool The pool we are constructiong. (It hasn't been assigned to @p document yet.)
     * @return A shared pointer to the created object.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker, const EntryPoolPtr& pool)
    {
        auto entnumAttr = element->findAttribute("entnum");
        if (!entnumAttr) {
            throw std::invalid_argument("missing entum attribute for entry");
        }
        auto prevAttr = element->findAttribute("prev");
        if (!entnumAttr) {
            throw std::invalid_argument("missing prev attribute for entry");
        }
        auto nextAttr = element->findAttribute("next");
        if (!entnumAttr) {
            throw std::invalid_argument("missing next attribute for entry");
        }
        return RegisteredEntries::createInstance(pool, element, elementLinker, document,
            entnumAttr->getValueAs<EntryNumber>(),
            prevAttr->getValueAs<EntryNumber>(),
            nextAttr->getValueAs<EntryNumber>());
    }
};

/**
 * @brief Factory class for creating `Texts` objects from XML.
 *
 * This class specializes `PoolFactory` to handle the creation of `Texts` objects, representing 
 * various attributes stored in an `TextsPool`. It includes an XML parsing mechanism to 
 * extract and create these objects, which are used in the document model.
 */
class TextsFactory : public PoolFactory<TextsFactory, dom::TextsBase, dom::TextsPool>
{
public:
    using PoolFactory::create;

    /**
     * @brief Extracts a `TextsBase` object from an XML element.
     *
     * Extracts a `TextsBase` derived object from the given XML element using the specified 
     * attributes such as `number` or `type`. Throws an exception if a required attribute is missing.
     *
     * @param element The XML element from which to extract the object.
     * @param document The document object providing context for the XML parsing.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param pool The pool we are constructiong. (It hasn't been assigned to @p document yet.)
     * @return A shared pointer to the created object.
     * @throws std::invalid_argument if required attributes are missing.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker, const TextsPoolPtr& pool)
    {
        auto textAttributeName = [element]() -> std::string {
            if (element->getTagName() == texts::FileInfoText::XmlNodeName) {
                return "type";
            }
            return "number";
        }();
        auto textAttribute = element->findAttribute(textAttributeName);
        if (!textAttribute) {
            throw std::invalid_argument("Element <" + element->getTagName() + "> does not have attribute " + textAttributeName);
        }
        auto textNumber = [textAttribute]() -> Cmper {
            if (textAttribute->getName() == "type") {
                return toCmper(textAttribute->getValue());
            }
            return textAttribute->getValueAs<Cmper>();
        }();
        return RegisteredTexts::createInstance(pool, element, elementLinker, document, textNumber);
    }

private:
    static Cmper toCmper(const std::string& type)
    {
        using TextType = texts::FileInfoText::TextType;

        static const std::unordered_map<std::string_view, TextType> typeMap = {
            {"title", TextType::Title},
            {"composer", TextType::Composer},
            {"copyright", TextType::Copyright},
            {"description", TextType::Description},
            {"lyricist", TextType::Lyricist},
            {"arranger", TextType::Arranger},
            {"subtitle", TextType::Subtitle}
        };

        auto it = typeMap.find(type);
        if (it != typeMap.end()) {
            return Cmper(it->second);
        }

        throw std::invalid_argument("Unknown type attribute value for <fileInfo> node: " + type);
    }
};

} // namespace factory
} // namespace musx
