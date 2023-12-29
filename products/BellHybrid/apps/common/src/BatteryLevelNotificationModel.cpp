// Copyright (c) 2017-2024, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "models/BatteryLevelNotificationModel.hpp"
#include <apps-common/ApplicationCommon.hpp>

namespace app
{
    BatteryLevelNotificationModel::BatteryLevelNotificationModel()
    {}

    void BatteryLevelNotificationModel::updateBatteryLevelInterval(std::uint32_t currentBatteryLevel)
    {
        auto updateBatteryIntervalState = [&](BatteryLevelInterval newBatteryLevelIntervalState) {
            if (currentBatteryLevelInterval == newBatteryLevelIntervalState) {
                return;
            }
            currentBatteryLevelInterval     = newBatteryLevelIntervalState;
            isCurrentBatteryIntervalHandled = (newBatteryLevelIntervalState == BatteryLevelInterval::ABOVE_15_PERCENT);
        };

        auto updateBatteryIntervalStateWithHysteresis = [&](std::uint32_t above15PercentHysteresis,
                                                            std::uint32_t below15PercentHysteresis,
                                                            std::uint32_t below10PercentHysteresis,
                                                            std::uint32_t below5PercentHysteresis) {
            if (currentBatteryLevel <= 4 + below5PercentHysteresis) {
                updateBatteryIntervalState(BatteryLevelInterval::BELOW_5_PERCENT);
            }
            else if (currentBatteryLevel <= 9 + below10PercentHysteresis) {
                updateBatteryIntervalState(BatteryLevelInterval::BELOW_10_PERCENT);
            }
            else if (currentBatteryLevel <= 15 + below15PercentHysteresis) {
                updateBatteryIntervalState(BatteryLevelInterval::BELOW_15_PERCENT);
            }
            else if (currentBatteryLevel > 15 + above15PercentHysteresis) {
                updateBatteryIntervalState(BatteryLevelInterval::ABOVE_15_PERCENT);
            }
        };

        switch (currentBatteryLevelInterval) {
        case (BatteryLevelInterval::UNKNOWN):
        case (BatteryLevelInterval::ABOVE_15_PERCENT):
            updateBatteryIntervalStateWithHysteresis(0, 0, 0, 0);
            break;
        case (BatteryLevelInterval::BELOW_15_PERCENT):
            updateBatteryIntervalStateWithHysteresis(batteryLevelIntervalUpperHysteresis, 0, 0, 0);
            break;
        case (BatteryLevelInterval::BELOW_10_PERCENT):
            updateBatteryIntervalStateWithHysteresis(0, batteryLevelIntervalUpperHysteresis, 0, 0);
            break;
        case (BatteryLevelInterval::BELOW_5_PERCENT):
            updateBatteryIntervalStateWithHysteresis(0, 0, batteryLevelIntervalUpperHysteresis, 0);
            break;
        default:
            break;
        }
    }

    BatteryLevelInterval BatteryLevelNotificationModel::getBatteryLevelInterval() const
    {
        return currentBatteryLevelInterval;
    }

    bool BatteryLevelNotificationModel::isBatteryIntervalHandled() const
    {
        return isCurrentBatteryIntervalHandled;
    }

    void BatteryLevelNotificationModel::setCurrentBatteryIntervalAsHandled()
    {
        isCurrentBatteryIntervalHandled = true;
    }

} // namespace app