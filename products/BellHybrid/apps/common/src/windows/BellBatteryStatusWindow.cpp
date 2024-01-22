// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "windows/BellBatteryStatusWindow.hpp"
#include <data/BatteryStatusSwitchData.hpp>
#include <common/data/BatteryUtils.hpp>
#include <gui/widgets/text/TextFixedSize.hpp>
#include <gui/widgets/ImageBox.hpp>
#include <gui/input/InputEvent.hpp>
#include <Application.hpp>

#include <chrono>

namespace
{
    using namespace std::chrono_literals;

    constexpr auto top_description_font          = style::window::font::largelight;
    constexpr auto bottom_description_font       = style::window::font::verybiglight;
    constexpr auto bottom_description_max_size_w = style::bell_base_layout::outer_layouts_w;
    constexpr auto bottom_description_max_size_h = 85U;
    constexpr auto imageType                     = gui::ImageTypeSpecifier::W_G;
    constexpr auto batteryEntries =
        std::array<battery_utils::BatteryLevelEntry, 6>{{{{0, 9}, "bell_status_battery_lvl0"},
                                                         {{10, 19}, "bell_status_battery_lvl1"},
                                                         {{20, 39}, "bell_status_battery_lvl2"},
                                                         {{40, 69}, "bell_status_battery_lvl3"},
                                                         {{70, 95}, "bell_status_battery_lvl4"},
                                                         {{96, 100}, "bell_status_battery_lvl5"}}};
    // this time must be longer than the long press time (10s) to turn off the system
    constexpr auto windowLongTimeout{12s};
    // default screen timeout
    constexpr auto windowDefaultTimeout{3s};
    // default screen timeout for low battery warning
    constexpr auto windowDefaultTimeoutForLowBatteryWarning{6s};
} // namespace

namespace gui
{
    BellBatteryStatusWindow::BellBatteryStatusWindow(app::ApplicationCommon *app, const std::string &name)
        : WindowWithTimer{app, name, windowLongTimeout}
    {
        buildInterface();
    }
    void BellBatteryStatusWindow::buildInterface()
    {
        WindowWithTimer::buildInterface();
        statusBar->setVisible(false);
        header->setTitleVisibility(false);
        navBar->setVisible(false);

        body = new BellBaseLayout(this, 0, 0, style::window_width, style::window_height, false);

        topDescription = new TextFixedSize(body->firstBox);
        topDescription->setMinimumSize(style::bell_base_layout::outer_layouts_w,
                                       style::bell_base_layout::outer_layouts_h);
        topDescription->setFont(top_description_font);
        topDescription->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        topDescription->setEdges(RectangleEdge::None);
        topDescription->activeItem = false;
        topDescription->drawUnderline(false);

        hbox = new HBox(body->lastBox);
        hbox->setMinimumSize(style::bell_base_layout::outer_layouts_w, style::bell_base_layout::outer_layouts_h);
        hbox->setEdges(RectangleEdge::None);
        hbox->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));

        chargingIcon = new ImageBox(hbox, 0, 0, 0, 0, new Image("bell_lightning", imageType));
        chargingIcon->setMargins(gui::Margins(0, 0, 8, 0));
        chargingIcon->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        chargingIcon->setMinimumSizeToFitImage();
        chargingIcon->activeItem = false;

        bottomDescription = new TextFixedSize(hbox);
        bottomDescription->setMaximumSize(bottom_description_max_size_w, bottom_description_max_size_h);
        bottomDescription->setFont(bottom_description_font);
        bottomDescription->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        bottomDescription->setEdges(RectangleEdge::None);
        bottomDescription->activeItem = false;
        bottomDescription->drawUnderline(false);

        center = new ImageBox(body->centerBox,
                              0,
                              0,
                              style::bell_base_layout::w,
                              style::bell_base_layout::h,
                              new Image("bell_status_battery_lvl0", imageType));
        center->setAlignment(Alignment(Alignment::Horizontal::Center, Alignment::Vertical::Center));
        center->setEdges(RectangleEdge::None);
        center->setMinimumSizeToFitImage();
        center->activeItem = false;

        body->resize();
    }
    bool BellBatteryStatusWindow::onInput(const InputEvent &inputEvent)
    {
        if (inputEvent.isShortRelease(KeyCode::KEY_ENTER) || inputEvent.isShortRelease(KeyCode::KEY_RF)) {
            detachTimerIfExists();
            application->returnToPreviousWindow();
            return true;
        }
        else if (inputEvent.getKeyCode() == KeyCode::KEY_RF) {
            if (inputEvent.isLongRelease()) {
                // here the "back" button is held all the time, so we set the screen timeout to a long time
                // to stay on this screen until the system shutdown popup is displayed
                resetTimer(windowLongTimeout);
            }
            else {
                // here the "back" button is released, so we set the screen timeout to the default time
                resetTimer(windowDefaultTimeout);
            }
            return true;
        }

        return false;
    }
    void BellBatteryStatusWindow::onBeforeShow(ShowMode mode, SwitchData *data)
    {
        if (data != nullptr) {
            const auto batteryData = dynamic_cast<BatteryStatusSwitchData *>(data);
            if (batteryData == nullptr) {
                LOG_ERROR("Received data nullptr");
                return;
            }
            const auto soc                 = batteryData->getBatteryLevel();
            const bool isCharging          = batteryData->isCharging();
            const bool isLowBatteryWarning = batteryData->isLowBatteryWarning();
            const auto image               = battery_utils::getBatteryLevelImage(batteryEntries, soc);
            if (isLowBatteryWarning) {
                topDescription->setText(utils::translate("battery_low"));
                auto tokenMap = text::RichTextParser::TokenMap({{"$BATTERY", std::to_string(soc)}});
                bottomDescription->setRichText(utils::translate("battery_remaining"), std::move(tokenMap));
                chargingIcon->setVisible(false);
                resetTimer(windowDefaultTimeoutForLowBatteryWarning);
            }
            else {
                topDescription->setText(utils::translate("app_settings_tech_info_battery"));
                bottomDescription->setText(std::to_string(soc) + "%");
                chargingIcon->setVisible(isCharging);
            }
            if (image) {
                center->setImage(image->data(), imageType);
                hbox->resizeItems();
                body->resize();
            }
        }
        WindowWithTimer::onBeforeShow(mode, data);
    }
    void BellBatteryStatusWindow::onClose(CloseReason reason)
    {
        application->popCurrentWindow();
    }
} // namespace gui
