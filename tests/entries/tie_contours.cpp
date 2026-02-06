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

} // namespace

TEST(TieContour, ContourTypes)
{
    std::vector<char> xml;
    // tie_contour_types.musx is the source file, but we load the extracted EnigmaXML here.
    musxtest::readFile(musxtest::getInputPath() / "tie_contour_types.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    const int staff = 1;
    const size_t entryIndex = 0;
    const size_t noteIndex = 0;

    const std::vector<int> measures{1, 3, 5};

    for (int measure : measures) {
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
    }
}
