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
#include "musx/util/Tie.h"
#include "test_utils.h"

using namespace musxtest;
using namespace musx::dom;
using musx::util::Tie;

namespace {

struct ExpectedContour
{
    options::TieOptions::ControlStyleType styleType{};
    Evpu length{};
};

struct ExpectedControlPoints
{
    options::TieOptions::InsetStyle insetStyle{};
    Efix leftInsetRatio{};
    Evpu leftHeight{};
    Efix rightInsetRatio{};
    Evpu rightHeight{};
};

struct ContourCase
{
    int measure{};
    bool forPageView{};
    Tie::ContourGeometry geometry{};
    std::optional<ExpectedContour> expected; // TODO: fill per case
};

static void runContourCase(const NoteInfoPtr& noteInfo, const ContourCase& testCase)
{
    if (!testCase.expected.has_value()) {
        GTEST_SKIP() << "Expected contour values not set for measure " << testCase.measure
                     << (testCase.forPageView ? " (page view)" : " (scroll view)");
    }

    auto result = Tie::calcContourStyleType(noteInfo, testCase.geometry, /*forTieEnd=*/false);
    ASSERT_TRUE(result) << "No contour result for measure " << testCase.measure
                        << (testCase.forPageView ? " (page view)" : " (scroll view)");
    EXPECT_EQ(result->styleType, testCase.expected->styleType);
    EXPECT_EQ(result->length, testCase.expected->length);
}

static void runControlPointCase(const NoteInfoPtr& noteInfo, int measure, bool forPageView,
                                options::TieOptions::ControlStyleType styleType, Evpu length,
                                const ExpectedControlPoints& expected)
{
    auto result = Tie::calcDefaultContourControlPoints(noteInfo, styleType, length);
    ASSERT_TRUE(result) << "No contour control points for measure " << measure
                        << (forPageView ? " (page view)" : " (scroll view)");
    EXPECT_EQ(result->insetStyle, expected.insetStyle);
    EXPECT_EQ(result->left.insetRatio, expected.leftInsetRatio);
    EXPECT_EQ(result->left.height, expected.leftHeight);
    EXPECT_EQ(result->right.insetRatio, expected.rightInsetRatio);
    EXPECT_EQ(result->right.height, expected.rightHeight);
}

} // namespace

TEST(TieContour, ContourTypes)
{
    std::vector<char> xml;
    // tie_contour_types.musx is the source file, but we load the extracted EnigmaXML here.
    musxtest::readFile(musxtest::getInputPath() / "tie_contour_types.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    const Cmper staff = 1;
    const size_t entryIndex = 0;
    const size_t noteIndex = 0;

    const std::vector<Cmper> measures{1, 3, 5};

    for (Cmper measure : measures) {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, staff, measure);
        ASSERT_TRUE(gfhold) << "gfhold not found for staff " << staff << ", measure " << measure;
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        NoteInfoPtr noteInfo = createNoteInfo(entryFrame, entryIndex, noteIndex);
        ASSERT_TRUE(noteInfo) << "note info not found for staff " << staff
                              << ", measure " << measure
                              << ", entry index " << entryIndex
                              << ", note index " << noteIndex;
        ASSERT_TRUE(noteInfo->tieStart) << "note is not a tie start for measure " << measure;

        ContourCase scrollCase{
            measure,
            false,
            Tie::ContourGeometry{
                /*startToEndLeft=*/0,
                /*startNoteheadWidth=*/0,
                /*endNoteheadWidth=*/std::nullopt,
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            },
            std::nullopt
        };

        ContourCase pageCase{
            measure,
            true,
            Tie::ContourGeometry{
                /*startToEndLeft=*/0,
                /*startNoteheadWidth=*/0,
                /*endNoteheadWidth=*/std::nullopt,
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            },
            std::nullopt
        };

        if (measure == 1) {
            scrollCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/627.714907,
                /*startNoteheadWidth=*/39.803325,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{30.437837},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            scrollCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::MediumSpan,
                611
            };

            pageCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/1612.034652,
                /*startNoteheadWidth=*/39.803325,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{30.437837},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            pageCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::LongSpan,
                1596
            };
        }
        if (measure == 3) {
            scrollCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/125.906677,
                /*startNoteheadWidth=*/40.974011,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{30.437837},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            scrollCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::ShortSpan,
                109
            };

            pageCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/359.400609,
                /*startNoteheadWidth=*/40.974011,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{30.437837},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            pageCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::MediumSpan,
                343
            };
        }
        if (measure == 5) {
            scrollCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/900.771174,
                /*startNoteheadWidth=*/40.974011,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{31.608523},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            scrollCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::LongSpan,
                885
            };

            pageCase.geometry = Tie::ContourGeometry{
                /*startToEndLeft=*/1665.886209,
                /*startNoteheadWidth=*/40.974011,
                /*endNoteheadWidth=*/std::optional<EvpuFloat>{31.608523},
                /*startAdjustment=*/0,
                /*endAdjustment=*/0,
                /*startPointKind=*/Tie::EndPointKind::Notehead,
                /*endPointKind=*/Tie::EndPointKind::Notehead,
            };
            pageCase.expected = ExpectedContour{
                options::TieOptions::ControlStyleType::LongSpan,
                1650
            };
        }

        runContourCase(noteInfo, scrollCase);
        runContourCase(noteInfo, pageCase);

        const auto scrollDefault = Tie::calcDefaultContourControlPoints(
            noteInfo, scrollCase.expected->styleType, scrollCase.expected->length);
        ASSERT_TRUE(scrollDefault) << "No default contour control points for measure " << measure
                                   << " (scroll view)";
        const auto scrollEffective = Tie::calcEffectiveContourMetrics(
            noteInfo, scrollCase.geometry, /*forTieEnd=*/false);
        ASSERT_TRUE(scrollEffective) << "No effective contour metrics for measure " << measure
                                     << " (scroll view)";
        EXPECT_EQ(scrollEffective->contour.styleType, scrollCase.expected->styleType);
        EXPECT_EQ(scrollEffective->contour.length, scrollCase.expected->length);
        EXPECT_EQ(scrollEffective->controlPoints.insetStyle, scrollDefault->insetStyle);
        EXPECT_EQ(scrollEffective->controlPoints.left.insetRatio, scrollDefault->left.insetRatio);
        EXPECT_EQ(scrollEffective->controlPoints.left.height, scrollDefault->left.height);
        EXPECT_EQ(scrollEffective->controlPoints.right.insetRatio, scrollDefault->right.insetRatio);
        EXPECT_EQ(scrollEffective->controlPoints.right.height, scrollDefault->right.height);

        const auto pageDefault = Tie::calcDefaultContourControlPoints(
            noteInfo, pageCase.expected->styleType, pageCase.expected->length);
        ASSERT_TRUE(pageDefault) << "No default contour control points for measure " << measure
                                 << " (page view)";
        const auto pageEffective = Tie::calcEffectiveContourMetrics(
            noteInfo, pageCase.geometry, /*forTieEnd=*/false);
        ASSERT_TRUE(pageEffective) << "No effective contour metrics for measure " << measure
                                   << " (page view)";
        EXPECT_EQ(pageEffective->contour.styleType, pageCase.expected->styleType);
        EXPECT_EQ(pageEffective->contour.length, pageCase.expected->length);
        EXPECT_EQ(pageEffective->controlPoints.insetStyle, pageDefault->insetStyle);
        EXPECT_EQ(pageEffective->controlPoints.left.insetRatio, pageDefault->left.insetRatio);
        EXPECT_EQ(pageEffective->controlPoints.left.height, pageDefault->left.height);
        EXPECT_EQ(pageEffective->controlPoints.right.insetRatio, pageDefault->right.insetRatio);
        EXPECT_EQ(pageEffective->controlPoints.right.height, pageDefault->right.height);
    }
}

