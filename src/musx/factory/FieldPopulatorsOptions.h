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

using namespace dom;
using namespace dom::options;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

// Field populator arrays are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

template <>
inline const XmlElementArray<AccidentalOptions> FieldPopulator<AccidentalOptions>::xmlElements = {
    {"overlap", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->minOverlap = e->getTextAs<int>(); }},
    {"bacciAdd", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->multiCharSpace = e->getTextAs<Evpu>(); }},
    {"useNewAcciPositioning", [](const XmlElementPtr&, const std::shared_ptr<AccidentalOptions>& i) { i->crossLayerPositioning = true; }},
    {"frontAcciSepar", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->startMeasureSepar = e->getTextAs<Evpu>(); }},
    {"acciNoteSpace", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciNoteSpace = e->getTextAs<Evpu>(); }},
    {"acciAcciSpace", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalOptions>& i) { i->acciAcciSpace = e->getTextAs<Evpu>(); }},
};

template <>
inline const XmlElementArray<AlternateNotationOptions> FieldPopulator<AlternateNotationOptions>::xmlElements = {
    {"halfSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashLift = e->getTextAs<Evpu>(); }},
    {"wholeSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->wholeSlashLift = e->getTextAs<Evpu>(); }},
    {"dWholeSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->dWholeSlashLift = e->getTextAs<Evpu>(); }},
    {"halfSlashStemLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->halfSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashStemLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashStemLift = e->getTextAs<Evpu>(); }},
    {"quartSlashLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->quartSlashLift = e->getTextAs<Evpu>(); }},
    {"twoMeasNumLift", [](const XmlElementPtr& e, const std::shared_ptr<AlternateNotationOptions>& i) { i->twoMeasNumLift = e->getTextAs<Evpu>(); }},
};

template <>
inline const XmlElementArray<AugmentationDotOptions> FieldPopulator<AugmentationDotOptions>::xmlElements = {
    {"dotUpFlagOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotUpFlagOffset = e->getTextAs<Evpu>(); }},
    {"dotOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotOffset = e->getTextAs<Evpu>(); }},
    {"doDotDownAdjust", [](const XmlElementPtr&, const std::shared_ptr<AugmentationDotOptions>& i) { i->adjMultipleVoices = true; }},
    {"dotFirstOffset", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotNoteOffset = e->getTextAs<Evpu>(); }},
    {"dotLift", [](const XmlElementPtr& e, const std::shared_ptr<AugmentationDotOptions>& i) { i->dotLift = e->getTextAs<Evpu>(); }},
    {"skip27_4DotAdjust", [](const XmlElementPtr&, const std::shared_ptr<AugmentationDotOptions>& i) { i->useLegacyFlippedStemPositioning = true; }},
};

template <>
inline const XmlElementArray<BarlineOptions> FieldPopulator<BarlineOptions>::xmlElements = {
    {"drawCloseSystemBarline", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseSystemBarline = true; }},
    {"drawCloseFinalBarline", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawCloseFinalBarline = true; }},
    {"drawFinalBarlineOnLastMeas", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawFinalBarlineOnLastMeas = true; }},
    {"leftBarlineUsePrevStyle", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->leftBarlineUsePrevStyle = true; }},
    {"drawLeftBarlineSingleStaff", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineSingleStaff = true; }},
    {"drawLeftBarlineMultipleStaves", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawLeftBarlineMultipleStaves = true; }},
    {"drawBarlines", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawBarlines = true; }},
    {"barlineWidth", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineWidth = e->getTextAs<Efix>(); }},
    {"thickBarlineWidth", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->thickBarlineWidth = e->getTextAs<Efix>(); }},
    {"doubleBarlineSpace", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->doubleBarlineSpace = e->getTextAs<Efix>(); }},
    {"finalBarlineSpace", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->finalBarlineSpace = e->getTextAs<Efix>(); }},
    {"barlineDashOn", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOn = e->getTextAs<Evpu>(); }},
    {"barlineDashOff", [](const XmlElementPtr& e, const std::shared_ptr<BarlineOptions>& i) { i->barlineDashOff = e->getTextAs<Evpu>(); }},
    {"drawDoubleBarlineBeforeKeyChanges", [](const XmlElementPtr&, const std::shared_ptr<BarlineOptions>& i) { i->drawDoubleBarlineBeforeKeyChanges = true; }},
};

template <>
inline XmlEnumMapping<BeamOptions::FlattenStyle> EnumMapper<BeamOptions::FlattenStyle>::mapping = {
    {"onEndNotes", BeamOptions::FlattenStyle::OnEndNotes},
    {"onStandardNote", BeamOptions::FlattenStyle::OnStandardNote},
    {"onExtremeNote", BeamOptions::FlattenStyle::OnExtremeNote},
    {"alwaysFlat", BeamOptions::FlattenStyle::AlwaysFlat}
};

