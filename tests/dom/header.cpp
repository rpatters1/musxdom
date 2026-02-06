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

static constexpr musxtest::string_view fullHeaderXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <wordOrder>lo-endian</wordOrder>
      <textEncoding>Mac</textEncoding>
      <created>
        <year>2024</year>
        <month>11</month>
        <day>28</day>
        <modifiedBy>RGP</modifiedBy>
        <enigmaVersion>
          <major>27</major>
          <minor>4</minor>
          <devStatus>dev</devStatus>
        </enigmaVersion>
        <application>FIN</application>
        <platform>MAC</platform>
        <appVersion>
          <major>27</major>
          <minor>4</minor>
          <maint>1</maint>
          <devStatus>dev</devStatus>
          <build>146</build>
        </appVersion>
        <fileVersion>
          <major>27</major>
          <minor>4</minor>
          <devStatus>dev</devStatus>
        </fileVersion>
        <appRegion>US</appRegion>
      </created>
      <modified>
        <year>2024</year>
        <month>12</month>
        <day>1</day>
        <modifiedBy>RGP</modifiedBy>
        <enigmaVersion>
          <major>27</major>
          <minor>4</minor>
          <devStatus>dev</devStatus>
        </enigmaVersion>
        <application>FIN</application>
        <platform>MAC</platform>
        <appVersion>
          <major>27</major>
          <minor>4</minor>
          <maint>1</maint>
          <devStatus>dev</devStatus>
          <build>177</build>
        </appVersion>
        <fileVersion>
          <major>27</major>
          <minor>4</minor>
          <devStatus>dev</devStatus>
        </fileVersion>
        <appRegion>US</appRegion>
      </modified>
    </headerData>
  </header>
</finale>
)xml";

class HeaderTest : public ::testing::Test
{
protected:
    void validateHeader(const musx::dom::DocumentPtr& doc)
    {
        auto header = doc->getHeader();
        ASSERT_TRUE(header);
        EXPECT_EQ(header->wordOrder, musx::dom::WordOrder::LittleEndian);
        EXPECT_EQ(header->textEncoding, musx::dom::TextEncoding::Mac);
        auto checkFileInfo = [](const musx::dom::header::FileInfo& info, int year, int month, int day, int build) {
                EXPECT_EQ(info.year, year);
                EXPECT_EQ(info.month, month);
                EXPECT_EQ(info.day, day);
                EXPECT_EQ(info.application, "FIN");
                EXPECT_EQ(info.modifiedBy, "RGP");
                EXPECT_EQ(info.appRegion, "US");
                EXPECT_EQ(info.platform, musx::dom::header::Platform::Mac);
                EXPECT_EQ(info.finaleVersion.major, 27);
                EXPECT_EQ(info.finaleVersion.minor, 4);
                ASSERT_FALSE(info.finaleVersion.maint.has_value());
                EXPECT_EQ(info.finaleVersion.devStatus, "dev");
                ASSERT_FALSE(info.finaleVersion.build.has_value());
                EXPECT_EQ(info.appVersion.major, 27);
                EXPECT_EQ(info.appVersion.minor, 4);
                EXPECT_EQ(info.appVersion.maint, 1);
                EXPECT_EQ(info.appVersion.devStatus, "dev");
                EXPECT_EQ(info.appVersion.build, build);
                EXPECT_EQ(info.fileVersion.major, 27);
                EXPECT_EQ(info.fileVersion.minor, 4);
                ASSERT_FALSE(info.fileVersion.maint.has_value());
                EXPECT_EQ(info.fileVersion.devStatus, "dev");
                ASSERT_FALSE(info.fileVersion.build.has_value());
            };
        checkFileInfo(header->created, 2024, 11, 28, 146);
        checkFileInfo(header->modified, 2024, 12, 1, 177);
    }
};

TEST_F(HeaderTest, FieldsExistRapidXml)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(fullHeaderXml);
    validateHeader(doc);
}

TEST_F(HeaderTest, FieldsExistTinyXml2)
{
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(fullHeaderXml);
    validateHeader(doc);
}

TEST_F(HeaderTest, NoHeaderDataTag)
{
    constexpr static musxtest::string_view malformed = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
  </header>
</finale>
     )xml";
    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(malformed),
        std::invalid_argument
    );
}


TEST_F(HeaderTest, EnumTests)
{
    constexpr static musxtest::string_view invalidWordOrder = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <wordOrder>malformed</wordOrder>
    </headerData>
  </header>
</finale>
     )xml";
    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(invalidWordOrder),
        std::invalid_argument
    );

    constexpr static musxtest::string_view invalidTextEncoding = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <wordOrder>hi-endian</wordOrder>
      <textEncoding>XXX</textEncoding>
    </headerData>
  </header>
</finale>
     )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(invalidTextEncoding);
        auto header = doc->getHeader();
        ASSERT_TRUE(header);
        EXPECT_EQ(header->wordOrder, musx::dom::header::WordOrder::BigEndian);
        EXPECT_EQ(header->textEncoding, musx::dom::header::TextEncoding::Other);
    }

    constexpr static musxtest::string_view winTextEncoding = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <wordOrder>lo-endian</wordOrder>
      <textEncoding>Windows</textEncoding>
    </headerData>
  </header>
</finale>
     )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(winTextEncoding);
        auto header = doc->getHeader();
        ASSERT_TRUE(header);
        EXPECT_EQ(header->wordOrder, musx::dom::header::WordOrder::LittleEndian);
        EXPECT_EQ(header->textEncoding, musx::dom::header::TextEncoding::Windows);
    }

    // Mac platform tested above.

    constexpr static musxtest::string_view winPlatform = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <created>
        <platform>WIN</platform>
      </created>
    </headerData>
  </header>
</finale>
     )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(winPlatform);
        auto header = doc->getHeader();
        ASSERT_TRUE(header);
        EXPECT_EQ(header->created.platform, musx::dom::header::Platform::Windows);
    }

    constexpr static musxtest::string_view otherPlatform = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
      <created>
        <platform>zzzzfdddsf</platform>
      </created>
    </headerData>
  </header>
</finale>
     )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(otherPlatform);
        auto header = doc->getHeader();
        ASSERT_TRUE(header);
        EXPECT_EQ(header->created.platform, musx::dom::header::Platform::Other);
    }
}