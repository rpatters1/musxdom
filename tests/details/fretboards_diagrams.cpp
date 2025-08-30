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
using namespace musx::util;

TEST(FretboardDiagramTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <fretboard cmper1="65534" cmper2="1">
      <numFrets>4</numFrets>
      <fretNum>2</fretNum>
      <lock/>
      <showNum/>
      <numFretCells>12</numFretCells>
      <numFretBarres>1</numFretBarres>
      <cell>
        <string>4</string>
        <fret>3</fret>
        <shape>closed</shape>
      </cell>
      <cell>
        <string>3</string>
        <fret>3</fret>
        <shape>closed</shape>
      </cell>
      <cell>
        <string>2</string>
        <fret>3</fret>
        <shape>closed</shape>
      </cell>
      <cell>
        <string>1</string>
        <fret>1</fret>
        <shape>closed</shape>
      </cell>
      <cell>
        <string>5</string>
        <fret>1</fret>
        <shape>open</shape>
      </cell>
      <cell>
        <string>4</string>
        <fret>2</fret>
        <shape>muted</shape>
      </cell>
      <cell>
        <string>3</string>
        <fret>1</fret>
        <shape>custom</shape>
      </cell>
      <cell>
        <string>3</string>
        <fret>2</fret>
        <fingerNum>1</fingerNum>
      </cell>
      <cell>
        <string>2</string>
        <fret>2</fret>
        <fingerNum>2</fingerNum>
      </cell>
      <cell>
        <string>1</string>
        <fret>2</fret>
        <fingerNum>3</fingerNum>
      </cell>
      <cell>
        <string>1</string>
        <fret>4</fret>
        <fingerNum>4</fingerNum>
      </cell>
      <cell>
        <string>6</string>
        <fret>4</fret>
        <fingerNum>5</fingerNum>
      </cell>
      <barre>
        <fret>0</fret>
        <startString>1</startString>
        <endString>5</endString>
      </barre>
    </fretboard>
  </details>
</finale>
)xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto fb = details->get<details::FretboardDiagram>(SCORE_PARTID, 65534, 1);
    ASSERT_TRUE(fb);

    // Top-level fields
    EXPECT_EQ(fb->numFrets, 4);
    EXPECT_EQ(fb->fretboardNum, 2); // starting fret for diagram
    EXPECT_TRUE(fb->lock);
    EXPECT_TRUE(fb->showNum);

    // Counts (and that integrityCheck left them consistent)
    EXPECT_EQ(fb->numFretCells, 12);
    EXPECT_EQ(fb->numFretBarres, 1);
    ASSERT_EQ(fb->cells.size(), static_cast<size_t>(12));
    ASSERT_EQ(fb->barres.size(), static_cast<size_t>(1));

    // Spot-check a few cells for enum mapping and values
    {
        // First cell: closed on string 4, fret 3
        auto c0 = fb->cells[0];
        ASSERT_TRUE(c0);
        EXPECT_EQ(c0->string, 4);
        EXPECT_EQ(c0->fret, 3);
        EXPECT_EQ(c0->shape, details::FretboardDiagram::Shape::Closed);
        EXPECT_EQ(c0->fingerNum, 0); // not present -> default 0
    }
    {
        // A cell with open shape
        auto c4 = fb->cells[4];
        ASSERT_TRUE(c4);
        EXPECT_EQ(c4->string, 5);
        EXPECT_EQ(c4->fret, 1);
        EXPECT_EQ(c4->shape, details::FretboardDiagram::Shape::Open);
    }
    {
        // A cell with muted shape
        auto c5 = fb->cells[5];
        ASSERT_TRUE(c5);
        EXPECT_EQ(c5->shape, details::FretboardDiagram::Shape::Muted);
    }
    {
        // A cell with custom shape
        auto c6 = fb->cells[6];
        ASSERT_TRUE(c6);
        EXPECT_EQ(c6->shape, details::FretboardDiagram::Shape::Custom);
    }
    {
        // A finger-number-only cell should default shape to None
        auto c7 = fb->cells[7];
        ASSERT_TRUE(c7);
        EXPECT_EQ(c7->string, 3);
        EXPECT_EQ(c7->fret, 2);
        EXPECT_EQ(c7->fingerNum, 1);
        EXPECT_EQ(c7->shape, details::FretboardDiagram::Shape::None);
    }
    {
        // Thumb = 5
        auto c11 = fb->cells[11];
        ASSERT_TRUE(c11);
        EXPECT_EQ(c11->string, 6);
        EXPECT_EQ(c11->fret, 4);
        EXPECT_EQ(c11->fingerNum, 5);
    }

    // Barre
    {
        auto b0 = fb->barres[0];
        ASSERT_TRUE(b0);
        EXPECT_EQ(b0->fret, 0); // Finale allows nut/capo representation as fret 0
        EXPECT_EQ(b0->startString, 1);
        EXPECT_EQ(b0->endString, 5);
    }
}

TEST(FretboardDiagramTest, RetrieveFretboardDiagrams)
{
    std::vector<char> enigmaXml;
    musxtest::readFile(musxtest::getInputPath() / "finale_maestro_default.enigmaxml", enigmaXml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(enigmaXml);
    ASSERT_TRUE(doc);

    auto fretGroup = doc->getOthers()->get<others::FretboardGroup>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(fretGroup);
    EXPECT_EQ(fretGroup->fretInstId, 2);
    EXPECT_EQ(fretGroup->name, "Major   (copy)");

    auto fretInst = fretGroup->getFretInstrument();
    ASSERT_TRUE(fretInst);
    EXPECT_EQ(fretInst->numFrets, 21);
    EXPECT_EQ(fretInst->numStrings, 6);
    EXPECT_EQ(fretInst->name, " Standard Guitar");

    auto fretDiagrams = fretGroup->getFretboardDiagrams();
    ASSERT_EQ(fretDiagrams.size(), 12);
    EXPECT_EQ(fretDiagrams[0]->getCmper1(), 1);
    EXPECT_EQ(fretDiagrams[0]->getCmper2(), 16);
    EXPECT_EQ(fretDiagrams[11]->getCmper1(), 1);
    EXPECT_EQ(fretDiagrams[11]->getCmper2(), 27);
}