template <>
inline const XmlElementArray<BeamOptions> FieldPopulator<BeamOptions>::xmlElements = {
    {"beamStubLength", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamStubLength = e->getTextAs<Evpu>(); }},
    {"maxSlope", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxSlope = e->getTextAs<Evpu>(); }},
    {"beamSepar", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamSepar = e->getTextAs<Evpu>(); }},
    {"hmBeamTip", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxFromMiddle = e->getTextAs<Evpu>(); }},
    {"beamingStyle", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamingStyle = toEnum<BeamOptions::FlattenStyle>(e->getTextTrimmed()); }},
    {"incEdgeRestsInBeamGroups", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->extendBeamsOverRests = true; }},
    {"incRestsInClassicBeams", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->incRestsInFourGroups = true; }},
    {"beamFourEighthsInCommonTime", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->beamFourEighthsInCommonTime = true; }},
    {"beamThreeEighthsInCommonTime", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->beamThreeEighthsInCommonTime = true; }},
    {"doStemStubs", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->dispHalfStemsOnRests = true; }},
    {"spanSpace", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->spanSpace = true; }},
    {"extendSecBeamsOverRests", [](const XmlElementPtr&, const std::shared_ptr<BeamOptions>& i) { i->extendSecBeamsOverRests = true; }},
    {"beamWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamWidth = e->getTextAs<Efix>(); }},
};

template <>
inline const XmlElementArray<ClefOptions::ClefDef> FieldPopulator<ClefOptions::ClefDef>::xmlElements = {
    {"adjust", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->middleCPos = e->getTextAs<int>(); }},
    {"clefChar", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->clefChar = e->getTextAs<char32_t>(); }},
    {"clefYDisp", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->staffPositon = e->getTextAs<int>(); }},
    {"baseAdjust", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->baselineAdjust = e->getTextAs<Efix>(); }},
    {"shapeID", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->shapeId = e->getTextAs<int>(); }},
    {"isShape", [](const XmlElementPtr&, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->isShape = true; }},
    {"scaleToStaffHeight", [](const XmlElementPtr&, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->scaleToStaffHeight = true; }},
    {"font", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->font = FieldPopulator<FontInfo>::createAndPopulate(e, i->getDocument()); }},
    {"useOwnFont", [](const XmlElementPtr&, const std::shared_ptr<ClefOptions::ClefDef>& i) { i->useOwnFont = true; }},
};

template <>
inline const XmlElementArray<ClefOptions> FieldPopulator<ClefOptions>::xmlElements = {
    {"defaultClef", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->defaultClef = e->getTextAs<int>(); }},
    {"endMeasClefPercent", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangePercent = e->getTextAs<int>(); }},
    {"endMeasClefPosAdd", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefChangeOffset = e->getTextAs<Evpu>(); }},
    {"clefFront", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefFrontSepar = e->getTextAs<Evpu>(); }},
    {"clefBack", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefBackSepar = e->getTextAs<Evpu>(); }},
    {"showClefFirstSystemOnly", [](const XmlElementPtr&, const std::shared_ptr<ClefOptions>& i) { i->showClefFirstSystemOnly = true; }},
    {"clefKey", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefKeySepar = e->getTextAs<Evpu>(); }},
    {"clefTime", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) { i->clefTimeSepar = e->getTextAs<Evpu>(); }},
    {"cautionaryClefChanges", [](const XmlElementPtr&, const std::shared_ptr<ClefOptions>& i) { i->cautionaryClefChanges = true; }},
    {"clefDef", [](const XmlElementPtr& e, const std::shared_ptr<ClefOptions>& i) {
            auto indexAttr = e->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : -1;
            if (i->clefDefs.size() != index) {
                throw std::invalid_argument("ClefDef index mismatch. Expected: " + std::to_string(i->clefDefs.size()) + ", Found: " + std::to_string(index));
            }
            i->clefDefs.push_back(FieldPopulator<ClefOptions::ClefDef>::createAndPopulate(e, i->getDocument()));
        }
    },
};

template <>
struct ResolverArray<ClefOptions>
{
    inline static const ResolverList value = {
        [](const dom::DocumentPtr& document) {
            auto clefOptions = document->getOptions()->get<ClefOptions>();
            for (size_t i = 0; i < clefOptions->clefDefs.size(); i++) {
                const auto& def = clefOptions->clefDefs[i];
                if (def->useOwnFont && !def->font) {
                    throw std::invalid_argument("Use own font was specified for clef " + std::to_string(i) + ", but no font was found in the xml.");
                }
            }
        },
    };
};

template <>
inline const XmlElementArray<FlagOptions> FieldPopulator<FlagOptions>::xmlElements = {
    {"straightFlags", [](const XmlElementPtr&, const std::shared_ptr<FlagOptions>& i) { i->straightFlags = true; }},
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
};

template <>
inline XmlEnumMapping<FontOptions::FontType> EnumMapper<FontOptions::FontType>::mapping = {
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
};

