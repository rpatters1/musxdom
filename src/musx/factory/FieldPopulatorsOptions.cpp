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
using namespace ::musx::dom::options;

MUSX_XML_ENUM_MAPPING(BeamOptions::FlattenStyle, {
    {"onEndNotes", BeamOptions::FlattenStyle::OnEndNotes},
    {"onStandardNote", BeamOptions::FlattenStyle::OnStandardNote},
    {"onExtremeNote", BeamOptions::FlattenStyle::OnExtremeNote},
    {"alwaysFlat", BeamOptions::FlattenStyle::AlwaysFlat}
});

MUSX_XML_ENUM_MAPPING(ChordOptions::ChordAlignment, {
    {"left",   ChordOptions::ChordAlignment::Left},     // this value may never appear in the xml
    {"center", ChordOptions::ChordAlignment::Center}
});

MUSX_XML_ENUM_MAPPING(ChordOptions::ChordStyle, {
    {"Standard",     ChordOptions::ChordStyle::Standard},     // this value may never appear in the xml
    {"European",     ChordOptions::ChordStyle::European},
    {"German",       ChordOptions::ChordStyle::German},
    {"Roman",        ChordOptions::ChordStyle::Roman},
    {"NashvilleA",   ChordOptions::ChordStyle::NashvilleA},
    {"NashvilleB",   ChordOptions::ChordStyle::NashvilleB},
    {"solfeggio",    ChordOptions::ChordStyle::Solfeggio}, // lower case: this is an observed inconsistency for this enum type
    {"Scandinavian", ChordOptions::ChordStyle::Scandinavian}
});

MUSX_XML_ENUM_MAPPING(FontOptions::FontType, {
    {"music", FontOptions::FontType::Music},
    {"key", FontOptions::FontType::Key},
    {"clef", FontOptions::FontType::Clef},
    {"time", FontOptions::FontType::Time},
    {"chord", FontOptions::FontType::Chord},
    {"chordAcci", FontOptions::FontType::ChordAcci},
    {"ending", FontOptions::FontType::Ending},
    {"tuplet", FontOptions::FontType::Tuplet},
    {"textBlock", FontOptions::FontType::TextBlock},
    {"lyricVerse", FontOptions::FontType::LyricVerse},
    {"lyricChorus", FontOptions::FontType::LyricChorus},
    {"lyricSection", FontOptions::FontType::LyricSection},
    {"multiMeasRest", FontOptions::FontType::MultiMeasRest},
    {"tablature", FontOptions::FontType::Tablature},
    {"chordSuffix", FontOptions::FontType::ChordSuffix},
    {"expression", FontOptions::FontType::Expression},
    {"repeat", FontOptions::FontType::Repeat},
    {"fretboard", FontOptions::FontType::Fretboard},
    {"flags", FontOptions::FontType::Flags},
    {"accis", FontOptions::FontType::Accis},
    {"altNotSlash", FontOptions::FontType::AltNotSlash},
    {"altNotNum", FontOptions::FontType::AltNotNum},
    {"rests", FontOptions::FontType::Rests},
    {"reptDots", FontOptions::FontType::ReptDots},
    {"noteheads", FontOptions::FontType::Noteheads},
    {"augDots", FontOptions::FontType::AugDots},
    {"timePlus", FontOptions::FontType::TimePlus},
    {"articulation", FontOptions::FontType::Articulation},
    {"percussion", FontOptions::FontType::Percussion},
    {"smartShape8va", FontOptions::FontType::SmartShape8va},
    {"measNumb", FontOptions::FontType::MeasNumb},
    {"staffNames", FontOptions::FontType::StaffNames},
    {"abbrvStaffNames", FontOptions::FontType::AbbrvStaffNames},
    {"groupNames", FontOptions::FontType::GroupNames},
    {"smartShape8vb", FontOptions::FontType::SmartShape8vb},
    {"smartShape15ma", FontOptions::FontType::SmartShape15ma},
    {"smartShape15mb", FontOptions::FontType::SmartShape15mb},
    {"smartShapeTrill", FontOptions::FontType::SmartShapeTrill},
    {"smartShapeWiggle", FontOptions::FontType::SmartShapeWiggle},
    {"abbrvGroupNames", FontOptions::FontType::AbbrvGroupNames},
    {"bendCurveFull", FontOptions::FontType::BendCurveFull},
    {"bendCurveWhole", FontOptions::FontType::BendCurveWhole},
    {"bendCurveFrac", FontOptions::FontType::BendCurveFrac},
    {"timeParts", FontOptions::FontType::TimeParts},
    {"timePlusParts", FontOptions::FontType::TimePlusParts}
});

MUSX_XML_ENUM_MAPPING(LyricOptions::SmartHyphenStart, {
    {"always", LyricOptions::SmartHyphenStart::Always},
    {"sometimes", LyricOptions::SmartHyphenStart::Sometimes},
    {"never", LyricOptions::SmartHyphenStart::Never},
});

MUSX_XML_ENUM_MAPPING(LyricOptions::AutoNumberingAlign, {
    {"none", LyricOptions::AutoNumberingAlign::None},   // this default value may never appear in the xml
    {"align", LyricOptions::AutoNumberingAlign::Align},
});

MUSX_XML_ENUM_MAPPING(LyricOptions::AlignJustify, {
    {"left", LyricOptions::AlignJustify::Left},
    {"center", LyricOptions::AlignJustify::Center},
    {"right", LyricOptions::AlignJustify::Right},
});

MUSX_XML_ENUM_MAPPING(LyricOptions::WordExtConnectIndex, {
    {"lyricRightBottom", LyricOptions::WordExtConnectIndex::LyricRightBottom},
    {"headRightLyrBaseline", LyricOptions::WordExtConnectIndex::HeadRightLyrBaseline},
    {"systemLeft", LyricOptions::WordExtConnectIndex::SystemLeft},
    {"systemRight", LyricOptions::WordExtConnectIndex::SystemRight},
    {"dotRightLyrBaseline", LyricOptions::WordExtConnectIndex::DotRightLyrBaseline},
    {"durationLyrBaseline", LyricOptions::WordExtConnectIndex::DurationLyrBaseline},
});

MUSX_XML_ENUM_MAPPING(LyricOptions::WordExtConnectStyleType, {
    {"defaultStart", LyricOptions::WordExtConnectStyleType::DefaultStart},
    {"defaultEnd", LyricOptions::WordExtConnectStyleType::DefaultEnd},
    {"systemStart", LyricOptions::WordExtConnectStyleType::SystemStart},
    {"systemEnd", LyricOptions::WordExtConnectStyleType::SystemEnd},
    {"dottedEnd", LyricOptions::WordExtConnectStyleType::DottedEnd},
    {"durationEnd", LyricOptions::WordExtConnectStyleType::DurationEnd},
    {"oneEntryEnd", LyricOptions::WordExtConnectStyleType::OneEntryEnd},
    {"zeroLengthEnd", LyricOptions::WordExtConnectStyleType::ZeroLengthEnd},
    {"zeroOffset", LyricOptions::WordExtConnectStyleType::ZeroOffset},
});

MUSX_XML_ENUM_MAPPING(LyricOptions::SyllablePosStyleType, {
    {"default", LyricOptions::SyllablePosStyleType::Default},
    {"wordExt", LyricOptions::SyllablePosStyleType::WordExt},
    {"first", LyricOptions::SyllablePosStyleType::First},
    {"systemStart", LyricOptions::SyllablePosStyleType::SystemStart},
});

MUSX_XML_ENUM_MAPPING(MusicSpacingOptions::ColUnisonsChoice, {
    {"diffNoteheads", MusicSpacingOptions::ColUnisonsChoice::DiffNoteheads},
    {"all", MusicSpacingOptions::ColUnisonsChoice::All}
});

MUSX_XML_ENUM_MAPPING(MusicSpacingOptions::ManualPositioning, {
    // {"clear", MusicSpacingOptions::ManualPositioning::Clear}, the default value is not known to occur in the xml
    {"ignore", MusicSpacingOptions::ManualPositioning::Ignore},
    {"incorp", MusicSpacingOptions::ManualPositioning::Incorporate}
});

MUSX_XML_ENUM_MAPPING(MusicSpacingOptions::GraceNoteSpacing, {
    {"recomp", MusicSpacingOptions::GraceNoteSpacing::Automatic},
    {"keep", MusicSpacingOptions::GraceNoteSpacing::KeepCurrent}
});

MUSX_XML_ENUM_MAPPING(PageFormatOptions::AdjustPageScope, {
    {"current", PageFormatOptions::AdjustPageScope::Current},
    {"all", PageFormatOptions::AdjustPageScope::All},
    {"leftOrRight", PageFormatOptions::AdjustPageScope::LeftOrRight},
    {"range", PageFormatOptions::AdjustPageScope::PageRange}
});

