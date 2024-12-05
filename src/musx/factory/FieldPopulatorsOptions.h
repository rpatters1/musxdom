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

#include "musx/dom/BaseClasses.h"
#include "musx/dom/Options.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

namespace musx {
namespace factory {

using namespace dom::options;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

template <>
struct FieldPopulator<DefaultFonts> : public FactoryBase
{
    static void populate(const std::shared_ptr<DefaultFonts>& fonts, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        auto fontElements = getFirstChildElement(element, "font");
        for (auto fontElement = getFirstChildElement(element, "font"); fontElement; fontElement = fontElement->getNextSibling("font")) {
            auto typeStr = fontElement->findAttribute("type");
            if (!typeStr) {
                throw std::invalid_argument("font option has no type");
            }
            FontType fontType = fromString(typeStr->getValue());

            auto fontInfo = std::make_shared<dom::FontInfo>(fonts->getDocument());
            FieldPopulator<dom::FontInfo>::populate(fontInfo, fontElement);

            // Add the populated font instance to the vector.
            fonts->defaultFonts.emplace(fontType, fontInfo);
        }
    }

private:
    using FontType = DefaultFonts::FontType;

    static FontType fromString(const std::string& typeStr)
    {
        if (typeStr == "music") return FontType::Music;
        else if (typeStr == "key") return FontType::Key;
        else if (typeStr == "clef") return FontType::Clef;
        else if (typeStr == "time") return FontType::Time;
        else if (typeStr == "chord") return FontType::Chord;
        else if (typeStr == "chordAcci") return FontType::ChordAcci;
        else if (typeStr == "ending") return FontType::Ending;
        else if (typeStr == "tuplet") return FontType::Tuplet;
        else if (typeStr == "textBlock") return FontType::TextBlock;
        else if (typeStr == "lyricVerse") return FontType::LyricVerse;
        else if (typeStr == "lyricChorus") return FontType::LyricChorus;
        else if (typeStr == "lyricSection") return FontType::LyricSection;
        else if (typeStr == "multiMeasRest") return FontType::MultiMeasRest;
        else if (typeStr == "tablature") return FontType::Tablature;
        else if (typeStr == "chordSuffix") return FontType::ChordSuffix;
        else if (typeStr == "expression") return FontType::Expression;
        else if (typeStr == "repeat") return FontType::Repeat;
        else if (typeStr == "fretboard") return FontType::Fretboard;
        else if (typeStr == "flags") return FontType::Flags;
        else if (typeStr == "accis") return FontType::Accis;
        else if (typeStr == "altNotSlash") return FontType::AltNotSlash;
        else if (typeStr == "altNotNum") return FontType::AltNotNum;
        else if (typeStr == "rests") return FontType::Rests;
        else if (typeStr == "reptDots") return FontType::ReptDots;
        else if (typeStr == "noteheads") return FontType::Noteheads;
        else if (typeStr == "augDots") return FontType::AugDots;
        else if (typeStr == "timePlus") return FontType::TimePlus;
        else if (typeStr == "articulation") return FontType::Articulation;
        else if (typeStr == "percussion") return FontType::Percussion;
        else if (typeStr == "smartShape8va") return FontType::SmartShape8va;
        else if (typeStr == "measNumb") return FontType::MeasNumb;
        else if (typeStr == "staffNames") return FontType::StaffNames;
        else if (typeStr == "abbrvStaffNames") return FontType::AbbrvStaffNames;
        else if (typeStr == "groupNames") return FontType::GroupNames;
        else if (typeStr == "smartShape8vb") return FontType::SmartShape8vb;
        else if (typeStr == "smartShape15ma") return FontType::SmartShape15ma;
        else if (typeStr == "smartShape15mb") return FontType::SmartShape15mb;
        else if (typeStr == "smartShapeTrill") return FontType::SmartShapeTrill;
        else if (typeStr == "smartShapeWiggle") return FontType::SmartShapeWiggle;
        else if (typeStr == "abbrvGroupNames") return FontType::AbbrvGroupNames;
        else if (typeStr == "bendCurveFull") return FontType::BendCurveFull;
        else if (typeStr == "bendCurveWhole") return FontType::BendCurveWhole;
        else if (typeStr == "bendCurveFrac") return FontType::BendCurveFrac;
        else if (typeStr == "timeParts") return FontType::TimeParts;
        else if (typeStr == "timePlusParts") return FontType::TimePlusParts;
        else throw std::invalid_argument("Unknown FontType string: " + typeStr);
    }
};

// Repeat for other types...

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
