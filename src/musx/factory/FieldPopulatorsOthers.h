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
#include "musx/dom/Others.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

namespace musx {
namespace factory {

using namespace dom;
using namespace dom::others;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

// Field populators are maintained to populate in the order that nodes are observed to occur in EnigmaXml.
// The goal is that this may facilitate serialization in the future.

template <>
inline Enclosure::Shape toEnum<Enclosure::Shape>(const uint8_t& value)
{
    if (value >= static_cast<uint8_t>(Enclosure::Shape::NoEnclosure) &&
        value <= static_cast<uint8_t>(Enclosure::Shape::Octogon)) {
        return static_cast<Enclosure::Shape>(value);
    }
    throw std::invalid_argument("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
}

template <>
inline const XmlElementArray<Enclosure> FieldPopulator<Enclosure>::xmlElements = {
    {"xAdd", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->xAdd = e->getTextAs<Evpu>(); }},
    {"yAdd", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->yAdd = e->getTextAs<Evpu>(); }},
    {"xMargin", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->xMargin = e->getTextAs<Evpu>(); }},
    {"yMargin", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->yMargin = e->getTextAs<Evpu>(); }},
    {"lineWidth", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->lineWidth = e->getTextAs<Efix>(); }},
    {"sides", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->shape = toEnum<Enclosure::Shape>(e->getTextAs<uint8_t>()); }},
    {"cornerRadius", [](const XmlElementPtr& e, const std::shared_ptr<Enclosure>& i) { i->cornerRadius = e->getTextAs<Efix>(); }},
    {"fixedSize", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->fixedSize = true; }},
    {"notTall", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->notTall = true; }},
    {"opaque", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->opaque = true; }},
    {"roundCorners", [](const XmlElementPtr&, const std::shared_ptr<Enclosure>& i) { i->roundCorners = true; }},
};

template <>
inline const XmlElementArray<FontDefinition> FieldPopulator<FontDefinition>::xmlElements = {
    {"charsetBank", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetBank = e->getText(); }},
    {"charsetVal", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->charsetVal = e->getTextAs<int>(); }},
    {"pitch", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->pitch = e->getTextAs<int>(); }},
    {"family", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->family = e->getTextAs<int>(); }},
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<FontDefinition>& i) { i->name = e->getText(); }},
};

template <>
inline const XmlElementArray<LayerAttributes> FieldPopulator<LayerAttributes>::xmlElements = {
    {"restOffset", [](const XmlElementPtr& e, const std::shared_ptr<LayerAttributes>& i) { i->restOffset = e->getTextAs<int>(); }},
    {"flipTies", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezTiesToStems = true; }},
    {"floatLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->onlyIfOtherLayersHaveNotes = true; }},
    {"useRestOff", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->useRestOffset = true; }},
    {"freezeLayUp", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezeStemsUp = true; }},
    {"freezeLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->freezeLayer = true; }},
    {"playback", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->playback = true; }},
    {"spacing", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->affectSpacing = true; }},
    {"ignoreHidden", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenNotesOnly = true; }},
    {"ignoreHiddenLayers", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->ignoreHiddenLayers = true; }},
    {"hideLayer", [](const XmlElementPtr&, const std::shared_ptr<LayerAttributes>& i) { i->hideLayer = true; }},
};

template <>
inline const ResolverArray<LayerAttributes> FieldPopulator<LayerAttributes>::resolvers = {
        [](const dom::DocumentPtr& document) {
            auto layers = document->getOthers()->getArray<LayerAttributes>();
            if (layers.size() != 4) {
                throw std::invalid_argument("Expected exactly 4 <layerAtts> elements.");
            }
            for (size_t i = 0; i < layers.size(); i++) {
                if (layers[i]->getCmper() != i) {
                    throw std::invalid_argument("Expected <layerAtts> elements to have cmper values 0, 1, 2, 3 in order.");
                }
            }
        }
};

template <>
inline PlaybackType toEnum<PlaybackType>(const std::string& value)
{
    if (value == "none") return PlaybackType::None;
    if (value == "time") return PlaybackType::Tempo;
    if (value == "midiController") return PlaybackType::MidiController;
    if (value == "amplitude") return PlaybackType::KeyVelocity;
    if (value == "transpose") return PlaybackType::Transpose;
    if (value == "channel") return PlaybackType::Channel;
    if (value == "midiPatchChange") return PlaybackType::MidiPatchChange;
    if (value == "percMidiMap") return PlaybackType::PercussionMidiMap;
    if (value == "midiPitchwheel") return PlaybackType::MidiPitchWheel;
    if (value == "midiPressure") return PlaybackType::ChannelPressure;
    if (value == "rekey") return PlaybackType::RestrikeKeys;
    if (value == "dump") return PlaybackType::Dump;
    if (value == "startTempo") return PlaybackType::PlayTempoToolChanges;
    if (value == "stopTempo") return PlaybackType::IgnoreTempoToolChanges;
    if (value == "swing") return PlaybackType::Swing;
    if (value == "hpOn") return PlaybackType::SmartPlaybackOn;
    if (value == "hpOff") return PlaybackType::SmartPlaybackOff;
    throw std::invalid_argument("Unknown playbackType value in XML: " + value);
}

