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

namespace musx {
namespace factory {

/**
 * @brief Factory class for creating a `dom::ObjectPool` from XML.
 *
 * @tparam DerivedType the derived type for CRTP
 * @tparam ObjectBase the base type of objects in the pool
 * @tparam PoolType the pool type of the objects in the pool
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
     * @param element The XML element representing the `<others>` node.
     * @return A fully populated `ObjectPoolType` object.
     */
    static std::shared_ptr<PoolType> create(const std::shared_ptr<xml::IXmlElement>& element)
    {
        auto pool = std::make_shared<PoolType>();

        for (auto childElement = element->getFirstChildElement(); childElement; childElement = childElement->getNextSibling()) {
            auto basePtr = DerivedType::extractFromXml(childElement);
            if (basePtr) {
                auto typedPtr = std::static_pointer_cast<ObjectBase>(basePtr);
                assert(typedPtr); // program bug if null
                pool->add(element->getTagName(), typedPtr);
            }
        }

        return pool;
    }
};

/**
 * @brief Factory class for creating `Others` objects from XML.
 */
class OthersFactory : public PoolFactory<OthersFactory, dom::OthersBase, dom::OthersPool>
{
public:
    using PoolFactory::create;

    static auto extractFromXml(const std::shared_ptr<xml::IXmlElement>& element)
    {
        auto cmperAttribute = element->findAttribute("cmper");
        if (!cmperAttribute) {
            throw std::invalid_argument("missing cmper for others element " + element->getTagName());
        }
        dom::Cmper cmper = cmperAttribute->getValueAs<dom::Cmper>();
        auto inciAttribute = element->findAttribute("inci");
        return RegisteredTypes::createInstance(element, cmperAttribute->getValueAs<dom::Cmper>(),
                                               inciAttribute ? inciAttribute->getValueAs<dom::Inci>() : 0);
    }
};

} // namespace factory
} // namespace musx
