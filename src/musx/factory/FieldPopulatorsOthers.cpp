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
using namespace ::musx::dom::smartshape;
using namespace ::musx::dom::others;

extern template const XmlEnumMappingElement<ShowClefMode> XmlEnumMapping<ShowClefMode>::mapping;
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

MUSX_XML_ENUM_MAPPING(ShapeDef::InstructionType, {
    // {"undocumented", ShapeDef::InstructionType::Undocumented}, // Default value does not appear in the xml
    {"bracket", ShapeDef::InstructionType::Bracket},
    {"cloneChar", ShapeDef::InstructionType::CloneChar},
    {"closePath", ShapeDef::InstructionType::ClosePath},
    {"curveTo", ShapeDef::InstructionType::CurveTo},
    {"drawChar", ShapeDef::InstructionType::DrawChar},
    {"ellipse", ShapeDef::InstructionType::Ellipse},
    {"endGroup", ShapeDef::InstructionType::EndGroup},
    {"extGraphic", ShapeDef::InstructionType::ExternalGraphic},
    {"fillAlt", ShapeDef::InstructionType::FillAlt},
    {"fillSolid", ShapeDef::InstructionType::FillSolid},
    {"goToOrigin", ShapeDef::InstructionType::GoToOrigin},
    {"goToStart", ShapeDef::InstructionType::GoToStart},
    {"lineWidth", ShapeDef::InstructionType::LineWidth},
    {"rectangle", ShapeDef::InstructionType::Rectangle},
    {"rLineTo", ShapeDef::InstructionType::RLineTo},
    {"rMoveTo", ShapeDef::InstructionType::RMoveTo},
    {"setArrowhead", ShapeDef::InstructionType::SetArrowhead},
    {"setBlack", ShapeDef::InstructionType::SetBlack},
    {"setDash", ShapeDef::InstructionType::SetDash},
    {"setFont", ShapeDef::InstructionType::SetFont},
    {"setGray", ShapeDef::InstructionType::SetGray},
    {"setWhite", ShapeDef::InstructionType::SetWhite},
    {"slur", ShapeDef::InstructionType::Slur},
    {"startGroup", ShapeDef::InstructionType::StartGroup},
    {"startObject", ShapeDef::InstructionType::StartObject},
    {"stroke", ShapeDef::InstructionType::Stroke},
    {"vertMode", ShapeDef::InstructionType::VerticalMode},
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

MUSX_XML_ENUM_MAPPING(SmartShape::LyricTextType, {
    // {"none", SmartShape::LyricTextType::None}, // Default value, may not appear in XML.
    {"verse", SmartShape::LyricTextType::Verse},
    {"chorus", SmartShape::LyricTextType::Chorus},
    {"section", SmartShape::LyricTextType::Section},
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

MUSX_XML_ENUM_MAPPING(Staff::StemDirection, {
    // {"default", Staff::StemDirection::Default}, // this is the default and may not occur in the XML
    {"alwaysUp", Staff::StemDirection::AlwaysUp},
    {"alwaysDown", Staff::StemDirection::AlwaysDown},
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

namespace smartshape {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(ControlPointAdjustment, {
    {"startCtlPtX", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->startCtlPtX = e->getTextAs<Evpu>(); }},
    {"startCtlPtY", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->startCtlPtY = e->getTextAs<Evpu>(); }},
    {"endCtlPtX", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->endCtlPtX = e->getTextAs<Evpu>(); }},
    {"endCtlPtY", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->endCtlPtY = e->getTextAs<Evpu>(); }},
    {"on", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->active = populateBoolean(e, i); }},
    {"contextDir", [](const XmlElementPtr& e, const std::shared_ptr<ControlPointAdjustment>& i) { i->contextDir = toEnum<DirectionType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(EndPoint, {
    {"inst", [](const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->staffId = e->getTextAs<StaffCmper>(); }},
    {"meas", [](const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->measId = e->getTextAs<MeasCmper>(); }},
    {"edu", [](const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"entryNum", [](const XmlElementPtr& e, const std::shared_ptr<EndPoint>& i) { i->entryNumber = e->getTextAs<EntryNumber>(); }},
});

MUSX_XML_ELEMENT_ARRAY(EndPointAdjustment, {
    {"x", [](const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"y", [](const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"on", [](const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->active = populateBoolean(e, i); }},
    {"contextDir", [](const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->contextDir = toEnum<DirectionType>(e); }},
    {"contextEntCnct", [](const XmlElementPtr& e, const std::shared_ptr<EndPointAdjustment>& i) { i->contextEntCnct = toEnum<EntryConnectionType>(e); }},
});

} // namespace smartshape

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

MUSX_XML_ELEMENT_ARRAY(ArticulationDef, {
    {"charMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->charMain = e->getTextAs<char32_t>(); }},
    {"fontMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontMain->fontId = e->getTextAs<Cmper>(); }},
    {"sizeMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontMain->fontSize = e->getTextAs<int>(); }},
    {"efxMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { factory::populateFontEfx(e, i->fontMain); }},
    {"copyMode", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->copyMode = toEnum<ArticulationDef::CopyMode>(e); }},
    {"useTopNote", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->useTopNote = populateBoolean(e, i); }},
    {"autoHorz", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoHorz = populateBoolean(e, i); }},
    {"autoVert", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoVert = populateBoolean(e, i); }},
    {"autoVertMode", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoVertMode = toEnum<ArticulationDef::AutoVerticalMode>(e); }},
    {"outsideStaff", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->outsideStaff = populateBoolean(e, i); }},
    {"aboveSymbolAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->aboveSymbolAlt = populateBoolean(e, i); }},
    {"belowSymbolAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->belowSymbolAlt = populateBoolean(e, i); }},
    {"insideSlur", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->insideSlur = populateBoolean(e, i); }},
    {"noPrint", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->noPrint = populateBoolean(e, i); }},
    {"autoStack", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->autoStack = populateBoolean(e, i); }},
    {"centerOnStem", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->centerOnStem = populateBoolean(e, i); }},
    {"slurInteractionMode", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->slurInteractionMode = toEnum<ArticulationDef::SlurInteractionMode>(e); }},
    {"charAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->charAlt = e->getTextAs<char32_t>(); }},
    {"fontAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontAlt->fontId = e->getTextAs<Cmper>(); }},
    {"sizeAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->fontAlt->fontSize = e->getTextAs<int>(); }},
    {"efxAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { factory::populateFontEfx(e, i->fontAlt); }},
    {"xOffsetMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->xOffsetMain = e->getTextAs<Evpu>(); }},
    {"yOffsetMain", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->yOffsetMain = e->getTextAs<Evpu>(); }},
    {"defVertPos", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->defVertPos = e->getTextAs<Evpu>(); }},
    {"avoidStaffLines", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->avoidStaffLines = populateBoolean(e, i); }},
    {"isStemSideWhenMultipleLayers", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->isStemSideWhenMultipleLayers = populateBoolean(e, i); }},
    {"playArtic", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->playArtic = populateBoolean(e, i); }},
    {"xOffsetAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->xOffsetAlt = e->getTextAs<Evpu>(); }},
    {"yOffsetAlt", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->yOffsetAlt = e->getTextAs<Evpu>(); }},
    {"mainIsShape", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->mainIsShape = populateBoolean(e, i); }},
    {"altIsShape", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->altIsShape = populateBoolean(e, i); }},
    {"mainShape", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->mainShape = e->getTextAs<Cmper>(); }},
    {"altShape", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->altShape = e->getTextAs<Cmper>(); }},
    {"startTopNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startTopNoteDelta = e->getTextAs<int>(); }},
    {"startBotNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startBotNoteDelta = e->getTextAs<int>(); }},
    {"startTopNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startTopNotePercent = e->getTextAs<int>(); }},
    {"startBotNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->startBotNotePercent = e->getTextAs<int>(); }},
    {"durTopNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durTopNoteDelta = e->getTextAs<int>(); }},
    {"durBotNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durBotNoteDelta = e->getTextAs<int>(); }},
    {"durTopNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durTopNotePercent = e->getTextAs<int>(); }},
    {"durBotNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->durBotNotePercent = e->getTextAs<int>(); }},
    {"ampTopNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampTopNoteDelta = e->getTextAs<int>(); }},
    {"ampBotNoteDelta", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampBotNoteDelta = e->getTextAs<int>(); }},
    {"ampTopNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampTopNotePercent = e->getTextAs<int>(); }},
    {"ampBotNotePercent", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->ampBotNotePercent = e->getTextAs<int>(); }},
    {"distanceFromStemEnd", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationDef>& i) { i->distanceFromStemEnd = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeatChartElement::Control, {
    {"totalDur", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->totalDur = e->getTextAs<Edu>(); }},
    {"totalWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->totalWidth = e->getTextAs<Evpu>(); }},
    {"minWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->minWidth = e->getTextAs<Evpu>(); }},
    {"allotWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement::Control>& i) { i->allotWidth = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(BeatChartElement, {
    {"control", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i)
        { i->control = FieldPopulator<BeatChartElement::Control>::createAndPopulate(e); }},
    {"dur", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->dur = e->getTextAs<Edu>(); }},
    {"pos", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->pos = e->getTextAs<Evpu>(); }},
    {"endPos", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->endPos = e->getTextAs<Evpu>(); }},
    {"minPos", [](const XmlElementPtr& e, const std::shared_ptr<BeatChartElement>& i) { i->minPos = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(ChordSuffixElement, {
    { "fontID", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    { "fontSize", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    { "efx", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    { "suffix", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->symbol = e->getTextAs<char32_t>(); }},
    { "xdisp", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->xdisp = e->getTextAs<Evpu>(); }},
    { "ydisp", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->ydisp = e->getTextAs<Evpu>(); }},
    { "isNumber", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->isNumber = populateBoolean(e, i); }},
    { "prefix", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixElement>& i) { i->prefix = toEnum<ChordSuffixElement::Prefix>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordSuffixPlayback, {
    {"data", [](const XmlElementPtr& e, const std::shared_ptr<ChordSuffixPlayback>& i) { i->values.push_back(e->getTextAs<int16_t>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefList, {
    {"clef", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefIndex = e->getTextAs<ClefIndex>(); }},
    {"xEduPos", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEduPos = e->getTextAs<Edu>(); }},
    {"yEvpuPos", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->yEvpuPos = e->getTextAs<Evpu>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->percent = e->getTextAs<int>(); }},
    {"xEvpuOffset", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->xEvpuOffset = e->getTextAs<int>(); }},
    {"clefMode", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->clefMode = toEnum<ShowClefMode>(e); }},
    {"unlockVert", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->unlockVert = populateBoolean(e, i); }},
    {"afterBarline", [](const XmlElementPtr& e, const std::shared_ptr<ClefList>& i) { i->afterBarline = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(DrumStaff, {
    {"whichDrumLib", [](const XmlElementPtr& e, const std::shared_ptr<DrumStaff>& i) { i->whichDrumLib = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FileAlias, {
    {"length", [](const XmlElementPtr& e, const std::shared_ptr<FileAlias>& i) { i->length = e->getTextAs<size_t>(); }},
    {"aliasHandle", [](const XmlElementPtr& e, const std::shared_ptr<FileAlias>& i) { i->aliasHandle = hexToBytes(e->getText()); }},
});


MUSX_XML_ELEMENT_ARRAY(FileDescription, {
    {"version", [](const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"volRefNum", [](const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i){ i->volRefNum = e->getTextAs<int>(); }},
    {"dirID", [](const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i){ i->dirId = e->getTextAs<int>(); }},
    {"pathType", [](const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->pathType = toEnum<FileDescription::PathType>(e); }},
    {"pathID", [](const XmlElementPtr& e, const std::shared_ptr<FileDescription>& i) { i->pathId = e->getTextAs<Cmper>(); }},
});


MUSX_XML_ELEMENT_ARRAY(FilePath, {
    {"path", [](const XmlElementPtr& e, const std::shared_ptr<FilePath>& i) { i->path = e->getText(); }},
});


MUSX_XML_ELEMENT_ARRAY(FileUrlBookmark, {
    {"length", [](const XmlElementPtr& e, const std::shared_ptr<FileUrlBookmark>& i) { i->length = e->getTextAs<size_t>(); }},
    {"urlBookmarkData", [](const XmlElementPtr& e, const std::shared_ptr<FileUrlBookmark>& i) { i->urlBookmarkData = hexToBytes(e->getText()); }},
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

MUSX_XML_ELEMENT_ARRAY(FretboardGroup, {
    {"fretInstID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardGroup>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FretboardGroup>& i) { i->name = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretInstrument::StringInfo, {
    {"pitch", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument::StringInfo>& i) { i->pitch = e->getTextAs<int>(); }},
    {"nutOffset", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument::StringInfo>& i) { i->nutOffset = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretInstrument, {
    {"numFrets", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->numFrets = e->getTextAs<int>(); }},
    {"numStrings", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->numStrings = e->getTextAs<int>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->name = e->getText(); }},
    {"string", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i)
        { i->strings.push_back(FieldPopulator<FretInstrument::StringInfo>::createAndPopulate(e)); }},
    {"diatonic", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->fretSteps = populateEmbeddedArray<int>(e, "fret"); }},
    {"speedyClef", [](const XmlElementPtr& e, const std::shared_ptr<FretInstrument>& i) { i->speedyClef = e->getTextAs<ClefIndex>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardStyle, {
    {"showLastFret", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->showLastFret = populateBoolean(e, i); }},
    {"rotate", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->rotate = populateBoolean(e, i); }},
    {"fingNumWhite", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fingNumWhite = populateBoolean(e, i); }},
    {"fingStrShapeID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fingStrShapeId = e->getTextAs<Cmper>(); }},
    {"openStrShapeID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->openStrShapeId = e->getTextAs<Cmper>(); }},
    {"muteStrShapeID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->muteStrShapeId = e->getTextAs<Cmper>(); }},
    {"barreShapeID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->barreShapeId = e->getTextAs<Cmper>(); }},
    {"customShapeID", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->customShapeId = e->getTextAs<Cmper>(); }},
    {"defNumFrets", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->defNumFrets = e->getTextAs<int>(); }},
    {"stringGap", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->stringGap = e->getTextAs<Efix>(); }},
    {"fretGap", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretGap = e->getTextAs<Efix>(); }},
    {"stringWidth", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->stringWidth = e->getTextAs<Efix>(); }},
    {"fretWidth", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretWidth = e->getTextAs<Efix>(); }},
    {"nutWidth", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->nutWidth = e->getTextAs<Efix>(); }},
    {"vertTextOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertTextOff = e->getTextAs<Efix>(); }},
    {"horzTextOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzTextOff = e->getTextAs<Efix>(); }},
    {"horzHandleOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzHandleOff = e->getTextAs<Efix>(); }},
    {"vertHandleOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertHandleOff = e->getTextAs<Efix>(); }},
    {"whiteout", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->whiteout = e->getTextAs<Efix>(); }},
    {"fretNumFont", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i)
        { i->fretNumFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"fingNumFont", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i)
        { i->fingNumFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"horzFingNumOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->horzFingNumOff = e->getTextAs<Efix>(); }},
    {"vertFingNumOff", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->vertFingNumOff = e->getTextAs<Efix>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->name = e->getTextAs<std::string>(); }},
    {"fretNumText", [](const XmlElementPtr& e, const std::shared_ptr<FretboardStyle>& i) { i->fretNumText = e->getTextAs<std::string>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyFormat, {
    {"semitones", [](const XmlElementPtr& e, const std::shared_ptr<KeyFormat>& i) { i->semitones = e->getTextAs<unsigned>(); }},
    {"scaleTones", [](const XmlElementPtr& e, const std::shared_ptr<KeyFormat>& i) { i->scaleTones = e->getTextAs<unsigned>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyMapArray::StepElement, {
    {"diatonic", [](const XmlElementPtr& e, const std::shared_ptr<KeyMapArray::StepElement>& i) { i->diatonic = populateBoolean(e, i); }},
    {"hlevel", [](const XmlElementPtr& e, const std::shared_ptr<KeyMapArray::StepElement>& i) { i->hlevel = e->getTextAs<unsigned>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyMapArray, {
    {"keych", [](const XmlElementPtr& e, const std::shared_ptr<KeyMapArray>& i) { i->steps.push_back(FieldPopulator<KeyMapArray::StepElement>::createAndPopulate(e)); }},
});

MUSX_XML_ELEMENT_ARRAY(KeyAttributes, {
    {"harmRefer", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->harmRefer = e->getTextAs<int>(); }},
    {"middleCKey", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->middleCKey = e->getTextAs<int>(); }},
    {"fontSym", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->fontSym = e->getTextAs<Cmper>(); }},
    {"gotoKey", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->gotoKey = e->getTextAs<int>(); }},
    {"symbolList", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->symbolList = e->getTextAs<Cmper>(); }},
    {"hasClefOctv", [](const XmlElementPtr& e, const std::shared_ptr<KeyAttributes>& i) { i->hasClefOctv = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LayerAttributes, {
    {"restOffset", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->restOffset = e->getTextAs<int>(); }},
    {"flipTies", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezTiesToStems = populateBoolean(e, i); }},
    {"floatLayer", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->onlyIfOtherLayersHaveNotes = populateBoolean(e, i); }},
    {"useRestOff", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->useRestOffset = populateBoolean(e, i); }},
    {"freezeLayUp", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezeStemsUp = populateBoolean(e, i); }},
    {"freezeLayer", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->freezeLayer = populateBoolean(e, i); }},
    {"playback", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->playback = populateBoolean(e, i); }},
    {"spacing", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->affectSpacing = populateBoolean(e, i); }},
    {"ignoreHidden", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenNotesOnly = populateBoolean(e, i); }},
    {"ignoreHiddenLayers", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenLayers = populateBoolean(e, i); }},
    {"hideLayer", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->hideLayer = populateBoolean(e, i); }},
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
    {"usesTextFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesTextFont = populateBoolean(e, i); }},
    {"usesMusicFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesMusicFont = populateBoolean(e, i); }},
    {"usesNumberFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesNumberFont = populateBoolean(e, i); }},
    {"usesPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesPositioning = populateBoolean(e, i); }},
    {"usesStaffList", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesStaffList = populateBoolean(e, i); }},
    {"usesBreakMmRests", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->usesBreakMmRests = populateBoolean(e, i); }},
    {"breakMmRest", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->breakMmRest = populateBoolean(e, i); }},
    {"userCreated", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->userCreated = populateBoolean(e, i); }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->staffList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Measure, {
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"keySig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) {
        i->globalKeySig = FieldPopulator<KeySignature>::createAndPopulate(e, i->getDocument()); }},
    {"beats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivbeat", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->dispDivbeat = e->getTextAs<Cmper>(); }},
    {"customBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customBarShape = e->getTextAs<Cmper>(); }},
    {"customLeftBarShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->customLeftBarShape = e->getTextAs<Cmper>(); }},
    {"frontSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->frontSpaceExtra = e->getTextAs<Evpu>(); }},
    {"backSpaceExtra", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backSpaceExtra = e->getTextAs<Evpu>(); }},
    {"breakWordExt", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->breakWordExt = populateBoolean(e, i); }},
    {"hideCaution", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hideCaution = populateBoolean(e, i); }},
    {"hasSmartShape", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasSmartShape = populateBoolean(e, i); }},
    {"groupBarlineOverride", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->groupBarlineOverride = populateBoolean(e, i); }},
    {"showFullNames", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showFullNames = populateBoolean(e, i); }},
    {"mnSepPlace", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasMeasNumbIndivPos = populateBoolean(e, i); }},
    {"posSplit", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->allowSplitPoints = populateBoolean(e, i); }},
    {"altNumTsig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeNumerator = populateBoolean(e, i); }},
    {"altDenTsig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDenominator = populateBoolean(e, i); }},
    {"showKey", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showKey = toEnum<Measure::ShowKeySigMode>(e); }},
    {"showTime", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->showTime = toEnum<Measure::ShowTimeSigMode>(e); }},
    {"indivPosDef", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->evenlyAcrossMeasure = populateBoolean(e, i); }},
    {"posMode", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->positioningMode = toEnum<Measure::PositioningType>(e); }},
    {"lineBreak", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->beginNewSystem = populateBoolean(e, i); }},
    {"hasExpr", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasExpression = populateBoolean(e, i); }},
    {"breakRest", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->breakMmRest = populateBoolean(e, i); }},
    {"noMeasNum", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->noMeasNum = populateBoolean(e, i); }},
    {"arbitMusic", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasOssia = populateBoolean(e, i); }},
    {"hasTextBlock", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasTextBlock = populateBoolean(e, i); }},
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->barlineType = toEnum<Measure::BarlineType>(e); }},
    {"forRepBar", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->forwardRepeatBar = populateBoolean(e, i); }},
    {"bacRepBar", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->backwardsRepeatBar = populateBoolean(e, i); }},
    {"barEnding", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasEnding = populateBoolean(e, i); }},
    {"txtRepeats", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasTextRepeat = populateBoolean(e, i); }},
    {"abbrvTime", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->abbrvTime = populateBoolean(e, i); }},
    {"useDisplayTimesig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->useDisplayTimesig = populateBoolean(e, i); }},
    {"hasChord", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->hasChord = populateBoolean(e, i); }},
    {"leftBarline", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->leftBarlineType = toEnum<Measure::BarlineType>(e); }},
    {"displayAltNumTsig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDispNumerator = populateBoolean(e, i); }},
    {"displayAltDenTsig", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->compositeDispDenominator = populateBoolean(e, i); }},
    {"pageBreak", [](const XmlElementPtr& e, const std::shared_ptr<Measure>& i) { i->pageBreak = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureExprAssign, {
    {"textExprID", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->textExprId = e->getTextAs<Cmper>(); }},
    {"shapeExprID", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->shapeExprId = e->getTextAs<Cmper>(); }},
    {"horzEvpuOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->horzEvpuOff = e->getTextAs<Evpu>(); }},
    {"horzEduOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->eduPosition = e->getTextAs<Edu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->vertEvpuOff = e->getTextAs<Evpu>(); }},
    {"staffAssign", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffAssign = e->getTextAs<StaffCmper>(); }},
    {"layer", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->layer = e->getTextAs<int>(); }},
    {"channelSwitch", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->channelSwitch = toEnum<MeasureExprAssign::ChannelSwitchTarget>(e); }},
    {"dontScaleWithEntry", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->dontScaleWithEntry = populateBoolean(e, i); }},
    {"playbackStart", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->playbackStart = toEnum<MeasureExprAssign::PlaybackStart>(e); }},
    {"showStaffList", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->showStaffList = toEnum<MeasureExprAssign::ShowStaffList>(e); }},
    {"createdByHp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->createdByHp = populateBoolean(e, i); }},
    {"hidden", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->hidden = populateBoolean(e, i); }},
    {"staffGroup", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffGroup = e->getTextAs<int>(); }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
    {"graceNoteIndex", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->graceNoteIndex = e->getTextAs<int>(); }},
    {"rehearsalMarkOffset", [](const XmlElementPtr& e, const std::shared_ptr<MeasureExprAssign>& i) { i->rehearsalMarkOffset = e->getTextAs<int>(); }},
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
    {"startOfLine", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnStart = populateBoolean(e, i); }},
    {"multipleOf", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnEvery = populateBoolean(e, i); }},
    {"exceptFirstMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->hideFirstMeasure = populateBoolean(e, i); }},
    {"mmRestRange", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showMmRange = populateBoolean(e, i); }},
    {"mmRestRangeForce", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnMmRest = populateBoolean(e, i); }},
    {"useStartEncl", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useStartEncl = populateBoolean(e, i); }},
    {"useMultipleEncl", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->useMultipleEncl = populateBoolean(e, i); }},
    {"showOnTop", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnTop = populateBoolean(e, i); }},
    {"showOnBottom", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->showOnBottom = populateBoolean(e, i); }},
    {"excludeOthers", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->excludeOthers = populateBoolean(e, i); }},
    {"breakMmRest", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->breakMmRest = populateBoolean(e, i); }},
    {"startJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->startJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"multipleJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->multipleJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }},
    {"mmRestJustify", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion::ScorePartData>& i) { i->mmRestJustify = toEnum<MeasureNumberRegion::AlignJustify>(e); }}
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberRegion, {
    {"scoreData", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->scoreData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(e, i); }},
    {"partData", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->partData = FieldPopulator<MeasureNumberRegion::ScorePartData>::createAndPopulate(e, i); }},
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"startChar", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->startChar = e->getTextAs<char32_t>(); }},
    {"base", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->base = e->getTextAs<int>(); }},
    {"offset", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->numberOffset = e->getTextAs<int>(); }},
    {"prefix", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->prefix = e->getTextTrimmed(); }},
    {"suffix", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->suffix = e->getTextTrimmed(); }},
    {"countFromOne", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->countFromOne = populateBoolean(e, i); }},
    {"noZero", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->noZero = populateBoolean(e, i); }},
    {"doubleUp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->doubleUp = populateBoolean(e, i); }},
    {"time", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->time = populateBoolean(e, i); }},
    {"includeHours", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->includeHours = populateBoolean(e, i); }},
    {"smpteFrames", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->smpteFrames = populateBoolean(e, i); }},
    {"useScoreInfoForPart", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->useScoreInfoForPart = populateBoolean(e, i); }},
    {"region", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->region = e->getTextAs<int>(); }},
    {"timePrecision", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->timePrecision = toEnum<MeasureNumberRegion::TimePrecision>(e); }},
    {"hideScroll", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->hideScroll = populateBoolean(e, i); }},
    {"hidePage", [](const XmlElementPtr& e, const std::shared_ptr<MeasureNumberRegion>& i) { i->hidePage = populateBoolean(e, i); }}
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
    {"useCharRestStyle", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRest>& i) { i->useSymbols = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MultiStaffGroupId, {
    {"staffGroupID", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffGroupId>& i) { i->staffGroupId = e->getTextAs<Cmper>(); }},
});

