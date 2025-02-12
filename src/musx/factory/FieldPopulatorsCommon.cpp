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
#include "musx/musx.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::others;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

MUSX_XML_ENUM_MAPPING(NamePositioning::AlignJustify, {
    //{"left", NamePositioning::AlignJustify::Left}, this is the default and is not known to occur in the xml
    {"center", NamePositioning::AlignJustify::Center},
    {"right", NamePositioning::AlignJustify::Right},
});

} // namespace factory

namespace dom {

using namespace musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(FontInfo, {
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i) { i->fontId = e->getTextAs<Cmper>(); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i) { i->fontSize = e->getTextAs<int>(); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i) {
            for (auto efxChild = e->getFirstChildElement(); efxChild; efxChild = efxChild->getNextSibling()) {
                auto efxName = efxChild->getTagName();
                if (efxName == "bold") {
                    i->bold = true;
                } else if (efxName == "italic") {
                    i->italic = true;
                } else if (efxName == "underline") {
                    i->underline = true;
                } else if (efxName == "strikeout") {
                    i->strikeout = true;
                } else if (efxName == "absolute") {
                    i->absolute = true;
                } else if (efxName == "hidden") {
                    i->hidden = true;
                }
            }        
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(KeySignature, {
    {"key", [](const XmlElementPtr& e, const std::shared_ptr<KeySignature>& i) { i->key = e->getTextAs<Cmper>(); }},
    {"keyless", [](const XmlElementPtr&, const std::shared_ptr<KeySignature>& i) { i->keyless = true; }},
    {"hideKeySigShowAccis", [](const XmlElementPtr&, const std::shared_ptr<KeySignature>& i) { i->hideKeySigShowAccis = true; }},
});

namespace others {

MUSX_XML_ELEMENT_ARRAY(Enclosure, {
    {"xAdd", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->xAdd = e->getTextAs<Evpu>(); }},
    {"yAdd", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->yAdd = e->getTextAs<Evpu>(); }},
    {"xMargin", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->xMargin = e->getTextAs<Evpu>(); }},
    {"yMargin", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->yMargin = e->getTextAs<Evpu>(); }},
    {"lineWidth", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
    {"sides", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->shape = toEnum<Enclosure::Shape>(e->getTextAs<uint8_t>()); }},
    {"cornerRadius", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->cornerRadius = e->getTextAs<Efix>(); }},
    {"fixedSize", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->fixedSize = true; }},
    {"equalAspect", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->equalAspect = true; }},
    {"notTall", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->notTall = true; }},
    {"opaque", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->opaque = true; }},
    {"roundCorners", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->roundCorners = true; }},
});

MUSX_XML_ELEMENT_ARRAY(MusicRange, {
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<MusicRange>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"startEdu", [](const XmlElementPtr& e, const std::shared_ptr<MusicRange>& i) { i->startEdu = e->getTextAs<Edu>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<MusicRange>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"endEdu", [](const XmlElementPtr& e, const std::shared_ptr<MusicRange>& i) { i->endEdu = e->getTextAs<Edu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NamePositioning, {
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->horzOff = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->vertOff = e->getTextAs<Evpu>(); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->justify = toEnum<NamePositioning::AlignJustify>(e->getTextTrimmed()); }},
    {"indivPos", [](const XmlElementPtr&, const std::shared_ptr<NamePositioning>& i) { i->indivPos = true; }},
    {"halign", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->hAlign = toEnum<NamePositioning::AlignJustify>(e->getTextTrimmed()); }},
    {"expand", [](const XmlElementPtr&, const std::shared_ptr<NamePositioning>& i) { i->expand = true; }},
});
    
} // namespace others
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS