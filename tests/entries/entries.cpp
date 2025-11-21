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

TEST(EntryTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <entries>
    <entry entnum="1001" prev="1000" next="1002">
      <dura>896</dura>
      <numNotes>2</numNotes>
      <isValid/>
      <isNote/>
      <splitStem/>
      <crossStaff/>
      <smartShapeDetail/>
      <graceNote/>
      <floatRest/>
      <noLeger/>
      <sorted/>
      <note id="1">
        <harmLev>-4</harmLev>
        <harmAlt>0</harmAlt>
        <isValid/>
        <upStemSecond/>
        <upSplitStem/>
      </note>
      <note id="2">
        <harmLev>-2</harmLev>
        <harmAlt>1</harmAlt>
        <isValid/>
        <crossStaff/>
        <tieStart/>
        <tieEnd/>
        <showAcci/>
        <parenAcci/>
        <freezeAcci/>
      </note>
    </entry>
    <entry entnum="1002" prev="1001" next="1003">
      <dura>256</dura>
      <numNotes>0</numNotes>
      <isValid/>
      <doubleStem/>
      <slashGrace/>
      <floatRest/>
      <ignore/>
      <sorted/>
    </entry>
  </entries>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto entries = doc->getEntries();
    ASSERT_TRUE(entries);

    // Test Entry 1001
    {
        auto entry = entries->get(1001);
        ASSERT_TRUE(entry);

        EXPECT_EQ(entry->getEntryNumber(), 1001);
        EXPECT_EQ(entry->duration, 896);
        EXPECT_EQ(entry->numNotes, 2);
        EXPECT_TRUE(entry->isValid);
        EXPECT_TRUE(entry->isNote);
        EXPECT_TRUE(entry->crossStaff);
        EXPECT_TRUE(entry->smartShapeDetail);
        EXPECT_TRUE(entry->graceNote);
        EXPECT_TRUE(entry->floatRest);
        EXPECT_FALSE(entry->isHidden);
        EXPECT_TRUE(entry->noLeger);
        EXPECT_FALSE(entry->slashGrace);
        EXPECT_TRUE(entry->sorted);

        ASSERT_EQ(entry->notes.size(), 2);

        auto note = entry->notes[0];
        ASSERT_TRUE(note);

        EXPECT_TRUE(note->upStemSecond);
        EXPECT_TRUE(note->upSplitStem);
        EXPECT_FALSE(note->showAcci);
        EXPECT_FALSE(note->parenAcci);
        EXPECT_FALSE(note->freezeAcci);

        note = entry->notes[1];
        ASSERT_TRUE(note);

        EXPECT_EQ(note->getNoteId(), 2);
        EXPECT_EQ(note->harmLev, -2);
        EXPECT_EQ(note->harmAlt, 1);
        EXPECT_TRUE(note->isValid);
        EXPECT_FALSE(entry->doubleStem);
        EXPECT_TRUE(entry->splitStem);
        EXPECT_TRUE(note->crossStaff);
        EXPECT_TRUE(note->tieStart);
        EXPECT_TRUE(note->tieEnd);
        EXPECT_FALSE(note->upSplitStem);
        EXPECT_FALSE(note->upSplitStem);
        EXPECT_TRUE(note->showAcci);
        EXPECT_TRUE(note->parenAcci);
        EXPECT_TRUE(note->freezeAcci);

        auto [noteType, numDots] = entry->calcNoteInfo();
        EXPECT_EQ(noteType, NoteType::Eighth);
        EXPECT_EQ(numDots, 2);
    }

    // Test Entry 1002
    {
        auto entry = entries->get(1002);
        ASSERT_TRUE(entry);

        EXPECT_EQ(entry->getEntryNumber(), 1002);
        EXPECT_EQ(entry->duration, 256);
        EXPECT_EQ(entry->numNotes, 0);
        EXPECT_TRUE(entry->isValid);
        EXPECT_FALSE(entry->isNote);
        EXPECT_TRUE(entry->doubleStem);
        EXPECT_FALSE(entry->splitStem);
        EXPECT_FALSE(entry->crossStaff);
        EXPECT_FALSE(entry->smartShapeDetail);
        EXPECT_TRUE(entry->floatRest);
        EXPECT_TRUE(entry->isHidden);
        EXPECT_FALSE(entry->noLeger);
        EXPECT_TRUE(entry->slashGrace);
        EXPECT_TRUE(entry->sorted);

        EXPECT_EQ(entry->notes.size(), 0);

        auto [noteType, numDots] = entry->calcNoteInfo();
        EXPECT_EQ(noteType, NoteType::Note16th);
        EXPECT_EQ(numDots, 0);
    }
}