// Specialization for HorizontalMeasExprAlign
template <>
inline HorizontalMeasExprAlign toEnum<HorizontalMeasExprAlign>(const std::string& value)
{
    if (value == "manual") return HorizontalMeasExprAlign::Manual;
    if (value == "leftOfAllNoteheads") return HorizontalMeasExprAlign::LeftOfAllNoteheads;
    if (value == "leftOfPrimaryNotehead") return HorizontalMeasExprAlign::LeftOfPrimaryNotehead;
    if (value == "stem") return HorizontalMeasExprAlign::Stem;
    if (value == "centerPrimaryNotehead") return HorizontalMeasExprAlign::CenterPrimaryNotehead;
    if (value == "centerAllNoteheads") return HorizontalMeasExprAlign::CenterAllNoteheads;
    if (value == "rightOfAllNoteheads") return HorizontalMeasExprAlign::RightOfAllNoteheads;
    if (value == "leftEdge") return HorizontalMeasExprAlign::LeftBarline;
    if (value == "startTimeSig") return HorizontalMeasExprAlign::StartTimeSig;
    if (value == "afterClefKeyTime") return HorizontalMeasExprAlign::AfterClefKeyTime;
    if (value == "startOfMusic") return HorizontalMeasExprAlign::StartOfMusic;
    if (value == "centerOverBarlines") return HorizontalMeasExprAlign::CenterOverBarlines;
    if (value == "centerOverMusic") return HorizontalMeasExprAlign::CenterOverMusic;
    if (value == "rightEdge") return HorizontalMeasExprAlign::RightBarline;
    throw std::invalid_argument("Unknown horzMeasExprAlign value in XML: " + value);
}

// Specialization for VerticalMeasExprAlign
template <>
inline VerticalMeasExprAlign toEnum<VerticalMeasExprAlign>(const std::string& value)
{
    if (value == "manual") return VerticalMeasExprAlign::Manual;
    if (value == "refLine") return VerticalMeasExprAlign::RefLine;
    if (value == "aboveStaff") return VerticalMeasExprAlign::AboveStaff;
    if (value == "belowStaff") return VerticalMeasExprAlign::BelowStaff;
    if (value == "topNote") return VerticalMeasExprAlign::TopNote;
    if (value == "bottomNote") return VerticalMeasExprAlign::BottomNote;
    if (value == "aboveEntry") return VerticalMeasExprAlign::AboveEntry;
    if (value == "belowEntry") return VerticalMeasExprAlign::BelowEntry;
    if (value == "aboveStaffOrEntry") return VerticalMeasExprAlign::AboveStaffOrEntry;
    if (value == "belowStaffOrEntry") return VerticalMeasExprAlign::BelowStaffOrEntry;
    throw std::invalid_argument("Invalid vertMeasExprAlign value in XML: " + value);
}

// Specialization for HorizontalExprJustification
template <>
inline HorizontalExprJustification toEnum<HorizontalExprJustification>(const std::string& value)
{
    if (value == "left") return HorizontalExprJustification::Left;
    if (value == "center") return HorizontalExprJustification::Center;
    if (value == "right") return HorizontalExprJustification::Right;
    throw std::invalid_argument("Invalid horzExprJustification value in XML: " + value);
}

template <>
inline MarkingCategory::CategoryType toEnum<MarkingCategory::CategoryType>(const std::string& str)
{
    if (str == "dynamics") return MarkingCategory::CategoryType::Dynamics;
    if (str == "tempoMarks") return MarkingCategory::CategoryType::TempoMarks;
    if (str == "tempoAlts") return MarkingCategory::CategoryType::TempoAlterations;
    if (str == "expressiveText") return MarkingCategory::CategoryType::ExpressiveText;
    if (str == "techniqueText") return MarkingCategory::CategoryType::TechniqueText;
    if (str == "rehearsalMarks") return MarkingCategory::CategoryType::RehearsalMarks;
    if (str == "misc") return MarkingCategory::CategoryType::Misc;
    throw std::invalid_argument("Invalid marking category type value in XML: " + str);
}

