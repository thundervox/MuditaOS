// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <EventStore.hpp>
#include <common/widgets/BellConnectionStatus.hpp>
#include <gui/widgets/text/TextFixedSize.hpp>
#include "i18n/i18n.hpp"

namespace
{
    constexpr auto usb_connected_status     = "app_bellmain_usb_status_connected";
    constexpr auto status_text_max_w        = 350U;
    constexpr auto status_text_max_h        = 102U;
} // namespace

namespace gui
{
    BellConnectionStatus::BellConnectionStatus(Item *parent) : HBox(parent)
    {
        statusText = new Text(parent);
        statusText->setMaximumSize(status_text_max_w, status_text_max_h);
        statusText->setFont(style::window::font::verybiglight);
        statusText->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Top));
        statusText->setEdges(RectangleEdge::None);
        statusText->setEditMode(EditMode::Browse);
        statusText->activeItem = false;
        statusText->drawUnderline(false);
        statusText->setText(utils::translate(usb_connected_status));
        statusText->setVisible(true);
    }

    void BellConnectionStatus::setFont(const UTF8 &fontName)
    {
        statusText->setFont(fontName);
    }

    bool BellConnectionStatus::hideIfPossible(const Store::Battery::State &state)
    {
        if (state != Store::Battery::State::PluggedNotCharging) {
            statusText->setVisible(false);
            return true;
        }
        return false;
    }

    void BellConnectionStatus::show()
    {
        statusText->setVisible(true);
    }

    bool BellConnectionStatus::isVisible() const
    {
        return statusText->visible;
    }
} // namespace gui
