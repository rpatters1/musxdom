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
#include "musx/dom/ObjectPool.h"

namespace musx {
namespace factory {

/**
 * @brief Factory class for creating `Header` objects from XML.
 */
class OthersFactory : public FactoryBase
{
public:
    /**
     * @brief Creates a `OthersPool` object from an XML element.
     *
     * @param element The XML element representing the `<others>` node.
     * @return A fully populated `OthersPool` object.
     * @throws std::invalid_argument If required nodes or attributes are missing or invalid.
     */
    static dom::OthersPoolPtr create(const std::shared_ptr<xml::IXmlElement>& element)
    {
        auto othersPool = std::make_shared<dom::OthersPool>();

        for (auto otherElement = element->getFirstChildElement(); otherElement; otherElement = otherElement->getNextSibling()) {
            auto cmperAttribute = otherElement->findAttribute("cmper");
            if (!cmperAttribute) {
                throw std::invalid_argument("missing cmper for others element " + otherElement->getTagName());
            }
            dom::Cmper cmper = cmperAttribute->getValueAs<dom::Cmper>();
            auto inciAttribute = otherElement->findAttribute("inci");
            auto basePtr = RegisteredTypes::createInstance(otherElement,
                                                           cmperAttribute->getValueAs<dom::Cmper>(),
                                                           inciAttribute ? inciAttribute->getValueAs<dom::Inci>() : 0);
        // ToDo: extract properties from xml
            if (basePtr) {
                auto otherPtr = std::static_pointer_cast<dom::OthersBase>(basePtr);
                assert(otherPtr); // program bug if null
                othersPool->add(otherElement->getTagName(), otherPtr);
            }
        }

        return othersPool;
    }
};

} // namespace factory
} // namespace musx
