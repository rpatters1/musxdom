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
#include "musx/dom/Texts.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

namespace musx {
namespace factory {

using namespace dom;
using namespace dom::texts;

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

template <>
struct FieldPopulator<TextsBase> : public FactoryBase
{
    static void populate(const std::shared_ptr<TextsBase>& instance, const std::shared_ptr<xml::IXmlElement>& element, ElementLinker&)
    {
        instance->text = element->getText();
    }
};

template <>
struct FieldPopulator<FileInfoText> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<LyricsVerse> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<LyricsChorus> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<LyricsSection> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<BlockText> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<SmartShapeText> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<ExpressionText> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

template <>
struct FieldPopulator<BookmarkText> : public FieldPopulator<TextsBase>
{
    using FieldPopulator<TextsBase>::populate;
};

#endif // DOXYGEN_SHOULD_IGNORE_THIS

} // namespace factory
} // namespace musx
