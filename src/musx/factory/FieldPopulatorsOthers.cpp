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

MUSX_XML_ENUM_MAPPING(Measure::PositioningType, {
    // {"manual", Measure::PositioningType::Manual}, // This is the default and is not known to occur in the XML.
    {"timesig", Measure::PositioningType::TimeSignature},
    {"beatchart", Measure::PositioningType::BeatChart},
    {"timesigPlusPos", Measure::PositioningType::TimeSigPlusPositioning},
    {"beatchartPlusPos", Measure::PositioningType::BeatChartPlusPositioning},
});

MUSX_XML_ENUM_MAPPING(Measure::BarlineType, {
    // {"none", Measure::BarlineType::None}, // This is the default and is not known to occur in the XML.
    {"default", Measure::BarlineType::OptionsDefault},
    {"normal", Measure::BarlineType::Normal},
    {"double", Measure::BarlineType::Double},
    {"final", Measure::BarlineType::Final},
    {"solid", Measure::BarlineType::Solid},
    {"dash", Measure::BarlineType::Dashed},
    {"partial", Measure::BarlineType::Tick},
    {"custom", Measure::BarlineType::Custom},
});

MUSX_XML_ENUM_MAPPING(Measure::ShowKeySigMode, {
    // {"ifNeeded", Measure::ShowKeySigMode::IfNeeded}, // This is the default and is not known to occur in the XML.
    {"deltaKey", Measure::ShowKeySigMode::Always},
    {"ignoreKey", Measure::ShowKeySigMode::Never},
});

MUSX_XML_ENUM_MAPPING(Measure::ShowTimeSigMode, {
    // {"ifNeeded", Measure::ShowTimeSigMode::IfNeeded}, // This is the default and is not known to occur in the XML.
    {"deltaTime", Measure::ShowTimeSigMode::Always},
    {"ignoreTime", Measure::ShowTimeSigMode::Never},
});