MUSX_XML_ENUM_MAPPING(RepeatOptions::BackToBackStyle, {
    {"thin", RepeatOptions::BackToBackStyle::Thin},
    {"mixed", RepeatOptions::BackToBackStyle::Mixed},
    {"thick", RepeatOptions::BackToBackStyle::Thick}
});

MUSX_XML_ENUM_MAPPING(RepeatOptions::WingStyle, {
    {"none", RepeatOptions::WingStyle::None},
    {"curved", RepeatOptions::WingStyle::Curved},
    {"singleLine", RepeatOptions::WingStyle::SingleLine},
    {"doubleLine", RepeatOptions::WingStyle::DoubleLine}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::DefaultDirection, {
    {"automatic", SmartShapeOptions::DefaultDirection::Automatic},
    {"over", SmartShapeOptions::DefaultDirection::Over},
    {"under", SmartShapeOptions::DefaultDirection::Under},
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::ConnectionIndex, {
    {"headRightTop", SmartShapeOptions::ConnectionIndex::HeadRightTop},
    {"headLeftTop", SmartShapeOptions::ConnectionIndex::HeadLeftTop},
    {"stemRightTop", SmartShapeOptions::ConnectionIndex::StemRightTop},
    {"stemLeftTop", SmartShapeOptions::ConnectionIndex::StemLeftTop},
    {"headRightBottom", SmartShapeOptions::ConnectionIndex::HeadRightBottom},
    {"headLeftBottom", SmartShapeOptions::ConnectionIndex::HeadLeftBottom},
    {"stemRightBottom", SmartShapeOptions::ConnectionIndex::StemRightBottom},
    {"stemLeftBottom", SmartShapeOptions::ConnectionIndex::StemLeftBottom},
    {"noteRightCenter", SmartShapeOptions::ConnectionIndex::NoteRightCenter},
    {"noteLeftCenter", SmartShapeOptions::ConnectionIndex::NoteLeftCenter},
    {"noteRightBottom", SmartShapeOptions::ConnectionIndex::NoteRightBottom},
    {"noteLeftBottom", SmartShapeOptions::ConnectionIndex::NoteLeftBottom},
    {"noteLeftTop", SmartShapeOptions::ConnectionIndex::NoteLeftTop},
    {"noteRightTop", SmartShapeOptions::ConnectionIndex::NoteRightTop}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::SlurConnectStyleType, {
    {"overNoteStart", SmartShapeOptions::SlurConnectStyleType::OverNoteStart},
    {"overNoteEnd", SmartShapeOptions::SlurConnectStyleType::OverNoteEnd},
    {"overStemStart", SmartShapeOptions::SlurConnectStyleType::OverStemStart},
    {"overStemEnd", SmartShapeOptions::SlurConnectStyleType::OverStemEnd},
    {"underNoteStart", SmartShapeOptions::SlurConnectStyleType::UnderNoteStart},
    {"underNoteEnd", SmartShapeOptions::SlurConnectStyleType::UnderNoteEnd},
    {"underStemStart", SmartShapeOptions::SlurConnectStyleType::UnderStemStart},
    {"underStemEnd", SmartShapeOptions::SlurConnectStyleType::UnderStemEnd},
    {"overMixStemStart", SmartShapeOptions::SlurConnectStyleType::OverMixStemStart},
    {"overMixStemEnd", SmartShapeOptions::SlurConnectStyleType::OverMixStemEnd},
    {"overStemGrace", SmartShapeOptions::SlurConnectStyleType::OverStemGrace},
    {"overStemPrincipal", SmartShapeOptions::SlurConnectStyleType::OverStemPrincipal},
    {"underStemGrace", SmartShapeOptions::SlurConnectStyleType::UnderStemGrace},
    {"underStemPrincipal", SmartShapeOptions::SlurConnectStyleType::UnderStemPrincipal},
    {"underNoteGrace", SmartShapeOptions::SlurConnectStyleType::UnderNoteGrace},
    {"underStemNotePrincipal", SmartShapeOptions::SlurConnectStyleType::UnderStemNotePrincipal},
    {"overNoteGrace", SmartShapeOptions::SlurConnectStyleType::OverNoteGrace},
    {"overStemNotePrincipal", SmartShapeOptions::SlurConnectStyleType::OverStemNotePrincipal},
    {"overBeamStart", SmartShapeOptions::SlurConnectStyleType::OverBeamStart},
    {"overBeamEnd", SmartShapeOptions::SlurConnectStyleType::OverBeamEnd},
    {"underBeamStart", SmartShapeOptions::SlurConnectStyleType::UnderBeamStart},
    {"underBeamEnd", SmartShapeOptions::SlurConnectStyleType::UnderBeamEnd},
    {"overMixFlagStart", SmartShapeOptions::SlurConnectStyleType::OverMixFlagStart},
    {"overFlagStart", SmartShapeOptions::SlurConnectStyleType::OverFlagStart},
    {"underFlagStart", SmartShapeOptions::SlurConnectStyleType::UnderFlagStart},
    {"overTabNumStart", SmartShapeOptions::SlurConnectStyleType::OverTabNumStart},
    {"overTabNumEnd", SmartShapeOptions::SlurConnectStyleType::OverTabNumEnd},
    {"underTabNumStart", SmartShapeOptions::SlurConnectStyleType::UnderTabNumStart},
    {"underTabNumEnd", SmartShapeOptions::SlurConnectStyleType::UnderTabNumEnd}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::TabSlideConnectStyleType, {
    {"diffLevelPitchUpLineStart", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchUpLineStart},
    {"diffLevelPitchUpLineEnd", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchUpLineEnd},
    {"diffLevelPitchUpSpaceStart", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchUpSpaceStart},
    {"diffLevelPitchUpSpaceEnd", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchUpSpaceEnd},
    {"diffLevelPitchDownLineStart", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchDownLineStart},
    {"diffLevelPitchDownLineEnd", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchDownLineEnd},
    {"diffLevelPitchDownSpaceStart", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchDownSpaceStart},
    {"diffLevelPitchDownSpaceEnd", SmartShapeOptions::TabSlideConnectStyleType::DiffLevelPitchDownSpaceEnd},
    {"sameLevelPitchUpLineStart", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchUpLineStart},
    {"sameLevelPitchUpLineEnd", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchUpLineEnd},
    {"sameLevelPitchUpSpaceStart", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchUpSpaceStart},
    {"sameLevelPitchUpSpaceEnd", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchUpSpaceEnd},
    {"sameLevelPitchDownLineStart", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchDownLineStart},
    {"sameLevelPitchDownLineEnd", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchDownLineEnd},
    {"sameLevelPitchDownSpaceStart", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchDownSpaceStart},
    {"sameLevelPitchDownSpaceEnd", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchDownSpaceEnd},
    {"sameLevelPitchSameStart", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchSameStart},
    {"sameLevelPitchSameEnd", SmartShapeOptions::TabSlideConnectStyleType::SameLevelPitchSameEnd}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::GlissandoConnectStyleType, {
    {"defaultStart", SmartShapeOptions::GlissandoConnectStyleType::DefaultStart},
    {"defaultEnd", SmartShapeOptions::GlissandoConnectStyleType::DefaultEnd}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::BendCurveConnectStyleType, {
    {"noteStart", SmartShapeOptions::BendCurveConnectStyleType::NoteStart},
    {"staffEnd", SmartShapeOptions::BendCurveConnectStyleType::StaffEnd},
    {"staffStart", SmartShapeOptions::BendCurveConnectStyleType::StaffStart},
    {"noteEnd", SmartShapeOptions::BendCurveConnectStyleType::NoteEnd},
    {"staffToTopLineStart", SmartShapeOptions::BendCurveConnectStyleType::StaffToTopLineStart},
    {"staffFromTopLineEnd", SmartShapeOptions::BendCurveConnectStyleType::StaffFromTopLineEnd},
    {"staffEndOffset", SmartShapeOptions::BendCurveConnectStyleType::StaffEndOffset},
    {"staffFromTopEndOffset", SmartShapeOptions::BendCurveConnectStyleType::StaffFromTopEndOffset}
});

MUSX_XML_ENUM_MAPPING(SmartShapeOptions::SlurControlStyleType, {
    {"shortSpan", SmartShapeOptions::SlurControlStyleType::ShortSpan},
    {"mediumSpan", SmartShapeOptions::SlurControlStyleType::MediumSpan},
    {"longSpan", SmartShapeOptions::SlurControlStyleType::LongSpan},
    {"extraLongSpan", SmartShapeOptions::SlurControlStyleType::ExtraLongSpan}
});

MUSX_XML_ENUM_MAPPING(TextOptions::DateFormat, {
    // {"short", TextOptions::DateFormat::Short}, // default value may not appear in the XML
    {"long",   TextOptions::DateFormat::Long},
    {"abbrev", TextOptions::DateFormat::Abbrev}
});

MUSX_XML_ENUM_MAPPING(TextOptions::HorizontalAlignment, {
    {"left", TextOptions::HorizontalAlignment::Left}, // This is the default.
    {"center", TextOptions::HorizontalAlignment::Center},
    {"right", TextOptions::HorizontalAlignment::Right},
});

MUSX_XML_ENUM_MAPPING(TextOptions::VerticalAlignment, {
    {"top", TextOptions::VerticalAlignment::Top}, // This is the default.
    {"center", TextOptions::VerticalAlignment::Center},
    {"bottom", TextOptions::VerticalAlignment::Bottom},
});

MUSX_XML_ENUM_MAPPING(TextOptions::TextJustify, {
    // {"left",       TextOptions::TextJustify::Left}, // This is the default and is not known to occur in the XML.
    {"center",     TextOptions::TextJustify::Center},
    {"right",      TextOptions::TextJustify::Right},
    {"full",       TextOptions::TextJustify::Full},
    {"forcedFull", TextOptions::TextJustify::ForcedFull}
});

MUSX_XML_ENUM_MAPPING(AccidentalInsertSymbolType, {
    {"sharp",    AccidentalInsertSymbolType::Sharp},
    {"flat",     AccidentalInsertSymbolType::Flat},
    {"natural",  AccidentalInsertSymbolType::Natural},
    {"dblSharp", AccidentalInsertSymbolType::DblSharp},
    {"dblFlat",  AccidentalInsertSymbolType::DblFlat}
});

MUSX_XML_ENUM_MAPPING(TieOptions::SecondsPlacement, {
    {"both", TieOptions::SecondsPlacement::ShiftForSeconds}
});

MUSX_XML_ENUM_MAPPING(TieOptions::ChordTieDirType, {
    {"stemReversal", TieOptions::ChordTieDirType::StemReversal},
    {"splitEvenly", TieOptions::ChordTieDirType::SplitEvenly}
});

MUSX_XML_ENUM_MAPPING(TieOptions::MixedStemDirection, {
    {"over", TieOptions::MixedStemDirection::Over},
    {"under", TieOptions::MixedStemDirection::Under},
    {"start", TieOptions::MixedStemDirection::OppositeFirst}
});

MUSX_XML_ENUM_MAPPING(TieOptions::SpecialPosMode, {
    {"avoid", TieOptions::SpecialPosMode::Avoid}
});

MUSX_XML_ENUM_MAPPING(TieOptions::InsetStyle, {
    {"percent", TieOptions::InsetStyle::Percent}
});

MUSX_XML_ENUM_MAPPING(TieOptions::ConnectStyleType, {
    {"overStartPosInner", TieOptions::ConnectStyleType::OverStartPosInner},
    {"overEndPosInner", TieOptions::ConnectStyleType::OverEndPosInner},
    {"underStartPosInner", TieOptions::ConnectStyleType::UnderStartPosInner},
    {"underEndPosInner", TieOptions::ConnectStyleType::UnderEndPosInner},
    {"overHighestNoteStartPosOver", TieOptions::ConnectStyleType::OverHighestNoteStartPosOver},
    {"overHighestNoteEndPosOver", TieOptions::ConnectStyleType::OverHighestNoteEndPosOver},
    {"underLowestNoteStartPosUnder", TieOptions::ConnectStyleType::UnderLowestNoteStartPosUnder},
    {"underLowestNoteEndPosUnder", TieOptions::ConnectStyleType::UnderLowestNoteEndPosUnder},
    {"overHighestNoteStemStartPosOver", TieOptions::ConnectStyleType::OverHighestNoteStemStartPosOver},
    {"overHighestNoteStemEndPosOver", TieOptions::ConnectStyleType::OverHighestNoteStemEndPosOver},
    {"underLowestNoteStemStartPosUnder", TieOptions::ConnectStyleType::UnderLowestNoteStemStartPosUnder},
    {"underLowestNoteStemEndPosUnder", TieOptions::ConnectStyleType::UnderLowestNoteStemEndPosUnder}
});

MUSX_XML_ENUM_MAPPING(TieOptions::ControlStyleType, {
    {"shortSpan", TieOptions::ControlStyleType::ShortSpan},
    {"mediumSpan", TieOptions::ControlStyleType::MediumSpan},
    {"longSpan", TieOptions::ControlStyleType::LongSpan},
    {"tieEnds", TieOptions::ControlStyleType::TieEnds}
});

MUSX_XML_ENUM_MAPPING(TupletOptions::AutoBracketStyle, {
    //{"always", TupletOptions::AutoBracketStyle::Always}, this is the default and is not known to occur in the xml
    {"unbeamedOnly", TupletOptions::AutoBracketStyle::UnbeamedOnly},
    {"neverBeamSide", TupletOptions::AutoBracketStyle::NeverBeamSide},
});

MUSX_XML_ENUM_MAPPING(TupletOptions::NumberStyle, {
    //{"nothing", TupletOptions::NumberStyle::Nothing}, this is the default and is not known to occur in the xml
    {"number", TupletOptions::NumberStyle::Number},
    {"useRatio", TupletOptions::NumberStyle::UseRatio},
    {"ratioPlusDenNote", TupletOptions::NumberStyle::RatioPlusBothNotes},           // Finale appears to have inverted the meanings of `ratioPlusDenNote`
    {"ratioPlusBothNotes", TupletOptions::NumberStyle::RatioPlusDenominatorNote},   //              and `ratioPlusBothNotes`
});

MUSX_XML_ENUM_MAPPING(TupletOptions::PositioningStyle, {
    {"manual", TupletOptions::PositioningStyle::Manual}, //this is the default and occurs in the xml for tupetDef
    {"beamSide", TupletOptions::PositioningStyle::BeamSide},
    {"noteSide", TupletOptions::PositioningStyle::NoteSide},
    {"above", TupletOptions::PositioningStyle::Above},
    {"below", TupletOptions::PositioningStyle::Below},
});

MUSX_XML_ENUM_MAPPING(TupletOptions::BracketStyle, {
    //{"nothing", TupletOptions::BracketStyle::Nothing}, this is the default and is not known to occur in the xml
    {"slur", TupletOptions::BracketStyle::Slur},
    {"bracket", TupletOptions::BracketStyle::Bracket},
});

} // namespace factory

namespace dom {

namespace options {

using namespace ::musx::xml;
using namespace ::musx::factory;

// Field populator arrays are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

MUSX_XML_ELEMENT_ARRAY(AccidentalOptions, {
    {"overlap", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->minOverlap = e->getTextAs<int>(); }},
    {"bacciAdd", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->multiCharSpace = e->getTextAs<Evpu>(); }},
    {"useNewAcciPositioning", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->crossLayerPositioning = populateBoolean(e, i); }},
    {"frontAcciSepar", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->startMeasureSepar = e->getTextAs<Evpu>(); }},
    {"acciNoteSpace", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciNoteSpace = e->getTextAs<Evpu>(); }},
    {"acciAcciSpace", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciAcciSpace = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(AlternateNotationOptions, {
    {"halfSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashLift = e->getTextAs<Evpu>(); }},
    {"wholeSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->wholeSlashLift = e->getTextAs<Evpu>(); }},
    {"dWholeSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->dWholeSlashLift = e->getTextAs<Evpu>(); }},
    {"halfSlashStemLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashStemLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashLift = e->getTextAs<Evpu>(); }},
    {"twoMeasNumLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->twoMeasNumLift = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(AugmentationDotOptions, {
    {"dotUpFlagOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotUpFlagOffset = e->getTextAs<Evpu>(); }},
    {"dotOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotOffset = e->getTextAs<Evpu>(); }},
    {"doDotDownAdjust", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->adjMultipleVoices = populateBoolean(e, i); }},
    {"dotFirstOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotNoteOffset = e->getTextAs<Evpu>(); }},
    {"dotLift", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotLift = e->getTextAs<Evpu>(); }},
    {"skip27_4DotAdjust", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->useLegacyFlippedStemPositioning = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BarlineOptions, {
    {"drawCloseSystemBarline", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseSystemBarline = populateBoolean(e, i); }},
    {"drawCloseFinalBarline", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseFinalBarline = populateBoolean(e, i); }},
    {"drawFinalBarlineOnLastMeas", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawFinalBarlineOnLastMeas = populateBoolean(e, i); }},
    {"leftBarlineUsePrevStyle", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->leftBarlineUsePrevStyle = populateBoolean(e, i); }},
    {"drawLeftBarlineSingleStaff", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineSingleStaff = populateBoolean(e, i); }},
    {"drawLeftBarlineMultipleStaves", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineMultipleStaves = populateBoolean(e, i); }},
    {"drawBarlines", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawBarlines = populateBoolean(e, i); }},
    {"barlineWidth", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineWidth = e->getTextAs<Efix>(); }},
    {"thickBarlineWidth", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->thickBarlineWidth = e->getTextAs<Efix>(); }},
    {"doubleBarlineSpace", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->doubleBarlineSpace = e->getTextAs<Efix>(); }},
    {"finalBarlineSpace", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->finalBarlineSpace = e->getTextAs<Efix>(); }},
    {"barlineDashOn", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOn = e->getTextAs<Evpu>(); }},
    {"barlineDashOff", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOff = e->getTextAs<Evpu>(); }},
    {"drawDoubleBarlineBeforeKeyChanges", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawDoubleBarlineBeforeKeyChanges = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamOptions, {
    {"beamStubLength", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamStubLength = e->getTextAs<Evpu>(); }},
    {"maxSlope", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxSlope = e->getTextAs<Evpu>(); }},
    {"beamSepar", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamSepar = e->getTextAs<Evpu>(); }},
    {"hmBeamTip", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxFromMiddle = e->getTextAs<int>(); }},
    {"beamingStyle", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamingStyle = toEnum<BeamOptions::FlattenStyle>(e); }},
    {"incEdgeRestsInBeamGroups", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->extendBeamsOverRests = populateBoolean(e, i); }},
    {"incRestsInClassicBeams", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->incRestsInFourGroups = populateBoolean(e, i); }},
    {"beamFourEighthsInCommonTime", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamFourEighthsInCommonTime = populateBoolean(e, i); }},
    {"beamThreeEighthsInCommonTime", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamThreeEighthsInCommonTime = populateBoolean(e, i); }},
    {"doStemStubs", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->dispHalfStemsOnRests = populateBoolean(e, i); }},
    {"oldFinaleRestBeams", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->oldFinaleRestBeams = populateBoolean(e, i); }},
    {"spanSpace", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->spanSpace = populateBoolean(e, i); }},
    {"extendSecBeamsOverRests", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->extendSecBeamsOverRests = populateBoolean(e, i); }},
    {"beamWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordOptions, {
    {"fretPercent", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretPercent = e->getTextAs<double>(); }},
    {"chordPercent", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordPercent = e->getTextAs<double>(); }},
    {"chordSharpLift", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordSharpLift = e->getTextAs<Evpu>(); }},
    {"chordFlatLift", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordFlatLift = e->getTextAs<Evpu>(); }},
    {"chordNaturalLift", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordNaturalLift = e->getTextAs<Evpu>(); }},
    {"showFretboards", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->showFretboards = populateBoolean(e, i); }},
    {"fretStyleID", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretStyleId = e->getTextAs<Cmper>(); }},
    {"fretInstID", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"multiFretItemsPerStr", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->multiFretItemsPerStr = populateBoolean(e, i); }},
    {"italicizeCapoChords", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->italicizeCapoChords = populateBoolean(e, i); }},
    {"chordAlignment", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordAlignment = toEnum<ChordOptions::ChordAlignment>(e); }},
    {"chordStyle", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordStyle = toEnum<ChordOptions::ChordStyle>(e); }},
    {"useSimpleChordSpelling", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->useSimpleChordSpelling = populateBoolean(e, i); }},
    {"chordPlayback", [](const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordPlayback = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOptions::ClefDef, {
    {"adjust", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->middleCPos = e->getTextAs<int>(); }},
    {"clefChar", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->clefChar = e->getTextAs<char32_t>(); }},
    {"clefYDisp", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->staffPosition = e->getTextAs<int>(); }},
    {"baseAdjust", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->baselineAdjust = e->getTextAs<Efix>(); }},
    {"shapeID", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->shapeId = e->getTextAs<Cmper>(); }},
    {"isShape", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->isShape = populateBoolean(e, i); }},
    {"scaleToStaffHeight", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->scaleToStaffHeight = populateBoolean(e, i); }},
    {"font", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->font = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"useOwnFont", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->useOwnFont = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOptions, {
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"endMeasClefPercent", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangePercent = e->getTextAs<int>(); }},
    {"endMeasClefPosAdd", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangeOffset = e->getTextAs<Evpu>(); }},
    {"clefFront", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefFrontSepar = e->getTextAs<Evpu>(); }},
    {"clefBack", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefBackSepar = e->getTextAs<Evpu>(); }},
    {"showClefFirstSystemOnly", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->showClefFirstSystemOnly = populateBoolean(e, i); }},
    {"clefKey", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefKeySepar = e->getTextAs<Evpu>(); }},
    {"clefTime", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefTimeSepar = e->getTextAs<Evpu>(); }},
    {"cautionaryClefChanges", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->cautionaryClefChanges = populateBoolean(e, i); }},
    {"clefDef", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) {
            auto indexAttr = e->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : static_cast<size_t>(-1);;
            if (i->clefDefs.size() != index) {
                throw std::invalid_argument("ClefDef index mismatch. Expected: " + std::to_string(i->clefDefs.size()) + ", Found: " + std::to_string(index));
            }
            i->clefDefs.push_back(FieldPopulator<ClefOptions::ClefDef>::createAndPopulate(e, i));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(FlagOptions, {
    {"straightFlags", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->straightFlags = populateBoolean(e, i); }},
    {"upHAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj = e->getTextAs<Efix>(); }},
    {"downHAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj = e->getTextAs<Efix>(); }},
    {"upHAdj2", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj2 = e->getTextAs<Efix>(); }},
    {"downHAdj2", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj2 = e->getTextAs<Efix>(); }},
    {"upHAdj16", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj16 = e->getTextAs<Efix>(); }},
    {"downHAdj16", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj16 = e->getTextAs<Efix>(); }},
    {"eighthFlagHoist", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->eighthFlagHoist = e->getTextAs<Efix>(); }},
    {"stUpHAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stUpHAdj = e->getTextAs<Efix>(); }},
    {"stDownHAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stDownHAdj = e->getTextAs<Efix>(); }},
    {"upVAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj = e->getTextAs<Efix>(); }},
    {"downVAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj = e->getTextAs<Efix>(); }},
    {"upVadj2", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj2 = e->getTextAs<Efix>(); }}, // "upVadj2" is intentionally preserved as is
    {"downVAdj2", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj2 = e->getTextAs<Efix>(); }},
    {"upVAdj16", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj16 = e->getTextAs<Efix>(); }},
    {"downVAdj16", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj16 = e->getTextAs<Efix>(); }},
    {"stUpVAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stUpVAdj = e->getTextAs<Efix>(); }},
    {"stDownVAdj", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stDownVAdj = e->getTextAs<Efix>(); }},
    {"separ", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->flagSpacing = e->getTextAs<Evpu>(); }},
    {"extra", [](const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->secondaryGroupAdj = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FontOptions, {
    {
        "font", [](const XmlElementPtr& fontElement, const std::shared_ptr<FontOptions>& fonts) {
            auto typeStr = fontElement->findAttribute("type");
            if (!typeStr) {
                throw std::invalid_argument("font option has no type");
            }
            auto fontType = toEnum<FontOptions::FontType>(typeStr->getValue());

            auto fontInfo = std::make_shared<dom::FontInfo>(fonts->getDocument());
            FieldPopulator<dom::FontInfo>::populate(fontInfo, fontElement);

            // Add the populated font instance to the vector.
            fonts->fontOptions.emplace(fontType, fontInfo);
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(GraceNoteOptions, {
    {"tabGracePerc", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->tabGracePerc = e->getTextAs<int>(); }},
    {"gracePerc", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->gracePerc = e->getTextAs<int>(); }},
    {"playbackDuration", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->playbackDuration = e->getTextAs<Edu>(); }},
    {"graceBackup", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->entryOffset = e->getTextAs<Evpu>(); }},
    {"slashFlaggedGraceNotes", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->slashFlaggedGraceNotes = populateBoolean(e, i); }},
    {"graceSlashWidth", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->graceSlashWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeySignatureOptions, {
    {"doKeyCancel", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancel = populateBoolean(e, i); }},
    {"doCStart", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doCStart = populateBoolean(e, i); }},
    {"doBankDiff", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->redisplayOnModeChange = populateBoolean(e, i); }},
    {"keyFront", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyFront = e->getTextAs<Evpu>(); }},
    {"keyMid", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyMid = e->getTextAs<Evpu>(); }},
    {"keyBack", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyBack = e->getTextAs<Evpu>(); }},
    {"acciAdd", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->acciAdd = e->getTextAs<Evpu>(); }},
    {"showKeyFirstSystemOnly", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->showKeyFirstSystemOnly = populateBoolean(e, i); }},
    {"keyTime", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyTimeSepar = e->getTextAs<Evpu>(); }},
    {"simplifyKeyHoldOctave", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->simplifyKeyHoldOctave = populateBoolean(e, i); }},
    {"cautionaryKeyChanges", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->cautionaryKeyChanges = populateBoolean(e, i); }},
    {"doKeyCancelBetweenSharpsFlats", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancelBetweenSharpsFlats = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LineCurveOptions, {
    {"bezierStep", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->bezierStep = e->getTextAs<int>(); }},
    {"enclosureWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureWidth = e->getTextAs<Efix>(); }},
    {"enclosureRoundCorners", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureRoundCorners = populateBoolean(e, i); }},
    {"enclosureCornerRadius", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureCornerRadius = e->getTextAs<Efix>(); }},
    {"staffLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->staffLineWidth = e->getTextAs<Efix>(); }},
    {"legerLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerLineWidth = e->getTextAs<Efix>(); }},
    {"legerFrontLength", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerFrontLength = e->getTextAs<Evpu>(); }},
    {"legerBackLength", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerBackLength = e->getTextAs<Evpu>(); }},
    {"restLegerFrontLength", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->restLegerFrontLength = e->getTextAs<Evpu>(); }},
    {"restLegerBackLength", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->restLegerBackLength = e->getTextAs<Evpu>(); }},
    {"psUlDepth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->psUlDepth = e->getTextAs<double>(); }},
    {"psUlWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->psUlWidth = e->getTextAs<double>(); }},
    {"pathSlurTipWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->pathSlurTipWidth = e->getTextAs<EvpuFloat>(); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions::SyllablePosStyle, {
    {"align",   [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->align = toEnum<LyricOptions::AlignJustify>(e); }},
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->justify = toEnum<LyricOptions::AlignJustify>(e); }},
    {"on",      [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->on = populateBoolean(e, i); }}
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions::WordExtConnectStyle, {
    {"connectIndex", [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->connectIndex = toEnum<LyricOptions::WordExtConnectIndex>(e); }},
    {"xOffset",      [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"yOffset",      [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->yOffset = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions, {
    {"hyphenChar",              [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->hyphenChar = e->getTextAs<char32_t>(); }},
    {"maxHyphenSeparation",     [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->maxHyphenSeparation = e->getTextAs<Evpu>(); }},
    {"wordExtVertOffset",       [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtVertOffset = e->getTextAs<Evpu>(); }},
    {"wordExtHorzOffset",       [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtHorzOffset = e->getTextAs<Evpu>(); }},
    {"useSmartWordExtensions",  [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useSmartWordExtensions = populateBoolean(e, i); }},
    {"useAltHyphenFont",        [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useAltHyphenFont = populateBoolean(e, i); }},
    {"altHyphenFont",           [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->altHyphenFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"useSmartHyphens",         [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useSmartHyphens = populateBoolean(e, i); }},
    {"smartHyphenStart",        [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->smartHyphenStart = toEnum<LyricOptions::SmartHyphenStart>(e); }},
    {"wordExtNeedUnderscore",   [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtNeedUnderscore = populateBoolean(e, i); }},
    {"wordExtMinLength",        [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtMinLength = e->getTextAs<Evpu>(); }},
    {"wordExtOffsetToNotehead", [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtOffsetToNotehead = populateBoolean(e, i); }},
    {"lyricUseEdgePunctuation", [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricUseEdgePunctuation = populateBoolean(e, i); }},
    {"lyricPunctuationToIgnore",[](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricPunctuationToIgnore = e->getText(); }},
    {"lyricAutoNumType",        [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricAutoNumType = toEnum<LyricOptions::AutoNumberingAlign>(e); }},
    {"wordExtLineWidth",        [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtLineWidth = e->getTextAs<Efix>(); }},
    {"lyricSyllPosStyle",       [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { populateEmbeddedClass(e, i->syllablePosStyles); }},
    {"wordExtConnectStyle",     [](const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { populateEmbeddedClass(e, i->wordExtConnectStyles); }}
});

MUSX_XML_ELEMENT_ARRAY(MiscOptions, {
    {"showRepeatsForParts", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->showRepeatsForParts = populateBoolean(e, i); }},
    {"pickupValue", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->pickupValue = e->getTextAs<Edu>();}},
    {"retainOctaveTransInConcertPitch", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->keepWrittenOctaveInConcertPitch = populateBoolean(e, i); }},
    {"showCurrentLayerOnly", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->showActiveLayerOnly = populateBoolean(e, i); }},
    {"combineRestsAcrossLayers", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->consolidateRestsAcrossLayers = populateBoolean(e, i); }},
    {"sdDashOn", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashLength = e->getTextAs<Evpu>();}},
    {"sdDashOff", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashSpace = e->getTextAs<Evpu>();}},
    {"drawMeasureNumbersOverBarlines", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->alignMeasureNumbersWithBarlines = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MultimeasureRestOptions, {
    {"meaSpace", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->measWidth = e->getTextAs<Evpu>();}},
    {"numdec", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjY = e->getTextAs<Evpu>();}},
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->shapeDef = e->getTextAs<Cmper>();}},
    {"numStart", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numStart = e->getTextAs<int>();}},
    {"threshold", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->useSymsThreshold = e->getTextAs<int>();}},
    {"spacing", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->symSpacing = e->getTextAs<Evpu>();}},
    {"numAdjX", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjX = e->getTextAs<Evpu>();}},
    {"startAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->startAdjust = e->getTextAs<Evpu>();}},
    {"endAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->endAdjust = e->getTextAs<Evpu>();}},
    {"useCharRestStyle", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) { i->useSymbols = populateBoolean(e, i); }},
    {"autoUpdateMmRests", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) { i->autoUpdateMmRests = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MusicSpacingOptions, {
    {"layoutType", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>&) { /* appears to be cruft left over from legacy Finale */ }},
    {"minWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minWidth = e->getTextAs<Evpu>(); }},
    {"maxWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->maxWidth = e->getTextAs<Evpu>(); }},
    {"minDistance", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistance = e->getTextAs<Evpu>(); }},
    {"minDistTiedNotes", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistTiedNotes = e->getTextAs<Evpu>(); }},
    {"avoidColNotes", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColNotes = populateBoolean(e, i); }},
    {"avoidColLyrics", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLyrics = populateBoolean(e, i); }},
    {"avoidColChords", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColChords = populateBoolean(e, i); }},
    {"avoidColArtics", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColArtics = populateBoolean(e, i); }},
    {"avoidColClefs", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColClefs = populateBoolean(e, i); }},
    {"avoidColSeconds", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColSeconds = populateBoolean(e, i); }},
    {"avoidColStems", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColStems = populateBoolean(e, i); }},
    {"avoidColUnisons", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColUnisons = toEnum<MusicSpacingOptions::ColUnisonsChoice>(e); }},
    {"avoidColLedgers", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLedgers = populateBoolean(e, i); }},
    {"manualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->manualPositioning = toEnum<MusicSpacingOptions::ManualPositioning>(e); }},
    {"ignoreHidden", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->ignoreHidden = populateBoolean(e, i); }},
    {"interpolateAllotments", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->interpolateAllotments = populateBoolean(e, i); }},
    {"usePrinter", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->usePrinter = populateBoolean(e, i); }},
    {"useAllottmentTables", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->useAllottmentTables = populateBoolean(e, i); }},
    {"referenceDuration", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"referenceWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceWidth = e->getTextAs<Evpu>(); }},
    {"scalingFactor", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->scalingFactor = e->getTextAs<double>(); }},
    {"defaultAllotment", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->defaultAllotment = e->getTextAs<Evpu>(); }},
    {"minDistGrace", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistGrace = e->getTextAs<Evpu>(); }},
    {"graceNoteManualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->graceNoteSpacing = toEnum<MusicSpacingOptions::GraceNoteSpacing>(e); }},
    {"musFront", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musFront = e->getTextAs<Evpu>(); }},
    {"musBack", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musBack = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(MusicSymbolOptions, {
    {"noteheadQuarter", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadQuarter = e->getTextAs<char32_t>(); }},
    {"noteheadHalf", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadHalf = e->getTextAs<char32_t>(); }},
    {"noteheadWhole", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadWhole = e->getTextAs<char32_t>(); }},
    {"noteheadDblWhole", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadDblWhole = e->getTextAs<char32_t>(); }},
    {"natural", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->natural = e->getTextAs<char32_t>(); }},
    {"flat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flat = e->getTextAs<char32_t>(); }},
    {"sharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->sharp = e->getTextAs<char32_t>(); }},
    {"dblFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblFlat = e->getTextAs<char32_t>(); }},
    {"dblSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblSharp = e->getTextAs<char32_t>(); }},
    {"parenNatural", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenNatural = e->getTextAs<char32_t>(); }},
    {"parenFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenFlat = e->getTextAs<char32_t>(); }},
    {"parenSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenSharp = e->getTextAs<char32_t>(); }},
    {"parenDblFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenDblFlat = e->getTextAs<char32_t>(); }},
    {"parenDblSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenDblSharp = e->getTextAs<char32_t>(); }},
    {"chordNatural", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordNatural = e->getTextAs<char32_t>(); }},
    {"chordFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordFlat = e->getTextAs<char32_t>(); }},
    {"chordSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordSharp = e->getTextAs<char32_t>(); }},
    {"chordDblFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordDblFlat = e->getTextAs<char32_t>(); }},
    {"chordDblSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordDblSharp = e->getTextAs<char32_t>(); }},
    {"keySigNatural", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigNatural = e->getTextAs<char32_t>(); }},
    {"keySigFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigFlat = e->getTextAs<char32_t>(); }},
    {"keySigSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigSharp = e->getTextAs<char32_t>(); }},
    {"keySigDblFlat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigDblFlat = e->getTextAs<char32_t>(); }},
    {"keySigDblSharp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigDblSharp = e->getTextAs<char32_t>(); }},
    {"restLonga", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restLonga = e->getTextAs<char32_t>(); }},
    {"restDblWhole", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restDblWhole = e->getTextAs<char32_t>(); }},
    {"restWhole", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restWhole = e->getTextAs<char32_t>(); }},
    {"restHalf", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restHalf = e->getTextAs<char32_t>(); }},
    {"restQuarter", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restQuarter = e->getTextAs<char32_t>(); }},
    {"restEighth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restEighth = e->getTextAs<char32_t>(); }},
    {"rest16th", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest16th = e->getTextAs<char32_t>(); }},
    {"rest32nd", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest32nd = e->getTextAs<char32_t>(); }},
    {"rest64th", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest64th = e->getTextAs<char32_t>(); }},
    {"rest128th", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest128th = e->getTextAs<char32_t>(); }},
    {"restDefMeas", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restDefMeas = e->getTextAs<char32_t>(); }},
    {"oneBarRepeat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->oneBarRepeat = e->getTextAs<char32_t>(); }},
    {"twoBarRepeat", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->twoBarRepeat = e->getTextAs<char32_t>(); }},
    {"slashBar", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->slashBar = e->getTextAs<char32_t>(); }},
    {"quarterSlash", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->quarterSlash = e->getTextAs<char32_t>(); }},
    {"halfSlash", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->halfSlash = e->getTextAs<char32_t>(); }},
    {"wholeSlash", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->wholeSlash = e->getTextAs<char32_t>(); }},
    {"dblWholeSlash", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblWholeSlash = e->getTextAs<char32_t>(); }},
    {"timeSigPlus", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigPlus = e->getTextAs<char32_t>(); }},
    {"timeSigPlusParts", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigPlusParts = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCommon", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCommon = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCut", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCut = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCommonParts", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCommonParts = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCutParts", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCutParts = e->getTextAs<char32_t>(); }},
    {"augDot", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->augDot = e->getTextAs<char32_t>(); }},
    {"forwardRepeatDot", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->forwardRepeatDot = e->getTextAs<char32_t>(); }},
    {"backRepeatDot", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->backRepeatDot = e->getTextAs<char32_t>(); }},
    {"eightVaUp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->eightVaUp = e->getTextAs<char32_t>(); }},
    {"eightVbDown", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->eightVbDown = e->getTextAs<char32_t>(); }},
    {"fifteenMaUp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->fifteenMaUp = e->getTextAs<char32_t>(); }},
    {"fifteenMbDown", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->fifteenMbDown = e->getTextAs<char32_t>(); }},
    {"trillChar", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->trillChar = e->getTextAs<char32_t>(); }},
    {"wiggleChar", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->wiggleChar = e->getTextAs<char32_t>(); }},
    {"flagUp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagUp = e->getTextAs<char32_t>(); }},
    {"flagDown", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagDown = e->getTextAs<char32_t>(); }},
    {"flag16Up", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag16Up = e->getTextAs<char32_t>(); }},
    {"flag16Down", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag16Down = e->getTextAs<char32_t>(); }},
    {"flag2Up", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag2Up = e->getTextAs<char32_t>(); }},
    {"flag2Down", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag2Down = e->getTextAs<char32_t>(); }},
    {"flagStraightUp", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagStraightUp = e->getTextAs<char32_t>(); }},
    {"flagStraightDown", [](const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagStraightDown = e->getTextAs<char32_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteRestOptions::NoteColor, {
    {"red", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->red = e->getTextAs<uint16_t>(); }},
    {"green", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->green = e->getTextAs<uint16_t>(); }},
    {"blue", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->blue = e->getTextAs<uint16_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteRestOptions, {
    {"doShapeNotes", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->doShapeNotes = populateBoolean(e, i); }},
    {"doCrossOver", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->doCrossStaffNotes = populateBoolean(e, i); }},
    {"drop8thRest", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop8thRest = e->getTextAs<Evpu>(); }},
    {"drop16thRest", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop16thRest = e->getTextAs<Evpu>(); }},
    {"drop32ndRest", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop32ndRest = e->getTextAs<Evpu>(); }},
    {"drop64thRest", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop64thRest = e->getTextAs<Evpu>(); }},
    {"drop128thRest", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop128thRest = e->getTextAs<Evpu>(); }},
    {"scaleManualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->scaleManualPositioning = populateBoolean(e, i); }},
    {"drawOutline", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drawOutline = populateBoolean(e, i); }},
    {"noteColor", [](const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) {
            auto idAttr = e->findAttribute("id");
            size_t id = idAttr ? idAttr->getValueAs<size_t>() : static_cast<size_t>(-1);
            if (i->noteColors.size() != id) {
                throw std::invalid_argument("NoteColor id mismatch. Expected: " + std::to_string(i->noteColors.size())
                                            + ", Found: " + std::to_string(id));
            }
            i->noteColors.push_back(FieldPopulator<NoteRestOptions::NoteColor>::createAndPopulate(e));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(PageFormatOptions::PageFormat, {
    {"pageHeight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageHeight = e->getTextAs<Evpu>(); }},
    {"pageWidth", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageWidth = e->getTextAs<Evpu>(); }},
    {"pagePercent", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pagePercent = e->getTextAs<int>(); }},
    {"sysPercent", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysPercent = e->getTextAs<int>(); }},
    {"rawStaffHeight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rawStaffHeight = e->getTextAs<Evpu16ths>(); }},
    {"leftPageMarginTop", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginTop = e->getTextAs<Evpu>(); }},
    {"leftPageMarginLeft", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginLeft = e->getTextAs<Evpu>(); }},
    {"leftPageMarginBottom", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginBottom = e->getTextAs<Evpu>(); }},
    {"leftPageMarginRight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginRight = e->getTextAs<Evpu>(); }},
    {"rightPageMarginTop", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginTop = e->getTextAs<Evpu>(); }},
    {"rightPageMarginLeft", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginLeft = e->getTextAs<Evpu>(); }},
    {"rightPageMarginBottom", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginBottom = e->getTextAs<Evpu>(); }},
    {"rightPageMarginRight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginRight = e->getTextAs<Evpu>(); }},
    {"sysMarginTop", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginTop = e->getTextAs<Evpu>(); }},
    {"sysMarginLeft", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginLeft = e->getTextAs<Evpu>(); }},
    {"sysMarginBottom", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginBottom = e->getTextAs<Evpu>(); }},
    {"sysMarginRight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginRight = e->getTextAs<Evpu>(); }},
    {"sysDistanceBetween", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysDistanceBetween = e->getTextAs<Evpu>(); }},
    {"firstPageMarginTop", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstPageMarginTop = e->getTextAs<Evpu>(); }},
    {"firstSysMarginTop", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginTop = e->getTextAs<Evpu>(); }},
    {"firstSysMarginLeft", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginLeft = e->getTextAs<Evpu>(); }},
    {"firstSysMarginDistance", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginDistance = e->getTextAs<Evpu>(); }},
    {"facingPages", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->facingPages = populateBoolean(e, i); }},
    {"differentFirstSysMargin", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstSysMargin = populateBoolean(e, i); }},
    {"differentFirstPageMargin", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstPageMargin = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(PageFormatOptions, {
    {"pageFormatScore", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i){ i->pageFormatScore = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(e); }},
    {"pageFormatParts", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->pageFormatParts = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(e); }},
    {"avoidSystemMarginCollisions", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->avoidSystemMarginCollisions = populateBoolean(e, i); }},
    {"adjustPageScope", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->adjustPageScope = toEnum<PageFormatOptions::AdjustPageScope>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(PianoBraceBracketOptions, {
    {"defBracketPos", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->defBracketPos = e->getTextAs<Evpu>(); }},
    {"centerThickness", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->centerThickness = e->getTextAs<EvpuFloat>(); }},
    {"endThickness", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->tipThickness = e->getTextAs<EvpuFloat>(); }},
    {"outerWingY", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerBodyV = e->getTextAs<EvpuFloat>(); }},
    {"innerTipY", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerTipV = e->getTextAs<EvpuFloat>(); }},
    {"innerWingY", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerBodyV = e->getTextAs<EvpuFloat>(); }},
    {"outerTipX", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerTipH = e->getTextAs<EvpuFloat>(); }},
    {"outerTipY", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerTipV = e->getTextAs<EvpuFloat>(); }},
    {"outerWingX", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerBodyH = e->getTextAs<EvpuFloat>(); }},
    {"width", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->width = e->getTextAs<EvpuFloat>(); }},
    {"innerTipX", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerTipH = e->getTextAs<EvpuFloat>(); }},
    {"innerWingX", [](const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerBodyH = e->getTextAs<EvpuFloat>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatOptions, {
    {"bracketHeight", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHeight = e->getTextAs<Evpu>(); }},
    {"maxPasses", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->maxPasses = e->getTextAs<int>(); }},
    {"addPeriod", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->addPeriod = populateBoolean(e, i); }},
    {"thickLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thickLineWidth = e->getTextAs<Efix>(); }},
    {"thinLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thinLineWidth = e->getTextAs<Efix>(); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lineSpace = e->getTextAs<Efix>(); }},
    {"backToBackStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backToBackStyle = toEnum<RepeatOptions::BackToBackStyle>(e); }},
    {"forwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->forwardDotHPos = e->getTextAs<Evpu>(); }},
    {"backwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backwardDotHPos = e->getTextAs<Evpu>(); }},
    {"upperDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->upperDotVPos = e->getTextAs<Evpu>(); }},
    {"lowerDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lowerDotVPos = e->getTextAs<Evpu>(); }},
    {"wingStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->wingStyle = toEnum<RepeatOptions::WingStyle>(e); }},
    {"afterClefSpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterClefSpace = e->getTextAs<Evpu>(); }},
    {"afterKeySpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterKeySpace = e->getTextAs<Evpu>(); }},
    {"afterTimeSpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterTimeSpace = e->getTextAs<Evpu>(); }},
    {"bracketHookLen", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHookLen = e->getTextAs<Evpu>(); }},
    {"bracketLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketLineWidth = e->getTextAs<Efix>(); }},
    {"bracketStartInset", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketStartInset = e->getTextAs<Evpu>(); }},
    {"bracketEndInset", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndInset = e->getTextAs<Evpu>(); }},
    {"bracketTextHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketTextHPos = e->getTextAs<Evpu>(); }},
    {"bracketTextVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketTextVPos = e->getTextAs<Evpu>(); }},
    {"bracketEndHookLen", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndHookLen = e->getTextAs<Evpu>(); }},
    {"bracketEndAnchorThinLine", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndAnchorThinLine = populateBoolean(e, i); }},
    {"showOnTopStaffOnly", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->showOnTopStaffOnly = populateBoolean(e, i); }},
    {"showOnStaffListNumber", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->showOnStaffListNumber = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions::ConnectionStyle, {
    {"connectIndex", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->connectIndex = toEnum<SmartShapeOptions::ConnectionIndex>(e); }},
    {"xOffset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"yOffset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->yOffset = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions::ControlStyle, {
    {"span", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->span = e->getTextAs<Efix>(); }},
    {"inset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->inset = e->getTextAs<Efix>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->height = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions, {
    {"shortHairpinOpeningWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->shortHairpinOpeningWidth = e->getTextAs<Evpu>(); }},
    {"crescHeight", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHeight = e->getTextAs<Evpu>(); }},
    {"maximumShortHairpinLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maximumShortHairpinLength = e->getTextAs<Evpu>(); }},
    {"crescLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescLineWidth = e->getTextAs<Efix>(); }},
    {"hookLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->hookLength = e->getTextAs<Evpu>(); }},
    {"smartLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartLineWidth = e->getTextAs<Efix>(); }},
    {"showOctavaAsText", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->showOctavaAsText = populateBoolean(e, i); }},
    {"smartDashOn", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOn = e->getTextAs<Evpu>(); }},
    {"smartDashOff", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOff = e->getTextAs<Evpu>(); }},
    {"crescHorizontal", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHorizontal = populateBoolean(e, i); }},
    {"direction", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->direction = toEnum<SmartShapeOptions::DefaultDirection>(e); }},
    {"slurThicknessCp1X", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp1Y", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1Y = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2X", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2Y", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2Y = e->getTextAs<Evpu>(); }},
    {"slurAvoidAccidentals", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidAccidentals = populateBoolean(e, i); }},
    {"slurAvoidStaffLinesAmt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLinesAmt = e->getTextAs<Evpu>(); }},
    {"maxSlurStretch", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretch = e->getTextAs<Efix>(); }},
    {"maxSlurLift", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurLift = e->getTextAs<Efix>(); }},
    {"slurSymmetry", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurSymmetry = e->getTextAs<int>(); }},
    {"useEngraverSlurs", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->useEngraverSlurs = populateBoolean(e, i); }},
    {"slurLeftBreakHorzAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurLeftBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurRightBreakHorzAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurRightBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurBreakVertAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurBreakVertAdj = e->getTextAs<Evpu>(); }},
    {"slurAvoidStaffLines", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLines = populateBoolean(e, i); }},
    {"slurPadding", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurPadding = e->getTextAs<Evpu>(); }},
    {"maxSlurAngle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurAngle = e->getTextAs<Efix>(); }},
    {"slurAcciPadding", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAcciPadding = e->getTextAs<Evpu>(); }},
    {"slurDoStretchFirst", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurDoStretchFirst = populateBoolean(e, i); }},
    {"slurStretchByPercent", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurStretchByPercent = populateBoolean(e, i); }},
    {"maxSlurStretchPercent", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretchPercent = e->getTextAs<int>(); }},
    {"articAvoidSlurAmt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->articAvoidSlurAmt = e->getTextAs<Evpu>(); }},
    {"ssLineStyleCmpCustom", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpCustom = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpGlissando", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpGlissando = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabSlide", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabSlide = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabBendCurve", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabBendCurve = e->getTextAs<Cmper>(); }},
    {"smartSlurTipWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartSlurTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"guitarBendUseParens", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseParens = populateBoolean(e, i); }},
    {"guitarBendHideBendTo", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendHideBendTo = populateBoolean(e, i); }},
    {"guitarBendGenText", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendGenText = populateBoolean(e, i); }},
    {"guitarBendUseFull", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseFull = populateBoolean(e, i); }},
    {"slurConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->slurConnectStyles); }},
    {"slurControlStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->slurControlStyles); }},
    {"tabSlideConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->tabSlideConnectStyles); }},
    {"glissandoConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->glissandoConnectStyles); }},
    {"bendCurveConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->bendCurveConnectStyles); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffOptions, {
    {"staffSeparation", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->staffSeparation = e->getTextAs<Evpu>(); }},
    {"staffSeparIncr", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->staffSeparIncr = e->getTextAs<Evpu>(); }},
    {"autoAdjustStaffSepar", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->autoAdjustStaffSepar = populateBoolean(e, i); }},
    {"namePos", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) 
        { i->namePos = FieldPopulator<others::NamePositioning>::createAndPopulate(e, i->getDocument()); }},
    {"namePosAbbrv", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) 
        { i->namePosAbbrv = FieldPopulator<others::NamePositioning>::createAndPopulate(e, i->getDocument()); }},
    {"groupNameFullPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) 
        { i->groupNameFullPos = FieldPopulator<others::NamePositioning>::createAndPopulate(e, i->getDocument()); }},
    {"groupNameAbbrvPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) 
        { i->groupNameAbbrvPos = FieldPopulator<others::NamePositioning>::createAndPopulate(e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(StemOptions::StemConnection, {
    {"font", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->fontId = e->getTextAs<Cmper>(); }},
    {"symbol", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->symbol = static_cast<char32_t>(e->getTextAs<uint32_t>()); }},
    {"upStemVert", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->upStemVert = e->getTextAs<Efix>(); }},
    {"downStemVert", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->downStemVert = e->getTextAs<Efix>(); }},
    {"upStemHorz", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->upStemHorz = e->getTextAs<Efix>(); }},
    {"downStemHorz", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->downStemHorz = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StemOptions, {
    {"halfStemLength", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->halfStemLength = e->getTextAs<Evpu>(); }},
    {"stemLength", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemLength = e->getTextAs<Evpu>(); }},
    {"stem2", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->shortStemLength = e->getTextAs<Evpu>(); }},
    {"revStemAdj", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->revStemAdj = e->getTextAs<Evpu>(); }},
    {"noReverseStems", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->noReverseStems = populateBoolean(e, i); }},
    {"stemWidth", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemWidth = e->getTextAs<Efix>(); }},
    {"stemLift", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemOffset = e->getTextAs<Efix>(); }},
    {"useStemConnections", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->useStemConnections = populateBoolean(e, i); }},
    {"stemConnect", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) {
            auto indexAttr = e->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : static_cast<size_t>(-1);
            if (i->stemConnections.size() > index) {
                throw std::invalid_argument("StemConnections index mismatch. Expected: " + std::to_string(i->stemConnections.size()) + ", Found: " + std::to_string(index));
            } else {
                for (size_t x = i->stemConnections.size(); x < index; x++) {
                    i->stemConnections.push_back(std::make_shared<StemOptions::StemConnection>());
                }
            }
            i->stemConnections.push_back(FieldPopulator<StemOptions::StemConnection>::createAndPopulate(e));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(TextOptions::InsertSymbolInfo, {
    {"trackingBefore",    [](const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->trackingBefore = e->getTextAs<int>(); }},
    {"trackingAfter",     [](const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->trackingAfter = e->getTextAs<int>(); }},
    {"baselineShiftPerc", [](const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->baselineShiftPerc = e->getTextAs<int>(); }},
    {"symFont",           [](const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i)
        { i->symFont = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument(), /*sizeIsPercent*/ true); }},
    {"symChar",           [](const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->symChar = e->getTextAs<char32_t>(); }}
    });

