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
    static void populate(Enclosure& instance, const std::shared_ptr<xml::IXmlElement>& element)
    {
        getFieldFromXml(element, "xAdd", instance.xAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yAdd", instance.yAdd, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "xMargin", instance.xMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "yMargin", instance.yMargin, [](auto element) { return element->template getTextAs<Evpu>(); });
        getFieldFromXml(element, "lineWidth", instance.lineWidth, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "sides", instance.shape, [](auto element) {  return toShape(element->template getTextAs<uint8_t>()); });
        getFieldFromXml(element, "cornerRadius", instance.cornerRadius, [](auto element) { return element->template getTextAs<Efix>(); });
        getFieldFromXml(element, "fixedSize", instance.fixedSize, [](auto element) { return true; }, false);
        getFieldFromXml(element, "notTall", instance.notTall, [](auto element) { return true; }, false);
        getFieldFromXml(element, "opaque", instance.opaque, [](auto element) { return true; }, false);
        getFieldFromXml(element, "roundCorners", instance.roundCorners, [](auto element) { return true; }, false);
    }
};

template <>
struct FieldPopulator<FontDefinition> : public FactoryBase
{
    static void populate(FontDefinition& instance, const std::shared_ptr<xml::IXmlElement>& element)
    {
        getFieldFromXml(element, "charsetBank", instance.charsetBank, [](auto element) { return element->getText(); });
        getFieldFromXml(element, "charsetVal", instance.charsetVal, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "pitch", instance.pitch, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "family", instance.family, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "name", instance.name, [](auto element) { return element->getText(); });
    }
};

template <>
struct FieldPopulator<TextExpressionDef> : public FactoryBase
{
private:
    /**
     * @brief Converts XML string values to PlaybackType enum values.
     */
    static TextExpressionDef::PlaybackType toPlaybackType(const std::string& value)
    {
        if (value == "none") return TextExpressionDef::PlaybackType::None;
        if (value == "time") return TextExpressionDef::PlaybackType::Tempo;
        if (value == "midiController") return TextExpressionDef::PlaybackType::MidiController;
        if (value == "amplitude") return TextExpressionDef::PlaybackType::KeyVelocity;
        if (value == "transpose") return TextExpressionDef::PlaybackType::Transpose;
        if (value == "channel") return TextExpressionDef::PlaybackType::Channel;
        if (value == "patchChange") return TextExpressionDef::PlaybackType::MidiPatchChange;
        if (value == "percMidiMap") return TextExpressionDef::PlaybackType::PercussionMidiMap;
        if (value == "midiPitchwheel") return TextExpressionDef::PlaybackType::MidiPitchWheel;
        if (value == "dump") return TextExpressionDef::PlaybackType::Dump;
        if (value == "startTempo") return TextExpressionDef::PlaybackType::PlayTempoToolChanges;
        if (value == "stopTempo") return TextExpressionDef::PlaybackType::IgnoreTempoToolChanges;
        if (value == "swing") return TextExpressionDef::PlaybackType::Swing;
        if (value == "hpOn") return TextExpressionDef::PlaybackType::SmartPlaybackOn;
        if (value == "hpOff") return TextExpressionDef::PlaybackType::SmartPlaybackOff;
        throw std::invalid_argument("Unknown playbackType value in XML: " + value);
    }

    /**
     * @brief Converts XML string values to HorizontalMeasExprAlign enum values.
     */
    static TextExpressionDef::HorizontalMeasExprAlign toHorizontalMeasExprAlign(const std::string& value)
    {
        if (value == "manual") return TextExpressionDef::HorizontalMeasExprAlign::Manual;
        if (value == "leftOfAllNoteheads") return TextExpressionDef::HorizontalMeasExprAlign::LeftOfAllNoteheads;
        if (value == "leftOfPrimaryNotehead") return TextExpressionDef::HorizontalMeasExprAlign::LeftOfPrimaryNotehead;
        if (value == "stem") return TextExpressionDef::HorizontalMeasExprAlign::Stem;
        if (value == "centerPrimaryNotehead") return TextExpressionDef::HorizontalMeasExprAlign::CenterPrimaryNotehead;
        if (value == "centerAllNoteheads") return TextExpressionDef::HorizontalMeasExprAlign::CenterAllNoteheads;
        if (value == "rightOfAllNoteheads") return TextExpressionDef::HorizontalMeasExprAlign::RightOfAllNoteheads;
        if (value == "leftEdge") return TextExpressionDef::HorizontalMeasExprAlign::LeftBarline;
        if (value == "startTimeSig") return TextExpressionDef::HorizontalMeasExprAlign::StartTimeSig;
        if (value == "afterClefKeyTime") return TextExpressionDef::HorizontalMeasExprAlign::AfterClefKeyTime;
        if (value == "startOfMusic") return TextExpressionDef::HorizontalMeasExprAlign::StartOfMusic;
        if (value == "centerOverBarlines") return TextExpressionDef::HorizontalMeasExprAlign::CenterOverBarlines;
        if (value == "centerOverMusic") return TextExpressionDef::HorizontalMeasExprAlign::CenterOverMusic;
        if (value == "rightEdge") return TextExpressionDef::HorizontalMeasExprAlign::RightBarline;
        throw std::invalid_argument("Unknown horzMeasExprAlign value in XML: " + value);
    }