MUSX_XML_ENUM_MAPPING(TextBlock::TextType, {
    {"block", TextBlock::TextType::Block},
    {"expression", TextBlock::TextType::Expression}
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

} // namespace factory
namespace dom {
namespace others {

using namespace ::musx::xml;
using namespace ::musx::factory;

// Classes from BaseClasses.h

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

MUSX_XML_ELEMENT_ARRAY(FontDefinition, {
    {"charsetBank", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetBank = e->getText(); }},
    {"charsetVal", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetVal = e->getTextAs<int>(); }},
    {"pitch", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->pitch = e->getTextAs<int>(); }},
    {"family", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->family = e->getTextAs<int>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(Frame, {
    {"startEntry", [](const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->startEntry = e->getTextAs<EntryNumber>(); }},
    {"endEntry", [](const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->endEntry = e->getTextAs<EntryNumber>(); }},
    {"startTime", [](const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->startTime = e->getTextAs<Edu>(); }},
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

// Classes from Others.h

MUSX_XML_ELEMENT_ARRAY(InstrumentUsed, {
    {"inst", [](const XmlElementPtr& e, const std::shared_ptr<InstrumentUsed>& i) { i->staffId = e->getTextAs<Cmper>(); }},
    {"trackType", [](const XmlElementPtr&, const std::shared_ptr<InstrumentUsed>&) { /* this field seems like it was for future enhancement */ }},
    {"distFromTop", [](const XmlElementPtr& e, const std::shared_ptr<InstrumentUsed>& i) { i->distFromTop = e->getTextAs<Evpu>(); }},
    {"range", [](const XmlElementPtr& e, const std::shared_ptr<InstrumentUsed>& i)
        { i->range = FieldPopulator<MusicRange>::createAndPopulate(e, i->getDocument()); }},
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

MUSX_XML_ELEMENT_ARRAY(MarkingCategoryName, {
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategoryName>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(Measure, {
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"beats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispDivbeat = e->getTextAs<Cmper>(); }},
    {"frontSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->frontSpaceExtra = e->getTextAs<Evpu>(); }},
    {"backSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backSpaceExtra = e->getTextAs<Evpu>(); }},
    {"breakWordExt", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->breakWordExt = true; }},
    {"hideCaution", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hideCaution = true; }},
    {"showFullNames", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->showFullNames = true; }},
    {"allowSplitPoints", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->allowSplitPoints = true; }},
    {"groupBarlineOverride", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->groupBarlineOverride = true; }},
    {"customBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customBarShape = e->getTextAs<Cmper>(); }},
    {"customLeftBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customLeftBarShape = e->getTextAs<Cmper>(); }},
    {"showKey", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showKey = toEnum<Measure::ShowKeySigMode>(e->getTextTrimmed()); }},
    {"showTime", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showTime = toEnum<Measure::ShowTimeSigMode>(e->getTextTrimmed()); }},
    {"posMode", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->positioningMode = toEnum<Measure::PositioningType>(e->getTextTrimmed()); }},
    {"lineBreak", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->beginNewSystem = true; }},
    {"breakRest", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->breakMmRest = true; }},
    {"noMeasNum", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->noMeasNum = true; }},
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->barlineType = toEnum<Measure::BarlineType>(e->getTextTrimmed()); }},
    {"hasSmartShape", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasSmartShape = true; }},
    {"indivPosDef", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->evenlyAcrossMeasure = true; }},
    {"hasExpr", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasExpression = true; }},
    {"altNumTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeNumerator = true; }},
    {"altDenTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDenominator = true; }},
    {"abbrvTime", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->abbrvTime = true; }},
    {"useDisplayTimesig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->useDisplayTimesig = true; }},
    {"leftBarline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->leftBarlineType = toEnum<Measure::BarlineType>(e->getTextTrimmed()); }},
    {"displayAltNumTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDispNumerator = true; }},
    {"displayAltDenTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDispDenominator = true; }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion::ScorePartData, {
    {"startFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"multipleFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"mmRestFont", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"startEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startEnclosure = FieldPopulator<Enclosure>::createAndPopulate(e, i->getDocument()); }},
    {"multipleEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleEnclosure = FieldPopulator<Enclosure>::createAndPopulate(e, i->getDocument()); }},
    {"startXdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startXdisp = e->getTextAs<Evpu>(); }},
    {"startYdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startYdisp = e->getTextAs<Evpu>(); }},
    {"multipleXdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleXdisp = e->getTextAs<Evpu>(); }},
    {"multipleYdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleYdisp = e->getTextAs<Evpu>(); }},
    {"mmRestXdisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestXdisp = e->getTextAs<Evpu>(); }},
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
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"startChar", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startChar = e->getTextAs<char32_t>(); }},
    {"base", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->base = e->getTextAs<int>(); }},
    {"offset", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->numberOffset = e->getTextAs<int>(); }},
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

MUSX_XML_ELEMENT_ARRAY(Page, {
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->percent = e->getTextAs<int>(); }},
    {"firstSystem", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->firstSystem = e->getTextAs<SystemCmper>(); }},
    {"scaleContentOnly", [](const XmlElementPtr&, const std::shared_ptr<Page>& i) { i->holdMargins = true; }},
    {"margTop", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margTop = e->getTextAs<Evpu>(); }},
    {"margLeft", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margLeft = e->getTextAs<Evpu>(); }},
    {"margBottom", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margBottom = e->getTextAs<Evpu>(); }},
    {"margRight", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margRight = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartDefinition, {
    {"nameID", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->nameId = e->getTextAs<Cmper>(); }},
    {"partOrder", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->partOrder = e->getTextAs<int>(); }},
    {"copies", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->copies = e->getTextAs<int>(); }},
    {"extractPart", [](const XmlElementPtr&, const std::shared_ptr<PartDefinition>& i) { i->extractPart = true; }},
    {"needsRecalc", [](const XmlElementPtr&, const std::shared_ptr<PartDefinition>& i) { i->needsRecalc = true; }},
    {"useAsSmpInst", [](const XmlElementPtr&, const std::shared_ptr<PartDefinition>& i) { i->useAsSmpInst = true; }},
    {"smartMusicInst", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->smartMusicInst = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartGlobals, {
    {"showTransposed", [](const XmlElementPtr&, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = true; }},
    {"scrollViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff, {
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"transposedClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->transposedClef = e->getTextAs<ClefIndex>(); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lineSpace = e->getTextAs<Evpu>(); }},
    {"topBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topBarlineOffset = e->getTextAs<Evpu>(); }},
    {"botBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botBarlineOffset = e->getTextAs<Evpu>(); }},
    {"dwRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->dwRestOffset = e->getTextAs<Evpu>(); }},
    {"wRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->wRestOffset = e->getTextAs<Evpu>(); }},
    {"hRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hRestOffset = e->getTextAs<Evpu>(); }},
    {"otherRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->otherRestOffset = e->getTextAs<Evpu>(); }},
    {"topRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"botRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"staffLines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->staffLines = e->getTextAs<int>(); }},
    {"stemReversal", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemReversal = e->getTextAs<int>(); }},
    {"fullName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fullNameTextId = e->getTextAs<Cmper>(); }},
    {"abbrvName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->abbrvNameTextId = e->getTextAs<Cmper>(); }},
    {"vertTabNumOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->vertTabNumOff = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffSystem, {
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"horzPercent", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->horzPercent = e->getTextAs<double>() / 100.0; }},
    {"ssysPercent", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->ssysPercent = e->getTextAs<int>(); }},
    {"staffHeight", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->staffHeight = e->getTextAs<Efix>(); }},
    {"top", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->top = e->getTextAs<Evpu>(); }},
    {"left", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"right", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->right = e->getTextAs<Evpu>(); }},
    {"bottom", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"hasStaffScaling", [](const XmlElementPtr&, const std::shared_ptr<StaffSystem>& i) { i->hasStaffScaling = true; }},
    {"placeEndSpaceBeforeBarline", [](const XmlElementPtr&, const std::shared_ptr<StaffSystem>& i) { i->placeEndSpaceBeforeBarline = true; }},
    {"scaleVert", [](const XmlElementPtr&, const std::shared_ptr<StaffSystem>& i) { i->scaleVert = true; }},
    {"scaleContentOnly", [](const XmlElementPtr&, const std::shared_ptr<StaffSystem>& i) { i->holdMargins = true; }},
    {"distanceToPrev", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->distanceToPrev = e->getTextAs<Evpu>(); }},
    {"extraStartSystemSpace", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraStartSystemSpace = e->getTextAs<Evpu>(); }},
    {"extraEndSystemSpace", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraEndSystemSpace = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextBlock, {
    {"textID", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textId = e->getTextAs<Cmper>(); }},
    {"lineSpacingPercent", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->lineSpacingPercentage = e->getTextAs<int>(); }},
    {"newPos36", [](const XmlElementPtr&, const std::shared_ptr<TextBlock>& i) { i->newPos36 = true; }},
    {"showShape", [](const XmlElementPtr&, const std::shared_ptr<TextBlock>& i) { i->showShape = true; }},
    {"noExpandSingleWord", [](const XmlElementPtr&, const std::shared_ptr<TextBlock>& i) { i->noExpandSingleWord = true; }},
    {"wordWrap", [](const XmlElementPtr&, const std::shared_ptr<TextBlock>& i) { i->wordWrap = true; }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"roundCorners", [](const XmlElementPtr&, const std::shared_ptr<TextBlock>& i) { i->roundCorners = true; }},
    {"cornerRadius", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->cornerRadius = e->getTextAs<Efix>(); }},
    {"textTag", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textType = toEnum<TextBlock::TextType>(e->getTextTrimmed()); }}
});

MUSX_XML_ELEMENT_ARRAY(TextExpressionDef, {
    {"textIDKey", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e->getTextTrimmed()); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"hideMeasureNum", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hideMeasureNum = true; }},
    {"matchPlayback", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->matchPlayback = true; }},
    {"useAuxData", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = true; }},
    {"newEnclosure", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = true; }},
    {"createdByHp", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->createdByHp = true; }},
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

} // namespace others
} // namespace dom
} // namespace musx


#endif // DOXYGEN_SHOULD_IGNORE_THIS
