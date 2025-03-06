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

extern template const XmlEnumMappingElement<ShowClefMode> XmlEnumMapping<ShowClefMode>::mapping;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

MUSX_XML_ENUM_MAPPING(FontDefinition::CharacterSetBank, {
    {"Mac", FontDefinition::CharacterSetBank::MacOS},
    {"Win", FontDefinition::CharacterSetBank::Windows},
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

MUSX_XML_ENUM_MAPPING(SmartShape::ShapeType, {
    {"slurDown", SmartShape::ShapeType::SlurDown},
    {"slurUp", SmartShape::ShapeType::SlurUp},
    {"decresc", SmartShape::ShapeType::Decrescendo},
    {"cresc", SmartShape::ShapeType::Crescendo},
    {"octaveDown", SmartShape::ShapeType::OctaveDown},
    {"octaveUp", SmartShape::ShapeType::OctaveUp},
    {"dashLineUp", SmartShape::ShapeType::DashLineUp},
    {"dashLineDown", SmartShape::ShapeType::DashLineDown},
    {"dashSlurDown", SmartShape::ShapeType::DashSlurDown},
    {"dashSlurUp", SmartShape::ShapeType::DashSlurUp},
    {"dashLine", SmartShape::ShapeType::DashLine},
    {"solidLine", SmartShape::ShapeType::SolidLine},
    {"solidLineDown", SmartShape::ShapeType::SolidLineDown},
    {"solidLineUp", SmartShape::ShapeType::SolidLineUp},
    {"trill", SmartShape::ShapeType::Trill},
    {"slurAuto", SmartShape::ShapeType::SlurAuto},
    {"dashSlurAuto", SmartShape::ShapeType::DashSlurAuto},
    {"trillExt", SmartShape::ShapeType::TrillExtension},
    {"solidLineDown2", SmartShape::ShapeType::SolidLineDownBoth},
    {"solidLineUp2", SmartShape::ShapeType::SolidLineUpBoth},
    {"twoOctaveDown", SmartShape::ShapeType::TwoOctaveDown},
    {"twoOctaveUp", SmartShape::ShapeType::TwoOctaveUp},
    {"dashLineDown2", SmartShape::ShapeType::DashLineDownBoth},
    {"dashLineUp2", SmartShape::ShapeType::DashLineUpBoth},
    {"glissando", SmartShape::ShapeType::Glissando},
    {"tabSlide", SmartShape::ShapeType::TabSlide},
    {"bendHat", SmartShape::ShapeType::BendHat},
    {"bendCurve", SmartShape::ShapeType::BendCurve},
    {"smartLine", SmartShape::ShapeType::CustomLine},
    {"solidLineUpLeft", SmartShape::ShapeType::SolidLineUpLeft},
    {"solidLineDownLeft", SmartShape::ShapeType::SolidLineDownLeft},
    {"dashLineUpLeft", SmartShape::ShapeType::DashLineUpLeft},
    {"dashLineDownLeft", SmartShape::ShapeType::DashLineDownLeft},
    {"solidLineUpDown", SmartShape::ShapeType::SolidLineUpDown},
    {"solidLineDownUp", SmartShape::ShapeType::SolidLineDownUp},
    {"dashLineUpDown", SmartShape::ShapeType::DashLineUpDown},
    {"dashLineDownUp", SmartShape::ShapeType::DashLineDownUp},
    {"hyphen", SmartShape::ShapeType::Hyphen},
    {"wordExt", SmartShape::ShapeType::WordExtension},
    {"dashContourSlurDown", SmartShape::ShapeType::DashContourSlurDown},
    {"dashContourSlurUp", SmartShape::ShapeType::DashContourSlurUp},
    {"dashContourSlurAuto", SmartShape::ShapeType::DashContouSlurAuto},
});

MUSX_XML_ENUM_MAPPING(Staff::AutoNumberingStyle, {
    {"arabicSuffix", Staff::AutoNumberingStyle::ArabicSuffix}, //this is the default and may not occur in the xml, but the string is in Finale
    {"romanSuffix", Staff::AutoNumberingStyle::RomanSuffix},
    {"ordinalPrefix", Staff::AutoNumberingStyle::OrdinalPrefix},
    {"alphaSuffix", Staff::AutoNumberingStyle::AlphaSuffix},
    {"arabicPrefix", Staff::AutoNumberingStyle::ArabicPrefix},
});

MUSX_XML_ENUM_MAPPING(Staff::StemDirection, {
    // {"default", Staff::StemDirection::Default}, // this is the default and may not occur in the XML
    {"alwaysUp", Staff::StemDirection::AlwaysUp},
    {"alwaysDown", Staff::StemDirection::AlwaysDown},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::TextBlock::TextType, {
    {"block", TextBlock::TextType::Block},
    {"expression", TextBlock::TextType::Expression}
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::RehearsalMarkStyle, {
    {"letters", RehearsalMarkStyle::Letters},
    {"letNum", RehearsalMarkStyle::LetterNumbers},
    {"lettersLc", RehearsalMarkStyle::LettersLowerCase},
    {"letNumLc", RehearsalMarkStyle::LettersNumbersLowerCase},
    {"numbers", RehearsalMarkStyle::Numbers},
    {"measNum", RehearsalMarkStyle::MeasureNumber}
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::RepeatActionType, {
    {"jumpAuto", RepeatActionType::JumpAuto}, // This is the default and is not known to occur in the XML, but the string exists in Finale
    {"jumpAbsolute", RepeatActionType::JumpAbsolute},
    {"jumpRelative", RepeatActionType::JumpRelative},
    {"jumpToMark", RepeatActionType::JumpToMark},
    {"stop", RepeatActionType::Stop},
    {"noJump", RepeatActionType::NoJump},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::RepeatTriggerType, {
    // {"always", RepeatTriggerType::Always}, // This is the default and is not known to occur in the XML.
    {"onPass", RepeatTriggerType::OnPass},
    {"untilPass", RepeatTriggerType::UntilPass},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::PlaybackType, {
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

MUSX_XML_ENUM_MAPPING(musx::dom::others::HorizontalMeasExprAlign, {
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

MUSX_XML_ENUM_MAPPING(musx::dom::others::VerticalMeasExprAlign, {
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

MUSX_XML_ENUM_MAPPING(musx::dom::others::HorizontalTextJustification, {
    {"left", HorizontalTextJustification::Left},
    {"center", HorizontalTextJustification::Center},
    {"right", HorizontalTextJustification::Right}
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::MarkingCategory::CategoryType, {
    {"dynamics", MarkingCategory::CategoryType::Dynamics},
    {"tempoMarks", MarkingCategory::CategoryType::TempoMarks},
    {"tempoAlts", MarkingCategory::CategoryType::TempoAlterations},
    {"expressiveText", MarkingCategory::CategoryType::ExpressiveText},
    {"techniqueText", MarkingCategory::CategoryType::TechniqueText},
    {"rehearsalMarks", MarkingCategory::CategoryType::RehearsalMarks},
    {"misc", MarkingCategory::CategoryType::Misc}
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::TextRepeatDef::PoundReplaceOption, {
    {"passes", TextRepeatDef::PoundReplaceOption::Passes}, // This is the default and may not appear in the XML, but the string in the Finale app binary
    {"repeatID", TextRepeatDef::PoundReplaceOption::RepeatID},
    {"measNum", TextRepeatDef::PoundReplaceOption::MeasureNumber},
});

} // namespace factory
namespace dom {
namespace others {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(AcciAmountFlats, {
    {"amount", [](const XmlElementPtr& e, const std::shared_ptr<AcciAmountFlats>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciAmountSharps, {
    {"amount", [](const XmlElementPtr& e, const std::shared_ptr<AcciAmountSharps>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciOrderFlats, {
    {"acci", [](const XmlElementPtr& e, const std::shared_ptr<AcciOrderFlats>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciOrderSharps, {
    {"acci", [](const XmlElementPtr& e, const std::shared_ptr<AcciOrderSharps>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefList, {
    {"clef", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefIndex = e->getTextAs<ClefIndex>(); }},
    {"xEduPos", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEduPos = e->getTextAs<Edu>(); }},
    {"yEvpuPos", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->yEvpuPos = e->getTextAs<Evpu>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->percent = e->getTextAs<int>(); }},
    {"xEvpuOffset", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEvpuOffset = e->getTextAs<int>(); }},
    {"clefMode", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefMode = toEnum<ShowClefMode>(e); }},
    {"unlockVert", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->unlockVert = true; }},
    {"afterBarline", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->afterBarline = true; }},
});

MUSX_XML_ELEMENT_ARRAY(FontDefinition, {
    {"charsetBank", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetBank = toEnum<FontDefinition::CharacterSetBank>(e); }},
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
    {"categoryType", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->categoryType = toEnum<MarkingCategory::CategoryType>(e); }},
    {"textFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->textFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"musicFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->musicFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"numberFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->numberFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"horzAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"vertAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"justification", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->justification = toEnum<HorizontalTextJustification>(e); }},
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
    {"keySig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) {
        i->globalKeySig = FieldPopulator<KeySignature>::createAndPopulate(e, i->getDocument()); }},
    {"beats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispDivbeat = e->getTextAs<Cmper>(); }},
    {"frontSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->frontSpaceExtra = e->getTextAs<Evpu>(); }},
    {"backSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backSpaceExtra = e->getTextAs<Evpu>(); }},
    {"breakWordExt", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->breakWordExt = true; }},
    {"hideCaution", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hideCaution = true; }},
    {"hasSmartShape", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasSmartShape = true; }},
    {"showFullNames", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->showFullNames = true; }},
    {"allowSplitPoints", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->allowSplitPoints = true; }},
    {"groupBarlineOverride", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->groupBarlineOverride = true; }},
    {"customBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customBarShape = e->getTextAs<Cmper>(); }},
    {"customLeftBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customLeftBarShape = e->getTextAs<Cmper>(); }},
    {"showKey", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showKey = toEnum<Measure::ShowKeySigMode>(e); }},
    {"showTime", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showTime = toEnum<Measure::ShowTimeSigMode>(e); }},
    {"posMode", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->positioningMode = toEnum<Measure::PositioningType>(e); }},
    {"lineBreak", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->beginNewSystem = true; }},
    {"breakRest", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->breakMmRest = true; }},
    {"noMeasNum", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->noMeasNum = true; }},
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->barlineType = toEnum<Measure::BarlineType>(e); }},
    {"indivPosDef", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->evenlyAcrossMeasure = true; }},
    {"forRepBar", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->forwardRepeatBar= true; }},
    {"bacRepBar", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->backwardsRepeatBar = true; }},
    {"barEnding", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasEnding= true; }},
    {"txtRepeats", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasTextRepeat = true; }},
    {"hasExpr", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->hasExpression = true; }},
    {"altNumTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeNumerator = true; }},
    {"altDenTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDenominator = true; }},
    {"abbrvTime", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->abbrvTime = true; }},
    {"useDisplayTimesig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->useDisplayTimesig = true; }},
    {"leftBarline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->leftBarlineType = toEnum<Measure::BarlineType>(e); }},
    {"displayAltNumTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDispNumerator = true; }},
    {"displayAltDenTsig", [](const XmlElementPtr&, const std::shared_ptr<Measure>& i) { i->compositeDispDenominator = true; }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureExprAssign, {
    {"textExprID", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->textExprId = e->getTextAs<Cmper>(); }},
    {"shapeExprID", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->shapeExprId = e->getTextAs<Cmper>(); }},
    {"horzEvpuOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->horzEvpuOff = e->getTextAs<Evpu>(); }},
    {"horzEduOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->vertEvpuOff = e->getTextAs<Evpu>(); }},
    {"staffAssign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffAssign = e->getTextAs<InstCmper>(); }},
    {"layer", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->layer = e->getTextAs<int>(); }},
    {"dontScaleWithEntry", [](const XmlElementPtr&, const std::shared_ptr<MeasureExprAssign>& i) { i->dontScaleWithEntry = true; }},
    {"staffGroup", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffGroup = e->getTextAs<Cmper>(); }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
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
    {"startAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startAlign = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"multipleAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleAlign = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"mmRestAlign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestAlign = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
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
    {"startJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"multipleJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"mmRestJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }}
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
    {"timePrecision", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->timePrecision = toEnum<MeasureNumberRegion::TimePrecision>(e); }},
    {"hideScroll", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->hideScroll = true; }},
    {"hidePage", [](const XmlElementPtr&, const std::shared_ptr<MeasureNumberRegion>& i) { i->hidePage = true; }}
});

MUSX_XML_ELEMENT_ARRAY(MultimeasureRest, {
    {"meaSpace", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->measWidth = e->getTextAs<Evpu>(); }},
    {"nextMeas", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->nextMeas = e->getTextAs<MeasCmper>(); }},
    {"numdec", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numVertAdj = e->getTextAs<Evpu>(); }},
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"numStart", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numStart = e->getTextAs<int>(); }},
    {"threshold", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->symbolThreshold = e->getTextAs<int>(); }},
    {"spacing", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->symbolSpacing = e->getTextAs<Evpu>(); }},
    {"numAdjX", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numHorzAdj = e->getTextAs<Evpu>(); }},
    {"startAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeStartAdjust = e->getTextAs<Evpu>(); }},
    {"endAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeEndAdjust = e->getTextAs<Evpu>(); }},
    {"useCharRestStyle", [](const XmlElementPtr&, const std::shared_ptr<MultimeasureRest>& i) { i->useSymbols = true; }},
});

MUSX_XML_ELEMENT_ARRAY(MultiStaffGroupId, {
    {"staffGroupID", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffGroupId>& i) { i->staffGroupId = e->getTextAs<Cmper>(); }},
});

// NOTE: zero values are not exported in the xml, so this mapping only pushes non-zero values to the vector of staffNums.
MUSX_XML_ELEMENT_ARRAY(MultiStaffInstrumentGroup, {
    {"staffNum1", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<InstCmper>()); }},
    {"staffNum2", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<InstCmper>()); }},
    {"staffNum3", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<InstCmper>()); }},
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
    {"defaultNameStaff", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameStaff = e->getTextAs<Cmper>(); }},
    {"defaultNameGroup", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameGroup = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartGlobals, {
    {"showTransposed", [](const XmlElementPtr&, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = true; }},
    {"scrollViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatBack, {
    {"actuate", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"pos1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftHPos = e->getTextAs<Evpu>(); }},
    {"line1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftVPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](const XmlElementPtr&, const std::shared_ptr<RepeatBack>& i) { i->individualPlacement = true; }},
    {"topStaffOnly", [](const XmlElementPtr&, const std::shared_ptr<RepeatBack>& i) { i->topStaffOnly = true; }},
    {"clrOnChange", [](const XmlElementPtr&, const std::shared_ptr<RepeatBack>& i) { i->resetOnAction = true; }},
    {"action", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"trigger", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"pos2", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->rightHPos = e->getTextAs<Evpu>(); }},
    {"line2", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->rightVPos = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatEndingStart, {
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"nextEnd", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"textPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->textHPos = e->getTextAs<Evpu>(); }},
    {"pos1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->leftHPos = e->getTextAs<Evpu>(); }},
    {"line1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->leftVPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](const XmlElementPtr&, const std::shared_ptr<RepeatEndingStart>& i) { i->individualPlacement = true; }},
    {"topStaffOnly", [](const XmlElementPtr&, const std::shared_ptr<RepeatEndingStart>& i) { i->topStaffOnly = true; }},
    {"action", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"trigger", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](const XmlElementPtr&, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpIfIgnoring = true; }},
    {"endLine", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->endLineVPos = e->getTextAs<Evpu>(); }},
    {"textLine", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->textVPos = e->getTextAs<Evpu>(); }},
    {"pos2", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->rightHPos = e->getTextAs<Evpu>(); }},
    {"line2", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->rightVPos = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatEndingText, {
    {"rptText", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingText>& i) { i->text = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatPassList, {
    {"act", [](const XmlElementPtr& e, const std::shared_ptr<RepeatPassList>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeExpressionDef, {
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->breakMmRest = true; }},
    {"useAuxData", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->useAuxData = true; }},
    {"masterShape", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->masterShape = true; }},
    {"noPrint", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->noPrint = true; }},
    {"noHorzStretch", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->noHorzStretch = true; }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalTextJustification>(e); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryPos", [](const XmlElementPtr&, const std::shared_ptr<ShapeExpressionDef>& i) { i->useCategoryPos = true; }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->description = e->getText(); }},
    });

MUSX_XML_ELEMENT_ARRAY(SmartShape::EndPoint, {
    {"inst", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::EndPoint>& i) { i->staffId = e->getTextAs<InstCmper>(); }},
    {"meas", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::EndPoint>& i) { i->measId = e->getTextAs<MeasCmper>(); }},
    {"entryNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::EndPoint>& i) { i->entryNumber = e->getTextAs<EntryNumber>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape::EndPointAdjustment, {
    {"x", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::EndPointAdjustment>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"y", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::EndPointAdjustment>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape::TerminationSeg, {
    {"endPt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPoint = FieldPopulator<SmartShape::EndPoint>::createAndPopulate(e, i->getDocument()); }},
    {"endPtAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPointAdj = FieldPopulator<SmartShape::EndPointAdjustment>::createAndPopulate(e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape, {
    {"shapeType", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->shapeType = toEnum<SmartShape::ShapeType>(e); }},
    {"entryBased", [](const XmlElementPtr&, const std::shared_ptr<SmartShape>& i) { i->entryBased = true; }},
    {"startTermSeg", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->startTermSeg = FieldPopulator<SmartShape::TerminationSeg>::createAndPopulate(e, i->getDocument()); }},
    {"endTermSeg", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->endTermSeg = FieldPopulator<SmartShape::TerminationSeg>::createAndPopulate(e, i->getDocument()); }},
    {"startNoteID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startNoteId= e->getTextAs<NoteNumber>(); }},
    {"endNoteID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endNoteId = e->getTextAs<NoteNumber>(); }},
    {"lineStyleID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->lineStyleId = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeMeasureAssign, {
    {"shapeNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->shapeNum = e->getTextAs<Cmper>(); }},
    {"centerShapeNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->centerShapeNum = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff, {
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"transposedClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->transposedClef = e->getTextAs<ClefIndex>(); }},
    {"staffLines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->staffLines = e->getTextAs<int>(); }},
    {"customStaff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->customStaff = populateEmbeddedArray<int>(e, "staffLine"); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lineSpace = e->getTextAs<Evpu>(); }},
    {"instUuid", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->instUuid = e->getTextTrimmed(); }},
    {"hasStyles", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->hasStyles = true; }},
    {"showNameParts", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->showNameInParts = true; }},
    {"hideStfNameInScore", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->hideNameInScore = true; }},
    {"topBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topBarlineOffset = e->getTextAs<Evpu>(); }},
    {"botBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botBarlineOffset = e->getTextAs<Evpu>(); }},
    {"dwRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->dwRestOffset = e->getTextAs<Evpu>(); }},
    {"wRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->wRestOffset = e->getTextAs<Evpu>(); }},
    {"hRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hRestOffset = e->getTextAs<Evpu>(); }},
    {"otherRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->otherRestOffset = e->getTextAs<Evpu>(); }},
    {"stemReversal", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemReversal = e->getTextAs<int>(); }},
    {"fullName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fullNameTextId = e->getTextAs<Cmper>(); }},
    {"abbrvName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->abbrvNameTextId = e->getTextAs<Cmper>(); }},
    {"staffLines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->staffLines = e->getTextAs<int>(); }},
    {"botRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"topRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"vertTabNumOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->vertTabNumOff = e->getTextAs<Evpu>(); }},
    {"hideStems", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->hideStems = true; }},
    {"hideBeams", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->hideBeams = true; }},
    {"stemDir", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemDirection= toEnum<Staff::StemDirection>(e); }},
    {"autoNum", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->autoNumbering = toEnum<Staff::AutoNumberingStyle>(e); }},
    {"useAutoNum", [](const XmlElementPtr&, const std::shared_ptr<Staff>& i) { i->useAutoNumbering = true; }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle::Masks, {
    {"defaultClef", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->defaultClef = true; }},
    {"staffType", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->staffType = true; }},
    {"transposition", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->transposition = true; }},
    {"negNameScore", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->negNameScore = true; }},
    {"fullName", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->fullName = true; }},
    {"abrvName", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->abrvName = true; }},
    {"showStems", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->showStems = true; }},
    {"showNameParts", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle::Masks>& i) { i->showNameParts = true; }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle, []() {
    xml::XmlElementArray<StaffStyle> additionalFields = {
        {"styleName", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->styleName = e->getText(); }},
        {"addToMenu", [](const XmlElementPtr&, const std::shared_ptr<StaffStyle>& i) { i->addToMenu = true; }},
        {"mask", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) {
            i->masks = FieldPopulator<StaffStyle::Masks>::createAndPopulate(e, i->getDocument()); }},
    };
    xml::XmlElementArray<StaffStyle> retval;
    retval.reserve(Staff::xmlMappingArray().size() + additionalFields.size());
    // add to retval in order that it has been observed to appear in xml
    // copy: DO NOT move, because Staff::XmlElementArray is used by Staff as well.
    std::copy(Staff::xmlMappingArray().begin(), Staff::xmlMappingArray().end(), std::back_inserter(retval));
    // move is okay because additionalFields is a local scratch variable.
    std::move(std::make_move_iterator(additionalFields.begin()), std::make_move_iterator(additionalFields.end()), std::back_inserter(retval));
    return retval;
}());

MUSX_XML_ELEMENT_ARRAY(StaffStyleAssign, []() {
    xml::XmlElementArray<StaffStyleAssign> additionalFields = {
        {"style", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyleAssign>& i) { i->styleId = e->getTextAs<Cmper>(); }},
    };
    xml::XmlElementArray<StaffStyleAssign> retval;
    retval.reserve(MusicRange::xmlMappingArray().size() + additionalFields.size());
    // add to retval in order that it has been observed to appear in xml
    // move is okay because additionalFields is a local scratch variable.
    std::move(std::make_move_iterator(additionalFields.begin()), std::make_move_iterator(additionalFields.end()), std::back_inserter(retval));
    // copy: DO NOT move, because Staff::XmlElementArray is used by Staff as well.
    std::copy(MusicRange::xmlMappingArray().begin(), MusicRange::xmlMappingArray().end(), std::back_inserter(retval));
    return retval;
}());

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

MUSX_XML_ELEMENT_ARRAY(TempoChange, {
    {"relativeRatio", [](const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i)
        { i->ratio = e->getTextAs<int>(); i->isRelative = true; }},
    {"absoluteRatio", [](const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i)
        { i->ratio = e->getTextAs<int>(); i->isRelative = false; }},
    {"eldur", [](const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"unit", [](const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i) { i->unit = e->getTextAs<int>(); }},
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
    {"textTag", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textType = toEnum<TextBlock::TextType>(e); }}
});

MUSX_XML_ELEMENT_ARRAY(TextExpressionDef, {
    {"textIDKey", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"hideMeasureNum", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hideMeasureNum = true; }},
    {"matchPlayback", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->matchPlayback = true; }},
    {"useAuxData", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = true; }},
    {"newEnclosure", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = true; }},
    {"createdByHp", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->createdByHp = true; }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalTextJustification>(e); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryFonts = true; }},
    {"useCategoryPos", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryPos = true; }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->description = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatAssign, {
    {"horzPos", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->horzPos = e->getTextAs<Evpu>(); }},
    {"actuate", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"repnum", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->textRepeatId = e->getTextAs<Cmper>(); }},
    {"vertPos", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->vertPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->individualPlacement = true; }},
    {"topStaffOnly", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->topStaffOnly = true; }},
    {"clrOnChange", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->resetOnAction = true; }},
    {"multiActuate", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpOnMultiplePasses = true; }},
    {"action", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"autoUpdate", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->autoUpdate = true; }},
    {"trigger", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](const XmlElementPtr&, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpIfIgnoring = true; }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatDef, {
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"newEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->hasEnclosure = true; }},
    {"useThisFont", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->useThisFont = true; }},
    {"poundReplace", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->poundReplace = toEnum<TextRepeatDef::PoundReplaceOption>(e); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->justification = toEnum<HorizontalTextJustification>(e); }},
    {"act", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->passList.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatText, {
    {"rptText", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatText>& i) { i->text = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeLower::CompositeItem, {
    {"integer", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->unit = e->getTextAs<Edu>(); }},
    {"startGroup", [](const XmlElementPtr&, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->startGroup = true; }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeLower, {
    {"tldata", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower>& i) {
        i->items.push_back(FieldPopulator<TimeCompositeLower::CompositeItem>::createAndPopulate(e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeUpper::CompositeItem, {
    {"integer", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) { i->beats = e->getTextAs<Edu>(); }},
    {"frac", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) {
        auto frac = e->getTextAs<uint16_t>();
        i->fraction = util::Fraction(frac >> 8, frac & 0xff);
    }},
    {"startGroup", [](const XmlElementPtr&, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) { i->startGroup = true; }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeUpper, {
    {"tudata", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper>& i) {
        i->items.push_back(FieldPopulator<TimeCompositeUpper::CompositeItem>::createAndPopulate(e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(TonalCenterFlats, {
    {"tcent", [](const XmlElementPtr& e, const std::shared_ptr<TonalCenterFlats>& i) { i->values.push_back(e->getTextAs<unsigned>()); }},
});

MUSX_XML_ELEMENT_ARRAY(TonalCenterSharps, {
    {"tcent", [](const XmlElementPtr& e, const std::shared_ptr<TonalCenterSharps>& i) { i->values.push_back(e->getTextAs<unsigned>()); }},
});

} // namespace others
} // namespace dom
} // namespace musx


#endif // DOXYGEN_SHOULD_IGNORE_THIS
