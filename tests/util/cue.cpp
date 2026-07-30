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

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::dom;
using musx::util::Cue;

namespace {

DocumentPtr loadCueVisibilityDocument(PartVoicingPolicy partVoicingPolicy = PartVoicingPolicy::Apply)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "cue-visibility.enigmaxml", xml);
    return musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml, partVoicingPolicy);
}

EntryInfoPtr getFirstEntry(const details::GFrameHoldContext& context, LayerIndex layerIndex)
{
    auto frame = context.createEntryFrame(layerIndex);
    EXPECT_TRUE(frame);
    if (!frame || frame->getEntries().empty()) {
        return {};
    }
    return EntryInfoPtr(frame, 0);
}

} // namespace

TEST(Cues, TestIndependentTime)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "indtime-cuebug.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 1);
    ASSERT_TRUE(gfhold) << " gfhold not found for 2, 1";
    auto entryFrame = gfhold.createEntryFrame(3);
    ASSERT_TRUE(entryFrame);

    const auto frameAnalysis = Cue::calcFrameAnalysis(entryFrame);
    EXPECT_TRUE(frameAnalysis.isCueFrame);
    EXPECT_TRUE(frameAnalysis.hasVisibleCueEntries);
    EXPECT_EQ(frameAnalysis.cueEntryIndices, (std::vector<size_t>{0, 1, 2}));

    const auto entryAnalysis = Cue::calcEntryAnalysis(EntryInfoPtr(entryFrame, 0));
    EXPECT_EQ(entryAnalysis.kind, Cue::EntryAnalysis::Kind::PartOnly);
    EXPECT_FALSE(entryAnalysis.visibleInScore);
    EXPECT_FALSE(entryAnalysis.visibleInRequestedContext);
    EXPECT_TRUE(entryAnalysis.visibleInAnyLinkedPart);

    const auto staffMeasureAnalysis = Cue::calcStaffMeasureAnalysis(gfhold);
    EXPECT_TRUE(staffMeasureAnalysis.isCueOnly);
    EXPECT_TRUE(gfhold.calcIsCuesOnly());
    EXPECT_EQ(staffMeasureAnalysis.cueLayers, (std::vector<LayerIndex>{3}));
    EXPECT_EQ(staffMeasureAnalysis.visibleCueLayers, (std::vector<LayerIndex>{3}));
}

TEST(Cues, VisibilityAcrossScoreAndLinkedParts)
{
    const auto doc = loadCueVisibilityDocument();
    ASSERT_TRUE(doc);

    const details::GFrameHoldContext scoreContext(doc, SCORE_PARTID, 1, 1);
    const details::GFrameHoldContext visiblePartContext(doc, 1, 1, 1);
    const details::GFrameHoldContext hiddenPartContext(doc, 2, 1, 1);
    const details::GFrameHoldContext voicedPartContext(doc, 3, 1, 1);

    const auto scoreEntry = getFirstEntry(scoreContext, 0);
    const auto visiblePartEntry = getFirstEntry(visiblePartContext, 0);
    const auto hiddenPartEntry = getFirstEntry(hiddenPartContext, 0);
    ASSERT_TRUE(scoreEntry);
    ASSERT_TRUE(visiblePartEntry);
    ASSERT_TRUE(hiddenPartEntry);

    for (const auto& entry : { scoreEntry, visiblePartEntry, hiddenPartEntry }) {
        const auto entryAnalysis = Cue::calcEntryAnalysis(entry);
        EXPECT_EQ(entryAnalysis.entrySizePercent, 75);
        EXPECT_EQ(entryAnalysis.kind, Cue::EntryAnalysis::Kind::PartOnly);
        EXPECT_FALSE(entryAnalysis.visibleInScore);
        EXPECT_TRUE(entryAnalysis.visibleInAnyLinkedPart);
    }

    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(scoreEntry), Cue::EntryVisibility::HiddenByAlternateNotation);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(visiblePartEntry), Cue::EntryVisibility::Visible);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(hiddenPartEntry), Cue::EntryVisibility::HiddenByAlternateNotation);
    EXPECT_EQ(Cue::calcVisibility(scoreEntry, voicedPartContext.getRequestedPartId()), Cue::EntryVisibility::ExcludedByVoicing);

    EXPECT_FALSE(Cue::calcIsVisibleInRequestedContext(scoreEntry));
    EXPECT_TRUE(Cue::calcIsVisibleInRequestedContext(visiblePartEntry));
    EXPECT_FALSE(Cue::calcIsVisibleInRequestedContext(hiddenPartEntry));

    const auto ignoreVoicingDoc = loadCueVisibilityDocument(PartVoicingPolicy::Ignore);
    ASSERT_TRUE(ignoreVoicingDoc);
    const details::GFrameHoldContext ignoreVoicingContext(ignoreVoicingDoc, 3, 1, 1);
    const auto ignoreVoicingEntry = getFirstEntry(ignoreVoicingContext, 0);
    ASSERT_TRUE(ignoreVoicingEntry);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(ignoreVoicingEntry), Cue::EntryVisibility::Visible);
    const auto ignoreVoicingAnalysis = Cue::calcStaffMeasureAnalysis(ignoreVoicingContext);
    EXPECT_EQ(ignoreVoicingAnalysis.cueLayers, (std::vector<LayerIndex>{0, 1}));
    EXPECT_EQ(ignoreVoicingAnalysis.visibleCueLayers, (std::vector<LayerIndex>{0, 1}));
}

