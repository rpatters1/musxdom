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

#include <algorithm>

#include "gtest/gtest.h"
#include "musx/musx.h"
#include "test_utils.h"

using namespace musx::dom;

TEST(DocumentTest, DocumentFormation)
{
    constexpr static musxtest::string_view missingData = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
    )xml";
    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(missingData),
        std::invalid_argument
    );

    constexpr static musxtest::string_view emptyData = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale/>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(emptyData);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateHeader)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <header>
    <headerData>
    </headerData>
  </header>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateOptions)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <options>
  </options>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateOthers)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
  </others>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateDetails)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
  </details>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateEntries)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <entries>
  </entries>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, CreateTexts)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <texts>
  </texts>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
        EXPECT_TRUE(doc->getDetails());
        EXPECT_TRUE(doc->getEntries());
        EXPECT_TRUE(doc->getTexts());
    }
}

TEST(DocumentTest, MalformedXml)
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
</finale>
    )xml";

    // </entries> tag is missing

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml),
        musx::xml::load_error
    );

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
        musx::xml::load_error
    );

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml),
        musx::xml::load_error
    );
}

TEST(DocumentText, UnknownXml)
{
    constexpr static musxtest::string_view unknownTag = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <partGlobals cmper="65534">
      <showTransposed/>
      <studioViewIUlist>65400</studioViewIUlist>
      <unknownTag/>
    </partGlobals>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(unknownTag),
        musx::factory::unknown_xml_error
    );
}

TEST(DocumentTest, CalcJumpFromMeasures)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "crazy_jumps.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xml);
    ASSERT_TRUE(doc);

    {
        auto ending = doc->getOthers()->get<musx::dom::others::RepeatEndingStart>(SCORE_PARTID, 5);
        ASSERT_TRUE(ending);
        EXPECT_EQ(ending->jumpAction, musx::dom::others::RepeatActionType::JumpAbsolute);
        EXPECT_EQ(ending->targetValue, 3);
        auto target = ending->calcTargetMeasure();
        ASSERT_TRUE(target);
        EXPECT_EQ(*target, 3);
        auto passList = doc->getOthers()->get<musx::dom::others::RepeatPassList>(SCORE_PARTID, 5);
        ASSERT_TRUE(passList);
        EXPECT_TRUE(passList->containsValue(1));
    }
    {
        auto endings = doc->getOthers()->getArray<musx::dom::others::RepeatEndingStart>(SCORE_PARTID);
        ASSERT_FALSE(endings.empty());
        const bool hasEndingFive = std::any_of(endings.begin(), endings.end(),
            [](const auto& ending) { return ending && ending->getCmper() == 5; });
        EXPECT_TRUE(hasEndingFive);
    }

    std::vector<MeasCmper> expected{ 4, 5, 8, 2 };
    auto result = doc->calcJumpFromMeasures(SCORE_PARTID, 3);
    EXPECT_EQ(result, expected);
}
