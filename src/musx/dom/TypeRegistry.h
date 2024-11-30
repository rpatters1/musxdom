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

#include <array>
#include <memory>
#include <string>
#include <tuple>

#include "BaseClasses.h"
#include "Others.h"

namespace musx {
namespace dom {

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

    /**
     * @brief A compile-time registry of types, each associated with an XML node name.
     *
     * The registry is an array of pairs, where each pair contains a node name (as a string)
     * and a corresponding type pointer (as nullptr).
     */
    static constexpr auto registry = std::array {
        std::pair<std::string_view, VariantType>{Types::XmlNodeName, static_cast<Types*>(nullptr)}...
    };

    /**
     * @brief Finds the registered type corresponding to the provided node name.
     *
     * Searches the registry for an entry that matches the given node name.
     *
     * @param nodeName The XML node name to search for.
     * @return A pair consisting of a boolean indicating success and a type pointer if found.
     */
    static constexpr std::optional<VariantType> findRegisteredType(std::string_view nodeName)
    {
        for (const auto& entry : registry) {
            if (entry.first == nodeName) {
                return entry.second;
            }
        }
        return std::nullopt;
    }

public:
    /**
     * @brief Creates an instance of the registered type corresponding to the provided node name.
     *
     * Uses the node name to look up the registered type and create an instance of it.
     *
     * @tparam Args The argument types required by the constructor of the target type.
     * @param nodeName The XML node name for which an instance is to be created.
     * @param args Arguments to be forwarded to the constructor of the target type.
     * @return A shared pointer to the created instance of the base type, or nullptr if not found.
     */
    template <typename... Args>
    static std::shared_ptr<Base> createInstance(std::string_view nodeName, Args&&... args)
    {
        auto typePtr = TypeRegistry::findRegisteredType(nodeName);
        if (!typePtr.has_value()) {
            return nullptr; // Type not found
        }

        return std::visit(
            [&](auto const& ptr) {
                using T = std::remove_pointer_t<std::remove_reference_t<decltype(ptr)>>;
                return std::make_shared<T>(std::forward<Args>(args)...);
            },
            typePtr.value()
        );
    }
};

/**
 * @brief The type registery.
 */
using RegisteredTypes = TypeRegistry<
    FontDefinition
    // Add pointers to additional supported types here.
>;

} // namespace dom
} // namespace musx
