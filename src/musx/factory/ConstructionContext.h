#pragma once

#include <set>

#include "musx/dom/DocumentElement.h"

namespace musx {
namespace factory {

/// @brief State shared by all factories during one document construction.
class ConstructionContext
{
public:
    /// @brief Records a non-default font definition ID referenced during construction.
    void registerFontId(dom::Cmper fontId)
    {
        if (fontId != 0) {
            m_referencedFontIds.insert(fontId);
        }
    }

private:
    friend void resolveFontDefinitions(const dom::DocumentPtr&, const ConstructionContext&);

    /// @brief Returns all non-default font definition IDs referenced during construction.
    [[nodiscard]] const std::set<dom::Cmper>& referencedFontIds() const { return m_referencedFontIds; }
    [[nodiscard]] std::set<dom::Cmper>& referencedFontIds() { return m_referencedFontIds; }

    std::set<dom::Cmper> m_referencedFontIds;
};

} // namespace factory
} // namespace musx
