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

#include <exception>

 // This header includes method implementations that need to see all the classes in the dom

#include "Options.h"
#include "Others.h"
#include "Document.h"

namespace musx {
namespace dom {

// ************************
// ***** DefaultFonts *****
// ************************

inline std::shared_ptr<FontInfo> options::DefaultFonts::getFontInfo(options::DefaultFonts::FontType type) const
{
    auto it = defaultFonts.find(type);
    if (it == defaultFonts.end()) {
        throw std::invalid_argument("Font type " + std::to_string(int(type)) + " not found in document");
    }
    return it->second;
}

inline std::shared_ptr<FontInfo> options::DefaultFonts::getFontInfo(const DocumentPtr& document, options::DefaultFonts::FontType type)
{
    auto options = document->getOptions();
    if (!options) {
        throw std::invalid_argument("No options found in document");
    }
    auto defaultFonts = options->get<options::DefaultFonts>();
    if (!defaultFonts) {
        throw std::invalid_argument("Default fonts not found in document");
    }
    return defaultFonts->getFontInfo(type);
}

// ********************
// ***** FontInfo *****
// ********************

inline std::string FontInfo::getFontName() const
{
    auto document = this->getDocument().lock();
    assert(document); // program bug if fail
    auto others = document->getOthers();
    assert(others); // program bug if fail
    auto fontDef = others->get<others::FontDefinition>(fontId);
    if (fontDef) {
        return fontDef->name;
    }
    throw std::invalid_argument("Font defintion not found for font id " + std::to_string(fontId));
}

// ****************************
// ***** MarkingCategiory *****
// ****************************

inline std::string others::MarkingCategory::getName() const
{
    auto document = this->getDocument().lock();
    assert(document); // program bug if fail
    auto others = document->getOthers();
    assert(others); // program bug if fail
    auto catName = others->get<others::MarkingCategoryName>(getCmper());
    if (catName) {
        return catName->name;
    }
    return {};
}

// *****************************
// ***** TextExpressionDef *****
// *****************************

inline std::shared_ptr<others::Enclosure> others::TextExpressionDef::getEnclosure() const
{
    if (!hasEnclosure) return nullptr;
    auto document = getDocument().lock();
    assert(document);
    auto others = document->getOthers();
    assert(others); // program bug if fail
    return others->get<others::TextExpressionEnclosure>(getCmper());
}

} // namespace dom    
} // namespace musx