template <>
inline const XmlElementArray<FontOptions> FieldPopulator<FontOptions>::xmlElements = {
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
};

template <>
inline const XmlElementArray<GraceNoteOptions> FieldPopulator<GraceNoteOptions>::xmlElements = {
    {"tabGracePerc", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->tabGracePerc = e->getTextAs<int>(); }},
    {"gracePerc", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->gracePerc = e->getTextAs<int>(); }},
    {"playbackDuration", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->playbackDuration = e->getTextAs<Edu>(); }},
    {"graceBackup", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->entryOffset = e->getTextAs<Evpu>(); }},
    {"slashFlaggedGraceNotes", [](const XmlElementPtr&, const std::shared_ptr<GraceNoteOptions>& i) { i->slashFlaggedGraceNotes = true; }},
    {"graceSlashWidth", [](const XmlElementPtr& e, const std::shared_ptr<GraceNoteOptions>& i) { i->graceSlashWidth = e->getTextAs<Efix>(); }},
};

template <>
inline const XmlElementArray<KeySignatureOptions> FieldPopulator<KeySignatureOptions>::xmlElements = {
    {"doKeyCancel", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancel = true; }},
    {"doCStart", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->doCStart = true; }},
    {"doBankDiff", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->redisplayOnModeChange = true; }},
    {"keyFront", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyFront = e->getTextAs<Evpu>(); }},
    {"keyMid", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyMid = e->getTextAs<Evpu>(); }},
    {"keyBack", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyBack = e->getTextAs<Evpu>(); }},
    {"acciAdd", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->acciAdd = e->getTextAs<Evpu>(); }},
    {"showKeyFirstSystemOnly", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->showKeyFirstSystemOnly = true; }},
    {"keyTime", [](const XmlElementPtr& e, const std::shared_ptr<KeySignatureOptions>& i) { i->keyTimeSepar = e->getTextAs<Evpu>(); }},
    {"simplifyKeyHoldOctave", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->simplifyKeyHoldOctave = true; }},
    {"cautionaryKeyChanges", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->cautionaryKeyChanges = true; }},
    {"doKeyCancelBetweenSharpsFlats", [](const XmlElementPtr&, const std::shared_ptr<KeySignatureOptions>& i) { i->doKeyCancelBetweenSharpsFlats = true; }},
};

template <>
inline const XmlElementArray<LineCurveOptions> FieldPopulator<LineCurveOptions>::xmlElements = {
    {"bezierStep", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->bezierStep = e->getTextAs<int>(); }},
    {"enclosureWidth", [](const XmlElementPtr& e, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureWidth = e->getTextAs<Efix>(); }},
    {"enclosureRoundCorners", [](const XmlElementPtr&, const std::shared_ptr<LineCurveOptions>& i) { i->enclosureRoundCorners = true; }},
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
};

template <>
inline const XmlElementArray<MiscOptions> FieldPopulator<MiscOptions>::xmlElements = {
    {"showRepeatsForParts", [](const XmlElementPtr&, const std::shared_ptr<MiscOptions>& i) {i->showRepeatsForParts = true;}},
    {"retainOctaveTransInConcertPitch", [](const XmlElementPtr&, const std::shared_ptr<MiscOptions>& i) {i->keepOctaveTransInConcertPitch = true;}},
    {"showCurrentLayerOnly", [](const XmlElementPtr&, const std::shared_ptr<MiscOptions>& i) {i->showActiveLayerOnly = true;}},
    {"combineRestsAcrossLayers", [](const XmlElementPtr&, const std::shared_ptr<MiscOptions>& i) {i->consolidateRestsAcrossLayers = true;}},
    {"sdDashOn", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashLength = e->getTextAs<Evpu>();}},
    {"sdDashOff", [](const XmlElementPtr& e, const std::shared_ptr<MiscOptions>& i) {i->shapeDesignerDashSpace = e->getTextAs<Evpu>();}},
    {"drawMeasureNumbersOverBarlines", [](const XmlElementPtr&, const std::shared_ptr<MiscOptions>& i) {i->alignMeasureNumbersWithBarlines = true;}},
};

template <>
inline const XmlElementArray<MultimeasureRestOptions> FieldPopulator<MultimeasureRestOptions>::xmlElements = {
    {"meaSpace", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->measWidth = e->getTextAs<Evpu>();}},
    {"numdec", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjY = e->getTextAs<Evpu>();}},
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->shapeDef = e->getTextAs<Cmper>();}},
    {"numStart", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numStart = e->getTextAs<int>();}},
    {"threshold", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->useSymsThreshold = e->getTextAs<int>();}},
    {"spacing", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->symSpacing = e->getTextAs<Evpu>();}},
    {"numAdjX", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->numAdjX = e->getTextAs<Evpu>();}},
    {"startAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->startAdjust = e->getTextAs<Evpu>();}},
    {"endAdjust", [](const XmlElementPtr& e, const std::shared_ptr<MultimeasureRestOptions>& i) {i->endAdjust = e->getTextAs<Evpu>();}},
    {"useCharRestStyle", [](const XmlElementPtr&, const std::shared_ptr<MultimeasureRestOptions>& i) {i->useSymbols = true;}},
    {"autoUpdateMmRests", [](const XmlElementPtr&, const std::shared_ptr<MultimeasureRestOptions>& i) {i->autoUpdateMmRests = true;}},
};

