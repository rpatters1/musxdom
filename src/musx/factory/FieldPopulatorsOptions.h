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

using namespace ::musx::xml;
using namespace ::musx::dom::options;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

MUSX_XML_ENUM_MAPPING(BeamOptions::FlattenStyle, {
    {"onEndNotes", BeamOptions::FlattenStyle::OnEndNotes},
    {"onStandardNote", BeamOptions::FlattenStyle::OnStandardNote},
    {"onExtremeNote", BeamOptions::FlattenStyle::OnExtremeNote},
    {"alwaysFlat", BeamOptions::FlattenStyle::AlwaysFlat}
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

MUSX_XML_ENUM_MAPPING(MusicSpacingOptions::ColUnisonsChoice, {
    {"diffNoteheads", MusicSpacingOptions::ColUnisonsChoice::DiffNoteheads},
    {"all", MusicSpacingOptions::ColUnisonsChoice::All}
    });

MUSX_XML_ENUM_MAPPING(MusicSpacingOptions::ManualPositioning, {
    {"clear", MusicSpacingOptions::ManualPositioning::Clear},
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
    {"ratioPlusBothNotes", TupletOptions::NumberStyle::RatioPlusBothNotes},
    {"ratioPlusDenNote", TupletOptions::NumberStyle::RatioPlusDenominatorNote},
    });

MUSX_XML_ENUM_MAPPING(TupletOptions::PositioningStyle, {
    //{"manual", TupletOptions::PositioningStyle::Manual}, this is the default and is not known to occur in the xml
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

MUSX_RESOLVER_ARRAY(ClefOptions, {
    [](const dom::DocumentPtr& document) {
        auto clefOptions = document->getOptions()->get<ClefOptions>();
        for (size_t i = 0; i < clefOptions->clefDefs.size(); i++) {
            const auto& def = clefOptions->clefDefs[i];
            if (def->useOwnFont && !def->font) {
                throw std::invalid_argument("Use own font was specified for clef " + std::to_string(i) + ", but no font was found in the xml.");
            }
        }
    },
    });

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
