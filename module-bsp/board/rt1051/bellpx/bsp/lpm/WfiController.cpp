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
#include <MIMXRT1051.h>

#include <log/log.hpp>

namespace bsp
{
    namespace
    {
        bool wfiModeAllowed = false;
        std::uint32_t savedPrimask;

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

        bool isGpr7Ready()
        {
            constexpr std::uint32_t gpr7AckMask =
                IOMUXC_GPR_GPR7_LPI2C1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPI2C3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPSPI1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPSPI3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPUART1_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPUART3_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPUART5_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_ACK_MASK |
                IOMUXC_GPR_GPR7_LPUART7_STOP_ACK_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_ACK_MASK;

            constexpr std::uint32_t gpr7ReqMask =
                IOMUXC_GPR_GPR7_LPI2C1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C2_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPI2C3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPI2C4_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPSPI1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI2_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPSPI3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPSPI4_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPUART1_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART2_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPUART3_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART4_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPUART5_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART6_STOP_REQ_MASK |
                IOMUXC_GPR_GPR7_LPUART7_STOP_REQ_MASK | IOMUXC_GPR_GPR7_LPUART8_STOP_REQ_MASK;

            IOMUXC_GPR->GPR7 = gpr7ReqMask;

            constexpr auto timeoutMs    = utils::time::milisecondsInSecond;
            constexpr auto retryDelayMs = 10;
            const auto initialTicks     = cpp_freertos::Ticks::GetTicks();

            while ((IOMUXC_GPR->GPR7 & gpr7AckMask) != gpr7AckMask) {
                if ((cpp_freertos::Ticks::GetTicks() - initialTicks) > cpp_freertos::Ticks::MsToTicks(timeoutMs)) {
                    LOG_ERROR("GPR7 is not ready (0x%08lx)", IOMUXC_GPR->GPR7);
                    return false;
                }
                vTaskDelay(retryDelayMs);
            }
            return true;
        }

        bool isGpr4Ready()
        {
            constexpr std::uint32_t gpr4AckMask =
                IOMUXC_GPR_GPR4_ENET_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_ACK_MASK |
                IOMUXC_GPR_GPR4_SAI2_STOP_ACK_MASK | IOMUXC_GPR_GPR4_SAI3_STOP_ACK_MASK |
                IOMUXC_GPR_GPR4_SEMC_STOP_ACK_MASK | IOMUXC_GPR_GPR4_PIT_STOP_ACK_MASK |
                IOMUXC_GPR_GPR4_FLEXIO1_STOP_ACK_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_ACK_MASK;

            constexpr std::uint32_t gpr4ReqMask =
                IOMUXC_GPR_GPR4_ENET_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI1_STOP_REQ_MASK |
                IOMUXC_GPR_GPR4_SAI2_STOP_REQ_MASK | IOMUXC_GPR_GPR4_SAI3_STOP_REQ_MASK |
                IOMUXC_GPR_GPR4_SEMC_STOP_REQ_MASK | IOMUXC_GPR_GPR4_PIT_STOP_REQ_MASK |
                IOMUXC_GPR_GPR4_FLEXIO1_STOP_REQ_MASK | IOMUXC_GPR_GPR4_FLEXIO2_STOP_REQ_MASK;

            IOMUXC_GPR->GPR4 = gpr4ReqMask;

            constexpr auto timeoutMs    = utils::time::milisecondsInSecond;
            constexpr auto retryDelayMs = 10;
            const auto initialTicks     = cpp_freertos::Ticks::GetTicks();

            while ((IOMUXC_GPR->GPR4 & gpr4AckMask) != gpr4AckMask) {
                if ((cpp_freertos::Ticks::GetTicks() - initialTicks) > cpp_freertos::Ticks::MsToTicks(timeoutMs)) {
                    LOG_ERROR("GPR4 is not ready (0x%08lx)", IOMUXC_GPR->GPR4);
                    return false;
                }
                vTaskDelay(retryDelayMs);
            }
            return true;
        }

        void peripheralExitDozeMode()
        {
            IOMUXC_GPR->GPR8  = 0x00000000;
            IOMUXC_GPR->GPR12 = 0x00000000;
        }

        void peripheralEnterDozeMode()
        {
            IOMUXC_GPR->GPR8 = IOMUXC_GPR_GPR8_LPI2C1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C2_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPI2C3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPI2C4_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPSPI1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI2_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPSPI3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPSPI4_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPUART1_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART2_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPUART3_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART4_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPUART5_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART6_IPG_DOZE_MASK |
                               IOMUXC_GPR_GPR8_LPUART7_IPG_DOZE_MASK | IOMUXC_GPR_GPR8_LPUART8_IPG_DOZE_MASK;

            IOMUXC_GPR->GPR12 = IOMUXC_GPR_GPR12_FLEXIO1_IPG_DOZE_MASK | IOMUXC_GPR_GPR12_FLEXIO2_IPG_DOZE_MASK;
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
        // if (!isGpr7Ready()) {
        //     return;
        // }
        // if (!isGpr4Ready()) {
        //     return;
        // }
        if (!isWfiModeAllowed()) {
            return;
        }
        if (isTimerTaskScheduledSoon()) {
            return;
        }

        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();

        peripheralEnterDozeMode();

        savedPrimask = DisableGlobalIRQ();
        __DSB();
        __ISB();

        /* Clear the SLEEPDEEP bit to go into sleep mode (WAIT) */
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

        __WFI();

        EnableGlobalIRQ(savedPrimask);
        __DSB();
        __ISB();

        peripheralExitDozeMode();

        /* Block WFI mode so that OS wakes up fully and goes to sleep only after
         * frequency has dropped back to the lowest level */
        blockEnteringWfiMode();

        const auto exitWfiTimerTicks = ulHighFrequencyTimerTicks();
        const auto sleepTimeMs =
            highFrequencyTimerTicksToMs(utils::computeIncrease(exitWfiTimerTicks, enterWfiTimerTicks));
        xTaskCatchUpTicks(cpp_freertos::Ticks::MsToTicks(sleepTimeMs));
        LOG_INFO("*** WFI OUT sleep time: %ld ***", sleepTimeMs);
    }
} // namespace bsp