MUSX_XML_ELEMENT_ARRAY(TextOptions, {
    {"textLineSpacingPercent", [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textLineSpacingPercent = e->getTextAs<int>(); }},
    {"showTimeSeconds",        [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->showTimeSeconds = populateBoolean(e, i); }},
    {"dateFormat",             [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->dateFormat = toEnum<TextOptions::DateFormat>(e); }},
    {"tabSpaces",              [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->tabSpaces = e->getTextAs<int>(); }},
    {"textTracking",           [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textTracking = e->getTextAs<int>(); }},
    {"textBaselineShift",      [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textBaselineShift = e->getTextAs<Evpu>(); }},
    {"textSuperscript",        [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textSuperscript = e->getTextAs<Evpu>(); }},
    {"textWordWrap",           [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textWordWrap = populateBoolean(e, i); }},
    {"textPageOffset",         [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textPageOffset = e->getTextAs<Evpu>(); }},
    {"textJustify",            [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textJustify = toEnum<TextOptions::TextJustify>(e); }},
    {"textExpandSingleWord",   [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textExpandSingleWord = populateBoolean(e, i); }},
    {"textHorzAlign",          [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textHorzAlign = toEnum<TextOptions::HorizontalAlignment>(e); }},
    {"textVertAlign",          [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textVertAlign = toEnum<TextOptions::VerticalAlignment>(e); }},
    {"textIsEdgeAligned",      [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textIsEdgeAligned = populateBoolean(e, i); }},
    {"insertSymbolInfo",       [](const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { populateEmbeddedClass(e, i->symbolInserts, i); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ConnectStyle, {
    {"offsetX", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetX = e->getTextAs<Evpu>(); }},
    {"offsetY", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetY = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ControlPoint, {
    {"insetRatio", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetRatio = e->getTextAs<Efix>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"insetFixed", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetFixed = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ControlStyle, {
    {"span", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->span = e->getTextAs<Evpu>(); }},
    {"cp1", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp1 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(e); }},
    {"cp2", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp2 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(e); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions, {
    {"frontTieSepar", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->frontTieSepar = e->getTextAs<Evpu>(); }},
    {"thicknessRight", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessRight = e->getTextAs<Evpu>(); }},
    {"thicknessLeft", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessLeft = e->getTextAs<Evpu>(); }},
    {"breakForTimeSigs", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakForTimeSigs = populateBoolean(e, i); }},
    {"breakForKeySigs", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakForKeySigs = populateBoolean(e, i); }},
    {"breakTimeSigLeftHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakTimeSigRightHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigRightHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigLeftHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigRightHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigRightHOffset = e->getTextAs<Evpu>(); }},
    {"sysBreakLeftHAdj", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakLeftHAdj = e->getTextAs<Evpu>(); }},
    {"sysBreakRightHAdj", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakRightHAdj = e->getTextAs<Evpu>(); }},
    {"useOuterPlacement", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useOuterPlacement = populateBoolean(e, i); }},
    {"secondsPlacement", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->secondsPlacement = toEnum<TieOptions::SecondsPlacement>(e); }},
    {"chordTieDirType", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->chordTieDirType = toEnum<TieOptions::ChordTieDirType>(e); }},
    {"chordTieDirOpposingSeconds", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->chordTieDirOpposingSeconds = populateBoolean(e, i); }},
    {"mixedStemDirection", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->mixedStemDirection = toEnum<TieOptions::MixedStemDirection>(e); }},
    {"afterSingleDot", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->afterSingleDot = populateBoolean(e, i); }},
    {"afterMultipleDots", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->afterMultipleDots = populateBoolean(e, i); }},
    {"beforeAcciSingleNote", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->beforeAcciSingleNote = populateBoolean(e, i); }},
    {"specialPosMode", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->specialPosMode = toEnum<TieOptions::SpecialPosMode>(e); }},
    {"avoidStaffLinesDistance", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesDistance = e->getTextAs<Evpu>(); }},
    {"insetStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->insetStyle = toEnum<TieOptions::InsetStyle>(e); }},
    {"useInterpolation", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useInterpolation = populateBoolean(e, i); }},
    {"useTieEndCtlStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useTieEndCtlStyle = populateBoolean(e, i); }},
    {"avoidStaffLinesOnly", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesOnly = populateBoolean(e, i); }},
    {"tieTipWidth", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->tieTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"tieConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(e, i->tieConnectStyles); }},
    {"tieControlStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(e, i->tieControlStyles); }}
});

