// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Error.hpp"
#include "interface/profiles/ProfileManager.hpp"
#include "interface/BluetoothDriver.hpp"

#include <cstdint>

namespace sys
{
    class Service;
}

namespace bluetooth
{
    class SettingsHolder;
    class Command
    {
      public:
        enum Type
        {
            StartScan,
            StopScan,
            StartPan,
            VisibilityOn,
            VisibilityOff,
            ConnectAudio,
            DisconnectAudio,
            PowerOn,
            PowerOff,
            Pair,
            StartStream,
            StopStream,
            SwitchProfile,
            None,
        };

        explicit Command(Command::Type type, std::optional<uint8_t *> addr = std::nullopt) : type(type)
        {
            if (addr) {
                bd_addr_copy(address, addr.value());
            }
            else {
                memset(address, 0, sizeof(bd_addr_t));
            }
        }

        auto getType() const noexcept -> Command::Type
        {
            return type;
        }

        auto getAddress() -> uint8_t *
        {
            return address;
        }

      private:
        bd_addr_t address{};
        Type type;
    };

    class AbstractCommandHandler
    {
      public:
        virtual ~AbstractCommandHandler() noexcept = default;

        virtual auto handle(Command command) -> Error::Code = 0;
    };

    class CommandHandler : public AbstractCommandHandler
    {
      public:
        explicit CommandHandler(sys::Service *service,
                                std::shared_ptr<bluetooth::SettingsHolder> settings,
                                std::shared_ptr<bluetooth::ProfileManager> profileManager,
                                std::shared_ptr<bluetooth::AbstractDriver> driver);

        auto handle(Command command) -> Error::Code override;

      private:
        Error::Code scan();
        Error::Code stopScan();
        Error::Code startPan();
        Error::Code setVisibility(bool visibility);
        Error::Code establishAudioConnection(bd_addr_t addr);
        Error::Code disconnectAudioConnection();
        Error::Code pair(bd_addr_t addr);

        Error::Code switchAudioProfile();
        sys::Service *service;
        std::shared_ptr<bluetooth::SettingsHolder> settings;
        std::shared_ptr<bluetooth::ProfileManager> profileManager;
        std::shared_ptr<AbstractDriver> driver;
    };
} // namespace bluetooth
