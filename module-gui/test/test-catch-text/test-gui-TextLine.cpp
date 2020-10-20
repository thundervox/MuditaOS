// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "InitializedFontManager.hpp"
#include <catch2/catch.hpp>
#include <limits>
#include <memory>
#include <sstream>
#include <limits>
#include <module-gui/gui/widgets/TextLine.hpp>

#include <mock/buildTextDocument.hpp>
#include <mock/multi-line-string.hpp>
#include <mock/BlockFactory.hpp>
#include <module-gui/gui/widgets/Text.hpp>
#include <Text.hpp>

const auto maxwidth = std::numeric_limits<unsigned int>().max();

TEST_CASE("TextLine - ctor")
{
    using namespace gui;

    SECTION("no document")
    {
        Text text;
        auto cursor = new TextCursor(&text);
        auto line   = TextLine(*cursor, maxwidth);
        REQUIRE(line.length() == 0);
    }

    SECTION("first line of multiline - all fits in")
    {
        auto texts            = mockup::lineStrings(3);
        auto [document, font] = mockup::buildMultilineTestDocument(texts);

        Text text;
        text.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&text);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() > 0);
        REQUIRE(line.length() == texts.front().length());
    }

    SECTION("oneline line - all fits in")
    {
        auto test_text        = mockup::multiWordString(5);
        auto [document, font] = mockup::buildOnelineTestDocument(test_text);

        Text text;
        text.setText(test_text);

        auto cursor = new TextCursor(&text);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() > 0);
        REQUIRE(line.length() == test_text.length());
    }
}

TEST_CASE("TextLine - non fitting text")
{
    SECTION("text > one line to show")
    {
        auto test_text        = mockup::multiWordString(5);
        auto [document, font] = mockup::buildOnelineTestDocument(test_text);

        gui::Text text;
        text.setText(std::make_unique<gui::TextDocument>(document));
        auto cursor = new gui::TextCursor(&text);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>::max());

        REQUIRE(line.length() != 0);
        REQUIRE(line.length() <= test_text.length());
    }
}

TEST_CASE("TextLine - multiple styles text")
{
    using namespace gui;
    mockup::fontManager();

    auto getTextLen = [](auto blocks) -> unsigned int {
        unsigned int ret = 0;
        for (auto el : blocks) {
            ret += el.length();
        }
        return ret;
    };

    SECTION("multi - style text, one paragraph")
    {
        auto testblock = mockup::getBlock(mockup::BlockFactory::Type::Block0);
        auto document  = TextDocument(testblock);

        gui::Text txt;
        txt.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&txt);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.width() > 0);
        REQUIRE(line.length() == getTextLen(testblock));
    }

    SECTION("multi - style text, two paragraphs, first paragraph")
    {
        auto testblock = mockup::getBlock(mockup::BlockFactory::Type::Block0);
        auto block1    = mockup::getBlock(mockup::BlockFactory::Type::Block1);
        auto block0    = mockup::getBlock(mockup::BlockFactory::Type::Block0);

        testblock.insert(testblock.end(), block1.begin(), block1.end());
        auto document = TextDocument(testblock);

        gui::Text txt;
        txt.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&txt);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() == getTextLen(block0));
    }

    SECTION("multi - style text, two paragraphs, second paragraph")
    {

        auto testblock = mockup::getBlock(mockup::BlockFactory::Type::Block0);
        auto block1    = mockup::getBlock(mockup::BlockFactory::Type::Block1);
        auto block0    = mockup::getBlock(mockup::BlockFactory::Type::Block0);

        testblock.insert(testblock.end(), block1.begin(), block1.end());
        auto document = TextDocument(testblock);

        gui::Text txt;
        txt.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&txt, 0, block0.size());
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() == getTextLen(block1));
    }

    SECTION("multi - empty text, end newline")
    {
        auto testblock = mockup::getBlock(mockup::BlockFactory::Type::NoneBlock0);
        auto document  = TextDocument(testblock);

        gui::Text txt;
        txt.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&txt);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() == 0);
        REQUIRE(line.width() == 0);
    }

    SECTION("multi - empty text, end none")
    {
        auto testblock = mockup::getBlock(mockup::BlockFactory::Type::NoneBlock1);
        auto document  = TextDocument(testblock);

        gui::Text txt;
        txt.setText(std::make_unique<TextDocument>(document));
        auto cursor = new TextCursor(&txt);
        auto line   = gui::TextLine(*cursor, std::numeric_limits<unsigned int>().max());

        REQUIRE(line.length() == 0);
        REQUIRE(line.width() == 0);
    }
}

TEST_CASE("TextLine - elements sizes checkup")
{
    using namespace gui;

    auto testblock = mockup::getBlock(mockup::BlockFactory::Type::Block0);
    auto document  = TextDocument(testblock);

    Text text;
    text.setText(std::make_unique<TextDocument>(testblock));
    auto cursor    = new TextCursor(&text);
    auto text_line = TextLine(*cursor, maxwidth);

    REQUIRE(text_line.length() > 0);
    const Item *it = nullptr;
    for (unsigned int cnt = 0; cnt < text_line.count(); ++cnt) {
        it = text_line.getElement(cnt);
        REQUIRE(it != nullptr);
        REQUIRE(it->getWidth() > 0);
        REQUIRE(it->getHeight() > 0);
    }
}
