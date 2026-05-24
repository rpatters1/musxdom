/*
 * Copyright (C) 2026, Robert Patterson
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

#include <cstddef>
#include <map>
#include <unordered_map>
#include <vector>

#include "CommonClasses.h"

namespace musx {
namespace dom {

namespace others {
class StaffComposite;
}

/**
 * @class InstrumentInfo
 * @brief Derived description of a logical instrument in a document.
 *
 * This is not a Finale data class and is not populated directly from EnigmaXML. It is a normalized,
 * interchange-oriented view calculated from Finale staves, staff groups, and multistaff instrument groups.
 */
class InstrumentMap;

class InstrumentInfo : public DocumentElement
{
public:
    /// @brief Stable identity for a distinct instrument used by this logical instrument.
    struct InstrumentIdentity {
        std::string instUuid; ///< The effective instrument UUID.

        /// @brief Equality comparison operator.
        bool operator==(const InstrumentIdentity& other) const
        { return instUuid == other.instUuid; }

        /// @brief Ordering operator for use in ordered containers.
        bool operator<(const InstrumentIdentity& other) const
        { return instUuid < other.instUuid; }
    };

    /// @brief Effective instrument state beginning at a musical location.
    struct InstrumentChange {
        InstrumentIdentity identity;                               ///< Stable identity for the effective instrument.
        MusxInstance<others::StaffComposite> topStaffComposite;    ///< Effective state of the top staff.
    };

    /// @brief Effective top-staff states keyed by the musical location where each state begins.
    using InstrumentChangeEvents = std::map<MusicPoint, InstrumentChange>;

    /// @brief Constructs an empty instrument info for a score or linked part.
    /// @param document The document this instrument belongs to.
    /// @param partId The score or linked part ID represented by this instrument.
    explicit InstrumentInfo(const DocumentWeakPtr& document, Cmper partId = SCORE_PARTID)
        : DocumentElement(document, partId) {}

    std::unordered_map<StaffCmper, size_t> staves;  ///< List of each staffId with its sequence index from top to bottom.
    Cmper staffGroupId{};                           ///< The @ref details::StaffGroup that visually represents the instrument. (May be zero.)
    Cmper multistaffGroupId{};                      ///< The @ref others::MultiStaffInstrumentGroup that defines the instrument. (May be zero.)

    /// @brief Returns the staffIds in sequence as they appear in Scroll View in the score.
    std::vector<StaffCmper> getSequentialStaves() const;

    /// @brief Returns effective instrument states for this logical instrument.
    ///
    /// Instrument changes are recognized only when every staff in the logical instrument has an aligned
    /// @ref others::StaffStyleAssign whose @ref others::StaffStyle::containsInstrumentChange returns true.
    /// Reversion to the base staff instrument is represented by an additional change at the first location
    /// after the staff style assignment. If no changes are found, the result contains one change at the start
    /// of the document.
    InstrumentChangeEvents getChanges() const;

    /// @brief Returns the unique instrument identities in order of first appearance.
    std::vector<InstrumentIdentity> getInstrumentIdentities() const;

    /// @brief Returns the instrument identity in effect at the specified music point.
    /// @throws std::logic_error if no identity is in effect at @p point.
    InstrumentIdentity getInstrumentIdentityAt(MusicPoint point) const;
};

/// @brief A list of instruments, which may be single- or multi-staff.
///
/// This is not a Finale data class. It is a normalized, interchange-oriented view of the logical
/// instruments available in a score or linked part.
class InstrumentMap : public DocumentElement, public std::unordered_map<StaffCmper, InstrumentInfo>
{
public:
    using Base = std::unordered_map<StaffCmper, InstrumentInfo>;
    using Base::Base;

    /// @brief Constructs an empty instrument map for a score or linked part.
    /// @param document The document this map describes.
    /// @param partId The score or linked part ID represented by this map.
    explicit InstrumentMap(const DocumentWeakPtr& document, Cmper partId = SCORE_PARTID)
        : DocumentElement(document, partId) {}

    /// @brief Get the instrument info for the given staffId in the given map
    /// @param staffId The staffId to find.
    /// @return The InstrumentInfo for the @p staffId or null if not found.
    const InstrumentInfo* getInstrumentForStaff(StaffCmper staffId) const;
};

} // namespace dom
} // namespace musx
