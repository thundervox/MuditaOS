#include "log/log.hpp"
#include "utf8/UTF8.hpp"

#include "../core/DrawCommand.hpp"

#include "Label.hpp"
#include <Style.hpp>
#include <cassert>
#include "TextConstants.hpp"
#include <FontManager.hpp>
#include <RawFont.hpp>

namespace gui
{

    Label::Label()
        : Rect(), text{""}, textDisplayed{""}, charDrawableCount{0}, stringPixelWidth{0}, textColor{0, 0}, font{nullptr}
    {
        setFont(style::window::font::medium);
    }

    Label::Label(
        Item *parent, const uint32_t &x, const uint32_t &y, const uint32_t &w, const uint32_t &h, const UTF8 &newText)
        : Rect{parent, x, y, w, h},
          text(newText), textDisplayed{""}, charDrawableCount{0}, stringPixelWidth{0}, textColor{0, 0}
    {
        setFont(style::window::font::medium);
    }

    Label::Label(Item *parent, meta::Label label) : Label(parent, label.x, label.y, label.w, label.h, label.text)
    {
        setPenFocusWidth(style::window::default_border_no_focus_w);
        setPenWidth(style::window::default_border_no_focus_w);
        setFont(label.font);
        setEllipsis(Ellipsis::Right);
        setAlignment(label.align);
        setRadius(label.radius);
        setEdges(label.edges);
    }

    void Label::calculateDisplayText()
    {
        uint32_t availableSpace = widgetArea.w;
        if (font == nullptr) {
            LOG_ERROR("No font loaded!");
            return;
        }
        charDrawableCount = font->getCharCountInSpace(text, availableSpace);
        textArea.w        = font->getPixelWidth(text.substr(0, charDrawableCount));
        textDisplayed     = text;
        /// do not try to draw text::newline in label
        if (text.length() > 0 && text[text.length() - 1] == text::newline) {
            textDisplayed.removeChar(textDisplayed.length() - 1);
        }
        textDisplayed    = font->getTextWithElipsis(textDisplayed, availableSpace, ellipsis);
        stringPixelWidth = font->getPixelWidth(textDisplayed, 0, textDisplayed.length());
        textArea.h       = font->info.line_height;

        // calculate vertical position of text

        switch (alignment.vertical) {
        case (Alignment::Vertical::Center):
            textArea.y = (widgetArea.h - font->info.line_height) / 2 + font->info.base;
            break;
        case Alignment::Vertical::Top:
            textArea.y = font->info.base + margins.top;
            break;
        case Alignment::Vertical::Bottom:
            textArea.y = widgetArea.h - font->info.line_height + font->info.base - margins.bottom;
            break;
        default:
            break;
        }

        switch (alignment.horizontal) {
        case (Alignment::Horizontal::Center):
            textArea.x = (widgetArea.w - textArea.w) / 2;
            break;
        case Alignment::Horizontal::Left:
            textArea.x = margins.left;
            break;
        case Alignment::Horizontal::Right:
            textArea.x = widgetArea.w - textArea.w - margins.right;
            break;
        default:
            break;
        }

        // if dots mode is disabled and line mode is enabled calculate positiona and width of the line
        if ((ellipsis != Ellipsis::None) && (lineMode) && (lineFront != nullptr)) {
            uint32_t spaceWidth = font->getCharPixelWidth(' ');
            int32_t lineW       = availableSpace - stringPixelWidth;
            uint32_t lineY      = textArea.y - font->getCharPixelHeight('a') / 2;
            if (lineW < 0)
                lineW = 0;

            lineFront->setVisible(true);
            lineBack->setVisible(true);
            // both lines are visible

            switch (alignment.horizontal) {
            case (Alignment::Horizontal::Center):
                lineFront->setPosition(0, lineY);
                lineFront->setSize(lineW / 2 - spaceWidth, 2);
                lineBack->setPosition(lineW / 2 + stringPixelWidth + spaceWidth, lineY);
                lineBack->setSize(lineW / 2 - spaceWidth, 2);
                break;
            case Alignment::Horizontal::Right:
                lineFront->setPosition(0, lineY);
                lineFront->setSize(lineW - spaceWidth, 2);
                lineBack->setVisible(false);
                break;
            case Alignment::Horizontal::Left:
                lineBack->setPosition(stringPixelWidth + spaceWidth, lineY);
                lineBack->setSize(lineW - spaceWidth, 2);
                lineFront->setVisible(false);
                break;
            default:
                break;
            }
        }
    }

