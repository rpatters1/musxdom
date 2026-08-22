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

constexpr static musxtest::string_view playbackXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <playbackRoute cmper="1">
      <virtChannel>7</virtChannel>
      <patchType>C0_C32_PC</patchType>
      <patch>42</patch>
      <lsb>5</lsb>
      <msb>121</msb>
      <percMapRefID>3</percMapRefID>
      <soundUUID>db30b478-5871-3ba5-9553-6f964bb3aeb7</soundUUID>
    </playbackRoute>
    <playbackRouteName cmper="1">
      <name>ARIA Player</name>
    </playbackRouteName>
    <playbackRouteName cmper="2">
      <name>Expression Route</name>
    </playbackRouteName>
    <playbackRouteName cmper="3">
      <name>Chord Route</name>
    </playbackRouteName>
    <playbackRouteName cmper="4">
      <name/>
    </playbackRouteName>
    <staffPlayData cmper="5">
      <chords>
        <playbackRouteNum>2</playbackRouteNum>
        <velControlLevel>101</velControlLevel>
        <velControlSaveFlags>
          <controlOn/>
          <function>relative</function>
        </velControlSaveFlags>
        <play/>
        <solo/>
      </chords>
      <midiExpressions>
        <playbackRouteNum>1</playbackRouteNum>
        <velControlLevel>64</velControlLevel>
        <velControlSaveFlags>
          <function>absolute</function>
        </velControlSaveFlags>
      </midiExpressions>
      <layer1><play/></layer1>
      <layer2><playbackRouteNum>1</playbackRouteNum></layer2>
      <layer3><playbackRouteNum>2</playbackRouteNum></layer3>
      <layer4><playbackRouteNum>3</playbackRouteNum></layer4>
    </staffPlayData>
    <staffPlayData cmper="6">
      <chords><playbackRouteNum>2</playbackRouteNum></chords>
      <midiExpressions><playbackRouteNum>1</playbackRouteNum></midiExpressions>
      <layer1><playbackRouteNum>3</playbackRouteNum></layer1>
      <layer2><playbackRouteNum>4</playbackRouteNum></layer2>
    </staffPlayData>
    <staffSpec cmper="5"><staffLines>5</staffLines></staffSpec>
    <staffSpec cmper="6"><staffLines>5</staffLines></staffSpec>
    <staffSpec cmper="7"><staffLines>5</staffLines></staffSpec>
  </others>
</finale>
)xml";

TEST(PlaybackTest, PopulatePlaybackRoute)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(playbackXml);
    auto route = doc->getOthers()->get<others::PlaybackRoute>(SCORE_PARTID, 1);
    ASSERT_TRUE(route);
    EXPECT_EQ(route->virtualChannel, 7);
    EXPECT_EQ(route->patchType, "C0_C32_PC");
    EXPECT_EQ(route->patch, 42);
    EXPECT_EQ(route->midiBankSelectLeastSignificantByte, 5);
    EXPECT_EQ(route->midiBankSelectMostSignificantByte, 121);
    EXPECT_EQ(route->percussionMapId, 3);
    EXPECT_EQ(route->soundUuid, "db30b478-5871-3ba5-9553-6f964bb3aeb7");

    auto routeName = doc->getOthers()->get<others::PlaybackRouteName>(SCORE_PARTID, 1);
    ASSERT_TRUE(routeName);
    EXPECT_EQ(routeName->name, "ARIA Player");
}

TEST(PlaybackTest, PopulateStaffPlayData)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(playbackXml);
    auto staffPlayData = doc->getOthers()->get<others::StaffPlayData>(SCORE_PARTID, 5);
    ASSERT_TRUE(staffPlayData);

    ASSERT_TRUE(staffPlayData->chords);
    EXPECT_EQ(staffPlayData->chords->playbackRouteIndex, 2);
    EXPECT_EQ(staffPlayData->chords->velocityControlLevel, 101);
    EXPECT_TRUE(staffPlayData->chords->play);
    EXPECT_TRUE(staffPlayData->chords->solo);
    ASSERT_TRUE(staffPlayData->chords->velocityControl);
    EXPECT_TRUE(staffPlayData->chords->velocityControl->controlOn);
    EXPECT_EQ(staffPlayData->chords->velocityControl->function,
        others::StaffPlayData::VelocityControlFunction::Relative);

    ASSERT_TRUE(staffPlayData->midiExpressions);
    EXPECT_EQ(staffPlayData->midiExpressions->playbackRouteIndex, 1);
    EXPECT_EQ(staffPlayData->midiExpressions->velocityControlLevel, 64);
    EXPECT_FALSE(staffPlayData->midiExpressions->play);
    EXPECT_FALSE(staffPlayData->midiExpressions->solo);
    ASSERT_TRUE(staffPlayData->midiExpressions->velocityControl);
    EXPECT_FALSE(staffPlayData->midiExpressions->velocityControl->controlOn);
    EXPECT_EQ(staffPlayData->midiExpressions->velocityControl->function,
        others::StaffPlayData::VelocityControlFunction::Absolute);

    for (const auto& layer : staffPlayData->layers) {
        ASSERT_TRUE(layer);
    }
    EXPECT_TRUE(staffPlayData->layers[0]->play);
    EXPECT_EQ(staffPlayData->layers[0]->playbackRouteIndex, 0);
    EXPECT_EQ(staffPlayData->layers[1]->playbackRouteIndex, 1);
    EXPECT_EQ(staffPlayData->layers[2]->playbackRouteIndex, 2);
    EXPECT_EQ(staffPlayData->layers[3]->playbackRouteIndex, 3);
}

TEST(PlaybackTest, StaffPlaybackRouteName)
{
    constexpr StaffCmper layerRouteStaffId = 5;
    constexpr StaffCmper fallbackRouteStaffId = 6;
    constexpr StaffCmper noRouteStaffId = 7;

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(playbackXml);

    auto layerRouteStaff = doc->getOthers()->get<others::Staff>(SCORE_PARTID, layerRouteStaffId);
    ASSERT_TRUE(layerRouteStaff);
    EXPECT_EQ(layerRouteStaff->getPlaybackRouteName(), "ARIA Player");

    auto fallbackRouteStaff = doc->getOthers()->get<others::Staff>(SCORE_PARTID, fallbackRouteStaffId);
    ASSERT_TRUE(fallbackRouteStaff);
    EXPECT_EQ(fallbackRouteStaff->getPlaybackRouteName(), "Chord Route");

    auto noRouteStaff = doc->getOthers()->get<others::Staff>(SCORE_PARTID, noRouteStaffId);
    ASSERT_TRUE(noRouteStaff);
    EXPECT_TRUE(noRouteStaff->getPlaybackRouteName().empty());
}
