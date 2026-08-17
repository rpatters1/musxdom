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
#include "musx/factory/FieldPopulatorsOthers.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::smartshape;
using namespace ::musx::dom::others;

extern template const XmlEnumMappingElement<AlignJustify> XmlEnumMapping<AlignJustify>::mapping;
extern template const XmlEnumMappingElement<LyricTextType> XmlEnumMapping<LyricTextType>::mapping;
extern template const XmlEnumMappingElement<ShowClefMode> XmlEnumMapping<ShowClefMode>::mapping;
extern template const XmlEnumMappingElement<StemDirection> XmlEnumMapping<StemDirection>::mapping;
extern template const XmlEnumMappingElement<options::TextOptions::HorizontalAlignment> XmlEnumMapping<options::TextOptions::HorizontalAlignment>::mapping;
extern template const XmlEnumMappingElement<options::TextOptions::VerticalAlignment> XmlEnumMapping<options::TextOptions::VerticalAlignment>::mapping;
extern template const XmlEnumMappingElement<options::TextOptions::TextJustify> XmlEnumMapping<options::TextOptions::TextJustify>::mapping;

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

MUSX_XML_ENUM_MAPPING(ArticulationDef::AutoVerticalMode, {
    // {"alwaysNoteheadSide", ArticulationDef::AutoVerticalMode::AlwaysNoteheadSide}, // Default value, may not appear in XML.
    {"autoNoteStem", ArticulationDef::AutoVerticalMode::AutoNoteStem},
    {"stemSide", ArticulationDef::AutoVerticalMode::StemSide},
    {"alwaysOnStem", ArticulationDef::AutoVerticalMode::AlwaysOnStem},
    {"aboveEntry", ArticulationDef::AutoVerticalMode::AboveEntry},
    {"belowEntry", ArticulationDef::AutoVerticalMode::BelowEntry},
});

MUSX_XML_ENUM_MAPPING(ArticulationDef::CopyMode, {
    // {"none", ArticulationDef::CopyMode::None}, // Default value, may not appear in XML.
    {"both", ArticulationDef::CopyMode::Vertical},
    {"horizontal", ArticulationDef::CopyMode::Horizontal},
});

MUSX_XML_ENUM_MAPPING(ArticulationDef::SlurInteractionMode, {
    // {"ignore", ArticulationDef::SlurInteractionMode::Ignore}, // Default value, may not appear in XML.
    {"insideSlur", ArticulationDef::SlurInteractionMode::InsideSlur},
    {"avoidSlur", ArticulationDef::SlurInteractionMode::AvoidSlur},
});

