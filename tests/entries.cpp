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
      <graceNote/>
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
        <freezeAcci/>
      </note>
    </entry>
    <entry entnum="1002" prev="1001" next="1003">
      <dura>256</dura>
      <numNotes>0</numNotes>
      <isValid/>
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
        auto entry = entries->get<musx::dom::Entry>(1001);
        ASSERT_TRUE(entry);

        EXPECT_EQ(entry->getEntryNumber(), 1001);
        EXPECT_EQ(entry->duration, 896);
        EXPECT_EQ(entry->numNotes, 2);
        EXPECT_TRUE(entry->isValid);
        EXPECT_TRUE(entry->isNote);
        EXPECT_TRUE(entry->graceNote);
        EXPECT_TRUE(entry->floatRest);
        EXPECT_FALSE(entry->isHidden);
        EXPECT_TRUE(entry->sorted);

        ASSERT_EQ(entry->notes.size(), 2);
        auto note = entry->notes[1];
        ASSERT_TRUE(note);

        EXPECT_EQ(note->getNoteId(), 2);
        EXPECT_EQ(note->harmLev, -2);
        EXPECT_EQ(note->harmAlt, 1);
        EXPECT_TRUE(note->isValid);
        EXPECT_TRUE(note->showAcci);
        EXPECT_TRUE(note->freezeAcci);

        EXPECT_EQ(entry->calcNoteType(), Entry::NoteType::Eighth);
        EXPECT_EQ(entry->calcAugmentationDots(), 2);
    }

    // Test Entry 1002
    {
        auto entry = entries->get<musx::dom::Entry>(1002);
        ASSERT_TRUE(entry);

        EXPECT_EQ(entry->getEntryNumber(), 1002);
        EXPECT_EQ(entry->duration, 256);
        EXPECT_EQ(entry->numNotes, 0);
        EXPECT_TRUE(entry->isValid);
        EXPECT_FALSE(entry->isNote);
        EXPECT_TRUE(entry->floatRest);
        EXPECT_TRUE(entry->isHidden);
        EXPECT_TRUE(entry->sorted);

        EXPECT_EQ(entry->notes.size(), 0);

        EXPECT_EQ(entry->calcNoteType(), Entry::NoteType::Note16th);
        EXPECT_EQ(entry->calcAugmentationDots(), 0);
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

    auto entry = entries->get<musx::dom::Entry>(1001);
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
