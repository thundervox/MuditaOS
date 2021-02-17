// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "BluetoothCheckPasskeyWindow.hpp"
#include "application-settings-new/ApplicationSettings.hpp"
#include "application-settings-new/widgets/SettingsStyle.hpp"
#include <widgets/Text.hpp>

namespace gui
{
    namespace
    {
        constexpr auto maxPasskeyCharactersCount = 16U;
    }
    namespace passkey_style = style::settings::window::bluetooth::passkey;

    BluetoothCheckPasskeyWindow::BluetoothCheckPasskeyWindow(app::Application *app)
        : AppWindow(app, window::name::bluetooth_check_passkey)
    {
        buildInterface();
    }

    void BluetoothCheckPasskeyWindow::buildInterface()
    {
        AppWindow::buildInterface();

        setTitle(utils::localize.get("app_settings_bt"));

        bottomBar->setActive(BottomBar::Side::CENTER, true);
        bottomBar->setActive(BottomBar::Side::RIGHT, true);
        bottomBar->setText(BottomBar::Side::CENTER, utils::localize.get(style::strings::common::confirm));
        bottomBar->setText(BottomBar::Side::RIGHT, utils::localize.get(style::strings::common::back));

        image = new Image(this, passkey_style::image::x, passkey_style::image::y, 0, 0, "bluetooth_W_G");

        label = new Label(this,
                          passkey_style::label::x,
                          passkey_style::label::y,
                          passkey_style::label::w,
                          style::window::label::default_h);
        label->setFont(style::window::font::big);
        label->setEdges(RectangleEdge::None);
        label->setText(utils::localize.get("app_settings_bluetooth_enter_passkey"));

        text = new Text(
            this, passkey_style::text::x, passkey_style::text::y, passkey_style::text::w, passkey_style::text::h);
        text->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Top));
        text->setTextLimitType(TextLimitType::MaxSignsCount, maxPasskeyCharactersCount);
        text->setEdges(RectangleEdge::Bottom);
        text->setInputMode(new InputMode({InputMode::digit}));
        text->setPenWidth(style::window::default_border_rect_no_focus);
        text->setFont(style::window::font::largelight);

        setFocusItem(text);
    }
} // namespace gui