template <>
inline XmlEnumMapping<MusicSpacingOptions::ColUnisonsChoice> EnumMapper<MusicSpacingOptions::ColUnisonsChoice>::mapping = {
    {"diffNoteheads", MusicSpacingOptions::ColUnisonsChoice::DiffNoteheads},
    {"all", MusicSpacingOptions::ColUnisonsChoice::All}
};

template <>
inline XmlEnumMapping<MusicSpacingOptions::ManualPositioning> EnumMapper<MusicSpacingOptions::ManualPositioning>::mapping = {
    {"clear", MusicSpacingOptions::ManualPositioning::Clear},
    {"incorp", MusicSpacingOptions::ManualPositioning::Incorporate}
};

template <>
inline XmlEnumMapping<MusicSpacingOptions::GraceNoteSpacing> EnumMapper<MusicSpacingOptions::GraceNoteSpacing>::mapping = {
    {"recomp", MusicSpacingOptions::GraceNoteSpacing::Automatic},
    {"keep", MusicSpacingOptions::GraceNoteSpacing::KeepCurrent}
};

template <>
inline const XmlElementArray<MusicSpacingOptions> FieldPopulator<MusicSpacingOptions>::xmlElements = {
    {"minWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minWidth = e->getTextAs<Evpu>(); }},
    {"maxWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->maxWidth = e->getTextAs<Evpu>(); }},
    {"minDistance", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistance = e->getTextAs<Evpu>(); }},
    {"minDistTiedNotes", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistTiedNotes = e->getTextAs<Evpu>(); }},
    {"avoidColNotes", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColNotes = true; }},
    {"avoidColLyrics", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLyrics = true; }},
    {"avoidColChords", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColChords = true; }},
    {"avoidColArtics", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColArtics = true; }},
    {"avoidColClefs", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColClefs = true; }},
    {"avoidColSeconds", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColSeconds = true; }},
    {"avoidColStems", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColStems = true; }},
    {"avoidColUnisons", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColUnisons = toEnum<MusicSpacingOptions::ColUnisonsChoice>(e->getTextTrimmed()); }},
    {"avoidColLedgers", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLedgers = true; }},
    {"manualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->manualPositioning = toEnum<MusicSpacingOptions::ManualPositioning>(e->getTextTrimmed()); }},
    {"ignoreHidden", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->ignoreHidden = true; }},
    {"useAllottmentTables", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->useAllottmentTables = true; }},
    {"referenceDuration", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"referenceWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceWidth = e->getTextAs<Evpu>(); }},
    {"scalingFactor", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->scalingFactor = e->getTextAs<double>(); }},
    {"defaultAllotment", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->defaultAllotment = e->getTextAs<Evpu>(); }},
    {"minDistGrace", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistGrace = e->getTextAs<Evpu>(); }},
    {"graceNoteManualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->graceNoteSpacing = toEnum<MusicSpacingOptions::GraceNoteSpacing>(e->getTextTrimmed()); }},
    {"musFront", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musFront = e->getTextAs<Evpu>(); }},
    {"musBack", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musBack = e->getTextAs<Evpu>(); }},
};

template <>
inline const XmlElementArray<PageFormatOptions::PageFormat> FieldPopulator<PageFormatOptions::PageFormat>::xmlElements = {
    {"pageHeight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageHeight = e->getTextAs<Evpu>(); }},
    {"pageWidth", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pageWidth = e->getTextAs<Evpu>(); }},
    {"pagePercent", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->pagePercent = e->getTextAs<int>(); }},
    {"sysPercent", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->sysPercent = e->getTextAs<int>(); }},
    {"rawStaffHeight", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->rawStaffHeight = e->getTextAs<int>(); }},
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
    {"facingPages", [](const XmlElementPtr&, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->facingPages = true; }},
    {"differentFirstSysMargin", [](const XmlElementPtr&, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstSysMargin = true; }},
    {"differentFirstPageMargin", [](const XmlElementPtr&, const std::shared_ptr<PageFormatOptions::PageFormat>& i) { i->differentFirstPageMargin = true; }},
};

template <>
inline XmlEnumMapping<PageFormatOptions::AdjustPageScope> EnumMapper<PageFormatOptions::AdjustPageScope>::mapping = {
    {"current", PageFormatOptions::AdjustPageScope::Current},
    {"all", PageFormatOptions::AdjustPageScope::All},
    {"leftOrRight", PageFormatOptions::AdjustPageScope::LeftOrRight},
    {"range", PageFormatOptions::AdjustPageScope::PageRange}
};

