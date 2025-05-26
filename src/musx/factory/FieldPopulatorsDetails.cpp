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
using namespace ::musx::dom::details;

extern template const XmlEnumMappingElement<ShowClefMode> XmlEnumMapping<ShowClefMode>::mapping;
extern template const XmlEnumMappingElement<others::Measure::BarlineType> XmlEnumMapping<others::Measure::BarlineType>::mapping;
extern template const XmlEnumMappingElement<others::NamePositioning::AlignJustify> XmlEnumMapping<others::NamePositioning::AlignJustify>::mapping;
extern template const XmlEnumMappingElement<options::BeamOptions::FlattenStyle> XmlEnumMapping<options::BeamOptions::FlattenStyle>::mapping;
extern template const XmlEnumMappingElement<options::LyricOptions::AlignJustify> XmlEnumMapping<options::LyricOptions::AlignJustify>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::AutoBracketStyle> XmlEnumMapping<options::TupletOptions::AutoBracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::BracketStyle> XmlEnumMapping<options::TupletOptions::BracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::NumberStyle> XmlEnumMapping<options::TupletOptions::NumberStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::PositioningStyle> XmlEnumMapping<options::TupletOptions::PositioningStyle>::mapping;

MUSX_XML_ENUM_MAPPING(ChordAssign::BassPosition, {
    {"afterRoot", ChordAssign::BassPosition::AfterRoot},   // this value may never appear in the xml
    {"underRoot", ChordAssign::BassPosition::UnderRoot},
    {"subtext",   ChordAssign::BassPosition::Subtext}
});

MUSX_XML_ENUM_MAPPING(StaffGroup::HideStaves, {
    // {"normally", StaffGroup::HideStaves::Normally}, // Default value, may not appear in the XML
    {"asGroup", StaffGroup::HideStaves::AsGroup},
    {"none", StaffGroup::HideStaves::None}
});

MUSX_XML_ENUM_MAPPING(StaffGroup::DrawBarlineStyle, {
    // {"onlyOnStaves", StaffGroup::DrawBarlineStyle::OnlyOnStaves}, // Default value, may not appear in the XML
    {"group", StaffGroup::DrawBarlineStyle::ThroughStaves},
    {"Mensurstriche", StaffGroup::DrawBarlineStyle::Mensurstriche}
});

} // namespace factory

