// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <Application.hpp>

namespace app
{
    inline constexpr auto name_calculator     = "ApplicationCalculator";
    inline constexpr std::uint16_t stack_size = 4096;

    class ApplicationCalculator : public Application
    {
      public:
        ApplicationCalculator(std::string name                    = name_calculator,
                              std::string parent                  = {},
                              StartInBackground startInBackground = {false});
        ~ApplicationCalculator() override = default;

        sys::Message_t DataReceivedHandler(sys::DataMessage *msgl, sys::ResponseMessage *resp) override;
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
            return {{manager::actions::Launch}};
        }
    };
} /* namespace app */