template <>
inline const XmlElementArray<PageFormatOptions> FieldPopulator<PageFormatOptions>::xmlElements = {
    {"pageFormatScore", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i){ i->pageFormatScore = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(e); }},
    {"pageFormatParts", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->pageFormatParts = FieldPopulator<PageFormatOptions::PageFormat>::createAndPopulate(e); }},
    {"avoidSystemMarginCollisions", [](const XmlElementPtr&, const std::shared_ptr<PageFormatOptions>& i) { i->avoidSystemMarginCollisions = true; }},
    {"adjustPageScope", [](const XmlElementPtr& e, const std::shared_ptr<PageFormatOptions>& i) { i->adjustPageScope = toEnum<PageFormatOptions::AdjustPageScope>(e->getTextTrimmed()); }},
};

template <>
inline const XmlElementArray<PianoBraceBracketOptions> FieldPopulator<PianoBraceBracketOptions>::xmlElements = {
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
};

template <>
inline XmlEnumMapping<RepeatOptions::WingStyle> EnumMapper<RepeatOptions::WingStyle>::mapping = {
    {"none", RepeatOptions::WingStyle::None},
    {"curved", RepeatOptions::WingStyle::Curved},
    {"singleLine", RepeatOptions::WingStyle::SingleLine},
    {"doubleLine", RepeatOptions::WingStyle::DoubleLine}
};

template <>
inline XmlEnumMapping<RepeatOptions::BackToBackStyle> EnumMapper<RepeatOptions::BackToBackStyle>::mapping = {
    {"thin", RepeatOptions::BackToBackStyle::Thin},
    {"mixed", RepeatOptions::BackToBackStyle::Mixed},
    {"thick", RepeatOptions::BackToBackStyle::Thick}
};

