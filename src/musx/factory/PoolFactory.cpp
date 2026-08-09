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

#include "musx/factory/PoolFactory.h"

#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "musx/dom/Details.h"
#include "musx/dom/Entries.h"
#include "musx/dom/Graphics.h"
#include "musx/dom/Options.h"
#include "musx/dom/Ossia.h"
#include "musx/dom/Others.h"
#include "musx/dom/ShapeDesigner.h"
#include "musx/dom/SmartShape.h"
#include "musx/dom/Staff.h"
#include "musx/dom/Texts.h"
#include "musx/factory/FactoryBase.h"
#include "musx/factory/FieldPopulatorsDetails.h"
#include "musx/factory/FieldPopulatorsEntries.h"
#include "musx/factory/FieldPopulatorsOptions.h"
#include "musx/factory/FieldPopulatorsOthers.h"
#include "musx/factory/FieldPopulatorsTexts.h"
#include "musx/util/Logger.h"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4244) // template registry dispatch has intentional typed conversions
#endif

namespace musx {
namespace factory {
namespace {

struct CreatedInstanceInfo
{
    std::shared_ptr<dom::EnigmaBase> instance;
    std::string_view xmlNodeName;
};

template <typename T, typename PoolPtr, typename... Args>
auto getScoreValue(const PoolPtr& pool, Args&&... args)
{
    if constexpr (std::is_same_v<PoolPtr, dom::OthersPoolPtr>
                  || std::is_same_v<PoolPtr, dom::DetailsPoolPtr>) {
        return pool->template get<T>(dom::SCORE_PARTID, std::forward<Args>(args)...);
    } else {
        return pool->template get<T>(std::forward<Args>(args)...);
    }
}

template <typename T, typename PoolPtr, typename... Args>
CreatedInstanceInfo createRegisteredType(const PoolPtr& pool,
                                         const xml::XmlElementPtr& node,
                                         const dom::DocumentPtr& document,
                                         Args&&... args)
{
    static_assert(std::is_constructible_v<T, const dom::DocumentPtr&, dom::Cmper,
        dom::EnigmaBase::ShareMode, Args...>);
    const auto partAttr = node->findAttribute("part");
    const dom::Cmper partId = partAttr
        ? partAttr->template getValueAs<dom::Cmper>() : dom::SCORE_PARTID;
    auto shareMode = dom::EnigmaBase::ShareMode::All;
    if (const auto shareAttr = node->findAttribute("shared")) {
        shareMode = shareAttr->template getValueAs<bool>()
            ? dom::EnigmaBase::ShareMode::Partial : dom::EnigmaBase::ShareMode::None;
    }
    auto instance = std::make_shared<T>(
        document, partId, shareMode, std::forward<Args>(args)...);
    if constexpr (!std::is_same_v<PoolPtr, dom::EntryPoolPtr>) {
        if (shareMode == dom::EnigmaBase::ShareMode::Partial) {
            std::vector<std::string> unlinkedNodeNames;
            for (auto child = node->getFirstChildElement(); child; child = child->getNextSibling()) {
                unlinkedNodeNames.emplace_back(child->getTagName());
            }
            const auto scoreValue = getScoreValue<T>(pool, std::forward<Args>(args)...);
            if (!scoreValue) {
                throw std::invalid_argument(
                    "Score instance not found for partially linked part instance");
            }
            PartSharingFactory::initializePartial(instance, scoreValue, unlinkedNodeNames);
        }
    }
    FieldPopulator<T>::populate(instance, node);
    return {instance, T::XmlNodeName};
}

template <typename T, typename... Rest, typename PoolPtr, typename... Args>
std::optional<CreatedInstanceInfo> createRegisteredInstance(
    const PoolPtr& pool, const xml::XmlElementPtr& node,
    const dom::DocumentPtr& document, Args&&... args)
{
    if (node->getTagName() == T::XmlNodeName) {
        if constexpr (std::is_constructible_v<T, const dom::DocumentPtr&, dom::Cmper,
            dom::EnigmaBase::ShareMode, Args...>) {
            return createRegisteredType<T>(
                pool, node, document, std::forward<Args>(args)...);
        } else {
            assert(false);
            throw std::logic_error("Type for " + node->getTagName()
                + " is not constructible with given arguments");
        }
    }
    if constexpr (sizeof...(Rest) > 0) {
        return createRegisteredInstance<Rest...>(
            pool, node, document, std::forward<Args>(args)...);
    } else {
        return std::nullopt;
    }
}

template <typename... Types>
struct RegisteredTypes
{
    template <typename PoolPtr, typename... Args>
    static std::optional<CreatedInstanceInfo> createInstance(
        const PoolPtr& pool, const xml::XmlElementPtr& node,
        const dom::DocumentPtr& document, Args&&... args)
    {
        return createRegisteredInstance<Types...>(
            pool, node, document, std::forward<Args>(args)...);
    }
};

using RegisteredOptions = RegisteredTypes<
    dom::options::AccidentalOptions,
    dom::options::AlternateNotationOptions,
    dom::options::AugmentationDotOptions,
    dom::options::BarlineOptions,
    dom::options::BeamOptions,
    dom::options::ChordOptions,
    dom::options::ClefOptions,
    dom::options::FlagOptions,
    dom::options::FontOptions,
    dom::options::GraceNoteOptions,
    dom::options::KeySignatureOptions,
    dom::options::LineCurveOptions,
    dom::options::LyricOptions,
    dom::options::MiscOptions,
    dom::options::MultimeasureRestOptions,
    dom::options::MusicSpacingOptions,
    dom::options::MusicSymbolOptions,
    dom::options::NoteRestOptions,
    dom::options::PageFormatOptions,
    dom::options::PianoBraceBracketOptions,
    dom::options::RepeatOptions,
    dom::options::SmartShapeOptions,
    dom::options::StaffOptions,
    dom::options::StemOptions,
    dom::options::TextOptions,
    dom::options::TieOptions,
    dom::options::TimeSignatureOptions,
    dom::options::TupletOptions>;

using RegisteredOthers = RegisteredTypes<
    dom::others::AcciAmountFlats,
    dom::others::AcciAmountSharps,
    dom::others::AcciAmountSharps,
    dom::others::AcciOrderFlats,
    dom::others::AcciOrderSharps,
    dom::others::OssiaBounds,
    dom::others::OssiaHeader,
    dom::others::OssiaMusic,
    dom::others::ArticulationDef,
    dom::others::BeatChartElement,
    dom::others::ChordSuffixElement,
    dom::others::ChordSuffixPlayback,
    dom::others::ClefList,
    dom::others::DrumStaff,
    dom::others::DrumStaffStyle,
    dom::others::FileAlias,
    dom::others::FileDescription,
    dom::others::FilePath,
    dom::others::FontDefinition,
    dom::others::Frame,
    dom::others::TonalCenterFlats,
    dom::others::TonalCenterSharps,
    dom::others::SystemLock,
    dom::others::FretboardGroup,
    dom::others::FretInstrument,
    dom::others::FretboardStyle,
    dom::others::StaffUsed,
    dom::others::KeyFormat,
    dom::others::KeyMapArray,
    dom::others::KeyAttributes,
    dom::others::LayerAttributes,
    dom::others::MeasureNumberRegion,
    dom::others::MultimeasureRest,
    dom::others::Measure,
    dom::others::MeasureExprAssign,
    dom::others::NamePositionAbbreviated,
    dom::others::NamePositionStyleAbbreviated,
    dom::others::NamePositionFull,
    dom::others::NamePositionStyleFull,
    dom::others::TextBlock,
    dom::others::Page,
    dom::others::PageGraphicAssign,
    dom::others::PageOssiaAssign,
    dom::others::PageTextAssign,
    dom::others::RepeatBack,
    dom::others::RepeatEndingStart,
    dom::others::RepeatEndingText,
    dom::others::RepeatPassList,
    dom::others::RepeatBackIndividualPositioning,
    dom::others::RepeatEndingStartIndividualPositioning,
    dom::others::RepeatEndingTextIndividualPositioning,
    dom::others::TextRepeatIndividualPositioning,
    dom::others::ShapeData,
    dom::others::ShapeDef,
    dom::others::ShapeInstructionList,
    dom::others::ShapeGraphicAssign,
    dom::others::SplitMeasure,
    dom::others::SmartShape,
    dom::others::SmartShapeMeasureAssign,
    dom::others::SmartShapeCustomLine,
    dom::others::StaffSystem,
    dom::others::StaffListRepeatName,
    dom::others::StaffListRepeatParts,
    dom::others::StaffListRepeatPartsForced,
    dom::others::StaffListRepeatScore,
    dom::others::StaffListRepeatScoreForced,
    dom::others::Staff,
    dom::others::StaffStyle,
    dom::others::StaffStyleAssign,
    dom::others::ShapeExpressionDef,
    dom::others::TimeCompositeLower,
    dom::others::TimeCompositeUpper,
    dom::others::TempoChange,
    dom::others::TextExpressionDef,
    dom::others::TextExpressionEnclosure,
    dom::others::TextRepeatAssign,
    dom::others::TextRepeatDef,
    dom::others::TextRepeatEnclosure,
    dom::others::TextRepeatText,
    dom::others::PartDefinition,
    dom::others::PartGlobals,
    dom::others::PartVoicing,
    dom::others::MarkingCategory,
    dom::others::MarkingCategoryName,
    dom::others::StaffListCategoryName,
    dom::others::StaffListCategoryParts,
    dom::others::StaffListCategoryScore,
    dom::others::PercussionNoteInfo,
    dom::others::MultiStaffInstrumentGroup,
    dom::others::MultiStaffGroupId,
    dom::others::FileUrlBookmark>;

using RegisteredDetails = RegisteredTypes<
    dom::details::AccidentalAlterations,
    dom::details::EntrySize,
    dom::details::ArticulationAssign,
    dom::details::BaselineChords,
    dom::details::BaselineExpressionsAbove,
    dom::details::BaselineExpressionsBelow,
    dom::details::BaselineFretboards,
    dom::details::BaselineLyricsChorus,
    dom::details::BaselineLyricsSection,
    dom::details::BaselineLyricsVerse,
    dom::details::BeamExtensionDownStem,
    dom::details::BeamExtensionUpStem,
    dom::details::StemAlterationsUnderBeam,
    dom::details::BeamStubDirection,
    dom::details::BeamAlterationsDownStem,
    dom::details::BeamAlterationsUpStem,
    dom::details::SecondaryBeamAlterationsDownStem,
    dom::details::SecondaryBeamAlterationsUpStem,
    dom::details::Bracket,
    dom::details::CenterShape,
    dom::details::ChordAssign,
    dom::details::ClefOctaveFlats,
    dom::details::ClefOctaveSharps,
    dom::details::CrossStaff,
    dom::details::DotAlterations,
    dom::details::IndependentStaffDetails,
    dom::details::StaffSize,
    dom::details::FretboardDiagram,
    dom::details::GFrameHold,
    dom::details::KeySymbolListElement,
    dom::details::LyricEntryInfo,
    dom::details::MeasureGraphicAssign,
    dom::details::MeasureNumberIndividualPositioning,
    dom::details::MeasureOssiaAssign,
    dom::details::MeasureTextAssign,
    dom::details::StaffGroup,
    dom::details::NoteAlterations,
    dom::details::SecondaryBeamBreak,
    dom::details::ShapeNote,
    dom::details::ShapeNoteStyle,
    dom::details::SmartShapeEntryAssign,
    dom::details::StemAlterations,
    dom::details::CustomDownStem,
    dom::details::CustomUpStem,
    dom::details::TablatureNoteMods,
    dom::details::TieAlterEnd,
    dom::details::TieAlterStart,
    dom::details::TupletDef,
    dom::details::BaselineSystemChords,
    dom::details::BaselineSystemExpressionsAbove,
    dom::details::BaselineSystemExpressionsBelow,
    dom::details::BaselineSystemFretboards,
    dom::details::BaselineSystemLyricsChorus,
    dom::details::BaselineSystemLyricsSection,
    dom::details::BaselineSystemLyricsVerse,
    dom::details::PercussionNoteCode,
    dom::details::LyricAssignChorus,
    dom::details::LyricAssignSection,
    dom::details::LyricAssignVerse,
    dom::details::EntryPartFieldDetail>;

using RegisteredEntries = RegisteredTypes<dom::Entry>;

using RegisteredTexts = RegisteredTypes<
    dom::texts::FileInfoText,
    dom::texts::LyricsVerse,
    dom::texts::LyricsChorus,
    dom::texts::LyricsSection,
    dom::texts::BlockText,
    dom::texts::SmartShapeText,
    dom::texts::ExpressionText,
    dom::texts::BookmarkText>;

template <typename ObjectBase, typename PoolType, typename Extractor>
std::shared_ptr<PoolType> createPool(const xml::XmlElementPtr& element,
                                     const dom::DocumentPtr& document,
                                     const NodeFilter& filter,
                                     Extractor&& extractor)
{
    auto pool = std::make_shared<PoolType>(document);
#ifdef MUSX_DISPLAY_NODE_NAMES
    std::string currentTag;
    size_t currentTagCount = 0;
    util::Logger::log(util::Logger::LogLevel::Verbose, "============");
    util::Logger::log(util::Logger::LogLevel::Verbose, element->getTagName());
    util::Logger::log(util::Logger::LogLevel::Verbose, "============");
#endif
    for (auto child = element->getFirstChildElement(); child; child = child->getNextSibling()) {
        if (filter && !filter(child)) {
            continue;
        }
        if (auto info = extractor(child, pool)) {
#ifdef MUSX_DISPLAY_NODE_NAMES
            if (currentTag != child->getTagName()) {
                if (!currentTag.empty()) {
                    util::Logger::log(util::Logger::LogLevel::Verbose,
                        "  " + currentTag + " [" + std::to_string(currentTagCount) + "]");
                }
                currentTag = child->getTagName();
                currentTagCount = 0;
            }
            ++currentTagCount;
#endif
            MUSX_ASSERT_IF(child->getTagName() != info->xmlNodeName) {
                throw std::logic_error("Instance of " + std::string(info->xmlNodeName)
                    + " does not match xml tag " + child->getTagName());
            }
            auto typed = std::dynamic_pointer_cast<ObjectBase>(info->instance);
            MUSX_ASSERT_IF(!typed) {
                throw std::logic_error("Unable to cast instance to correct type for "
                    + std::string(info->xmlNodeName));
            }
            if constexpr (std::is_same_v<PoolType, dom::EntryPool>) {
                pool->add(typed->getEntryNumber(), typed);
            } else {
                pool->add(info->xmlNodeName, typed);
            }
        }
    }
#ifdef MUSX_DISPLAY_NODE_NAMES
    if (!currentTag.empty() && currentTagCount != 0) {
        util::Logger::log(util::Logger::LogLevel::Verbose,
            "  " + currentTag + " [" + std::to_string(currentTagCount) + "]");
    }
#endif
    return pool;
}

dom::Cmper textTypeToCmper(const std::string& type)
{
    using TextType = dom::texts::FileInfoText::TextType;
    static const std::unordered_map<std::string_view, TextType> typeMap = {
        {"title", TextType::Title}, {"composer", TextType::Composer},
        {"copyright", TextType::Copyright}, {"description", TextType::Description},
        {"lyricist", TextType::Lyricist}, {"arranger", TextType::Arranger},
        {"subtitle", TextType::Subtitle}
    };
    const auto it = typeMap.find(type);
    if (it == typeMap.end()) {
        throw std::invalid_argument("Unknown type attribute value for <fileInfo> node: " + type);
    }
    return dom::Cmper(it->second);
}

} // namespace

dom::OptionsPoolPtr OptionsFactory::create(
    const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
    const NodeFilter& filter)
{
    return createPool<dom::OptionsBase, dom::OptionsPool>(element, document, filter,
        [&](const auto& child, const auto& pool) {
            return RegisteredOptions::createInstance(pool, child, document);
        });
}

dom::OthersPoolPtr OthersFactory::create(
    const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
    const NodeFilter& filter)
{
    return createPool<dom::OthersBase, dom::OthersPool>(element, document, filter,
        [&](const auto& child, const auto& pool) {
            auto cmper = child->findAttribute("cmper");
            if (!cmper) {
                throw std::invalid_argument("missing cmper for others element " + child->getTagName());
            }
            if (auto inci = child->findAttribute("inci")) {
                return RegisteredOthers::createInstance(pool, child, document,
                    cmper->template getValueAs<dom::Cmper>(),
                    inci->template getValueAs<dom::Inci>());
            }
            return RegisteredOthers::createInstance(pool, child, document,
                cmper->template getValueAs<dom::Cmper>());
        });
}

dom::DetailsPoolPtr DetailsFactory::create(
    const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
    const NodeFilter& filter)
{
    return createPool<dom::DetailsBase, dom::DetailsPool>(element, document, filter,
        [&](const auto& child, const auto& pool) {
            std::optional<dom::Inci> inci;
            if (auto attr = child->findAttribute("inci")) {
                inci = attr->template getValueAs<dom::Inci>();
            }
            if (auto entnum = child->findAttribute("entnum")) {
                if (inci) {
                    return RegisteredDetails::createInstance(pool, child, document,
                        entnum->template getValueAs<dom::EntryNumber>(), *inci);
                }
                return RegisteredDetails::createInstance(pool, child, document,
                    entnum->template getValueAs<dom::EntryNumber>());
            }
            auto cmper1 = child->findAttribute("cmper1");
            auto cmper2 = child->findAttribute("cmper2");
            if (!cmper1 || !cmper2) {
                throw std::invalid_argument("missing cmper1 or cmper2 for details element "
                    + child->getTagName());
            }
            if (inci) {
                return RegisteredDetails::createInstance(pool, child, document,
                    cmper1->template getValueAs<dom::Cmper>(),
                    cmper2->template getValueAs<dom::Cmper>(), *inci);
            }
            return RegisteredDetails::createInstance(pool, child, document,
                cmper1->template getValueAs<dom::Cmper>(),
                cmper2->template getValueAs<dom::Cmper>());
        });
}

dom::EntryPoolPtr EntryFactory::create(
    const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
    const NodeFilter& filter)
{
    return createPool<dom::Entry, dom::EntryPool>(element, document, filter,
        [&](const auto& child, const auto& pool) {
            auto entnum = child->findAttribute("entnum");
            auto prev = child->findAttribute("prev");
            auto next = child->findAttribute("next");
            if (!entnum || !prev || !next) {
                throw std::invalid_argument("entry is missing entnum, prev, or next attribute");
            }
            return RegisteredEntries::createInstance(pool, child, document,
                entnum->template getValueAs<dom::EntryNumber>(),
                prev->template getValueAs<dom::EntryNumber>(),
                next->template getValueAs<dom::EntryNumber>());
        });
}

dom::TextsPoolPtr TextsFactory::create(
    const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
    const NodeFilter& filter)
{
    return createPool<dom::TextsBase, dom::TextsPool>(element, document, filter,
        [&](const auto& child, const auto& pool) {
            const std::string attributeName = child->getTagName() == dom::texts::FileInfoText::XmlNodeName
                ? "type" : "number";
            auto attribute = child->findAttribute(attributeName);
            if (!attribute) {
                throw std::invalid_argument("Element <" + child->getTagName()
                    + "> does not have attribute " + attributeName);
            }
            const auto number = attributeName == "type"
                ? textTypeToCmper(attribute->getValue())
                : attribute->template getValueAs<dom::Cmper>();
            return RegisteredTexts::createInstance(pool, child, document, number);
        });
}

} // namespace factory
} // namespace musx

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
