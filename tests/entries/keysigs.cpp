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
 
TEST(KeySigs, Test12EDO)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "keysigs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<std::optional<int>> expectedKeyAlters = { -4, 5, -2, 3, 0 };
    std::vector<unsigned> expectedIndices = { 5, 4, 2, 0, 1 }; // AbMaj, G#min, Eb Lydian, C# Phrygian, D Freyish (non-linear)
    EXPECT_EQ(expectedKeyAlters.size(), expectedIndices.size());
    std::vector<std::optional<std::vector<int>>> expectedKeyMaps = {
        std::nullopt,
        std::nullopt,
        std::vector<int>{0, 2, 4, 6, 7, 9, 11},
        std::vector<int>{0, 1, 3, 5, 7, 8, 10},
        std::vector<int>{0, 1, 4, 5, 7, 8, 10}
    };

    std::vector<Note::NoteName> expectedNotes = {
        Note::NoteName::E, Note::NoteName::F, Note::NoteName::G, Note::NoteName::A,
        Note::NoteName::B, Note::NoteName::C, Note::NoteName::D, Note::NoteName::E };
    std::vector<int> expectedOctaves = { 4, 4, 4, 4, 4, 5, 5, 5 };
    std::vector<int> expectedAlters = { 0, 1, 0, -1, -1, 0, 0, -1 };
    std::vector<int> expectedPositions = { -8, -7, -6, -5, -4, -3, -2, -1 };

    auto measures = others->getArray<others::Measure>(SCORE_PARTID);
    ASSERT_GE(measures.size(), expectedKeyAlters.size());

    for (size_t i = 0; i < expectedKeyAlters.size(); i++) {
        auto key = measures[i]->createKeySignature();
        EXPECT_EQ(key->getAlteration(), expectedKeyAlters[i]);
        EXPECT_EQ(key->calcTonalCenterIndex(), expectedIndices[i]);
        EXPECT_EQ(key->calcKeyMap(), expectedKeyMaps[i]);
        if (auto keyMap = key->calcKeyMap()) {
            EXPECT_EQ(keyMap->size(), 7);
        }
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, measures[i]->getCmper());
        ASSERT_TRUE(gfhold);
        size_t x = 0;
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) -> bool {
            EXPECT_LT(x, expectedNotes.size()) << "too few expected values";
            if (x >= expectedNotes.size()) return false;
            //ASSERT_GE(entryInfo->getEntry()->notes.size(), 1);
            auto note = entryInfo->getEntry()->notes[0];
            auto [noteName, octave, alter, position] = note->calcNoteProperties(key, entryInfo->clefIndex);
            EXPECT_EQ(expectedOctaves[x], octave);
            EXPECT_EQ(expectedAlters[x], alter);
            EXPECT_EQ(expectedPositions[x], position);
            x++;
            return true;
        });
        EXPECT_EQ(x, expectedNotes.size());            
    }
}

 
TEST(KeySigs, Test31EDO)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "keysigs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<std::optional<int>> expectedKeyAlters = { 4, -5 };
    std::vector<unsigned> expectedIndices = { 2, 6 }; // EMaj, Bbmin
    EXPECT_EQ(expectedKeyAlters.size(), expectedIndices.size());
    std::vector<std::optional<std::vector<int>>> expectedKeyMaps = {
        std::vector<int>{0, 5, 10, 13, 18, 23, 28},
        std::vector<int>{0, 5, 8, 13, 18, 21, 26}
    };

    std::vector<Note::NoteName> expectedNotes = {
        Note::NoteName::E, Note::NoteName::F, Note::NoteName::G, Note::NoteName::A,
        Note::NoteName::B, Note::NoteName::C, Note::NoteName::D, Note::NoteName::E };
    std::vector<int> expectedOctaves = { 4, 4, 4, 4, 4, 5, 5, 5 };
    std::vector<int> expectedAlters = { 0, 2, 0, -2, -2, 3, 0, -1 };
    std::vector<int> expectedPositions = { -8, -7, -6, -5, -4, -3, -2, -1 };

    static constexpr size_t FIRST_31EDO_MEASURE_INDEX = 5;

    auto measures = others->getArray<others::Measure>(SCORE_PARTID);
    ASSERT_GE(measures.size(), expectedKeyAlters.size() + FIRST_31EDO_MEASURE_INDEX);

    for (size_t i = 0; i < expectedKeyAlters.size(); i++) {
        auto measure = measures[i + FIRST_31EDO_MEASURE_INDEX];
        auto key = measure->createKeySignature();
        EXPECT_EQ(key->getAlteration(), expectedKeyAlters[i]);
        EXPECT_EQ(key->calcTonalCenterIndex(), expectedIndices[i]);
        EXPECT_EQ(key->calcKeyMap(), expectedKeyMaps[i]);
        if (auto keyMap = key->calcKeyMap()) {
            EXPECT_EQ(keyMap->size(), 7);
        }
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, measure->getCmper());
        ASSERT_TRUE(gfhold);
        size_t x = 0;
        gfhold.iterateEntries([&](const EntryInfoPtr& entryInfo) -> bool {
            EXPECT_LT(x, expectedNotes.size()) << "too few expected values";
            if (x >= expectedNotes.size()) return false;
            //ASSERT_GE(entryInfo->getEntry()->notes.size(), 1);
            auto note = entryInfo->getEntry()->notes[0];
            auto [noteName, octave, alter, position] = note->calcNoteProperties(key, entryInfo->clefIndex);
            EXPECT_EQ(expectedOctaves[x], octave);
            EXPECT_EQ(expectedAlters[x], alter);
            EXPECT_EQ(expectedPositions[x], position);
            x++;
            return true;
        });
        EXPECT_EQ(x, expectedNotes.size());            
    }
}
