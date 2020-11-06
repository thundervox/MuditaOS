// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Application.hpp"

#include <bsp/common.hpp>

namespace gui::window::name
{
    inline constexpr auto bluetooth   = "Bluetooth";
    inline constexpr auto all_devices = "AllDevices";
    inline constexpr auto phone_name  = "PhoneName";
    inline constexpr auto add_device  = "AddDevice";

    inline constexpr auto network        = "Network";
    inline constexpr auto phone_modes    = "PhoneModes";
    inline constexpr auto apps_and_tools = "AppsAndTools";
    inline constexpr auto security       = "Security";
    inline constexpr auto system         = "System";

    inline constexpr auto display_light  = "DisplayLight";
    inline constexpr auto font_size      = "FontSize";
    inline constexpr auto keypad_light   = "KeypadLight";
    inline constexpr auto input_language = "InputLanguage";
    inline constexpr auto locked_screen  = "LockedScreen";

    inline constexpr auto messages  = "Messages";
    inline constexpr auto torch     = "Torch";
    inline constexpr auto templates = "Templates";

    inline constexpr auto autolock  = "Autolock";
    inline constexpr auto wallpaper = "Wallpaper";
    inline constexpr auto quotes    = "Quotes";
    inline constexpr auto new_quote = "NewQuote";

    inline constexpr auto display_and_keypad = "DisplayAndKeypad";
    inline constexpr auto change_settings    = "ChangeSettings";
    inline constexpr auto all_operators      = "AllOperators";
    inline constexpr auto import_contacts    = "ImportContacts";
    inline constexpr auto dialog_settings    = "DialogSettings";

} // namespace gui::window::name

namespace app
{
    inline constexpr auto name_settings_new = "ApplicationSettingsNew";

    class ApplicationSettingsNew : public app::Application
    {
      public:
        ApplicationSettingsNew(std::string name                    = name_settings_new,
                               std::string parent                  = {},
                               StartInBackground startInBackground = {false});
        auto DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) -> sys::MessagePointer override;
        auto InitHandler() -> sys::ReturnCodes override;

        auto SwitchPowerModeHandler(const sys::ServicePowerMode mode) -> sys::ReturnCodes final
        {
            return sys::ReturnCodes::Success;
        }

        void createUserInterface() override;
        void destroyUserInterface() override;
        bsp::Board board = bsp::Board::none;
    };

    template <> struct ManifestTraits<ApplicationSettingsNew>
    {
        static auto GetManifest() -> manager::ApplicationManifest
        {
            return {{manager::actions::Launch}};
        }
    };
} /* namespace app */
