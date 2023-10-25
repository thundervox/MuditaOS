// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>
#include <fsl_pmu.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <ticks.hpp>
#include <timers.h>
#include <MIMXRT1051.h>

#include <magic_enum.hpp>
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

        void checkPendingIrq()
        {
            const auto start = static_cast<std::uint8_t>(DMA0_DMA16_IRQn);
            const auto end   = static_cast<std::uint8_t>(NMI_WAKEUP_IRQn) + 1;
            for (auto irq = start; irq < end; ++irq) {
                const auto name = static_cast<IRQn_Type>(irq);
                if (NVIC_GetPendingIRQ(name)) {
                    LOG_INFO("Pending IRQ: %s", magic_enum::enum_name(name).data());
                }
            }
        }

        void savePendingIrq()
        {
            constexpr auto registerOffset{3};
            constexpr auto registerMaxNumber{7};

            const auto start     = static_cast<std::uint8_t>(DMA0_DMA16_IRQn);
            const auto end       = static_cast<std::uint8_t>(NMI_WAKEUP_IRQn) + 1;
            auto pendingIrqCount = 0;
            for (auto irq = start; irq < end; ++irq) {
                const auto irqNr = static_cast<IRQn_Type>(irq);
                if (NVIC_GetPendingIRQ(irqNr)) {
                    const auto reg = pendingIrqCount + registerOffset;
                    if (reg <= registerMaxNumber) {
                        SNVS->LPGPR[reg] = irqNr;
                    }
                    pendingIrqCount++;
                }
            }
        }

        void logAndClearLastSavedPendingIrq()
        {
            for (auto reg = 3; reg < 8; ++reg) {
                if (SNVS->LPGPR[reg] != 0) {
                    const auto name = static_cast<IRQn_Type>(SNVS->LPGPR[reg]);
                    LOG_INFO("Saved register LPGPR[%d]: %s", reg, magic_enum::enum_name(name).data());
                    SNVS->LPGPR[reg] = 0;
                }
            }
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

        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();

        logAndClearLastSavedPendingIrq();
        checkPendingIrq();
        peripheralEnterDozeMode();

        /*
         * ERR006223: CCM: Failure to resuem from wait/stop mode with power gating
         *   Configure REG_BYPASS_COUNTER to 2
         *   Enable the RBC bypass counter here to hold off the interrupts. RBC counter
         *  needs to be no less than 2.
         */
        CCM->CCR = (CCM->CCR & ~CCM_CCR_REG_BYPASS_COUNT_MASK) | CCM_CCR_REG_BYPASS_COUNT(2);
        CCM->CCR |= (CCM_CCR_OSCNT(0xAF) | CCM_CCR_COSC_EN_MASK | CCM_CCR_RBC_EN_MASK);

        /* Now delay for a short while (3usec) at this point
         * so a short loop should be enough. This delay is required to ensure that
         * the RBC counter can start counting in case an interrupt is already pending
         * or in case an interrupt arrives just as ARM is about to assert DSM_request.
         */
        SDK_DelayAtLeastUs(3, CLOCK_GetCpuClkFreq());

        __disable_irq();
        __DSB();
        __ISB();

        /* Clear the SLEEPDEEP bit to go into sleep mode (WAIT) */
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

        __DSB();
        __WFI();
        __ISB();

        __NOP();
        __NOP();
        __NOP();
        __NOP();
        savePendingIrq();
        peripheralExitDozeMode();

        __enable_irq();
        __NOP();

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
