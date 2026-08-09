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
#include "musx/factory/PoolFactory.h"
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

TEST(DocumentTest, EmbeddedGraphicsRoundTrip)
{
    constexpr static musxtest::string_view emptyData = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale/>
    )xml";

    musx::factory::DocumentFactory::CreateOptions::EmbeddedGraphicFiles graphics{
        { "101.png", EmbeddedGraphicBlob{0x89, 0x50, 0x4E, 0x47} },
        { "202.jpg", EmbeddedGraphicBlob{0xFF, 0xD8, 0xFF} },
        { "not-a-cmper.png", EmbeddedGraphicBlob{0x00} },
        { "303", EmbeddedGraphicBlob{0x11} }
    };

    musx::factory::DocumentFactory::CreateOptions options(
        std::filesystem::path(""),
        std::vector<char>{},
        std::move(graphics));
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(
        emptyData.data(), emptyData.size(), std::move(options));

    ASSERT_TRUE(doc);
    const auto& embedded = doc->getEmbeddedGraphics();
    ASSERT_EQ(embedded.size(), 2u);
    auto itPng = embedded.find(Cmper(101));
    ASSERT_NE(itPng, embedded.end());
    EXPECT_EQ(itPng->second.extension, "png");
    EXPECT_EQ(itPng->second.bytes, (EmbeddedGraphicBlob{0x89, 0x50, 0x4E, 0x47}));
    auto itJpg = embedded.find(Cmper(202));
    ASSERT_NE(itJpg, embedded.end());
    EXPECT_EQ(itJpg->second.extension, "jpg");
    EXPECT_EQ(itJpg->second.bytes, (EmbeddedGraphicBlob{0xFF, 0xD8, 0xFF}));
}

namespace {

class CountingOptions final : public musx::dom::OptionsBase
{
public:
    CountingOptions(const DocumentPtr& document, int& checkCount)
        : OptionsBase(document, SCORE_PARTID, ShareMode::All), m_checkCount(checkCount) {}

    void integrityCheck(const std::shared_ptr<EnigmaBase>&) override { ++m_checkCount; }

private:
    int& m_checkCount;
};

} // namespace

TEST(DocumentConstructionTest, SessionInitializesAndValidatesPoolsAtFinish)
{
    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();
    ASSERT_TRUE(document->getHeader());
    ASSERT_TRUE(document->getOptions());
    ASSERT_TRUE(document->getOthers());
    ASSERT_TRUE(document->getDetails());
    ASSERT_TRUE(document->getEntries());
    ASSERT_TRUE(document->getTexts());

    int checkCount = 0;
    document->getOptions()->add("countingOptions",
        std::make_shared<CountingOptions>(document, checkCount));
    EXPECT_EQ(checkCount, 0);

    auto finished = std::move(session).finish();
    EXPECT_EQ(checkCount, 1);
    EXPECT_TRUE(finished->getInstruments().empty());
    EXPECT_THROW((void)std::move(session).finish(), std::logic_error);
}

TEST(DocumentConstructionTest, OptionsFactorySupportsOverlayBeforeFinish)
{
    constexpr static musxtest::string_view xml = R"xml(
<options>
  <musicSpacingOptions>
    <minWidth>12</minWidth>
    <maxWidth>48</maxWidth>
  </musicSpacingOptions>
</options>
    )xml";
    auto xmlDocument = std::make_unique<musx::xml::pugi::Document>();
    xmlDocument->loadFromBuffer(xml.data(), xml.size());

    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();
    document->getOptions() = musx::factory::OptionsFactory::create(
        xmlDocument->getRootElement(), document);
    auto spacing = document->getOptions()->get<musx::dom::options::MusicSpacingOptions>();
    ASSERT_TRUE(spacing);
    const_cast<musx::dom::options::MusicSpacingOptions*>(spacing.get())->minWidth = 24;

    auto finished = std::move(session).finish();
    ASSERT_TRUE(finished->getOptions()->get<musx::dom::options::MusicSpacingOptions>());
    EXPECT_EQ(finished->getOptions()->get<musx::dom::options::MusicSpacingOptions>()->minWidth, 24);
}

TEST(DocumentConstructionTest, DefersContainedIntegrityChecksUntilFinish)
{
    constexpr static musxtest::string_view xml = R"xml(
<others>
  <measNumbRegion cmper="1">
    <scoreData/>
    <partData/>
  </measNumbRegion>
</others>
    )xml";
    auto xmlDocument = std::make_unique<musx::xml::pugi::Document>();
    xmlDocument->loadFromBuffer(xml.data(), xml.size());

    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();
    EXPECT_NO_THROW(document->getOthers() = musx::factory::OthersFactory::create(
        xmlDocument->getRootElement(), document));
    EXPECT_THROW((void)std::move(session).finish(), musx::dom::integrity_error);
}

