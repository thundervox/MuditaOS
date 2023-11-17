// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>
//#include <fsl_pmu.h>
#include <fsl_rtwdog.h>
#include <fsl_gpc.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <ticks.hpp>
#include <timers.h>
#include <MIMXRT1051.h>

//#include <magic_enum.hpp>

/* TODO
 * There's some weird .ARM.exidx "relocation truncated to fit: R_ARM_PREL31" link time error when
 * trying to relocate C++ function to OCRAM, seems to be something exception handling related. This
 * needs to be investigated, for now let's use this quick hack. */
extern "C"
{
#include "EnterWfiMode.h"
}

namespace bsp
{
    namespace
    {
        /* RTC wakes up CPU every minute, so go to sleep only if next timer will
         * trigger after more than minute - this way no event will ever be missed */
        constexpr auto timersInactivityTimeMs{60 * utils::time::milisecondsInSecond};

        constexpr std::uint32_t CCM_CLPCR_BYPASS_LPM_HS_BITS{CCM_CLPCR_BYPASS_LPM_HS0_MASK |
                                                             CCM_CLPCR_BYPASS_LPM_HS1_MASK};

        bool wfiModeAllowed = false;
        std::uint32_t timeSpentInWFI;

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

        bool isWfiModeAllowed()
        {
            return wfiModeAllowed;
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

        void SetRunModeConfig()
        {
            CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
        }

        void SetWaitModeConfig()
        {
            uint32_t clpcr;

            /*
             * ERR050143: CCM: When improper low-power sequence is used,
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
            clpcr      = CCM->CLPCR & (~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK));
            CCM->CLPCR = clpcr | CCM_CLPCR_LPM(kCLOCK_ModeWait) | CCM_CLPCR_MASK_SCU_IDLE_MASK |
                         CCM_CLPCR_MASK_L2CC_IDLE_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK | CCM_CLPCR_STBY_COUNT_MASK |
                         CCM_CLPCR_BYPASS_LPM_HS_BITS;
            GPC_DisableIRQ(GPC, GPR_IRQ_IRQn);
        }

        bool isBrownoutOn2P5Output()
        {
            return (PMU_REG_2P5_BO_VDD2P5_MASK == (PMU_REG_2P5_BO_VDD2P5_MASK & PMU->REG_2P5));
        }
    } // namespace

    void allowEnteringWfiMode()
    {
        wfiModeAllowed = true;
    }

    /* Block WFI mode so that OS wakes up fully and goes to sleep only after
     * frequency has dropped back to the lowest level */
    void blockEnteringWfiMode()
    {
        wfiModeAllowed = false;
    }

    std::uint32_t getLastTimeSpentInWfi()
    {
        return timeSpentInWFI;
    }

    std::uint32_t enterWfiModeIfAllowed()
    {
        if (!isWfiModeAllowed()) {
            return 0;
        }
        timeSpentInWFI = 0;
        if (isTimerTaskScheduledSoon()) {
            blockEnteringWfiMode();
            return 0;
        }
        if (isBrownoutOn2P5Output()) {
            LOG_WARN("WFI disabled - brownout detected on 2P5 VDD output");
            blockEnteringWfiMode();
            return 0;
        }

        const auto enterWfiTimerTicks = ulHighFrequencyTimerTicks();

        RTWDOG_Refresh(RTWDOG);
        SetWaitModeConfig();
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

        //        for (size_t irq = DMA0_DMA16_IRQn; irq <= NMI_WAKEUP_IRQn; ++irq) {
        //            NVIC_ClearPendingIRQ((IRQn_Type)irq);
        //        }

        __disable_irq();
        __DSB();
        __ISB();

        RTWDOG_Unlock(RTWDOG);
        RTWDOG_Disable(RTWDOG);

        /* Clear the SLEEPDEEP bit to go into sleep mode (WAIT) */
        SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;

        EnterWfiMode();

        __NOP();
        __NOP();
        __NOP();
        __NOP();
        peripheralExitDozeMode();

        RTWDOG_Unlock(RTWDOG);
        RTWDOG_Enable(RTWDOG);

        __enable_irq();

        RTWDOG_Refresh(RTWDOG);

        blockEnteringWfiMode();
        SetRunModeConfig();

        const auto exitWfiTimerTicks = ulHighFrequencyTimerTicks();
        timeSpentInWFI = highFrequencyTimerTicksToMs(utils::computeIncrease(exitWfiTimerTicks, enterWfiTimerTicks));
        xTaskCatchUpTicks(cpp_freertos::Ticks::MsToTicks(timeSpentInWFI));
        LOG_ERROR("!!! Leaving WFI after %lums!!!", timeSpentInWFI);
        return timeSpentInWFI;
    }
} // namespace bsp