TEST(EntryTest, IntegrityCheck)
{
    constexpr static musxtest::string_view xmlWrongNumNotes = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <entries>
    <entry entnum="1001" prev="1000" next="1002">
      <dura>896</dura>
      <numNotes>3</numNotes>
      <isValid/>
      <isNote/>
      <floatRest/>
      <sorted/>
      <note id="1">
        <harmLev>-4</harmLev>
        <harmAlt>0</harmAlt>
        <isValid/>
      </note>
      <note id="2">
        <harmLev>-2</harmLev>
        <harmAlt>1</harmAlt>
        <isValid/>
        <showAcci/>
      </note>
    </entry>
  </entries>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xmlWrongNumNotes),
        musx::dom::integrity_error
    ) << "number of notes does not match numNotes";

    constexpr static musxtest::string_view xmlMissingPrevNext = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <entries>
    <entry entnum="1001" prev="1000" next="1002">
      <dura>896</dura>
      <numNotes>2</numNotes>
      <isValid/>
      <isNote/>
      <floatRest/>
      <sorted/>
      <note id="1">
        <harmLev>-4</harmLev>
        <harmAlt>0</harmAlt>
        <isValid/>
      </note>
      <note id="2">
        <harmLev>-2</harmLev>
        <harmAlt>1</harmAlt>
        <isValid/>
        <showAcci/>
      </note>
    </entry>
  </entries>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xmlMissingPrevNext);
    ASSERT_TRUE(doc);

    auto entries = doc->getEntries();
    ASSERT_TRUE(entries);

    auto entry = entries->get(1001);
    ASSERT_TRUE(entry);

    EXPECT_THROW(
        entry->getNext(),
        musx::dom::integrity_error
    ) << "next entry does not exist";

    EXPECT_THROW(
        entry->getPrevious(),
        musx::dom::integrity_error
    ) << "previous entry does not exist";
}
 
TEST(EntryTest, UnlinkedEnharmonicSpelling)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "enharmonic_unlinked.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto checkEntry = [](bool forWrittenPitch, const EntryInfoPtr& entryInfo, music_theory::NoteName expectedNoteName, int expectedOctave, int expectedAlteration,
                            const std::optional<bool>& enharmonicRespell = std::nullopt) {
        NoteInfoPtr noteInfo(entryInfo, 0); // assume 1st note
        ASSERT_TRUE(noteInfo);
        auto [noteName, octave, alteration, staffLine] = forWrittenPitch
                                                       ? noteInfo.calcNoteProperties(enharmonicRespell)
                                                       : noteInfo.calcNotePropertiesConcert();
        EXPECT_EQ(noteName, expectedNoteName);
        EXPECT_EQ(octave, expectedOctave);
        EXPECT_EQ(alteration, expectedAlteration);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(false, EntryInfoPtr(entryFrame, 0), music_theory::NoteName::E, 4, 1);
        checkEntry(false, EntryInfoPtr(entryFrame, 1), music_theory::NoteName::E, 4, 1);
        checkEntry(true, EntryInfoPtr(entryFrame, 0), music_theory::NoteName::F, 4, 2);
        checkEntry(true, EntryInfoPtr(entryFrame, 1), music_theory::NoteName::F, 4, 2);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, 1, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(false, EntryInfoPtr(entryFrame, 0), music_theory::NoteName::F, 4, 0);
        checkEntry(false, EntryInfoPtr(entryFrame, 1), music_theory::NoteName::E, 4, 1);
        checkEntry(true, EntryInfoPtr(entryFrame, 0), music_theory::NoteName::G, 4, 0);
        checkEntry(true, EntryInfoPtr(entryFrame, 1), music_theory::NoteName::F, 4, 2);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        NoteInfoPtr firstNote(EntryInfoPtr(entryFrame, 0), 0);
        auto noteAlts = details->getForNote<details::NoteAlterations>(firstNote);
        ASSERT_TRUE(noteAlts);
        EXPECT_FALSE(noteAlts->enharmonic) << "Score is not enharmonically respelled";
        noteAlts = details->getForNote<details::NoteAlterations>(firstNote, 1);
        ASSERT_TRUE(noteAlts);
        EXPECT_TRUE(noteAlts->enharmonic) << "Part is enharmonically respelled";
        checkEntry(true, firstNote.getEntryInfo(), music_theory::NoteName::F, 4, 2, false);
        checkEntry(true, firstNote.getEntryInfo(), music_theory::NoteName::G, 4, 0, true);
    }
}

