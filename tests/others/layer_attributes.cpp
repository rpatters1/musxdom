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

constexpr static musxtest::string_view layerAttributesXml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <layerAtts cmper="0">
      <restOffset>3</restOffset>
      <flipTies/>
      <floatLayer/>
      <useRestOff/>
      <freezeLayUp/>
      <freezeLayer/>
      <playback/>
      <spacing/>
      <ignoreHidden/>
    </layerAtts>
    <layerAtts cmper="1">
      <restOffset>-4</restOffset>
      <flipTies/>
      <floatLayer/>
      <useRestOff/>
      <freezeLayer/>
      <playback/>
      <spacing/>
      <ignoreHidden/>
    </layerAtts>
    <layerAtts cmper="2">
      <hideLayer/>
      <freezeLayer/>
      <playback/>
      <ignoreHidden/>
    </layerAtts>
    <layerAtts cmper="3"/>
  </others>
</finale>
    )xml";

TEST(LayerAttributesTest, PopulateLayerAttributes)
{
    // Parse the XML into a Document
    auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(layerAttributesXml);
    auto others = doc->getOthers();
    ASSERT_TRUE(others);

    // Test LayerAttributes for each cmper value
    for (int i = 0; i < 4; ++i)
    {
        auto layerAttributes = others->get<musx::dom::others::LayerAttributes>(i);
        ASSERT_TRUE(layerAttributes);

        if (i == 0)
        {
            EXPECT_EQ(layerAttributes->restOffset, 3);
            EXPECT_TRUE(layerAttributes->freezTiesToStems);
            EXPECT_TRUE(layerAttributes->onlyIfOtherLayersHaveNotes);
            EXPECT_TRUE(layerAttributes->useRestOffset);
            EXPECT_TRUE(layerAttributes->freezeStemsUp);
            EXPECT_TRUE(layerAttributes->freezeLayer);
            EXPECT_TRUE(layerAttributes->playback);
            EXPECT_TRUE(layerAttributes->affectSpacing);
            EXPECT_TRUE(layerAttributes->ignoreHiddenNotesOnly);
        }
        else if (i == 1)
        {
            EXPECT_EQ(layerAttributes->restOffset, -4);
            EXPECT_TRUE(layerAttributes->freezTiesToStems);
            EXPECT_TRUE(layerAttributes->onlyIfOtherLayersHaveNotes);
            EXPECT_TRUE(layerAttributes->useRestOffset);
            EXPECT_FALSE(layerAttributes->freezeStemsUp); // Not set in XML
            EXPECT_TRUE(layerAttributes->freezeLayer);
            EXPECT_TRUE(layerAttributes->playback);
            EXPECT_TRUE(layerAttributes->affectSpacing);
            EXPECT_TRUE(layerAttributes->ignoreHiddenNotesOnly);
        }
        else if (i == 2)
        {
            EXPECT_EQ(layerAttributes->restOffset, 0); // Default value
            EXPECT_FALSE(layerAttributes->freezTiesToStems);
            EXPECT_FALSE(layerAttributes->onlyIfOtherLayersHaveNotes);
            EXPECT_FALSE(layerAttributes->useRestOffset);
            EXPECT_FALSE(layerAttributes->freezeStemsUp);
            EXPECT_TRUE(layerAttributes->freezeLayer);
            EXPECT_TRUE(layerAttributes->playback);
            EXPECT_FALSE(layerAttributes->affectSpacing); // Not set in XML
            EXPECT_TRUE(layerAttributes->ignoreHiddenNotesOnly);
            EXPECT_TRUE(layerAttributes->hideLayer);
        }
        else if (i == 3)
        {
            EXPECT_EQ(layerAttributes->restOffset, 0); // Default value
            EXPECT_FALSE(layerAttributes->freezTiesToStems);
            EXPECT_FALSE(layerAttributes->onlyIfOtherLayersHaveNotes);
            EXPECT_FALSE(layerAttributes->useRestOffset);
            EXPECT_FALSE(layerAttributes->freezeStemsUp);
            EXPECT_FALSE(layerAttributes->freezeLayer);
            EXPECT_FALSE(layerAttributes->playback);
            EXPECT_FALSE(layerAttributes->affectSpacing);
            EXPECT_FALSE(layerAttributes->ignoreHiddenNotesOnly);
            EXPECT_FALSE(layerAttributes->hideLayer);
        }
    }
}

TEST(LayerAttributesTest, WrongNumberOfLayers)
{
  constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <layerAtts cmper="0"/>
    <layerAtts cmper="1"/>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
        std::invalid_argument
    );

    constexpr static musxtest::string_view xml2 = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <layerAtts cmper="0"/>
    <layerAtts cmper="1"/>
    <layerAtts cmper="2"/>
    <layerAtts cmper="3"/>
    <layerAtts cmper="4"/>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml2),
        std::invalid_argument
    );
}

TEST(LayerAttributesTest, WrongCmperVals)
{
  constexpr static musxtest::string_view xml = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <layerAtts cmper="0"/>
    <layerAtts cmper="2"/>
    <layerAtts cmper="1"/>
    <layerAtts cmper="4"/>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml),
        std::invalid_argument
    );
    
    constexpr static musxtest::string_view xml2 = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale>
  <others>
    <layerAtts cmper="1"/>
    <layerAtts cmper="2"/>
    <layerAtts cmper="3"/>
    <layerAtts cmper="4"/>
  </others>
</finale>
    )xml";

    EXPECT_THROW(
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(xml2),
        std::invalid_argument
    );
}
