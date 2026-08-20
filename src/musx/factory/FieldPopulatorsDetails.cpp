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
#include "musx/factory/FieldPopulatorsDetails.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::details;

extern template const XmlEnumMappingElement<AlignJustify> XmlEnumMapping<AlignJustify>::mapping;
extern template const XmlEnumMappingElement<options::BeamOptions::FlattenStyle> XmlEnumMapping<options::BeamOptions::FlattenStyle>::mapping;
extern template const XmlEnumMappingElement<ShowClefMode> XmlEnumMapping<ShowClefMode>::mapping;
extern template const XmlEnumMappingElement<others::Measure::BarlineType> XmlEnumMapping<others::Measure::BarlineType>::mapping;
extern template const XmlEnumMappingElement<others::PageGraphicAssign::PageAssignType> XmlEnumMapping<others::PageGraphicAssign::PageAssignType>::mapping;
extern template const XmlEnumMappingElement<others::PageGraphicAssign::PositionFrom> XmlEnumMapping<others::PageGraphicAssign::PositionFrom>::mapping;
extern template const XmlEnumMappingElement<options::TextOptions::HorizontalAlignment> XmlEnumMapping<options::TextOptions::HorizontalAlignment>::mapping;
extern template const XmlEnumMappingElement<options::TextOptions::VerticalAlignment> XmlEnumMapping<options::TextOptions::VerticalAlignment>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::AutoBracketStyle> XmlEnumMapping<options::TupletOptions::AutoBracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::BracketStyle> XmlEnumMapping<options::TupletOptions::BracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::NumberStyle> XmlEnumMapping<options::TupletOptions::NumberStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::PositioningStyle> XmlEnumMapping<options::TupletOptions::PositioningStyle>::mapping;

MUSX_XML_ENUM_MAPPING(ChordAssign::BassPosition, {
    {"afterRoot", ChordAssign::BassPosition::AfterRoot},   // this value may never appear in the xml
    {"underRoot", ChordAssign::BassPosition::UnderRoot},
    {"subtext",   ChordAssign::BassPosition::Subtext}
});

MUSX_XML_ENUM_MAPPING(FretboardDiagram::Shape, {
    // {"none", FretboardDiagram::Shape::None}, // Default value, may not appear in the XML
    {"closed", FretboardDiagram::Shape::Closed},
    {"open", FretboardDiagram::Shape::Open},
    {"muted", FretboardDiagram::Shape::Muted},
    {"custom", FretboardDiagram::Shape::Custom}
});

