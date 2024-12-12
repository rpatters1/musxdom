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

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

template <>
struct FieldPopulator<AlternateNotationOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<AlternateNotationOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "halfSlashLift", instance->halfSlashLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "wholeSlashLift", instance->wholeSlashLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "dWholeSlashLift", instance->dWholeSlashLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "halfSlashStemLift", instance->halfSlashStemLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "quartSlashStemLift", instance->quartSlashStemLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "quartSlashLift", instance->quartSlashLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "twoMeasNumLift", instance->twoMeasNumLift, [](auto element) { return element->template getTextAs<Evpu>(); });
    }
};

template <>
struct FieldPopulator<BarlineOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<BarlineOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "drawCloseSystemBarline", instance->drawCloseSystemBarline, [](auto) { return true; }, false);
        getFieldFromXml(element, "drawCloseFinalBarline", instance->drawCloseFinalBarline, [](auto) { return true; }, false);
        getFieldFromXml(element, "drawFinalBarlineOnLastMeas", instance->drawFinalBarlineOnLastMeas, [](auto) { return true; }, false);
        getFieldFromXml(element, "leftBarlineUsePrevStyle", instance->leftBarlineUsePrevStyle, [](auto) { return true; }, false);
        getFieldFromXml(element, "drawLeftBarlineSingleStaff", instance->drawLeftBarlineSingleStaff, [](auto) { return true; }, false);
        getFieldFromXml(element, "drawLeftBarlineMultipleStaves", instance->drawLeftBarlineMultipleStaves, [](auto) { return true; }, false);
        getFieldFromXml(element, "drawBarlines", instance->drawBarlines, [](auto) { return true; }, false);
        getFieldFromXml(element, "barlineWidth", instance->barlineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "thickBarlineWidth", instance->thickBarlineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "doubleBarlineSpace", instance->doubleBarlineSpace, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "finalBarlineSpace", instance->finalBarlineSpace, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "barlineDashOn", instance->barlineDashOn, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "barlineDashOff", instance->barlineDashOff, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "drawDoubleBarlineBeforeKeyChanges", instance->drawDoubleBarlineBeforeKeyChanges, [](auto) { return true; }, false);
    }
};

template <>
struct FieldPopulator<ClefOptions> : public FactoryBase
{
private:
    static void populateClefDef(const std::shared_ptr<xml::IXmlElement>& element, const DocumentWeakPtr& document, ClefOptions::ClefDef& def)
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
    static void populate(const std::shared_ptr<ClefOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
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
struct FieldPopulator<FlagOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<FlagOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "straightFlags", instance->straightFlags, [](auto) { return true; }, false);
        getFieldFromXml(element, "upHAdj", instance->upHAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "downHAdj", instance->downHAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "upHAdj2", instance->upHAdj2, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "downHAdj2", instance->downHAdj2, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "upHAdj16", instance->upHAdj16, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "downHAdj16", instance->downHAdj16, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "eighthFlagHoist", instance->eighthFlagHoist, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "stUpHAdj", instance->stUpHAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "stDownHAdj", instance->stDownHAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "upVAdj", instance->upVAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "downVAdj", instance->downVAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "upVadj2", instance->upVAdj2, [](auto element) { return element->template getTextAs<Efix>(); }); // "upVadj2" is not a typo!
        getFieldFromXml(element, "downVAdj2", instance->downVAdj2, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "upVAdj16", instance->upVAdj16, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "downVAdj16", instance->downVAdj16, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "stUpVAdj", instance->stUpVAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "stDownVAdj", instance->stDownVAdj, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "separ", instance->flagSpacing, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "extra", instance->secondaryGroupAdj, [](auto element) { return element->template getTextAs<Evpu>(); });
    }
};

template <>
struct FieldPopulator<FontOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<FontOptions>& fonts, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
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
struct FieldPopulator<GraceNoteOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<GraceNoteOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "tabGracePerc", instance->tabGracePerc, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "gracePerc", instance->gracePerc, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "playbackDuration", instance->playbackDuration, [](auto element) { return element->template getTextAs<Edu>(); });
        getFieldFromXml(element, "graceBackup", instance->entryOffset, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "slashFlaggedGraceNotes", instance->slashFlaggedGraceNotes, [](auto) { return true; }, false);
        getFieldFromXml(element, "graceSlashWidth", instance->graceSlashWidth, [](auto element) { return element->template getTextAs<Efix>(); });
    }
};

template <>
struct FieldPopulator<KeySignatureOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<KeySignatureOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "doKeyCancel", instance->doKeyCancel, [](auto) { return true; }, false);
        getFieldFromXml(element, "doCStart", instance->doCStart, [](auto) { return true; }, false);
        getFieldFromXml(element, "doBankDiff", instance->redisplayOnModeChange, [](auto) { return true; }, false);
        getFieldFromXml(element, "keyFront", instance->keyFront, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "keyMid", instance->keyMid, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "keyBack", instance->keyBack, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "acciAdd", instance->acciAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "showKeyFirstSystemOnly", instance->showKeyFirstSystemOnly, [](auto) { return true; }, false);
        getFieldFromXml(element, "keyTime", instance->keyTimeSepar, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "simplifyKeyHoldOctave", instance->simplifyKeyHoldOctave, [](auto) { return true; }, false);
        getFieldFromXml(element, "cautionaryKeyChanges", instance->cautionaryKeyChanges, [](auto) { return true; }, false);
        getFieldFromXml(element, "doKeyCancelBetweenSharpsFlats", instance->doKeyCancelBetweenSharpsFlats, [](auto) { return true; }, false);
    }
};

