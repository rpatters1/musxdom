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

TEST(GraceNoteOptionsTest, PropertiesTest)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
    <graceNoteOptions>
      <tabGracePerc>85</tabGracePerc>
      <gracePerc>70</gracePerc>
      <playbackDuration>128</playbackDuration>
      <graceBackup>30</graceBackup>
      <slashFlaggedGraceNotes/>
      <graceSlashWidth>128</graceSlashWidth>
    </graceNoteOptions>
  </options>
</finale>
)xml";
    
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    auto options = doc->getOptions();
    ASSERT_TRUE(options);

    auto graceNoteOptions = options->get<musx::dom::options::GraceNoteOptions>();
    ASSERT_TRUE(graceNoteOptions);

    // Test all properties of GraceNoteOptions
    EXPECT_EQ(graceNoteOptions->tabGracePerc, 85);
    EXPECT_EQ(graceNoteOptions->gracePerc, 70);
    EXPECT_EQ(graceNoteOptions->playbackDuration, 128);
    EXPECT_EQ(graceNoteOptions->entryOffset, 30);
    EXPECT_TRUE(graceNoteOptions->slashFlaggedGraceNotes);
    EXPECT_EQ(graceNoteOptions->graceSlashWidth, 128);
}
