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

#include "musx/dom/BaseClasses.h"
#include "musx/dom/Options.h"
#include "musx/dom/Others.h"
#include "musx/dom/Details.h"
#include "musx/dom/Entries.h"
#include "musx/dom/Texts.h"
#include "musx/xml/XmlInterface.h"
#include "FieldPopulatorsOptions.h"
#include "FieldPopulatorsOthers.h"
#include "FieldPopulatorsDetails.h"
#include "FieldPopulatorsTexts.h"

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

public:
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
    static std::shared_ptr<Base> createInstance(const PoolPtr& pool, const XmlElementPtr& node, ElementLinker& elementLinker, const DocumentPtr& document, Args&&... args)
    {
        auto typePtr = TypeRegistry::findRegisteredType(node->getTagName());
        if (!typePtr.has_value()) {
            return nullptr; // Type not yet implemented
        }

        return std::visit(
            [&](auto const& ptr) -> std::shared_ptr<Base> {
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
                    if constexpr (!std::is_same_v<PoolPtr, EntryPoolPtr>) {
                        if (instance->getShareMode() == Base::ShareMode::Partial) {
                            for (auto child = node->getFirstChildElement(); child; child = child->getNextSibling()) {
                                instance->addUnlinkedNode(child->getTagName());
                            }
                            auto scoreValue = [&]() {
                                if constexpr (std::is_same_v<PoolPtr, OthersPoolPtr> || std::is_same_v<PoolPtr, DetailsPoolPtr>) {
                                    return pool->template get<T>(SCORE_PARTID, std::forward<Args>(args)...);
                                } else {
                                    return pool->template get<T>(std::forward<Args>(args)...);
                                }
                            }();
                            if (scoreValue) {
                                *instance = *scoreValue;
                            }
                            else {
                                throw std::invalid_argument("Score instance not found for partially linked part instance");
                            }
                        }
                    }
                    factory::FieldPopulator<T>::populate(instance, node, elementLinker);
                    return instance;
                } else {
                    throw std::runtime_error("Type for " + node->getTagName() + " is not constructible with given arguments");
                }
            },
            typePtr.value()
        );
    }
};

/**
 * @brief The type registery for `<options>`.
 *
 * These types are maintained in the order in which Finale serializes them.
 */
using RegisteredOptions = TypeRegistry <
    dom::options::AccidentalOptions,
    dom::options::AlternateNotationOptions,
    dom::options::AugmentationDotOptions,
    dom::options::BarlineOptions,
    dom::options::BeamOptions,
    dom::options::ClefOptions,
    dom::options::FlagOptions,
    dom::options::FontOptions,
    dom::options::GraceNoteOptions,
    dom::options::KeySignatureOptions,
    dom::options::LineCurveOptions,
    dom::options::MiscOptions,
    dom::options::MultimeasureRestOptions,
    dom::options::MusicSpacingOptions,
    dom::options::MusicSymbolOptions,
    dom::options::PageFormatOptions,
    dom::options::PianoBraceBracketOptions,
    dom::options::RepeatOptions,
    dom::options::SmartShapeOptions,
    dom::options::StaffOptions,
    dom::options::StemOptions,
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
    dom::others::FontDefinition,
    dom::others::Frame,
    dom::others::InstrumentUsed,
    dom::others::LayerAttributes,
    dom::others::MeasureNumberRegion,
    dom::others::MultimeasureRest,
    dom::others::Measure,
    dom::others::MeasureExprAssign,
    dom::others::TextBlock,
    dom::others::Page,
    dom::others::RepeatEndingStart,
    dom::others::StaffSystem,
    dom::others::Staff,
    dom::others::StaffStyle,
    dom::others::StaffStyleAssign,
    dom::others::ShapeExpressionDef,
    dom::others::TimeCompositeLower,
    dom::others::TimeCompositeUpper,
    dom::others::TempoChange,
    dom::others::TextExpressionDef,
    dom::others::TextExpressionEnclosure,
    dom::others::TextRepeatEnclosure,
    dom::others::PartDefinition,
    dom::others::PartGlobals,
    dom::others::MarkingCategory,
    dom::others::MarkingCategoryName,
    dom::others::MultiStaffInstrumentGroup,
    dom::others::MultiStaffGroupId
>;

/**
 * @brief The type registery for `<details>`.
 *
 * These types are maintained in the order in which Finale serializes them.
 */
using RegisteredDetails = TypeRegistry <
    dom::details::GFrameHold,
    dom::details::StaffGroup,
    dom::details::TupletDef
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
} // namespace musx
