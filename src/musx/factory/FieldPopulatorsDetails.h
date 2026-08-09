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

#include "musx/dom/BaseClasses.h"
#include "musx/dom/Others.h"
#include "musx/dom/Details.h"
#include "musx/dom/Document.h"
#include "musx/xml/XmlInterface.h"
#include "FactoryBase.h"

#ifndef DOXYGEN_SHOULD_IGNORE_THIS

namespace musx {
namespace factory {

using namespace ::musx::xml;
using namespace ::musx::dom::details;

template <>
struct FieldPopulator<BaselineChords> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineExpressionsAbove> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineExpressionsBelow> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineFretboards> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineLyricsChorus> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineLyricsSection> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineLyricsVerse> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemChords> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemExpressionsAbove> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemExpressionsBelow> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemFretboards> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemLyricsChorus> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemLyricsSection> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BaselineSystemLyricsVerse> : private FieldPopulator<Baseline>
{
    using FieldPopulator<Baseline>::populate;
};

template <>
struct FieldPopulator<BeamAlterationsDownStem> : private FieldPopulator<BeamAlterations>
{
    using FieldPopulator<BeamAlterations>::populate;
};

template <>
struct FieldPopulator<BeamAlterationsUpStem> : private FieldPopulator<BeamAlterations>
{
    using FieldPopulator<BeamAlterations>::populate;
};

template <>
struct FieldPopulator<BeamExtensionDownStem> : private FieldPopulator<BeamExtension>
{
    using FieldPopulator<BeamExtension>::populate;
};

template <>
struct FieldPopulator<BeamExtensionUpStem> : private FieldPopulator<BeamExtension>
{
    using FieldPopulator<BeamExtension>::populate;
};

template <>
struct FieldPopulator<CustomDownStem> : private FieldPopulator<CustomStem>
{
    using FieldPopulator<CustomStem>::populate;
};

template <>
struct FieldPopulator<CustomUpStem> : private FieldPopulator<CustomStem>
{
    using FieldPopulator<CustomStem>::populate;
};

template <>
struct FieldPopulator<LyricAssignChorus> : private FieldPopulator<LyricAssign>
{
    using FieldPopulator<LyricAssign>::populate;
};

template <>
struct FieldPopulator<LyricAssignSection> : private FieldPopulator<LyricAssign>
{
    using FieldPopulator<LyricAssign>::populate;
};

template <>
struct FieldPopulator<LyricAssignVerse> : private FieldPopulator<LyricAssign>
{
    using FieldPopulator<LyricAssign>::populate;
};

template <>
struct FieldPopulator<SecondaryBeamAlterationsDownStem> : private FieldPopulator<BeamAlterations>
{
    using FieldPopulator<BeamAlterations>::populate;
};

template <>
struct FieldPopulator<SecondaryBeamAlterationsUpStem> : private FieldPopulator<BeamAlterations>
{
    using FieldPopulator<BeamAlterations>::populate;
};

template <>
struct FieldPopulator<ShapeNote> : private FieldPopulator<ShapeNoteBase>
{
    using FieldPopulator<ShapeNoteBase>::populate;
};

template <>
struct FieldPopulator<ShapeNoteStyle> : private FieldPopulator<ShapeNoteBase>
{
    using FieldPopulator<ShapeNoteBase>::populate;
};

template <>
struct FieldPopulator<StemAlterationsUnderBeam> : private FieldPopulator<StemAlterations>
{
    using FieldPopulator<StemAlterations>::populate;
};

template <>
struct FieldPopulator<TieAlterEnd> : private FieldPopulator<TieAlterBase>
{
    using FieldPopulator<TieAlterBase>::populate;
};

template <>
struct FieldPopulator<TieAlterStart> : private FieldPopulator<TieAlterBase>
{
    using FieldPopulator<TieAlterBase>::populate;
};

template <>
inline Bracket::BracketStyle toEnum<Bracket::BracketStyle>(const int& value)
{
    Bracket::BracketStyle result{};
    if (value >= static_cast<int>(Bracket::BracketStyle::None) &&
        value <= static_cast<int>(Bracket::BracketStyle::DeskBracket)) {
        result = static_cast<Bracket::BracketStyle>(value);
    } else {
        MUSX_UNKNOWN_XML("Invalid <bracket><id> value in XML for Bracket: " + std::to_string(value));
    }
    return result;
}

} // namespace factory
} // namespace musx

#endif // DOXYGEN_SHOULD_IGNORE THIS
