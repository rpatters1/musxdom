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

#include "FactoryBase.h"
#include "TypeRegistry.h"
#include "musx/xml/XmlInterface.h"
#include "musx/dom/BaseClasses.h"
#include "musx/dom/ObjectPool.h"
#include "musx/dom/Document.h"

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
        auto pool = std::make_shared<PoolType>();

#ifdef MUSX_DISPLAY_NODE_NAMES
        std::set<std::string> alreadyDisplayed;
        std::cout << "============" << std::endl
                  << element->getTagName() << std::endl
                  << "============" << std::endl;
#endif

        for (auto childElement = element->getFirstChildElement(); childElement; childElement = childElement->getNextSibling()) {            
            auto basePtr = DerivedType::extractFromXml(childElement, document, elementLinker);
            if (basePtr) {
#ifdef MUSX_DISPLAY_NODE_NAMES
                auto it = alreadyDisplayed.find(childElement->getTagName());
                if (it == alreadyDisplayed.end()) {
                    std::cout << "  " << childElement->getTagName() << std::endl;
                    alreadyDisplayed.emplace(childElement->getTagName());
                }
#endif
                auto typedPtr = std::dynamic_pointer_cast<ObjectBase>(basePtr);
                assert(typedPtr); // program bug if null
                pool->add(childElement->getTagName(), typedPtr);
            }
        }

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
class OptionsFactory : public PoolFactory<OptionsFactory, dom::OptionsBase, dom::ScalarPool<dom::OptionsBase>>
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
     * @return A shared pointer to the created object.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker)
    {
        return RegisteredOptions::createInstance(document->getOptions(), element, elementLinker, document);
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
     * @return A shared pointer to the created object.
     * @throws std::invalid_argument if required attributes are missing.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker)
    {
        auto cmperAttribute = element->findAttribute("cmper");
        if (!cmperAttribute) {
            throw std::invalid_argument("missing cmper for others element " + element->getTagName());
        }
        auto inciAttribute = element->findAttribute("inci");
        if (inciAttribute) {
            return RegisteredOthers::createInstance(document->getOthers(), element, elementLinker,
                document, cmperAttribute->getValueAs<dom::Cmper>(), inciAttribute->getValueAs<dom::Inci>());
        }
        else {
            return RegisteredOthers::createInstance(document->getOthers(), element, elementLinker,
                document, cmperAttribute->getValueAs<dom::Cmper>());
        }
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
     * @return A shared pointer to the created object.
     * @throws std::invalid_argument if required attributes are missing.
     */
    static auto extractFromXml(const XmlElementPtr& element, const dom::DocumentPtr& document, ElementLinker& elementLinker)
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
        return RegisteredTexts::createInstance(document->getTexts(), element, elementLinker, document, textNumber);
    }

private:
    static Cmper toCmper(const std::string& type) {
        using TextType = texts::FileInfoText::TextType;
        
        if (type == "title") {
            return Cmper(TextType::Title);
        } else if (type == "composer") {
            return Cmper(TextType::Composer);
        } else if (type == "copyright") {
            return Cmper(TextType::Copyright);
        } else if (type == "description") {
            return Cmper(TextType::Description);
        } else if (type == "lyricist") {
            return Cmper(TextType::Lyricist);
        } else if (type == "arranger") {
            return Cmper(TextType::Arranger);
        } else if (type == "subtitle") {
            return Cmper(TextType::Subtitle);
        } else {
            throw std::invalid_argument("Unknown type attribute value for <fileInfo> node: " + type);
        }
    }
};

} // namespace factory
} // namespace musx