TEST(Cues, ScoreVisibleEntryAndStaffMeasureAnalyses)
{
    const auto doc = loadCueVisibilityDocument();
    ASSERT_TRUE(doc);

    const details::GFrameHoldContext scoreContext(doc, SCORE_PARTID, 1, 1);
    const details::GFrameHoldContext visiblePartContext(doc, 1, 1, 1);
    const details::GFrameHoldContext hiddenPartContext(doc, 2, 1, 1);
    const details::GFrameHoldContext voicedPartContext(doc, 3, 1, 1);

    const auto scoreVisibleEntry = getFirstEntry(scoreContext, 1);
    ASSERT_TRUE(scoreVisibleEntry);
    const auto entryAnalysis = Cue::calcEntryAnalysis(scoreVisibleEntry);
    EXPECT_EQ(entryAnalysis.entrySizePercent, 80);
    EXPECT_EQ(entryAnalysis.kind, Cue::EntryAnalysis::Kind::ScoreVisible);
    EXPECT_TRUE(entryAnalysis.visibleInScore);
    EXPECT_TRUE(entryAnalysis.visibleInAnyLinkedPart);
    EXPECT_TRUE(entryAnalysis.visibleInRequestedContext);

    const auto scoreFrame0 = Cue::calcFrameAnalysis(scoreContext.createEntryFrame(0));
    EXPECT_TRUE(scoreFrame0.isCueFrame);
    EXPECT_FALSE(scoreFrame0.hasVisibleCueEntries);
    EXPECT_EQ(scoreFrame0.cueEntryIndices, (std::vector<size_t>{0}));

    const auto partFrame0 = Cue::calcFrameAnalysis(visiblePartContext.createEntryFrame(0));
    EXPECT_TRUE(partFrame0.isCueFrame);
    EXPECT_TRUE(partFrame0.hasVisibleCueEntries);

    const auto scoreAnalysis = Cue::calcStaffMeasureAnalysis(scoreContext);
    EXPECT_TRUE(scoreAnalysis.isCueOnly);
    EXPECT_EQ(scoreAnalysis.cueLayers, (std::vector<LayerIndex>{0, 1}));
    EXPECT_EQ(scoreAnalysis.visibleCueLayers, (std::vector<LayerIndex>{1}));

    const auto visiblePartAnalysis = Cue::calcStaffMeasureAnalysis(visiblePartContext);
    EXPECT_TRUE(visiblePartAnalysis.isCueOnly);
    EXPECT_EQ(visiblePartAnalysis.cueLayers, (std::vector<LayerIndex>{0, 1}));
    EXPECT_EQ(visiblePartAnalysis.visibleCueLayers, (std::vector<LayerIndex>{0, 1}));

    const auto hiddenPartAnalysis = Cue::calcStaffMeasureAnalysis(hiddenPartContext);
    EXPECT_TRUE(hiddenPartAnalysis.isCueOnly);
    EXPECT_EQ(hiddenPartAnalysis.cueLayers, (std::vector<LayerIndex>{0, 1}));
    EXPECT_EQ(hiddenPartAnalysis.visibleCueLayers, (std::vector<LayerIndex>{1}));

    const auto voicedPartAnalysis = Cue::calcStaffMeasureAnalysis(voicedPartContext);
    EXPECT_TRUE(voicedPartAnalysis.isCueOnly);
    EXPECT_EQ(voicedPartAnalysis.cueLayers, (std::vector<LayerIndex>{1}));
    EXPECT_EQ(voicedPartAnalysis.visibleCueLayers, (std::vector<LayerIndex>{1}));
}