// NOTE: zero values are not exported in the xml, so this mapping only pushes non-zero values to the vector of staffNums.
MUSX_XML_ELEMENT_ARRAY(MultiStaffInstrumentGroup, {
    {"staffNum1", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
    {"staffNum2", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
    {"staffNum3", [](const XmlElementPtr& e, const std::shared_ptr<MultiStaffInstrumentGroup>& i) { i->staffNums.push_back(e->getTextAs<StaffCmper>()); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaBounds, {
    {"topAdd", [](const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->topMargin = e->getTextAs<Evpu>(); }},
    {"leftAdd", [](const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->leftMargin = e->getTextAs<Evpu>(); }},
    {"rightAdd", [](const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->rightMargin = e->getTextAs<Evpu>(); }},
    {"centerPull", [](const XmlElementPtr& e, const std::shared_ptr<OssiaBounds>& i) { i->centerPull = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaHeader, {
    {"clef", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->clefId = e->getTextAs<ClefIndex>(); }},
    {"keySig", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) {
        i->keySig = FieldPopulator<KeySignature>::createAndPopulate(e, i->getDocument()); }},
    {"beats", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divbeat", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"group", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->bracketGroup = e->getTextAs<Cmper>(); }},
    {"altNumTsig", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->compositeNumerator = populateBoolean(e, i); }},
    {"altDenTsig", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->compositeDenominator = populateBoolean(e, i); }},
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<OssiaHeader>& i) { i->barlineType = toEnum<Measure::BarlineType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(OssiaMusic, {
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->percent = e->getTextAs<int>(); }},
    {"qinst", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->sourceStaffId = e->getTextAs<StaffCmper>(); }},
    {"qmeasure", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->sourceMeasureId = e->getTextAs<MeasCmper>(); }},
    {"negLeftLine", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideLeftLine = populateBoolean(e, i); }},
    {"negKey", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideKey = populateBoolean(e, i); }},
    {"negTime", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideTime = populateBoolean(e, i); }},
    {"negDynamic", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideDynamic = populateBoolean(e, i); }},
    {"negStaff", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideStaff = populateBoolean(e, i); }},
    {"negRepeat", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideRepeat = populateBoolean(e, i); }},
    {"negClef", [](const XmlElementPtr& e, const std::shared_ptr<OssiaMusic>& i) { i->hideClef = populateBoolean(e, i); }},
    {"posMode", [](const XmlElementPtr&, const std::shared_ptr<OssiaMusic>&) { /* no-op: intentionally suppressed */ }},
});

