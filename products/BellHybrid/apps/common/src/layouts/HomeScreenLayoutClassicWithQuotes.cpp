// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "layouts/HomeScreenLayoutClassicWithQuotes.hpp"
#include "data/BellMainStyle.hpp"
// #include "widgets/BellBattery.hpp"
// #include "widgets/DuoHBox.hpp"

// #include <date/date.h>
// #include <gui/widgets/text/TextFixedSize.hpp>
#include <widgets/TimeSetFmtSpinner.hpp>

// #include <service-time/api/TimeSettingsApi.hpp>

namespace
{
    constexpr auto quoteImageName{"bell_quote"};
    constexpr auto maxTimeBoxHeight{148U};
    constexpr auto imgBoxHeight{52U};
    constexpr auto quoteFont{style::window::font::mediumbiglight};
    constexpr auto authorFont{style::window::font::mediumbigbold};
    constexpr auto textBoxHeight{120U};
    constexpr auto quoteHeight{78U};
    constexpr auto authorHeight{42U};
}; // namespace

namespace gui
{
    HomeScreenLayoutClassicWithQuotes::HomeScreenLayoutClassicWithQuotes(std::string name)
        : HomeScreenLayoutClassic(std::move(name))
    {
        buildInterface();
        onShowMessage = [this]() {
            if (textBox->visible) {
                textBox->setVisible(false);
            }
        };
        onHideMessage = [this]() {
            if (!textBox->visible) {
                statusBox->setVisible(false);
                textBox->setVisible(true);
                textBox->informContentChanged();
            }
        };
    }

    void HomeScreenLayoutClassicWithQuotes::buildInterface()
    {
        using namespace bellMainStyle;

        timeHBox->setMaximumHeight(maxTimeBoxHeight);
        timeHBox->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Top));

        time->setMaximumHeight(maxTimeBoxHeight);
        time->setFont(style::window::font::huge);
        time->setMargins({mainWindow::ampm::time_margin_w, 0, 0, 0});

        ampm = new TextFixedSize(timeHBox);
        ampm->setMinimumSize(mainWindow::ampm::layout_w, style::bell_base_layout::center_layout_h);
        ampm->setMaximumSize(mainWindow::ampm::layout_w, style::bell_base_layout::center_layout_h);
        ampm->setMargins({mainWindow::ampm::margin_w, 0, 0, 0});
        ampm->setFont(mainWindow::time::font_small);
        ampm->setAlignment(Alignment(Alignment::Horizontal::Left, Alignment::Vertical::Top));
        ampm->setEdges(RectangleEdge::None);
        ampm->activeItem = false;
        ampm->drawUnderline(false);

        imgBox = new VBox(this->centerBox);
        imgBox->setMinimumSize(style::homescreen_classic::status_box_layout_w, imgBoxHeight);
        imgBox->setEdges(RectangleEdge::None);
        imgBox->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        imgBox->setVisible(true);

        quoteImg = new Image(imgBox, quoteImageName, gui::ImageTypeSpecifier::W_M);
        quoteImg->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Top));

        textBox = new VBox(nullptr);
        textBox->setMinimumSize(style::bell_base_layout::last_layout_w, textBoxHeight);
        textBox->setEdges(RectangleEdge::None);
        textBox->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Top));
        textBox->setVisible(true);

        quotes = new TextFixedSize(nullptr);
        quotes->setMaximumSize(style::bell_base_layout::last_layout_w, quoteHeight);
        quotes->setMargins({0, 0, 0, 0});
        quotes->setFont(quoteFont);
        quotes->setText("Wherever you are, and whatever you do, be in love.");
        quotes->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        quotes->setEdges(RectangleEdge::None);
        quotes->activeItem = false;
        quotes->drawUnderline(false);
        quotes->setVisible(true);

        author = new TextFixedSize(nullptr);
        author->setMaximumSize(style::bell_base_layout::last_layout_w, authorHeight);
        author->setMargins({0, 0, 0, 0});
        author->setFont(authorFont);
        author->setText("-Rumi");
        author->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        author->setEdges(RectangleEdge::None);
        author->activeItem = false;
        author->drawUnderline(false);
        author->setVisible(true);

        textBox->addWidget(quotes);
        textBox->addWidget(author);

        statusBox->setVisible(false);
        this->lastBox->addWidget(textBox);

        resizeItems();
    }

    void HomeScreenLayoutClassicWithQuotes::setTime(std::time_t newTime)
    {
        HomeScreenLayoutClassic::setTime(newTime);
        const auto t = std::localtime(&newTime);

        if (ampm->visible) {
            const auto hours = std::chrono::hours{t->tm_hour};
            const auto isPM  = date::is_pm(hours);
            ampm->setText(isPM ? utils::time::Locale::getPM() : utils::time::Locale::getAM());
        }
    }

    void HomeScreenLayoutClassicWithQuotes::setTimeFormat(utils::time::Locale::TimeFormat fmt)
    {
        HomeScreenLayoutClassic::setTimeFormat(fmt);
        if (fmt == utils::time::Locale::TimeFormat::FormatTime12H) {
            time->setMargins({bellMainStyle::mainWindow::ampm::time_margin_w, 0, 0, 0});
            ampm->setVisible(true);
        }
        else {
            time->setMargins({0, 0, 0, 0});
            ampm->setVisible(false);
        }
    }

}; // namespace gui
