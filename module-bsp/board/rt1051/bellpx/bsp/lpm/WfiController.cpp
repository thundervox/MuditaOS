// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>

#include <log/log.hpp>

namespace bsp
{
    namespace
    {
        bool wfiModeAllowed = false;

        inline constexpr std::uint32_t highFrequencyTimerTicksToMs(std::uint32_t ticks)
        {
            return ticks / (configRUN_TIME_STATS_CLOCK_HZ / utils::time::milisecondsInSecond);
        }
    } // namespace

    void allowEnteringWfiMode()
    {
        LOG_ERROR("Allowing WFI mode!");
        wfiModeAllowed = true;
    }

    void blockEnteringWfiMode()
    {
        LOG_ERROR("Blocking WFI mode!");
        wfiModeAllowed = false;
    }

    bool isWfiModeAllowed()
    {
        return wfiModeAllowed;
    }

    void enterWfiModeIfAllowed()
    {
        if (!isWfiModeAllowed()) {
            return;
        }

        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();
        LOG_INFO("*** WFI IN ***");

        __DSB(); // Mandatory before entering WFI, see ARM AN321, p.13, "Sleep"
        __WFI();
        __ISB(); // See https://stackoverflow.com/questions/47022456/why-is-an-isb-needed-after-wfi-in-cortex-m-freertos

        /* Block WFI mode so that CPU wakes up fully and goes to sleep
         * only after frequency has dropped back to the lowest one */
        blockEnteringWfiMode();

        const auto exitWfiTimerTicks = ulHighFrequencyTimerTicks();
        const auto sleepTimeMs =
            highFrequencyTimerTicksToMs(utils::computeIncrease(exitWfiTimerTicks, enterWfiTimerTicks));
        xTaskCatchUpTicks(pdMS_TO_TICKS(sleepTimeMs));
        LOG_INFO("*** WFI OUT sleep time: %ld ***", sleepTimeMs);
    }
} // namespace bsp