    void Label::setText(const UTF8 &text)
    {
        this->text = text;
        auto fits  = textFitsIn(text, area(Area::Normal).w);
        if (!fits) {
            fitTextIn(text);
        }
        calculateDisplayText();
    }

    Label::Fits Label::textFitsIn(const UTF8 &text, uint32_t width)
    {
        Fits fits;
        if (font == nullptr) {
            fits.fits = false;
        }
        else {
            auto cnt            = font->getCharCountInSpace(text, area(Area::Max).w);
            fits.fits           = cnt == text.length();
            fits.space_consumed = font->getPixelWidth(text);
        }
        return fits;
    }

    void Label::fitTextIn(const UTF8 &text)
    {
        Fits fits = textFitsIn(text, area(Item::Area::Max).w);
        if (fits) {
            setSize(fits.space_consumed, getHeight());
        }
        else {
            setSize(area(Item::Area::Max).w, getHeight());
        }
    }

    void Label::clear()
    {
        this->text = UTF8("");
        calculateDisplayText();
    }

    const UTF8 &Label::getText() const
    {
        return text;
    }

    unsigned int Label::getTextLength() const
    {
        return text.length();
    }

    void Label::setAlignment(const Alignment &value)
    {
        this->alignment = value;
        calculateDisplayText();
    }

    void Label::setMargins(const Margins &margins)
    {
        this->margins = margins;
        calculateDisplayText();
    }

    void Label::setEllipsis(Ellipsis ellipsis)
    {
        this->ellipsis = ellipsis;
        calculateDisplayText();
    }

    void Label::setLineMode(const bool &val)
    {
        // if line mode is disable remove the line if it was previously created
        if (val == false) {
            if (lineFront != nullptr) {
                this->removeWidget(lineFront);
                this->removeWidget(lineBack);
                delete lineFront;
                delete lineBack;
                lineFront = nullptr;
                lineBack  = nullptr;
            }
        }
        else {
            if (lineFront == nullptr) {
                lineFront = new Rect(this, 0, 0, 0, 0);
                lineBack  = new Rect(this, 0, 0, 0, 0);
            }
        }
        calculateDisplayText();
    }

    std::list<DrawCommand *> Label::buildDrawList()
    {

        std::list<DrawCommand *> commands;

        // check if widget is visible
        if (visible == false) {
            return commands;
        }

        // get children draw commands
        std::list<DrawCommand *> commandsChildren;
        commandsChildren = Item::buildDrawList();

        // base class draw commands
        std::list<DrawCommand *> commandsBase;
        commandsBase = gui::Rect::buildDrawList();

        commands.splice(commands.end(), commandsBase);
        // set local draw commands - text command
        if (font != nullptr) {
            CommandText *textCmd = new CommandText();
            textCmd->str         = textDisplayed;
            textCmd->fontID      = font->id;
            textCmd->color       = textColor;

            textCmd->x          = drawArea.x;
            textCmd->y          = drawArea.y;
            textCmd->w          = drawArea.w;
            textCmd->h          = drawArea.h;
            textCmd->tx         = textArea.x;
            textCmd->ty         = textArea.y;
            textCmd->tw         = textArea.w;
            textCmd->th         = textArea.h;
            textCmd->charsWidth = stringPixelWidth;

            textCmd->areaX = widgetArea.x;
            textCmd->areaY = widgetArea.y;
            textCmd->areaW = widgetArea.w;
            textCmd->areaH = widgetArea.h;
            commands.push_back(textCmd);
        }
        if (not commandsChildren.empty()) {
            commands.splice(commands.end(), commandsChildren);
        }

        return commands;
    }

    bool Label::onDimensionChanged(const BoundingBox &oldDim, const BoundingBox &newDim)
    {
        calculateDisplayText();
        return true;
    }

    void Label::setFont(const UTF8 &fontName)
    {
        RawFont *newFont = FontManager::getInstance().getFont(fontName);
        setFont(newFont);
    }

    void Label::setFont(RawFont *font)
    {
        this->font = font;
        if (font != nullptr) {
            calculateDisplayText();
        }
    }

    RawFont *Label::getFont() const
    {
        return font;
    }

    void Label::setTextColor(Color color)
    {
        textColor = color;
    }

    uint32_t Label::getTextNeedSpace() const
    {
        if (font == nullptr) {
            return 0;
        }
        return font->getPixelWidth(text);
    }

    uint32_t Label::getTextHeight() const
    {
        if (font == nullptr) {
            return 0;
        }
        return font->info.line_height;
    }

} /* namespace gui */
