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
#include "musx/dom/Others.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

namespace musx {

namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::others;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

template <>
inline Enclosure::Shape toEnum<Enclosure::Shape>(const uint8_t& value)
{
    if (value >= static_cast<uint8_t>(Enclosure::Shape::NoEnclosure) &&
        value <= static_cast<uint8_t>(Enclosure::Shape::Octogon)) {
        return static_cast<Enclosure::Shape>(value);
    }
    throw std::invalid_argument("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
}

MUSX_XML_ENUM_MAPPING(NamePositioning::AlignJustify, {
    //{"left", NamePositioning::AlignJustify::Left}, this is the default and is not known to occur in the xml
    {"center", NamePositioning::AlignJustify::Center},
    {"right", NamePositioning::AlignJustify::Right},
    });

MUSX_XML_ENUM_MAPPING(MeasureNumberRegion::AlignJustify, {
    //{"left", MeasureNumberRegion::AlignJustify::Left}, this is the default and is not known to occur in the xml
    {"center", MeasureNumberRegion::AlignJustify::Center},
    {"right", MeasureNumberRegion::AlignJustify::Right},
});

MUSX_XML_ENUM_MAPPING(MeasureNumberRegion::TimePrecision, {
    //{"wholeSeconds", MeasureNumberRegion::TimePrecision::WholeSeconds}, this is the default and is not known to occur in the xml
    {"tenths", MeasureNumberRegion::TimePrecision::Tenths},
    {"hundredths", MeasureNumberRegion::TimePrecision::Hundredths},
    {"thousandths", MeasureNumberRegion::TimePrecision::Thousandths},
});

MUSX_XML_ENUM_MAPPING(RehearsalMarkStyle, {
    {"letters", RehearsalMarkStyle::Letters},
    {"letNum", RehearsalMarkStyle::LetterNumbers},
    {"lettersLc", RehearsalMarkStyle::LettersLowerCase},
    {"letNumLc", RehearsalMarkStyle::LettersNumbersLowerCase},
    {"numbers", RehearsalMarkStyle::Numbers},
    {"measNum", RehearsalMarkStyle::MeasureNumber}
});

MUSX_XML_ENUM_MAPPING(PlaybackType, {
    {"none", PlaybackType::None},
    {"time", PlaybackType::Tempo},
    {"midiController", PlaybackType::MidiController},
    {"amplitude", PlaybackType::KeyVelocity},
    {"transpose", PlaybackType::Transpose},
    {"channel", PlaybackType::Channel},
    {"midiPatchChange", PlaybackType::MidiPatchChange},
    {"percMidiMap", PlaybackType::PercussionMidiMap},
    {"midiPitchwheel", PlaybackType::MidiPitchWheel},
    {"midiPressure", PlaybackType::ChannelPressure},
    {"rekey", PlaybackType::RestrikeKeys},
    {"dump", PlaybackType::Dump},
    {"startTempo", PlaybackType::PlayTempoToolChanges},
    {"stopTempo", PlaybackType::IgnoreTempoToolChanges},
    {"swing", PlaybackType::Swing},
    {"hpOn", PlaybackType::SmartPlaybackOn},
    {"hpOff", PlaybackType::SmartPlaybackOff}
});

MUSX_XML_ENUM_MAPPING(HorizontalMeasExprAlign, {
    {"manual", HorizontalMeasExprAlign::Manual},
    {"leftOfAllNoteheads", HorizontalMeasExprAlign::LeftOfAllNoteheads},
    {"leftOfPrimaryNotehead", HorizontalMeasExprAlign::LeftOfPrimaryNotehead},
    {"stem", HorizontalMeasExprAlign::Stem},
    {"centerPrimaryNotehead", HorizontalMeasExprAlign::CenterPrimaryNotehead},
    {"centerAllNoteheads", HorizontalMeasExprAlign::CenterAllNoteheads},
    {"rightOfAllNoteheads", HorizontalMeasExprAlign::RightOfAllNoteheads},
    {"leftEdge", HorizontalMeasExprAlign::LeftBarline},
    {"startTimeSig", HorizontalMeasExprAlign::StartTimeSig},
    {"afterClefKeyTime", HorizontalMeasExprAlign::AfterClefKeyTime},
    {"startOfMusic", HorizontalMeasExprAlign::StartOfMusic},
    {"centerOverBarlines", HorizontalMeasExprAlign::CenterOverBarlines},
    {"centerOverMusic", HorizontalMeasExprAlign::CenterOverMusic},
    {"rightEdge", HorizontalMeasExprAlign::RightBarline}
});

MUSX_XML_ENUM_MAPPING(VerticalMeasExprAlign, {
    {"manual", VerticalMeasExprAlign::Manual},
    {"refLine", VerticalMeasExprAlign::RefLine},
    {"aboveStaff", VerticalMeasExprAlign::AboveStaff},
    {"belowStaff", VerticalMeasExprAlign::BelowStaff},
    {"topNote", VerticalMeasExprAlign::TopNote},
    {"bottomNote", VerticalMeasExprAlign::BottomNote},
    {"aboveEntry", VerticalMeasExprAlign::AboveEntry},
    {"belowEntry", VerticalMeasExprAlign::BelowEntry},
    {"aboveStaffOrEntry", VerticalMeasExprAlign::AboveStaffOrEntry},
    {"belowStaffOrEntry", VerticalMeasExprAlign::BelowStaffOrEntry}
});

MUSX_XML_ENUM_MAPPING(HorizontalExprJustification, {
    {"left", HorizontalExprJustification::Left},
    {"center", HorizontalExprJustification::Center},
    {"right", HorizontalExprJustification::Right}
});

MUSX_XML_ENUM_MAPPING(MarkingCategory::CategoryType, {
    {"dynamics", MarkingCategory::CategoryType::Dynamics},
    {"tempoMarks", MarkingCategory::CategoryType::TempoMarks},
    {"tempoAlts", MarkingCategory::CategoryType::TempoAlterations},
    {"expressiveText", MarkingCategory::CategoryType::ExpressiveText},
    {"techniqueText", MarkingCategory::CategoryType::TechniqueText},
    {"rehearsalMarks", MarkingCategory::CategoryType::RehearsalMarks},
    {"misc", MarkingCategory::CategoryType::Misc}
});

MUSX_RESOLVER_ARRAY(LayerAttributes, {
    [](const dom::DocumentPtr& document) {
        auto layers = document->getOthers()->getArray<LayerAttributes>();
        if (layers.size() != 4) {
            throw std::invalid_argument("Expected exactly 4 <layerAtts> elements.");
        }
        for (size_t i = 0; i < layers.size(); i++) {
            if (layers[i]->getCmper() != i) {
                throw std::invalid_argument("Expected <layerAtts> elements to have cmper values 0, 1, 2, 3 in order.");
            }
        }
    }
});

MUSX_RESOLVER_ARRAY(MarkingCategory, {
    [](const dom::DocumentPtr& document) {
        auto cats = document->getOthers()->getArray<MarkingCategory>();
        for (const auto& cat : cats) {
            if (cat->categoryType == MarkingCategory::CategoryType::Invalid) {
                throw std::invalid_argument("Encountered <markingsCategory> node (cmper " + std::to_string(cat->getCmper()) + ") with no categoryType");
            }
        }
    }
});

MUSX_RESOLVER_ARRAY(TextExpressionDef, {
    [](const dom::DocumentPtr& document) {
        auto exps = document->getOthers()->getArray<TextExpressionDef>();
        for (const auto& instance : exps) {
            if (instance->categoryId) {
                auto markingCat = document->getOthers()->get<MarkingCategory>(instance->categoryId);
                if (!markingCat) {
                    throw std::invalid_argument("Marking category for text expression " + std::to_string(instance->getCmper()) + " does not exist.");
                }
                markingCat->textExpressions.emplace(instance->getCmper(), instance);
            }
        }
    }
});

template <>
struct FieldPopulator<TextExpressionEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

template <>
struct FieldPopulator<TextRepeatEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

#endif // DOXYGEN_SHOULD_IGNORE THIS

} // namespace factory
} // namespace musx
