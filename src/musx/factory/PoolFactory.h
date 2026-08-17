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

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "musx/dom/Document.h"
#include "musx/dom/ObjectPool.h"
#include "musx/xml/XmlInterface.h"
#include "musx/factory/ConstructionContext.h"

namespace musx {
namespace factory {

/**
 * @brief Selects which child elements a pool factory creates objects from.
 * @details Returning `false` skips the element entirely, as if it were absent from the
 * source. This lets a client seed a pool from an allowlisted subset of an element without
 * editing or rebuilding the source XML. An empty (default-constructed) filter accepts every
 * child, which is the behavior when no filter is supplied.
 *
 * Which nodes belong in a document is caller policy. A subset that omits nodes other nodes
 * depend on produces a document that fails integrity validation when the construction
 * session is finished, so the caller is responsible for selecting a self-consistent subset.
 */
using NodeFilter = std::function<bool(const xml::XmlElementPtr&)>;

/** @brief Initializes a partially shared part object from its score object. */
class PartSharingFactory
{
public:
    /**
     * @brief Copies score values into a partial part object and records its unlinked fields.
     * @details Call this before applying the part-specific field values. Integrity validation is
     * deferred until the document construction session is finished.
     */
    template <typename T, typename NodeNames>
    static void initializePartial(const std::shared_ptr<T>& partInstance,
                                  const std::shared_ptr<const T>& scoreInstance,
                                  const NodeNames& unlinkedNodeNames)
    {
        static_assert(std::is_base_of_v<dom::EnigmaBase, T>, "T must derive from EnigmaBase");
        if (!partInstance || !scoreInstance) {
            throw std::invalid_argument("Partial sharing requires both score and part instances.");
        }
        if (partInstance->getShareMode() != dom::EnigmaBase::ShareMode::Partial) {
            throw std::invalid_argument("Invalid score/part instances for partial sharing.");
        }
        *partInstance = *scoreInstance;
        dom::PartContextRebinder<T>::rebind(partInstance);
        for (const auto& nodeName : unlinkedNodeNames) {
            partInstance->addUnlinkedNode(std::string(nodeName));
        }
    }
};

/** @brief Creates an options pool from an XML `<options>` element. */
class OptionsFactory
{
public:
    /// @param element The element whose children are created.
    /// @param document The document that owns the created objects.
    /// @param filter [optional] Restricts creation to the children it accepts. See @ref NodeFilter.
    [[nodiscard]] static dom::OptionsPoolPtr create(
        ConstructionContext& context, const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
        const NodeFilter& filter = {});
};

/** @brief Creates an others pool from an XML `<others>` element. */
class OthersFactory
{
public:
    /// @param element The element whose children are created.
    /// @param document The document that owns the created objects.
    /// @param filter [optional] Restricts creation to the children it accepts. See @ref NodeFilter.
    [[nodiscard]] static dom::OthersPoolPtr create(
        ConstructionContext& context, const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
        const NodeFilter& filter = {});
};

/** @brief Creates a details pool from an XML `<details>` element. */
class DetailsFactory
{
public:
    /// @param element The element whose children are created.
    /// @param document The document that owns the created objects.
    /// @param filter [optional] Restricts creation to the children it accepts. See @ref NodeFilter.
    [[nodiscard]] static dom::DetailsPoolPtr create(
        ConstructionContext& context, const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
        const NodeFilter& filter = {});
};

/** @brief Creates an entry pool from an XML `<entries>` element. */
class EntryFactory
{
public:
    /// @param element The element whose children are created.
    /// @param document The document that owns the created objects.
    /// @param filter [optional] Restricts creation to the children it accepts. See @ref NodeFilter.
    [[nodiscard]] static dom::EntryPoolPtr create(
        ConstructionContext& context, const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
        const NodeFilter& filter = {});
};

/** @brief Creates a texts pool from an XML `<texts>` element. */
class TextsFactory
{
public:
    /// @param element The element whose children are created.
    /// @param document The document that owns the created objects.
    /// @param filter [optional] Restricts creation to the children it accepts. See @ref NodeFilter.
    [[nodiscard]] static dom::TextsPoolPtr create(
        ConstructionContext& context, const xml::XmlElementPtr& element, const dom::DocumentPtr& document,
        const NodeFilter& filter = {});
};

} // namespace factory
} // namespace musx
