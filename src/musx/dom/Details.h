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
#include <functional>

#include "BaseClasses.h"

 // do not add other dom class dependencies. Use Implementations.h for implementations that need total class access.

namespace musx {
namespace dom {
namespace details {

/**
 * @class GFrameHold
 * @brief Represents the attributes of a Finale frame holder.
 *
 * Cmper1 is the staff (inst) @ref Cmper and Cmper2 is the measur @ref Cmper
 * This class is identified by the XML node name "gfhold".
 */
class GFrameHold : public DetailsBase
{
public:
    /**
     * @brief Constructor function
     * @param document A weak pointer to the associated document.
     * @param partId The part that this is for (probably always 0).
     * @param shareMode The sharing mode for this GFrameHold (probably always #ShareMode::All)
     * @param inst The staff ID for this GFrameHold.
     * @param meas The measure ID for this GFrameHold.
     */
    explicit GFrameHold(const DocumentWeakPtr& document, Cmper partId, ShareMode shareMode, Cmper inst, Cmper meas)
        : DetailsBase(document, partId, shareMode, inst, meas) {}

    /**
     * @brief Enum representing the clef mode for the frame.
     */
    enum class ShowClefMode
    {
        WhenNeeded, ///< Clef is displayed only when needed (the default).
        Never,      ///< Clef is never displayed. (xml value is "hidden")
        Always      ///< Clef is always displayed. (xml value is "forced")
    };

    // Public properties corresponding to the XML structure
    std::optional<ClefIndex> clefId;        ///< clef index when there are no mid-measure clef changes. (xml tag is `<clefID>`).
    Cmper clefListId{};                     ///< The clef list ID when there are mid-measure clef changes, if non-zero. (xml tag is `<clefListID>`).
    ShowClefMode showClefMode{};            ///< "Show Clef" mode. (xml tag is `<clefMode>`)
    bool mirrorFrame{};                     ///< Indicates this is a mirror frame. (Not used after Finale 14.5.)
    int clefPercent{};                      ///< Clef percent where 100 means 100%.
    Cmper frame1{};                         ///< @ref others::Frame for layer 1 @ref Cmper (if non-zero)
    Cmper frame2{};                         ///< @ref others::Frame for layer 2 @ref Cmper (if non-zero)
    Cmper frame3{};                         ///< @ref others::Frame for layer 3 @ref Cmper (if non-zero)
    Cmper frame4{};                         ///< @ref others::Frame for layer 4 @ref Cmper (if non-zero)

    /// @brief returns the inst (staff) number for this GFrameHold
    InstCmper getStaff() const { return InstCmper(getCmper1()); }

    /// @brief returns the measure number for this GFrameHold
    MeasCmper getMeasure() const { return MeasCmper(getCmper2()); }

    /**
     * @brief iterates the entries for this gfhold from left to right for each existing layer in order
     */
    void iterateEntries(std::function<bool(const std::shared_ptr<const Entry>&)> iterator);

    void integrityCheck() const override
    {
        this->DetailsBase::integrityCheck();
        if (clefListId && clefId.has_value()) {
            MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getCmper1()) + " and measure " + std::to_string(getCmper2()) + " has both clef and clef list.");
        }
        if (!clefListId && !clefId.has_value()) {
            MUSX_INTEGRITY_ERROR("GFrameHold for staff " + std::to_string(getCmper1()) + " and measure " + std::to_string(getCmper2()) + " has neither clef nor clef list.");
        }
    }

    constexpr static std::string_view XmlNodeName = "gfhold"; ///< The XML node name for this type.
    static const xml::XmlElementArray<GFrameHold> XmlMappingArray; ///< Required for musx::factory::FieldPopulator.
};

} // namespace details
} // namespace dom
} // namespace entries