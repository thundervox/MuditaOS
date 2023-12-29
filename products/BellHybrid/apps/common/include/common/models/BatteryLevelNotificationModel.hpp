// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <module-utils/EventStore/EventStore.hpp>
#include <service-db/Settings.hpp>

#include <cstdint>
#include <functional>
#include <string>

namespace app
{
    class ApplicationCommon;
}

namespace app
{
    static constexpr auto batteryLevelIntervalUpperHysteresis = 2;

    enum class BatteryLevelInterval
    {
        UNKNOWN,
        ABOVE_15_PERCENT, // 16-100%
        BELOW_15_PERCENT, // 10-15%
        BELOW_10_PERCENT, // 5-9%
        BELOW_5_PERCENT,  // 0-4%
    };

    class AbstractBatteryLevelNotificationModel
    {
      public:
        virtual ~AbstractBatteryLevelNotificationModel() noexcept = default;

        virtual void updateBatteryLevelInterval(std::uint32_t currentBatteryLevel) = 0;
        [[nodiscard]] virtual BatteryLevelInterval getBatteryLevelInterval() const = 0;
        [[nodiscard]] virtual bool isBatteryIntervalHandled() const                = 0;
        virtual void setCurrentBatteryIntervalAsHandled()                          = 0;
    };

    class BatteryLevelNotificationModel : public AbstractBatteryLevelNotificationModel
    {
      public:
        explicit BatteryLevelNotificationModel();
        void updateBatteryLevelInterval(std::uint32_t currentBatteryLevel) override;
        [[nodiscard]] BatteryLevelInterval getBatteryLevelInterval() const override;
        [[nodiscard]] bool isBatteryIntervalHandled() const override;
        void setCurrentBatteryIntervalAsHandled() override;

      private:
        BatteryLevelInterval currentBatteryLevelInterval = BatteryLevelInterval::UNKNOWN; // TODO: name small
        bool isCurrentBatteryIntervalHandled             = false;
    };
} // namespace app
