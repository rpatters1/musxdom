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

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::dom;
using namespace musx::util;

namespace {

DocumentPtr createArpeggiosDoc()
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "arpeggios.enigmaxml", xml);
    return musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
}

std::optional<ArpeggioSpanCandidate> calcSpanForEntry(const DocumentPtr& doc, EntryNumber entryNumber,
    const ArpeggioSpanOptions& options = {})
{
    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, entryNumber);
    if (!sourceEntry) {
        return std::nullopt;
    }
    auto assign = doc->getDetails()->get<details::ArticulationAssign>(SCORE_PARTID, entryNumber, 0);
    if (!assign) {
        return std::nullopt;
    }
    return calcArpeggioSpanForAssignment(sourceEntry, assign, options);
}

MusxInstance<others::SmartShape> createVerticalHookSmartShape(
    const DocumentPtr& doc,
    const EntryInfoPtr& sourceEntry,
    const EntryInfoPtr& topEntry,
    const EntryInfoPtr& bottomEntry,
    StaffCmper endStaffOverride = 0)
{
    constexpr Cmper SMART_SHAPE_ID = 9001;
    constexpr Cmper LINE_STYLE_ID = 9001;

    auto customLine = std::make_shared<others::SmartShapeCustomLine>(
        doc, SCORE_PARTID, Base::ShareMode::All, LINE_STYLE_ID);
    customLine->lineStyle = others::SmartShapeCustomLine::LineStyle::Solid;
    customLine->lineCapStartType = others::SmartShapeCustomLine::LineCapType::Hook;
    customLine->lineCapEndType = others::SmartShapeCustomLine::LineCapType::Hook;

    const auto [topStaffTop, topStaffBottom] = topEntry.calcTopBottomStaffPositions();
    const auto [bottomStaffTop, bottomStaffBottom] = bottomEntry.calcTopBottomStaffPositions();
    const Evpu startY = static_cast<Evpu>(std::min(topStaffTop, topStaffBottom) * (EVPU_PER_SPACE / 2));
    Evpu endY = static_cast<Evpu>(std::max(bottomStaffTop, bottomStaffBottom) * (EVPU_PER_SPACE / 2));
    if (startY == endY) {
        endY += 1;
    }

    const Evpu hookLength = (startY > endY) ? -24 : 24;
    customLine->lineCapStartHookLength = hookLength;
    customLine->lineCapEndHookLength = hookLength;

    doc->getOthers()->add(others::SmartShapeCustomLine::XmlNodeName, customLine);

    auto smartShape = std::make_shared<others::SmartShape>(
        doc, SCORE_PARTID, Base::ShareMode::All, SMART_SHAPE_ID);
    smartShape->shapeType = others::SmartShape::ShapeType::CustomLine;
    smartShape->lineStyleId = LINE_STYLE_ID;
    smartShape->integrityCheck(smartShape);

    const auto measure = doc->getOthers()->get<others::Measure>(SCORE_PARTID, sourceEntry.getMeasure());
    MUSX_ASSERT_IF(!measure) {
        return nullptr;
    }
    const auto globalPosition = sourceEntry.calcGlobalElapsedDuration();
    const auto calcStaffEdu = [&](StaffCmper staffId) {
        return (globalPosition / measure->calcTimeStretch(staffId)).calcEduDuration();
    };

    smartShape->startTermSeg->endPoint->staffId = topEntry.getStaff();
    smartShape->startTermSeg->endPoint->measId = sourceEntry.getMeasure();
    smartShape->startTermSeg->endPoint->eduPosition = calcStaffEdu(topEntry.getStaff());
    smartShape->startTermSeg->endPointAdj->active = true;
    smartShape->startTermSeg->endPointAdj->vertOffset = startY;

    smartShape->endTermSeg->endPoint->staffId = endStaffOverride ? endStaffOverride : bottomEntry.getStaff();
    smartShape->endTermSeg->endPoint->measId = sourceEntry.getMeasure();
    smartShape->endTermSeg->endPoint->eduPosition = calcStaffEdu(smartShape->endTermSeg->endPoint->staffId);
    smartShape->endTermSeg->endPointAdj->active = true;
    smartShape->endTermSeg->endPointAdj->vertOffset = endY;

    return smartShape;
}

} // namespace

TEST(ArpeggioUtilTest, FixtureAssignmentsWithoutCrossStaffOrCrossEntrySpanResolveToSelf)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    const std::vector<EntryNumber> entries = { 125, 127, 133, 142, 145 };
    for (EntryNumber entryNumber : entries) {
        auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, entryNumber);
        ASSERT_TRUE(sourceEntry) << "Unable to locate source entry " << entryNumber;

        auto assign = doc->getDetails()->get<details::ArticulationAssign>(SCORE_PARTID, entryNumber, 0);
        ASSERT_TRUE(assign) << "Unable to locate articulation assignment for " << entryNumber;

        auto span = calcArpeggioSpanForAssignment(sourceEntry, assign);
        ASSERT_TRUE(span.has_value()) << "Expected non-null arpeggio span for " << entryNumber;

        EXPECT_EQ(span->type, ArpeggioSpanType::Normal);
        EXPECT_EQ(span->sourceEntry->getEntry()->getEntryNumber(), entryNumber);
        EXPECT_TRUE(span->topEntry);
        EXPECT_TRUE(span->bottomEntry);

        // Core invariant for this helper: only candidates at the same global location are considered.
        EXPECT_EQ(span->topEntry.calcGlobalElapsedDuration(), sourceEntry.calcGlobalElapsedDuration());
        EXPECT_EQ(span->bottomEntry.calcGlobalElapsedDuration(), sourceEntry.calcGlobalElapsedDuration());
        EXPECT_EQ(span->topEntry->getEntry()->getEntryNumber(), entryNumber);
        EXPECT_EQ(span->bottomEntry->getEntry()->getEntryNumber(), entryNumber);
    }
}

