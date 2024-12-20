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
namespace dom {
namespace others {

using namespace ::musx::xml;
using namespace ::musx::factory;

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

MUSX_XML_ENUM_MAPPING(NamePositioning::AlignJustify, {
    //{"left", NamePositioning::AlignJustify::Left}, this is the default and is not known to occur in the xml
    {"center", NamePositioning::AlignJustify::Center},
    {"right", NamePositioning::AlignJustify::Right},
});

MUSX_XML_ELEMENT_ARRAY(NamePositioning, {
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->horzOff = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->vertOff = e->getTextAs<Evpu>(); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->justify = toEnum<NamePositioning::AlignJustify>(e->getTextTrimmed()); }},
    {"indivPos", [](const XmlElementPtr&, const std::shared_ptr<NamePositioning>& i) { i->indivPos = true; }},
    {"halign", [](const XmlElementPtr& e, const std::shared_ptr<NamePositioning>& i) { i->hAlign = toEnum<NamePositioning::AlignJustify>(e->getTextTrimmed()); }},
    {"expand", [](const XmlElementPtr&, const std::shared_ptr<NamePositioning>& i) { i->expand = true; }},
});

MUSX_XML_ELEMENT_ARRAY(FontDefinition, {
    {"charsetBank", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetBank = e->getText(); }},
    {"charsetVal", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetVal = e->getTextAs<int>(); }},
    {"pitch", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->pitch = e->getTextAs<int>(); }},
    {"family", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->family = e->getTextAs<int>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(LayerAttributes, {
    {"restOffset", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->restOffset = e->getTextAs<int>(); }},
    {"flipTies", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezTiesToStems = true; }},
    {"floatLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->onlyIfOtherLayersHaveNotes = true; }},
    {"useRestOff", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->useRestOffset = true; }},
    {"freezeLayUp", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezeStemsUp = true; }},
    {"freezeLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezeLayer = true; }},
    {"playback", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->playback = true; }},
    {"spacing", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->affectSpacing = true; }},
    {"ignoreHidden", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenNotesOnly = true; }},
    {"ignoreHiddenLayers", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenLayers = true; }},
    {"hideLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->hideLayer = true; }},
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

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion::ScorePartData, {
    {"startFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"multipleFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"mmRestFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"startEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startEnclosure = FieldPopulator<Enclosure>::createAndPopulate(e, i->getDocument()); }},
    {"multipleEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleEnclosure = FieldPopulator<Enclosure>::createAndPopulate(e, i->getDocument()); }},
    {"startYdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startYdisp = e->getTextAs<Evpu>(); }},
    {"multipleYdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleYdisp = e->getTextAs<Evpu>(); }},
    {"mmRestYdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestYdisp = e->getTextAs<Evpu>(); }},
    {"leftMmBracketChar", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->leftMmBracketChar = e->getTextAs<char32_t>(); }},
    {"rightMmBracketChar", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->rightMmBracketChar = e->getTextAs<char32_t>(); }},
    {"startWith", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startWith = e->getTextAs<int>(); }},
    {"incidence", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->incidence = e->getTextAs<int>(); }},
    {"startAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startAlign = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }},
    {"multipleAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleAlign = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }},
    {"mmRestAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestAlign = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }},
    {"startOfLine", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnStart = true; }},
    {"multipleOf", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnEvery = true; }},
    {"exceptFirstMeas", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->hideFirstMeasure = true; }},
    {"mmRestRange", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showMmRange = true; }},
    {"mmRestRangeForce", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnMmRest = true; }},
    {"useStartEncl", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useStartEncl = true; }},
    {"useMultipleEncl", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useMultipleEncl = true; }},
    {"showOnTop", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnTop = true; }},
    {"showOnBottom", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnBottom = true; }},
    {"excludeOthers", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->excludeOthers = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->breakMmRest = true; }},
    {"startJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startJustify = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }},
    {"multipleJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleJustify = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }},
    {"mmRestJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestJustify = toEnum<MeasureNumberRegion::AlignJustify>(e->getTextTrimmed()); }}
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion, {
    {"scoreData", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->scoreData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(e, i->getDocument()); }},
    {"partData", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->partData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(e, i->getDocument()); }},
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startMeas = e->getTextAs<Cmper>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->endMeas = e->getTextAs<Cmper>(); }},
    {"startChar", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startChar = e->getTextAs<char32_t>(); }},
    {"base", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->base = e->getTextAs<int>(); }},
    {"prefix", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->prefix = e->getTextTrimmed(); }},
    {"suffix", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->suffix = e->getTextTrimmed(); }},
    {"countFromOne", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->countFromOne = true; }},
    {"noZero", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->noZero = true; }},
    {"doubleUp", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->doubleUp = true; }},
    {"time", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->time = true; }},
    {"includeHours", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->includeHours = true; }},
    {"smpteFrames", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->smpteFrames = true; }},
    {"useScoreInfoForPart", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->useScoreInfoForPart = true; }},
    {"region", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->region = e->getTextAs<int>(); }},
    {"timePrecision", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->timePrecision = toEnum<MeasureNumberRegion::TimePrecision>(e->getTextTrimmed()); }},
    {"hideScroll", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->hideScroll = true; }},
    {"hidePage", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->hidePage = true; }}
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

MUSX_XML_ELEMENT_ARRAY(MarkingCategory, {
    {"categoryType", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->categoryType = toEnum<MarkingCategory::CategoryType>(e->getTextTrimmed()); }},
    {"textFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->textFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"musicFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->musicFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"numberFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->numberFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"horzAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzAlign = toEnum<HorizontalMeasExprAlign>(e->getTextTrimmed()); }},
    {"vertAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertAlign = toEnum<VerticalMeasExprAlign>(e->getTextTrimmed()); }},
    {"justification", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->justification = toEnum<HorizontalExprJustification>(e->getTextTrimmed()); }},
    {"horzOffset", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"vertOffsetBaseline", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetBaseline = e->getTextAs<Evpu>(); }},
    {"vertOffsetEntry", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetEntry = e->getTextAs<Evpu>(); }},
    {"usesTextFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesTextFont = true; }},
    {"usesMusicFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesMusicFont = true; }},
    {"usesNumberFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesNumberFont = true; }},
    {"usesPositioning", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesPositioning = true; }},
    {"usesStaffList", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesStaffList = true; }},
    {"usesBreakMmRests", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesBreakMmRests = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->breakMmRest = true; }},
    {"userCreated", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->userCreated = true; }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->staffList = e->getTextAs<Cmper>(); }},
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

MUSX_XML_ELEMENT_ARRAY(PartGlobals, {
    {"showTransposed", [](const XmlElementPtr&, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = true; }},
    {"scrollViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(MarkingCategoryName, {
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategoryName>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextExpressionDef, {
    {"textIDKey", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e->getTextTrimmed()); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"hideMeasureNum", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hideMeasureNum = true; }},
    {"useAuxData", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = true; }},
    {"newEnclosure", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = true; }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e->getTextTrimmed()); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e->getTextTrimmed()); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalExprJustification>(e->getTextTrimmed()); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e->getTextTrimmed()); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryFonts = true; }},
    {"useCategoryPos", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryPos = true; }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->description = e->getText(); }},
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

// Repeat for other types...

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace others
} // namespace dom
} // namespace musx
