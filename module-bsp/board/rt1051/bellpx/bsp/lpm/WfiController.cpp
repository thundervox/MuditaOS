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

#include <fsl_gpc.h>

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

        void checkAndClearPendingIrq()
        {
            const auto start = static_cast<std::uint8_t>(DMA0_DMA16_IRQn);
            const auto end   = static_cast<std::uint8_t>(NMI_WAKEUP_IRQn) + 1;
            for (auto irq = start; irq < end; ++irq) {
                const auto name = static_cast<IRQn_Type>(irq);
                if (NVIC_GetPendingIRQ(name)) {
                    LOG_INFO("Pending IRQ: %s", magic_enum::enum_name(name).data());
                    if (name == ANATOP_EVENT0_IRQn) {
                        const std::uint32_t status = PMU_GetStatusFlags(PMU);
                        LOG_INFO("ANATOP_EVENT0_IRQn: 0x%lx", status);
                        //                        NVIC_ClearPendingIRQ(name);
                    }
                }
            }
        }

        __attribute__((optimize("O0"))) void setWaitModeConfig()
        {
            /*
             * ERR007265: CCM: When improper low-power sequence is used,
             * the SoC enters low power mode before the ARM core executes WFI.
             *
             * Software workaround:
             * 1) Software should trigger IRQ #41 (GPR_IRQ) to be always pending
             *      by setting IOMUXC_GPR_GPR1_GINT.
             * 2) Software should then unmask IRQ #41 in GPC before setting CCM
             *      Low-Power mode.
             * 3) Software should mask IRQ #41 right after CCM Low-Power mode
             *      is set (set bits 0-1 of CCM_CLPCR).
             */

            GPC_EnableIRQ(GPC, GPR_IRQ_IRQn);
            const auto clpcr = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
            CCM->CLPCR       = clpcr | CCM_CLPCR_MASK_L2CC_IDLE_MASK | CCM_CLPCR_MASK_SCU_IDLE_MASK |
                         CCM_CLPCR_BYPASS_LPM_HS0_MASK | CCM_CLPCR_BYPASS_LPM_HS1_MASK | CCM_CLPCR_STBY_COUNT_MASK |
                         CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_LPM(kCLOCK_ModeWait);
            GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
        }

        __attribute__((optimize("O0"))) void setRunModeConfig()
        {
            CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
        }

        __attribute__((optimize("O0"))) void enterSleepMode()
        {
            const auto savedPrimask = DisableGlobalIRQ();
            __DSB();
            __ISB();

            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

            __WFI();

            EnableGlobalIRQ(savedPrimask);
            __DSB();
            __ISB();
        }

        constexpr auto CCMGateRegsCount = 7;
        volatile std::uint32_t clockGateValues[CCMGateRegsCount];

        void setLowPowerClockGate()
        {
            /* Save of the clock gate registers */
            clockGateValues[0] =
                CCM->CCGR0; /* TODO all these additional clock disabling should be moved to clock_config.cpp */
            clockGateValues[1] = CCM->CCGR1 & ~(CCM_CCGR1_CG5(3));                      // Disable enet
            clockGateValues[2] = CCM->CCGR2 & ~(CCM_CCGR2_CG15(3) | CCM_CCGR2_CG14(3)); // Disable pxp and lcd
            clockGateValues[3] = CCM->CCGR3 & ~(CCM_CCGR3_CG13(3) | CCM_CCGR3_CG12(3) | CCM_CCGR3_CG11(3) |
                                                CCM_CCGR3_CG10(3)); // Disable acmps
            clockGateValues[4] = CCM->CCGR4 & ~(CCM_CCGR4_CG15(3) | CCM_CCGR4_CG14(3) | CCM_CCGR4_CG13(3) |
                                                CCM_CCGR4_CG12(3)); // Disable encs
            clockGateValues[5] =
                CCM->CCGR5 & ~(CCM_CCGR5_CG4(3)); // Disable kpp, TODO all wdogs have enabled clocks, is it needed?
            clockGateValues[6] = CCM->CCGR6;

            /* Set low power gate values */
            CCM->CCGR0 = CCM_CCGR0_CG0(1) | CCM_CCGR0_CG1(1) | CCM_CCGR0_CG3(3) | CCM_CCGR0_CG11(1) | CCM_CCGR0_CG12(1);
            CCM->CCGR1 = CCM_CCGR1_CG9(3) | CCM_CCGR1_CG10(3) /* was 1, GPT1 is used in wait mode as timebase */ |
                         CCM_CCGR1_CG13(1) | CCM_CCGR1_CG14(1) | CCM_CCGR1_CG15(1);
            CCM->CCGR2 = CCM_CCGR2_CG2(1) | CCM_CCGR2_CG8(1) | CCM_CCGR2_CG9(1) | CCM_CCGR2_CG10(1);
            CCM->CCGR3 = CCM_CCGR3_CG2(3) /* was 1, but SEMC clock needs to be enabled */ | CCM_CCGR3_CG4(1) |
                         CCM_CCGR3_CG9(1) | CCM_CCGR3_CG14(3) | CCM_CCGR3_CG15(1);
            CCM->CCGR4 = CCM_CCGR4_CG1(1) | CCM_CCGR4_CG2(1) | CCM_CCGR4_CG4(1) | CCM_CCGR4_CG5(1) | CCM_CCGR4_CG6(1) |
                         CCM_CCGR4_CG7(1);
            CCM->CCGR5 = CCM_CCGR5_CG0(1) | CCM_CCGR5_CG1(1) | CCM_CCGR5_CG4(1) | CCM_CCGR5_CG6(1) | CCM_CCGR5_CG12(1) |
                         CCM_CCGR5_CG14(3) /* was 1, but SNVS HP is needed for minute IRQ to work */ |
                         CCM_CCGR5_CG15(1);
            /* We can enable DCDC when need to config it and close it after configuration */
            CCM->CCGR6 = CCM_CCGR6_CG3(1) | CCM_CCGR6_CG4(1) | CCM_CCGR6_CG5(1) | CCM_CCGR6_CG9(1) | CCM_CCGR6_CG10(1) |
                         CCM_CCGR6_CG11(1) | CCM_CCGR4_CG15(3); // CG15 TMR3 added for encoder to wakeup
        }

        void restoreLowPowerClockGate()
        {
            CCM->CCGR0 = clockGateValues[0];
            CCM->CCGR1 = clockGateValues[1];
            CCM->CCGR2 = clockGateValues[2];
            CCM->CCGR3 = clockGateValues[3];
            CCM->CCGR4 = clockGateValues[4];
            CCM->CCGR5 = clockGateValues[5];
            CCM->CCGR6 = clockGateValues[6];
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

        checkAndClearPendingIrq();

        setLowPowerClockGate();

        setWaitModeConfig();
        peripheralEnterDozeMode();

        enterSleepMode();

        peripheralExitDozeMode();
        setRunModeConfig();

        restoreLowPowerClockGate();

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