TEST(ArpeggioUtilTest, SourceStaffConstraintRestrictsMatches)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    ArpeggioSpanOptions options;
    options.constrainToSourceStaffOnly = true;

    const std::vector<EntryNumber> entries = { 125, 127, 133, 136, 142, 145, 147 };
    for (EntryNumber entryNumber : entries) {
        auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, entryNumber);
        ASSERT_TRUE(sourceEntry) << "Unable to locate source entry " << entryNumber;

        auto span = calcSpanForEntry(doc, entryNumber, options);
        ASSERT_TRUE(span.has_value()) << "Expected non-null constrained span for " << entryNumber;

        EXPECT_EQ(span->topEntry.getStaff(), sourceEntry.getStaff());
        EXPECT_EQ(span->bottomEntry.getStaff(), sourceEntry.getStaff());
    }
}

TEST(ArpeggioUtilTest, SymbolFilterCanRejectAssignment)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 125);
    ASSERT_TRUE(sourceEntry);
    auto assign = doc->getDetails()->get<details::ArticulationAssign>(SCORE_PARTID, 125, 0);
    ASSERT_TRUE(assign);

    const auto rejected = calcArpeggioSpanForAssignment(sourceEntry, assign, {},
        [](const details::ArticulationAssign::SelectedSymbolContext&) {
            return false;
        });
    EXPECT_FALSE(rejected.has_value());
}

TEST(ArpeggioUtilTest, Entry136SpansToEntry142Below)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 136);
    ASSERT_TRUE(sourceEntry);
    auto assign = doc->getDetails()->get<details::ArticulationAssign>(SCORE_PARTID, 136, 0);
    ASSERT_TRUE(assign);

    auto span = calcArpeggioSpanForAssignment(sourceEntry, assign);
    ASSERT_TRUE(span.has_value()) << "Expected non-null arpeggio span for entry 136";

    ASSERT_TRUE(span->bottomEntry);
    EXPECT_EQ(span->bottomEntry->getEntry()->getEntryNumber(), 142);
}

TEST(ArpeggioUtilTest, Entry147SpansToEntry148Below)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 147);
    ASSERT_TRUE(sourceEntry);
    auto assign = doc->getDetails()->get<details::ArticulationAssign>(SCORE_PARTID, 147, 0);
    ASSERT_TRUE(assign);

    auto span = calcArpeggioSpanForAssignment(sourceEntry, assign);
    ASSERT_TRUE(span.has_value()) << "Expected non-null arpeggio span for entry 147";

    ASSERT_TRUE(span->bottomEntry);
    EXPECT_EQ(span->bottomEntry->getEntry()->getEntryNumber(), 148);
}

TEST(ArpeggioUtilTest, SmartShapeVerticalHooksResolveBracketSpan)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 136);
    ASSERT_TRUE(sourceEntry);
    auto smartShape = createVerticalHookSmartShape(doc, sourceEntry, sourceEntry, sourceEntry);
    ASSERT_TRUE(smartShape);
    EXPECT_EQ(recognizeSmartShape(smartShape), KnownSmartShapeType::VerticalLineRightHooks);

    auto span = calcNonArpeggioSpanForSmartShape(sourceEntry, smartShape);
    ASSERT_TRUE(span.has_value()) << "Expected non-null smart shape bracket span";

    EXPECT_EQ(span->type, ArpeggioSpanType::Bracket);
    EXPECT_EQ(span->sourceEntry->getEntry()->getEntryNumber(), 136);
    ASSERT_TRUE(span->topEntry);
    ASSERT_TRUE(span->bottomEntry);
    EXPECT_EQ(span->topEntry->getEntry()->getEntryNumber(), 136);
    EXPECT_EQ(span->bottomEntry->getEntry()->getEntryNumber(), 136);
}

TEST(ArpeggioUtilTest, SmartShapeVerticalHooksRejectStavesOutsideSourceInstrument)
{
    auto doc = createArpeggiosDoc();
    ASSERT_TRUE(doc);

    auto sourceEntry = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 136);
    ASSERT_TRUE(sourceEntry);
    auto expectedBottom = EntryInfoPtr::fromEntryNumber(doc, SCORE_PARTID, 142);
    ASSERT_TRUE(expectedBottom);

    auto smartShape = createVerticalHookSmartShape(doc, sourceEntry, sourceEntry, expectedBottom, 32767);
    ASSERT_TRUE(smartShape);
    EXPECT_EQ(recognizeSmartShape(smartShape), KnownSmartShapeType::VerticalLineRightHooks);

    EXPECT_FALSE(calcNonArpeggioSpanForSmartShape(sourceEntry, smartShape).has_value());
}