TEST(DocumentConstructionTest, SupportsNoneAndPartialSharing)
{
    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();

    for (Cmper layerId = 0; layerId < 4; ++layerId) {
        auto score = std::make_shared<musx::dom::others::LayerAttributes>(
            document, SCORE_PARTID, EnigmaBase::ShareMode::All, layerId);
        score->restOffset = static_cast<int>(layerId + 10);
        document->getOthers()->add(musx::dom::others::LayerAttributes::XmlNodeName, score);
        if (layerId == 0) {
            auto part = std::make_shared<musx::dom::others::LayerAttributes>(
                document, 1, EnigmaBase::ShareMode::Partial, layerId);
            musx::factory::PartSharingFactory::initializePartial(
                part, std::shared_ptr<const musx::dom::others::LayerAttributes>(score),
                std::vector<std::string_view>{"restOffset"});
            part->restOffset = 99;
            document->getOthers()->add(musx::dom::others::LayerAttributes::XmlNodeName, part);
        }
    }

    auto unshared = std::make_shared<musx::dom::others::PartGlobals>(
        document, 1, EnigmaBase::ShareMode::None, MUSX_GLOBALS_CMPER);
    document->getOthers()->add(musx::dom::others::PartGlobals::XmlNodeName, unshared);

    auto finished = std::move(session).finish();
    auto scoreLayer = finished->getOthers()->get<musx::dom::others::LayerAttributes>(SCORE_PARTID, 0);
    auto partLayer = finished->getOthers()->get<musx::dom::others::LayerAttributes>(1, 0);
    ASSERT_TRUE(scoreLayer);
    ASSERT_TRUE(partLayer);
    EXPECT_EQ(scoreLayer->restOffset, 10);
    EXPECT_EQ(partLayer->restOffset, 99);
    EXPECT_TRUE(partLayer->getUnlinkedNodes().count("restOffset"));
    EXPECT_EQ(unshared->getShareMode(), EnigmaBase::ShareMode::None);
}

TEST(DocumentConstructionTest, NodeFilterRestrictsPoolToAllowlistedNodes)
{
    constexpr static musxtest::string_view xml = R"xml(
<others>
  <layerAtts cmper="0"><restOffset>7</restOffset></layerAtts>
  <fontName cmper="1"><name>Finale Maestro</name></fontName>
  <layerAtts cmper="1"/>
  <measSpec cmper="1"><width>500</width></measSpec>
  <layerAtts cmper="2"/>
  <layerAtts cmper="3"/>
</others>
    )xml";
    auto xmlDocument = std::make_unique<musx::xml::pugi::Document>();
    xmlDocument->loadFromBuffer(xml.data(), xml.size());

    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();
    document->getOthers() = musx::factory::OthersFactory::create(
        xmlDocument->getRootElement(), document,
        [](const musx::xml::XmlElementPtr& node) {
            return node->getTagName() == musx::dom::others::LayerAttributes::XmlNodeName;
        });

    auto finished = std::move(session).finish();
    EXPECT_EQ(finished->getOthers()->getArray<musx::dom::others::LayerAttributes>(SCORE_PARTID).size(), 4u);
    EXPECT_TRUE(finished->getOthers()->getArray<musx::dom::others::FontDefinition>(SCORE_PARTID).empty());
    EXPECT_TRUE(finished->getOthers()->getArray<musx::dom::others::Measure>(SCORE_PARTID).empty());
    auto layer = finished->getOthers()->get<musx::dom::others::LayerAttributes>(SCORE_PARTID, 0);
    ASSERT_TRUE(layer);
    EXPECT_EQ(layer->restOffset, 7);
}

TEST(DocumentConstructionTest, AbsentNodeFilterCreatesEveryChild)
{
    constexpr static musxtest::string_view xml = R"xml(
<others>
  <layerAtts cmper="0"/>
  <layerAtts cmper="1"/>
  <layerAtts cmper="2"/>
  <layerAtts cmper="3"/>
  <fontName cmper="1"><name>Finale Maestro</name></fontName>
</others>
    )xml";
    auto xmlDocument = std::make_unique<musx::xml::pugi::Document>();
    xmlDocument->loadFromBuffer(xml.data(), xml.size());

    auto session = musx::factory::DocumentFactory::begin();
    auto document = session.getDocument();
    document->getOthers() = musx::factory::OthersFactory::create(
        xmlDocument->getRootElement(), document);

    auto finished = std::move(session).finish();
    EXPECT_EQ(finished->getOthers()->getArray<musx::dom::others::LayerAttributes>(SCORE_PARTID).size(), 4u);
    EXPECT_EQ(finished->getOthers()->getArray<musx::dom::others::FontDefinition>(SCORE_PARTID).size(), 1u);
}