template <>
struct FieldPopulator<LineCurveOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<LineCurveOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "bezierStep", instance->bezierStep, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "enclosureWidth", instance->enclosureWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "enclosureRoundCorners", instance->enclosureRoundCorners, [](auto) { return true; }, false);
        getFieldFromXml(element, "enclosureCornerRadius", instance->enclosureCornerRadius, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "staffLineWidth", instance->staffLineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "legerLineWidth", instance->legerLineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "legerFrontLength", instance->legerFrontLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "legerBackLength", instance->legerBackLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "restLegerFrontLength", instance->restLegerFrontLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "restLegerBackLength", instance->restLegerBackLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "psUlDepth", instance->psUlDepth, [](auto element) { return element->template getTextAs<double>(); });
        getFieldFromXml(element, "psUlWidth", instance->psUlWidth, [](auto element) { return element->template getTextAs<double>(); });
        getFieldFromXml(element, "pathSlurTipWidth", instance->pathSlurTipWidth, [](auto element) { return element->template getTextAs<EvpuFloat>(); });
    }
};

template <>
struct FieldPopulator<MusicSpacingOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<MusicSpacingOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "minWidth", instance->minWidth, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "maxWidth", instance->maxWidth, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "minDistance", instance->minDistance, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "minDistTiedNotes", instance->minDistTiedNotes, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "avoidColNotes", instance->avoidColNotes, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColLyrics", instance->avoidColLyrics, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColChords", instance->avoidColChords, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColArtics", instance->avoidColArtics, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColClefs", instance->avoidColClefs, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColSeconds", instance->avoidColSeconds, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColStems", instance->avoidColStems, [](auto) { return true; }, false);
        getFieldFromXml(element, "avoidColUnisons", instance->avoidColUnisons, [](auto element) { return toColUnisonsChoice(element->getText()); });
        getFieldFromXml(element, "avoidColLedgers", instance->avoidColLedgers, [](auto) { return true; }, false);
        getFieldFromXml(element, "manualPositioning", instance->manualPositioning, [](auto element) { return toManualPositioning(element->getText()); });
        getFieldFromXml(element, "ignoreHidden", instance->ignoreHidden, [](auto) { return true; }, false);
        getFieldFromXml(element, "useAllottmentTables", instance->useAllottmentTables, [](auto) { return true; }, false);
        getFieldFromXml(element, "referenceDuration", instance->referenceDuration, [](auto element) { return element->template getTextAs<Edu>(); });
        getFieldFromXml(element, "referenceWidth", instance->referenceWidth, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "scalingFactor", instance->scalingFactor, [](auto element) { return element->template getTextAs<double>(); });
        getFieldFromXml(element, "defaultAllotment", instance->defaultAllotment, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "minDistGrace", instance->minDistGrace, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "graceNoteManualPositioning", instance->graceNoteSpacing, [](auto element) { return toGraceNoteSpacing(element->getText()); });
        getFieldFromXml(element, "musFront", instance->musFront, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "musBack", instance->musBack, [](auto element) { return element->template getTextAs<Evpu>(); });
    }

private:
    static MusicSpacingOptions::ColUnisonsChoice toColUnisonsChoice(const std::string& value)
    {
        if (value == "diffNoteheads") return MusicSpacingOptions::ColUnisonsChoice::DiffNoteheads;
        if (value == "all") return MusicSpacingOptions::ColUnisonsChoice::All;
        return MusicSpacingOptions::ColUnisonsChoice::None; // Default
    }

    static MusicSpacingOptions::ManualPositioning toManualPositioning(const std::string& value)
    {
        if (value == "clear") return MusicSpacingOptions::ManualPositioning::Clear;
        if (value == "incorp") return MusicSpacingOptions::ManualPositioning::Incorporate;
        return MusicSpacingOptions::ManualPositioning::Ignore; // Default
    }

    static MusicSpacingOptions::GraceNoteSpacing toGraceNoteSpacing(const std::string& value)
    {
        if (value == "recomp") return MusicSpacingOptions::GraceNoteSpacing::Automatic;
        if (value == "keep") return MusicSpacingOptions::GraceNoteSpacing::KeepCurrent;
        return MusicSpacingOptions::GraceNoteSpacing::ResetToEntry; // Default
    }
};