MUSX_XML_ELEMENT_ARRAY(Page, {
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->percent = e->getTextAs<int>(); }},
    {"firstSystem", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->firstSystemId = e->getTextAs<SystemCmper>(); }},
    {"scaleContentOnly", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->holdMargins = populateBoolean(e, i); }},
    {"margTop", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margTop = e->getTextAs<Evpu>(); }},
    {"margLeft", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margLeft = e->getTextAs<Evpu>(); }},
    {"margBottom", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margBottom = e->getTextAs<Evpu>(); }},
    {"margRight", [](const XmlElementPtr& e, const std::shared_ptr<Page>& i) { i->margRight = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PageGraphicAssign, {
    {"version", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"left", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"bottom", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"fDescID", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->fDescId = e->getTextAs<Cmper>(); }},
    {"displayType", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->displayType = toEnum<PageGraphicAssign::PageAssignType>(e); }},
    {"displayHidden", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->hidden = populateBoolean(e, i); }},
    {"halign", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->hAlign = toEnum<PageGraphicAssign::HorizontalAlignment>(e); }},
    {"valign", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->vAlign = toEnum<PageGraphicAssign::VerticalAlignment>(e); }},
    {"posFrom", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->posFrom = toEnum<PageGraphicAssign::PositionFrom>(e); }},
    {"fixedPerc", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->fixedPerc = populateBoolean(e, i); }},
    {"startPage", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->startPage = e->getTextAs<PageCmper>(); }},
    {"endPage", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->endPage = e->getTextAs<PageCmper>(); }},
    {"savedRecord", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->savedRecord = populateBoolean(e, i); }},
    {"origWidth", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->origWidth = e->getTextAs<Evpu>(); }},
    {"origHeight", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->origHeight = e->getTextAs<Evpu>(); }},
    {"rightPgHAlign", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgHAlign = toEnum<PageGraphicAssign::HorizontalAlignment>(e); }},
    {"rightPgVAlign", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgVAlign = toEnum<PageGraphicAssign::VerticalAlignment>(e); }},
    {"rightPgPosFrom", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgPosFrom = toEnum<PageGraphicAssign::PositionFrom>(e); }},
    {"rightPgFixedPerc", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgFixedPerc = populateBoolean(e, i); }},
    {"rightPgLeft", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgLeft = e->getTextAs<Evpu>(); }},
    {"rightPgBottom", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->rightPgBottom = e->getTextAs<Evpu>(); }},
    {"graphicCmper", [](const XmlElementPtr& e, const std::shared_ptr<PageGraphicAssign>& i) { i->graphicCmper = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PageOssiaAssign, {
    {"arbnum", [](const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->ossiaId = e->getTextAs<Cmper>(); }},
    {"topAdd", [](const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"leftAdd", [](const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
    {"mwidth", [](const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->measureWidth = e->getTextAs<Evpu>(); }},
    {"hidden", [](const XmlElementPtr& e, const std::shared_ptr<PageOssiaAssign>& i) { i->hidden = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(PageTextAssign, {
    {"block", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->block = e->getTextAs<Cmper>(); }},
    {"xdisp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->xDisp = e->getTextAs<Evpu>(); }},
    {"ydisp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->yDisp = e->getTextAs<Evpu>(); }},
    {"startPage", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->startPage = e->getTextAs<PageCmper>(); }},
    {"endPage", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->endPage = e->getTextAs<PageCmper>(); }},
    {"oddEven", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->oddEven = toEnum<PageTextAssign::PageAssignType>(e); }},
    {"hposLp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosLp = toEnum<PageTextAssign::HorizontalAlignment>(e); }},
    {"hposRp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosRp = toEnum<PageTextAssign::HorizontalAlignment>(e); }},
    {"postIt", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hidden = populateBoolean(e, i); }},
    {"vpos", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->vPos = toEnum<PageTextAssign::VerticalAlignment>(e); }},
    {"hposPageEdge", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->hPosPageEdge = populateBoolean(e, i); }},
    {"vposPageEdge", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->vPosPageEdge = populateBoolean(e, i); }},
    {"indRpPos", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->indRpPos = populateBoolean(e, i); }},
    {"rightPgXdisp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->rightPgXDisp = e->getTextAs<Evpu>(); }},
    {"rightPgYdisp", [](const XmlElementPtr& e, const std::shared_ptr<PageTextAssign>& i) { i->rightPgYDisp = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartDefinition, {
    {"nameID", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->nameId = e->getTextAs<Cmper>(); }},
    {"partOrder", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->partOrder = e->getTextAs<int>(); }},
    {"copies", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->copies = e->getTextAs<int>(); }},
    {"printPart", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->printPart = populateBoolean(e, i); }},
    {"extractPart", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->extractPart = populateBoolean(e, i); }},
    {"applyFormat", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->applyFormat = populateBoolean(e, i); }},
    {"needsRecalc", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->needsRecalc = populateBoolean(e, i); }},
    {"useAsSmpInst", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->useAsSmpInst = populateBoolean(e, i); }},
    {"smartMusicInst", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->smartMusicInst = e->getTextAs<int>(); }},
    {"defaultNameStaff", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameStaff = e->getTextAs<Cmper>(); }},
    {"defaultNameGroup", [](const XmlElementPtr& e, const std::shared_ptr<PartDefinition>& i) { i->defaultNameGroup = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartGlobals, {
    {"showTransposed", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = populateBoolean(e, i); }},
    {"scrollViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PartVoicing, {
    {"enabled", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->enabled = populateBoolean(e, i); }},
    {"voicingType", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->voicingType = toEnum<PartVoicing::VoicingType>(e); }},
    {"singleVoiceType", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->singleLayerVoiceType = toEnum<PartVoicing::SingleLayerVoiceType>(e); }},
    {"select1st", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select1st = populateBoolean(e, i); }},
    {"select2nd", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select2nd = populateBoolean(e, i); }},
    {"select3rd", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select3rd = populateBoolean(e, i); }},
    {"select4th", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select4th = populateBoolean(e, i); }},
    {"select5th", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->select5th = populateBoolean(e, i); }},
    {"selectFromBottom", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->selectFromBottom = populateBoolean(e, i); }},
    {"selectSingleNote", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->selectSingleNote = populateBoolean(e, i); }},
    {"singleLayer", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->singleLayer = e->getTextAs<LayerIndex>(); }},
    {"multiLayer", [](const XmlElementPtr& e, const std::shared_ptr<PartVoicing>& i) { i->multiLayer = e->getTextAs<LayerIndex>(); }},
});

