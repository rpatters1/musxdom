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
inline BeamOptions::FlattenStyle toEnum<BeamOptions::FlattenStyle>(const std::string& text)
{
    if (text == "onEndNotes") return BeamOptions::FlattenStyle::OnEndNotes;
    if (text == "onStandardNote") return BeamOptions::FlattenStyle::OnStandardNote;
    if (text == "onExtremeNote") return BeamOptions::FlattenStyle::OnExtremeNote;
    if (text == "alwaysFlat") return BeamOptions::FlattenStyle::AlwaysFlat;
    throw std::invalid_argument("Encountered unknown value for BeamOptions::FlattenStyle: " + text);
}

// Refactored FieldPopulator
template <>
inline const XmlElementArray<BeamOptions> FieldPopulator<BeamOptions>::xmlElements = {
    {"beamStubLength", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamStubLength = e->getTextAs<Evpu>(); }},
    {"maxSlope", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxSlope = e->getTextAs<Evpu>(); }},
    {"beamSepar", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamSepar = e->getTextAs<Evpu>(); }},
    {"hmBeamTip", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->maxFromMiddle = e->getTextAs<Evpu>(); }},
    {"beamingStyle", [](const XmlElementPtr& e, const std::shared_ptr<BeamOptions>& i) { i->beamingStyle = toEnum<BeamOptions::FlattenStyle>(e->getText()); }},
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
struct FieldPopulator<ClefOptions> : public FactoryBase
{
private:
    static void populateClefDef(const XmlElementPtr& element, const DocumentWeakPtr& document, ClefOptions::ClefDef& def)
    {
        getFieldFromXml(element, "adjust", def.middleCPos, [](auto el) { return el->template getTextAs<int>(); });
        getFieldFromXml(element, "clefChar", def.clefChar, [](auto el) { return el->template getTextAs<char32_t>(); });
        getFieldFromXml(element, "clefYDisp", def.staffPositon, [](auto el) { return el->template getTextAs<int>(); });
        getFieldFromXml(element, "baseAdjust", def.baselineAdjust, [](auto el) { return el->template getTextAs<Efix>(); });
        getFieldFromXml(element, "shapeID", def.shapeId, [](auto el) { return el->template getTextAs<int>(); }, false);
        getFieldFromXml(element, "isShape", def.isShape, [](auto) { return true; }, false);
        getFieldFromXml(element, "scaleToStaffHeight", def.scaleToStaffHeight, [](auto) { return true; }, false);
        def.font = FieldPopulator<FontInfo>::getFontFromXml(element, "font", document, false);
        if (def.useOwnFont && !def.font) {
            throw std::invalid_argument("Use own font was specified, but no font was found in the xml.");
        }
        getFieldFromXml(element, "useOwnFont", def.useOwnFont, [](auto) { return true; }, false);
    }

public:
    static void populate(const std::shared_ptr<ClefOptions>& instance, const XmlElementPtr& element, ElementLinker&)
    {
        getFieldFromXml(element, "defaultClef", instance->defaultClef, [](auto el) { return el->template getTextAs<int>(); });
        getFieldFromXml(element, "endMeasClefPercent", instance->clefChangePercent, [](auto el) { return el->template getTextAs<int>(); });
        getFieldFromXml(element, "endMeasClefPosAdd", instance->clefChangeOffset, [](auto el) { return el->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "clefFront", instance->clefFrontSepar, [](auto el) { return el->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "clefBack", instance->clefBackSepar, [](auto el) { return el->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "showClefFirstSystemOnly", instance->showClefFirstSystemOnly, [](auto) { return true; }, false);
        getFieldFromXml(element, "clefKey", instance->clefKeySepar, [](auto el) { return el->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "clefTime", instance->clefTimeSepar, [](auto el) { return el->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "cautionaryClefChanges", instance->cautionaryClefChanges, [](auto) { return true; }, false);

        size_t i = 0;
        for (auto clefDefElement = element->getFirstChildElement("clefDef");
             clefDefElement;
             clefDefElement = clefDefElement->getNextSibling("clefDef"), i++) {
            if (i >= instance->clefDefs.size()) {
                instance->clefDefs.resize(i + 1);
            }
            auto indexAttr = clefDefElement->findAttribute("index");
            size_t index = indexAttr ? indexAttr->getValueAs<size_t>() : -1;
            if (index != i) {
                throw std::invalid_argument("ClefDef index mismatch. Expected: " + std::to_string(i) + ", Found: " + std::to_string(index));
            }
            populateClefDef(clefDefElement, instance->getDocument(), instance->clefDefs[i]);
        }
        instance->clefDefs.resize(i);
    }
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
struct FieldPopulator<FontOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<FontOptions>& fonts, const XmlElementPtr& element, ElementLinker&)
    {
        auto fontElements = getFirstChildElement(element, "font");
        for (auto fontElement = getFirstChildElement(element, "font"); fontElement; fontElement = fontElement->getNextSibling("font")) {
            auto typeStr = fontElement->findAttribute("type");
            if (!typeStr) {
                throw std::invalid_argument("font option has no type");
            }
            FontType fontType = fromString(typeStr->getValue());

            auto fontInfo = std::make_shared<dom::FontInfo>(fonts->getDocument());
            FieldPopulator<dom::FontInfo>::populate(fontInfo, fontElement);

            // Add the populated font instance to the vector.
            fonts->fontOptions.emplace(fontType, fontInfo);
        }
    }

private:
    using FontType = FontOptions::FontType;

    static FontType fromString(const std::string& typeStr)
    {
        if (typeStr == "music") return FontType::Music;
        else if (typeStr == "key") return FontType::Key;
        else if (typeStr == "clef") return FontType::Clef;
        else if (typeStr == "time") return FontType::Time;
        else if (typeStr == "chord") return FontType::Chord;
        else if (typeStr == "chordAcci") return FontType::ChordAcci;
        else if (typeStr == "ending") return FontType::Ending;
        else if (typeStr == "tuplet") return FontType::Tuplet;
        else if (typeStr == "textBlock") return FontType::TextBlock;
        else if (typeStr == "lyricVerse") return FontType::LyricVerse;
        else if (typeStr == "lyricChorus") return FontType::LyricChorus;
        else if (typeStr == "lyricSection") return FontType::LyricSection;
        else if (typeStr == "multiMeasRest") return FontType::MultiMeasRest;
        else if (typeStr == "tablature") return FontType::Tablature;
        else if (typeStr == "chordSuffix") return FontType::ChordSuffix;
        else if (typeStr == "expression") return FontType::Expression;
        else if (typeStr == "repeat") return FontType::Repeat;
        else if (typeStr == "fretboard") return FontType::Fretboard;
        else if (typeStr == "flags") return FontType::Flags;
        else if (typeStr == "accis") return FontType::Accis;
        else if (typeStr == "altNotSlash") return FontType::AltNotSlash;
        else if (typeStr == "altNotNum") return FontType::AltNotNum;
        else if (typeStr == "rests") return FontType::Rests;
        else if (typeStr == "reptDots") return FontType::ReptDots;
        else if (typeStr == "noteheads") return FontType::Noteheads;
        else if (typeStr == "augDots") return FontType::AugDots;
        else if (typeStr == "timePlus") return FontType::TimePlus;
        else if (typeStr == "articulation") return FontType::Articulation;
        else if (typeStr == "percussion") return FontType::Percussion;
        else if (typeStr == "smartShape8va") return FontType::SmartShape8va;
        else if (typeStr == "measNumb") return FontType::MeasNumb;
        else if (typeStr == "staffNames") return FontType::StaffNames;
        else if (typeStr == "abbrvStaffNames") return FontType::AbbrvStaffNames;
        else if (typeStr == "groupNames") return FontType::GroupNames;
        else if (typeStr == "smartShape8vb") return FontType::SmartShape8vb;
        else if (typeStr == "smartShape15ma") return FontType::SmartShape15ma;
        else if (typeStr == "smartShape15mb") return FontType::SmartShape15mb;
        else if (typeStr == "smartShapeTrill") return FontType::SmartShapeTrill;
        else if (typeStr == "smartShapeWiggle") return FontType::SmartShapeWiggle;
        else if (typeStr == "abbrvGroupNames") return FontType::AbbrvGroupNames;
        else if (typeStr == "bendCurveFull") return FontType::BendCurveFull;
        else if (typeStr == "bendCurveWhole") return FontType::BendCurveWhole;
        else if (typeStr == "bendCurveFrac") return FontType::BendCurveFrac;
        else if (typeStr == "timeParts") return FontType::TimeParts;
        else if (typeStr == "timePlusParts") return FontType::TimePlusParts;
        else throw std::invalid_argument("Unknown FontType string: " + typeStr);
    }
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

#if 0
template <>
struct FieldPopulator<MiscOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<MiscOptions>& instance, const XmlElementPtr& element, ElementLinker&)
    {
        getFieldFromXml(element, "showRepeatsForParts", instance->showRepeatsForParts, [](auto) { return true; }, false);
        getFieldFromXml(element, "retainOctaveTransInConcertPitch", instance->keepOctaveTransInConcertPitch, [](auto) { return true; }, false);
        getFieldFromXml(element, "showCurrentLayerOnly", instance->showActiveLayerOnly, [](auto) { return true; }, false);
        getFieldFromXml(element, "combineRestsAcrossLayers", instance->consolidateRestsAcrossLayers, [](auto) { return true; }, false);
        getFieldFromXml(element, "sdDashOn", instance->shapeDesignerDashLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sdDashOff", instance->shapeDesignerDashSpace, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "drawMeasureNumbersOverBarlines", instance->alignMeasureNumbersWithBarlines, [](auto) { return true; }, false);
    }
};
#endif

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
inline MusicSpacingOptions::ColUnisonsChoice toEnum<MusicSpacingOptions::ColUnisonsChoice>(const std::string& value)
{
    if (value == "diffNoteheads") return MusicSpacingOptions::ColUnisonsChoice::DiffNoteheads;
    if (value == "all") return MusicSpacingOptions::ColUnisonsChoice::All;
    return MusicSpacingOptions::ColUnisonsChoice::None; // Default
}

template <>
inline MusicSpacingOptions::ManualPositioning toEnum<MusicSpacingOptions::ManualPositioning>(const std::string& value)
{
    if (value == "clear") return MusicSpacingOptions::ManualPositioning::Clear;
    if (value == "incorp") return MusicSpacingOptions::ManualPositioning::Incorporate;
    return MusicSpacingOptions::ManualPositioning::Ignore; // Default
}

template <>
inline MusicSpacingOptions::GraceNoteSpacing toEnum<MusicSpacingOptions::GraceNoteSpacing>(const std::string& value)
{
    if (value == "recomp") return MusicSpacingOptions::GraceNoteSpacing::Automatic;
    if (value == "keep") return MusicSpacingOptions::GraceNoteSpacing::KeepCurrent;
    return MusicSpacingOptions::GraceNoteSpacing::ResetToEntry; // Default
}

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
    {"avoidColUnisons", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColUnisons = toEnum<MusicSpacingOptions::ColUnisonsChoice>(e->getText()); }},
    {"avoidColLedgers", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->avoidColLedgers = true; }},
    {"manualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->manualPositioning = toEnum<MusicSpacingOptions::ManualPositioning>(e->getText()); }},
    {"ignoreHidden", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->ignoreHidden = true; }},
    {"useAllottmentTables", [](const XmlElementPtr&, const std::shared_ptr<MusicSpacingOptions>& i) { i->useAllottmentTables = true; }},
    {"referenceDuration", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"referenceWidth", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->referenceWidth = e->getTextAs<Evpu>(); }},
    {"scalingFactor", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->scalingFactor = e->getTextAs<double>(); }},
    {"defaultAllotment", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->defaultAllotment = e->getTextAs<Evpu>(); }},
    {"minDistGrace", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->minDistGrace = e->getTextAs<Evpu>(); }},
    {"graceNoteManualPositioning", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->graceNoteSpacing = toEnum<MusicSpacingOptions::GraceNoteSpacing>(e->getText()); }},
    {"musFront", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musFront = e->getTextAs<Evpu>(); }},
    {"musBack", [](const XmlElementPtr& e, const std::shared_ptr<MusicSpacingOptions>& i) { i->musBack = e->getTextAs<Evpu>(); }},
};