template <>
struct FieldPopulator<PageFormatOptions> : public FactoryBase {
    static void populate(const std::shared_ptr<PageFormatOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
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

    static void populatePageFormat(const std::shared_ptr<PageFormatOptions::PageFormat>& instance, const std::shared_ptr<xml::IXmlElement>& element)
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
struct FieldPopulator<PianoBraceBracketOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<PianoBraceBracketOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "defBracketPos", instance->defBracketPos, [](auto elem) { return elem->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "centerThickness", instance->centerThickness, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "endThickness", instance->tipThickness, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "outerWingY", instance->outerBodyV, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "innerTipY", instance->innerTipV, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "innerWingY", instance->innerBodyV, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "outerTipX", instance->outerTipH, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "outerTipY", instance->outerTipV, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "outerWingX", instance->outerBodyH, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "width", instance->width, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "innerTipX", instance->innerTipH, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
        getFieldFromXml(element, "innerWingX", instance->innerBodyH, [](auto elem) { return elem->template getTextAs<EvpuFloat>(); });
    }
};

template <>
struct FieldPopulator<RepeatOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<RepeatOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "bracketHeight", instance->bracketHeight, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "maxPasses", instance->maxPasses, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "addPeriod", instance->addPeriod, [](auto) { return true; }, false);
        getFieldFromXml(element, "thickLineWidth", instance->thickLineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "thinLineWidth", instance->thinLineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "lineSpace", instance->lineSpace, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "backToBackStyle", instance->backToBackStyle, [](auto element) { return toBackToBackStyle(element->getText()); });
        getFieldFromXml(element, "forwardDotHPos", instance->forwardDotHPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "backwardDotHPos", instance->backwardDotHPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "upperDotVPos", instance->upperDotVPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "lowerDotVPos", instance->lowerDotVPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "wingStyle", instance->wingStyle, [](auto element) { return toWingStyle(element->getText()); });
        getFieldFromXml(element, "afterClefSpace", instance->afterClefSpace, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "afterKeySpace", instance->afterKeySpace, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "afterTimeSpace", instance->afterTimeSpace, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketHookLen", instance->bracketHookLen, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketLineWidth", instance->bracketLineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "bracketStartInset", instance->bracketStartInset, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketEndInset", instance->bracketEndInset, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketTextHPos", instance->bracketTextHPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketTextVPos", instance->bracketTextVPos, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketEndHookLen", instance->bracketEndHookLen, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "bracketEndAnchorThinLine", instance->bracketEndAnchorThinLine, [](auto) { return true; }, false);
        getFieldFromXml(element, "showOnStaffListNumber", instance->showOnStaffListNumber, [](auto element) { return element->template getTextAs<Cmper>(); });
    }

private:
    static RepeatOptions::WingStyle toWingStyle(const std::string& str)
    {
        // there may not be a "none" value ever generated in the xml, but this is the default the element is missing
        if (str == "none") return RepeatOptions::WingStyle::None;
        if (str == "curved") return RepeatOptions::WingStyle::Curved;
        if (str == "singleLine") return RepeatOptions::WingStyle::SingleLine;
        if (str == "doubleLine") return RepeatOptions::WingStyle::DoubleLine;
        throw std::invalid_argument("Invalid wing style value in XML: " + str);
    }

    static RepeatOptions::BackToBackStyle toBackToBackStyle(const std::string& str)
    {
        // there may not be a "thin" value ever generated in the xml, but this is the default the element is missing
        if (str == "thin") return RepeatOptions::BackToBackStyle::Thin;
        if (str == "mixed") return RepeatOptions::BackToBackStyle::Mixed;
        if (str == "thick") return RepeatOptions::BackToBackStyle::Thick;
        throw std::invalid_argument("Invalid back-to-back style value in XML: " + str);
    }
};

template <>
struct FieldPopulator<StemOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<StemOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "halfStemLength", instance->halfStemLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "stemLength", instance->stemLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "stem2", instance->shortStemLength, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "revStemAdj", instance->revStemAdj, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "noReverseStems", instance->noReverseStems, [](auto) { return true; }, false);
        getFieldFromXml(element, "stemWidth", instance->stemWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "stemLift", instance->stemOffset, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "useStemConnections", instance->useStemConnections, [](auto) { return true; }, false);
    }
};

template <>
struct FieldPopulator<TimeSignatureOptions> : public FactoryBase
{
    static void populate(const std::shared_ptr<TimeSignatureOptions>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "timeUpperLift", instance->timeUpperLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeFront", instance->timeFront, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeBack", instance->timeBack, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeFrontParts", instance->timeFrontParts, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeBackParts", instance->timeBackParts, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeUpperLiftParts", instance->timeUpperLiftParts, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeLowerLiftParts", instance->timeLowerLiftParts, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeAbrvLiftParts", instance->timeAbrvLiftParts, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeSigDoAbrvCommon", instance->timeSigDoAbrvCommon, [](auto) { return true; }, false);
        getFieldFromXml(element, "timeSigDoAbrvCut", instance->timeSigDoAbrvCut, [](auto) { return true; }, false);
        getFieldFromXml(element, "defFloat", instance->numCompositeDecimalPlaces, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "cautionaryTimeChanges", instance->cautionaryTimeChanges, [](auto) { return true; }, false);
        getFieldFromXml(element, "timeLowerLift", instance->timeLowerLift, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "timeAbrvLift", instance->timeAbrvLift, [](auto element) { return element->template getTextAs<Evpu>(); });
    }
};

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