    /**
     * @brief Converts XML string values to VerticalMeasExprAlign enum values.
     */
    static TextExpressionDef::VerticalMeasExprAlign toVerticalMeasExprAlign(const std::string& value)
    {
        if (value == "manual") return TextExpressionDef::VerticalMeasExprAlign::Manual;
        if (value == "refLine") return TextExpressionDef::VerticalMeasExprAlign::RefLine;
        if (value == "aboveStaff") return TextExpressionDef::VerticalMeasExprAlign::AboveStaff;
        if (value == "belowStaff") return TextExpressionDef::VerticalMeasExprAlign::BelowStaff;
        if (value == "topNote") return TextExpressionDef::VerticalMeasExprAlign::TopNote;
        if (value == "bottomNote") return TextExpressionDef::VerticalMeasExprAlign::BottomNote;
        if (value == "aboveEntry") return TextExpressionDef::VerticalMeasExprAlign::AboveEntry;
        if (value == "belowEntry") return TextExpressionDef::VerticalMeasExprAlign::BelowEntry;
        if (value == "aboveStaffOrEntry") return TextExpressionDef::VerticalMeasExprAlign::AboveStaffOrEntry;
        if (value == "belowStaffOrEntry") return TextExpressionDef::VerticalMeasExprAlign::BelowStaffOrEntry;
        throw std::invalid_argument("Invalid vertMeasExprAlign value in XML: " + value);
    }

    /**
     * @brief Converts XML string values to HorizontalExprJustification enum values.
     */
    static TextExpressionDef::HorizontalExprJustification toHorizontalExprJustification(const std::string& value)
    {
        if (value == "left") return TextExpressionDef::HorizontalExprJustification::Left;
        if (value == "center") return TextExpressionDef::HorizontalExprJustification::Center;
        if (value == "right") return TextExpressionDef::HorizontalExprJustification::Right;
        throw std::invalid_argument("Invalid horzExprJustification value in XML: " + value);
    }

public:
    /**
     * @brief Populates the fields of a TextExpressionDef instance from an XML element.
     */
    static void populate(TextExpressionDef& instance, const std::shared_ptr<xml::IXmlElement>& element)
    {
        getFieldFromXml(element, "textIDKey", instance.textIDKey, [](auto element) { return element->template getTextAs<Cmper>(); });
        getFieldFromXml(element, "categoryID", instance.categoryID, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "value", instance.value, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "auxData1", instance.auxData1, [](auto element) { return element->template getTextAs<int>(); }, false);
        getFieldFromXml(element, "playType", instance.playbackType, [](auto element) { return toPlaybackType(element->template getTextAs<std::string>()); }, false);
        getFieldFromXml(element, "horzMeasExprAlign", instance.horzMeasExprAlign, [](auto element) { return toHorizontalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "horzExprAlign", instance.horzExprJustification, [](auto element) { return toHorizontalExprJustification(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "vertMeasExprAlign", instance.vertMeasExprAlign, [](auto element) { return toVerticalMeasExprAlign(element->template getTextAs<std::string>()); });
        getFieldFromXml(element, "measXAdjust", instance.measXAdjust, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "yAdjustEntry", instance.yAdjustEntry, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "yAdjustBaseline", instance.yAdjustBaseline, [](auto element) { return element->template getTextAs<int>(); });
        getFieldFromXml(element, "useCategoryFonts", instance.useCategoryFonts, [](auto element) { return true; }, false);
        getFieldFromXml(element, "useCategoryPos", instance.useCategoryPos, [](auto element) { return true; }, false);
        getFieldFromXml(element, "newEnclosure", instance.hasEnclosure, [](auto element) { return true; }, false);
        getFieldFromXml(element, "breakMmRest", instance.breakMmRest, [](auto element) { return true; }, false);
        getFieldFromXml(element, "useAuxData", instance.useAuxData, [](auto element) { return true; }, false);
        getFieldFromXml(element, "descStr", instance.descStr, [](auto element) { return element->getText(); }, false);
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
