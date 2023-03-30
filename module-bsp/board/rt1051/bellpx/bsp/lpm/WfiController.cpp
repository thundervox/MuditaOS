// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <cstdint>
#include <fsl_common.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>

#include <log/log.hpp>

namespace bsp
{
    static inline std::uint32_t HighFrequencyTimerTicksToMs(std::uint32_t ticks)
    {
        return ticks / (configRUN_TIME_STATS_CLOCK_HZ / utils::time::milisecondsInSecond);
    }

    void enterWfiMode()
    {
        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();
        LOG_INFO("*** WFI IN ***");
        __DSB();
        __WFI();
        __ISB();
        const auto exitWfiTimerTicks = ulHighFrequencyTimerTicks();
        const auto sleepTimeMs =
            HighFrequencyTimerTicksToMs(utils::computeIncrease(exitWfiTimerTicks, enterWfiTimerTicks));
        xTaskCatchUpTicks(sleepTimeMs);
        LOG_INFO("*** WFI OUT sleep time: %ld ***", sleepTimeMs);
    }
} // namespace bsp
