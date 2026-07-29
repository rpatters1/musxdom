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

#include "Cue.h"

#include "musx/dom/Details.h"
#include "musx/dom/Document.h"
#include "musx/dom/Others.h"
#include "musx/dom/Staff.h"

namespace musx::util {

namespace {

bool calcPolicyVoicingIncludesLayer(const dom::EntryInfoPtr& entry, dom::Cmper partId)
{
    const auto frame = entry.getFrame();
    const auto document = frame->getDocument();
    if (document->getPartVoicingPolicy() != dom::PartVoicingPolicy::Apply) {
        return true;
    }
    if (const auto partVoicing = document->getOthers()->get<dom::others::PartVoicing>(partId, frame->getStaff())) {
        return partVoicing->calcShowsLayer(frame->getLayerIndex(), frame->getContext()->calcIsMultiLayer());
    }
    return true;
}

} // namespace

Cue::EntryVisibility Cue::calcVisibility(const dom::EntryInfoPtr& entry, dom::Cmper targetPartId)
{
    if (entry->getEntry()->isHidden) {
        return EntryVisibility::HiddenEntry;
    }

    const auto frame = entry.getFrame();
    const auto document = frame->getDocument();
    const auto contextStaves = document->getScrollViewStaves(targetPartId);
    if (!contextStaves.getIndexForStaff(entry.getStaff())) {
        return EntryVisibility::HiddenStaff;
    }

    const auto staff = dom::others::StaffComposite::createCurrent(document, targetPartId, entry.getStaff(), entry.getMeasure(),
        entry->elapsedDuration.calcEduDuration());
    if (!staff) {
        return EntryVisibility::HiddenStaff;
    }
    const bool staffIsHidden = staff->hideMode == dom::others::Staff::HideMode::ScoreParts
        || staff->hideMode == dom::others::Staff::HideMode::Cutaway
        || (staff->hideMode == dom::others::Staff::HideMode::Score && targetPartId == dom::SCORE_PARTID);
    if (staffIsHidden) {
        return EntryVisibility::HiddenStaff;
    }
    if (staff->calcAlternateNotationHidesEntries(entry.getLayerIndex())) {
        return EntryVisibility::HiddenByAlternateNotation;
    }
    if (!calcPolicyVoicingIncludesLayer(entry, targetPartId)) {
        return EntryVisibility::ExcludedByVoicing;
    }
    return EntryVisibility::Visible;
}

Cue::EntryVisibility Cue::calcVisibilityInRequestedContext(const dom::EntryInfoPtr& entry)
{
    return calcVisibility(entry, entry.getFrame()->getRequestedPartId());
}

Cue::EntryAnalysis Cue::calcEntryAnalysis(const dom::EntryInfoPtr& entry)
{
    EntryAnalysis result;
    result.entrySizePercent = entry.calcEntrySize();
    result.visibleInRequestedContext = calcVisibilityInRequestedContext(entry) == EntryVisibility::Visible;
    result.visibleInScore = calcVisibility(entry, dom::SCORE_PARTID) == EntryVisibility::Visible;

    const auto document = entry.getFrame()->getDocument();
    if (const auto sourceStaff = document->getOthers()->get<dom::others::Staff>(dom::SCORE_PARTID, entry.getStaff())) {
        for (const auto& part : sourceStaff->getContainingParts(/*includeScore*/ false)) {
            if (calcVisibility(entry, part->getCmper()) == EntryVisibility::Visible) {
                result.visibleInAnyLinkedPart = true;
                break;
            }
        }
    } else {
        MUSX_INTEGRITY_ERROR("Staff " + std::to_string(entry.getStaff()) + " not found.");
    }

    if (result.entrySizePercent > dom::MAX_CUE_PERCENTAGE) {
        result.kind = EntryAnalysis::Kind::None;
    } else if (result.visibleInScore) {
        result.kind = EntryAnalysis::Kind::ScoreVisible;
    } else if (result.visibleInAnyLinkedPart) {
        result.kind = EntryAnalysis::Kind::PartOnly;
    }
    return result;
}

Cue::FrameAnalysis Cue::calcFrameAnalysis(const std::shared_ptr<const dom::EntryFrame>& frame)
{
    FrameAnalysis result;
    bool allApplicableEntriesAreCues = true;
    bool foundCueEntry = false;
    for (size_t x = 0; x < frame->getEntries().size(); x++) {
        if (!frame->getEntries()[x]->getEntry()->isHidden) {
            const dom::EntryInfoPtr entry(frame, x);
            const auto cueAnalysis = calcEntryAnalysis(entry);
            if (cueAnalysis.isCue()) {
                foundCueEntry = true;
                result.cueEntryIndices.push_back(x);
                result.hasVisibleCueEntries |= cueAnalysis.visibleInRequestedContext;
            } else {
                allApplicableEntriesAreCues = false;
            }
        }
    }
    result.isCueFrame = foundCueEntry && allApplicableEntriesAreCues;
    return result;
}

Cue::StaffMeasureAnalysis Cue::calcStaffMeasureAnalysis(const dom::details::GFrameHoldContext& context)
{
    StaffMeasureAnalysis result;
    bool foundNonCue = false;
    for (dom::LayerIndex layerIndex = 0; layerIndex < context->frames.size(); layerIndex++) {
        if (!context.calcPolicyVoicingIncludesLayer(layerIndex)) {
            continue;
        }
        auto [frame, startEdu] = context->findLayerFrame(layerIndex);
        if (!frame) {
            continue;
        }
        const auto entries = frame->getEntries();
        if (startEdu == 0 && entries.size() == 1 && entries[0]->isPossibleFullMeasureRest()) {
            continue;
        }
        if (const auto entryFrame = context.createEntryFrame(layerIndex)) {
            const auto frameAnalysis = calcFrameAnalysis(entryFrame);
            if (frameAnalysis.isCueFrame) {
                result.cueLayers.push_back(layerIndex);
                if (frameAnalysis.hasVisibleCueEntries) {
                    result.visibleCueLayers.push_back(layerIndex);
                }
            } else {
                foundNonCue = true;
            }
        }
    }
    result.isCueOnly = !foundNonCue && !result.cueLayers.empty();
    return result;
}

} // namespace musx::util
