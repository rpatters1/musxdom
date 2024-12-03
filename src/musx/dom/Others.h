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

#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {
namespace others {

/**
 * @class FontDefinition
 * @brief The name and font characteristics of fonts contained in the musx file.
 *
 * The cmper is the font ID used in classes throughout the document.
 *
 * This class is identified by the XML node name "fontName".
 */
class FontDefinition : public OthersBase
{
public:
    /** @brief Constructor function */
    FontDefinition(int cmper, int inci, const std::weak_ptr<Document>& document)
        : OthersBase(cmper, inci, document)
    {
    }

    // Public properties corresponding to the XML structure
    std::string charsetBank; ///< probably only "Mac" or "Win"
    int charsetVal = 0;      ///< A value specifying the character set, usually 4095 or 0
    int pitch = 0;           ///< Represents the `<pitch>` element, e.g., 0. (use unknown)
    int family = 0;          ///< Represents the `<family>` element, e.g., 0. (use unknown)
    std::string name;        ///< The font name e.g., "Broadway Copyist Text".

    constexpr static std::string_view XmlNodeName = "fontName"; ///< The XML node name for this type.
};

} // namespace others

/**
 * @struct FontInfo
 * @brief Represents the default font settings for a particular element type.
 *
 * The FontInfo struct holds information about font properties, such as the font ID, size, and styles like
 * bold, italic, underline, strikeout, fixed size, and visibility.
 */
class FontInfo : public Base
{
public:
    /** @enum FontEffect describes each of the font effect style bits. */
    enum FontEffect : uint16_t
    {
        Plain      = 0x00, ///< No effects
        Bold       = 0x01, ///< Bold effect
        Italic     = 0x02, ///< Italic effect
        Underline  = 0x04, ///< Underline effect
        Strikeout  = 0x20, ///< Strikeout effect
        Absolute   = 0x40, ///< Fixed size effect
        Hidden     = 0x80  ///< Hidden effect
    };

    Cmper fontID = 0;           ///< Font ID. This is a Cmper for others::FontDefinition.
    int fontSize = 0;           ///< Font size.
    uint16_t fontEfx = Plain;   ///< Font effects stored as bitmask.

    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     * @param fontID The font ID.
     * @param fontSize The font size.
     * @param fontEfx The font effects (default: Plain).
     */
    FontInfo(const std::weak_ptr<Document>& document, Cmper fontID, int fontSize, uint16_t fontEfx = Plain)
        : Base(document), fontID(fontID), fontSize(fontSize), fontEfx(fontEfx) {}

    /**
     * @brief Check if a specific effect is enabled.
     * @param effect The font effect to check.
     * @return True if the effect is enabled, false otherwise.
     */
    bool hasEffect(FontEffect effect) const { return (fontEfx & effect) != 0; }

    /**
     * @brief Enable a specific font effect.
     * @param effect The font effect to enable.
     */
    void enableEffect(FontEffect effect) { fontEfx |= effect; }

    /**
     * @brief Disable a specific font effect.
     * @param effect The font effect to disable.
     */
    void disableEffect(FontEffect effect) { fontEfx &= ~effect; }

    /**
     * @brief Check if the font is bold.
     * @return True if bold is enabled, false otherwise.
     */
    bool bold() const { return hasEffect(Bold); }

    /**
     * @brief Check if the font is italic.
     * @return True if italic is enabled, false otherwise.
     */
    bool italic() const { return hasEffect(Italic); }

    /**
     * @brief Check if the font is underlined.
     * @return True if underline is enabled, false otherwise.
     */
    bool underline() const { return hasEffect(Underline); }

    /**
     * @brief Check if the font is strikeout.
     * @return True if strikeout is enabled, false otherwise.
     */
    bool strikeout() const { return hasEffect(Strikeout); }

    /**
     * @brief Check if the font has an absolute size.
     * @return True if absolute is enabled, false otherwise.
     */
    bool absolute() const { return hasEffect(Absolute); }

    /**
     * @brief Check if the font is hidden.
     * @return True if hidden is enabled, false otherwise.
     */
    bool hidden() const { return hasEffect(Hidden); }

    /**
     * @brief Get the name of the font.
     * @return The name of the font as a string.
     */
    std::string getFontName() const
    {
        auto document = this->getDocument().lock();
        assert(document); // program bug if fail
        auto others = document->getOthers();
        assert(others); // program bug if fail
        auto fontDef = others->get<others::FontDefinition>(fontID);
        if (fontDef) {
            return fontDef->name;
        }
        throw std::invalid_argument("Font defintion not found for font id " + std::to_string(fontID));
    }
};

namespace others {

} // namespace others
} // namespace dom
} // namespace musx