MUSX_XML_ELEMENT_ARRAY(PercussionNoteInfo, {
    {"percNoteType",    [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->percNoteType = e->getTextAs<PercussionNoteTypeId>(); }},
    {"harmLev",         [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->staffPosition = e->getTextAs<int>(); }},
    {"closedNotehead",  [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->closedNotehead = e->getTextAs<char32_t>(); }},
    {"halfNotehead",    [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->halfNotehead = e->getTextAs<char32_t>(); }},
    {"wholeNotehead",   [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->wholeNotehead = e->getTextAs<char32_t>(); }},
    {"dwholeNotehead",  [](const XmlElementPtr& e, const std::shared_ptr<PercussionNoteInfo>& i) { i->dwholeNotehead = e->getTextAs<char32_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatBack, {
    {"actuate", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"pos1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftHPos = e->getTextAs<Evpu>(); }},
    {"line1", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->leftVPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->individualPlacement = populateBoolean(e, i); }},
    {"topStaffOnly", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->topStaffOnly = populateBoolean(e, i); }},
    {"clrOnChange", [](const XmlElementPtr& e, const std::shared_ptr<RepeatBack>& i) { i->resetOnAction = populateBoolean(e, i); }},
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
    {"indivPlac", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->individualPlacement = populateBoolean(e, i); }},
    {"topStaffOnly", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->topStaffOnly = populateBoolean(e, i); }},
    {"action", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"trigger", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](const XmlElementPtr& e, const std::shared_ptr<RepeatEndingStart>& i) { i->jumpIfIgnoring = populateBoolean(e, i); }},
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

MUSX_XML_ELEMENT_ARRAY(ShapeData, {
    {"data", [](const XmlElementPtr& e, const std::shared_ptr<ShapeData>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeDef, {
    {"instList", [](const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->instructionList = e->getTextAs<Cmper>(); }},
    {"dataList", [](const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->dataList = e->getTextAs<Cmper>(); }},
    {"shapeType", [](const XmlElementPtr& e, const std::shared_ptr<ShapeDef>& i) { i->shapeType = toEnum<ShapeType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeExpressionDef, {
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"execShape", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->execShape = e->getTextAs<Cmper>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"breakMmRest", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->breakMmRest = populateBoolean(e, i); }},
    {"useAuxData", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->useAuxData = populateBoolean(e, i); }},
    {"masterShape", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->masterShape = populateBoolean(e, i); }},
    {"noPrint", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->noPrint = populateBoolean(e, i); }},
    {"noHorzStretch", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->noHorzStretch = populateBoolean(e, i); }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalTextJustification>(e); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryPos", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->useCategoryPos = populateBoolean(e, i); }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<ShapeExpressionDef>& i) { i->description = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeInstructionList::InstructionInfo, {
    {"numData", [](const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList::InstructionInfo>& i) { i->numData = e->getTextAs<int>(); }},
    {"tag", [](const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList::InstructionInfo>& i) { i->type = toEnum<ShapeDef::InstructionType, true>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeInstructionList, {
    {"instruct", [](const XmlElementPtr& e, const std::shared_ptr<ShapeInstructionList>& i) {
        i->instructions.push_back(FieldPopulator<ShapeInstructionList::InstructionInfo>::createAndPopulate(e));
    }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape::TerminationSeg, {
    {"endPt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPoint = FieldPopulator<smartshape::EndPoint>::createAndPopulate(e, i->getParent()); }},
    {"endPtAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->endPointAdj = FieldPopulator<smartshape::EndPointAdjustment>::createAndPopulate(e, i->getParent()); }},
    {"ctlPtAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->ctlPtAdj = FieldPopulator<smartshape::ControlPointAdjustment>::createAndPopulate(e, i->getParent()); }},
    {"breakAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape::TerminationSeg>& i)
        { i->breakAdj = FieldPopulator<smartshape::EndPointAdjustment>::createAndPopulate(e, i->getParent()); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShape, {
    {"shapeType", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->shapeType = toEnum<SmartShape::ShapeType>(e); }},
    {"entryBased", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->entryBased = populateBoolean(e, i); }},
    {"rotate", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->rotate = populateBoolean(e, i); }},
    {"noPresetShape", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->noPresetShape = populateBoolean(e, i); }},
    {"makeHorz", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->makeHorz = populateBoolean(e, i); }},
    {"noPushEndStart", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->noPushEndStart = populateBoolean(e, i); }},
    {"makeVert", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->makeVert = populateBoolean(e, i); }},
    {"engraverSlurState", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->engraverSlurState = toEnum<SmartShape::EngraverSlurState>(e); }},
    {"slurAvoidAcciState", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->slurAvoidAcciState = toEnum<SmartShape::SlurAvoidAccidentalsState>(e); }},
    {"yBreakType", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->yBreakType = toEnum<SmartShape::SystemBreakType>(e); }},
    {"startTermSeg", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->startTermSeg = FieldPopulator<SmartShape::TerminationSeg>::createAndPopulate(e, i); }},
    {"endTermSeg", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->endTermSeg = FieldPopulator<SmartShape::TerminationSeg>::createAndPopulate(e, i); }},
    {"fullCtlPtAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i)
        { i->fullCtlPtAdj = FieldPopulator<smartshape::ControlPointAdjustment>::createAndPopulate(e, i); }},
    {"hidden", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->hidden = populateBoolean(e, i); }},
    {"startNoteID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startNoteId= e->getTextAs<NoteNumber>(); }},
    {"endNoteID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endNoteId = e->getTextAs<NoteNumber>(); }},
    {"lineStyleID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->lineStyleId = e->getTextAs<Cmper>(); }},
    {"startLyricNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startLyricNum = e->getTextAs<Cmper>(); }},
    {"endLyricNum",   [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endLyricNum = e->getTextAs<Cmper>(); }},
    {"startLyricTag", [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->startLyricType = toEnum<SmartShape::LyricTextType>(e); }},
    {"endLyricTag",   [](const XmlElementPtr& e, const std::shared_ptr<SmartShape>& i) { i->endLyricType = toEnum<SmartShape::LyricTextType>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::CharParams, {
    {"lineChar", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { i->lineChar = e->getTextAs<char32_t>(); }},
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"baselineShiftEms", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::CharParams>& i) { i->baselineShiftEms = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::SolidParams, {
    {"lineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::SolidParams>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine::DashedParams, {
    {"lineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
    {"dashOn", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->dashOn = e->getTextAs<Efix>(); }},
    {"dashOff", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine::DashedParams>& i) { i->dashOff = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeCustomLine, {
    {"lineStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStyle = toEnum<SmartShapeCustomLine::LineStyle>(e); }},
    {"charParams", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->charParams = FieldPopulator<SmartShapeCustomLine::CharParams>::createAndPopulate(e, i); }},
    {"solidParams", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->solidParams = FieldPopulator<SmartShapeCustomLine::SolidParams>::createAndPopulate(e, i); }},
    {"dashedParams", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i)
        { i->dashedParams = FieldPopulator<SmartShapeCustomLine::DashedParams>::createAndPopulate(e, i); }},
    {"lineCapStartType", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartType = toEnum<SmartShapeCustomLine::LineCapType>(e); }},
    {"lineCapEndType", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndType = toEnum<SmartShapeCustomLine::LineCapType>(e); }},
    {"lineCapStartArrowID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartArrowId = e->getTextAs<Cmper>(); }},
    {"lineCapEndArrowID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndArrowId = e->getTextAs<Cmper>(); }},
    {"makeHorz", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->makeHorz = populateBoolean(e, i); }},
    {"lineAfterLeftStartText", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineAfterLeftStartText = populateBoolean(e, i); }},
    {"lineBeforeRightEndText", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineBeforeRightEndText = populateBoolean(e, i); }},
    {"lineAfterLeftContText", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineAfterLeftContText = populateBoolean(e, i); }},
    {"leftStartRawTextID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartRawTextId = e->getTextAs<Cmper>(); }},
    {"leftContRawTextID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContRawTextId = e->getTextAs<Cmper>(); }},
    {"rightEndRawTextID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndRawTextId = e->getTextAs<Cmper>(); }},
    {"centerFullRawTextID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullRawTextId = e->getTextAs<Cmper>(); }},
    {"centerAbbrRawTextID", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrRawTextId = e->getTextAs<Cmper>(); }},
    {"leftStartX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartX = e->getTextAs<Evpu>(); }},
    {"leftStartY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftStartY = e->getTextAs<Evpu>(); }},
    {"leftContX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContX = e->getTextAs<Evpu>(); }},
    {"leftContY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->leftContY = e->getTextAs<Evpu>(); }},
    {"rightEndX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndX = e->getTextAs<Evpu>(); }},
    {"rightEndY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->rightEndY = e->getTextAs<Evpu>(); }},
    {"centerFullX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullX = e->getTextAs<Evpu>(); }},
    {"centerFullY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerFullY = e->getTextAs<Evpu>(); }},
    {"centerAbbrX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrX = e->getTextAs<Evpu>(); }},
    {"centerAbbrY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->centerAbbrY = e->getTextAs<Evpu>(); }},
    {"lineStartX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStartX = e->getTextAs<Evpu>(); }},
    {"lineStartY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineStartY = e->getTextAs<Evpu>(); }},
    {"lineEndX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineEndX = e->getTextAs<Evpu>(); }},
    {"lineEndY", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineEndY = e->getTextAs<Evpu>(); }},
    {"lineContX", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineContX = e->getTextAs<Evpu>(); }},
    {"lineCapStartHookLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapStartHookLength = e->getTextAs<Efix>(); }},
    {"lineCapEndHookLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeCustomLine>& i) { i->lineCapEndHookLength = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeMeasureAssign, {
    {"shapeNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->shapeNum = e->getTextAs<Cmper>(); }},
    {"centerShapeNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeMeasureAssign>& i) { i->centerShapeNum = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SplitMeasure, {
    {"data", [](const XmlElementPtr& e, const std::shared_ptr<SplitMeasure>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::KeySigTransposition, {
    {"interval", [](const XmlElementPtr& e, const std::shared_ptr<Staff::KeySigTransposition>& i) { i->interval = e->getTextAs<int>(); }},
    {"adjust", [](const XmlElementPtr& e, const std::shared_ptr<Staff::KeySigTransposition>& i) { i->adjust = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::ChromaticTransposition, {
    {"alteration", [](const XmlElementPtr& e, const std::shared_ptr<Staff::ChromaticTransposition>& i) { i->alteration = e->getTextAs<int>(); }},
    {"diatonic", [](const XmlElementPtr& e, const std::shared_ptr<Staff::ChromaticTransposition>& i) { i->diatonic = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff::Transposition, {
    {"setToClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i) { i->setToClef = populateBoolean(e, i); }},
    {"noKeyOpt", [](const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i) { i->noSimplifyKey = populateBoolean(e, i); }},
    {"keysig", [](const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i)
        { i->keysig = FieldPopulator<Staff::KeySigTransposition>::createAndPopulate(e); }},
    {"chromatic", [](const XmlElementPtr& e, const std::shared_ptr<Staff::Transposition>& i)
        { i->chromatic = FieldPopulator<Staff::ChromaticTransposition>::createAndPopulate(e); }},
});

MUSX_XML_ELEMENT_ARRAY(Staff, {
    {"notationStyle", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->notationStyle = toEnum<Staff::NotationStyle>(e); }},
    {"noteFont", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i)
        { i->noteFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"useNoteShapes", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useNoteShapes = populateBoolean(e, i); }},
    {"useNoteFont", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useNoteFont = populateBoolean(e, i); }},
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"transposedClef", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->transposedClef = e->getTextAs<ClefIndex>(); }},
    {"staffLines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->staffLines = e->getTextAs<int>(); }},
    {"customStaff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->customStaff = populateEmbeddedArray<int>(e, "staffLine"); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lineSpace = e->getTextAs<Evpu>(); }},
    {"instUuid", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) {
        auto s = e->getTextTrimmed();
        std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); });
        i->instUuid = std::move(s);
    }},
    {"capoPos", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->capoPos = e->getTextAs<int>(); }},
    {"lowestFret", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->lowestFret = e->getTextAs<int>(); }},
    {"floatKeys", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->floatKeys = populateBoolean(e, i); }},
    {"floatTime", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->floatTime = populateBoolean(e, i); }},
    {"blineBreak", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->blineBreak = populateBoolean(e, i); }},
    {"rbarBreak", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->rbarBreak = populateBoolean(e, i); }},
    {"hasStyles", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hasStyles = populateBoolean(e, i); }},
    {"showNameParts", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showNameInParts = populateBoolean(e, i); }},
    {"showNoteColors", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showNoteColors = populateBoolean(e, i); }},
    {"transposition", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i)
        { i->transposition = FieldPopulator<Staff::Transposition>::createAndPopulate(e); }},
    {"hideStfNameInScore", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideNameInScore = populateBoolean(e, i); }},
    {"botBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botBarlineOffset = e->getTextAs<Evpu>(); }},
    {"altNotation", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altNotation = toEnum<Staff::AlternateNotation>(e); }},
    {"altLayer", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altLayer = e->getTextAs<LayerIndex>(); }},
    {"altHideArtics", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideArtics = populateBoolean(e, i); }},
    {"altHideLyrics", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideLyrics = populateBoolean(e, i); }},
    {"altHideSmartShapes", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideSmartShapes = populateBoolean(e, i); }},
    {"altRhythmStemsUp", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altRhythmStemsUp = populateBoolean(e, i); }},
    {"altSlashDots", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altSlashDots = populateBoolean(e, i); }},
    {"altHideOtherNotes", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherNotes = populateBoolean(e, i); }},
    {"altHideOtherArtics", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherArtics = populateBoolean(e, i); }},
    {"altHideExpressions", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideExpressions = populateBoolean(e, i); }},
    {"altHideOtherLyrics", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherLyrics = populateBoolean(e, i); }},
    {"altHideOtherSmartShapes", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherSmartShapes = populateBoolean(e, i); }},
    {"altHideOtherExpressions", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->altHideOtherExpressions = populateBoolean(e, i); }},
    {"hideRepeatBottomDot", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeatBottomDot = populateBoolean(e, i); }},
    {"flatBeams", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->flatBeams = populateBoolean(e, i); }},
    {"hideFretboards", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideFretboards = populateBoolean(e, i); }},
    {"blankMeasure", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->blankMeasure = populateBoolean(e, i); }},
    {"hideRepeatTopDot", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeatTopDot = populateBoolean(e, i); }},
    {"hideLyrics", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideLyrics = populateBoolean(e, i); }},
    {"noOptimize", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->noOptimize = populateBoolean(e, i); }},
    {"topBarlineOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topBarlineOffset = e->getTextAs<Evpu>(); }},
    {"hideMeasNums", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideMeasNums = populateBoolean(e, i); }},
    {"hideRepeats", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRepeats = populateBoolean(e, i); }},
    {"hideBarlines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideBarlines = populateBoolean(e, i); }},
    {"hideRptBars", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRptBars = populateBoolean(e, i); }},
    {"hideKeySigs", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideKeySigs = populateBoolean(e, i); }},
    {"hideTimeSigs", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTimeSigs = populateBoolean(e, i); }},
    {"hideClefs", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideClefs = populateBoolean(e, i); }},
    {"hideStaffLines", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideStaffLines = populateBoolean(e, i); }},
    {"hideChords", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideChords = populateBoolean(e, i); }},
    {"noKey", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->noKey = populateBoolean(e, i); }},
    {"dwRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->dwRestOffset = e->getTextAs<Evpu>(); }},
    {"wRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->wRestOffset = e->getTextAs<Evpu>(); }},
    {"hRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hRestOffset = e->getTextAs<Evpu>(); }},
    {"otherRestOffset", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->otherRestOffset = e->getTextAs<Evpu>(); }},
    {"hideRests", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideRests = populateBoolean(e, i); }},
    {"hideTies", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTies = populateBoolean(e, i); }},
    {"hideDots", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideDots = populateBoolean(e, i); }},
    {"stemReversal", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemReversal = e->getTextAs<int>(); }},
    {"fullName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fullNameTextId = e->getTextAs<Cmper>(); }},
    {"abbrvName", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->abbrvNameTextId = e->getTextAs<Cmper>(); }},
    {"botRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->botRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"topRepeatDotOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->topRepeatDotOff = e->getTextAs<Evpu>(); }},
    {"vertTabNumOff", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->vertTabNumOff = e->getTextAs<Efix>(); }},
    {"showTabClefAllSys", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->showTabClefAllSys = populateBoolean(e, i); }},
    {"useTabLetters", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useTabLetters = populateBoolean(e, i); }},
    {"breakTabLinesAtNotes", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->breakTabLinesAtNotes = populateBoolean(e, i); }},
    {"hideTuplets", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTuplets = populateBoolean(e, i); }},
    {"fretInstID", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"hideStems", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideStems = populateBoolean(e, i); }},
    {"stemDir", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->stemDirection = toEnum<Staff::StemDirection>(e); }},
    {"hideBeams", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideBeams = populateBoolean(e, i); }},
    {"stemStartFromStaff",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemStartFromStaff = populateBoolean(e, i); }},
    {"stemsFixedEnd",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemsFixedEnd = populateBoolean(e, i); }},
    {"stemsFixedStart",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->stemsFixedStart = populateBoolean(e, i); }},
    {"horzStemOffUp",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->horzStemOffUp = e->getTextAs<Efix>(); }},
    {"horzStemOffDown",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->horzStemOffDown = e->getTextAs<Efix>(); }},
    {"vertStemStartOffUp",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemStartOffUp = e->getTextAs<Efix>(); }},
    {"vertStemStartOffDown",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemStartOffDown = e->getTextAs<Efix>(); }},
    {"vertStemEndOffUp",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemEndOffUp = e->getTextAs<Efix>(); }},
    {"vertStemEndOffDown",[] (const XmlElementPtr& e, const std::shared_ptr<Staff>& i){ i->vertStemEndOffDown = e->getTextAs<Efix>(); }},
    {"hideMode", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideMode = toEnum<Staff::HideMode>(e); }},
    {"redisplayLayerAccis", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->redisplayLayerAccis = populateBoolean(e, i); }},
    {"hideTimeSigsInParts", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideTimeSigsInParts = populateBoolean(e, i); }},
    {"autoNum", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->autoNumbering = toEnum<Staff::AutoNumberingStyle>(e); }},
    {"useAutoNum", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->useAutoNumbering = populateBoolean(e, i); }},
    {"hideKeySigsShowAccis", [](const XmlElementPtr& e, const std::shared_ptr<Staff>& i) { i->hideKeySigsShowAccis = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffList, {
    {"inst", [](const XmlElementPtr& e, const std::shared_ptr<StaffList>& i) { i->values.push_back(e->getTextAs<StaffCmper>()); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle::Masks, {
    {"floatNoteheadFont", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatNoteheadFont = populateBoolean(e, i); }},
    {"useNoteShapes", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->useNoteShapes = populateBoolean(e, i); }},
    {"flatBeams", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->flatBeams = populateBoolean(e, i); }},
    {"blankMeasureRest", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->blankMeasureRest = populateBoolean(e, i); }},
    {"noOptimize", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->noOptimize = populateBoolean(e, i); }},
    {"notationStyle", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->notationStyle = populateBoolean(e, i); }},
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->defaultClef = populateBoolean(e, i); }},
    {"staffType", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->staffType = populateBoolean(e, i); }},
    {"transposition", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->transposition = populateBoolean(e, i); }},
    {"blineBreak", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->blineBreak = populateBoolean(e, i); }},
    {"rbarBreak", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->rbarBreak = populateBoolean(e, i); }},
    {"negMnumb", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negMnumb = populateBoolean(e, i); }},
    {"negRepeat", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negRepeat = populateBoolean(e, i); }},
    {"negNameScore", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negNameScore = populateBoolean(e, i); }},
    {"hideBarlines", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideBarlines = populateBoolean(e, i); }},
    {"fullName", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->fullName = populateBoolean(e, i); }},
    {"abrvName", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->abrvName = populateBoolean(e, i); }},
    {"floatKeys", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatKeys = populateBoolean(e, i); }},
    {"floatTime", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->floatTime = populateBoolean(e, i); }},
    {"hideRptBars", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideRptBars = populateBoolean(e, i); }},
    {"negKey", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negKey = populateBoolean(e, i); }},
    {"negTime", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negTime = populateBoolean(e, i); }},
    {"negClef", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negClef = populateBoolean(e, i); }},
    {"hideStaff", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideStaff = populateBoolean(e, i); }},
    {"noKey", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->noKey = populateBoolean(e, i); }},
    {"fullNamePos", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->fullNamePos = populateBoolean(e, i); }},
    {"abrvNamePos", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->abrvNamePos = populateBoolean(e, i); }},
    {"altNotation", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->altNotation = populateBoolean(e, i); }},
    {"showTies", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showTies = populateBoolean(e, i); }},
    {"showDots", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showDots = populateBoolean(e, i); }},
    {"showRests", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showRests = populateBoolean(e, i); }},
    {"showStems", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showStems = populateBoolean(e, i); }},
    {"hideChords", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideChords = populateBoolean(e, i); }},
    {"hideFretboards", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideFretboards = populateBoolean(e, i); }},
    {"hideLyrics", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideLyrics = populateBoolean(e, i); }},
    {"showNameParts", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showNameParts = populateBoolean(e, i); }},
    {"showNoteColors", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->showNoteColors = populateBoolean(e, i); }},
    {"hideStaffLines", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideStaffLines = populateBoolean(e, i); }},
    {"redisplayLayerAccis", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->redisplayLayerAccis = populateBoolean(e, i); }},
    {"negTimeParts", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->negTimeParts = populateBoolean(e, i); }},
    {"hideKeySigsShowAccis", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle::Masks>& i) { i->hideKeySigsShowAccis = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffStyle, []() {
    xml::XmlElementArray<StaffStyle> additionalFields = {
        {"styleName", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->styleName = e->getText(); }},
        {"copyable", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->copyable = populateBoolean(e, i); }},
        {"addToMenu", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) { i->addToMenu = populateBoolean(e, i); }},
        {"mask", [](const XmlElementPtr& e, const std::shared_ptr<StaffStyle>& i) {
            i->masks = FieldPopulator<StaffStyle::Masks>::createAndPopulate(e, i); }},
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
    {"noNames", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->noNames = populateBoolean(e, i); }},
    {"hasStaffScaling", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->hasStaffScaling = populateBoolean(e, i); }},
    {"placeEndSpaceBeforeBarline", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->placeEndSpaceBeforeBarline = populateBoolean(e, i); }},
    {"scaleVert", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->scaleVert = populateBoolean(e, i); }},
    {"scaleContentOnly", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->holdMargins = populateBoolean(e, i); }},
    {"distanceToPrev", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->distanceToPrev = e->getTextAs<Evpu>(); }},
    {"extraStartSystemSpace", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraStartSystemSpace = e->getTextAs<Evpu>(); }},
    {"extraEndSystemSpace", [](const XmlElementPtr& e, const std::shared_ptr<StaffSystem>& i) { i->extraEndSystemSpace = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffUsed, {
    {"inst", [](const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i) { i->staffId = e->getTextAs<Cmper>(); }},
    {"trackType", [](const XmlElementPtr&, const std::shared_ptr<StaffUsed>&) { /* this field seems like it was for future enhancement */ }},
    {"distFromTop", [](const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i) { i->distFromTop = e->getTextAs<Evpu>(); }},
    {"range", [](const XmlElementPtr& e, const std::shared_ptr<StaffUsed>& i)
        { i->range = FieldPopulator<MusicRange>::createAndPopulate(e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(SystemLock, {
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<SystemLock>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
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
    {"shapeID", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->shapeId = e->getTextAs<Cmper>(); }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"lineSpacingPercent", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->lineSpacingPercentage = e->getTextAs<int>(); }},
    {"xAdd", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->xAdd = e->getTextAs<Evpu>(); }},
    {"yAdd", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->yAdd = e->getTextAs<Evpu>(); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->justify = toEnum<TextBlock::TextJustify>(e); }},
    {"newPos36", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->newPos36 = populateBoolean(e, i); }},
    {"showShape", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->showShape = populateBoolean(e, i); }},
    {"noExpandSingleWord", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->noExpandSingleWord = populateBoolean(e, i); }},
    {"wordWrap", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->wordWrap = populateBoolean(e, i); }},
    {"inset", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->inset = e->getTextAs<Efix>(); }},
    {"stdLine", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->stdLineThickness = e->getTextAs<Efix>(); }},
    {"roundCorners", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->roundCorners = populateBoolean(e, i); }},
    {"cornerRadius", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->cornerRadius = e->getTextAs<Efix>(); }},
    {"textTag", [](const XmlElementPtr& e, const std::shared_ptr<TextBlock>& i) { i->textType = toEnum<TextBlock::TextType>(e); }}
});