template <>
struct FieldPopulator<PageFormatOptions> : public FactoryBase {
    static void populate(const std::shared_ptr<PageFormatOptions>& instance, const XmlElementPtr& element, ElementLinker&)
    {
        getFieldFromXml(element, "pageFormatScore", instance->pageFormatScore, [](auto element) {
            auto pageFormat = std::make_shared<PageFormatOptions::PageFormat>();
            populatePageFormat(pageFormat, element);
            return pageFormat;
        });
        getFieldFromXml(element, "pageFormatParts", instance->pageFormatParts, [](auto element) {
            auto pageFormat = std::make_shared<PageFormatOptions::PageFormat>();
            populatePageFormat(pageFormat, element);
            return pageFormat;
        });
        getFieldFromXml(element, "avoidSystemMarginCollisions", instance->avoidSystemMarginCollisions, [](auto) { return true; }, false);
        getFieldFromXml(element, "adjustPageScope", instance->adjustPageScope, [](auto element) {
            return toAdjustPageScope(element->getText());
        });
    }

private:
    /**
     * @brief Converts a string to an AdjustPageScope enum value.
     * 
     * @param value The string representation of the scope.
     * @return PageFormatOptions::AdjustPageScope The corresponding enum value.
     */
    static PageFormatOptions::AdjustPageScope toAdjustPageScope(const std::string& value)
    {
        if (value == "current") {
            return PageFormatOptions::AdjustPageScope::Current;
        } else if (value == "all") {
            return PageFormatOptions::AdjustPageScope::All;
        } else if (value == "leftOrRight") {
            return PageFormatOptions::AdjustPageScope::LeftOrRight;
        } else if (value == "range") {
            return PageFormatOptions::AdjustPageScope::PageRange;
        }
        throw std::runtime_error("Invalid value for adjustPageScope: " + value);
    }

