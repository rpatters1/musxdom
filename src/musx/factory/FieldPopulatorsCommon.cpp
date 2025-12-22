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

MUSX_XML_ENUM_MAPPING(AlignJustify, {
    {"left", AlignJustify::Left},
    {"center", AlignJustify::Center},
    {"right", AlignJustify::Right},
});

MUSX_XML_ENUM_MAPPING(LyricTextType, {
    {"verse", LyricTextType::Verse},
    {"chorus", LyricTextType::Chorus},
    {"section", LyricTextType::Section},
});

MUSX_XML_ENUM_MAPPING(ShowClefMode, {
    // {"whenNeeded", ShowClefMode::WhenNeeded}, this is the default and may never appear in the xml
    {"hidden", ShowClefMode::Never},
    {"forced", ShowClefMode::Always}
});

#ifndef DOXYGEN_SHOULD_IGNORE_THIS
void populateFontEfx(const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i)
{
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
#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory

namespace dom {

using namespace musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(FontInfo, {
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i) { i->fontId = e->getTextAs<Cmper>(); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<dom::FontInfo>& i) { i->fontSize = e->getTextAs<int>(); }},
    {"efx", factory::populateFontEfx },
});

MUSX_XML_ELEMENT_ARRAY(KeySignature, {
    {"key", [](const XmlElementPtr& e, const std::shared_ptr<KeySignature>& i) { i->key = e->getTextAs<Cmper>(); }},
    {"keyless", [](const XmlElementPtr& e, const std::shared_ptr<KeySignature>& i) { i->keyless = populateBoolean(e, i); }},
    {"hideKeySigShowAccis", [](const XmlElementPtr& e, const std::shared_ptr<KeySignature>& i) { i->hideKeySigShowAccis = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(OthersName, {
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<OthersName>& i) { i->name = e->getText(); }},
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
    {"fixedSize", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->fixedSize = populateBoolean(e, i); }},
    {"equalAspect", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->equalAspect = populateBoolean(e, i); }},
    {"notTall", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->notTall = populateBoolean(e, i); }},
    {"opaque", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->opaque = populateBoolean(e, i); }},
    {"roundCorners", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->roundCorners = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(EnigmaMusicRange, {
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<EnigmaMusicRange>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"startEdu", [](const XmlElementPtr& e, const std::shared_ptr<EnigmaMusicRange>& i) { i->startEdu = e->getTextAs<Edu>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<EnigmaMusicRange>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"endEdu", [](const XmlElementPtr& e, const std::shared_ptr<EnigmaMusicRange>& i) { i->endEdu = e->getTextAs<Edu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NamePositioning, {
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->horzOff = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->vertOff = e->getTextAs<Evpu>(); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->justify = toEnum<AlignJustify>(e); }},
    {"indivPos", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->indivPos = populateBoolean(e, i); }},
    {"halign", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->hAlign = toEnum<AlignJustify>(e); }},
    {"expand", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->expand = populateBoolean(e, i); }},
});

} // namespace others
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS