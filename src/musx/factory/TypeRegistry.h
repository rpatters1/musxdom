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
#pragma once

#include <array>
#include <memory>
#include <string>
#include <tuple>

#include "musx/dom/ObjectPool.h"
#include "musx/dom/BaseClasses.h"
#include "musx/dom/CommonClasses.h"
#include "musx/dom/Options.h"
#include "musx/dom/Others.h"
#include "musx/dom/Details.h"
#include "musx/dom/Graphics.h"
#include "musx/dom/Ossia.h"
#include "musx/dom/ShapeDesigner.h"
#include "musx/dom/SmartShape.h"
#include "musx/dom/Staff.h"
#include "musx/dom/Entries.h"
#include "musx/dom/Texts.h"
#include "musx/xml/XmlInterface.h"
#include "FieldPopulatorsOptions.h"
#include "FieldPopulatorsOthers.h"
#include "FieldPopulatorsDetails.h"
#include "FieldPopulatorsTexts.h"

#ifdef _MSC_VER
#  pragma warning(push)
#  pragma warning(disable : 4244) // disable spurious warnings due to overzealous MSC checking paths that are protected by constexpr
#endif

namespace musx {
namespace factory {

/**
 * @brief A registry of types for mapping XML node names to types.
 *
 * This template struct provides a mechanism to map XML node names to their corresponding types
 * and supports instantiation of types based on these names.
 *
 * @tparam Types The list of types to be registered.
 */
template <typename... Types>
class TypeRegistry
{
private:
    using VariantType = std::variant<Types*...>;
    using Base = dom::Base;

    /**
     * @brief A compile-time registry of types, each associated with an XML node name.
     *
     * The registry is an array of pairs, where each pair contains a node name (as a string)
     * and a corresponding type pointer (as nullptr).
     */
    static inline const auto registry = []() {
        return std::unordered_map<std::string_view, VariantType>{
            {Types::XmlNodeName, VariantType(static_cast<Types*>(nullptr))}...
        };
    }();

    /**
     * @brief Finds the registered type corresponding to the provided node name.
     *
     * Searches the registry for an entry that matches the given node name.
     *
     * @param nodeName The XML node name to search for.
     * @return A pair consisting of a boolean indicating success and a type pointer if found.
     */
    static std::optional<VariantType> findRegisteredType(std::string_view nodeName)
    {
        const auto it = registry.find(nodeName);
        if (it == registry.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    /// @brief Explicit function to avoid bug in MSC around lamdas inside std::visit.
    template <typename T, typename PoolPtr, typename... Args>
    static auto getScoreValue(const PoolPtr& pool, Args&&... args)
    {
        if constexpr (std::is_same_v<PoolPtr, ::musx::dom::OthersPoolPtr> || std::is_same_v<PoolPtr, ::musx::dom::DetailsPoolPtr>) {
            return pool->template get<T>(SCORE_PARTID, std::forward<Args>(args)...);
        } else {
            return pool->template get<T>(std::forward<Args>(args)...);
        }
    }

public:
    /// @brief Determine at compile-time if type T is a class in this registry.
    /// @tparam T The class to check.
    template <typename T>
    struct is_registered_type : std::disjunction<std::is_same<T, Types>...> {};

    /// @brief Shorthand to get the value of @ref is_registered_type.
    /// @tparam T  The class to check.
    template <typename T>
    static constexpr bool is_registered_type_v = is_registered_type<T>::value;

    /// @struct CreatedInstanceInfo
    /// @brief Information about a created instance returned by #createInstance
    struct CreatedInstanceInfo
    {
        /// @brief Constructor
        CreatedInstanceInfo(std::shared_ptr<Base> inst, std::string_view nodeName)
            : instance(inst), xmlNodeName(nodeName) {}

        std::shared_ptr<Base> instance; ///< The newly created instance.
        std::string_view xmlNodeName;   ///< The static std::string_view containing the instance's node name.
    };

    /**
     * @brief Creates an instance of the registered type corresponding to the provided node name.
     *
     * Uses the node name to look up the registered type and create an instance of it.
     *
     * @tparam PoolPtr The object pool type for getting score versions (when doing part linkage)
     * @tparam Args The argument types required by the constructor of the target type.
     * @param pool The object pool for getting score versions (when doing part linkage)
     * @param node The XML node from which an instance is to be created.
     * @param elementLinker The @ref ElementLinker instance that is used to resolve all internal connections after the document is created.
     * @param document The document that we are creating the instance for.
     * @param args Arguments to be forwarded to the constructor of the target type.
     * @return A shared pointer to the created instance of the base type, or nullptr if not found.
     */
    template <typename PoolPtr, typename... Args>
    static std::optional<CreatedInstanceInfo> createInstance([[maybe_unused]]const PoolPtr& pool, const XmlElementPtr& node, ElementLinker& elementLinker, const DocumentPtr& document, Args&&... args)
    {
        auto typePtr = TypeRegistry::findRegisteredType(node->getTagName());
        if (!typePtr.has_value()) {
            return std::nullopt; // Type not yet implemented
        }
        
        return std::visit(
            [&](auto const& ptr) -> CreatedInstanceInfo {
                using T = std::remove_pointer_t<std::remove_reference_t<decltype(ptr)>>;
                // Only enable this part if T is constructible with Args...
                if constexpr (std::is_constructible_v<T, const DocumentPtr&, Cmper, Base::ShareMode, Args...>) {
                    auto partAttr = node->findAttribute("part");
                    Cmper partId = partAttr ? partAttr->getValueAs<Cmper>() : SCORE_PARTID; // zero is the score ID
                    auto shareMode = Base::ShareMode::All;
                    if (auto shareAttr = node->findAttribute("shared")) {
                        shareMode = shareAttr->getValueAs<bool>() ? Base::ShareMode::Partial : Base::ShareMode::None;
                    }
                    auto instance = std::make_shared<T>(document, partId, shareMode, std::forward<Args>(args)...);
                    if constexpr (!std::is_same_v<PoolPtr, ::musx::dom::EntryPoolPtr>) {
                        if (instance->getShareMode() == Base::ShareMode::Partial) {
                            for (auto child = node->getFirstChildElement(); child; child = child->getNextSibling()) {
                                instance->addUnlinkedNode(child->getTagName());
                            }
                            auto scoreValue = getScoreValue<T>(pool, std::forward<Args>(args)...);
                            if (scoreValue) {
                                *instance = *scoreValue;
                            } else {
                                throw std::invalid_argument("Score instance not found for partially linked part instance");
                            }
                        }
                    }
                    factory::FieldPopulator<T>::populate(instance, node, elementLinker);
                    return CreatedInstanceInfo(instance,T::XmlNodeName);
                } else {
                    assert(false); // This path should never actually be taken, but it is required for std::visit.
                    throw std::logic_error("Type for " + node->getTagName() + " is not constructible with given arguments");
                }
            },
            typePtr.value()
        );
    }
};

/**
 * @brief The type registery for `<options>`.
 *
 * These types are maintained in the order in which Finale serializes them (based on observation).
 */
using RegisteredOptions = TypeRegistry <
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
    dom::options::TupletOptions
>;

/**
 * @brief The type registery for `<others>`.
 *
 * These types are maintained in the order in which Finale serializes them (based on observation).
 */
using RegisteredOthers = TypeRegistry <
    dom::others::AcciAmountFlats,
    dom::others::AcciAmountSharps,
    dom::others::AcciOrderFlats,
    dom::others::AcciOrderSharps,
    dom::others::OssiaBounds,           // this was possibly internally "arbitBounds" at one time, which may be why it serializes here  
    dom::others::OssiaHeader,           // this was possibly internally "arbitHeader" at one time, which may be why it serializes here  
    dom::others::OssiaMusic,            // this was possibly internally "arbitMusic" at one time, which may be why it serializes here
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
    dom::others::SystemLock,        // xml node is "lockMeas", which is still sequenced non-alphabetically
    dom::others::FretboardGroup,
    dom::others::FretInstrument,
    dom::others::FretboardStyle,
    dom::others::StaffUsed,         // xml node is "instUsed"
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
    dom::others::ShapeData,
    dom::others::ShapeDef,
    dom::others::ShapeInstructionList,
    dom::others::SplitMeasure,
    dom::others::SmartShape,
    dom::others::SmartShapeMeasureAssign,
    dom::others::SmartShapeCustomLine,      // node name is `ssLineStyle`
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
    dom::others::FileUrlBookmark
>;

/**
 * @brief The type registery for `<details>`.
 *
 * These types are maintained in the order in which Finale serializes them (based on observation).
 */
using RegisteredDetails = TypeRegistry <
    dom::details::AccidentalAlterations,
    dom::details::EntrySize,
    dom::details::ArticulationAssign,
    dom::details::BaselineLyricsChorus,
    dom::details::BaselineLyricsSection,
    dom::details::BaselineLyricsVerse,
    dom::details::BeamExtensionDownStem,
    dom::details::BeamExtensionUpStem,
    dom::details::StemAlterationsUnderBeam,         // "beamStemAdjust" is the xml key
    dom::details::BeamStubDirection,
    dom::details::BeamAlterationsDownStem,          // Finale serializes the beam alts out of alpha sequence
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
    dom::details::IndependentStaffDetails,  // "floats" is the xml key
    dom::details::StaffSize,
    dom::details::FretboardDiagram,
    dom::details::GFrameHold,
    dom::details::KeySymbolListElement,
    dom::details::LyricEntryInfo,
    dom::details::MeasureNumberIndividualPositioning,
    dom::details::MeasureOssiaAssign,
    dom::details::MeasureTextAssign,
    dom::details::StaffGroup,
    dom::details::NoteAlterations,          // this is out of alpha sequence, but that's how Finale serializes it
    dom::details::SecondaryBeamBreak,
    dom::details::ShapeNote,
    dom::details::ShapeNoteStyle,
    dom::details::SmartShapeEntryAssign,
    dom::details::StemAlterations,
    dom::details::CustomDownStem,           // "stemDefDown" is the xml key
    dom::details::CustomUpStem,             // "stemDefUp" is the xml key
    dom::details::TablatureNoteMods,
    dom::details::TieAlterEnd,
    dom::details::TieAlterStart,
    dom::details::TupletDef,
    dom::details::PercussionNoteCode,
    dom::details::LyricAssignChorus,
    dom::details::LyricAssignSection,
    dom::details::LyricAssignVerse
>;

/**
 * @brief The type registery for `<entries>`.
 *
 * These types are maintained in the order in which Finale serializes them.
 */
using RegisteredEntries = TypeRegistry <
    dom::Entry
>;

/**
 * @brief The type registery for `<texts>`.
 *
 * These types are maintained in the order in which Finale serializes them.
 */
using RegisteredTexts = TypeRegistry <
    dom::texts::FileInfoText,
    dom::texts::LyricsVerse,
    dom::texts::LyricsChorus,
    dom::texts::LyricsSection,
    dom::texts::BlockText,
    dom::texts::SmartShapeText,
    dom::texts::ExpressionText,
    dom::texts::BookmarkText
>;

} // namespace factory

namespace dom {

/// @brief Pool type specialization for RegisteredOptions
template <typename T>
struct is_pool_type<OptionsPool, T> : musx::factory::RegisteredOptions::template is_registered_type<T> {};

/// @brief Pool type specialization for RegisteredOthers
template <typename T>
struct is_pool_type<OthersPool, T> : musx::factory::RegisteredOthers::template is_registered_type<T> {};

/// @brief Pool type specialization for RegisteredDetails
template <typename T>
struct is_pool_type<DetailsPool, T> : musx::factory::RegisteredDetails::template is_registered_type<T> {};

/// @brief Pool type specialization for RegisteredEntries
template <typename T>
struct is_pool_type<EntryPool, T> : musx::factory::RegisteredEntries::template is_registered_type<T> {};

/// @brief Pool type specialization for RegisteredTexts
template <typename T>
struct is_pool_type<TextsPool, T> : musx::factory::RegisteredTexts::template is_registered_type<T> {};

} // namespace dom
} // namespace musx

#ifdef _MSC_VER
#  pragma warning(pop)
#endif