TEST(Cues, ClassificationBoundariesAndHiddenEverywhere)
{
    const auto doc = loadCueVisibilityDocument();
    ASSERT_TRUE(doc);

    const auto hiddenEverywhere = getFirstEntry(details::GFrameHoldContext(doc, SCORE_PARTID, 2, 2), 0);
    ASSERT_TRUE(hiddenEverywhere);
    const auto hiddenAnalysis = Cue::calcEntryAnalysis(hiddenEverywhere);
    EXPECT_EQ(hiddenAnalysis.entrySizePercent, 70);
    EXPECT_EQ(hiddenAnalysis.kind, Cue::EntryAnalysis::Kind::None);
    EXPECT_FALSE(hiddenAnalysis.visibleInScore);
    EXPECT_FALSE(hiddenAnalysis.visibleInAnyLinkedPart);
    EXPECT_FALSE(hiddenAnalysis.visibleInRequestedContext);

    const auto tooLarge = getFirstEntry(details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3), 0);
    ASSERT_TRUE(tooLarge);
    const auto largeAnalysis = Cue::calcEntryAnalysis(tooLarge);
    EXPECT_EQ(largeAnalysis.entrySizePercent, 95);
    EXPECT_EQ(largeAnalysis.kind, Cue::EntryAnalysis::Kind::None);
    EXPECT_FALSE(largeAnalysis.visibleInScore);
    EXPECT_TRUE(largeAnalysis.visibleInAnyLinkedPart);
}

TEST(Cues, MixedAndExplicitlyHiddenEntries)
{
    const auto doc = loadCueVisibilityDocument();
    ASSERT_TRUE(doc);

    const details::GFrameHoldContext mixedContext(doc, SCORE_PARTID, 1, 4);
    const auto mixedFrame = mixedContext.createEntryFrame(0);
    ASSERT_TRUE(mixedFrame);
    const auto mixedInfo = Cue::calcFrameAnalysis(mixedFrame);
    EXPECT_FALSE(mixedInfo.isCueFrame);
    EXPECT_FALSE(mixedInfo.hasVisibleCueEntries);
    EXPECT_EQ(mixedInfo.cueEntryIndices, (std::vector<size_t>{0}));
    EXPECT_EQ(Cue::calcEntryAnalysis(EntryInfoPtr(mixedFrame, 0)).kind, Cue::EntryAnalysis::Kind::PartOnly);
    EXPECT_EQ(Cue::calcEntryAnalysis(EntryInfoPtr(mixedFrame, 1)).kind, Cue::EntryAnalysis::Kind::None);

    const details::GFrameHoldContext hiddenContext(doc, SCORE_PARTID, 1, 5);
    const auto hiddenFrame = hiddenContext.createEntryFrame(0);
    ASSERT_TRUE(hiddenFrame);
    ASSERT_EQ(hiddenFrame->getEntries().size(), 2);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(EntryInfoPtr(hiddenFrame, 0)), Cue::EntryVisibility::HiddenEntry);
    EXPECT_EQ(Cue::calcEntryAnalysis(EntryInfoPtr(hiddenFrame, 0)).kind, Cue::EntryAnalysis::Kind::None);
    const auto hiddenFrameAnalysis = Cue::calcFrameAnalysis(hiddenFrame);
    EXPECT_TRUE(hiddenFrameAnalysis.isCueFrame);
    EXPECT_EQ(hiddenFrameAnalysis.cueEntryIndices, (std::vector<size_t>{1}));
}

TEST(Cues, HideModeAndAlternateOtherLayer)
{
    const auto doc = loadCueVisibilityDocument();
    ASSERT_TRUE(doc);

    const auto scoreHiddenByMode = getFirstEntry(details::GFrameHoldContext(doc, SCORE_PARTID, 3, 6), 0);
    const auto partVisibleByMode = getFirstEntry(details::GFrameHoldContext(doc, 1, 3, 6), 0);
    const auto partHiddenByMode = getFirstEntry(details::GFrameHoldContext(doc, 2, 3, 6), 0);
    ASSERT_TRUE(scoreHiddenByMode);
    ASSERT_TRUE(partVisibleByMode);
    ASSERT_TRUE(partHiddenByMode);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(scoreHiddenByMode), Cue::EntryVisibility::HiddenStaff);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(partVisibleByMode), Cue::EntryVisibility::Visible);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(partHiddenByMode), Cue::EntryVisibility::HiddenStaff);
    EXPECT_EQ(Cue::calcEntryAnalysis(scoreHiddenByMode).kind, Cue::EntryAnalysis::Kind::PartOnly);

    const auto otherLayer = getFirstEntry(details::GFrameHoldContext(doc, SCORE_PARTID, 4, 7), 1);
    ASSERT_TRUE(otherLayer);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(otherLayer), Cue::EntryVisibility::HiddenByAlternateNotation);
    EXPECT_EQ(Cue::calcEntryAnalysis(otherLayer).kind, Cue::EntryAnalysis::Kind::PartOnly);

    const auto absentPartEntry = getFirstEntry(details::GFrameHoldContext(doc, 2, 4, 7), 1);
    ASSERT_TRUE(absentPartEntry);
    EXPECT_EQ(Cue::calcVisibilityInRequestedContext(absentPartEntry), Cue::EntryVisibility::HiddenStaff);
}