namespace dom {
namespace details {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(AccidentalAlterations, {
    {"noteID", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->percent = e->getTextAs<int>(); }},
    {"ayDisp", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->vOffset = e->getTextAs<Evpu>(); }},
    {"axDisp", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->hOffset = e->getTextAs<Evpu>(); }},
    {"altChar", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->altChar = e->getTextAs<char32_t>(); }},
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"useOwnFont", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->useOwnFont = populateBoolean(e, i); }},
    {"allowVertPos", [](const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->allowVertPos = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ArticulationAssign, {
    {"articDef", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->articDef = e->getTextAs<Cmper>(); }},
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"horzAdd", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->horzAdd = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"vertAdd", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->vertAdd = e->getTextAs<Evpu>(); }},
    {"overridePlacement", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->overridePlacement = populateBoolean(e, i); }},
    {"aboveEntry", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->aboveEntry = populateBoolean(e, i); }},
    {"hide", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->hide = populateBoolean(e, i); }},
    {"neverStack", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->neverStack = populateBoolean(e, i); }},
    {"avoidSlur", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->avoidSlur = populateBoolean(e, i); }},
    {"numSlursAvoided", [](const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->numSlursAvoided = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Baseline, {
    {"basedisp", [](const XmlElementPtr& e, const std::shared_ptr<Baseline>& i) { i->baselineDisplacement = e->getTextAs<Evpu>(); }},
    {"lyricNumber", [](const XmlElementPtr& e, const std::shared_ptr<Baseline>& i) { i->lyricNumber = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamAlterations, {
    {"xAdd", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->leftOffsetH = e->getTextAs<Evpu>(); }},
    {"yAdd", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->leftOffsetY = e->getTextAs<Evpu>(); }},
    {"sxAdd", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->rightOffsetH = e->getTextAs<Evpu>(); }},
    {"syAdd", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->rightOffsetY = e->getTextAs<Evpu>(); }},
    {"dura", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->dura = e->getTextAs<Edu>(); }},
    {"context", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->flattenStyle = toEnum<BeamAlterations::FlattenStyle>(e); }},
    {"beamWidth", [](const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->beamWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamExtension, {
    {"x3Disp", [](const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->leftOffset = e->getTextAs<Evpu>(); }},
    {"x4Disp", [](const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->rightOffset = e->getTextAs<Evpu>(); }},
    {"do8th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Eighth); }},
    {"do16th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
    {"extBeyond8th", [](const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->extBeyond8th = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamStubDirection, {
    {"do8th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Eighth); }},
    {"do16th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
});

MUSX_XML_ELEMENT_ARRAY(CenterShape, {
    { "startBreakAdj", [](const XmlElementPtr& e, const std::shared_ptr<CenterShape>& i)
        { i->startBreakAdj = FieldPopulator<others::SmartShape::EndPointAdjustment>::createAndPopulate(e, i->getDocument()); }},
    { "endBreakAdj", [](const XmlElementPtr& e, const std::shared_ptr<CenterShape>& i)
        { i->endBreakAdj = FieldPopulator<others::SmartShape::EndPointAdjustment>::createAndPopulate(e, i->getDocument()); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordAssign, {
    {"suffix", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->suffixId = e->getTextAs<Cmper>(); }},
    {"fbStyleID", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbStyleId = e->getTextAs<Cmper>(); }},
    {"rootScaleNum", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootScaleNum = e->getTextAs<int>(); }},
    {"rootAlter", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootAlter = e->getTextAs<int>(); }},
    {"rootLowerCase", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootLowerCase = populateBoolean(e, i); }},
    {"playSuffix", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playSuffix = populateBoolean(e, i); }},
    {"showRoot", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showRoot = populateBoolean(e, i); }},
    {"playRoot", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playRoot = populateBoolean(e, i); }},
    {"showFretboard", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showFretboard = populateBoolean(e, i); }},
    {"showSuffix", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showSuffix = populateBoolean(e, i); }},
    {"playFretboard", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playFretboard = populateBoolean(e, i); }},
    {"bassScaleNum", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassScaleNum = e->getTextAs<int>(); }},
    {"bassAlter", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassAlter = e->getTextAs<int>(); }},
    {"bassLowerCase", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassLowerCase = populateBoolean(e, i); }},
    {"bassPosition", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassPosition = toEnum<ChordAssign::BassPosition>(e); }},
    {"showAltBass", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showAltBass = populateBoolean(e, i); }},
    {"playAltBass", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playAltBass = populateBoolean(e, i); }},
    {"capoValue", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->capoValue = e->getTextAs<int>(); }},
    {"useLocalCapo", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->useLocalCapo = populateBoolean(e, i); }},
    {"fretInci", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fretInci = e->getTextAs<Cmper>(); }},
    {"useFretFont", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->useFretFont = populateBoolean(e, i); }},
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->horzOff = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->vertOff = e->getTextAs<Evpu>(); }},
    {"fbHorzOff", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbHorzOff = e->getTextAs<Evpu>(); }},
    {"fbVertOff", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbVertOff = e->getTextAs<Evpu>(); }},
    {"horzEdu", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->horzEdu = e->getTextAs<Edu>(); }},
    {"chPercent", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->chPercent = e->getTextAs<int>(); }},
    {"fbPercent", [](const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbPercent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(CrossStaff, {
    {"noteID", [](const XmlElementPtr& e, const std::shared_ptr<CrossStaff>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"instrument", [](const XmlElementPtr& e, const std::shared_ptr<CrossStaff>& i) { i->staff = e->getTextAs<InstCmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(CustomStem, {
    {"shapeDef", [](const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"xdisp",    [](const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"ydisp",    [](const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(EntrySize, {
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<EntrySize>& i) { i->percent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(GFrameHold, {
    {"clefID", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefId = e->getTextAs<ClefIndex>(); }},
    {"clefListID", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefListId = e->getTextAs<Cmper>(); }},
    {"clefMode", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->showClefMode = toEnum<ShowClefMode>(e); }},
    {"mirrorFrame", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->mirrorFrame = populateBoolean(e, i); }},
    {"clefAfterBarline", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefAfterBarline = populateBoolean(e, i); }},
    {"clefPercent", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefPercent = e->getTextAs<int>(); }},
    {"frame1", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[0] = e->getTextAs<Cmper>(); }},
    {"frame2", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[1] = e->getTextAs<Cmper>(); }},
    {"frame3", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[2] = e->getTextAs<Cmper>(); }},
    {"frame4", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[3] = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(IndependentStaffDetails, {
    {"keySig", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i)
        { i->keySig = FieldPopulator<KeySignature>::createAndPopulate(e, i->getDocument()); }},
    {"hasKey", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasKey = populateBoolean(e, i); }},
    {"beats", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divBeat", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivBeat", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->dispDivBeat = e->getTextAs<Cmper>(); }},
    {"displayAltNumTsig", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAltNumTsig = populateBoolean(e, i); }},
    {"displayAltDenTsig", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAltDenTsig = populateBoolean(e, i); }},
    {"altNumTsig", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->altNumTsig = populateBoolean(e, i); }},
    {"altDenTsig", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->altDenTsig = populateBoolean(e, i); }},
    {"displayAbbrvTime", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAbbrvTime = populateBoolean(e, i); }},
    {"hasDispTime", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasDispTime = populateBoolean(e, i); }},
    {"hasTime", [](const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasTime = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricAssign, {
    {"lyricNumber", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->lyricNumber = e->getTextAs<Cmper>(); }},
    {"syll", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->syllable = e->getTextAs<unsigned>(); }},
    {"horzOff", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"vertOff", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"floatingHorzOff", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->floatingHorzOff = e->getTextAs<Evpu>(); }},
    {"wext", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->wext = e->getTextAs<int>(); }},
    {"displayVerseNum", [](const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->displayVerseNum = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricEntryInfo, {
    {"justify", [](const XmlElementPtr& e, const std::shared_ptr<LyricEntryInfo>& i) { i->justify = toEnum<LyricEntryInfo::AlignJustify>(e); }},
    {"align",   [](const XmlElementPtr& e, const std::shared_ptr<LyricEntryInfo>& i) { i->align   = toEnum<LyricEntryInfo::AlignJustify>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureTextAssign, {
    {"block", [](const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->block = e->getTextAs<Cmper>(); }},
    {"xdispEdu", [](const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->xDispEdu = e->getTextAs<Edu>(); }},
    {"xdispEvpu", [](const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->xDispEvpu = e->getTextAs<Evpu>(); }},
    {"ydisp", [](const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->yDisp = e->getTextAs<Evpu>(); }},
    {"postIt", [](const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->hidden = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteAlterations, {
    {"noteID", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"percent", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->percent = e->getTextAs<int>(); }},
    {"nxdisp", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->nxdisp = e->getTextAs<Evpu>(); }},
    {"altNhead", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->altNhead = e->getTextAs<char32_t>(); }},
    {"useOwnFont", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->useOwnFont = populateBoolean(e, i); }},
    {"fontID", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"fontSize", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"efx", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(i->customFont, e); }},
    {"allowVertPos", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->allowVertPos = populateBoolean(e, i); }},
    {"nydisp", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->nydisp = e->getTextAs<Evpu>(); }},
    {"enharmonic", [](const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->enharmonic = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(SecondaryBeamBreak, {
    {"do16th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
    {"beamThrough", [](const XmlElementPtr& e, const std::shared_ptr<SecondaryBeamBreak>& i) { i->breakThrough = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffGroup::Bracket, {
    {"id", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->style = toEnum<StaffGroup::BracketStyle>(e->getTextAs<int>()); }},
    {"bracPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->horzAdjLeft = e->getTextAs<Evpu>(); }},
    {"bracTop", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->vertAdjTop = e->getTextAs<Evpu>(); }},
    {"bracBot", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->vertAdjBot = e->getTextAs<Evpu>(); }},
    {"onSingle", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->showOnSingleStaff = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeEntryAssign, {
    {"shapeNum", [](const XmlElementPtr& e, const std::shared_ptr<SmartShapeEntryAssign>& i) { i->shapeNum = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffGroup, {
    {"startInst", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->startInst = e->getTextAs<InstCmper>(); }},
    {"endInst", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->endInst = e->getTextAs<InstCmper>(); }},
    {"startMeas", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"fullID", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameId = e->getTextAs<Cmper>(); }},
    {"fullXadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameXadj = e->getTextAs<int>(); }},
    {"fullYadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameYadj = e->getTextAs<int>(); }},
    {"bracket", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->bracket = FieldPopulator<StaffGroup::Bracket>::createAndPopulate(e); }},
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->barlineType = toEnum<StaffGroup::BarlineType>(e); }},
    {"fullJustify", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameJustify = toEnum<StaffGroup::AlignJustify>(e); }},
    {"abbrvJustify", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameJustify = toEnum<StaffGroup::AlignJustify>(e); }},
    {"groupBarlineStyle", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->drawBarlines = toEnum<StaffGroup::DrawBarlineStyle>(e); }},
    {"ownBarline", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->ownBarline = populateBoolean(e, i); }},
    {"fullIndivPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameIndivPos = populateBoolean(e, i); }},
    {"abbrvIndivPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameIndivPos = populateBoolean(e, i); }},
    {"hideName", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->hideName = populateBoolean(e, i); }},
    {"abbrvID", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameId = e->getTextAs<Cmper>(); }},
    {"abbrvXadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameXadj = e->getTextAs<int>(); }},
    {"abbrvYadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameYadj = e->getTextAs<int>(); }},
    {"fullHAlign", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameAlign = toEnum<StaffGroup::AlignJustify>(e); }},
    {"abbrvHAlign", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameAlign = toEnum<StaffGroup::AlignJustify>(e); }},
    {"fullExpand", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameExpand = populateBoolean(e, i); }},
    {"abbrvExpand", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameExpand = populateBoolean(e, i); }},
    {"optimize", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->hideStaves = toEnum<StaffGroup::HideStaves>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffSize, {
    {"staffPercent", [](const XmlElementPtr& e, const std::shared_ptr<StaffSize>& i) { i->staffPercent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StemAdjustments, {
    {"upVertAdjust",   [](const XmlElementPtr& e, const std::shared_ptr<StemAdjustments>& i) { i->upVertAdjust = e->getTextAs<Evpu>(); }},
    {"downVertAdjust", [](const XmlElementPtr& e, const std::shared_ptr<StemAdjustments>& i) { i->downVertAdjust = e->getTextAs<Evpu>(); }},
    {"upHorzAdjust",   [](const XmlElementPtr& e, const std::shared_ptr<StemAdjustments>& i) { i->upHorzAdjust = e->getTextAs<Evpu>(); }},
    {"downHorzAdjust", [](const XmlElementPtr& e, const std::shared_ptr<StemAdjustments>& i) { i->downHorzAdjust = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TieAlterBase, {
    {"noteID", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"xStart", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->xStart = e->getTextAs<Evpu>(); }},
    {"xEnd", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->xEnd = e->getTextAs<Evpu>(); }},
    {"yStart", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->yStart = e->getTextAs<Evpu>(); }},
    {"yEnd", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->yEnd = e->getTextAs<Evpu>(); }},
    {"outerLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->outerLocal = populateBoolean(e, i); }},
    {"afterSingleDotLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterSingleDotLocal = populateBoolean(e, i); }},
    {"afterSingleDotOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterSingleDotOn = populateBoolean(e, i); }},
    {"afterMultiDotsLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterMultiDotsLocal = populateBoolean(e, i); }},
    {"afterMultiDotsOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterMultiDotsOn = populateBoolean(e, i); }},
    {"shiftForSecondsLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->shiftForSecondsLocal = populateBoolean(e, i); }},
    {"shiftForSecondsOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->shiftForSecondsOn = populateBoolean(e, i); }},
    {"beforeSingleAcciLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->beforeSingleAcciLocal = populateBoolean(e, i); }},
    {"beforeSingleAcciOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->beforeSingleAcciOn = populateBoolean(e, i); }},
    {"down", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->down = populateBoolean(e, i); }},
    {"breakTimeLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakTimeLocal = populateBoolean(e, i); }},
    {"breakTimeOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakTimeOn = populateBoolean(e, i); }},
    {"breakKeyLocal", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakKeyLocal = populateBoolean(e, i); }},
    {"breakKeyOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakKeyOn = populateBoolean(e, i); }},
    {"freeze", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->freezeDirection = populateBoolean(e, i); }},
    {"stPtAdjOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->stPtAdjOn = populateBoolean(e, i); }},
    {"enPtAdjOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->enPtAdjOn = populateBoolean(e, i); }},
    {"insetRatio1", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->insetRatio1 = e->getTextAs<Evpu>(); }},
    {"height1", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->height1 = e->getTextAs<Evpu>(); }},
    {"insetRatio2", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->insetRatio2 = e->getTextAs<Evpu>(); }},
    {"height2", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->height2 = e->getTextAs<Evpu>(); }},
    {"ctlPtAdjOn", [](const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->ctlPtAdjOn = populateBoolean(e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(TupletDef, {
    {"symbolicNum", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayNumber = e->getTextAs<int>(); }},
    {"symbolicDur", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayDuration = e->getTextAs<Edu>(); }},
    {"refNum", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceNumber = e->getTextAs<int>(); }},
    {"refDur", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"flat", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->alwaysFlat = populateBoolean(e, i); }},
    {"fullDura", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->fullDura = populateBoolean(e, i); }},
    {"metricCenter", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->metricCenter = populateBoolean(e, i); }},
    {"avoidStaff", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->avoidStaff = populateBoolean(e, i); }},
    {"autoBracketStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->autoBracketStyle = toEnum<TupletDef::AutoBracketStyle>(e); }},
    {"hidden", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->hidden = populateBoolean(e, i); }},
    {"tupOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffX = e->getTextAs<Evpu>(); }},
    {"tupOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffY = e->getTextAs<Evpu>(); }},
    {"brackOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffX = e->getTextAs<Evpu>(); }},
    {"brackOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffY = e->getTextAs<Evpu>(); }},
    {"numStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->numStyle = toEnum<TupletDef::NumberStyle>(e); }},
    {"posStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->posStyle = toEnum<TupletDef::PositioningStyle>(e); }},
    {"allowHorz", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->allowHorz = populateBoolean(e, i); }},
    {"ignoreGlOffs", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->ignoreHorzNumOffset = populateBoolean(e, i); }},
    {"breakBracket", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->breakBracket = populateBoolean(e, i); }},
    {"matchHooks", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->matchHooks = populateBoolean(e, i); }},
    {"noteBelow", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->useBottomNote = populateBoolean(e, i); }},
    {"brackStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackStyle = toEnum<TupletDef::BracketStyle>(e); }},
    {"smartTuplet", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->smartTuplet = populateBoolean(e, i); }},
    {"leftHookLen", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->leftHookLen = e->getTextAs<Evpu>(); }},
    {"leftHookExt", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->leftHookExt = e->getTextAs<Evpu>(); }},
    {"rightHookLen", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->rightHookLen = e->getTextAs<Evpu>(); }},
    {"rightHookExt", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->rightHookExt = e->getTextAs<Evpu>(); }},
    {"slope", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->manualSlopeAdj = e->getTextAs<Evpu>(); }},
});
// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

} // namespace details
} // namespace dom
} // namespace musx


#endif // DOXYGEN_SHOULD_IGNORE_THIS
