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

#include <cstdint>

#if defined(_WIN32)
#define MUSX_RUNNING_ON_WINDOWS
#elif defined(__APPLE__) && defined(__MACH__)
#define MUSX_RUNNING_ON_MACOS
#elif defined(__unix__) || defined(__linux__)
#define MUSX_RUNNING_ON_LINUX_UNIX
#else
#error "Unable to detect operating system platform."
#endif

#ifdef MUSX_THROW_ON_INTEGRITY_CHECK_FAIL
#define MUSX_INTEGRITY_ERROR(S) throw ::musx::dom::integrity_error(S)
#else
#define MUSX_INTEGRITY_ERROR(S) ::musx::util::Logger::log(::musx::util::Logger::LogLevel::Warning, (S))
#endif

#ifdef MUSX_THROW_ON_UNKNOWN_XML
#define MUSX_UNKNOWN_XML(S) throw ::musx::factory::unknown_xml_error(S)
#else
#define MUSX_UNKNOWN_XML(S) ::musx::util::Logger::log(::musx::util::Logger::LogLevel::Warning, (S))
#endif

#define MUSX_ASSERT_IF(TEST) \
assert(!(TEST)); \
if (TEST)

namespace musx {
namespace dom {

using Cmper = uint16_t;             ///< Enigma "comperator" key type
using Inci = int16_t;               ///< Enigma "incident" key type
using Evpu = int32_t;               ///< EVPU value (288 per inch)
using EvpuFloat = double;           ///< EVPU fractional value (288.0 per inch)
using Evpu16ths = int32_t;          ///< 1/16 of an EVPU.
using Efix = int32_t;               ///< EFIX value (64 per EVPU, 64*288=18432 per inch)
using Edu = int32_t;                ///< "Enigma Durational Units" value (1024 per quarter note)
using EduFloat = double;            ///< "Enigma Durational Units" floating point value (1024.0 per quarter note)

using MeasCmper = int16_t;          ///< Enigma meas Cmper (may be negative when not applicable)
using InstCmper = int16_t;          ///< Enigma staff (inst) Cmper (may be negative when not applicable)
using SystemCmper = int16_t;        ///< Enigma systems Cmper (may be negative when not applicable)
using ClefIndex = uint16_t;         ///< Index into @ref options::ClefOptions::clefDefs.
using EntryNumber = int32_t;        ///< Entry identifier.
using NoteNumber = uint16_t;        ///< Note identifier.
using LayerIndex = unsigned int;    ///< Layer index (valid values are 0..3)
using BeamNumber = unsigned int;    ///< A number where 1 corresponds to the primary (8th note) beam, 2 the 16th beam, 3 the 32nd beam, etc.
using PercussionNoteType = uint16_t; ///< Percussion note code type. The top 4 bits are an order code for when the same "pure" note type is used
                                    ///< multiple times in one layout. The rest are the "pure" note type. Generally you do not have to
                                    ///< worry about this unless you are looking for a known note type.
                                    ///< @todo update this comment when we have an enum for known note types.

constexpr Cmper MUSX_GLOBALS_CMPER = 65534; ///< The prefs cmper for global variables (used sparingly since Finale 26.2)
constexpr int MAX_LAYERS = 4;       ///< The maximum number of music layers in a Finale document.
constexpr int MAX_ALTERATIONS = 7;  ///< Maximum absolute value for chromatic alterations supported in Finale.
constexpr Cmper SCORE_PARTID = 0;   ///< The part id of the score.
constexpr Cmper BASE_SYSTEM_ID = 0; ///< The base system cmper that gives a list of all available staves and their score order (@ref others::InstrumentUsed cmper)

constexpr double EVPU_PER_INCH = 288.0;         ///< Number of Evpu units per inch
constexpr double EVPU_PER_MM = EVPU_PER_INCH / 25.4; ///< Number of Evpu units per millimeter
constexpr double EVPU_PER_SPACE = 24.0;         ///< Number of Evpu units per space (spatium)
constexpr double EFIX_PER_EVPU = 64.0;          ///< Number of Efix units per Evpu
constexpr double EFIX_PER_SPACE = EVPU_PER_SPACE * EFIX_PER_EVPU; ///< Number of Efix units per space (spatium)

constexpr int MAX_CUE_PERCENTAGE = 90; ///< Arbitrary value that determines the max scaling value for cue notes. This could be configurable in the future.

} // namespace dom
} // namespace musx
