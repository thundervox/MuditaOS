#pragma once

#include <string>
#include <list>

#include "../core/BoundingBox.hpp"
#include "../core/Color.hpp"
#include "../core/DrawCommand.hpp"
#include "../Common.hpp"
#include "Margins.hpp"
#include "Alignment.hpp"
#include "Rect.hpp"

#include "Style.hpp"
#include "utf8/UTF8.hpp"
#include <Ellipsis.hpp>

namespace gui
{

    class RawFont;

    namespace meta
    {
        struct Meta
        {};

        struct Item : public Meta
        {
            uint32_t radius = 0;
        };

        struct Rect : public Item
        {
            Rect &operator()(std::array<uint32_t, 4> xywh)
            {
                x = xywh[0], y = xywh[1], w = xywh[2], h = xywh[3];
                return *this;
            }
            uint32_t x = 0, y = 0, w = 0, h = 0;
            std::string font = style::window::font::medium;
            uint32_t align   = gui::Alignment::ALIGN_HORIZONTAL_LEFT | gui::Alignment::ALIGN_VERTICAL_CENTER;
            RectangleEdgeFlags edges =
                gui::RectangleEdgeFlags::GUI_RECT_EDGE_TOP | gui::RectangleEdgeFlags::GUI_RECT_EDGE_BOTTOM;
        };

        struct Label : public Rect
        {
            const UTF8 text;
            Margins margins;
            Label(std::array<uint32_t, 4> xywh)
            {
                operator()(xywh);
            }
        };
    }; // namespace meta

    class Label : public Rect
    {
      protected:
        UTF8 text;
        UTF8 textDisplayed;
        Ellipsis ellipsis          = gui::Ellipsis::Right;
        uint32_t charDrawableCount = 0;
        uint32_t stringPixelWidth  = 0;
        Color textColor            = {0, 0};
        RawFont *font              = nullptr;
        Margins margins            = {0, 0, 0, 0};
        bool lineMode              = true; // TODO PLZ REMOVE - this was working by accident (in Phonebook)
        Alignment alignment;

        // area specified in pixels occupied by text inside label space.
        // This defines also position of the text considering alignment and margins.
        BoundingBox textArea;
        // widgets to add line
        Rect *lineFront = nullptr;
        Rect *lineBack = nullptr;
        void calculateDisplayText();

      private:
        /// helper class storing 2 information from c function with bad interface:
        /// 1. whether text will fit in size
        /// 2. how much provided size will be in use
        struct Fits
        {
            bool fits               = false; /// whether element fits in or not
            uint32_t space_consumed = 0;     /// size needed to render text
            Fits(bool fits = false, uint32_t space_consumed = 0) : fits(fits), space_consumed(space_consumed)
            {}
            operator bool()
            {
                return fits;
            }
        };
        /// check if text will fit in Label
        Fits textFitsIn(const UTF8 &text, uint32_t width);
        /// resize widget width ( 0 <= size needed to render text <= max width )
        /// @note it doesn't call calculateDisplay text which should probably be in resize callback (onDimensionChanged)
        void fitTextIn(const UTF8 &text);

      public:
        Label();
        Label(Item *parent,
              const uint32_t &x = 0,
              const uint32_t &y = 0,
              const uint32_t &w = 0,
              const uint32_t &h = 0,
              const UTF8 &text  = UTF8{});
        Label(Item *parent, meta::Label label);

        // Label's specific methods
        virtual void setText(const UTF8 &text);
        virtual void clear();
        virtual const UTF8 &getText() const;
        virtual unsigned int getTextLength() const;
        virtual void setAlignment(const Alignment &alignment);
        auto getAlignment() -> auto const &
        {
            return alignment;
        }
        virtual void setMargins(const Margins &margins);
        void setEllipsis(gui::Ellipsis ellipsis);
        /**
         * @brief Defines if remaining area of the label has a horizontal line.
         */
        void setLineMode(const bool &val);
        void setTextColor(Color color);

        void setFont(const UTF8 &fontName);
        void setFont(RawFont *font);
        RawFont *getFont() const;
        // virtual methods
        std::list<DrawCommand *> buildDrawList() override;
        uint32_t getTextNeedSpace() const;
        /// line: height
        uint32_t getTextHeight() const;

        bool onDimensionChanged(const BoundingBox &oldDim, const BoundingBox &newDim) override;
    };

} /* namespace gui */