template <>
inline const XmlElementArray<RepeatOptions> FieldPopulator<RepeatOptions>::xmlElements = {
    {"bracketHeight", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHeight = e->getTextAs<Evpu>(); }},
    {"maxPasses", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->maxPasses = e->getTextAs<int>(); }},
    {"addPeriod", [](const XmlElementPtr&, const std::shared_ptr<RepeatOptions>& i) { i->addPeriod = true; }},
    {"thickLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thickLineWidth = e->getTextAs<Efix>(); }},
    {"thinLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thinLineWidth = e->getTextAs<Efix>(); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lineSpace = e->getTextAs<Efix>(); }},
    {"backToBackStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backToBackStyle = toEnum<RepeatOptions::BackToBackStyle>(e->getTextTrimmed()); }},
    {"forwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->forwardDotHPos = e->getTextAs<Evpu>(); }},
    {"backwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backwardDotHPos = e->getTextAs<Evpu>(); }},
    {"upperDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->upperDotVPos = e->getTextAs<Evpu>(); }},
    {"lowerDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lowerDotVPos = e->getTextAs<Evpu>(); }},
    {"wingStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->wingStyle = toEnum<RepeatOptions::WingStyle>(e->getTextTrimmed()); }},
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
    {"bracketEndAnchorThinLine", [](const XmlElementPtr&, const std::shared_ptr<RepeatOptions>& i) { i->bracketEndAnchorThinLine = true; }},
    {"showOnStaffListNumber", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->showOnStaffListNumber = e->getTextAs<Cmper>(); }},
};

template <>
inline XmlEnumMapping<SmartShapeOptions::DefaultDirection> EnumMapper<SmartShapeOptions::DefaultDirection>::mapping = {
    {"automatic", SmartShapeOptions::DefaultDirection::Automatic},
    {"over", SmartShapeOptions::DefaultDirection::Over},
    {"under", SmartShapeOptions::DefaultDirection::Under},
};

template <>
inline XmlEnumMapping<SmartShapeOptions::ConnectionIndex> EnumMapper<SmartShapeOptions::ConnectionIndex>::mapping = {
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
};

template <>
inline XmlEnumMapping<SmartShapeOptions::SlurConnectStyleType> EnumMapper<SmartShapeOptions::SlurConnectStyleType>::mapping = {
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
};

template <>
inline XmlEnumMapping<SmartShapeOptions::TabSlideConnectStyleType> EnumMapper<SmartShapeOptions::TabSlideConnectStyleType>::mapping = {
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
};

template <>
inline XmlEnumMapping<SmartShapeOptions::GlissandoConnectStyleType> EnumMapper<SmartShapeOptions::GlissandoConnectStyleType>::mapping = {
    {"defaultStart", SmartShapeOptions::GlissandoConnectStyleType::DefaultStart},
    {"defaultEnd", SmartShapeOptions::GlissandoConnectStyleType::DefaultEnd}
};

template <>
inline XmlEnumMapping<SmartShapeOptions::BendCurveConnectStyleType> EnumMapper<SmartShapeOptions::BendCurveConnectStyleType>::mapping = {
    {"noteStart", SmartShapeOptions::BendCurveConnectStyleType::NoteStart},
    {"staffEnd", SmartShapeOptions::BendCurveConnectStyleType::StaffEnd},
    {"staffStart", SmartShapeOptions::BendCurveConnectStyleType::StaffStart},
    {"noteEnd", SmartShapeOptions::BendCurveConnectStyleType::NoteEnd},
    {"staffToTopLineStart", SmartShapeOptions::BendCurveConnectStyleType::StaffToTopLineStart},
    {"staffFromTopLineEnd", SmartShapeOptions::BendCurveConnectStyleType::StaffFromTopLineEnd},
    {"staffEndOffset", SmartShapeOptions::BendCurveConnectStyleType::StaffEndOffset},
    {"staffFromTopEndOffset", SmartShapeOptions::BendCurveConnectStyleType::StaffFromTopEndOffset}
};

template <>
inline XmlEnumMapping<SmartShapeOptions::SlurControlStyleType> EnumMapper<SmartShapeOptions::SlurControlStyleType>::mapping = {
    {"shortSpan", SmartShapeOptions::SlurControlStyleType::ShortSpan},
    {"mediumSpan", SmartShapeOptions::SlurControlStyleType::MediumSpan},
    {"longSpan", SmartShapeOptions::SlurControlStyleType::LongSpan},
    {"extraLongSpan", SmartShapeOptions::SlurControlStyleType::ExtraLongSpan}
};

template <>
inline const XmlElementArray<SmartShapeOptions::ConnectionStyle> FieldPopulator<SmartShapeOptions::ConnectionStyle>::xmlElements = {
    {"connectIndex", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->connectIndex = toEnum<SmartShapeOptions::ConnectionIndex>(e->getTextTrimmed()); }},
    {"xOffset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"yOffset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ConnectionStyle>& i) { i->yOffset = e->getTextAs<Evpu>(); }}
};

template <>
inline const XmlElementArray<SmartShapeOptions::ControlStyle> FieldPopulator<SmartShapeOptions::ControlStyle>::xmlElements = {
    {"span", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->span = e->getTextAs<Efix>(); }},
    {"inset", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->inset = e->getTextAs<Efix>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions::ControlStyle>& i) { i->height = e->getTextAs<Evpu>(); }}
};

template <>
inline const XmlElementArray<SmartShapeOptions> FieldPopulator<SmartShapeOptions>::xmlElements = {
    {"shortHairpinOpeningWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->shortHairpinOpeningWidth = e->getTextAs<Evpu>(); }},
    {"crescHeight", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHeight = e->getTextAs<Evpu>(); }},
    {"maximumShortHairpinLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maximumShortHairpinLength = e->getTextAs<Evpu>(); }},
    {"crescLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->crescLineWidth = e->getTextAs<Efix>(); }},
    {"hookLength", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->hookLength = e->getTextAs<Evpu>(); }},
    {"smartLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartLineWidth = e->getTextAs<Efix>(); }},
    {"showOctavaAsText", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->showOctavaAsText = true; }},
    {"smartDashOn", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOn = e->getTextAs<Evpu>(); }},
    {"smartDashOff", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartDashOff = e->getTextAs<Evpu>(); }},
    {"crescHorizontal", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->crescHorizontal = true; }},
    {"direction", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->direction = toEnum<SmartShapeOptions::DefaultDirection>(e->getTextTrimmed()); }},
    {"slurThicknessCp1X", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp1Y", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp1Y = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2X", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2X = e->getTextAs<Evpu>(); }},
    {"slurThicknessCp2Y", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurThicknessCp2Y = e->getTextAs<Evpu>(); }},
    {"slurAvoidAccidentals", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidAccidentals = true; }},
    {"slurAvoidStaffLinesAmt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLinesAmt = e->getTextAs<Evpu>(); }},
    {"maxSlurStretch", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretch = e->getTextAs<Efix>(); }},
    {"maxSlurLift", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurLift = e->getTextAs<Efix>(); }},
    {"slurSymmetry", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurSymmetry = e->getTextAs<int>(); }},
    {"useEngraverSlurs", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->useEngraverSlurs = true; }},
    {"slurLeftBreakHorzAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurLeftBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurRightBreakHorzAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurRightBreakHorzAdj = e->getTextAs<Evpu>(); }},
    {"slurBreakVertAdj", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurBreakVertAdj = e->getTextAs<Evpu>(); }},
    {"slurAvoidStaffLines", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAvoidStaffLines = true; }},
    {"slurPadding", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurPadding = e->getTextAs<Evpu>(); }},
    {"maxSlurAngle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurAngle = e->getTextAs<Efix>(); }},
    {"slurAcciPadding", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->slurAcciPadding = e->getTextAs<Evpu>(); }},
    {"slurDoStretchFirst", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->slurDoStretchFirst = true; }},
    {"slurStretchByPercent", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->slurStretchByPercent = true; }},
    {"maxSlurStretchPercent", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->maxSlurStretchPercent = e->getTextAs<int>(); }},
    {"articAvoidSlurAmt", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->articAvoidSlurAmt = e->getTextAs<Evpu>(); }},
    {"ssLineStyleCmpCustom", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpCustom = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpGlissando", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpGlissando = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabSlide", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabSlide = e->getTextAs<Cmper>(); }},
    {"ssLineStyleCmpTabBendCurve", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->ssLineStyleCmpTabBendCurve = e->getTextAs<Cmper>(); }},
    {"smartSlurTipWidth", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { i->smartSlurTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"guitarBendUseParens", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseParens = true; }},
    {"guitarBendHideBendTo", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendHideBendTo = true; }},
    {"guitarBendGenText", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendGenText = true; }},
    {"guitarBendUseFull", [](const XmlElementPtr&, const std::shared_ptr<SmartShapeOptions>& i) { i->guitarBendUseFull = true; }},
    {"slurConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->slurConnectStyles); }},
    {"slurControlStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->slurControlStyles); }},
    {"tabSlideConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->tabSlideConnectStyles); }},
    {"glissandoConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->glissandoConnectStyles); }},
    {"bendCurveConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeOptions>& i) { populateEmbeddedClass(e, i->bendCurveConnectStyles); }},
};

template <>
inline const XmlElementArray<StemOptions> FieldPopulator<StemOptions>::xmlElements = {
    {"halfStemLength", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->halfStemLength = e->getTextAs<Evpu>(); }},
    {"stemLength", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemLength = e->getTextAs<Evpu>(); }},
    {"stem2", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->shortStemLength = e->getTextAs<Evpu>(); }},
    {"revStemAdj", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->revStemAdj = e->getTextAs<Evpu>(); }},
    {"noReverseStems", [](const XmlElementPtr&, const std::shared_ptr<StemOptions>& i) { i->noReverseStems = true; }},
    {"stemWidth", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemWidth = e->getTextAs<Efix>(); }},
    {"stemLift", [](const XmlElementPtr& e, const std::shared_ptr<StemOptions>& i) { i->stemOffset = e->getTextAs<Efix>(); }},
    {"useStemConnections", [](const XmlElementPtr&, const std::shared_ptr<StemOptions>& i) { i->useStemConnections = true; }},
};