template <>
inline const XmlElementArray<MarkingCategory> FieldPopulator<MarkingCategory>::xmlElements = {
    {"categoryType", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->categoryType = toEnum<MarkingCategory::CategoryType>(e->getText()); }},
    {"textFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->textFont = FieldPopulator<FontInfo>::createAndPopulateNullDefault(e, i->getDocument()); }},
    {"musicFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->musicFont = FieldPopulator<FontInfo>::createAndPopulateNullDefault(e, i->getDocument()); }},
    {"numberFont", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i)
        { i->numberFont = FieldPopulator<FontInfo>::createAndPopulateNullDefault(e, i->getDocument()); }},
    {"horzAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzAlign = toEnum<HorizontalMeasExprAlign>(e->getText()); }},
    {"vertAlign", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertAlign = toEnum<VerticalMeasExprAlign>(e->getText()); }},
    {"justification", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->justification = toEnum<HorizontalExprJustification>(e->getText()); }},
    {"horzOffset", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->horzOffset = e->getTextAs<Evpu>(); }},
    {"vertOffsetBaseline", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetBaseline = e->getTextAs<Evpu>(); }},
    {"vertOffsetEntry", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->vertOffsetEntry = e->getTextAs<Evpu>(); }},
    {"usesTextFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesTextFont = true; }},
    {"usesMusicFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesMusicFont = true; }},
    {"usesNumberFont", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesNumberFont = true; }},
    {"usesPositioning", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesPositioning = true; }},
    {"usesStaffList", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesStaffList = true; }},
    {"usesBreakMmRests", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->usesBreakMmRests = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->breakMmRest = true; }},
    {"userCreated", [](const XmlElementPtr&, const std::shared_ptr<MarkingCategory>& i) { i->userCreated = true; }},
    {"staffList", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategory>& i) { i->staffList = e->getTextAs<Cmper>(); }},
};

template <>
inline const ResolverArray<MarkingCategory> FieldPopulator<MarkingCategory>::resolvers = {
        [](const dom::DocumentPtr& document) {
            auto cats = document->getOthers()->getArray<MarkingCategory>();
            for (const auto& cat : cats) {
                if (cat->categoryType == MarkingCategory::CategoryType::Invalid) {
                    throw std::invalid_argument("Encountered <markingsCategory> node (cmper " + std::to_string(cat->getCmper()) + ") with no categoryType");
                }
            }
        }
};

template <>
inline const XmlElementArray<PartGlobals> FieldPopulator<PartGlobals>::xmlElements = {
    {"showTransposed", [](const XmlElementPtr&, const std::shared_ptr<PartGlobals>& i) { i->showTransposed = true; }},
    {"scrollViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->scrollViewIUlist = e->getTextAs<Cmper>(); }},
    {"studioViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->studioViewIUlist = e->getTextAs<Cmper>(); }},
    {"pageViewIUlist", [](const XmlElementPtr& e, const std::shared_ptr<PartGlobals>& i) { i->specialPartExtractionIUList = e->getTextAs<Cmper>(); }},
};

template <>
inline const XmlElementArray<MarkingCategoryName> FieldPopulator<MarkingCategoryName>::xmlElements = {
    {"name", [](const XmlElementPtr& e, const std::shared_ptr<MarkingCategoryName>& i) { i->name = e->getText(); }},
};

template <>
inline const XmlElementArray<TextExpressionDef> FieldPopulator<TextExpressionDef>::xmlElements = {
    {"textIDKey", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->textIdKey = e->getTextAs<Cmper>(); }},
    {"categoryID", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->categoryId = e->getTextAs<Cmper>(); }},
    {"value", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->value = e->getTextAs<int>(); }},
    {"auxdata1", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->auxData1 = e->getTextAs<int>(); }},
    {"playType", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->playbackType = toEnum<PlaybackType>(e->getText()); }},
    {"horzMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzMeasExprAlign = toEnum<HorizontalMeasExprAlign>(e->getText()); }},
    {"horzExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->horzExprJustification = toEnum<HorizontalExprJustification>(e->getText()); }},
    {"vertMeasExprAlign", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->vertMeasExprAlign = toEnum<VerticalMeasExprAlign>(e->getText()); }},
    {"measXAdjust", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->measXAdjust = e->getTextAs<Evpu>(); }},
    {"yAdjustEntry", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustEntry = e->getTextAs<Evpu>(); }},
    {"yAdjustBaseline", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->yAdjustBaseline = e->getTextAs<Evpu>(); }},
    {"useCategoryFonts", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryFonts = true; }},
    {"useCategoryPos", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useCategoryPos = true; }},
    {"newEnclosure", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->hasEnclosure = true; }},
    {"breakMmRest", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->breakMmRest = true; }},
    {"useAuxData", [](const XmlElementPtr&, const std::shared_ptr<TextExpressionDef>& i) { i->useAuxData = true; }},
    {"descStr", [](const XmlElementPtr& e, const std::shared_ptr<TextExpressionDef>& i) { i->description = e->getText(); }},
};

template <>
inline const ResolverArray<TextExpressionDef> FieldPopulator<TextExpressionDef>::resolvers = {
        [](const dom::DocumentPtr& document) {
            auto exps = document->getOthers()->getArray<TextExpressionDef>();
            for (const auto& instance : exps) {
                if (instance->categoryId) {
                    auto markingCat = document->getOthers()->get<MarkingCategory>(instance->categoryId);
                    if (!markingCat) {
                        throw std::invalid_argument("Marking category for text expression " + std::to_string(instance->getCmper()) + " does not exist.");
                    }
                    markingCat->textExpressions.emplace(instance->getCmper(), instance);
                }
            }
        }
};

template <>
struct FieldPopulator<TextExpressionEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

template <>
struct FieldPopulator<TextRepeatEnclosure> : private FieldPopulator<Enclosure>
{
    using FieldPopulator<Enclosure>::populate;
};

// Repeat for other types...

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