TEST(TieContour, InterpolatedControlPoints)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "tie_contour_interpolated.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    const Cmper staff = 1;
    const size_t entryIndex = 0;
    const size_t noteIndex = 0;

    struct InterpCase
    {
        Cmper measure{};
        options::TieOptions::ControlStyleType scrollStyle{};
        Evpu scrollLength{};
        ExpectedControlPoints scrollExpected{};
        options::TieOptions::ControlStyleType pageStyle{};
        Evpu pageLength{};
        ExpectedControlPoints pageExpected{};
    };

    const std::vector<InterpCase> cases{
        {
            1,
            options::TieOptions::ControlStyleType::MediumSpan,
            611,
            {options::TieOptions::InsetStyle::Percent, 710, 50, 915, 18},
            options::TieOptions::ControlStyleType::MediumSpan,
            1596,
            {options::TieOptions::InsetStyle::Percent, 1634, 96, 1839, 96},
        },
        {
            3,
            options::TieOptions::ControlStyleType::ShortSpan,
            109,
            {options::TieOptions::InsetStyle::Percent, 205, 2, 410, 72},
            options::TieOptions::ControlStyleType::MediumSpan,
            343,
            {options::TieOptions::InsetStyle::Percent, 381, 19, 586, 53},
        },
        {
            5,
            options::TieOptions::ControlStyleType::MediumSpan,
            885,
            {options::TieOptions::InsetStyle::Percent, 987, 67, 1192, 25},
            options::TieOptions::ControlStyleType::LongSpan,
            1650,
            {options::TieOptions::InsetStyle::Percent, 1638, 96, 1843, 96},
        },
    };

    for (const auto& testCase : cases) {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, staff, testCase.measure);
        ASSERT_TRUE(gfhold) << "gfhold not found for staff " << staff << ", measure " << testCase.measure;
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);

        NoteInfoPtr noteInfo = createNoteInfo(entryFrame, entryIndex, noteIndex);
        ASSERT_TRUE(noteInfo) << "note info not found for staff " << staff
                              << ", measure " << testCase.measure
                              << ", entry index " << entryIndex
                              << ", note index " << noteIndex;
        ASSERT_TRUE(noteInfo->tieStart) << "note is not a tie start for measure " << testCase.measure;

        runControlPointCase(noteInfo, testCase.measure, false,
                            testCase.scrollStyle, testCase.scrollLength, testCase.scrollExpected);
        runControlPointCase(noteInfo, testCase.measure, true,
                            testCase.pageStyle, testCase.pageLength, testCase.pageExpected);
    }
}
