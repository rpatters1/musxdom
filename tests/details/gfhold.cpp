/*
 * Copyright (C) 2024, Robert Patterson
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

TEST(GFrameHoldTest, PopulateFields)
{
    constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <gfhold cmper1="3" cmper2="915">
      <clefID>0</clefID>
      <clefMode>forced</clefMode>
      <clefPercent>75</clefPercent>
      <frame1>21240</frame1>
    </gfhold>
    <gfhold cmper1="3" cmper2="1083">
      <clefID>3</clefID>
      <clefMode>hidden</clefMode>
      <clefPercent>75</clefPercent>
      <frame1>22464</frame1>
      <frame3>22465</frame3>
    </gfhold>
    <gfhold cmper1="3" cmper2="1129">
      <clefListID>1234</clefListID>
      <clefPercent>75</clefPercent>
      <mirrorFrame/>
      <frame4>22911</frame4>
    </gfhold>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    // Test GFrameHold for cmper1=3, cmper2=915
    {
        auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 3, 915);
        ASSERT_TRUE(gfhold);

        EXPECT_EQ(gfhold->clefId.value_or(-1), 0); // Default to -1 if not set
        EXPECT_EQ(gfhold->clefListId, 0); // Default to zero
        EXPECT_EQ(gfhold->showClefMode, details::GFrameHold::ShowClefMode::Always);
        EXPECT_EQ(gfhold->clefPercent, 75);
        EXPECT_FALSE(gfhold->mirrorFrame);
        EXPECT_EQ(gfhold->frames[0], 21240);
        EXPECT_EQ(gfhold->frames[1], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[2], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[3], 0);  // Not present, should be default
    }

    // Test GFrameHold for cmper1=3, cmper2=1083
    {
        auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 3, 1083);
        ASSERT_TRUE(gfhold);

        EXPECT_EQ(gfhold->clefId.value_or(-1), 3); // Default to -1 if not set
        EXPECT_EQ(gfhold->clefListId, 0); // Default to zero
        EXPECT_EQ(gfhold->showClefMode, details::GFrameHold::ShowClefMode::Never);
        EXPECT_EQ(gfhold->clefPercent, 75);
        EXPECT_FALSE(gfhold->mirrorFrame);
        EXPECT_EQ(gfhold->frames[0], 22464);
        EXPECT_EQ(gfhold->frames[1], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[2], 22465);
        EXPECT_EQ(gfhold->frames[3], 0);  // Not present, should be default
    }

    // Test GFrameHold for cmper1=3, cmper2=1129
    {
        auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 3, 1129);
        ASSERT_TRUE(gfhold);

        EXPECT_FALSE(gfhold->clefId.has_value());
        EXPECT_EQ(gfhold->clefListId, 1234); // Default to zero
        EXPECT_EQ(gfhold->showClefMode, details::GFrameHold::ShowClefMode::WhenNeeded);
        EXPECT_EQ(gfhold->clefPercent, 75);
        EXPECT_TRUE(gfhold->mirrorFrame);
        EXPECT_EQ(gfhold->frames[0], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[1], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[2], 0);  // Not present, should be default
        EXPECT_EQ(gfhold->frames[3], 22911);
    }
}

TEST(GFrameHoldTest, IntegrityCheck)
{
    constexpr static musxtest::string_view xmlBothClefs = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <gfhold cmper1="3" cmper2="915">
      <clefID>0</clefID>
      <clefListID>123</clefListID>
      <clefMode>forced</clefMode>
      <clefPercent>75</clefPercent>
      <frame1>21240</frame1>
    </gfhold>
  </details>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xmlBothClefs),
        musx::dom::integrity_error
    ) << "clef and clef list both specified";

constexpr static musxtest::string_view xmlNoClefs = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <details>
    <gfhold cmper1="3" cmper2="915">
      <clefMode>forced</clefMode>
      <clefPercent>75</clefPercent>
      <frame1>21240</frame1>
    </gfhold>
  </details>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xmlNoClefs),
        musx::dom::integrity_error
    ) << "neither clef nor clef list specified";

    constexpr static musxtest::string_view xmlNotIterable = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <frameSpec cmper="1" inci="0">
      <startEntry>1</startEntry>
      <endEntry>2</endEntry>
    </frameSpec>
  </others>
  <details>
    <gfhold cmper1="3" cmper2="915">
      <clefID>0</clefID>
      <clefMode>forced</clefMode>
      <clefPercent>75</clefPercent>
      <frame1>1</frame1>
    </gfhold>
  </details>
</finale>
    )xml";

    auto doc = musx::factory::DocumentFactory::create<musx::xml::pugi::Document>(xmlNotIterable);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 3, 915);
    ASSERT_TRUE(gfhold);

    EXPECT_THROW(
        gfhold->iterateEntries([](const auto&) -> bool { return false; }),
        musx::dom::integrity_error
    ) << "gfhold not iterable";

}

TEST(GFrameHold, IterationTest)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "layers.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, 2);
    ASSERT_TRUE(gfhold);
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        auto entry = entryInfo->getEntry();
        EXPECT_TRUE(entryInfo->getLayerIndex() == 0 || entryInfo->getLayerIndex() == 1) << "unexpected layer index " << entryInfo->getLayerIndex();
        if (entryInfo->getLayerIndex() == 0) {
            EXPECT_EQ(entry->duration, Edu(Entry::NoteType::Whole)) << "unexpected note duration " << entry->duration;
            EXPECT_TRUE(entry->isNote) << "layerIndex 0 entry is not a note";
        } else {
            EXPECT_EQ(entry->duration, Edu(Entry::NoteType::Half)) << "unexpected note duration " << entry->duration;
        }
        return true;
    });

    gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 2, 1);
    ASSERT_TRUE(gfhold);
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        auto entry = entryInfo->getEntry();
        EXPECT_TRUE(entryInfo->getLayerIndex() == 2) << "unexpected layer index " << entryInfo->getLayerIndex();
        EXPECT_EQ(entry->duration, Edu(Entry::NoteType::Whole)) << "unexpected note duration " << entry->duration;
        EXPECT_TRUE(entry->isNote) << "layerIndex 0 entry is not a note";
        return true;
    });
}

TEST(GFrameHold, QuintupletTest)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "quintuplet.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<Fraction> expectedValues = {
      Fraction(1, 10), Fraction(1, 10), Fraction(1, 10), Fraction(1, 10), Fraction(1, 10), Fraction(1, 2)
    };

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    size_t x = 0;
    Fraction total = 0;
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        EXPECT_LT(x, expectedValues.size()) << "too few expected values";
        if (x >= expectedValues.size()) return false;
        EXPECT_EQ(expectedValues[x], entryInfo->actualDuration);
        EXPECT_EQ(total, entryInfo->elapsedDuration);
        total += expectedValues[x++];
        return true;
    });
}

TEST(GFrameHold, TripletTest)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "triplet.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<Fraction> expectedValues = {
      Fraction(1, 6), Fraction(1, 6), Fraction(1, 6), Fraction(1, 2)
    };

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    size_t x = 0;
    Fraction total = 0;
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        EXPECT_LT(x, expectedValues.size()) << "too few expected values";
        if (x >= expectedValues.size()) return false;
        EXPECT_EQ(expectedValues[x], entryInfo->actualDuration);
        EXPECT_EQ(total, entryInfo->elapsedDuration);
        total += expectedValues[x++];
        return true;
    });
}

TEST(GFrameHold, NestedTupletTest)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "nested_tuplets.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<Fraction> expectedValues = {
        Fraction(1, 2),
        Fraction(1, 18), Fraction(1, 18), Fraction(1, 18),
        Fraction(1, 18), Fraction(1, 18), Fraction(1, 18),
        Fraction(1, 12), Fraction(1, 12)
    };

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    size_t x = 0;
    Fraction total = 0;
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        EXPECT_LT(x, expectedValues.size()) << "too few expected values";
        if (x >= expectedValues.size()) return false;
        EXPECT_EQ(expectedValues[x], entryInfo->actualDuration);
        EXPECT_EQ(total, entryInfo->elapsedDuration);
        total += expectedValues[x++];
        return true;
    });
}

TEST(GFrameHold, V1V2TupletTest)
{
    std::vector<char> xml;
    musxtest::readFile(musxtest::getInputPath() / "v1v2tuplets.enigmaxml", xml);
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml);
    ASSERT_TRUE(doc);

    auto details = doc->getDetails();
    ASSERT_TRUE(details);

    std::vector<Fraction> expectedValues = {
        Fraction(1, 3),
        Fraction(1, 12), Fraction(1, 12), Fraction(1, 12), Fraction(1, 8), Fraction(1, 8),
        Fraction(0), Fraction(1, 6),
        Fraction(1, 6), Fraction(1, 3),
        Fraction(0), Fraction(1, 12), Fraction(1, 12), Fraction(1, 12), Fraction(1, 4),
    };

    auto gfhold = details->get<details::GFrameHold>(SCORE_PARTID, 1, 1);
    ASSERT_TRUE(gfhold);
    size_t x = 0;
    Fraction v1Total = 0;
    Fraction v2Total = 0;
    gfhold->iterateEntries([&](const auto& entryInfo) -> bool {
        EXPECT_LT(x, expectedValues.size()) << "too few expected values";
        if (x >= expectedValues.size()) return false;
        EXPECT_EQ(expectedValues[x], entryInfo->actualDuration);
        if (entryInfo->v2Launch) {
          v2Total = v1Total;
        }
        Fraction& total = entryInfo->getEntry()->voice2 ? v2Total : v1Total;
        EXPECT_EQ(total, entryInfo->elapsedDuration);
        total += expectedValues[x++];
    /*
        std::cout << entryInfo->elapsedDuration << '\t' << entryInfo->actualDuration << '\t'
                        << std::to_string(entryInfo->elapsedDuration.calcDuration()) << '\t'
                        << std::to_string(entryInfo->actualDuration.calcDuration()) << std::endl;
    */
        return true;
    });
}
