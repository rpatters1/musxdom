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

#include "musx/util/Logger.h"

#include "BaseClasses.h"
// do not add other dom class dependencies. Use Implementations.cpp for implementations that need total class access.

namespace musx {
namespace dom {

namespace others {

/**
 * @enum SmartShapeType
 * @brief All the smart shape types supported by Finale
 *
 * The enum values match Finale's internal values. (Note that 24 is skipped.)
 * However, they are saved in the xml as text values. These are camel case and otherwise match
 * the enum names, except as noted.
 */
enum class SmartShapeType : int
{
    SlurDown = 0,       ///< Downward Slur (tips point up). This is the default and may not appear in the xml, but the text is in the Finale binary.
    SlurUp,             ///< Upward Slur (tips point down).
    Decrescendo,        ///< Decrescendo hairpin. (xml value is "decresc")
    Crescendo,          ///< Crescendo hairpin. (xml value is "cresc")
    OctaveDown,         ///< Ottava bassa. (8vb)
    OctaveUp,           ///< Ottava. (8va)
    DashLineUp,         ///< Dashed line with upward hook.
    DashLineDown,       ///< Dashed line with downward hook.                                == 7
    DashSlurDown,       ///< Thin line dashed downward slur (tips point up).
    DashSlurUp,         ///< Thin line dashed upward slur (tips point down).
    DashLine,           ///< Dashed line.
    SolidLine,          ///< Solid line.
    SolidLineDown,      ///< Solid line with downward right hook.
    SolidLineUp,        ///< Solid line with upward right hook.
    Trill,              ///< Trill (includes "tr" symbol at the beginning).                 == 14
    SlurAuto,           ///< Slur with direction automatically determined.
    DashSlurAuto,       ///< Thin line dashed slur with direction automatically determined.
    TrillExtension,     ///< Trill extension: only the wavy line. (xml value is "trillExt")
    SolidLineDownBoth,  ///< Solid line with downward hooks on both ends. (xml value is "solidLineDown2")
    SolidLineUpBoth,    ///< Solid line with upward hooks on both ends. (xml value is "solidLineUp2")
    TwoOctaveDown,      ///< Quindicesima bassa (15ma bassa)
    TwoOctaveUp,        ///< Quindicesima (15ma)                                            == 21
    DashLineDownBoth,   ///< Dashed line with downward hooks on both ends. (xml value is "dashLineDown2")
    DashLineUpBoth,     ///< Dashed line with upward hooks on both ends. (xml value is "dashLineUp2")
    Glissando = 25,     ///< Solid line glissando                                           == 25 (24 has no value)
    TabSlide,           ///< Tab slide (a solid line that is often used as a note-attached glissando)
    BendHat,            ///< Bend hat (solid up-down line with a corner in the middle)
    BendCurve,          ///< Bend curve
    CustomLine,         ///< User-created custom line. (xml value is "smartLine")
    SolidLineUpLeft,    ///< Solid line with upward left hook.
    SolidLineDownLeft,  ///< Solid line with downward left hook.
    DashLineUpLeft,     ///< Dashed line with upward left hook.                             == 32
    DashLineDownLeft,   ///< Dashed line with downward left hook.
    SolidLineUpDown,    ///< Solid line with upward left hook and downward right hook.
    SolidLineDownUp,    ///< Solid line with downward left hook and upward right hook.
    DashLineUpDown,     ///< Dashed line with upward left hook and downward right hook.
    DashLineDownUp,     ///< Dashed line with downward left hook and upward right hook.
    Hyphen,             ///< Lyric hyphen (used only with lyric assignments)
    WordExtension,      ///< Lyric word extension (used only with lyric assignments)
    DashContourSlurDown, ///< Contoured line dashed downward slur (tips point up).
    DashContourSlurUp,  ///< Contoured line dashed upward slur (tips point down).
    DashContouSlurAuto, ///< Contoured line dashed slur with direction automatically determined.
};

} // namespace others

} // namespace dom
} // namespace musx