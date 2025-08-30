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

TEST(FretboardGroup, PopulateTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fretGroup cmper="4" inci="0">
      <fretInstID>2</fretInstID>
      <name>6/9 R5  </name>
    </fretGroup>
    <fretGroup cmper="4" inci="1">
      <fretInstID>2</fretInstID>
      <name>6/9 R6 </name>
    </fretGroup>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // First group (cmper=4, inci=0)
    auto g0 = others->get<others::FretboardGroup>(SCORE_PARTID, 4, 0);
    ASSERT_TRUE(g0);

    EXPECT_EQ(g0->fretInstId, 2);
    // Verify that trailing spaces are preserved
    EXPECT_EQ(g0->name, "6/9 R5  ");

    // Second group (cmper=4, inci=1)
    auto g1 = others->get<others::FretboardGroup>(SCORE_PARTID, 4, 1);
    ASSERT_TRUE(g1);

    EXPECT_EQ(g1->fretInstId, 2);
    // One trailing space here
    EXPECT_EQ(g1->name, "6/9 R6 ");

    // Sanity: both groups belong to the same cmper (4)
    EXPECT_EQ(g0->getCmper(), 4);
    EXPECT_EQ(g1->getCmper(), 4);
}

TEST(FretInstrument, PopulateTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fretInst cmper="6">
      <numFrets>20</numFrets>
      <numStrings>6</numStrings>
      <name>Gamba XYZ</name>
      <string>
        <pitch>60</pitch>
        <nutOffset>1</nutOffset>
      </string>
      <string>
        <pitch>55</pitch>
      </string>
      <string>
        <pitch>50</pitch>
      </string>
      <string>
        <pitch>47</pitch>
      </string>
      <string>
        <pitch>41</pitch>
      </string>
      <string>
        <pitch>36</pitch>
      </string>
      <diatonic>
        <fret>2</fret>
        <fret>4</fret>
        <fret>5</fret>
        <fret>7</fret>
        <fret>9</fret>
        <fret>11</fret>
        <fret>12</fret>
        <fret>14</fret>
        <fret>16</fret>
        <fret>17</fret>
        <fret>19</fret>
        <fret>21</fret>
        <fret>22</fret>
      </diatonic>
      <speedyClef>5</speedyClef>
    </fretInst>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto fi = others->get<others::FretInstrument>(SCORE_PARTID, 6);
    ASSERT_TRUE(fi);

    // Top-level fields
    EXPECT_EQ(fi->numFrets, 20);
    EXPECT_EQ(fi->numStrings, 6);
    EXPECT_EQ(fi->name, "Gamba XYZ");

    // Strings array
    ASSERT_EQ(fi->strings.size(), 6u);

    ASSERT_TRUE(fi->strings[0]);
    EXPECT_EQ(fi->strings[0]->pitch, 60);
    EXPECT_EQ(fi->strings[0]->nutOffset, 1);

    ASSERT_TRUE(fi->strings[1]);
    EXPECT_EQ(fi->strings[1]->pitch, 55);
    EXPECT_EQ(fi->strings[1]->nutOffset, 0);

    ASSERT_TRUE(fi->strings[2]);
    EXPECT_EQ(fi->strings[2]->pitch, 50);
    EXPECT_EQ(fi->strings[2]->nutOffset, 0);

    ASSERT_TRUE(fi->strings[3]);
    EXPECT_EQ(fi->strings[3]->pitch, 47);
    EXPECT_EQ(fi->strings[3]->nutOffset, 0);

    ASSERT_TRUE(fi->strings[4]);
    EXPECT_EQ(fi->strings[4]->pitch, 41);
    EXPECT_EQ(fi->strings[4]->nutOffset, 0);

    ASSERT_TRUE(fi->strings[5]);
    EXPECT_EQ(fi->strings[5]->pitch, 36);
    EXPECT_EQ(fi->strings[5]->nutOffset, 0);

    // Diatonic fret steps
    const std::vector<int> expectedSteps{2,4,5,7,9,11,12,14,16,17,19,21,22};
    ASSERT_EQ(fi->fretSteps.size(), expectedSteps.size());
    for (size_t i = 0; i < expectedSteps.size(); ++i) {
        EXPECT_EQ(fi->fretSteps[i], expectedSteps[i]) << "Mismatch at fretSteps[" << i << "]";
    }

    // Speedy clef
    EXPECT_EQ(static_cast<int>(fi->speedyClef), 5);

    // Sanity check: declared string count matches parsed array size
    EXPECT_EQ(static_cast<int>(fi->strings.size()), fi->numStrings);
}