MUSX_XML_ENUM_MAPPING(MeasureNumberIndividualPositioning::ForceVisibility, {
    // {"none", MeasureNumberIndividualPositioning::ForceVisibility::None}, // Default value, may not appear in the XML
    {"force", MeasureNumberIndividualPositioning::ForceVisibility::Show},
    {"hide", MeasureNumberIndividualPositioning::ForceVisibility::Hide}
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

MUSX_XML_ENUM_MAPPING(TieAlterBase::ConnectionType, {
    // {"none", TieAlterBase::ConnectionType::None}, // Default value, may not appear in the XML
    {"entryLeftNoteCenter", TieAlterBase::ConnectionType::EntryLeftNoteCenter},
    {"entryRightNoteCenter", TieAlterBase::ConnectionType::EntryRightNoteCenter},
    {"noteLeftNoteCenter", TieAlterBase::ConnectionType::NoteLeftNoteCenter},
    {"noteRightNoteCenter", TieAlterBase::ConnectionType::NoteRightNoteCenter},
    {"noteCenterNoteBottom", TieAlterBase::ConnectionType::NoteCenterNoteBottom},
    {"noteCenterNoteTop", TieAlterBase::ConnectionType::NoteCenterNoteTop},
    {"dotRightNoteCenter", TieAlterBase::ConnectionType::DotRightNoteCenter},
    {"acciLeftNoteCenter", TieAlterBase::ConnectionType::AccidentalLeftNoteCenter},
    {"entryCenterNoteBottom", TieAlterBase::ConnectionType::EntryCenterNoteBottom},
    {"entryCenterNoteTop", TieAlterBase::ConnectionType::EntryCenterNoteTop},
    {"noteLeftNoteBottom", TieAlterBase::ConnectionType::NoteLeftNoteBottom},
    {"noteRightNoteBottom", TieAlterBase::ConnectionType::NoteRightNoteBottom},
    {"noteLeftNoteTop", TieAlterBase::ConnectionType::NoteLeftNoteTop},
    {"noteRightNoteTop", TieAlterBase::ConnectionType::NoteRightNoteTop},
    {"systemEnd", TieAlterBase::ConnectionType::SystemEnd},
    {"systemStart", TieAlterBase::ConnectionType::SystemStart}
});

MUSX_XML_ENUM_MAPPING(TieAlterBase::DirectionContext, {
    // {"none", TieAlterBase::DirectionContext::None}, // Default value, may not appear in the XML
    {"under", TieAlterBase::DirectionContext::Under},
    {"over", TieAlterBase::DirectionContext::Over}
});

} // namespace factory

namespace dom {
namespace details {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(AccidentalAlterations, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->percent = e->getTextAs<int>(); }},
    {"ayDisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->vOffset = e->getTextAs<Evpu>(); }},
    {"axDisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->hOffset = e->getTextAs<Evpu>(); }},
    {"altChar", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->altChar = e->getTextAs<char32_t>(); }},
    {"fontID", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"fontSize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"efx", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"useOwnFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->useOwnFont = populateBoolean(c, e, i); }},
    {"allowVertPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<AccidentalAlterations>& i) { i->allowVertPos = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ArticulationAssign, {
    {"articDef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->articDef = e->getTextAs<Cmper>(); }},
    {"horzOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"horzAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->horzAdd = e->getTextAs<Evpu>(); }},
    {"vertOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"vertAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->vertAdd = e->getTextAs<Evpu>(); }},
    {"overridePlacement", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->overridePlacement = populateBoolean(c, e, i); }},
    {"aboveEntry", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->aboveEntry = populateBoolean(c, e, i); }},
    {"hide", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->hide = populateBoolean(c, e, i); }},
    {"neverStack", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->neverStack = populateBoolean(c, e, i); }},
    {"avoidSlur", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->avoidSlur = populateBoolean(c, e, i); }},
    {"numSlursAvoided", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ArticulationAssign>& i) { i->numSlursAvoided = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(Baseline, {
    {"basedisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Baseline>& i) { i->baselineDisplacement = e->getTextAs<Evpu>(); }},
    {"lyricNumber", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Baseline>& i) { i->lyricNumber = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamAlterations, {
    {"xAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->leftOffsetH = e->getTextAs<Evpu>(); }},
    {"yAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->leftOffsetY = e->getTextAs<Evpu>(); }},
    {"sxAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->rightOffsetH = e->getTextAs<Evpu>(); }},
    {"syAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->rightOffsetY = e->getTextAs<Evpu>(); }},
    {"dura", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->dura = e->getTextAs<Edu>(); }},
    {"context", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->flattenStyle = toEnum<BeamAlterations::FlattenStyle>(e); }},
    {"beamWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamAlterations>& i) { i->beamWidth = e->getTextAs<Efix>(); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamExtension, {
    {"x3Disp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->leftOffset = e->getTextAs<Evpu>(); }},
    {"x4Disp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->rightOffset = e->getTextAs<Evpu>(); }},
    {"do8th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Eighth); }},
    {"do16th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamExtension>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
    {"extBeyond8th", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<BeamExtension>& i) { i->extBeyond8th = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(BeamStubDirection, {
    {"do8th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Eighth); }},
    {"do16th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<BeamStubDirection>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
});

MUSX_XML_ELEMENT_ARRAY(Bracket, {
    {"id", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Bracket>& i) { i->style = toEnum<Bracket::BracketStyle>(e->getTextAs<int>()); }},
    {"bracPos", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Bracket>& i) { i->horzAdjLeft = e->getTextAs<Evpu>(); }},
    {"bracTop", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Bracket>& i) { i->vertAdjTop = e->getTextAs<Evpu>(); }},
    {"bracBot", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<Bracket>& i) { i->vertAdjBot = e->getTextAs<Evpu>(); }},
    {"onSingle", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<Bracket>& i) { i->showOnSingleStaff = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(CenterShape, {
    {"startBreakAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<CenterShape>& i)
        { i->startBreakAdj = FieldPopulator<smartshape::EndPointAdjustment>::populateExistingOrCreate(c, e, i->startBreakAdj, i); }},
    {"endBreakAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<CenterShape>& i)
        { i->endBreakAdj = FieldPopulator<smartshape::EndPointAdjustment>::populateExistingOrCreate(c, e, i->endBreakAdj, i); }},
    {"ctlPtAdj", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<CenterShape>& i)
        { i->ctlPtAdj = FieldPopulator<smartshape::ControlPointAdjustment>::populateExistingOrCreate(c, e, i->ctlPtAdj, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ChordAssign, {
    {"suffix", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->suffixId = e->getTextAs<Cmper>(); }},
    {"fbStyleID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbStyleId = e->getTextAs<Cmper>(); }},
    {"rootScaleNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootScaleNum = e->getTextAs<int>(); }},
    {"rootAlter", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootAlter = e->getTextAs<int>(); }},
    {"rootLowerCase", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->rootLowerCase = populateBoolean(c, e, i); }},
    {"playSuffix", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playSuffix = populateBoolean(c, e, i); }},
    {"showRoot", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showRoot = populateBoolean(c, e, i); }},
    {"playRoot", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playRoot = populateBoolean(c, e, i); }},
    {"showFretboard", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showFretboard = populateBoolean(c, e, i); }},
    {"showSuffix", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showSuffix = populateBoolean(c, e, i); }},
    {"playFretboard", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playFretboard = populateBoolean(c, e, i); }},
    {"bassScaleNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassScaleNum = e->getTextAs<int>(); }},
    {"bassAlter", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassAlter = e->getTextAs<int>(); }},
    {"bassLowerCase", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassLowerCase = populateBoolean(c, e, i); }},
    {"bassPosition", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->bassPosition = toEnum<ChordAssign::BassPosition>(e); }},
    {"showAltBass", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->showAltBass = populateBoolean(c, e, i); }},
    {"playAltBass", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->playAltBass = populateBoolean(c, e, i); }},
    {"capoValue", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->capoValue = e->getTextAs<int>(); }},
    {"useLocalCapo", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->useLocalCapo = populateBoolean(c, e, i); }},
    {"fretInci", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fretboardGroupInci = e->getTextAs<Inci>(); }},
    {"useFretFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->useFretboardFont = populateBoolean(c, e, i); }},
    {"horzOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->horzOff = e->getTextAs<Evpu>(); }},
    {"vertOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->vertOff = e->getTextAs<Evpu>(); }},
    {"fbHorzOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbHorzOff = e->getTextAs<Evpu>(); }},
    {"fbVertOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbVertOff = e->getTextAs<Evpu>(); }},
    {"horzEdu", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->horzEdu = e->getTextAs<Edu>(); }},
    {"chPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->chPercent = e->getTextAs<int>(); }},
    {"fbPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ChordAssign>& i) { i->fbPercent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOctaveFlats, {
    {"ocAdds", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOctaveFlats>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(ClefOctaveSharps, {
    {"ocAdds", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ClefOctaveSharps>& i) { i->values.push_back(e->getTextAs<int>()); }},
});

MUSX_XML_ELEMENT_ARRAY(CrossStaff, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<CrossStaff>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"instrument", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<CrossStaff>& i) { i->staff = e->getTextAs<StaffCmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(CustomStem, {
    {"shapeDef", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->shapeDef = e->getTextAs<Cmper>(); }},
    {"xdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"ydisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<CustomStem>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(DotAlterations, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<DotAlterations>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"xadd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<DotAlterations>& i) { i->hOffset = e->getTextAs<Evpu>(); }},
    {"yadd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<DotAlterations>& i) { i->vOffset = e->getTextAs<Evpu>(); }},
    {"posIncr", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<DotAlterations>& i) { i->interdotSpacing = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(EntryPartFieldDetail, {
    {"posi", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EntryPartFieldDetail>& i) { i->hOffset = e->getTextAs<Evpu>(); }},
    {"freezeStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<EntryPartFieldDetail>& i) { i->freezeStem = populateBoolean(c, e, i); }},
    {"upStem", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<EntryPartFieldDetail>& i) { i->upStem = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(EntrySize, {
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<EntrySize>& i) { i->percent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardDiagram::Cell, {
    {"string",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Cell>& i) { i->string = e->getTextAs<int>(); }},
    {"fret",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Cell>& i){ i->fret = e->getTextAs<int>(); }},
    {"shape",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Cell>& i){ i->shape = toEnum<FretboardDiagram::Shape>(e); }},
    {"fingerNum",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Cell>& i){ i->fingerNum = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardDiagram::Barre, {
    {"fret",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Barre>& i){ i->fret = e->getTextAs<int>(); }},
    {"startString",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Barre>& i){ i->startString = e->getTextAs<int>(); }},
    {"endString",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram::Barre>& i){ i->endString = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(FretboardDiagram, {
    {"numFrets",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->numFrets = e->getTextAs<int>(); }},
    {"fretNum",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->fretboardNum = e->getTextAs<int>(); }},
    {"lock",[](ConstructionContext& c, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->lock = populateBoolean(c, e, i); }},
    {"showNum",[](ConstructionContext& c, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->showNum = populateBoolean(c, e, i); }},
    {"numFretCells",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->numFretCells = e->getTextAs<int>(); }},
    {"numFretBarres",[](ConstructionContext&, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i){ i->numFretBarres = e->getTextAs<int>(); }},
    {"cell",[](ConstructionContext& c, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i)
        { i->cells.push_back(FieldPopulator<FretboardDiagram::Cell>::createAndPopulate(c, e)); }},
    {"barre",[](ConstructionContext& c, const XmlElementPtr& e,const std::shared_ptr<FretboardDiagram>& i)
        { i->barres.push_back(FieldPopulator<FretboardDiagram::Barre>::createAndPopulate(c, e)); }},
});

MUSX_XML_ELEMENT_ARRAY(GFrameHold, {
    {"clefID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefId = e->getTextAs<ClefIndex>(); }},
    {"clefListID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefListId = e->getTextAs<Cmper>(); }},
    {"clefMode", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->showClefMode = toEnum<ShowClefMode>(e); }},
    {"mirrorFrame", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->mirrorFrame = populateBoolean(c, e, i); }},
    {"clefAfterBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefAfterBarline = populateBoolean(c, e, i); }},
    {"clefPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefPercent = e->getTextAs<int>(); }},
    {"frame1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[0] = e->getTextAs<Cmper>(); }},
    {"frame2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[1] = e->getTextAs<Cmper>(); }},
    {"frame3", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[2] = e->getTextAs<Cmper>(); }},
    {"frame4", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[3] = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(IndependentStaffDetails, {
    {"keySig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i)
        { i->keySig = FieldPopulator<KeySignature>::createAndPopulate(c, e, i->getDocument()); }},
    {"hasKey", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasKey = populateBoolean(c, e, i); }},
    {"beats", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->beats = e->getTextAs<Cmper>(); }},
    {"divBeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->divBeat = e->getTextAs<Cmper>(); }},
    {"dispBeats", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->dispBeats = e->getTextAs<Cmper>(); }},
    {"dispDivBeat", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->dispDivBeat = e->getTextAs<Cmper>(); }},
    {"displayAltNumTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAltNumTsig = populateBoolean(c, e, i); }},
    {"displayAltDenTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAltDenTsig = populateBoolean(c, e, i); }},
    {"altNumTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->altNumTsig = populateBoolean(c, e, i); }},
    {"altDenTsig", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->altDenTsig = populateBoolean(c, e, i); }},
    {"displayAbbrvTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->displayAbbrvTime = populateBoolean(c, e, i); }},
    {"hasDispTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasDispTime = populateBoolean(c, e, i); }},
    {"hasTime", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<IndependentStaffDetails>& i) { i->hasTime = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(KeySymbolListElement, {
    {"string", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<KeySymbolListElement>& i) { i->accidentalString = e->getText(); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricAssign, {
    {"lyricNumber", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->lyricNumber = e->getTextAs<Cmper>(); }},
    {"syll", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->syllable = e->getTextAs<unsigned>(); }},
    {"horzOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"vertOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->vertOffset = e->getTextAs<Evpu>(); }},
    {"floatingHorzOff", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->floatingHorzOff = e->getTextAs<Evpu>(); }},
    {"wext", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->wext = e->getTextAs<Evpu>(); }},
    {"displayVerseNum", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<LyricAssign>& i) { i->displayVerseNum = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(LyricEntryInfo, {
    {"justify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricEntryInfo>& i) { i->justify = toEnum<AlignJustify>(e); }},
    {"align",   [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<LyricEntryInfo>& i) { i->align   = toEnum<AlignJustify>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureGraphicAssign, {
    {"version", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->version = e->getTextAs<uint32_t>(); }},
    {"left", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->left = e->getTextAs<Evpu>(); }},
    {"bottom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->bottom = e->getTextAs<Evpu>(); }},
    {"width", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->width = e->getTextAs<Evpu>(); }},
    {"height", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->height = e->getTextAs<Evpu>(); }},
    {"fDescID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->fDescId = e->getTextAs<Cmper>(); }},
    {"displayType", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>&)
        {
            if (toEnum<others::PageGraphicAssign::PageAssignType>(e) != others::PageGraphicAssign::PageAssignType::One) {
                util::Logger::log(util::Logger::LogLevel::Warning, "Encountered measure graphic with multipage positioning.");
            }
        }
    },
    {"displayHidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"halign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->hAlign = toEnum<MeasureGraphicAssign::HorizontalAlignment>(e); }},
    {"valign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->vAlign = toEnum<MeasureGraphicAssign::VerticalAlignment>(e); }},
    {"posFrom", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->posFrom = toEnum<MeasureGraphicAssign::PositionFrom>(e); }},
    {"fixedPerc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->fixedPerc = populateBoolean(c, e, i); }},
    {"savedRecord", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->savedRecord = populateBoolean(c, e, i); }},
    {"origWidth", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->origWidth = e->getTextAs<Evpu>(); }},
    {"origHeight", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->origHeight = e->getTextAs<Evpu>(); }},
    {"graphicCmper", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureGraphicAssign>& i) { i->graphicCmper = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureNumberIndividualPositioning, {
    {"region", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->measNumRegion = e->getTextAs<Cmper>(); }},
    {"x1add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"y1add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
    {"x2add", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->xOffset2 = e->getTextAs<Evpu>(); }},
    {"forceHide", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i)
        { i->forceVisibility = toEnum<MeasureNumberIndividualPositioning::ForceVisibility>(e); }},
    {"isAltNum", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->isAlternateNumber = populateBoolean(c, e, i); }},
    {"useEncl", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i) { i->useEnclosure = populateBoolean(c, e, i); }},
    {"encl", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureNumberIndividualPositioning>& i)
        { i->enclosure = FieldPopulator<others::Enclosure>::populateExistingOrCreate(c, e, i->enclosure, i->getDocument()); }}
});

MUSX_XML_ELEMENT_ARRAY(MeasureOssiaAssign, {
    {"arbnum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureOssiaAssign>& i) { i->ossiaId = e->getTextAs<Cmper>(); }},
    {"topAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureOssiaAssign>& i) { i->xOffset = e->getTextAs<Evpu>(); }},
    {"leftAdd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureOssiaAssign>& i) { i->yOffset = e->getTextAs<Evpu>(); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureOssiaAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(MeasureTextAssign, {
    {"block", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->block = e->getTextAs<Cmper>(); }},
    {"xdispEdu", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->xDispEdu = e->getTextAs<Edu>(); }},
    {"xdispEvpu", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->xDispEvpu = e->getTextAs<Evpu>(); }},
    {"ydisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->yDisp = e->getTextAs<Evpu>(); }},
    {"postIt", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<MeasureTextAssign>& i) { i->hidden = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(NoteAlterations, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"percent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->percent = e->getTextAs<int>(); }},
    {"nxdisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->nxdisp = e->getTextAs<Evpu>(); }},
    {"altNhead", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->altNhead = e->getTextAs<char32_t>(); }},
    {"useOwnFont", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->useOwnFont = populateBoolean(c, e, i); }},
    {"fontID", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"fontSize", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"efx", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { FieldPopulator<FontInfo>::populateField(c, i->customFont, e); }},
    {"allowVertPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->allowVertPos = populateBoolean(c, e, i); }},
    {"nydisp", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->nydisp = e->getTextAs<Evpu>(); }},
    {"enharmonic", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<NoteAlterations>& i) { i->enharmonic = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(PercussionNoteCode, {
    {"noteID",    [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteCode>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"noteCode",  [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<PercussionNoteCode>& i) { i->noteCode = e->getTextAs<PercussionNoteTypeId>(); }},
});

MUSX_XML_ELEMENT_ARRAY(SecondaryBeamBreak, {
    {"do16th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note16th); }},
    {"do32nd", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note32nd); }},
    {"do64th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note64th); }},
    {"do128th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note128th); }},
    {"do256th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note256th); }},
    {"do512th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note512th); }},
    {"do1024th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note1024th); }},
    {"do2048th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note2048th); }},
    {"do4096th", [](ConstructionContext&, const XmlElementPtr&, const std::shared_ptr<SecondaryBeamBreak>& i) { i->mask |= unsigned(NoteType::Note4096th); }},
    {"beamThrough", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<SecondaryBeamBreak>& i) { i->breakThrough = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeNoteBase::NoteShapes, {
    {"d", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase::NoteShapes>& i) { i->doubleWhole = e->getTextAs<char32_t>(); }},
    {"w", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase::NoteShapes>& i) { i->whole = e->getTextAs<char32_t>(); }},
    {"h", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase::NoteShapes>& i) { i->half = e->getTextAs<char32_t>(); }},
    {"q", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase::NoteShapes>& i) { i->quarter = e->getTextAs<char32_t>(); }},
});

MUSX_XML_ELEMENT_ARRAY(ShapeNoteBase, {
    {"noteShapes", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase>& i) {
            auto idxAttr = e->findAttribute("index");
            size_t index = idxAttr ? idxAttr->getValueAs<size_t>() : static_cast<size_t>(-1);
            if (i->noteShapes.size() != index) {
                throw std::invalid_argument("noteShapes index mismatch. Expected: " + std::to_string(i->noteShapes.size())
                                            + ", Found: " + std::to_string(index));
            }
            i->noteShapes.push_back(FieldPopulator<ShapeNoteBase::NoteShapes>::createAndPopulate(c, e));
        }
    },
    {"arrangedByPitch", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<ShapeNoteBase>& i) { i->arrangedByPitch = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(SmartShapeEntryAssign, {
    {"shapeNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<SmartShapeEntryAssign>& i) { i->shapeNum = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffGroup, {
    {"startInst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->startInst = e->getTextAs<StaffCmper>(); }},
    {"endInst", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->endInst = e->getTextAs<StaffCmper>(); }},
    {"startMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->startMeas = e->getTextAs<MeasCmper>(); }},
    {"endMeas", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->endMeas = e->getTextAs<MeasCmper>(); }},
    {"fullID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameId = e->getTextAs<Cmper>(); }},
    {"fullXadj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameXadj = e->getTextAs<int>(); }},
    {"fullYadj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameYadj = e->getTextAs<int>(); }},
    {"bracket", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->bracket = FieldPopulator<Bracket>::createAndPopulate(c, e, i->getDocument()); }},
    {"barline", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->barlineType = toEnum<StaffGroup::BarlineType>(e); }},
    {"fullJustify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameJustify = toEnum<AlignJustify>(e); }},
    {"abbrvJustify", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameJustify = toEnum<AlignJustify>(e); }},
    {"groupBarlineStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->drawBarlines = toEnum<StaffGroup::DrawBarlineStyle>(e); }},
    {"ownBarline", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->ownBarline = populateBoolean(c, e, i); }},
    {"fullIndivPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameIndivPos = populateBoolean(c, e, i); }},
    {"abbrvIndivPos", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameIndivPos = populateBoolean(c, e, i); }},
    {"hideName", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->hideName = populateBoolean(c, e, i); }},
    {"abbrvID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameId = e->getTextAs<Cmper>(); }},
    {"abbrvXadj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameXadj = e->getTextAs<int>(); }},
    {"abbrvYadj", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameYadj = e->getTextAs<int>(); }},
    {"fullHAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameAlign = toEnum<AlignJustify>(e); }},
    {"abbrvHAlign", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameAlign = toEnum<AlignJustify>(e); }},
    {"fullExpand", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameExpand = populateBoolean(c, e, i); }},
    {"abbrvExpand", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameExpand = populateBoolean(c, e, i); }},
    {"optimize", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->hideStaves = toEnum<StaffGroup::HideStaves>(e); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffSize, {
    {"staffPercent", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StaffSize>& i) { i->staffPercent = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StemAlterations, {
    {"upVertAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemAlterations>& i) { i->upVertAdjust = e->getTextAs<Evpu>(); }},
    {"downVertAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemAlterations>& i) { i->downVertAdjust = e->getTextAs<Evpu>(); }},
    {"upHorzAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemAlterations>& i) { i->upHorzAdjust = e->getTextAs<Evpu>(); }},
    {"downHorzAdjust", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<StemAlterations>& i) { i->downHorzAdjust = e->getTextAs<Evpu>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TablatureNoteMods, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TablatureNoteMods>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"stringNumber", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TablatureNoteMods>& i) { i->stringNumber = e->getTextAs<int>(); }},
});

MUSX_XML_ELEMENT_ARRAY(TieAlterBase, {
    {"noteID", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->noteId = e->getTextAs<NoteNumber>(); }},
    {"xStart", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->xStart = e->getTextAs<Evpu>(); }},
    {"xEnd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->xEnd = e->getTextAs<Evpu>(); }},
    {"yStart", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->yStart = e->getTextAs<Evpu>(); }},
    {"yEnd", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->yEnd = e->getTextAs<Evpu>(); }},
    {"outerLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->outerLocal = populateBoolean(c, e, i); }},
    {"outerOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->outerOn = populateBoolean(c, e, i); }},
    {"afterSingleDotLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterSingleDotLocal = populateBoolean(c, e, i); }},
    {"afterSingleDotOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterSingleDotOn = populateBoolean(c, e, i); }},
    {"afterMultiDotsLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterMultiDotsLocal = populateBoolean(c, e, i); }},
    {"afterMultiDotsOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->afterMultiDotsOn = populateBoolean(c, e, i); }},
    {"shiftForSecondsLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->shiftForSecondsLocal = populateBoolean(c, e, i); }},
    {"shiftForSecondsOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->shiftForSecondsOn = populateBoolean(c, e, i); }},
    {"beforeSingleAcciLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->beforeSingleAcciLocal = populateBoolean(c, e, i); }},
    {"beforeSingleAcciOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->beforeSingleAcciOn = populateBoolean(c, e, i); }},
    {"down", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->down = populateBoolean(c, e, i); }},
    {"breakTimeLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakTimeLocal = populateBoolean(c, e, i); }},
    {"breakTimeOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakTimeOn = populateBoolean(c, e, i); }},
    {"breakKeyLocal", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakKeyLocal = populateBoolean(c, e, i); }},
    {"breakKeyOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->breakKeyOn = populateBoolean(c, e, i); }},
    {"freeze", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->freezeDirection = populateBoolean(c, e, i); }},
    {"noSpecialArc", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->noSpecialArc = populateBoolean(c, e, i); }},
    {"stPtAdjOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->stPtAdjOn = populateBoolean(c, e, i); }},
    {"stPtEntCnct", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->stPtEntCnct = toEnum<TieAlterBase::ConnectionType>(e); }},
    {"stPtContext", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->stPtContext = toEnum<TieAlterBase::DirectionContext>(e); }},
    {"enPtAdjOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->enPtAdjOn = populateBoolean(c, e, i); }},
    {"enPtEntCnct", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->enPtEntCnct = toEnum<TieAlterBase::ConnectionType>(e); }},
    {"enPtContext", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->enPtContext = toEnum<TieAlterBase::DirectionContext>(e); }},
    {"insetRatio1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->insetRatio1 = e->getTextAs<Evpu>(); }},
    {"height1", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->height1 = e->getTextAs<Evpu>(); }},
    {"insetRatio2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->insetRatio2 = e->getTextAs<Evpu>(); }},
    {"height2", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->height2 = e->getTextAs<Evpu>(); }},
    {"ctlPtAdjOn", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->ctlPtAdjOn = populateBoolean(c, e, i); }},
    {"ctlPtFixed", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TieAlterBase>& i) { i->ctlPtFixed = populateBoolean(c, e, i); }},
});

MUSX_XML_ELEMENT_ARRAY(TupletDef, {
    {"symbolicNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayNumber = e->getTextAs<int>(); }},
    {"symbolicDur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayDuration = e->getTextAs<Edu>(); }},
    {"refNum", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceNumber = e->getTextAs<int>(); }},
    {"refDur", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"flat", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->alwaysFlat = populateBoolean(c, e, i); }},
    {"fullDura", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->fullDura = populateBoolean(c, e, i); }},
    {"metricCenter", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->metricCenter = populateBoolean(c, e, i); }},
    {"avoidStaff", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->avoidStaff = populateBoolean(c, e, i); }},
    {"autoBracketStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->autoBracketStyle = toEnum<TupletDef::AutoBracketStyle>(e); }},
    {"hidden", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->hidden = populateBoolean(c, e, i); }},
    {"tupOffX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffX = e->getTextAs<Evpu>(); }},
    {"tupOffY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffY = e->getTextAs<Evpu>(); }},
    {"brackOffX", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffX = e->getTextAs<Evpu>(); }},
    {"brackOffY", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffY = e->getTextAs<Evpu>(); }},
    {"numStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->numStyle = toEnum<TupletDef::NumberStyle>(e); }},
    {"posStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->posStyle = toEnum<TupletDef::PositioningStyle>(e); }},
    {"allowHorz", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->allowHorz = populateBoolean(c, e, i); }},
    {"ignoreGlOffs", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->ignoreHorzNumOffset = populateBoolean(c, e, i); }},
    {"breakBracket", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->breakBracket = populateBoolean(c, e, i); }},
    {"matchHooks", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->matchHooks = populateBoolean(c, e, i); }},
    {"noteBelow", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->useBottomNote = populateBoolean(c, e, i); }},
    {"brackStyle", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackStyle = toEnum<TupletDef::BracketStyle>(e); }},
    {"smartTuplet", [](ConstructionContext& c, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->smartTuplet = populateBoolean(c, e, i); }},
    {"leftHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->leftHookLen = e->getTextAs<Evpu>(); }},
    {"leftHookExt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->leftHookExt = e->getTextAs<Evpu>(); }},
    {"rightHookLen", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->rightHookLen = e->getTextAs<Evpu>(); }},
    {"rightHookExt", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->rightHookExt = e->getTextAs<Evpu>(); }},
    {"slope", [](ConstructionContext&, const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->manualSlopeAdj = e->getTextAs<Evpu>(); }},
});
// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

} // namespace details
} // namespace dom
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE_THIS