MUSX_XML_ELEMENT_ARRAY(TextExpressionDef, {
    {"textIDKey", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"rehearsalMarkStyle", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->rehearsalMarkStyle = toEnum<RehearsalMarkStyle>(e); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"execShape", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->execShape = e->getTextAs<Cmper>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playPass", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playPass = e->getTextAs<int>(); }},
    {"hideMeasureNum", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->hideMeasureNum = populateBoolean(e, i); }},
    {"matchPlayback", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->matchPlayback = populateBoolean(e, i); }},
    {"useAuxData", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = populateBoolean(e, i); }},
    {"newEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = populateBoolean(e, i); }},
    {"breakMmRest", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = populateBoolean(e, i); }},
    {"createdByHp", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->createdByHp = populateBoolean(e, i); }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalTextJustification>(e); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryFonts = populateBoolean(e, i); }},
    {"useCategoryPos", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryPos = populateBoolean(e, i); }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->description = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatAssign, {
    {"horzPos", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->horzPos = e->getTextAs<Evpu>(); }},
    {"actuate", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->passNumber = e->getTextAs<int>(); }},
    {"target", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->targetValue = e->getTextAs<int>(); }},
    {"repnum", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->textRepeatId = e->getTextAs<Cmper>(); }},
    {"vertPos", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->vertPos = e->getTextAs<Evpu>(); }},
    {"indivPlac", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->individualPlacement = populateBoolean(e, i); }},
    {"topStaffOnly", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->topStaffOnly = populateBoolean(e, i); }},
    {"clrOnChange", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->resetOnAction = populateBoolean(e, i); }},
    {"multiActuate", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpOnMultiplePasses = populateBoolean(e, i); }},
    {"action", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpAction = toEnum<RepeatActionType>(e); }},
    {"autoUpdate", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->autoUpdate = populateBoolean(e, i); }},
    {"trigger", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->trigger = toEnum<RepeatTriggerType>(e); }},
    {"jmpIgnore", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->jumpIfIgnoring = populateBoolean(e, i); }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatAssign>& i) { i->staffList = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatDef, {
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { FieldPopulator<FontInfo>::populateField(i->font, e); }},
    {"newEnclosure", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->hasEnclosure = populateBoolean(e, i); }},
    {"useThisFont", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->useThisFont = populateBoolean(e, i); }},
    {"poundReplace", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->poundReplace = toEnum<TextRepeatDef::PoundReplaceOption>(e); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->justification = toEnum<HorizontalTextJustification>(e); }},
    {"act", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatDef>& i) { i->passList.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(TextRepeatText, {
    {"rptText", [](const XmlElementPtr& e, const std::shared_ptr<TextRepeatText>& i) { i->text = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(TimeCompositeLower::CompositeItem, {
    {"integer", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->unit = e->getTextAs<Edu>(); }},
    {"startGroup", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeLower::CompositeItem>& i) { i->startGroup = populateBoolean(e, i); }},
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
    {"startGroup", [](const XmlElementPtr& e, const std::shared_ptr<TimeCompositeUpper::CompositeItem>& i) { i->startGroup = populateBoolean(e, i); }},
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