TEST(FretboardStyle, PopulateTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <fretStyle cmper="4">
      <showLastFret/>
      <rotate/>
      <fingNumWhite/>
      <fingStrShapeID>65</fingStrShapeID>
      <openStrShapeID>66</openStrShapeID>
      <muteStrShapeID>67</muteStrShapeID>
      <barreShapeID>64</barreShapeID>
      <customShapeID>68</customShapeID>
      <defNumFrets>5</defNumFrets>
      <stringGap>1106</stringGap>
      <fretGap>1659</fretGap>
      <stringWidth>184</stringWidth>
      <fretWidth>184</fretWidth>
      <nutWidth>553</nutWidth>
      <vertTextOff>704</vertTextOff>
      <horzTextOff>448</horzTextOff>
      <horzHandleOff>192</horzHandleOff>
      <vertHandleOff>320</vertHandleOff>
      <whiteout>166</whiteout>
      <fretNumFont>
        <fontID>16</fontID>
        <fontSize>8</fontSize>
        <efx>
          <italic/>
        </efx>
      </fretNumFont>
      <fingNumFont>
        <fontID>16</fontID>
        <fontSize>6</fontSize>
        <efx>
          <bold/>
        </efx>
      </fingNumFont>
      <horzFingNumOff>64</horzFingNumOff>
      <vertFingNumOff>-111</vertFingNumOff>
      <name>Jazz</name>
      <fretNumText>fr.</fretNumText>
    </fretStyle>
  </others>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto style = others->get<others::FretboardStyle>(SCORE_PARTID, 4);
    ASSERT_TRUE(style);

    // Booleans
    EXPECT_TRUE(style->showLastFret);
    EXPECT_TRUE(style->rotate);
    EXPECT_TRUE(style->fingNumWhite);

    // Shape IDs
    EXPECT_EQ(style->fingStrShapeId, Cmper{65});
    EXPECT_EQ(style->openStrShapeId, Cmper{66});
    EXPECT_EQ(style->muteStrShapeId, Cmper{67});
    EXPECT_EQ(style->barreShapeId,   Cmper{64});
    EXPECT_EQ(style->customShapeId,  Cmper{68});

    // Counts
    EXPECT_EQ(style->defNumFrets, 5);

    // Dimensions & spacing (Efix)
    EXPECT_EQ(style->stringGap,     Efix{1106});
    EXPECT_EQ(style->fretGap,       Efix{1659});
    EXPECT_EQ(style->stringWidth,   Efix{184});
    EXPECT_EQ(style->fretWidth,     Efix{184});
    EXPECT_EQ(style->nutWidth,      Efix{553});
    EXPECT_EQ(style->vertTextOff,   Efix{704});
    EXPECT_EQ(style->horzTextOff,   Efix{448});
    EXPECT_EQ(style->horzHandleOff, Efix{192});
    EXPECT_EQ(style->vertHandleOff, Efix{320});
    EXPECT_EQ(style->whiteout,      Efix{166});

    // Fonts exist (integrityCheck also guarantees non-null if missing)
    ASSERT_TRUE(style->fretNumFont);
    ASSERT_TRUE(style->fingNumFont);

    EXPECT_EQ(style->fretNumFont->fontId, Cmper{16});
    EXPECT_EQ(style->fretNumFont->fontSize, 8);
    EXPECT_FALSE(style->fretNumFont->bold);
    EXPECT_TRUE(style->fretNumFont->italic);
    EXPECT_EQ(style->fingNumFont->fontId, Cmper{16});
    EXPECT_EQ(style->fingNumFont->fontSize, 6);
    EXPECT_TRUE(style->fingNumFont->bold);
    EXPECT_FALSE(style->fingNumFont->italic);

    // Fingering number offsets
    EXPECT_EQ(style->horzFingNumOff, Efix{64});
    EXPECT_EQ(style->vertFingNumOff, Efix{-111});

    // Text fields
    EXPECT_EQ(style->name, "Jazz");
    EXPECT_EQ(style->fretNumText, "fr.");
}