template <>
inline XmlEnumMapping<TieOptions::SecondsPlacement> EnumMapper<TieOptions::SecondsPlacement>::mapping = {
    {"both", TieOptions::SecondsPlacement::ShiftForSeconds}
};

template <>
inline XmlEnumMapping<TieOptions::ChordTieDirType> EnumMapper<TieOptions::ChordTieDirType>::mapping = {
    {"stemReversal", TieOptions::ChordTieDirType::StemReversal},
    {"splitEvenly", TieOptions::ChordTieDirType::SplitEvenly}
};

template <>
inline XmlEnumMapping<TieOptions::MixedStemDirection> EnumMapper<TieOptions::MixedStemDirection>::mapping = {
    {"over", TieOptions::MixedStemDirection::Over},
    {"under", TieOptions::MixedStemDirection::Under},
    {"start", TieOptions::MixedStemDirection::OppositeFirst}
};

template <>
inline XmlEnumMapping<TieOptions::SpecialPosMode> EnumMapper<TieOptions::SpecialPosMode>::mapping = {
    {"avoid", TieOptions::SpecialPosMode::Avoid}
};

template <>
inline XmlEnumMapping<TieOptions::InsetStyle> EnumMapper<TieOptions::InsetStyle>::mapping = {
    {"percent", TieOptions::InsetStyle::Percent}
};

template <>
inline XmlEnumMapping<TieOptions::ConnectStyleType> EnumMapper<TieOptions::ConnectStyleType>::mapping = {
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
};

template <>
inline XmlEnumMapping<TieOptions::ControlStyleType> EnumMapper<TieOptions::ControlStyleType>::mapping = {
    {"shortSpan", TieOptions::ControlStyleType::ShortSpan},
    {"mediumSpan", TieOptions::ControlStyleType::MediumSpan},
    {"longSpan", TieOptions::ControlStyleType::LongSpan},
    {"tieEnds", TieOptions::ControlStyleType::TieEnds}
};

template <>
inline const XmlElementArray<TieOptions::ConnectStyle> FieldPopulator<TieOptions::ConnectStyle>::xmlElements = {
    {"offsetX", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetX = e->getTextAs<Evpu>(); }},
    {"offsetY", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ConnectStyle>& i) { i->offsetY = e->getTextAs<Evpu>(); }}
};

template <>
inline const XmlElementArray<TieOptions::ControlPoint> FieldPopulator<TieOptions::ControlPoint>::xmlElements = {
    {"insetRatio", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetRatio = e->getTextAs<Efix>(); }},
    {"height", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"insetFixed", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlPoint>& i) { i->insetFixed = e->getTextAs<Evpu>(); }}
};

