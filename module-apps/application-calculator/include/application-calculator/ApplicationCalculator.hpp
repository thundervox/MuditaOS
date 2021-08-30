// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <apps-common/Application.hpp>

namespace app
{
    inline constexpr auto name_calculator     = "ApplicationCalculator";
    inline constexpr std::uint16_t stack_size = 2560;

    class ApplicationCalculator : public Application
    {
      public:
        explicit ApplicationCalculator(
            std::string name                            = name_calculator,
            std::string parent                          = {},
            sys::phone_modes::PhoneMode phoneMode       = sys::phone_modes::PhoneMode::Connected,
            sys::bluetooth::BluetoothMode bluetoothMode = sys::bluetooth::BluetoothMode::Disabled,
            StartInBackground startInBackground         = {false});
        ~ApplicationCalculator() override = default;

        sys::MessagePointer DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) override;
        sys::ReturnCodes InitHandler() override;
        sys::ReturnCodes DeinitHandler() override;

        sys::ReturnCodes SwitchPowerModeHandler(const sys::ServicePowerMode mode) final
        {
            return sys::ReturnCodes::Success;
        }

        void createUserInterface() final;
        void destroyUserInterface() final;
    };

    template <> struct ManifestTraits<ApplicationCalculator>
    {
        static auto GetManifest() -> manager::ApplicationManifest
        {
            return {
                {manager::actions::Launch, manager::actions::PhoneModeChanged, manager::actions::BluetoothModeChanged}};
        }
    };
} /* namespace app */