MUSX_XML_ENUM_MAPPING(FontDefinition::CharacterSetBank, {
    {"Mac", FontDefinition::CharacterSetBank::MacOS},
    {"Win", FontDefinition::CharacterSetBank::Windows},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::ChordSuffixElement::Prefix, {
    // {"none", ChordSuffixElement::Prefix::None}, // Default value, may not appear in XML.
    {"minus", ChordSuffixElement::Prefix::Minus},
    {"plus", ChordSuffixElement::Prefix::Plus},
    {"sharp", ChordSuffixElement::Prefix::Sharp},
    {"flat", ChordSuffixElement::Prefix::Flat},
});

MUSX_XML_ENUM_MAPPING(FileDescription::PathType, {
    {"dosPath", FileDescription::PathType::DosPath},
    {"macFsSpec", FileDescription::PathType::MacFsSpec},
    {"macAlias", FileDescription::PathType::MacAlias},
    {"macPosixPath", FileDescription::PathType::MacPosixPath},
    {"macURLBookmark", FileDescription::PathType::MacUrlBookmark},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::MeasureExprAssign::ChannelSwitchTarget, {
    // {"", MeasureExprAssign::ChannelSwitchTarget::Current}, // Default; may not appear in the XML.
    {"toL1", MeasureExprAssign::ChannelSwitchTarget::ToLayer1},
    {"toL2", MeasureExprAssign::ChannelSwitchTarget::ToLayer2},
    {"toL3", MeasureExprAssign::ChannelSwitchTarget::ToLayer3},
    {"toL4", MeasureExprAssign::ChannelSwitchTarget::ToLayer4},
    {"toChord", MeasureExprAssign::ChannelSwitchTarget::ToChord},
    {"toDyn", MeasureExprAssign::ChannelSwitchTarget::ToExpression},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::MeasureExprAssign::PlaybackStart, {
    // {"", MeasureExprAssign::PlaybackStart::BeginningOfMeasure}, // Default; may not appear in the XML.
    {"attach", MeasureExprAssign::PlaybackStart::AlignmentPoint},
    {"measPos", MeasureExprAssign::PlaybackStart::PositionInMeasure},
});

MUSX_XML_ENUM_MAPPING(musx::dom::others::MeasureExprAssign::ShowStaffList, {
    // {"", MeasureExprAssign::ShowStaffList::ScoreAndPart}, // Default; may not appear in the XML.
    {"scoreOnly", MeasureExprAssign::ShowStaffList::ScoreOnly},
    {"partOnly", MeasureExprAssign::ShowStaffList::PartOnly},
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

MUSX_XML_ENUM_MAPPING(PageGraphicAssign::PageAssignType, {
    {"all", PageGraphicAssign::PageAssignType::AllPages},
    {"even", PageGraphicAssign::PageAssignType::Even},
    {"odd", PageGraphicAssign::PageAssignType::Odd},
    {"one", PageGraphicAssign::PageAssignType::One},
});

MUSX_XML_ENUM_MAPPING(PageGraphicAssign::PositionFrom, {
    {"margins", PageGraphicAssign::PositionFrom::Margins},
    {"paper", PageGraphicAssign::PositionFrom::PageEdge},
});

MUSX_XML_ENUM_MAPPING(PageTextAssign::PageAssignType, {
    // {"all", PageTextAssign::PageAssignType::All}, // This is the default and is not known to occur in the XML.
    {"even", PageTextAssign::PageAssignType::Even},
    {"odd", PageTextAssign::PageAssignType::Odd},
});

MUSX_XML_ENUM_MAPPING(PartVoicing::VoicingType, {
    {"useSingleLayer", PartVoicing::VoicingType::UseSingleLayer}, // Default value, may not appear in XML, but the Finale binary contains the string.
    {"useMultipleLayers", PartVoicing::VoicingType::UseMultipleLayers},
});

MUSX_XML_ENUM_MAPPING(PartVoicing::SingleLayerVoiceType, {
    {"allNotes", PartVoicing::SingleLayerVoiceType::AllNotes}, // Default value, may not appear in XML, but the Finale binary contains the string.
    {"topNote", PartVoicing::SingleLayerVoiceType::TopNote},
    {"botNote", PartVoicing::SingleLayerVoiceType::BottomNote},
    {"selected", PartVoicing::SingleLayerVoiceType::SelectedNotes},
});

MUSX_XML_ENUM_MAPPING(ShapeDefInstructionType, {
    // {"undocumented", ShapeDefInstructionType::Undocumented}, // Default value does not appear in the xml
    {"bracket", ShapeDefInstructionType::Bracket},
    {"cloneChar", ShapeDefInstructionType::CloneChar},
    {"closePath", ShapeDefInstructionType::ClosePath},
    {"curveTo", ShapeDefInstructionType::CurveTo},
    {"drawChar", ShapeDefInstructionType::DrawChar},
    {"ellipse", ShapeDefInstructionType::Ellipse},
    {"endGroup", ShapeDefInstructionType::EndGroup},
    {"extGraphic", ShapeDefInstructionType::ExternalGraphic},
    {"fillAlt", ShapeDefInstructionType::FillAlt},
    {"fillSolid", ShapeDefInstructionType::FillSolid},
    {"goToOrigin", ShapeDefInstructionType::GoToOrigin},
    {"goToStart", ShapeDefInstructionType::GoToStart},
    {"lineWidth", ShapeDefInstructionType::LineWidth},
    {"rectangle", ShapeDefInstructionType::Rectangle},
    {"rLineTo", ShapeDefInstructionType::RLineTo},
    {"rMoveTo", ShapeDefInstructionType::RMoveTo},
    {"setArrowhead", ShapeDefInstructionType::SetArrowhead},
    {"setBlack", ShapeDefInstructionType::SetBlack},
    {"setDash", ShapeDefInstructionType::SetDash},
    {"setFont", ShapeDefInstructionType::SetFont},
    {"setGray", ShapeDefInstructionType::SetGray},
    {"setWhite", ShapeDefInstructionType::SetWhite},
    {"slur", ShapeDefInstructionType::Slur},
    {"startGroup", ShapeDefInstructionType::StartGroup},
    {"startObject", ShapeDefInstructionType::StartObject},
    {"stroke", ShapeDefInstructionType::Stroke},
    {"vertMode", ShapeDefInstructionType::VerticalMode},
});

MUSX_XML_ENUM_MAPPING(DirectionType, {
    // {"none", DirectionType::None}, // Default value, may not appear in XML.
    {"under", DirectionType::Under},
    {"over", DirectionType::Over},
});

MUSX_XML_ENUM_MAPPING(EntryConnectionType, {
    {"headLeftTop", EntryConnectionType::HeadLeftTop}, // Default value, may not appear in XML.
    {"headRightTop", EntryConnectionType::HeadRightTop},
    {"headRightBottom", EntryConnectionType::HeadRightBottom},
    {"headLeftBottom", EntryConnectionType::HeadLeftBottom},
    {"stemLeftTop", EntryConnectionType::StemLeftTop},
    {"stemRightTop", EntryConnectionType::StemRightTop},
    {"stemRightBottom", EntryConnectionType::StemRightBottom},
    {"stemLeftBottom", EntryConnectionType::StemLeftBottom},
    {"noteLeftTop", EntryConnectionType::NoteLeftTop},
    {"noteRightTop", EntryConnectionType::NoteRightTop},
    {"noteRightBottom", EntryConnectionType::NoteRightBottom},
    {"noteLeftBottom", EntryConnectionType::NoteLeftBottom},
    {"noteLeftCenter", EntryConnectionType::NoteLeftCenter},
    {"noteRightCenter", EntryConnectionType::NoteRightCenter},
    {"lyricRightCenter", EntryConnectionType::LyricRightCenter},
    {"lyricLeftCenter", EntryConnectionType::LyricLeftCenter},
    {"lyricRightBottom", EntryConnectionType::LyricRightBottom},
    {"headRightLyrBaseline", EntryConnectionType::HeadRightLyricBaseline},
    {"dotRightLyrBaseline", EntryConnectionType::DotRightLyricBaseline},
    {"durationLyrBaseline", EntryConnectionType::DurationLyricBaseline},
    {"systemLeft", EntryConnectionType::SystemLeft},
    {"systemRight", EntryConnectionType::SystemRight},
});

MUSX_XML_ENUM_MAPPING(ShapeDef::ShapeType, {
    {"other", ShapeDef::ShapeType::Other}, // Default value, may not appear in XML, but the Finale binary contains the string.
    {"articulation", ShapeDef::ShapeType::Articulation},
    {"barline", ShapeDef::ShapeType::Barline},
    {"executable", ShapeDef::ShapeType::Executable},
    {"expression", ShapeDef::ShapeType::Expression},
    {"note", ShapeDef::ShapeType::CustomStem},
    {"frame", ShapeDef::ShapeType::Frame},
    {"arrowhead", ShapeDef::ShapeType::Arrowhead},
    {"fretboard", ShapeDef::ShapeType::Fretboard},
    {"clef", ShapeDef::ShapeType::Clef},
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
    {"dashContourSlurAuto", SmartShape::ShapeType::DashContourSlurAuto},
});

MUSX_XML_ENUM_MAPPING(SmartShape::EngraverSlurState, {
    // {"auto", SmartShape::EngraverSlurState::Auto}, // Default value, may not appear in XML.
    {"off", SmartShape::EngraverSlurState::Off},
    {"on", SmartShape::EngraverSlurState::On},
});

MUSX_XML_ENUM_MAPPING(SmartShape::SlurAvoidAccidentalsState, {
    // {"auto", SmartShape::SlurAvoidAccidentalsState::Auto}, // Default value, may not appear in XML.
    {"off", SmartShape::SlurAvoidAccidentalsState::Off},
    {"on", SmartShape::SlurAvoidAccidentalsState::On},
    {"invalid", SmartShape::SlurAvoidAccidentalsState::Invalid},
});

MUSX_XML_ENUM_MAPPING(SmartShape::SystemBreakType, {
    // {"same", SystemBreakType::Same}, // Default value, may not appear in XML.
    {"opposite", SmartShape::SystemBreakType::Opposite},
});

MUSX_XML_ENUM_MAPPING(SmartShapeCustomLine::LineStyle, {
    {"char", SmartShapeCustomLine::LineStyle::Char},
    {"solid", SmartShapeCustomLine::LineStyle::Solid},
    {"dashed", SmartShapeCustomLine::LineStyle::Dashed},
});

MUSX_XML_ENUM_MAPPING(SmartShapeCustomLine::LineCapType, {
    {"none", SmartShapeCustomLine::LineCapType::None},
    {"hook", SmartShapeCustomLine::LineCapType::Hook},
    {"arrowheadPreset", SmartShapeCustomLine::LineCapType::ArrowheadPreset},
    {"arrowheadCustom", SmartShapeCustomLine::LineCapType::ArrowheadCustom},
});

MUSX_XML_ENUM_MAPPING(Staff::AutoNumberingStyle, {
    {"arabicSuffix", Staff::AutoNumberingStyle::ArabicSuffix}, //this is the default and may not occur in the xml, but the string is in Finale
    {"romanSuffix", Staff::AutoNumberingStyle::RomanSuffix},
    {"ordinalPrefix", Staff::AutoNumberingStyle::OrdinalPrefix},
    {"alphaSuffix", Staff::AutoNumberingStyle::AlphaSuffix},
    {"arabicPrefix", Staff::AutoNumberingStyle::ArabicPrefix},
});

MUSX_XML_ENUM_MAPPING(Staff::NotationStyle, {
    // {"standard", Staff::NotationStyle::Standard}, // this is the default and may not occur in the xml
    {"percussion", Staff::NotationStyle::Percussion},
    {"tab", Staff::NotationStyle::Tablature},
});

MUSX_XML_ENUM_MAPPING(Staff::HideMode, {
    // {"none", Staff::HideMode::None}, // default value probably does not appear in xml
    {"cutaway", Staff::HideMode::Cutaway},
    {"scoreParts",  Staff::HideMode::ScoreParts},
    {"score", Staff::HideMode::Score},
});

MUSX_XML_ENUM_MAPPING(Staff::AlternateNotation, {
    // {"normal", Staff::AlternateNotation::Normal}, // this is the default and may not occur in the XML
    {"slashBeats", Staff::AlternateNotation::SlashBeats},
    {"rhythmic", Staff::AlternateNotation::Rhythmic},
    {"blank", Staff::AlternateNotation::Blank},
    {"oneBarRepeat", Staff::AlternateNotation::OneBarRepeat},
    {"twoBarRepeat", Staff::AlternateNotation::TwoBarRepeat},
    {"blankWithRests", Staff::AlternateNotation::BlankWithRests},
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

MUSX_XML_ENUM_MAPPING(musx::dom::others::StaffPlayData::VelocityControlFunction, {
    {"absolute", StaffPlayData::VelocityControlFunction::Absolute},
    {"relative", StaffPlayData::VelocityControlFunction::Relative},
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

namespace smartshape {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(ControlPointAdjustment, {
    {"startCtlPtX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->startCtlPtX = e->getTextAs<Evpu>(); }},
    {"startCtlPtY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->startCtlPtY = e->getTextAs<Evpu>(); }},
    {"endCtlPtX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->endCtlPtX = e->getTextAs<Evpu>(); }},
    {"endCtlPtY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->endCtlPtY = e->getTextAs<Evpu>(); }},
    {"on", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->active = populateBoolean(c, e, i); }},
    {"contextDir", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->contextDir = toEnum<DirectionType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(EndPoint, {
    {"inst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->staffId = e->getTextAs<StaffCmper>(); }},
    {"meas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->measId = e->getTextAs<MeasCmper>(); }},
    {"edu", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"entryNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->entryNumber = e->getTextAs<EntryNumber>(); }},
});

MUSX_XML_ELEMENT_ARRAY(EndPointAdjustment, {
    {"x", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"y", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"on", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->active = populateBoolean(c, e, i); }},
    {"contextDir", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->contextDir = toEnum<DirectionType>(e); }},
    {"contextEntCnct", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->contextEntCnct = toEnum<EntryConnectionType>(e); }},
});

} // namespace smartshape

namespace others {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(AcciAmountFlats, {
    {"amount", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AcciAmountFlats>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciAmountSharps, {
    {"amount", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AcciAmountSharps>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciOrderFlats, {
    {"acci", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AcciOrderFlats>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(AcciOrderSharps, {
    {"acci", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AcciOrderSharps>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ArticulationDef, {
    {"charMain", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->charMain = e->getTextAs<char32_t>(); }},
    {"fontMain", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { populateFontId(c, e, i->fontMain->fontId); }},
    {"sizeMain", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontMain->fontSize = e->getTextAs<int>(); }},
    {"efxMain", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { factory::populateFontEfx(c, e, i->fontMain); }},
    {"copyMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->copyMode = toEnum<ArticulationDef::CopyMode>(e); }},
    {"useTopNote", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->useTopNote = populateBoolean(c, e, i); }},
    {"autoHorz", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoHorz = populateBoolean(c, e, i); }},
    {"autoVert", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoVert = populateBoolean(c, e, i); }},
    {"autoVertMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoVertMode = toEnum<ArticulationDef::AutoVerticalMode>(e); }},
    {"outsideStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->outsideStaff = populateBoolean(c, e, i); }},
    {"aboveSymbolAlt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->aboveSymbolAlt = populateBoolean(c, e, i); }},
    {"belowSymbolAlt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->belowSymbolAlt = populateBoolean(c, e, i); }},
    {"insideSlur", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->insideSlur = populateBoolean(c, e, i); }},
    {"noPrint", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->noPrint = populateBoolean(c, e, i); }},
    {"autoStack", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoStack = populateBoolean(c, e, i); }},
    {"centerOnStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->centerOnStem = populateBoolean(c, e, i); }},
    {"slurInteractionMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->slurInteractionMode = toEnum<ArticulationDef::SlurInteractionMode>(e); }},
    {"charAlt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->charAlt = e->getTextAs<char32_t>(); }},
    {"fontAlt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { populateFontId(c, e, i->fontAlt->fontId); }},
    {"sizeAlt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontAlt->fontSize = e->getTextAs<int>(); }},
    {"efxAlt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { factory::populateFontEfx(c, e, i->fontAlt); }},
    {"xOffsetMain", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->xOffsetMain = e->getTextAs<Evpu>(); }},
    {"yOffsetMain", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->yOffsetMain = e->getTextAs<Evpu>(); }},
    {"defVertPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->defVertPos = e->getTextAs<Evpu>(); }},
    {"avoidStaffLines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->avoidStaffLines = populateBoolean(c, e, i); }},
    {"isStemSideWhenMultipleLayers", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->isStemSideWhenMultipleLayers = populateBoolean(c, e, i); }},
    {"playArtic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->playArtic = populateBoolean(c, e, i); }},
    {"xOffsetAlt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->xOffsetAlt = e->getTextAs<Evpu>(); }},
    {"yOffsetAlt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->yOffsetAlt = e->getTextAs<Evpu>(); }},
    {"mainIsShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->mainIsShape = populateBoolean(c, e, i); }},
    {"altIsShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->altIsShape = populateBoolean(c, e, i); }},
    {"mainShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->mainShape = e->getTextAs<Cmper>(); }},
    {"altShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->altShape = e->getTextAs<Cmper>(); }},
    {"startTopNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startTopNoteDelta = e->getTextAs<int>(); }},
    {"startBotNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startBotNoteDelta = e->getTextAs<int>(); }},
    {"startTopNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startTopNotePercent = e->getTextAs<int>(); }},
    {"startBotNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startBotNotePercent = e->getTextAs<int>(); }},
    {"durTopNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durTopNoteDelta = e->getTextAs<int>(); }},
    {"durBotNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durBotNoteDelta = e->getTextAs<int>(); }},
    {"durTopNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durTopNotePercent = e->getTextAs<int>(); }},
    {"durBotNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durBotNotePercent = e->getTextAs<int>(); }},
    {"ampTopNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampTopNoteDelta = e->getTextAs<int>(); }},
    {"ampBotNoteDelta", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampBotNoteDelta = e->getTextAs<int>(); }},
    {"ampTopNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampTopNotePercent = e->getTextAs<int>(); }},
    {"ampBotNotePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampBotNotePercent = e->getTextAs<int>(); }},
    {"distanceFromStemEnd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->distanceFromStemEnd = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeatChartElement::Control, {
    {"totalDur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->totalDur = e->getTextAs<Edu>(); }},
    {"totalWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->totalWidth = e->getTextAs<Evpu>(); }},
    {"minWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->minWidth = e->getTextAs<Evpu>(); }},
    {"allotWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->allotWidth = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(BeatChartElement, {
    {"control", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i)
        { i->control = FieldPopulator<BeatChartElement::Control>::createAndPopulate(c, e); }},
    {"dur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->dur = e->getTextAs<Edu>(); }},
    {"pos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->pos = e->getTextAs<Evpu>(); }},
    {"endPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->endPos = e->getTextAs<Evpu>(); }},
    {"minPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->minPos = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(ChordSuffixElement, {
    { "fontID", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    { "fontSize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    { "efx", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    { "suffix", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->symbol = e->getTextAs<char32_t>(); }},
    { "xdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->xdisp = e->getTextAs<Evpu>(); }},
    { "ydisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->ydisp = e->getTextAs<Evpu>(); }},
    { "isNumber", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->isNumber = populateBoolean(c, e, i); }},
    { "prefix", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->prefix = toEnum<ChordSuffixElement::Prefix>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordSuffixPlayback, {
    {"data", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordSuffixPlayback>& i) { i->values.push_back(e->getTextAs<int16_t>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefList, {
    {"clef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefIndex = e->getTextAs<ClefIndex>(); }},
    {"xEduPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEduPos = e->getTextAs<Edu>(); }},
    {"yEvpuPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->yEvpuPos = e->getTextAs<Evpu>(); }},
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->percent = e->getTextAs<int>(); }},
    {"xEvpuOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEvpuOffset = e->getTextAs<int>(); }},
    {"clefMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefMode = toEnum<ShowClefMode>(e); }},
    {"unlockVert", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->unlockVert = populateBoolean(c, e, i); }},
    {"afterBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->afterBarline = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(DrumStaff, {
    {"whichDrumLib", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<DrumStaff>& i) { i->whichDrumLib = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FileAlias, {
    {"length", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileAlias>& i) { i->length = e->getTextAs<size_t>(); }},
    {"aliasHandle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileAlias>& i) { i->aliasHandle = hexToBytes(e->getText()); }},
});

MUSX_XML_ELEMENT_ARRAY(FileDescription, {
    {"version", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"volRefNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i){ i->volRefNum = e->getTextAs<int>(); }},
    {"dirID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i){ i->dirId = e->getTextAs<int>(); }},
    {"pathType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->pathType = toEnum<FileDescription::PathType>(e); }},
    {"pathID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->pathId = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FilePath, {
    {"path", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FilePath>& i) { i->path = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(FileUrlBookmark, {
    {"length", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileUrlBookmark>& i) { i->length = e->getTextAs<size_t>(); }},
    {"urlBookmarkData", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FileUrlBookmark>& i) { i->urlBookmarkData = hexToBytes(e->getText()); }},
});

MUSX_XML_ELEMENT_ARRAY(FontDefinition, {
    {"charsetBank", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetBank = toEnum<FontDefinition::CharacterSetBank>(e); }},
    {"charsetVal", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetVal = e->getTextAs<int>(); }},
    {"pitch", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->pitch = e->getTextAs<int>(); }},
    {"family", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->family = e->getTextAs<int>(); }},
    {"name", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(Frame, {
    {"startEntry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->startEntry = e->getTextAs<EntryNumber>(); }},
    {"endEntry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->endEntry = e->getTextAs<EntryNumber>(); }},
    {"startTime", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Frame>& i) { i->startTime = e->getTextAs<Edu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardGroup, {
    {"fretInstID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardGroup>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"name", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardGroup>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretInstrument::StringInfo, {
    {"pitch", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument::StringInfo>& i) { i->pitch = e->getTextAs<int>(); }},
    {"nutOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument::StringInfo>& i) { i->nutOffset = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretInstrument, {
    {"numFrets", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->numFrets = e->getTextAs<int>(); }},
    {"numStrings", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->numStrings = e->getTextAs<int>(); }},
    {"name", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->name = e->getText(); }},
    {"string", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i)
        { i->strings.push_back(FieldPopulator<FretInstrument::StringInfo>::createAndPopulate(c, e)); }},
    {"diatonic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->fretSteps = populateEmbeddedArray<int>(c, e, "fret"); }},
    {"speedyClef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->speedyClef = e->getTextAs<ClefIndex>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardStyle, {
    {"showLastFret", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->showLastFret = populateBoolean(c, e, i); }},
    {"rotate", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->rotate = populateBoolean(c, e, i); }},
    {"fingNumWhite", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fingNumWhite = populateBoolean(c, e, i); }},
    {"fingStrShapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fingStrShapeId = e->getTextAs<Cmper>(); }},
    {"openStrShapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->openStrShapeId = e->getTextAs<Cmper>(); }},
    {"muteStrShapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->muteStrShapeId = e->getTextAs<Cmper>(); }},
    {"barreShapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->barreShapeId = e->getTextAs<Cmper>(); }},
    {"customShapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->customShapeId = e->getTextAs<Cmper>(); }},
    {"defNumFrets", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->defNumFrets = e->getTextAs<int>(); }},
    {"stringGap", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->stringGap = e->getTextAs<Efix>(); }},
    {"fretGap", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretGap = e->getTextAs<Efix>(); }},
    {"stringWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->stringWidth = e->getTextAs<Efix>(); }},
    {"fretWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretWidth = e->getTextAs<Efix>(); }},
    {"nutWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->nutWidth = e->getTextAs<Efix>(); }},
    {"vertTextOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertTextOff = e->getTextAs<Efix>(); }},
    {"horzTextOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzTextOff = e->getTextAs<Efix>(); }},
    {"horzHandleOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzHandleOff = e->getTextAs<Efix>(); }},
    {"vertHandleOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertHandleOff = e->getTextAs<Efix>(); }},
    {"whiteout", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->whiteout = e->getTextAs<Efix>(); }},
    {"fretNumFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i)
        { i->fretNumFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"fingNumFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i)
        { i->fingNumFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"horzFingNumOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzFingNumOff = e->getTextAs<Efix>(); }},
    {"vertFingNumOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertFingNumOff = e->getTextAs<Efix>(); }},
    {"name", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->name = e->getTextAs<std::string>(); }},
    {"fretNumText", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretNumText = e->getTextAs<std::string>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyFormat, {
    {"semitones", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyFormat>& i) { i->semitones = e->getTextAs<unsigned>(); }},
    {"scaleTones", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyFormat>& i) { i->scaleTones = e->getTextAs<unsigned>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyMapArray::StepElement, {
    {"diatonic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeyMapArray::StepElement>& i) { i->diatonic = populateBoolean(c, e, i); }},
    {"hlevel", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyMapArray::StepElement>& i) { i->hlevel = e->getTextAs<unsigned>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyMapArray, {
    {"keych", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeyMapArray>& i) { i->steps.push_back(FieldPopulator<KeyMapArray::StepElement>::createAndPopulate(c, e)); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyAttributes, {
    {"harmRefer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->harmRefer = e->getTextAs<int>(); }},
    {"middleCKey", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->middleCKey = e->getTextAs<int>(); }},
    {"fontSym", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { populateFontId(c, e, i->fontSym); }},
    {"gotoKey", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->gotoKey = e->getTextAs<int>(); }},
    {"symbolList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->symbolList = e->getTextAs<Cmper>(); }},
    {"hasClefOctv", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->hasClefOctv = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LayerAttributes, {
    {"restOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->restOffset = e->getTextAs<int>(); }},
    {"flipTies", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezTiesToStems = populateBoolean(c, e, i); }},
    {"floatLayer", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->onlyIfOtherLayersHaveNotes = populateBoolean(c, e, i); }},
    {"useRestOff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->useRestOffset = populateBoolean(c, e, i); }},
    {"freezeLayUp", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezeStemsUp = populateBoolean(c, e, i); }},
    {"freezeLayer", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezeLayer = populateBoolean(c, e, i); }},
    {"playback", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->playback = populateBoolean(c, e, i); }},
    {"spacing", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->affectSpacing = populateBoolean(c, e, i); }},
    {"ignoreHidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenNotesOnly = populateBoolean(c, e, i); }},
    {"ignoreHiddenLayers", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenLayers = populateBoolean(c, e, i); }},
    {"hideLayer", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->hideLayer = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MarkingCategory, {
    {"categoryType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->categoryType = toEnum<MarkingCategory::CategoryType>(e); }},
    {"textFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->textFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"musicFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->musicFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"numberFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->numberFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"horzAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"vertAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"justification", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->justification = toEnum<AlignJustify>(e); }},
    {"horzOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"vertOffsetBaseline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetBaseline = e->getTextAs<Evpu>(); }},
    {"vertOffsetEntry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetEntry = e->getTextAs<Evpu>(); }},
    {"usesTextFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesTextFont = populateBoolean(c, e, i); }},
    {"usesMusicFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesMusicFont = populateBoolean(c, e, i); }},
    {"usesNumberFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesNumberFont = populateBoolean(c, e, i); }},
    {"usesPositioning", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesPositioning = populateBoolean(c, e, i); }},
    {"usesStaffList", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesStaffList = populateBoolean(c, e, i); }},
    {"usesBreakMmRests", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesBreakMmRests = populateBoolean(c, e, i); }},
    {"breakMmRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->breakMmRest = populateBoolean(c, e, i); }},
    {"userCreated", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->userCreated = populateBoolean(c, e, i); }},
    {"staffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->staffList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Measure, {
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"keySig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) {
        i->globalKeySig = FieldPopulator<KeySignature>::createAndPopulate(c, e, i->getDocument()); }},
    {"beats", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivbeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispDivbeat = e->getTextAs<Cmper>(); }},
    {"customBarShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customBarShape = e->getTextAs<Cmper>(); }},
    {"customLeftBarShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customLeftBarShape = e->getTextAs<Cmper>(); }},
    {"frontSpaceExtra", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->frontSpaceExtra = e->getTextAs<Evpu>(); }},
    {"backSpaceExtra", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backSpaceExtra = e->getTextAs<Evpu>(); }},
    {"breakWordExt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->breakWordExt = populateBoolean(c, e, i); }},
    {"hideCaution", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hideCaution = populateBoolean(c, e, i); }},
    {"hasSmartShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasSmartShape = populateBoolean(c, e, i); }},
    {"groupBarlineOverride", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->groupBarlineOverride = populateBoolean(c, e, i); }},
    {"showFullNames", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showFullNames = populateBoolean(c, e, i); }},
    {"mnSepPlace", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasMeasNumbIndivPos = populateBoolean(c, e, i); }},
    {"posSplit", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->allowSplitPoints = populateBoolean(c, e, i); }},
    {"altNumTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeNumerator = populateBoolean(c, e, i); }},
    {"altDenTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDenominator = populateBoolean(c, e, i); }},
    {"showKey", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showKey = toEnum<Measure::ShowKeySigMode>(e); }},
    {"showTime", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showTime = toEnum<Measure::ShowTimeSigMode>(e); }},
    {"indivPosDef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->evenlyAcrossMeasure = populateBoolean(c, e, i); }},
    {"posMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->positioningMode = toEnum<Measure::PositioningType>(e); }},
    {"lineBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beginNewSystem = populateBoolean(c, e, i); }},
    {"hasExpr", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasExpression = populateBoolean(c, e, i); }},
    {"breakRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->breakMmRest = populateBoolean(c, e, i); }},
    {"noMeasNum", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->noMeasNum = populateBoolean(c, e, i); }},
    {"arbitMusic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasOssia = populateBoolean(c, e, i); }},
    {"hasTextBlock", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasTextBlock = populateBoolean(c, e, i); }},
    {"barline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->barlineType = toEnum<Measure::BarlineType>(e); }},
    {"forRepBar", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->forwardRepeatBar = populateBoolean(c, e, i); }},
    {"bacRepBar", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backwardsRepeatBar = populateBoolean(c, e, i); }},
    {"barEnding", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasEnding = populateBoolean(c, e, i); }},
    {"txtRepeats", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasTextRepeat = populateBoolean(c, e, i); }},
    {"abbrvTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->abbrvTime = populateBoolean(c, e, i); }},
    {"useDisplayTimesig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->useDisplayTimesig = populateBoolean(c, e, i); }},
    {"hasChord", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasChord = populateBoolean(c, e, i); }},
    {"leftBarline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->leftBarlineType = toEnum<Measure::BarlineType>(e); }},
    {"displayAltNumTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDispNumerator = populateBoolean(c, e, i); }},
    {"displayAltDenTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDispDenominator = populateBoolean(c, e, i); }},
    {"pageBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->pageBreak = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureExprAssign, {
    {"textExprID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->textExprId = e->getTextAs<Cmper>(); }},
    {"shapeExprID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->shapeExprId = e->getTextAs<Cmper>(); }},
    {"horzEvpuOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->horzEvpuOff = e->getTextAs<Evpu>(); }},
    {"horzEduOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"vertOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->vertEvpuOff = e->getTextAs<Evpu>(); }},
    {"staffAssign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffAssign = e->getTextAs<StaffCmper>(); }},
    {"layer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->layer = e->getTextAs<int>(); }},
    {"v2", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->voice2 = populateBoolean(c, e, i); }},
    {"channelSwitch", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->channelSwitch = toEnum<MeasureExprAssign::ChannelSwitchTarget>(e); }},
    {"dontScaleWithEntry", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->dontScaleWithEntry = populateBoolean(c, e, i); }},
    {"playbackStart", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->playbackStart = toEnum<MeasureExprAssign::PlaybackStart>(e); }},
    {"showStaffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->showStaffList = toEnum<MeasureExprAssign::ShowStaffList>(e); }},
    {"createdByHp", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->createdByHp = populateBoolean(c, e, i); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"staffGroup", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffGroup = e->getTextAs<int>(); }},
    {"staffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"graceNoteIndex", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->graceNoteIndex = e->getTextAs<unsigned>(); }},
    {"rehearsalMarkOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->rehearsalMarkOffset = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion::ScorePartData, {
    {"startFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"multipleFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"mmRestFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"startEnclosure", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startEnclosure = FieldPopulator<Enclosure>::createAndPopulate(c, e, i->getDocument()); }},
    {"multipleEnclosure", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleEnclosure = FieldPopulator<Enclosure>::createAndPopulate(c, e, i->getDocument()); }},
    {"startXdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startXdisp = e->getTextAs<Evpu>(); }},
    {"startYdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startYdisp = e->getTextAs<Evpu>(); }},
    {"multipleXdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleXdisp = e->getTextAs<Evpu>(); }},
    {"multipleYdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleYdisp = e->getTextAs<Evpu>(); }},
    {"mmRestXdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestXdisp = e->getTextAs<Evpu>(); }},
    {"mmRestYdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestYdisp = e->getTextAs<Evpu>(); }},
    {"leftMmBracketChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->leftMmBracketChar = e->getTextAs<char32_t>(); }},
    {"rightMmBracketChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->rightMmBracketChar = e->getTextAs<char32_t>(); }},
    {"startWith", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startWith = e->getTextAs<int>(); }},
    {"incidence", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->incidence = e->getTextAs<int>(); }},
    {"startAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startAlign = toEnum<AlignJustify>(e); }},
    {"multipleAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleAlign = toEnum<AlignJustify>(e); }},
    {"mmRestAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestAlign = toEnum<AlignJustify>(e); }},
    {"startOfLine", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnStart = populateBoolean(c, e, i); }},
    {"multipleOf", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnEvery = populateBoolean(c, e, i); }},
    {"exceptFirstMeas", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->hideFirstMeasure = populateBoolean(c, e, i); }},
    {"mmRestRange", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showMmRange = populateBoolean(c, e, i); }},
    {"mmRestRangeForce", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnMmRest = populateBoolean(c, e, i); }},
    {"useStartEncl", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useStartEncl = populateBoolean(c, e, i); }},
    {"useMultipleEncl", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useMultipleEncl = populateBoolean(c, e, i); }},
    {"showOnTop", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnTop = populateBoolean(c, e, i); }},
    {"showOnBottom", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnBottom = populateBoolean(c, e, i); }},
    {"excludeOthers", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->excludeOthers = populateBoolean(c, e, i); }},
    {"breakMmRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->breakMmRest = populateBoolean(c, e, i); }},
    {"startJustify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startJustify = toEnum<AlignJustify>(e); }},
    {"multipleJustify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleJustify = toEnum<AlignJustify>(e); }},
    {"mmRestJustify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestJustify = toEnum<AlignJustify>(e); }}
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion, {
    {"scoreData", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->scoreData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(c, e, i); }},
    {"partData", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->partData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(c, e, i); }},
    {"startMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"startChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startChar = e->getTextAs<char32_t>(); }},
    {"base", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->base = e->getTextAs<int>(); }},
    {"offset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->numberOffset = e->getTextAs<int>(); }},
    {"prefix", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->prefix = e->getTextTrimmed(); }},
    {"suffix", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->suffix = e->getTextTrimmed(); }},
    {"countFromOne", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->countFromOne = populateBoolean(c, e, i); }},
    {"noZero", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->noZero = populateBoolean(c, e, i); }},
    {"doubleUp", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->doubleUp = populateBoolean(c, e, i); }},
    {"time", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->time = populateBoolean(c, e, i); }},
    {"includeHours", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->includeHours = populateBoolean(c, e, i); }},
    {"smpteFrames", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->smpteFrames = populateBoolean(c, e, i); }},
    {"useScoreInfoForPart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->useScoreInfoForPart = populateBoolean(c, e, i); }},
    {"region", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->region = e->getTextAs<int>(); }},
    {"timePrecision", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->timePrecision = toEnum<MeasureNumberRegion::TimePrecision>(e); }},
    {"hideScroll", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->hideScroll = populateBoolean(c, e, i); }},
    {"hidePage", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->hidePage = populateBoolean(c, e, i); }}
});

MUSX_XML_ELEMENT_ARRAY(MultimeasureRest, {
    {"meaSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->measWidth = e->getTextAs<Evpu>(); }},
    {"nextMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->nextMeas = e->getTextAs<MeasCmper>(); }},
    {"numdec", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numVertAdj = e->getTextAs<Evpu>(); }},
    {"shapeDef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"numStart", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numStart = e->getTextAs<int>(); }},
    {"threshold", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->symbolThreshold = e->getTextAs<int>(); }},
    {"spacing", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->symbolSpacing = e->getTextAs<Evpu>(); }},
    {"numAdjX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->numHorzAdj = e->getTextAs<Evpu>(); }},
    {"startAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeStartAdjust = e->getTextAs<Evpu>(); }},
    {"endAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->shapeEndAdjust = e->getTextAs<Evpu>(); }},
    {"useCharRestStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->useSymbols = populateBoolean(c, e, i); }},
    {"noHorizontalStretch", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->noHorizontalStretch = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MultiStaffGroupId, {
    {"staffGroupID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultiStaffGroupId>& i) { i->staffGroupId = e->getTextAs<Cmper>(); }},
});

// NOTE: zero values are not exported in the xml, so this mapping only pushes non-zero values to the vector of staffNums.
MUSX_XML_ELEMENT_ARRAY(MultiStaffInstrumentGroup, {
    {"staffNum1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
    {"staffNum2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
    {"staffNum3", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaBounds, {
    {"topAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->topMargin = e->getTextAs<Evpu>(); }},
    {"leftAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->leftMargin = e->getTextAs<Evpu>(); }},
    {"rightAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->rightMargin = e->getTextAs<Evpu>(); }},
    {"centerPull", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->centerPull = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaHeader, {
    {"clef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->clefId = e->getTextAs<ClefIndex>(); }},
    {"keySig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) {
        i->keySig = FieldPopulator<KeySignature>::createAndPopulate(c, e, i->getDocument()); }},
    {"beats", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"group", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->bracketGroup = e->getTextAs<Cmper>(); }},
    {"altNumTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->compositeNumerator = populateBoolean(c, e, i); }},
    {"altDenTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->compositeDenominator = populateBoolean(c, e, i); }},
    {"barline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->barlineType = toEnum<Measure::BarlineType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaMusic, {
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->percent = e->getTextAs<int>(); }},
    {"qinst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->sourceStaffId = e->getTextAs<StaffCmper>(); }},
    {"qmeasure", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->sourceMeasureId = e->getTextAs<MeasCmper>(); }},
    {"negLeftLine", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideLeftLine = populateBoolean(c, e, i); }},
    {"negKey", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideKey = populateBoolean(c, e, i); }},
    {"negTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideTime = populateBoolean(c, e, i); }},
    {"negDynamic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideDynamic = populateBoolean(c, e, i); }},
    {"negStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideStaff = populateBoolean(c, e, i); }},
    {"negRepeat", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideRepeat = populateBoolean(c, e, i); }},
    {"negClef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideClef = populateBoolean(c, e, i); }},
    {"posMode", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<OssiaMusic>&) { /* no-op: intentionally suppressed */ }},
});

MUSX_XML_ELEMENT_ARRAY(Page, {
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->percent = e->getTextAs<int>(); }},
    {"firstSystem", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->firstSystemId = e->getTextAs<SystemCmper>(); }},
    {"scaleContentOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->holdMargins = populateBoolean(c, e, i); }},
    {"margTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margTop = e->getTextAs<Evpu>(); }},
    {"margLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margLeft = e->getTextAs<Evpu>(); }},
    {"margBottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margBottom = e->getTextAs<Evpu>(); }},
    {"margRight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margRight = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PageGraphicAssign, {
    {"version", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"left", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"bottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"fDescID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->fDescId = e->getTextAs<Cmper>(); }},
    {"displayType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->displayType = toEnum<PageGraphicAssign::PageAssignType>(e); }},
    {"displayHidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"halign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->hAlign = toEnum<PageGraphicAssign::HorizontalAlignment>(e); }},
    {"valign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->vAlign = toEnum<PageGraphicAssign::VerticalAlignment>(e); }},
    {"posFrom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->posFrom = toEnum<PageGraphicAssign::PositionFrom>(e); }},
    {"fixedPerc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->fixedPerc = populateBoolean(c, e, i); }},
    {"startPage", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->startPage = e->getTextAs<PageCmper>(); }},
    {"endPage", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->endPage = e->getTextAs<PageCmper>(); }},
    {"savedRecord", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->savedRecord = populateBoolean(c, e, i); }},
    {"origWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->origWidth = e->getTextAs<Evpu>(); }},
    {"origHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->origHeight = e->getTextAs<Evpu>(); }},
    {"rightPgHAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgHAlign = toEnum<PageGraphicAssign::HorizontalAlignment>(e); }},
    {"rightPgVAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgVAlign = toEnum<PageGraphicAssign::VerticalAlignment>(e); }},
    {"rightPgPosFrom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgPosFrom = toEnum<PageGraphicAssign::PositionFrom>(e); }},
    {"rightPgFixedPerc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgFixedPerc = populateBoolean(c, e, i); }},
    {"rightPgLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgLeft = e->getTextAs<Evpu>(); }},
    {"rightPgBottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgBottom = e->getTextAs<Evpu>(); }},
    {"graphicCmper", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->graphicCmper = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PageOssiaAssign, {
    {"arbnum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->ossiaId = e->getTextAs<Cmper>(); }},
    {"topAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"leftAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
    {"mwidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->measureWidth = e->getTextAs<Evpu>(); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(PageTextAssign, {
    {"block", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->block = e->getTextAs<Cmper>(); }},
    {"xdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->xDisp = e->getTextAs<Evpu>(); }},
    {"ydisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->yDisp = e->getTextAs<Evpu>(); }},
    {"startPage", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->startPage = e->getTextAs<PageCmper>(); }},
    {"endPage", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->endPage = e->getTextAs<PageCmper>(); }},
    {"oddEven", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->oddEven = toEnum<PageTextAssign::PageAssignType>(e); }},
    {"hposLp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosLp = toEnum<PageTextAssign::HorizontalAlignment>(e); }},
    {"hposRp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosRp = toEnum<PageTextAssign::HorizontalAlignment>(e); }},
    {"postIt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"vpos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->vPos = toEnum<PageTextAssign::VerticalAlignment>(e); }},
    {"hposPageEdge", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosPageEdge = populateBoolean(c, e, i); }},
    {"vposPageEdge", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->vPosPageEdge = populateBoolean(c, e, i); }},
    {"indRpPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->indRpPos = populateBoolean(c, e, i); }},
    {"rightPgXdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->rightPgXDisp = e->getTextAs<Evpu>(); }},
    {"rightPgYdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->rightPgYDisp = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartDefinition, {
    {"nameID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->nameId = e->getTextAs<Cmper>(); }},
    {"partOrder", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->partOrder = e->getTextAs<int>(); }},
    {"copies", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->copies = e->getTextAs<int>(); }},
    {"printPart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->printPart = populateBoolean(c, e, i); }},
    {"extractPart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->extractPart = populateBoolean(c, e, i); }},
    {"applyFormat", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->applyFormat = populateBoolean(c, e, i); }},
    {"needsRecalc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->needsRecalc = populateBoolean(c, e, i); }},
    {"useAsSmpInst", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->useAsSmpInst = populateBoolean(c, e, i); }},
    {"smartMusicInst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->smartMusicInst = e->getTextAs<int>(); }},
    {"defaultNameStaff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameStaff = e->getTextAs<Cmper>(); }},
    {"defaultNameGroup", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameGroup = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartGlobals, {
    {"showTransposed", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = populateBoolean(c, e, i); }},
    {"scrollViewIUlist", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartVoicing, {
    {"enabled", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->enabled = populateBoolean(c, e, i); }},
    {"voicingType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->voicingType = toEnum<PartVoicing::VoicingType>(e); }},
    {"singleVoiceType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->singleLayerVoiceType = toEnum<PartVoicing::SingleLayerVoiceType>(e); }},
    {"select1st", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select1st = populateBoolean(c, e, i); }},
    {"select2nd", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select2nd = populateBoolean(c, e, i); }},
    {"select3rd", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select3rd = populateBoolean(c, e, i); }},
    {"select4th", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select4th = populateBoolean(c, e, i); }},
    {"select5th", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select5th = populateBoolean(c, e, i); }},
    {"selectFromBottom", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->selectFromBottom = populateBoolean(c, e, i); }},
    {"selectSingleNote", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->selectSingleNote = populateBoolean(c, e, i); }},
    {"singleLayer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->singleLayer = e->getTextAs<LayerIndex>(); }},
    {"multiLayer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->multiLayer = e->getTextAs<LayerIndex>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PercussionNoteInfo, {
    {"percNoteType",    [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->percNoteType = e->getTextAs<PercussionNoteTypeId>(); }},
    {"harmLev",         [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->staffPosition = e->getTextAs<int>(); }},
    {"closedNotehead",  [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->closedNotehead = e->getTextAs<char32_t>(); }},
    {"halfNotehead",    [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->halfNotehead = e->getTextAs<char32_t>(); }},
    {"wholeNotehead",   [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->wholeNotehead = e->getTextAs<char32_t>(); }},
    {"dwholeNotehead",  [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->dwholeNotehead = e->getTextAs<char32_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PlaybackRoute, {
    {"virtChannel", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->virtualChannel = e->getTextAs<int>(); }},
    {"patchType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->patchType = e->getText(); }},
    {"patch", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->patch = e->getTextAs<int>(); }},
    {"msb", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->midiBankSelectMostSignificantByte = e->getTextAs<int>(); }},
    {"percMapRefID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->percussionMapId = e->getTextAs<Cmper>(); }},
    {"soundUUID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRoute>& i) { i->soundUuid = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(PlaybackRouteName, {
    {"name", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PlaybackRouteName>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatBack, {
    {"actuate", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"pos1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftHPos = e->getTextAs<Evpu>(); }},
    {"line1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftVPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->individualPlacement = populateBoolean(c, e, i); }},
    {"topStaffOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->topStaffOnly = populateBoolean(c, e, i); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"clrOnChange", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->resetOnAction = populateBoolean(c, e, i); }},
    {"action", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"trigger", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"staffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"pos2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->rightHPos = e->getTextAs<Evpu>(); }},
    {"line2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->rightVPos = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatEndingStart, {
    {"staffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"nextEnd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"textPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->textHPos = e->getTextAs<Evpu>(); }},
    {"pos1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->leftHPos = e->getTextAs<Evpu>(); }},
    {"line1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->leftVPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->individualPlacement = populateBoolean(c, e, i); }},
    {"topStaffOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->topStaffOnly = populateBoolean(c, e, i); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"action", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"trigger", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpIfIgnoring = populateBoolean(c, e, i); }},
    {"endLine", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->endLineVPos = e->getTextAs<Evpu>(); }},
    {"textLine", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->textVPos = e->getTextAs<Evpu>(); }},
    {"pos2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->rightHPos = e->getTextAs<Evpu>(); }},
    {"line2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->rightVPos = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatEndingText, {
    {"rptText", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatEndingText>& i) { i->text = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatIndividualPositioning, {
    { "instno", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->staffId = e->getTextAs<StaffCmper>(); }},
    { "id", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->measureId = e->getTextAs<MeasCmper>(); }},
    { "hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->hidden = populateBoolean(c, e, i); }},
    { "x1add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->x1add = e->getTextAs<Evpu>(); }},
    { "y1add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->y1add = e->getTextAs<Evpu>(); }},
    { "x2add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->x2add = e->getTextAs<Evpu>(); }},
    { "y2add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatIndividualPositioning>& i) { i->y2add = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatPassList, {
    {"act", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatPassList>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeData, {
    {"data", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeData>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeDef, {
    {"instList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->instructionList = e->getTextAs<Cmper>(); }},
    {"dataList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->dataList = e->getTextAs<Cmper>(); }},
    {"shapeType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->shapeType = toEnum<ShapeType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeExpressionDef, {
    {"shapeDef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"categoryID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"execShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->execShape = e->getTextAs<Cmper>(); }},
    {"auxdata1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"breakMmRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->breakMmRest = populateBoolean(c, e, i); }},
    {"useAuxData", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->useAuxData = populateBoolean(c, e, i); }},
    {"masterShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->masterShape = populateBoolean(c, e, i); }},
    {"noPrint", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->noPrint = populateBoolean(c, e, i); }},
    {"noHorzStretch", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->noHorzStretch = populateBoolean(c, e, i); }},
    {"playType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"vertMeasExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"horzExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzExprJustification = toEnum<AlignJustify>(e); }},
    {"measXAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->useCategoryFonts = populateBoolean(c, e, i); }},
    {"useCategoryPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->useCategoryPos = populateBoolean(c, e, i); }},
    {"descStr", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->description = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeGraphicAssign, {
    {"version", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"left", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"bottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"fDescID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->fDescId = e->getTextAs<Cmper>(); }},
    {"displayType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>&)
        {
            if (toEnum<PageGraphicAssign::PageAssignType>(e) != PageGraphicAssign::PageAssignType::One) {
                util::Logger::log(util::Logger::LogLevel::Warning, "Encountered shape graphic with multipage positioning.");
            }
        }
    },
    {"halign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->hAlign = toEnum<ShapeGraphicAssign::HorizontalAlignment>(e); }},
    {"valign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->vAlign = toEnum<ShapeGraphicAssign::VerticalAlignment>(e); } },
    {"posFrom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>&)
        {
            if (toEnum<PageGraphicAssign::PositionFrom>(e) != PageGraphicAssign::PositionFrom::PageEdge) {
                util::Logger::log(util::Logger::LogLevel::Warning, "Encountered shape graphic with page margin positioning.");
            }
        }
    },
    {"displayHidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"fixedPerc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->fixedPerc = populateBoolean(c, e, i); }},
    {"savedRecord", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->savedRecord = populateBoolean(c, e, i); }},
    {"origWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->origWidth = e->getTextAs<Evpu>(); }},
    {"origHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->origHeight = e->getTextAs<Evpu>(); }},
    {"graphicCmper", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeGraphicAssign>& i) { i->graphicCmper = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeInstructionList::InstructionInfo, {
    {"numData", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList::InstructionInfo>& i) { i->numData = e->getTextAs<int>(); }},
    {"tag", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList::InstructionInfo>& i) { i->type = toEnum<ShapeDefInstructionType, true>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeInstructionList, {
    {"instruct", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList>& i) {
        i->instructions.push_back(FieldPopulator<ShapeInstructionList::InstructionInfo>::createAndPopulate(c, e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape::TerminationSeg, {
    {"endPt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPoint = FieldPopulator<smartshape::EndPoint>::populateExistingOrCreate(c, e, i->endPoint, i->getParent()); }},
    {"endPtAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPointAdj = FieldPopulator<smartshape::EndPointAdjustment>::populateExistingOrCreate(c, e, i->endPointAdj, i->getParent()); }},
    {"ctlPtAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->ctlPtAdj = FieldPopulator<smartshape::ControlPointAdjustment>::populateExistingOrCreate(c, e, i->ctlPtAdj, i->getParent()); }},
    {"breakAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->breakAdj = FieldPopulator<smartshape::EndPointAdjustment>::populateExistingOrCreate(c, e, i->breakAdj, i->getParent()); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape, {
    {"shapeType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->shapeType = toEnum<SmartShape::ShapeType>(e); }},
    {"entryBased", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->entryBased = populateBoolean(c, e, i); }},
    {"rotate", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->rotate = populateBoolean(c, e, i); }},
    {"noPresetShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->noPresetShape = populateBoolean(c, e, i); }},
    {"makeHorz", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->makeHorz = populateBoolean(c, e, i); }},
    {"noPushEndStart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->noPushEndStart = populateBoolean(c, e, i); }},
    {"makeVert", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->makeVert = populateBoolean(c, e, i); }},
    {"engraverSlurState", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->engraverSlurState = toEnum<SmartShape::EngraverSlurState>(e); }},
    {"slurAvoidAcciState", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->slurAvoidAcciState = toEnum<SmartShape::SlurAvoidAccidentalsState>(e); }},
    {"yBreakType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->yBreakType = toEnum<SmartShape::SystemBreakType>(e); }},
    {"startTermSeg", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->startTermSeg = FieldPopulator<SmartShape::TerminationSeg>::populateExistingOrCreate(c, e, i->startTermSeg, i); }},
    {"endTermSeg", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->endTermSeg = FieldPopulator<SmartShape::TerminationSeg>::populateExistingOrCreate(c, e, i->endTermSeg, i); }},
    {"fullCtlPtAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->fullCtlPtAdj = FieldPopulator<smartshape::ControlPointAdjustment>::populateExistingOrCreate(c, e, i->fullCtlPtAdj, i); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"startNoteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startNoteId= e->getTextAs<NoteNumber>(); }},
    {"endNoteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endNoteId = e->getTextAs<NoteNumber>(); }},
    {"lineStyleID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->lineStyleId = e->getTextAs<Cmper>(); }},
    {"startLyricNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startLyricNum = e->getTextAs<Cmper>(); }},
    {"endLyricNum",   [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endLyricNum = e->getTextAs<Cmper>(); }},
    {"startLyricTag", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startLyricType = toEnum<LyricTextType>(e); }},
    {"endLyricTag",   [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endLyricType = toEnum<LyricTextType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::CharParams, {
    {"lineChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { i->lineChar = e->getTextAs<char32_t>(); }},
    {"fontID", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"fontSize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"efx", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"baselineShiftEms", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { i->baselineShiftEms = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::SolidParams, {
    {"lineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::SolidParams>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::DashedParams, {
    {"lineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
    {"dashOn", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->dashOn = e->getTextAs<Efix>(); }},
    {"dashOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->dashOff = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine, {
    {"lineStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStyle = toEnum<SmartShapeCustomLine::LineStyle>(e); }},
    {"charParams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->charParams = FieldPopulator<SmartShapeCustomLine::CharParams>::createAndPopulate(c, e, i); }},
    {"solidParams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->solidParams = FieldPopulator<SmartShapeCustomLine::SolidParams>::createAndPopulate(c, e, i); }},
    {"dashedParams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->dashedParams = FieldPopulator<SmartShapeCustomLine::DashedParams>::createAndPopulate(c, e, i); }},
    {"lineCapStartType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartType = toEnum<SmartShapeCustomLine::LineCapType>(e); }},
    {"lineCapEndType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndType = toEnum<SmartShapeCustomLine::LineCapType>(e); }},
    {"lineCapStartArrowID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartArrowId = e->getTextAs<Cmper>(); }},
    {"lineCapEndArrowID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndArrowId = e->getTextAs<Cmper>(); }},
    {"makeHorz", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->makeHorz = populateBoolean(c, e, i); }},
    {"lineAfterLeftStartText", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineAfterLeftStartText = populateBoolean(c, e, i); }},
    {"lineBeforeRightEndText", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineBeforeRightEndText = populateBoolean(c, e, i); }},
    {"lineAfterLeftContText", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineAfterLeftContText = populateBoolean(c, e, i); }},
    {"leftStartRawTextID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartRawTextId = e->getTextAs<Cmper>(); }},
    {"leftContRawTextID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContRawTextId = e->getTextAs<Cmper>(); }},
    {"rightEndRawTextID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndRawTextId = e->getTextAs<Cmper>(); }},
    {"centerFullRawTextID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullRawTextId = e->getTextAs<Cmper>(); }},
    {"centerAbbrRawTextID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrRawTextId = e->getTextAs<Cmper>(); }},
    {"leftStartX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartX = e->getTextAs<Evpu>(); }},
    {"leftStartY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartY = e->getTextAs<Evpu>(); }},
    {"leftContX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContX = e->getTextAs<Evpu>(); }},
    {"leftContY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContY = e->getTextAs<Evpu>(); }},
    {"rightEndX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndX = e->getTextAs<Evpu>(); }},
    {"rightEndY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndY = e->getTextAs<Evpu>(); }},
    {"centerFullX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullX = e->getTextAs<Evpu>(); }},
    {"centerFullY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullY = e->getTextAs<Evpu>(); }},
    {"centerAbbrX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrX = e->getTextAs<Evpu>(); }},
    {"centerAbbrY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrY = e->getTextAs<Evpu>(); }},
    {"lineStartX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStartX = e->getTextAs<Evpu>(); }},
    {"lineStartY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStartY = e->getTextAs<Evpu>(); }},
    {"lineEndX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineEndX = e->getTextAs<Evpu>(); }},
    {"lineEndY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineEndY = e->getTextAs<Evpu>(); }},
    {"lineContX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineContX = e->getTextAs<Evpu>(); }},
    {"lineCapStartHookLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartHookLength = e->getTextAs<Efix>(); }},
    {"lineCapEndHookLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndHookLength = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeMeasureAssign, {
    {"shapeNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->shapeNum = e->getTextAs<Cmper>(); }},
    {"centerShapeNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->centerShapeNum = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SplitMeasure, {
    {"data", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SplitMeasure>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::KeySigTransposition, {
    {"interval", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff::KeySigTransposition>& i) { i->interval = e->getTextAs<int>(); }},
    {"adjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff::KeySigTransposition>& i) { i->adjust = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::ChromaticTransposition, {
    {"alteration", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff::ChromaticTransposition>& i) { i->alteration = e->getTextAs<int>(); }},
    {"diatonic", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff::ChromaticTransposition>& i) { i->diatonic = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::Transposition, {
    {"setToClef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i) { i->setToClef = populateBoolean(c, e, i); }},
    {"noKeyOpt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i) { i->noSimplifyKey = populateBoolean(c, e, i); }},
    {"keysig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i)
        { i->keysig = FieldPopulator<Staff::KeySigTransposition>::createAndPopulate(c, e); }},
    {"chromatic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i)
        { i->chromatic = FieldPopulator<Staff::ChromaticTransposition>::createAndPopulate(c, e); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff, {
    {"notationStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->notationStyle = toEnum<Staff::NotationStyle>(e); }},
    {"noteFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i)
        { i->noteFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"useNoteShapes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useNoteShapes = populateBoolean(c, e, i); }},
    {"useNoteFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useNoteFont = populateBoolean(c, e, i); }},
    {"defaultClef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"transposedClef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->transposedClef = e->getTextAs<ClefIndex>(); }},
    {"staffLines", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->staffLines = e->getTextAs<int>(); }},
    {"customStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->customStaff = populateEmbeddedArray<int>(c, e, "staffLine"); }},
    {"lineSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lineSpace = e->getTextAs<Evpu>(); }},
    {"instUuid", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) {
        auto s = e->getTextTrimmed();
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); });
        i->instUuid = std::move(s);
    }},
    {"capoPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->capoPos = e->getTextAs<int>(); }},
    {"lowestFret", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lowestFret = e->getTextAs<int>(); }},
    {"floatKeys", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->floatKeys = populateBoolean(c, e, i); }},
    {"floatTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->floatTime = populateBoolean(c, e, i); }},
    {"blineBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->blineBreak = populateBoolean(c, e, i); }},
    {"rbarBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->rbarBreak = populateBoolean(c, e, i); }},
    {"hasStyles", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hasStyles = populateBoolean(c, e, i); }},
    {"showNameParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showNameInParts = populateBoolean(c, e, i); }},
    {"showNoteColors", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showNoteColors = populateBoolean(c, e, i); }},
    {"transposition", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i)
        { i->transposition = FieldPopulator<Staff::Transposition>::createAndPopulate(c, e, i); }},
    {"hideStfNameInScore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideNameInScore = populateBoolean(c, e, i); }},
    {"botBarlineOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botBarlineOffset = e->getTextAs<Evpu>(); }},
    {"altNotation", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altNotation = toEnum<Staff::AlternateNotation>(e); }},
    {"altLayer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altLayer = e->getTextAs<LayerIndex>(); }},
    {"altHideArtics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideArtics = populateBoolean(c, e, i); }},
    {"altHideLyrics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideLyrics = populateBoolean(c, e, i); }},
    {"altHideSmartShapes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideSmartShapes = populateBoolean(c, e, i); }},
    {"altRhythmStemsUp", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altRhythmStemsUp = populateBoolean(c, e, i); }},
    {"altSlashDots", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altSlashDots = populateBoolean(c, e, i); }},
    {"altHideOtherNotes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherNotes = populateBoolean(c, e, i); }},
    {"altHideOtherArtics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherArtics = populateBoolean(c, e, i); }},
    {"altHideExpressions", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideExpressions = populateBoolean(c, e, i); }},
    {"altHideOtherLyrics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherLyrics = populateBoolean(c, e, i); }},
    {"altHideOtherSmartShapes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherSmartShapes = populateBoolean(c, e, i); }},
    {"altHideOtherExpressions", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherExpressions = populateBoolean(c, e, i); }},
    {"hideRepeatBottomDot", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeatBottomDot = populateBoolean(c, e, i); }},
    {"flatBeams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->flatBeams = populateBoolean(c, e, i); }},
    {"hideFretboards", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideFretboards = populateBoolean(c, e, i); }},
    {"blankMeasure", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->blankMeasure = populateBoolean(c, e, i); }},
    {"hideRepeatTopDot", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeatTopDot = populateBoolean(c, e, i); }},
    {"hideLyrics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideLyrics = populateBoolean(c, e, i); }},
    {"noOptimize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->noOptimize = populateBoolean(c, e, i); }},
    {"topBarlineOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topBarlineOffset = e->getTextAs<Evpu>(); }},
    {"hideMeasNums", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideMeasNums = populateBoolean(c, e, i); }},
    {"hideRepeats", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeats = populateBoolean(c, e, i); }},
    {"hideBarlines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideBarlines = populateBoolean(c, e, i); }},
    {"hideRptBars", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRptBars = populateBoolean(c, e, i); }},
    {"hideKeySigs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideKeySigs = populateBoolean(c, e, i); }},
    {"hideTimeSigs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTimeSigs = populateBoolean(c, e, i); }},
    {"hideClefs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideClefs = populateBoolean(c, e, i); }},
    {"hideStaffLines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideStaffLines = populateBoolean(c, e, i); }},
    {"hideChords", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideChords = populateBoolean(c, e, i); }},
    {"noKey", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->noKey = populateBoolean(c, e, i); }},
    {"dwRestOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->dwRestOffset = e->getTextAs<Evpu>(); }},
    {"wRestOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->wRestOffset = e->getTextAs<Evpu>(); }},
    {"hRestOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hRestOffset = e->getTextAs<Evpu>(); }},
    {"otherRestOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->otherRestOffset = e->getTextAs<Evpu>(); }},
    {"hideRests", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRests = populateBoolean(c, e, i); }},
    {"hideTies", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTies = populateBoolean(c, e, i); }},
    {"hideDots", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideDots = populateBoolean(c, e, i); }},
    {"stemReversal", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemReversal = e->getTextAs<int>(); }},
    {"fullName", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fullNameTextId = e->getTextAs<Cmper>(); }},
    {"abbrvName", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->abbrvNameTextId = e->getTextAs<Cmper>(); }},
    {"botRepeatDotOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"topRepeatDotOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"vertTabNumOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->vertTabNumOff = e->getTextAs<Efix>(); }},
    {"showTabClefAllSys", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showTabClefAllSys = populateBoolean(c, e, i); }},
    {"useTabLetters", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useTabLetters = populateBoolean(c, e, i); }},
    {"breakTabLinesAtNotes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->breakTabLinesAtNotes = populateBoolean(c, e, i); }},
    {"hideTuplets", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTuplets = populateBoolean(c, e, i); }},
    {"fretInstID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"hideStems", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideStems = populateBoolean(c, e, i); }},
    {"stemDir", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemDirection = toEnum<StemDirection>(e); }},
    {"hideBeams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideBeams = populateBoolean(c, e, i); }},
    {"stemStartFromStaff",[] (ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemStartFromStaff = populateBoolean(c, e, i); }},
    {"stemsFixedEnd",[] (ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemsFixedEnd = populateBoolean(c, e, i); }},
    {"stemsFixedStart",[] (ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemsFixedStart = populateBoolean(c, e, i); }},
    {"horzStemOffUp",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->horzStemOffUp = e->getTextAs<Efix>(); }},
    {"horzStemOffDown",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->horzStemOffDown = e->getTextAs<Efix>(); }},
    {"vertStemStartOffUp",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemStartOffUp = e->getTextAs<Efix>(); }},
    {"vertStemStartOffDown",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemStartOffDown = e->getTextAs<Efix>(); }},
    {"vertStemEndOffUp",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemEndOffUp = e->getTextAs<Efix>(); }},
    {"vertStemEndOffDown",[] (ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemEndOffDown = e->getTextAs<Efix>(); }},
    {"hideMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideMode = toEnum<Staff::HideMode>(e); }},
    {"redisplayLayerAccis", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->redisplayLayerAccis = populateBoolean(c, e, i); }},
    {"hideTimeSigsInParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTimeSigsInParts = populateBoolean(c, e, i); }},
    {"autoNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->autoNumbering = toEnum<Staff::AutoNumberingStyle>(e); }},
    {"useAutoNum", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useAutoNumbering = populateBoolean(c, e, i); }},
    {"hideKeySigsShowAccis", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideKeySigsShowAccis = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffList, {
    {"inst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffList>& i) { i->values.push_back(e->getTextAs<StaffCmper>()); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffPlayData::PlaybackSettings, {
    {"playbackRouteNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::PlaybackSettings>& i) { i->playbackRouteIndex = e->getTextAs<int>(); }},
    {"velControlLevel", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::PlaybackSettings>& i) { i->velocityControlLevel = e->getTextAs<int>(); }},
    {"velControlSaveFlags", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::PlaybackSettings>& i)
        { i->velocityControl = FieldPopulator<StaffPlayData::VelocityControl>::createAndPopulate(c, e, i); }},
    {"play", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::PlaybackSettings>& i) { i->play = populateBoolean(c, e, i); }},
    {"solo", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::PlaybackSettings>& i) { i->solo = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffPlayData::VelocityControl, {
    {"controlOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::VelocityControl>& i) { i->controlOn = populateBoolean(c, e, i); }},
    {"function", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData::VelocityControl>& i) { i->function = toEnum<StaffPlayData::VelocityControlFunction>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffPlayData, {
    {"chords", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->chords = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
    {"midiExpressions", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->midiExpressions = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
    {"layer1", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->layers[0] = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
    {"layer2", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->layers[1] = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
    {"layer3", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->layers[2] = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
    {"layer4", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffPlayData>& i)
        { i->layers[3] = FieldPopulator<StaffPlayData::PlaybackSettings>::createAndPopulate(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle::Masks, {
    {"floatNoteheadFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatNoteheadFont = populateBoolean(c, e, i); }},
    {"useNoteShapes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->useNoteShapes = populateBoolean(c, e, i); }},
    {"flatBeams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->flatBeams = populateBoolean(c, e, i); }},
    {"blankMeasureRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->blankMeasureRest = populateBoolean(c, e, i); }},
    {"noOptimize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->noOptimize = populateBoolean(c, e, i); }},
    {"notationStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->notationStyle = populateBoolean(c, e, i); }},
    {"defaultClef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->defaultClef = populateBoolean(c, e, i); }},
    {"staffType", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->staffType = populateBoolean(c, e, i); }},
    {"transposition", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->transposition = populateBoolean(c, e, i); }},
    {"blineBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->blineBreak = populateBoolean(c, e, i); }},
    {"rbarBreak", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->rbarBreak = populateBoolean(c, e, i); }},
    {"negMnumb", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negMnumb = populateBoolean(c, e, i); }},
    {"negRepeat", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negRepeat = populateBoolean(c, e, i); }},
    {"negNameScore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negNameScore = populateBoolean(c, e, i); }},
    {"hideBarlines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideBarlines = populateBoolean(c, e, i); }},
    {"fullName", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->fullName = populateBoolean(c, e, i); }},
    {"abrvName", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->abrvName = populateBoolean(c, e, i); }},
    {"floatKeys", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatKeys = populateBoolean(c, e, i); }},
    {"floatTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatTime = populateBoolean(c, e, i); }},
    {"hideRptBars", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideRptBars = populateBoolean(c, e, i); }},
    {"negKey", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negKey = populateBoolean(c, e, i); }},
    {"negTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negTime = populateBoolean(c, e, i); }},
    {"negClef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negClef = populateBoolean(c, e, i); }},
    {"hideStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideStaff = populateBoolean(c, e, i); }},
    {"noKey", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->noKey = populateBoolean(c, e, i); }},
    {"fullNamePos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->fullNamePos = populateBoolean(c, e, i); }},
    {"abrvNamePos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->abrvNamePos = populateBoolean(c, e, i); }},
    {"altNotation", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->altNotation = populateBoolean(c, e, i); }},
    {"showTies", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showTies = populateBoolean(c, e, i); }},
    {"showDots", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showDots = populateBoolean(c, e, i); }},
    {"showRests", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showRests = populateBoolean(c, e, i); }},
    {"showStems", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showStems = populateBoolean(c, e, i); }},
    {"hideChords", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideChords = populateBoolean(c, e, i); }},
    {"hideFretboards", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideFretboards = populateBoolean(c, e, i); }},
    {"hideLyrics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideLyrics = populateBoolean(c, e, i); }},
    {"showNameParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showNameParts = populateBoolean(c, e, i); }},
    {"showNoteColors", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showNoteColors = populateBoolean(c, e, i); }},
    {"hideStaffLines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideStaffLines = populateBoolean(c, e, i); }},
    {"redisplayLayerAccis", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->redisplayLayerAccis = populateBoolean(c, e, i); }},
    {"negTimeParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negTimeParts = populateBoolean(c, e, i); }},
    {"hideKeySigsShowAccis", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideKeySigsShowAccis = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle, []() {
    xml::XmlElementArray<StaffStyle> additionalFields = {
        {"styleName", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->styleName = e->getText(); }},
        {"copyable", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->copyable = populateBoolean(c, e, i); }},
        {"addToMenu", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->addToMenu = populateBoolean(c, e, i); }},
        {"mask", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) {
            i->masks = FieldPopulator<StaffStyle::Masks>::createAndPopulate(c, e, i); }},
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
        {"style", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffStyleAssign>& i) { i->styleId = e->getTextAs<Cmper>(); }},
    };
    xml::XmlElementArray<StaffStyleAssign> retval;
    retval.reserve(EnigmaMusicRange::xmlMappingArray().size() + additionalFields.size());
    // add to retval in order that it has been observed to appear in xml
    // move is okay because additionalFields is a local scratch variable.
    std::move(std::make_move_iterator(additionalFields.begin()), std::make_move_iterator(additionalFields.end()), std::back_inserter(retval));
    // copy: DO NOT move, because Staff::XmlElementArray is used by Staff as well.
    std::copy(EnigmaMusicRange::xmlMappingArray().begin(), EnigmaMusicRange::xmlMappingArray().end(), std::back_inserter(retval));
    return retval;
}());

MUSX_XML_ELEMENT_ARRAY(StaffSystem, {
    {"startMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"horzPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->horzPercent = e->getTextAs<double>() / 100.0; }},
    {"ssysPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->ssysPercent = e->getTextAs<int>(); }},
    {"staffHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->staffHeight = e->getTextAs<Efix>(); }},
    {"top", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->top = e->getTextAs<Evpu>(); }},
    {"left", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"right", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->right = e->getTextAs<Evpu>(); }},
    {"bottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"noNames", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->noNames = populateBoolean(c, e, i); }},
    {"hasStaffScaling", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->hasStaffScaling = populateBoolean(c, e, i); }},
    {"placeEndSpaceBeforeBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->placeEndSpaceBeforeBarline = populateBoolean(c, e, i); }},
    {"scaleVert", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->scaleVert = populateBoolean(c, e, i); }},
    {"scaleContentOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->holdMargins = populateBoolean(c, e, i); }},
    {"distanceToPrev", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->distanceToPrev = e->getTextAs<Evpu>(); }},
    {"extraStartSystemSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraStartSystemSpace = e->getTextAs<Evpu>(); }},
    {"extraEndSystemSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraEndSystemSpace = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffUsed, {
    {"inst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i) { i->staffId = e->getTextAs<Cmper>(); }},
    {"trackType", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<StaffUsed>&) { /* this field seems like it was for future enhancement */ }},
    {"distFromTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i) { i->distFromTop = e->getTextAs<Evpu>(); }},
    {"range", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i)
        { i->range = FieldPopulator<EnigmaMusicRange>::createAndPopulate(c, e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(SystemLock, {
    {"endMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SystemLock>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TempoChange, {
    {"relativeRatio", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i)
        { i->ratio = e->getTextAs<int>(); i->isRelative = true; }},
    {"absoluteRatio", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i)
        { i->ratio = e->getTextAs<int>(); i->isRelative = false; }},
    {"eldur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"unit", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TempoChange>& i) { i->unit = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextBlock, {
    {"textID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textId = e->getTextAs<Cmper>(); }},
    {"shapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->shapeId = e->getTextAs<Cmper>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"lineSpacingPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->lineSpacingPercentage = e->getTextAs<int>(); }},
    {"xAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->xAdd = e->getTextAs<Evpu>(); }},
    {"yAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->yAdd = e->getTextAs<Evpu>(); }},
    {"justify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->justify = toEnum<TextBlock::TextJustify>(e); }},
    {"newPos36", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->newPos36 = populateBoolean(c, e, i); }},
    {"showShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->showShape = populateBoolean(c, e, i); }},
    {"noExpandSingleWord", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->noExpandSingleWord = populateBoolean(c, e, i); }},
    {"wordWrap", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->wordWrap = populateBoolean(c, e, i); }},
    {"inset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->inset = e->getTextAs<Efix>(); }},
    {"stdLine", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->stdLineThickness = e->getTextAs<Efix>(); }},
    {"roundCorners", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->roundCorners = populateBoolean(c, e, i); }},
    {"cornerRadius", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->cornerRadius = e->getTextAs<Efix>(); }},
    {"textTag", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textType = toEnum<TextBlock::TextType>(e); }}
});

MUSX_XML_ELEMENT_ARRAY(TextExpressionDef, {
    {"textIDKey", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"execShape", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->execShape = e->getTextAs<Cmper>(); }},
    {"auxdata1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"hideMeasureNum", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->hideMeasureNum = populateBoolean(c, e, i); }},
    {"matchPlayback", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->matchPlayback = populateBoolean(c, e, i); }},
    {"useAuxData", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = populateBoolean(c, e, i); }},
    {"newEnclosure", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = populateBoolean(c, e, i); }},
    {"breakMmRest", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = populateBoolean(c, e, i); }},
    {"createdByHp", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->createdByHp = populateBoolean(c, e, i); }},
    {"playType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"horzExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzExprJustification = toEnum<AlignJustify>(e); }},
    {"vertMeasExprAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"measXAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryFonts = populateBoolean(c, e, i); }},
    {"useCategoryPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryPos = populateBoolean(c, e, i); }},
    {"descStr", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->description = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatAssign, {
    {"horzPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->horzPos = e->getTextAs<Evpu>(); }},
    {"actuate", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"repnum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->textRepeatId = e->getTextAs<Cmper>(); }},
    {"vertPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->vertPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->individualPlacement = populateBoolean(c, e, i); }},
    {"topStaffOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->topStaffOnly = populateBoolean(c, e, i); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"clrOnChange", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->resetOnAction = populateBoolean(c, e, i); }},
    {"multiActuate", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpOnMultiplePasses = populateBoolean(c, e, i); }},
    {"action", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"autoUpdate", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->autoUpdate = populateBoolean(c, e, i); }},
    {"trigger", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpIfIgnoring = populateBoolean(c, e, i); }},
    {"staffList", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatDef, {
    {"fontID", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"fontSize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"efx", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(c, i->font, e); }},
    {"newEnclosure", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->hasEnclosure = populateBoolean(c, e, i); }},
    {"useThisFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->useThisFont = populateBoolean(c, e, i); }},
    {"poundReplace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->poundReplace = toEnum<TextRepeatDef::PoundReplaceOption>(e); }},
    {"justify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->justification = toEnum<AlignJustify>(e); }},
    {"act", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->passList.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatText, {
    {"rptText", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextRepeatText>& i) { i->text = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeLower::CompositeItem, {
    {"integer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->unit = e->getTextAs<Edu>(); }},
    {"startGroup", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->startGroup = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeLower, {
    {"tldata", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower>& i) {
        i->items.push_back(FieldPopulator<TimeCompositeLower::CompositeItem>::createAndPopulate(c, e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeUpper::CompositeItem, {
    {"integer", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) { i->beats = e->getTextAs<Edu>(); }},
    {"frac", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) {
        auto frac = e->getTextAs<uint16_t>();
        i->fraction = util::Fraction(frac >> 8, frac & 0xff);
    }},
    {"startGroup", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) { i->startGroup = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeUpper, {
    {"tudata", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper>& i) {
        i->items.push_back(FieldPopulator<TimeCompositeUpper::CompositeItem>::createAndPopulate(c, e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(TonalCenterFlats, {
    {"tcent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TonalCenterFlats>& i) { i->values.push_back(e->getTextAs<unsigned>()); }},
});

MUSX_XML_ELEMENT_ARRAY(TonalCenterSharps, {
    {"tcent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TonalCenterSharps>& i) { i->values.push_back(e->getTextAs<unsigned>()); }},
});

} // namespace others
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS
