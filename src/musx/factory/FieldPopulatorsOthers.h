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

template <>
struct FieldPopulator<Enclosure> : public FactoryBase
{
private:
    static Enclosure::Shape toShape(int value)
    {
        if (value >= static_cast<int>(Enclosure::Shape::NoEnclosure) &&
            value <= static_cast<int>(Enclosure::Shape::Octogon)) {
            return static_cast<Enclosure::Shape>(value);
        }
        throw std::invalid_argument("Invalid <sides> value in XML for enclosure: " + std::to_string(value));
    }

public:
    static void populate(const std::shared_ptr<Enclosure>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "xAdd", instance->xAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yAdd", instance->yAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "xMargin", instance->xMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yMargin", instance->yMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "lineWidth", instance->lineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "sides", instance->shape, [](auto element) { return toShape(element->template getTextAs<uint8_t>()); });
        getFieldFromXml(element, "cornerRadius", instance->cornerRadius, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "fixedSize", instance->fixedSize, [](auto) { return true; }, false);
        getFieldFromXml(element, "notTall", instance->notTall, [](auto) { return true; }, false);
        getFieldFromXml(element, "opaque", instance->opaque, [](auto) { return true; }, false);
        getFieldFromXml(element, "roundCorners", instance->roundCorners, [](auto) { return true; }, false);
    }
};

template <>
struct FieldPopulator<MarkingCategory>;
template <>
struct FieldPopulator<TextExpressionDef>;

/** @brief Shared enum conversion utlities for expressions and marking categories */
struct ExpressionEnumUtils
{
private:
    /**
     * @brief Converts XML string values to PlaybackType enum values.
     */
    static PlaybackType toPlaybackType(const std::string& value)
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

    /**
     * @brief Converts XML string values to HorizontalMeasExprAlign enum values.
     */
    static HorizontalMeasExprAlign toHorizontalMeasExprAlign(const std::string& value)
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

    /**
     * @brief Converts XML string values to VerticalMeasExprAlign enum values.
     */
    static VerticalMeasExprAlign toVerticalMeasExprAlign(const std::string& value)
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

    /**
     * @brief Converts XML string values to HorizontalExprJustification enum values.
     */
    static HorizontalExprJustification toHorizontalExprJustification(const std::string& value)
    {
        if (value == "left") return HorizontalExprJustification::Left;
        if (value == "center") return HorizontalExprJustification::Center;
        if (value == "right") return HorizontalExprJustification::Right;
        throw std::invalid_argument("Invalid horzExprJustification value in XML: " + value);
    }

    friend struct FieldPopulator<MarkingCategory>;
    friend struct FieldPopulator<TextExpressionDef>;
};

template <>
struct FieldPopulator<FontDefinition> : public FactoryBase
{
    static void populate(const std::shared_ptr<FontDefinition>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "charsetBank", instance->charsetBank, [](auto element) { return element->getText(); });
        getFieldFromXml(element, "charsetVal", instance->charsetVal, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "pitch", instance->pitch, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "family", instance->family, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "name", instance->name, [](auto element) { return element->getText(); });
    }
};

template <>
struct FieldPopulator<MarkingCategory> : public FactoryBase
{
private:
    using Utils = ExpressionEnumUtils;

    static MarkingCategory::CategoryType toCategoryType(const std::string& str)
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

public:
    static void populate(const std::shared_ptr<MarkingCategory>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        // Populate categoryType field
        getFieldFromXml(element, "categoryType", instance->categoryType, [](auto element) { return toCategoryType(element->getText()); });
        if (instance->categoryType == MarkingCategory::CategoryType::Invalid) {
            throw std::invalid_argument("Encountered <markingsCategory> node (cmper " + std::to_string(instance->getCmper()) + ") with no categoryType");
        }

        // Populate textFont, musicFont, and numberFont if the corresponding font tags exist
        instance->textFont = FieldPopulator<FontInfo>::getFontFromXml(element, "textFont", instance->getDocument());
        instance->musicFont = FieldPopulator<FontInfo>::getFontFromXml(element, "musicFont", instance->getDocument());
        instance->numberFont = FieldPopulator<FontInfo>::getFontFromXml(element, "numberFont", instance->getDocument());

        // Populate alignment and justification fields
        getFieldFromXml(element, "horzAlign", instance->horzAlign, [](auto element) { return Utils::toHorizontalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "vertAlign", instance->vertAlign, [](auto element) { return Utils::toVerticalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "justification", instance->justification, [](auto element) { return Utils::toHorizontalExprJustification(element->template getTextAs<std::string>()); });

        // Populate offset fields
        getFieldFromXml(element, "horzOffset", instance->horzOffset, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "vertOffsetBaseline", instance->vertOffsetBaseline, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "vertOffsetEntry", instance->vertOffsetEntry, [](auto element) { return element->template getTextAs<Evpu>(); });

        // Populate boolean usage fields
        getFieldFromXml(element, "usesTextFont", instance->usesTextFont, [](auto) { return true; }, false);
        getFieldFromXml(element, "usesMusicFont", instance->usesMusicFont, [](auto) { return true; }, false);
        getFieldFromXml(element, "usesNumberFont", instance->usesNumberFont, [](auto) { return true; }, false);
        getFieldFromXml(element, "usesPositioning", instance->usesPositioning, [](auto) { return true; }, false);
        getFieldFromXml(element, "usesStaffList", instance->usesStaffList, [](auto) { return true; }, false);
        getFieldFromXml(element, "usesBreakMmRests", instance->usesBreakMmRests, [](auto) { return true; }, false);
        getFieldFromXml(element, "breakMmRest", instance->breakMmRest, [](auto) { return true; }, false);
        getFieldFromXml(element, "userCreated", instance->userCreated, [](auto) { return true; }, false);

        // Populate staffList field
        getFieldFromXml(element, "staffList", instance->staffList, [](auto element) { return element->template getTextAs<Cmper>(); });
    }
};

template <>
struct FieldPopulator<MarkingCategoryName> : public FactoryBase
{
public:
    static void populate(const std::shared_ptr<MarkingCategoryName>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        getFieldFromXml(element, "name", instance->name, [](auto element) { return element->getText(); });
    }
};

template <>
struct FieldPopulator<TextExpressionDef> : public FactoryBase
{
private:
    using Utils = ExpressionEnumUtils;

public:
    /**
     * @brief Populates the fields of a TextExpressionDef instance from an XML element.
     */
    static void populate(const std::shared_ptr<TextExpressionDef>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker& elementLinker)
    {
        getFieldFromXml(element, "textIDKey", instance->textIdKey, [](auto element) { return element->template getTextAs<Cmper>(); });
        getFieldFromXml(element, "categoryID", instance->categoryId, [](auto element) { return element->template getTextAs<Cmper>(); });
        getFieldFromXml(element, "value", instance->value, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "auxData1", instance->auxData1, [](auto element) { return element->template getTextAs<int>(); }, false);
        getFieldFromXml(element, "playType", instance->playbackType, [](auto element) { return Utils::toPlaybackType(element->template getTextAs<std::string>()); }, false);
        getFieldFromXml(element, "horzMeasExprAlign", instance->horzMeasExprAlign, [](auto element) { return Utils::toHorizontalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "horzExprAlign", instance->horzExprJustification, [](auto element) { return Utils::toHorizontalExprJustification(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "vertMeasExprAlign", instance->vertMeasExprAlign, [](auto element) { return Utils::toVerticalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "measXAdjust", instance->measXAdjust, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "yAdjustEntry", instance->yAdjustEntry, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "yAdjustBaseline", instance->yAdjustBaseline, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "useCategoryFonts", instance->useCategoryFonts, [](auto) { return true; }, false);
        getFieldFromXml(element, "useCategoryPos", instance->useCategoryPos, [](auto) { return true; }, false);
        getFieldFromXml(element, "newEnclosure", instance->hasEnclosure, [](auto) { return true; }, false);
        getFieldFromXml(element, "breakMmRest", instance->breakMmRest, [](auto) { return true; }, false);
        getFieldFromXml(element, "useAuxData", instance->useAuxData, [](auto) { return true; }, false);
        getFieldFromXml(element, "descStr", instance->description, [](auto element) { return element->getText(); }, false);

        //Add this instance's id to the marking category's set.
        if (instance->categoryId) {
            elementLinker.addResolver([instance](const DocumentPtr& document) {
                auto markingCat = document->getOthers()->get<MarkingCategory>(instance->categoryId);
                if (!markingCat) {
                    throw std::invalid_argument("Marking category for text expression " + std::to_string(instance->getCmper()) + " does not exist.");
                }
                markingCat->textExpressions.emplace(instance->getCmper(), instance);                
            });
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
