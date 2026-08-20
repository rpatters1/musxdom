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
#include "musx/factory/FieldPopulatorsOptions.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::options;

extern template const XmlEnumMappingElement<AlignJustify> XmlEnumMapping<AlignJustify>::mapping;
extern template const XmlEnumMappingElement<DateFormat> XmlEnumMapping<DateFormat>::mapping;

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

MUSX_XML_ENUM_MAPPING(TieConnectStyleType, {
    {"overStartPosInner", TieConnectStyleType::OverStartPosInner},
    {"overEndPosInner", TieConnectStyleType::OverEndPosInner},
    {"underStartPosInner", TieConnectStyleType::UnderStartPosInner},
    {"underEndPosInner", TieConnectStyleType::UnderEndPosInner},
    {"overHighestNoteStartPosOver", TieConnectStyleType::OverHighestNoteStartPosOver},
    {"overHighestNoteEndPosOver", TieConnectStyleType::OverHighestNoteEndPosOver},
    {"underLowestNoteStartPosUnder", TieConnectStyleType::UnderLowestNoteStartPosUnder},
    {"underLowestNoteEndPosUnder", TieConnectStyleType::UnderLowestNoteEndPosUnder},
    {"overHighestNoteStemStartPosOver", TieConnectStyleType::OverHighestNoteStemStartPosOver},
    {"overHighestNoteStemEndPosOver", TieConnectStyleType::OverHighestNoteStemEndPosOver},
    {"underLowestNoteStemStartPosUnder", TieConnectStyleType::UnderLowestNoteStemStartPosUnder},
    {"underLowestNoteStemEndPosUnder", TieConnectStyleType::UnderLowestNoteStemEndPosUnder}
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
    {"overlap", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->minOverlap = e->getTextAs<int>(); }},
    {"bacciAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->multiCharSpace = e->getTextAs<Evpu>(); }},
    {"useNewAcciPositioning", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->crossLayerPositioning = populateBoolean(c, e, i); }},
    {"frontAcciSepar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->startMeasureSepar = e->getTextAs<Evpu>(); }},
    {"acciNoteSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciNoteSpace = e->getTextAs<Evpu>(); }},
    {"acciAcciSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciAcciSpace = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(AlternateNotationOptions, {
    {"halfSlashLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashLift = e->getTextAs<Evpu>(); }},
    {"wholeSlashLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->wholeSlashLift = e->getTextAs<Evpu>(); }},
    {"dWholeSlashLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->dWholeSlashLift = e->getTextAs<Evpu>(); }},
    {"halfSlashStemLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashStemLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashLift = e->getTextAs<Evpu>(); }},
    {"twoMeasNumLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->twoMeasNumLift = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(AugmentationDotOptions, {
    {"dotUpFlagOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotUpFlagOffset = e->getTextAs<Evpu>(); }},
    {"dotOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotOffset = e->getTextAs<Evpu>(); }},
    {"doDotDownAdjust", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->adjMultipleVoices = populateBoolean(c, e, i); }},
    {"dotFirstOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotNoteOffset = e->getTextAs<Evpu>(); }},
    {"dotLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotLift = e->getTextAs<Evpu>(); }},
    {"skip27_4DotAdjust", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->useLegacyFlippedStemPositioning = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BarlineOptions, {
    {"drawCloseSystemBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseSystemBarline = populateBoolean(c, e, i); }},
    {"drawCloseFinalBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseFinalBarline = populateBoolean(c, e, i); }},
    {"drawFinalBarlineOnLastMeas", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawFinalBarlineOnLastMeas = populateBoolean(c, e, i); }},
    {"leftBarlineUsePrevStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->leftBarlineUsePrevStyle = populateBoolean(c, e, i); }},
    {"drawLeftBarlineSingleStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineSingleStaff = populateBoolean(c, e, i); }},
    {"drawLeftBarlineMultipleStaves", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineMultipleStaves = populateBoolean(c, e, i); }},
    {"drawBarlines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawBarlines = populateBoolean(c, e, i); }},
    {"barlineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineWidth = e->getTextAs<Efix>(); }},
    {"thickBarlineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->thickBarlineWidth = e->getTextAs<Efix>(); }},
    {"doubleBarlineSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->doubleBarlineSpace = e->getTextAs<Efix>(); }},
    {"finalBarlineSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->finalBarlineSpace = e->getTextAs<Efix>(); }},
    {"barlineDashOn", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOn = e->getTextAs<Evpu>(); }},
    {"barlineDashOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOff = e->getTextAs<Evpu>(); }},
    {"drawDoubleBarlineBeforeKeyChanges", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->drawDoubleBarlineBeforeKeyChanges = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamOptions, {
    {"beamStubLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamStubLength = e->getTextAs<Evpu>(); }},
    {"maxSlope", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxSlope = e->getTextAs<Evpu>(); }},
    {"beamSepar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamSepar = e->getTextAs<Evpu>(); }},
    {"hmBeamTip", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxFromMiddle = e->getTextAs<Evpu>(); }},
    {"beamingStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamingStyle = toEnum<BeamOptions::FlattenStyle>(e); }},
    {"incEdgeRestsInBeamGroups", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->extendBeamsOverRests = populateBoolean(c, e, i); }},
    {"incRestsInClassicBeams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->incRestsInFourGroups = populateBoolean(c, e, i); }},
    {"beamFourEighthsInCommonTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamFourEighthsInCommonTime = populateBoolean(c, e, i); }},
    {"beamThreeEighthsInCommonTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamThreeEighthsInCommonTime = populateBoolean(c, e, i); }},
    {"doStemStubs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->dispHalfStemsOnRests = populateBoolean(c, e, i); }},
    {"oldFinaleRestBeams", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->oldFinaleRestBeams = populateBoolean(c, e, i); }},
    {"spanSpace", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->spanSpace = populateBoolean(c, e, i); }},
    {"extendSecBeamsOverRests", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->extendSecBeamsOverRests = populateBoolean(c, e, i); }},
    {"beamWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordOptions, {
    {"fretPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretPercent = e->getTextAs<double>(); }},
    {"chordPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordPercent = e->getTextAs<double>(); }},
    {"chordSharpLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordSharpLift = e->getTextAs<Evpu>(); }},
    {"chordFlatLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordFlatLift = e->getTextAs<Evpu>(); }},
    {"chordNaturalLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordNaturalLift = e->getTextAs<Evpu>(); }},
    {"showFretboards", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->showFretboards = populateBoolean(c, e, i); }},
    {"fretStyleID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretStyleId = e->getTextAs<Cmper>(); }},
    {"fretInstID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->fretInstId = e->getTextAs<Cmper>(); }},
    {"multiFretItemsPerStr", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->multiFretItemsPerStr = populateBoolean(c, e, i); }},
    {"italicizeCapoChords", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->italicizeCapoChords = populateBoolean(c, e, i); }},
    {"chordAlignment", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordAlignment = toEnum<ChordOptions::ChordAlignment>(e); }},
    {"chordStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordStyle = toEnum<ChordOptions::ChordStyle>(e); }},
    {"useSimpleChordSpelling", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->useSimpleChordSpelling = populateBoolean(c, e, i); }},
    {"chordPlayback", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordOptions>& i) { i->chordPlayback = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOptions::ClefDef, {
    {"adjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->middleCPos = e->getTextAs<int>(); }},
    {"clefChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->clefChar = e->getTextAs<char32_t>(); }},
    {"clefYDisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->staffPosition = e->getTextAs<int>(); }},
    {"baseAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->baselineAdjust = e->getTextAs<Efix>(); }},
    {"shapeID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->shapeId = e->getTextAs<Cmper>(); }},
    {"isShape", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->isShape = populateBoolean(c, e, i); }},
    {"scaleToStaffHeight", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->scaleToStaffHeight = populateBoolean(c, e, i); }},
    {"font", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->font = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"useOwnFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->useOwnFont = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOptions, {
    {"defaultClef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->defaultClef = e->getTextAs<ClefIndex>(); }},
    {"endMeasClefPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangePercent = e->getTextAs<int>(); }},
    {"endMeasClefPosAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangeOffset = e->getTextAs<Evpu>(); }},
    {"clefFront", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefFrontSepar = e->getTextAs<Evpu>(); }},
    {"clefBack", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefBackSepar = e->getTextAs<Evpu>(); }},
    {"showClefFirstSystemOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->showClefFirstSystemOnly = populateBoolean(c, e, i); }},
    {"clefKey", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefKeySepar = e->getTextAs<Evpu>(); }},
    {"clefTime", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefTimeSepar = e->getTextAs<Evpu>(); }},
    {"cautionaryClefChanges", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->cautionaryClefChanges = populateBoolean(c, e, i); }},
    {"clefDef", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) {
            auto indexAttr = e->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : static_cast<size_t>(-1);;
            if (i->clefDefs.size() != index) {
                throw std::invalid_argument("ClefDef index mismatch. Expected: " + std::to_string(i->clefDefs.size()) + ", Found: " + std::to_string(index));
            }
            i->clefDefs.push_back(FieldPopulator<ClefOptions::ClefDef>::createAndPopulate(c, e, i));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(FlagOptions, {
    {"straightFlags", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->straightFlags = populateBoolean(c, e, i); }},
    {"upHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj = e->getTextAs<Efix>(); }},
    {"downHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj = e->getTextAs<Efix>(); }},
    {"upHAdj2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj2 = e->getTextAs<Efix>(); }},
    {"downHAdj2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj2 = e->getTextAs<Efix>(); }},
    {"upHAdj16", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upHAdj16 = e->getTextAs<Efix>(); }},
    {"downHAdj16", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downHAdj16 = e->getTextAs<Efix>(); }},
    {"eighthFlagHoist", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->eighthFlagHoist = e->getTextAs<Efix>(); }},
    {"stUpHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stUpHAdj = e->getTextAs<Efix>(); }},
    {"stDownHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stDownHAdj = e->getTextAs<Efix>(); }},
    {"upVAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj = e->getTextAs<Efix>(); }},
    {"downVAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj = e->getTextAs<Efix>(); }},
    {"upVadj2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj2 = e->getTextAs<Efix>(); }}, // "upVadj2" is intentionally preserved as is
    {"downVAdj2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj2 = e->getTextAs<Efix>(); }},
    {"upVAdj16", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->upVAdj16 = e->getTextAs<Efix>(); }},
    {"downVAdj16", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->downVAdj16 = e->getTextAs<Efix>(); }},
    {"stUpVAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stUpVAdj = e->getTextAs<Efix>(); }},
    {"stDownVAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->stDownVAdj = e->getTextAs<Efix>(); }},
    {"separ", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->flagSpacing = e->getTextAs<Evpu>(); }},
    {"extra", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<FlagOptions>& i) { i->secondaryGroupAdj = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FontOptions, {
    {
        "font", [](ConstructionContext& c, const XmlElementPtr& fontElement, const std::shared_ptr<FontOptions>& fonts) {
            auto typeStr = fontElement->findAttribute("type");
            if (!typeStr) {
                throw std::invalid_argument("font option has no type");
            }
            auto fontType = toEnum<FontOptions::FontType>(typeStr->getValue());

            auto fontInfo = std::make_shared<dom::FontInfo>(fonts->getDocument());
            FieldPopulator<dom::FontInfo>::populate(c, fontInfo, fontElement);

            // Add the populated font instance to the vector.
            fonts->fontOptions.emplace(fontType, fontInfo);
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(GraceNoteOptions, {
    {"tabGracePerc", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->tabGracePerc = e->getTextAs<int>(); }},
    {"gracePerc", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->gracePerc = e->getTextAs<int>(); }},
    {"playbackDuration", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->playbackDuration = e->getTextAs<Edu>(); }},
    {"graceBackup", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->entryOffset = e->getTextAs<Evpu>(); }},
    {"slashFlaggedGraceNotes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->slashFlaggedGraceNotes = populateBoolean(c, e, i); }},
    {"graceSlashWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->graceSlashWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(KeySignatureOptions, {
    {"doKeyCancel", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancel = populateBoolean(c, e, i); }},
    {"doCStart", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doCStart = populateBoolean(c, e, i); }},
    {"doBankDiff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->redisplayOnModeChange = populateBoolean(c, e, i); }},
    {"keyFront", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyFront = e->getTextAs<Evpu>(); }},
    {"keyMid", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyMid = e->getTextAs<Evpu>(); }},
    {"keyBack", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyBack = e->getTextAs<Evpu>(); }},
    {"acciAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->acciAdd = e->getTextAs<Evpu>(); }},
    {"showKeyFirstSystemOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->showKeyFirstSystemOnly = populateBoolean(c, e, i); }},
    {"keyTime", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyTimeSepar = e->getTextAs<Evpu>(); }},
    {"simplifyKeyHoldOctave", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->simplifyKeyHoldOctave = populateBoolean(c, e, i); }},
    {"cautionaryKeyChanges", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->cautionaryKeyChanges = populateBoolean(c, e, i); }},
    {"doKeyCancelBetweenSharpsFlats", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancelBetweenSharpsFlats = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LineCurveOptions, {
    {"bezierStep", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->bezierStep = e->getTextAs<int>(); }},
    {"enclosureWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureWidth = e->getTextAs<Efix>(); }},
    {"enclosureRoundCorners", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureRoundCorners = populateBoolean(c, e, i); }},
    {"enclosureCornerRadius", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureCornerRadius = e->getTextAs<Efix>(); }},
    {"staffLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->staffLineWidth = e->getTextAs<Efix>(); }},
    {"legerLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerLineWidth = e->getTextAs<Efix>(); }},
    {"legerFrontLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerFrontLength = e->getTextAs<Evpu>(); }},
    {"legerBackLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->legerBackLength = e->getTextAs<Evpu>(); }},
    {"restLegerFrontLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->restLegerFrontLength = e->getTextAs<Evpu>(); }},
    {"restLegerBackLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->restLegerBackLength = e->getTextAs<Evpu>(); }},
    {"psUlDepth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->psUlDepth = e->getTextAs<double>(); }},
    {"psUlWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->psUlWidth = e->getTextAs<double>(); }},
    {"pathSlurTipWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->pathSlurTipWidth = e->getTextAs<EvpuFloat>(); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions::SyllablePosStyle, {
    {"align",   [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->align = toEnum<AlignJustify>(e); }},
    {"justify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->justify = toEnum<AlignJustify>(e); }},
    {"on",      [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::SyllablePosStyle>& i) { i->on = populateBoolean(c, e, i); }}
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions::WordExtConnectStyle, {
    {"connectIndex", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->connectIndex = toEnum<LyricOptions::WordExtConnectIndex>(e); }},
    {"xOffset",      [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"yOffset",      [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions::WordExtConnectStyle>& i) { i->yOffset = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(LyricOptions, {
    {"hyphenChar",              [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->hyphenChar = e->getTextAs<char32_t>(); }},
    {"maxHyphenSeparation",     [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->maxHyphenSeparation = e->getTextAs<Evpu>(); }},
    {"wordExtVertOffset",       [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtVertOffset = e->getTextAs<Evpu>(); }},
    {"wordExtHorzOffset",       [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtHorzOffset = e->getTextAs<Evpu>(); }},
    {"useSmartWordExtensions",  [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useSmartWordExtensions = populateBoolean(c, e, i); }},
    {"useAltHyphenFont",        [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useAltHyphenFont = populateBoolean(c, e, i); }},
    {"altHyphenFont",           [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->altHyphenFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument()); }},
    {"useSmartHyphens",         [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->useSmartHyphens = populateBoolean(c, e, i); }},
    {"smartHyphenStart",        [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->smartHyphenStart = toEnum<LyricOptions::SmartHyphenStart>(e); }},
    {"wordExtNeedUnderscore",   [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtNeedUnderscore = populateBoolean(c, e, i); }},
    {"wordExtMinLength",        [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtMinLength = e->getTextAs<Evpu>(); }},
    {"wordExtOffsetToNotehead", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtOffsetToNotehead = populateBoolean(c, e, i); }},
    {"lyricUseEdgePunctuation", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricUseEdgePunctuation = populateBoolean(c, e, i); }},
    {"showAutoNumbersOnVerses", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->showAutoNumbersOnVerses = populateBoolean(c, e, i); }},
    {"showAutoNumbersOnChoruses", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->showAutoNumbersOnChoruses = populateBoolean(c, e, i); }},
    {"showAutoNumbersOnSections", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->showAutoNumbersOnSections = populateBoolean(c, e, i); }},
    {"lyricPunctuationToIgnore",[](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricPunctuationToIgnore = e->getText(); }},
    {"lyricAutoNumType",        [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->lyricAutoNumType = toEnum<LyricOptions::AutoNumberingAlign>(e); }},
    {"wordExtLineWidth",        [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { i->wordExtLineWidth = e->getTextAs<Efix>(); }},
    {"lyricSyllPosStyle",       [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { populateEmbeddedClass(c, e, i->syllablePosStyles); }},
    {"wordExtConnectStyle",     [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricOptions>& i) { populateEmbeddedClass(c, e, i->wordExtConnectStyles); }}
});

MUSX_XML_ELEMENT_ARRAY(MiscOptions, {
    {"showRepeatsForParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->showRepeatsForParts = populateBoolean(c, e, i); }},
    {"pickupValue", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->pickupValue = e->getTextAs<Edu>();}},
    {"retainOctaveTransInConcertPitch", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->keepWrittenOctaveInConcertPitch = populateBoolean(c, e, i); }},
    {"showCurrentLayerOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->showActiveLayerOnly = populateBoolean(c, e, i); }},
    {"combineRestsAcrossLayers", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->consolidateRestsAcrossLayers = populateBoolean(c, e, i); }},
    {"sdDashOn", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashLength = e->getTextAs<Evpu>();}},
    {"sdDashOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashSpace = e->getTextAs<Evpu>();}},
    {"restWidthAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->restWidthAdjust = e->getTextAs<Evpu>();}},
    {"dblWholeVertAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->dblWholeVertAdjust = e->getTextAs<Evpu>();}},
    {"drawMeasureNumbersOverBarlines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) { i->alignMeasureNumbersWithBarlines = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MultimeasureRestOptions, {
    {"meaSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->measWidth = e->getTextAs<Evpu>();}},
    {"numdec", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjY = e->getTextAs<Evpu>();}},
    {"shapeDef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->shapeDef = e->getTextAs<Cmper>();}},
    {"numStart", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numStart = e->getTextAs<int>();}},
    {"threshold", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->useSymsThreshold = e->getTextAs<int>();}},
    {"spacing", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->symSpacing = e->getTextAs<Evpu>();}},
    {"numAdjX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjX = e->getTextAs<Evpu>();}},
    {"startAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->startAdjust = e->getTextAs<Evpu>();}},
    {"endAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->endAdjust = e->getTextAs<Evpu>();}},
    {"useCharRestStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) { i->useSymbols = populateBoolean(c, e, i); }},
    {"noHorizontalStretch", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) { i->noHorizontalStretch = populateBoolean(c, e, i); }},
    {"autoUpdateMmRests", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) { i->autoUpdateMmRests = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MusicSpacingOptions, {
    {"layoutType", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>&) { /* appears to be cruft left over from legacy Finale */ }},
    {"minWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minWidth = e->getTextAs<Evpu>(); }},
    {"maxWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->maxWidth = e->getTextAs<Evpu>(); }},
    {"minDistance", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistance = e->getTextAs<Evpu>(); }},
    {"minDistTiedNotes", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistTiedNotes = e->getTextAs<Evpu>(); }},
    {"avoidColNotes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColNotes = populateBoolean(c, e, i); }},
    {"avoidColLyrics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLyrics = populateBoolean(c, e, i); }},
    {"avoidColChords", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColChords = populateBoolean(c, e, i); }},
    {"avoidColArtics", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColArtics = populateBoolean(c, e, i); }},
    {"avoidColClefs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColClefs = populateBoolean(c, e, i); }},
    {"avoidColSeconds", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColSeconds = populateBoolean(c, e, i); }},
    {"avoidColStems", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColStems = populateBoolean(c, e, i); }},
    {"avoidColUnisons", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColUnisons = toEnum<MusicSpacingOptions::ColUnisonsChoice>(e); }},
    {"avoidColLedgers", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLedgers = populateBoolean(c, e, i); }},
    {"manualPositioning", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->manualPositioning = toEnum<MusicSpacingOptions::ManualPositioning>(e); }},
    {"ignoreHidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->ignoreHidden = populateBoolean(c, e, i); }},
    {"interpolateAllotments", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->interpolateAllotments = populateBoolean(c, e, i); }},
    {"usePrinter", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->usePrinter = populateBoolean(c, e, i); }},
    {"useAllottmentTables", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->useAllottmentTables = populateBoolean(c, e, i); }},
    {"referenceDuration", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"referenceWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceWidth = e->getTextAs<Evpu>(); }},
    {"scalingFactor", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->scalingFactor = e->getTextAs<double>(); }},
    {"defaultAllotment", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->defaultAllotment = e->getTextAs<Evpu>(); }},
    {"minDistGrace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistGrace = e->getTextAs<Evpu>(); }},
    {"graceNoteManualPositioning", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->graceNoteSpacing = toEnum<MusicSpacingOptions::GraceNoteSpacing>(e); }},
    {"musFront", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musFront = e->getTextAs<Evpu>(); }},
    {"musBack", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musBack = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(MusicSymbolOptions, {
    {"noteheadQuarter", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadQuarter = e->getTextAs<char32_t>(); }},
    {"noteheadHalf", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadHalf = e->getTextAs<char32_t>(); }},
    {"noteheadWhole", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadWhole = e->getTextAs<char32_t>(); }},
    {"noteheadDblWhole", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->noteheadDblWhole = e->getTextAs<char32_t>(); }},
    {"natural", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->natural = e->getTextAs<char32_t>(); }},
    {"flat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flat = e->getTextAs<char32_t>(); }},
    {"sharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->sharp = e->getTextAs<char32_t>(); }},
    {"dblFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblFlat = e->getTextAs<char32_t>(); }},
    {"dblSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblSharp = e->getTextAs<char32_t>(); }},
    {"parenNatural", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenNatural = e->getTextAs<char32_t>(); }},
    {"parenFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenFlat = e->getTextAs<char32_t>(); }},
    {"parenSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenSharp = e->getTextAs<char32_t>(); }},
    {"parenDblFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenDblFlat = e->getTextAs<char32_t>(); }},
    {"parenDblSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->parenDblSharp = e->getTextAs<char32_t>(); }},
    {"chordNatural", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordNatural = e->getTextAs<char32_t>(); }},
    {"chordFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordFlat = e->getTextAs<char32_t>(); }},
    {"chordSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordSharp = e->getTextAs<char32_t>(); }},
    {"chordDblFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordDblFlat = e->getTextAs<char32_t>(); }},
    {"chordDblSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->chordDblSharp = e->getTextAs<char32_t>(); }},
    {"keySigNatural", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigNatural = e->getTextAs<char32_t>(); }},
    {"keySigFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigFlat = e->getTextAs<char32_t>(); }},
    {"keySigSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigSharp = e->getTextAs<char32_t>(); }},
    {"keySigDblFlat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigDblFlat = e->getTextAs<char32_t>(); }},
    {"keySigDblSharp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->keySigDblSharp = e->getTextAs<char32_t>(); }},
    {"restLonga", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restLonga = e->getTextAs<char32_t>(); }},
    {"restDblWhole", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restDblWhole = e->getTextAs<char32_t>(); }},
    {"restWhole", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restWhole = e->getTextAs<char32_t>(); }},
    {"restHalf", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restHalf = e->getTextAs<char32_t>(); }},
    {"restQuarter", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restQuarter = e->getTextAs<char32_t>(); }},
    {"restEighth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restEighth = e->getTextAs<char32_t>(); }},
    {"rest16th", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest16th = e->getTextAs<char32_t>(); }},
    {"rest32nd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest32nd = e->getTextAs<char32_t>(); }},
    {"rest64th", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest64th = e->getTextAs<char32_t>(); }},
    {"rest128th", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->rest128th = e->getTextAs<char32_t>(); }},
    {"restDefMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->restDefMeas = e->getTextAs<char32_t>(); }},
    {"oneBarRepeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->oneBarRepeat = e->getTextAs<char32_t>(); }},
    {"twoBarRepeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->twoBarRepeat = e->getTextAs<char32_t>(); }},
    {"slashBar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->slashBar = e->getTextAs<char32_t>(); }},
    {"quarterSlash", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->quarterSlash = e->getTextAs<char32_t>(); }},
    {"halfSlash", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->halfSlash = e->getTextAs<char32_t>(); }},
    {"wholeSlash", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->wholeSlash = e->getTextAs<char32_t>(); }},
    {"dblWholeSlash", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->dblWholeSlash = e->getTextAs<char32_t>(); }},
    {"timeSigPlus", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigPlus = e->getTextAs<char32_t>(); }},
    {"timeSigPlusParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigPlusParts = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCommon", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCommon = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCut", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCut = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCommonParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCommonParts = e->getTextAs<char32_t>(); }},
    {"timeSigAbrvCutParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->timeSigAbrvCutParts = e->getTextAs<char32_t>(); }},
    {"augDot", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->augDot = e->getTextAs<char32_t>(); }},
    {"forwardRepeatDot", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->forwardRepeatDot = e->getTextAs<char32_t>(); }},
    {"backRepeatDot", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->backRepeatDot = e->getTextAs<char32_t>(); }},
    {"eightVaUp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->eightVaUp = e->getTextAs<char32_t>(); }},
    {"eightVbDown", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->eightVbDown = e->getTextAs<char32_t>(); }},
    {"fifteenMaUp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->fifteenMaUp = e->getTextAs<char32_t>(); }},
    {"fifteenMbDown", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->fifteenMbDown = e->getTextAs<char32_t>(); }},
    {"trillChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->trillChar = e->getTextAs<char32_t>(); }},
    {"wiggleChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->wiggleChar = e->getTextAs<char32_t>(); }},
    {"flagUp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagUp = e->getTextAs<char32_t>(); }},
    {"flagDown", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagDown = e->getTextAs<char32_t>(); }},
    {"flag16Up", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag16Up = e->getTextAs<char32_t>(); }},
    {"flag16Down", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag16Down = e->getTextAs<char32_t>(); }},
    {"flag2Up", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag2Up = e->getTextAs<char32_t>(); }},
    {"flag2Down", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flag2Down = e->getTextAs<char32_t>(); }},
    {"flagStraightUp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagStraightUp = e->getTextAs<char32_t>(); }},
    {"flagStraightDown", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MusicSymbolOptions>& i) { i->flagStraightDown = e->getTextAs<char32_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteRestOptions::NoteColor, {
    {"red", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->red = e->getTextAs<uint16_t>(); }},
    {"green", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->green = e->getTextAs<uint16_t>(); }},
    {"blue", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions::NoteColor>& i) { i->blue = e->getTextAs<uint16_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteRestOptions, {
    {"doShapeNotes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->doShapeNotes = populateBoolean(c, e, i); }},
    {"doCrossOver", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->doCrossStaffNotes = populateBoolean(c, e, i); }},
    {"drop8thRest", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop8thRest = e->getTextAs<Evpu>(); }},
    {"drop16thRest", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop16thRest = e->getTextAs<Evpu>(); }},
    {"drop32ndRest", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop32ndRest = e->getTextAs<Evpu>(); }},
    {"drop64thRest", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop64thRest = e->getTextAs<Evpu>(); }},
    {"drop128thRest", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drop128thRest = e->getTextAs<Evpu>(); }},
    {"scaleManualPositioning", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->scaleManualPositioning = populateBoolean(c, e, i); }},
    {"drawOutline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) { i->drawOutline = populateBoolean(c, e, i); }},
    {"noteColor", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteRestOptions>& i) {
            auto idAttr = e->findAttribute("id");
            size_t id = idAttr ? idAttr->getValueAs<size_t>() : static_cast<size_t>(-1);
            if (i->noteColors.size() != id) {
                throw std::invalid_argument("NoteColor id mismatch. Expected: " + std::to_string(i->noteColors.size())
                                            + ", Found: " + std::to_string(id));
            }
            i->noteColors.push_back(FieldPopulator<NoteRestOptions::NoteColor>::createAndPopulate(c, e));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(PageFormatOptions::PageFormat, {
    {"pageHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageHeight = e->getTextAs<Evpu>(); }},
    {"pageWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageWidth = e->getTextAs<Evpu>(); }},
    {"pagePercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pagePercent = e->getTextAs<int>(); }},
    {"sysPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysPercent = e->getTextAs<int>(); }},
    {"rawStaffHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rawStaffHeight = e->getTextAs<Evpu16ths>(); }},
    {"leftPageMarginTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginTop = e->getTextAs<Evpu>(); }},
    {"leftPageMarginLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginLeft = e->getTextAs<Evpu>(); }},
    {"leftPageMarginBottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginBottom = e->getTextAs<Evpu>(); }},
    {"leftPageMarginRight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->leftPageMarginRight = e->getTextAs<Evpu>(); }},
    {"rightPageMarginTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginTop = e->getTextAs<Evpu>(); }},
    {"rightPageMarginLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginLeft = e->getTextAs<Evpu>(); }},
    {"rightPageMarginBottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginBottom = e->getTextAs<Evpu>(); }},
    {"rightPageMarginRight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rightPageMarginRight = e->getTextAs<Evpu>(); }},
    {"sysMarginTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginTop = e->getTextAs<Evpu>(); }},
    {"sysMarginLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginLeft = e->getTextAs<Evpu>(); }},
    {"sysMarginBottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginBottom = e->getTextAs<Evpu>(); }},
    {"sysMarginRight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysMarginRight = e->getTextAs<Evpu>(); }},
    {"sysDistanceBetween", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysDistanceBetween = e->getTextAs<Evpu>(); }},
    {"firstPageMarginTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstPageMarginTop = e->getTextAs<Evpu>(); }},
    {"firstSysMarginTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginTop = e->getTextAs<Evpu>(); }},
    {"firstSysMarginLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginLeft = e->getTextAs<Evpu>(); }},
    {"firstSysMarginDistance", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->firstSysMarginDistance = e->getTextAs<Evpu>(); }},
    {"facingPages", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->facingPages = populateBoolean(c, e, i); }},
    {"differentFirstSysMargin", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstSysMargin = populateBoolean(c, e, i); }},
    {"differentFirstPageMargin", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstPageMargin = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(PageFormatOptions, {
    {"pageFormatScore", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i){ i->pageFormatScore = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(c, e); }},
    {"pageFormatParts", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->pageFormatParts = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(c, e); }},
    {"avoidSystemMarginCollisions", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->avoidSystemMarginCollisions = populateBoolean(c, e, i); }},
    {"adjustPageScope", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->adjustPageScope = toEnum<PageFormatOptions::AdjustPageScope>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(PianoBraceBracketOptions, {
    {"defBracketPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->defBracketPos = e->getTextAs<Evpu>(); }},
    {"centerThickness", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->centerThickness = e->getTextAs<EvpuFloat>(); }},
    {"endThickness", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->tipThickness = e->getTextAs<EvpuFloat>(); }},
    {"outerWingY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerBodyV = e->getTextAs<EvpuFloat>(); }},
    {"innerTipY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerTipV = e->getTextAs<EvpuFloat>(); }},
    {"innerWingY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerBodyV = e->getTextAs<EvpuFloat>(); }},
    {"outerTipX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerTipH = e->getTextAs<EvpuFloat>(); }},
    {"outerTipY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerTipV = e->getTextAs<EvpuFloat>(); }},
    {"outerWingX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->outerBodyH = e->getTextAs<EvpuFloat>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->width = e->getTextAs<EvpuFloat>(); }},
    {"innerTipX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerTipH = e->getTextAs<EvpuFloat>(); }},
    {"innerWingX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PianoBraceBracketOptions>& i) { i->innerBodyH = e->getTextAs<EvpuFloat>(); }},
});

MUSX_XML_ELEMENT_ARRAY(RepeatOptions, {
    {"bracketHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHeight = e->getTextAs<Evpu>(); }},
    {"maxPasses", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->maxPasses = e->getTextAs<int>(); }},
    {"addPeriod", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->addPeriod = populateBoolean(c, e, i); }},
    {"thickLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thickLineWidth = e->getTextAs<Efix>(); }},
    {"thinLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thinLineWidth = e->getTextAs<Efix>(); }},
    {"lineSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lineSpace = e->getTextAs<Efix>(); }},
    {"backToBackStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backToBackStyle = toEnum<RepeatOptions::BackToBackStyle>(e); }},
    {"forwardDotHPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->forwardDotHPos = e->getTextAs<Evpu>(); }},
    {"backwardDotHPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backwardDotHPos = e->getTextAs<Evpu>(); }},
    {"upperDotVPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->upperDotVPos = e->getTextAs<Evpu>(); }},
    {"lowerDotVPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lowerDotVPos = e->getTextAs<Evpu>(); }},
    {"wingStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->wingStyle = toEnum<RepeatOptions::WingStyle>(e); }},
    {"afterClefSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterClefSpace = e->getTextAs<Evpu>(); }},
    {"afterKeySpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterKeySpace = e->getTextAs<Evpu>(); }},
    {"afterTimeSpace", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->afterTimeSpace = e->getTextAs<Evpu>(); }},
    {"bracketHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHookLen = e->getTextAs<Evpu>(); }},
    {"bracketLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketLineWidth = e->getTextAs<Efix>(); }},
    {"bracketStartInset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketStartInset = e->getTextAs<Evpu>(); }},
    {"bracketEndInset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndInset = e->getTextAs<Evpu>(); }},
    {"bracketTextHPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketTextHPos = e->getTextAs<Evpu>(); }},
    {"bracketTextVPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketTextVPos = e->getTextAs<Evpu>(); }},
    {"bracketEndHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndHookLen = e->getTextAs<Evpu>(); }},
    {"bracketEndAnchorThinLine", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndAnchorThinLine = populateBoolean(c, e, i); }},
    {"showOnTopStaffOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->showOnTopStaffOnly = populateBoolean(c, e, i); }},
    {"showOnStaffListNumber", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->showOnStaffListNumber = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions::ConnectionStyle, {
    {"connectIndex", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->connectIndex = toEnum<SmartShapeOptions::ConnectionIndex>(e); }},
    {"xOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"yOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->yOffset = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions::ControlStyle, {
    {"span", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->span = e->getTextAs<Efix>(); }},
    {"inset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->inset = e->getTextAs<Efix>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->height = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeOptions, {
    {"shortHairpinOpeningWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->shortHairpinOpeningWidth = e->getTextAs<Evpu>(); }},
    {"crescHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHeight = e->getTextAs<Evpu>(); }},
    {"maximumShortHairpinLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maximumShortHairpinLength = e->getTextAs<Evpu>(); }},
    {"crescLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescLineWidth = e->getTextAs<Efix>(); }},
    {"hookLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->hookLength = e->getTextAs<Evpu>(); }},
    {"smartLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartLineWidth = e->getTextAs<Efix>(); }},
    {"showOctavaAsText", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->showOctavaAsText = populateBoolean(c, e, i); }},
    {"smartDashOn", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOn = e->getTextAs<Evpu>(); }},
    {"smartDashOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOff = e->getTextAs<Evpu>(); }},
    {"crescHorizontal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHorizontal = populateBoolean(c, e, i); }},
    {"direction", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->direction = toEnum<SmartShapeOptions::DefaultDirection>(e); }},
    {"slurThicknessCp1X", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp1Y", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1Y = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2X", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2Y", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2Y = e->getTextAs<Evpu>(); }},
    {"slurAvoidAccidentals", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidAccidentals = populateBoolean(c, e, i); }},
    {"slurAvoidStaffLinesAmt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLinesAmt = e->getTextAs<Evpu>(); }},
    {"maxSlurStretch", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretch = e->getTextAs<Efix>(); }},
    {"maxSlurLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurLift = e->getTextAs<Efix>(); }},
    {"slurSymmetry", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurSymmetry = e->getTextAs<int>(); }},
    {"useEngraverSlurs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->useEngraverSlurs = populateBoolean(c, e, i); }},
    {"slurLeftBreakHorzAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurLeftBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurRightBreakHorzAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurRightBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurBreakVertAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurBreakVertAdj = e->getTextAs<Evpu>(); }},
    {"slurAvoidStaffLines", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLines = populateBoolean(c, e, i); }},
    {"slurPadding", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurPadding = e->getTextAs<Evpu>(); }},
    {"maxSlurAngle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurAngle = e->getTextAs<Efix>(); }},
    {"slurAcciPadding", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAcciPadding = e->getTextAs<Evpu>(); }},
    {"slurDoStretchFirst", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurDoStretchFirst = populateBoolean(c, e, i); }},
    {"slurStretchByPercent", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurStretchByPercent = populateBoolean(c, e, i); }},
    {"maxSlurStretchPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretchPercent = e->getTextAs<int>(); }},
    {"articAvoidSlurAmt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->articAvoidSlurAmt = e->getTextAs<Evpu>(); }},
    {"ssLineStyleCmpCustom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpCustom = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpGlissando", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpGlissando = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabSlide", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabSlide = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabBendCurve", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabBendCurve = e->getTextAs<Cmper>(); }},
    {"smartSlurTipWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartSlurTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"guitarBendUseParens", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseParens = populateBoolean(c, e, i); }},
    {"guitarBendHideBendTo", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendHideBendTo = populateBoolean(c, e, i); }},
    {"guitarBendGenText", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendGenText = populateBoolean(c, e, i); }},
    {"guitarBendUseFull", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseFull = populateBoolean(c, e, i); }},
    {"slurConnectStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(c, e, i->slurConnectStyles); }},
    {"slurControlStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(c, e, i->slurControlStyles); }},
    {"tabSlideConnectStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(c, e, i->tabSlideConnectStyles); }},
    {"glissandoConnectStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(c, e, i->glissandoConnectStyles); }},
    {"bendCurveConnectStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(c, e, i->bendCurveConnectStyles); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffOptions, {
    {"staffSeparation", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->staffSeparation = e->getTextAs<Evpu>(); }},
    {"staffSeparIncr", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->staffSeparIncr = e->getTextAs<Evpu>(); }},
    {"autoAdjustStaffSepar", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i) { i->autoAdjustStaffSepar = populateBoolean(c, e, i); }},
    {"namePos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i)
        { i->namePos = FieldPopulator<others::NamePositioning>::createAndPopulate(c, e, i->getDocument()); }},
    {"namePosAbbrv", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i)
        { i->namePosAbbrv = FieldPopulator<others::NamePositioning>::createAndPopulate(c, e, i->getDocument()); }},
    {"groupNameFullPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i)
        { i->groupNameFullPos = FieldPopulator<others::NamePositioning>::createAndPopulate(c, e, i->getDocument()); }},
    {"groupNameAbbrvPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffOptions>& i)
        { i->groupNameAbbrvPos = FieldPopulator<others::NamePositioning>::createAndPopulate(c, e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(StemOptions::StemConnection, {
    {"font", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { populateFontId(c, e, i->fontId); }},
    {"symbol", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->symbol = static_cast<char32_t>(e->getTextAs<uint32_t>()); }},
    {"upStemVert", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->upStemVert = e->getTextAs<Efix>(); }},
    {"downStemVert", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->downStemVert = e->getTextAs<Efix>(); }},
    {"upStemHorz", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->upStemHorz = e->getTextAs<Efix>(); }},
    {"downStemHorz", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions::StemConnection>& i) { i->downStemHorz = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StemOptions, {
    {"halfStemLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->halfStemLength = e->getTextAs<Evpu>(); }},
    {"stemLength", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemLength = e->getTextAs<Evpu>(); }},
    {"stem2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->shortStemLength = e->getTextAs<Evpu>(); }},
    {"revStemAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->revStemAdj = e->getTextAs<Evpu>(); }},
    {"noReverseStems", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->noReverseStems = populateBoolean(c, e, i); }},
    {"stemWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemWidth = e->getTextAs<Efix>(); }},
    {"stemLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemOffset = e->getTextAs<Efix>(); }},
    {"useStemConnections", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->useStemConnections = populateBoolean(c, e, i); }},
    {"stemConnect", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) {
            auto indexAttr = e->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : static_cast<size_t>(-1);
            if (i->stemConnections.size() > index) {
                throw std::invalid_argument("StemConnections index mismatch. Expected: " + std::to_string(i->stemConnections.size()) + ", Found: " + std::to_string(index));
            } else {
                for (size_t x = i->stemConnections.size(); x < index; x++) {
                    i->stemConnections.push_back(std::make_shared<StemOptions::StemConnection>());
                }
            }
            i->stemConnections.push_back(FieldPopulator<StemOptions::StemConnection>::createAndPopulate(c, e));
        }
    },
});

MUSX_XML_ELEMENT_ARRAY(TextOptions::InsertSymbolInfo, {
    {"trackingBefore",    [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->trackingBefore = e->getTextAs<int>(); }},
    {"trackingAfter",     [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->trackingAfter = e->getTextAs<int>(); }},
    {"baselineShiftPerc", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->baselineShiftPerc = e->getTextAs<int>(); }},
    {"symFont",           [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i)
        { i->symFont = FieldPopulator<FontInfo>::createAndPopulate(c, e, i->getDocument(), /*sizeIsPercent*/ true); }},
    {"symChar",           [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions::InsertSymbolInfo>& i) { i->symChar = e->getTextAs<char32_t>(); }}
    });

MUSX_XML_ELEMENT_ARRAY(TextOptions, {
    {"textLineSpacingPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textLineSpacingPercent = e->getTextAs<int>(); }},
    {"textLineSpacingEvpu",    [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textLineSpacingEvpu = e->getTextAs<Evpu>(); }},
    {"showTimeSeconds",        [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->showTimeSeconds = populateBoolean(c, e, i); }},
    {"dateFormat",             [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->dateFormat = toEnum<DateFormat>(e); }},
    {"tabSpaces",              [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->tabSpaces = e->getTextAs<int>(); }},
    {"textTracking",           [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textTracking = e->getTextAs<int>(); }},
    {"textBaselineShift",      [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textBaselineShift = e->getTextAs<Evpu>(); }},
    {"textSuperscript",        [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textSuperscript = e->getTextAs<Evpu>(); }},
    {"textWordWrap",           [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textWordWrap = populateBoolean(c, e, i); }},
    {"textPageOffset",         [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textPageOffset = e->getTextAs<Evpu>(); }},
    {"textJustify",            [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textJustify = toEnum<TextOptions::TextJustify>(e); }},
    {"textExpandSingleWord",   [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textExpandSingleWord = populateBoolean(c, e, i); }},
    {"textHorzAlign",          [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textHorzAlign = toEnum<TextOptions::HorizontalAlignment>(e); }},
    {"textVertAlign",          [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textVertAlign = toEnum<TextOptions::VerticalAlignment>(e); }},
    {"textIsEdgeAligned",      [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { i->textIsEdgeAligned = populateBoolean(c, e, i); }},
    {"insertSymbolInfo",       [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TextOptions>& i) { populateEmbeddedClass(c, e, i->symbolInserts, i); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ConnectStyle, {
    {"offsetX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetX = e->getTextAs<Evpu>(); }},
    {"offsetY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetY = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ControlPoint, {
    {"insetRatio", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetRatio = e->getTextAs<Efix>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"insetFixed", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetFixed = e->getTextAs<Evpu>(); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions::ControlStyle, {
    {"span", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->span = e->getTextAs<Evpu>(); }},
    {"cp1", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp1 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(c, e); }},
    {"cp2", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp2 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(c, e); }}
});

MUSX_XML_ELEMENT_ARRAY(TieOptions, {
    {"frontTieSepar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->frontTieSepar = e->getTextAs<Evpu>(); }},
    {"thicknessRight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessRight = e->getTextAs<Evpu>(); }},
    {"thicknessLeft", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessLeft = e->getTextAs<Evpu>(); }},
    {"breakForTimeSigs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakForTimeSigs = populateBoolean(c, e, i); }},
    {"breakForKeySigs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakForKeySigs = populateBoolean(c, e, i); }},
    {"breakTimeSigLeftHOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakTimeSigRightHOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigRightHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigLeftHOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigRightHOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigRightHOffset = e->getTextAs<Evpu>(); }},
    {"sysBreakLeftHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakLeftHAdj = e->getTextAs<Evpu>(); }},
    {"sysBreakRightHAdj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakRightHAdj = e->getTextAs<Evpu>(); }},
    {"useOuterPlacement", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useOuterPlacement = populateBoolean(c, e, i); }},
    {"secondsPlacement", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->secondsPlacement = toEnum<TieOptions::SecondsPlacement>(e); }},
    {"chordTieDirType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->chordTieDirType = toEnum<TieOptions::ChordTieDirType>(e); }},
    {"chordTieDirOpposingSeconds", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->chordTieDirOpposingSeconds = populateBoolean(c, e, i); }},
    {"mixedStemDirection", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->mixedStemDirection = toEnum<TieOptions::MixedStemDirection>(e); }},
    {"afterSingleDot", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->afterSingleDot = populateBoolean(c, e, i); }},
    {"afterMultipleDots", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->afterMultipleDots = populateBoolean(c, e, i); }},
    {"beforeAcciSingleNote", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->beforeAcciSingleNote = populateBoolean(c, e, i); }},
    {"specialPosMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->specialPosMode = toEnum<TieOptions::SpecialPosMode>(e); }},
    {"avoidStaffLinesDistance", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesDistance = e->getTextAs<Evpu>(); }},
    {"insetStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->insetStyle = toEnum<TieOptions::InsetStyle>(e); }},
    {"useInterpolation", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useInterpolation = populateBoolean(c, e, i); }},
    {"useTieEndCtlStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->useTieEndCtlStyle = populateBoolean(c, e, i); }},
    {"avoidStaffLinesOnly", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesOnly = populateBoolean(c, e, i); }},
    {"tieTipWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->tieTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"tieConnectStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(c, e, i->tieConnectStyles); }},
    {"tieControlStyle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(c, e, i->tieControlStyles); }}
});

MUSX_XML_ELEMENT_ARRAY(TimeSignatureOptions, {
    {"timeUpperLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLift = e->getTextAs<Evpu>(); }},
    {"timeFront", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFront = e->getTextAs<Evpu>(); }},
    {"timeBack", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBack = e->getTextAs<Evpu>(); }},
    {"timeFrontParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFrontParts = e->getTextAs<Evpu>(); }},
    {"timeBackParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBackParts = e->getTextAs<Evpu>(); }},
    {"timeUpperLiftParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLiftParts = e->getTextAs<Evpu>(); }},
    {"timeLowerLiftParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLiftParts = e->getTextAs<Evpu>(); }},
    {"timeAbrvLiftParts", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLiftParts = e->getTextAs<Evpu>(); }},
    {"timeSigDoAbrvCommon", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCommon = populateBoolean(c, e, i); }},
    {"timeSigDoAbrvCut", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCut = populateBoolean(c, e, i); }},
    {"defFloat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->numCompositeDecimalPlaces = e->getTextAs<int>(); }},
    {"cautionaryTimeChanges", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->cautionaryTimeChanges = populateBoolean(c, e, i); }},
    {"timeLowerLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLift = e->getTextAs<Evpu>(); }},
    {"timeAbrvLift", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLift = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TupletOptions, {
    {"symbolicNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->displayNumber = e->getTextAs<int>(); }},
    {"symbolicDur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->displayDuration = e->getTextAs<Edu>(); }},
    {"refNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->referenceNumber = e->getTextAs<int>(); }},
    {"refDur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"flat", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->alwaysFlat = populateBoolean(c, e, i); }},
    {"fullDura", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->fullDura = populateBoolean(c, e, i); }},
    {"metricCenter", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->metricCenter = populateBoolean(c, e, i); }},
    {"avoidStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->avoidStaff = populateBoolean(c, e, i); }},
    {"autoBracketStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->autoBracketStyle = toEnum<TupletOptions::AutoBracketStyle>(e); }},
    {"tupOffX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupOffX = e->getTextAs<Evpu>(); }},
    {"tupOffY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupOffY = e->getTextAs<Evpu>(); }},
    {"brackOffX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackOffX = e->getTextAs<Evpu>(); }},
    {"brackOffY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackOffY = e->getTextAs<Evpu>(); }},
    {"numStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->numStyle = toEnum<TupletOptions::NumberStyle>(e); }},
    {"posStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->posStyle = toEnum<TupletOptions::PositioningStyle>(e); }},
    {"allowHorz", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->allowHorz = populateBoolean(c, e, i); }},
    {"ignoreGlOffs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->ignoreHorzNumOffset = populateBoolean(c, e, i); }},
    {"breakBracket", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->breakBracket = populateBoolean(c, e, i); }},
    {"matchHooks", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->matchHooks = populateBoolean(c, e, i); }},
    {"noteBelow", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->useBottomNote = populateBoolean(c, e, i); }},
    {"brackStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->brackStyle = toEnum<TupletOptions::BracketStyle>(e); }},
    {"smartTuplet", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->smartTuplet = populateBoolean(c, e, i); }},
    {"leftHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->leftHookLen = e->getTextAs<Evpu>(); }},
    {"leftHookExt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->leftHookExt = e->getTextAs<Evpu>(); }},
    {"rightHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->rightHookLen = e->getTextAs<Evpu>(); }},
    {"rightHookExt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->rightHookExt = e->getTextAs<Evpu>(); }},
    {"slope", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->manualSlopeAdj = e->getTextAs<Evpu>(); }},
    {"tupMaxSlope", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupMaxSlope = e->getTextAs<int>(); }},
    {"tupLineWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupLineWidth = e->getTextAs<Efix>(); }},
    {"tupNUpstemOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupNUpstemOffset = e->getTextAs<Evpu>(); }},
    {"tupNDownstemOffset", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletOptions>& i) { i->tupNDownstemOffset = e->getTextAs<Evpu>(); }},
});

} // namespace options
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS
