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

extern template const XmlEnumMappingElement<others::Measure::BarlineType> XmlEnumMapping<others::Measure::BarlineType>::mapping;
extern template const XmlEnumMappingElement<others::NamePositioning::AlignJustify> XmlEnumMapping<others::NamePositioning::AlignJustify>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::AutoBracketStyle> XmlEnumMapping<options::TupletOptions::AutoBracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::BracketStyle> XmlEnumMapping<options::TupletOptions::BracketStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::NumberStyle> XmlEnumMapping<options::TupletOptions::NumberStyle>::mapping;
extern template const XmlEnumMappingElement<options::TupletOptions::PositioningStyle> XmlEnumMapping<options::TupletOptions::PositioningStyle>::mapping;

MUSX_XML_ENUM_MAPPING(GFrameHold::ShowClefMode, {
    // {"whenNeeded", GFrameHold::ShowClefMode::WhenNeeded}, this is the default and may never appear in the xml
    {"hidden", GFrameHold::ShowClefMode::Never},
    {"forced", GFrameHold::ShowClefMode::Always}
});

MUSX_XML_ENUM_MAPPING(StaffGroup::HideStaves, {
    // {"Normally", StaffGroup::HideStaves::Normally}, // Default value, may not appear in the XML
    {"asGroup", StaffGroup::HideStaves::AsGroup},
    {"none", StaffGroup::HideStaves::None}
});

MUSX_XML_ENUM_MAPPING(StaffGroup::DrawBarlineStyle, {
    // {"OnlyOnStaves", StaffGroup::DrawBarlineStyle::OnlyOnStaves}, // Default value, may not appear in the XML
    {"group", StaffGroup::DrawBarlineStyle::ThroughStaves},
    {"Mensurstriche", StaffGroup::DrawBarlineStyle::Mensurstriche}
});

} // namespace factory

namespace dom {
namespace details {

using namespace ::musx::xml;
using namespace ::musx::factory;

MUSX_XML_ELEMENT_ARRAY(GFrameHold, {
    {"clefID", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefId = e->getTextAs<ClefIndex>(); }},
    {"clefListID", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefListId = e->getTextAs<Cmper>(); }},
    {"clefMode", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->showClefMode = toEnum<GFrameHold::ShowClefMode>(e->getTextTrimmed()); }},
    {"mirrorFrame", [](const XmlElementPtr&, const std::shared_ptr<GFrameHold>& i) { i->mirrorFrame = true; }},
    {"clefPercent", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->clefPercent = e->getTextAs<int>(); }},
    {"frame1", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[0] = e->getTextAs<Cmper>(); }},
    {"frame2", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[1] = e->getTextAs<Cmper>(); }},
    {"frame3", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[2] = e->getTextAs<Cmper>(); }},
    {"frame4", [](const XmlElementPtr& e, const std::shared_ptr<GFrameHold>& i) { i->frames[3] = e->getTextAs<Cmper>(); }},
});

MUSX_XML_ELEMENT_ARRAY(StaffGroup::Bracket, {
    {"id", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->style = toEnum<StaffGroup::BracketStyle>(e->getTextAs<int>()); }},
    {"bracPos", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->horzAdjLeft = e->getTextAs<Evpu>(); }},
    {"bracTop", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->vertAdjTop = e->getTextAs<Evpu>(); }},
    {"bracBot", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup::Bracket>& i) { i->vertAdjBot = e->getTextAs<Evpu>(); }},
    {"onSingle", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup::Bracket>& i) { i->showOnSingleStaff = true; }},
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
    {"barline", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->barlineType = toEnum<StaffGroup::BarlineType>(e->getTextTrimmed()); }},
    {"fullJustify", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameJustify = toEnum<StaffGroup::AlignJustify>(e->getTextTrimmed()); }},
    {"abbrvJustify", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameJustify = toEnum<StaffGroup::AlignJustify>(e->getTextTrimmed()); }},
    {"groupBarlineStyle", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->drawBarlines = toEnum<StaffGroup::DrawBarlineStyle>(e->getTextTrimmed()); }},
    {"ownBarline", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->ownBarline = true; }},
    {"fullIndivPos", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->fullNameIndivPos = true; }},
    {"abbrvIndivPos", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameIndivPos = true; }},
    {"hideName", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->hideName = true; }},
    {"abbrvID", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameId = e->getTextAs<Cmper>(); }},
    {"abbrvXadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameXadj = e->getTextAs<int>(); }},
    {"abbrvYadj", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameYadj = e->getTextAs<int>(); }},
    {"fullHAlign", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->fullNameAlign = toEnum<StaffGroup::AlignJustify>(e->getTextTrimmed()); }},
    {"abbrvHAlign", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameAlign = toEnum<StaffGroup::AlignJustify>(e->getTextTrimmed()); }},
    {"fullExpand", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->fullNameExpand = true; }},
    {"abbrvExpand", [](const XmlElementPtr&, const std::shared_ptr<StaffGroup>& i) { i->abbrvNameExpand = true; }},
    {"optimize", [](const XmlElementPtr& e, const std::shared_ptr<StaffGroup>& i) { i->hideStaves = toEnum<StaffGroup::HideStaves>(e->getTextTrimmed()); }},
});

MUSX_XML_ELEMENT_ARRAY(TupletDef, {
    {"symbolicNum", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayNumber = e->getTextAs<int>(); }},
    {"symbolicDur", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->displayDuration = e->getTextAs<Edu>(); }},
    {"refNum", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceNumber = e->getTextAs<int>(); }},
    {"refDur", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->referenceDuration = e->getTextAs<Edu>(); }},
    {"flat", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->alwaysFlat = true; }},
    {"fullDura", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->fullDura = true; }},
    {"metricCenter", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->metricCenter = true; }},
    {"avoidStaff", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->avoidStaff = true; }},
    {"autoBracketStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->autoBracketStyle = toEnum<TupletDef::AutoBracketStyle>(e->getTextTrimmed()); }},
    {"tupOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffX = e->getTextAs<Evpu>(); }},
    {"tupOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->tupOffY = e->getTextAs<Evpu>(); }},
    {"brackOffX", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffX = e->getTextAs<Evpu>(); }},
    {"brackOffY", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackOffY = e->getTextAs<Evpu>(); }},
    {"numStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->numStyle = toEnum<TupletDef::NumberStyle>(e->getTextTrimmed()); }},
    {"posStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->posStyle = toEnum<TupletDef::PositioningStyle>(e->getTextTrimmed()); }},
    {"allowHorz", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->allowHorz = true; }},
    {"ignoreGlOffs", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->ignoreHorzNumOffset = true; }},
    {"breakBracket", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->breakBracket = true; }},
    {"matchHooks", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->matchHooks = true; }},
    {"noteBelow", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->useBottomNote = true; }},
    {"brackStyle", [](const XmlElementPtr& e, const std::shared_ptr<TupletDef>& i) { i->brackStyle = toEnum<TupletDef::BracketStyle>(e->getTextTrimmed()); }},
    {"smartTuplet", [](const XmlElementPtr&, const std::shared_ptr<TupletDef>& i) { i->smartTuplet = true; }},
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
