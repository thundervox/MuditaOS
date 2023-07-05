// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <ticks.hpp>
#include <timers.h>

#include <log/log.hpp>

namespace bsp
{
    namespace
    {
        bool wfiModeAllowed = false;

        /* RTC wakes up CPU every minute, so go to sleep only if next timer will
         * trigger after more than minute - this way no event will ever be missed */
        constexpr auto timersInactivityTimeMs{60 * utils::time::milisecondsInSecond};

        inline constexpr std::uint32_t highFrequencyTimerTicksToMs(std::uint32_t ticks)
        {
            return ticks / (configRUN_TIME_STATS_CLOCK_HZ / utils::time::milisecondsInSecond);
        }

        bool isTimerTaskScheduledSoon()
        {
            const auto currentTick          = cpp_freertos::Ticks::GetTicks();
            const auto timersNextWakeUpTick = xTimerGetNextWakeUpTime();

            if (timersNextWakeUpTick > currentTick) {
                return (cpp_freertos::Ticks::TicksToMs(timersNextWakeUpTick - currentTick) < timersInactivityTimeMs);
            }
            return true;
        }

        auto f = [](bool value) { return !value ? "true" : "false"; };

        void printClocksInfo()
        {
            LOG_PRINTF("PLL_ARM locked? %s", f(CCM_ANALOG->PLL_ARM & CCM_ANALOG_PLL_ARM_LOCK_SHIFT));
            LOG_PRINTF("PLL_USB1 locked? %s", f(CCM_ANALOG->PLL_USB1 & CCM_ANALOG_PLL_USB1_LOCK_SHIFT));
            LOG_PRINTF(" - Pll3Pfd0Clk enabled? %s", f(CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD1_CLKGATE_MASK));
            LOG_PRINTF(" - Pll3Pfd1Clk enabled? %s", f(CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD1_CLKGATE_MASK));
            LOG_PRINTF(" - Pll3Pfd2Clk enabled? %s", f(CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD2_CLKGATE_MASK));
            LOG_PRINTF(" - Pll3Pfd3Clk enabled? %s", f(CCM_ANALOG->PFD_480 & CCM_ANALOG_PFD_480_PFD3_CLKGATE_MASK));
            LOG_PRINTF("PLL_USB2 locked? %s", f(CCM_ANALOG->PLL_USB2 & CCM_ANALOG_PLL_USB2_LOCK_SHIFT));
            LOG_PRINTF("PLL_SYS locked? %s", f(CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_LOCK_SHIFT));
            LOG_PRINTF(" - SysPllPfd0Clk enabled? %s", f(CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD0_CLKGATE_MASK));
            LOG_PRINTF(" - SysPllPfd1Clk enabled? %s", f(CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD1_CLKGATE_MASK));
            LOG_PRINTF(" - SysPllPfd2Clk enabled? %s", f(CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD2_CLKGATE_MASK));
            LOG_PRINTF(" - SysPllPfd3Clk enabled? %s", f(CCM_ANALOG->PFD_528 & CCM_ANALOG_PFD_528_PFD3_CLKGATE_MASK));
            LOG_PRINTF("PLL_AUDIO locked? %s", f(CCM_ANALOG->PLL_AUDIO & CCM_ANALOG_PLL_AUDIO_LOCK_SHIFT));
            LOG_PRINTF("PLL_VIDEO locked? %s", f(CCM_ANALOG->PLL_VIDEO & CCM_ANALOG_PLL_VIDEO_LOCK_SHIFT));
            LOG_PRINTF("PLL_ENET locked? %s", f(CCM_ANALOG->PLL_ENET & CCM_ANALOG_PLL_ENET_LOCK_SHIFT));
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
        if (isTimerTaskScheduledSoon()) {
            return;
        }

        LOG_ERROR("Before WFI!");
        printClocksInfo();

        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();
        LOG_INFO("*** WFI IN ***");

        __DSB(); // Mandatory before entering WFI, see ARM AN321, p.13, "Sleep"
        __WFI();
        __ISB(); // See https://stackoverflow.com/questions/47022456/why-is-an-isb-needed-after-wfi-in-cortex-m-freertos

        /* Block WFI mode so that OS wakes up fully and goes to sleep only after
         * frequency has dropped back to the lowest level */
        blockEnteringWfiMode();

        const auto exitWfiTimerTicks = ulHighFrequencyTimerTicks();
        const auto sleepTimeMs =
            highFrequencyTimerTicksToMs(utils::computeIncrease(exitWfiTimerTicks, enterWfiTimerTicks));
        xTaskCatchUpTicks(cpp_freertos::Ticks::MsToTicks(sleepTimeMs));
        LOG_INFO("*** WFI OUT sleep time: %ld ***", sleepTimeMs);

        LOG_ERROR("After WFI!");
        printClocksInfo();
    }
} // namespace bsp
