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

namespace musx {
namespace dom {

/// @enum AlignJustify
/// @brief Alignment and justification options for staff and group names.
enum class AlignJustify
{
    Left,   ///< Left alignment or justification (the default value.)
    Right,  ///< Right alignment.
    Center  ///< Center alignment.
};

/// @brief Curve contour direction for ties and slurs.
enum class CurveContourDirection
{
    // NOTE: the order here is important for sorting instances. Do not change it.
    Auto,       ///< contour is automatic, indeterminate, or does not apply
    Down,       ///< contour is curved downward (tips face up)
    Up          ///< contour is curved upward (tips face down)
};

/// @enum DateFormat
/// @brief Date format options. This value is coded into the Enigma `date` insert when the page title is created.
enum class DateFormat : int
{
    Short = 0,      ///< Short date format, based on locale. US format is MM/DD/YY (Default value may not appear in the xml)
    Long = 1,       ///< Long date format, based on locale. US format is Month DD, YYYY.
    Abbrev = 2      ///< Abbreviated date format, based on locale. US format is Mon DD, YYYY.
};

/// @enum DefaultClefType
/// @brief Clef types used by default in Finale documents. The values correspond to indices into
/// @ref musx::dom::options::ClefOptions::clefDefs.
/// Note that the user may change the clef list to any clefs the user wishes. These are only defaults, but
/// they are frequently left unchanged.
enum class DefaultClefType : ClefIndex
{
    Treble          = 0,  ///< G clef, standard treble.
    Alto            = 1,  ///< C clef, centered on third line (Alto clef).
    Tenor           = 2,  ///< C clef, centered on fourth line (Tenor clef).
    Bass            = 3,  ///< F clef, standard bass.
    Percussion      = 4,  ///< Percussion clef, open rectangle (no pitch).
    Treble8vb       = 5,  ///< G clef, sounds one octave lower (8vb).
    Bass8vb         = 6,  ///< F clef, sounds one octave lower (8vb).
    Baritone        = 7,  ///< F clef on third line (Baritone clef).
    FrenchViolin    = 8,  ///< G clef placed on first line (French violin clef).
    BaritoneC       = 9,  ///< C clef on fifth line (Baritone clef).
    MezzoSoprano    = 10, ///< C clef on second line (Mezzo-soprano clef).
    Soprano         = 11, ///< C clef on first line (Soprano clef).
    AltPercussion   = 12, ///< Alternate percussion clef, heavy vertical hash marks (no pitch).
    Treble8va       = 13, ///< G clef, sounds one octave higher (8va).
    Bass8va         = 14, ///< F clef, sounds one octave higher (8va).
    Blank           = 15, ///< Blank clef (invisible, no symbol).
    Tab1            = 16, ///< Tablature clef (5 lines).
    Tab2            = 17  ///< Tablature clef (5 lines, alternative style).
};

/**
 * @enum LyricTextType
 * @brief The lyric text type if this is a lyrics smart shape.
 */
enum class LyricTextType
{
    Verse,          ///< The assignment is to a Verse lyrics text block.
    Chorus,         ///< The assignment is to a Chorus lyrics text block.
    Section,        ///< The assignment is to a Section lyrics text block.
};

/**
 * @enum NoteType
 * @brief Enum class representing note types based on EDU values.
 *
 * The values are expressed in hexadecimal.
 */
enum class NoteType : Edu
{
    Maxima = 0x8000,
    Longa = 0x4000,
    Breve = 0x2000,
    Whole = EDU_PER_WHOLE_NOTE,
    Half = 0x0800,
    Quarter = 0x0400,
    Eighth = 0x0200,
    Note16th = 0x0100,
    Note32nd = 0x0080,
    Note64th = 0x0040,
    Note128th = 0x0020,
    Note256th = 0x0010,
    Note512th = 0x0008,
    Note1024th = 0x0004,
    Note2048th = 0x0002,
    Note4096th = 0x0001
};

/**
 * @enum ShowClefMode
 * @brief Enum representing the clef display mode for a frame.
 */
enum class ShowClefMode
{
    WhenNeeded, ///< Clef is displayed only when needed (the default).
    Never,      ///< Clef is never displayed. (xml value is "hidden")
    Always      ///< Clef is always displayed. (xml value is "forced")
};

} // namespace dom
} // namespace musx
