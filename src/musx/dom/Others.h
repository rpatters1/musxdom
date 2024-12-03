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
struct FontInfo : public Base
{
    Cmper fontID = 0;                      ///< Font ID. This is a Cmper for others::FontDefinition.
    int fontSize = 0;                      ///< Font size.
    bool bold = false;                     ///< Bold effect (default false).
    bool italic = false;                   ///< Italic effect (default false).
    bool underline = false;                ///< Underline effect (default false).
    bool strikeout = false;                ///< Strikeout effect (default false).
    bool absolute = false;                 ///< Fixed size effect (default false).
    bool hidden = false;                   ///< Hidden effect (default false).

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

    /**
     * @brief Constructor
     * @param document A weak pointer to the document object.
     *
     * Constructs a FontInfo object that is associated with the provided document.
     */
    FontInfo(const std::weak_ptr<Document>& document) : Base(document) {}
};

namespace others {

} // namespace others
} // namespace dom
} // namespace musx