    static void populatePageFormat(const std::shared_ptr<PageFormatOptions::PageFormat>& instance, const XmlElementPtr& element)
    {
        getFieldFromXml(element, "pageHeight", instance->pageHeight, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "pageWidth", instance->pageWidth, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "pagePercent", instance->pagePercent, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "sysPercent", instance->sysPercent, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "rawStaffHeight", instance->rawStaffHeight, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "leftPageMarginTop", instance->leftPageMarginTop, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "leftPageMarginLeft", instance->leftPageMarginLeft, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "leftPageMarginBottom", instance->leftPageMarginBottom, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "leftPageMarginRight", instance->leftPageMarginRight, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "rightPageMarginTop", instance->rightPageMarginTop, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "rightPageMarginLeft", instance->rightPageMarginLeft, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "rightPageMarginBottom", instance->rightPageMarginBottom, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "rightPageMarginRight", instance->rightPageMarginRight, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sysMarginTop", instance->sysMarginTop, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sysMarginLeft", instance->sysMarginLeft, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sysMarginBottom", instance->sysMarginBottom, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sysMarginRight", instance->sysMarginRight, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "sysDistanceBetween", instance->sysDistanceBetween, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "firstPageMarginTop", instance->firstPageMarginTop, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "firstSysMarginTop", instance->firstSysMarginTop, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "firstSysMarginLeft", instance->firstSysMarginLeft, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "firstSysMarginDistance", instance->firstSysMarginDistance, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "facingPages", instance->facingPages, [](auto) { return true; }, false);
        getFieldFromXml(element, "differentFirstSysMargin", instance->differentFirstSysMargin, [](auto) { return true; }, false);
        getFieldFromXml(element, "differentFirstPageMargin", instance->differentFirstPageMargin, [](auto) { return true; }, false);
    }
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
inline RepeatOptions::WingStyle toEnum<RepeatOptions::WingStyle>(const std::string& str)
{
    if (str == "none") return RepeatOptions::WingStyle::None;
    if (str == "curved") return RepeatOptions::WingStyle::Curved;
    if (str == "singleLine") return RepeatOptions::WingStyle::SingleLine;
    if (str == "doubleLine") return RepeatOptions::WingStyle::DoubleLine;
    throw std::invalid_argument("Invalid wing style value in XML: " + str);
}

template <>
inline RepeatOptions::BackToBackStyle toEnum<RepeatOptions::BackToBackStyle>(const std::string& str)
{
    if (str == "thin") return RepeatOptions::BackToBackStyle::Thin;
    if (str == "mixed") return RepeatOptions::BackToBackStyle::Mixed;
    if (str == "thick") return RepeatOptions::BackToBackStyle::Thick;
    throw std::invalid_argument("Invalid back-to-back style value in XML: " + str);
}

template <>
inline const XmlElementArray<RepeatOptions> FieldPopulator<RepeatOptions>::xmlElements = {
    {"bracketHeight", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->bracketHeight = e->getTextAs<Evpu>(); }},
    {"maxPasses", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->maxPasses = e->getTextAs<int>(); }},
    {"addPeriod", [](const XmlElementPtr&, const std::shared_ptr<RepeatOptions>& i) { i->addPeriod = true; }},
    {"thickLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thickLineWidth = e->getTextAs<Efix>(); }},
    {"thinLineWidth", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->thinLineWidth = e->getTextAs<Efix>(); }},
    {"lineSpace", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lineSpace = e->getTextAs<Efix>(); }},
    {"backToBackStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backToBackStyle = toEnum<RepeatOptions::BackToBackStyle>(e->getText()); }},
    {"forwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->forwardDotHPos = e->getTextAs<Evpu>(); }},
    {"backwardDotHPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->backwardDotHPos = e->getTextAs<Evpu>(); }},
    {"upperDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->upperDotVPos = e->getTextAs<Evpu>(); }},
    {"lowerDotVPos", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->lowerDotVPos = e->getTextAs<Evpu>(); }},
    {"wingStyle", [](const XmlElementPtr& e, const std::shared_ptr<RepeatOptions>& i) { i->wingStyle = toEnum<RepeatOptions::WingStyle>(e->getText()); }},
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
