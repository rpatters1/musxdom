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
        EXPECT_FALSE(doc->getHeader());
        EXPECT_FALSE(doc->getOptions());
        EXPECT_FALSE(doc->getOthers());
    }
}

TEST(DocumentTest, CreateHeader)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <header>
    <headerData>
    </headerData>
  </header>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(emptyHeader);
        EXPECT_TRUE(doc->getHeader());
        EXPECT_FALSE(doc->getOptions());
        EXPECT_FALSE(doc->getOthers());
    }
}

TEST(DocumentTest, CreateOptions)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <options>
  </options>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::tinyxml2::Document>(emptyHeader);
        EXPECT_FALSE(doc->getHeader());
        EXPECT_TRUE(doc->getOptions());
        EXPECT_FALSE(doc->getOthers());
    }
}

TEST(DocumentTest, CreateOthers)
{
    constexpr static musxtest::string_view emptyHeader = R"xml(
<?xml version="1.0" encoding="UTF-8"?>
<finale version="27.4" xmlns="http://www.makemusic.com/2012/finale">
  <others>
  </others>
</finale>
    )xml";
    {
        auto doc = musx::factory::DocumentFactory::create<musx::xml::rapidxml::Document>(emptyHeader);
        EXPECT_FALSE(doc->getHeader());
        EXPECT_FALSE(doc->getOptions());
        EXPECT_TRUE(doc->getOthers());
    }
}