MUSX_XML_ELEMENT_ARRAY(TimeSignatureOptions, {
    {"timeUpperLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLift = e->getTextAs<Evpu>(); }},
    {"timeFront", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFront = e->getTextAs<Evpu>(); }},
    {"timeBack", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBack = e->getTextAs<Evpu>(); }},
    {"timeFrontParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFrontParts = e->getTextAs<Evpu>(); }},
    {"timeBackParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBackParts = e->getTextAs<Evpu>(); }},
    {"timeUpperLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLiftParts = e->getTextAs<Evpu>(); }},
    {"timeLowerLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLiftParts = e->getTextAs<Evpu>(); }},
    {"timeAbrvLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLiftParts = e->getTextAs<Evpu>(); }},
    {"timeSigDoAbrvCommon", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCommon = populateBoolean(e, i); }},
    {"timeSigDoAbrvCut", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCut = populateBoolean(e, i); }},
    {"defFloat", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->numCompositeDecimalPlaces = e->getTextAs<int>(); }},
    {"cautionaryTimeChanges", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->cautionaryTimeChanges = populateBoolean(e, i); }},
    {"timeLowerLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLift = e->getTextAs<Evpu>(); }},
    {"timeAbrvLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLift = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TupletOptions, {
    {"flat", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->alwaysFlat = populateBoolean(e, i); }},
    {"fullDura", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->fullDura = populateBoolean(e, i); }},
    {"metricCenter", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->metricCenter = populateBoolean(e, i); }},
    {"avoidStaff", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->avoidStaff = populateBoolean(e, i); }},
    {"autoBracketStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->autoBracketStyle = toEnum<TupletOptions::AutoBracketStyle>(e); }},
    {"tupOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupOffX = e->getTextAs<Evpu>(); }},
    {"tupOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupOffY = e->getTextAs<Evpu>(); }},
    {"brackOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackOffX = e->getTextAs<Evpu>(); }},
    {"brackOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackOffY = e->getTextAs<Evpu>(); }},
    {"numStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->numStyle = toEnum<TupletOptions::NumberStyle>(e); }},
    {"posStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->posStyle = toEnum<TupletOptions::PositioningStyle>(e); }},
    {"allowHorz", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->allowHorz = populateBoolean(e, i); }},
    {"ignoreGlOffs", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->ignoreHorzNumOffset = populateBoolean(e, i); }},
    {"breakBracket", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->breakBracket = populateBoolean(e, i); }},
    {"matchHooks", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->matchHooks = populateBoolean(e, i); }},
    {"noteBelow", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->useBottomNote = populateBoolean(e, i); }},
    {"brackStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackStyle = toEnum<TupletOptions::BracketStyle>(e); }},
    {"smartTuplet", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->smartTuplet = populateBoolean(e, i); }},
    {"leftHookLen", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->leftHookLen = e->getTextAs<Evpu>(); }},
    {"leftHookExt", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->leftHookExt = e->getTextAs<Evpu>(); }},
    {"rightHookLen", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->rightHookLen = e->getTextAs<Evpu>(); }},
    {"rightHookExt", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->rightHookExt = e->getTextAs<Evpu>(); }},
    {"slope", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->manualSlopeAdj = e->getTextAs<Evpu>(); }},
    {"tupMaxSlope", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupMaxSlope = e->getTextAs<int>(); }},
    {"tupLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupLineWidth = e->getTextAs<Efix>(); }},
    {"tupNUpstemOffset", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupNUpstemOffset = e->getTextAs<Evpu>(); }},
    {"tupNDownstemOffset", [](const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupNDownstemOffset = e->getTextAs<Evpu>(); }},
});

} // namespace options
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS
