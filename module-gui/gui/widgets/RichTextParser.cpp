#include "RichTextParser.hpp"
#include "Color.hpp"
#include "Font.hpp"
#include "TextBlock.hpp"
#include "TextDocument.hpp"
#include <sstream>
#include <string>
#include <log/log.hpp>
#include "TextFormat.hpp"

#include <module-utils/pugixml/src/pugixml.hpp>
#include <utility>

#ifndef DEBUG_RTP
const std::string node_types[] = {"null", "document", "element", "pcdata ", "cdata", "comment", "pi", "declaration"};
#define log_parser(...) LOG_DEBUG(__VA_ARGS__)
#endif

namespace text
{
    class Attribute
    {
        std::string name;

      protected:
        explicit Attribute(std::string name) : name(std::move(name)){};

      public:
        Attribute() = delete;
        /// return true on success, othervise set fallback value and return false
        /// @note could run: preVisitHook -> visit -> postVisit hook
        virtual auto visit(gui::TextFormat &fmt, std::string value) -> bool = 0;
        [[nodiscard]] auto getName() const -> const std::string &
        {
            return name;
        }

        [[nodiscard]] auto is(const std::string &name) const
        {
            return getName() == name;
        }
    };

    class AttributeAlign : public Attribute
    {
        const std::string center = gui::text::center;
        const std::string left   = gui::text::left;
        const std::string right  = gui::text::right;

      public:
        AttributeAlign() : Attribute(gui::text::align)
        {}

        auto visit(gui::TextFormat &fmt, std::string value) -> bool final
        {
            log_parser("append: Attr %s", getName().c_str());
            using namespace gui;
            auto alignment = Alignment(Alignment::Horizontal::Left, Alignment::Vertical::Bottom);
            if (value == center) {
                alignment.horizontal = Alignment::Horizontal::Center;
                fmt.setAlignment(alignment);
                return true;
            }
            else if (value == right) {
                alignment.horizontal = Alignment::Horizontal::Right;
                fmt.setAlignment(alignment);
                return true;
            }
            else if (value == left) {
                alignment.horizontal = Alignment::Horizontal::Left;
                fmt.setAlignment(alignment);
                return true;
            }

            fmt.setAlignment(alignment);
            return false;
        }
    };

    class AttributeColor : public Attribute
    {

        const int max = 15;

      public:
        AttributeColor() : Attribute(gui::text::color)
        {}

        auto visit(gui::TextFormat &fmt, std::string value) -> bool final
        {
            log_parser("append: Attr %s", getName().c_str());
            using namespace gui;
            if (value.empty()) {
                fmt.setColor(ColorFullBlack);
                return false;
            }
            try {
                int val = std::stoi(value);
                if ((val == 0 && value.c_str()[0] != '0') || val > max) {
                    fmt.setColor(ColorFullBlack);
                    return false;
                }
                fmt.setColor(Color{static_cast<uint8_t>(val), 0});
                return true;
            }
            catch (const std::exception &exception) {
                LOG_ERROR("%s", exception.what());
                return false;
            }
            return false;
        }
    };

    class AttributeFont : public Attribute
    {
      public:
        AttributeFont() : Attribute(gui::text::font)
        {}

        auto visit(gui::TextFormat &fmt, std::string value) -> bool final
        {
            log_parser("append: Attr %s", getName().c_str());
            using namespace gui;
            // here it's tricky -> we need to get font copy from fmt -> change it -> set it
            auto font = gui::Font(fmt.getFont());
            font.setFont(value, font.getSize());
            fmt.setFont(font.raw());
            return true;
        }
    };

    class AttributeSize : public Attribute
    {
      public:
        AttributeSize() : Attribute(gui::text::size)
        {}

        auto visit(gui::TextFormat &fmt, std::string value) -> bool final
        {
            log_parser("append: Attr %s", getName().c_str());
            if (value.empty()) {
                return false;
            }
            try {
                int val = std::stoi(value);

                if (val == 0 && value.front() != '0') {
                    return false;
                }

                auto font = gui::Font(fmt.getFont());
                font.setSize(val);
                fmt.setFont(font.raw());
                return true;
            }
            catch (const std::exception &exception) {
                LOG_ERROR("%s", exception.what());
                return false;
            }
        }
    };

    class AttributeWeight : public Attribute
    {
        const std::string regular = "regular";
        const std::string bold    = "bold";
        const std::string light   = "light";

      public:
        AttributeWeight() : Attribute("weight")
        {}

        auto visit(gui::TextFormat &fmt, std::string value) -> bool final
        {
            log_parser("append: Attr %s", getName().c_str());
            using namespace gui;
            auto font = gui::Font(fmt.getFont());
            if (value == regular) {
                font.setWeight(Font::Weight::Regular);
            }
            else if (value == light) {
                font.setWeight(Font::Weight::Light);
            }
            else if (value == bold) {
                font.setWeight(Font::Weight::Bold);
            }
            else {
                font.setWeight(Font::Weight::Regular);
                fmt.setFont(font.raw());
                return false;
            }

            fmt.setFont(font.raw());
            return true;
        }
    };