template <>
inline const XmlElementArray<TieOptions::ControlStyle> FieldPopulator<TieOptions::ControlStyle>::xmlElements = {
    {"span", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->span = e->getTextAs<Evpu>(); }},
    {"cp1", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp1 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(e); }},
    {"cp2", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions::ControlStyle>& i) { i->cp2 = FieldPopulator<TieOptions::ControlPoint>::createAndPopulate(e); }}
};

template <>
inline const XmlElementArray<TieOptions> FieldPopulator<TieOptions>::xmlElements = {
    {"frontTieSepar", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->frontTieSepar = e->getTextAs<Evpu>(); }},
    {"thicknessRight", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessRight = e->getTextAs<Evpu>(); }},
    {"thicknessLeft", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->thicknessLeft = e->getTextAs<Evpu>(); }},
    {"breakForTimeSigs", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->breakForTimeSigs = true; }},
    {"breakForKeySigs", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->breakForKeySigs = true; }},
    {"breakTimeSigLeftHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakTimeSigRightHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakTimeSigRightHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigLeftHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigLeftHOffset = e->getTextAs<Evpu>(); }},
    {"breakKeySigRightHOffset", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->breakKeySigRightHOffset = e->getTextAs<Evpu>(); }},
    {"sysBreakLeftHAdj", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakLeftHAdj = e->getTextAs<Evpu>(); }},
    {"sysBreakRightHAdj", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->sysBreakRightHAdj = e->getTextAs<Evpu>(); }},
    {"useOuterPlacement", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->useOuterPlacement = true; }},
    {"secondsPlacement", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->secondsPlacement = toEnum<TieOptions::SecondsPlacement>(e->getTextTrimmed()); }},
    {"chordTieDirType", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->chordTieDirType = toEnum<TieOptions::ChordTieDirType>(e->getTextTrimmed()); }},
    {"chordTieDirOpposingSeconds", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->chordTieDirOpposingSeconds = true; }},
    {"mixedStemDirection", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->mixedStemDirection = toEnum<TieOptions::MixedStemDirection>(e->getTextTrimmed()); }},
    {"afterSingleDot", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->afterSingleDot = true; }},
    {"afterMultipleDots", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->afterMultipleDots = true; }},
    {"beforeAcciSingleNote", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->beforeAcciSingleNote = true; }},
    {"specialPosMode", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->specialPosMode = toEnum<TieOptions::SpecialPosMode>(e->getTextTrimmed()); }},
    {"avoidStaffLinesDistance", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesDistance = e->getTextAs<Evpu>(); }},
    {"insetStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->insetStyle = toEnum<TieOptions::InsetStyle>(e->getTextTrimmed()); }},
    {"useInterpolation", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->useInterpolation = true; }},
    {"useTieEndCtlStyle", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->useTieEndCtlStyle = true; }},
    {"avoidStaffLinesOnly", [](const XmlElementPtr&, const std::shared_ptr<TieOptions>& i) { i->avoidStaffLinesOnly = true; }},
    {"tieTipWidth", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { i->tieTipWidth = e->getTextAs<EvpuFloat>(); }},
    {"tieConnectStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(e, i->tieConnectStyles); }},
    {"tieControlStyle", [](const XmlElementPtr& e, const std::shared_ptr<TieOptions>& i) { populateEmbeddedClass(e, i->tieControlStyles); }}
};

template <>
inline const XmlElementArray<TimeSignatureOptions> FieldPopulator<TimeSignatureOptions>::xmlElements = {
    {"timeUpperLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLift = e->getTextAs<Evpu>(); }},
    {"timeFront", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFront = e->getTextAs<Evpu>(); }},
    {"timeBack", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBack = e->getTextAs<Evpu>(); }},
    {"timeFrontParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeFrontParts = e->getTextAs<Evpu>(); }},
    {"timeBackParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeBackParts = e->getTextAs<Evpu>(); }},
    {"timeUpperLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeUpperLiftParts = e->getTextAs<Evpu>(); }},
    {"timeLowerLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLiftParts = e->getTextAs<Evpu>(); }},
    {"timeAbrvLiftParts", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLiftParts = e->getTextAs<Evpu>(); }},
    {"timeSigDoAbrvCommon", [](const XmlElementPtr&, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCommon = true; }},
    {"timeSigDoAbrvCut", [](const XmlElementPtr&, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeSigDoAbrvCut = true; }},
    {"defFloat", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->numCompositeDecimalPlaces = e->getTextAs<int>(); }},
    {"cautionaryTimeChanges", [](const XmlElementPtr&, const std::shared_ptr<TimeSignatureOptions>& i) { i->cautionaryTimeChanges = true; }},
    {"timeLowerLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeLowerLift = e->getTextAs<Evpu>(); }},
    {"timeAbrvLift", [](const XmlElementPtr& e, const std::shared_ptr<TimeSignatureOptions>& i) { i->timeAbrvLift = e->getTextAs<Evpu>(); }},
};

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