TEST(EntryTest, TransposedConcert)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "hidden_keysigs.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto checkEntry = [](const EntryInfoPtr& entryInfo, music_theory::NoteName expectedNoteName, int expectedOctave, int expectedAlteration,
                            music_theory::NoteName expectedConcertNoteName, int expectedConcertOctave, int expectedConcertAlteration) {
        NoteInfoPtr noteInfo(entryInfo, 0); // assume 1st note
        ASSERT_TRUE(noteInfo);
        auto [noteName, octave, alteration, staffLine] = noteInfo.calcNoteProperties();
        EXPECT_EQ(noteName, expectedNoteName);
        EXPECT_EQ(octave, expectedOctave);
        EXPECT_EQ(alteration, expectedAlteration);
        auto [noteNameConcert, octaveConcert, alterationConcert, staffLineConcert] = noteInfo.calcNotePropertiesConcert();
        EXPECT_EQ(noteNameConcert, expectedConcertNoteName);
        EXPECT_EQ(octaveConcert, expectedConcertOctave);
        EXPECT_EQ(alterationConcert, expectedConcertAlteration);
    };

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);  // Clarinet in A bar 1
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::D, 5, -1, music_theory::NoteName::B, 4, -1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::F, 5, 0, music_theory::NoteName::D, 5, 0);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::G, 5, -1, music_theory::NoteName::E, 5, -1);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 3);  // Clarinet in A bar 3
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::A, 5, 0, music_theory::NoteName::F, 5, 1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::C, 6, 1, music_theory::NoteName::A, 5, 1);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::D, 6, 0, music_theory::NoteName::B, 5, 0);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 1);  // Clarinet in Bb bar 1
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::C, 5, 0, music_theory::NoteName::B, 4, -1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::E, 5, 0, music_theory::NoteName::D, 5, 0);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::F, 5, 0, music_theory::NoteName::E, 5, -1);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 2, 3);  // Clarinet in Bb bar 3
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::A, 5, -1, music_theory::NoteName::F, 5, 1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::C, 6, 0, music_theory::NoteName::A, 5, 1);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::D, 6, -1, music_theory::NoteName::B, 5, 0);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 4, 1);  // Horn in F bar 1
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::F, 4, 0, music_theory::NoteName::B, 3, -1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::A, 4, 0, music_theory::NoteName::D, 4, 0);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::B, 4, -1, music_theory::NoteName::E, 4, -1);
    }

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 4, 3);  // Horn in F bar 3
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        checkEntry(EntryInfoPtr(entryFrame, 0), music_theory::NoteName::C, 5, 1, music_theory::NoteName::F, 4, 1);
        checkEntry(EntryInfoPtr(entryFrame, 1), music_theory::NoteName::E, 5, 1, music_theory::NoteName::A, 4, 1);
        checkEntry(EntryInfoPtr(entryFrame, 2), music_theory::NoteName::F, 5, 1, music_theory::NoteName::B, 4, 0);
    }
}

TEST(EntryTest, IsTrillToNote)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "trill-to.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);
    ASSERT_GE(entryFrame->getEntries().size(), 3);
    EXPECT_TRUE(EntryInfoPtr(entryFrame, 1).calcIsTrillToGraceEntry());
    EXPECT_FALSE(EntryInfoPtr(entryFrame, 1).calcIsGlissToGraceEntry());
    EXPECT_FALSE(EntryInfoPtr(entryFrame, 2).calcIsTrillToGraceEntry());
}

TEST(EntryTest, IsGlissToNote)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "trill-to.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold);
    auto entryFrame = gfhold.createEntryFrame(0);
    ASSERT_TRUE(entryFrame);
    ASSERT_GE(entryFrame->getEntries().size(), 3);
    EXPECT_FALSE(EntryInfoPtr(entryFrame, 1).calcIsTrillToGraceEntry());
    EXPECT_TRUE(EntryInfoPtr(entryFrame, 1).calcIsGlissToGraceEntry());
    EXPECT_FALSE(EntryInfoPtr(entryFrame, 2).calcIsTrillToGraceEntry());
}

TEST(EntryTest, EntryPartFieldDetail)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "entrypart.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto gfhold = details::GFrameHoldContext(doc, SCORE_PARTID, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 1);
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        EXPECT_EQ(entryInfo.calcManuaOffset(), 0);
        EXPECT_EQ(entryInfo.calcEntryStemSettings(), std::make_pair(false, false));
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, 1, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 1);
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        EXPECT_EQ(entryInfo.calcManuaOffset(), 28);
        EXPECT_EQ(entryInfo.calcEntryStemSettings(), std::make_pair(true, true));
    }
    {
        auto gfhold = details::GFrameHoldContext(doc, 2, 1, 1);
        ASSERT_TRUE(gfhold);
        auto entryFrame = gfhold.createEntryFrame(0);
        ASSERT_TRUE(entryFrame);
        ASSERT_GE(entryFrame->getEntries().size(), 1);
        auto entryInfo = EntryInfoPtr(entryFrame, 0);
        EXPECT_EQ(entryInfo.calcManuaOffset(), 0);
        EXPECT_EQ(entryInfo.calcEntryStemSettings(), std::make_pair(false, false));
    }
}