    class NodeDecor
    {
        std::list<std::unique_ptr<Attribute>> attrs;
        NodeDecor()
        {
            attrs.emplace_back(std::make_unique<AttributeAlign>());
            attrs.emplace_back(std::make_unique<AttributeColor>());
            attrs.emplace_back(std::make_unique<AttributeFont>());
            attrs.emplace_back(std::make_unique<AttributeSize>());
            attrs.emplace_back(std::make_unique<AttributeWeight>());
        }

      public:
        // for each met style -> put it on stack to be used
        // too deep -> can be optimized
        auto stack_visit(gui::TextFormat &format, const std::string &name, const std::string &value) -> bool
        {
            for (auto &attr : attrs) {
                if (attr->is(name)) {
                    if (!attr->visit(format, value)) {
                        LOG_ERROR("Attribute %s parsing error, default set", name.c_str());
                    }
                    return true;
                }
            }
            LOG_ERROR("Attr: %s not found", name.c_str());
            return false;
        }

        static auto get() -> NodeDecor &
        {
            static NodeDecor *ptr = nullptr;
            if (ptr == nullptr) {
                ptr = new NodeDecor();
            }
            return *ptr;
        }
    };
}; // namespace text

struct walker : pugi::xml_tree_walker
{
  protected:
    std::list<gui::TextBlock> blocks;
    std::list<gui::TextFormat> style_stack;
    bool add_newline = false;

  public:
    walker(gui::TextFormat entry_style)
    {
        style_stack.push_back(entry_style);
    }

    enum class Action
    {
        Enter, /// enter/visit node
        Exit   /// exit/leave  node
    };

    auto log_node(pugi::xml_node &node, Action dir)
    {
        log_parser(
            "%s: %s format: %s",
            dir == Action::Enter ? "enter" : "leave",
            [&]() {
                std::stringstream ss;
                ss << node_types[node.type()] << ": name='" << node.name() << "', value='" << node.value() << "'";
                return ss.str();
            }()
                .c_str(),
            style_stack.back().str().c_str());
    }

    auto is_newline_node(pugi::xml_node &node) const
    {
        return std::string(node.name()) == gui::text::node_br || std::string(node.name()) == gui::text::node_p;
    }

    auto push_text_node(pugi::xml_node &node)
    {
        auto local_style = style_stack.back();
        for (auto &attribute : node.attributes()) {
            log_parser("attribute name: %s value: %s", attribute.name(), attribute.value());
            auto &decor = text::NodeDecor::get();
            decor.stack_visit(local_style, attribute.name(), attribute.value());
        }
        style_stack.push_back(local_style);
        log_parser("Attr loaded: %s", style_stack.back().str().c_str());
    }

    auto push_newline_node(pugi::xml_node &)
    {
        if (blocks.size() != 0u) {
            blocks.back().setEnd(gui::TextBlock::End::Newline);
        }
    }

    auto push_data_node(pugi::xml_node &node)
    {
        blocks.emplace_back(node.value(), std::make_unique<gui::TextFormat>(style_stack.back()));
    }

    auto for_each(pugi::xml_node &node) -> bool final
    {
        log_node(node, Action::Enter);
        if (node.type() == pugi::xml_node_type::node_element) {
            if (std::string(node.name()) == gui::text::node_text) {
                push_text_node(node);
                return true;
            }
            if (is_newline_node(node)) {
                push_newline_node(node);
                return true;
            }
        }

        std::string to_show = node.value();
        if (node.type() == pugi::xml_node_type::node_pcdata && !to_show.empty()) {
            push_data_node(node);
        }
        return true;
    }

    auto pop_text_node(pugi::xml_node &node)
    {
        style_stack.pop_back();
    }

    auto pop_newline_node(pugi::xml_node &node)
    {
        if (blocks.size() != 0u) {
            blocks.back().setEnd(gui::TextBlock::End::Newline);
        }
    }

    auto on_leave(pugi::xml_node &node) -> bool final
    {
        log_node(node, Action::Exit);

        if (node.type() == pugi::xml_node_type::node_element) {
            if (std::string(node.name()) == gui::text::node_text) {
                pop_text_node(node);
                return true;
            }
            if (is_newline_node(node)) {
                pop_newline_node(node);
                return true;
            }
        }
        return true;
    }

    auto end(pugi::xml_node &node) -> bool final
    {
        return true;
    }

    auto souvenirs() -> std::list<gui::TextBlock> &
    {
        return blocks;
    }
};

namespace gui::text
{

    auto RichTextParser::parse(const UTF8 &text, TextFormat *base_style) -> std::unique_ptr<TextDocument>
    {
        LOG_DEBUG("parsing: %s", text.c_str());
        if (text.empty() || base_style == nullptr) {
            LOG_ERROR("no: %s", text.empty() ? "text" : "base style");
            return std::unique_ptr<TextDocument>();
        }

        pugi::xml_document doc;
        walker walker(*base_style);

        doc.load_string(text.c_str());
        doc.traverse(walker);

        return std::make_unique<TextDocument>(walker.souvenirs());
    }

}; // namespace gui::text
