// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>
#include <fsl_pmu.h>
#include <fsl_gpc.h>
#include <Utils.hpp>
#include <time/time_constants.hpp>
#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>
#include <ticks.hpp>
#include <watchdog/watchdog.hpp>

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

        void setWaitModeConfig()
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

        void setRunModeConfig()
        {
            CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
        }

        void enterSleepMode()
        {
            const auto savedPrimask = DisableGlobalIRQ();
            __DSB();
            __ISB();

            bsp::watchdog::disable();

            SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
            __WFI();

            bsp::watchdog::enable();

            EnableGlobalIRQ(savedPrimask);
            __DSB();
            __ISB();
        }

        void setLowPowerClockGate()
        {
            /* CCM->CCGR0 */
            CLOCK_ControlGate(kCLOCK_Aips_tz1, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Aips_tz2, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Mqs, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_FlexSpiExsc, kCLOCK_ClockNeededRunWait); // reserved
            CLOCK_ControlGate(kCLOCK_Sim_M_Main, kCLOCK_ClockNeededRun);      // TODO not sure
            CLOCK_ControlGate(kCLOCK_Dcp, kCLOCK_ClockNotNeeded);             // TODO not sure
            CLOCK_ControlGate(kCLOCK_Lpuart3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Can1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Can1S, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Can2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Can2S, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Trace, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpt2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpt2S, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpio2, kCLOCK_ClockNeededRunWait);

            /* CCM->CCGR1 */
            CLOCK_ControlGate(kCLOCK_Lpspi1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpspi2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpspi3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpspi4, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Adc2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Enet, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Pit, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Aoi2, kCLOCK_ClockNotNeeded); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Adc1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_SemcExsc, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Gpt1, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Gpt1S, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart4, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpio1, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Csu, kCLOCK_ClockNotNeeded); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Gpio5, kCLOCK_ClockNeededRunWait);

            /* CCM->CCGR2 */
            CLOCK_ControlGate(kCLOCK_OcramExsc, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Csi, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_IomuxcSnvs, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Lpi2c1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpi2c2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpi2c3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Ocotp, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Xbar3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Ipmux1, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Ipmux2, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Ipmux3, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Xbar1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Xbar2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpio3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lcd, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Pxp, kCLOCK_ClockNotNeeded);

            /* CCM->CCGR3 */
            CLOCK_ControlGate(kCLOCK_Flexio2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart5, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Semc, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Lpuart6, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Aoi1, kCLOCK_ClockNotNeeded); // TODO not sure
            CLOCK_ControlGate(kCLOCK_LcdPixel, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Gpio4, kCLOCK_ClockNotNeeded);   // TODO not sure
            CLOCK_ControlGate(kCLOCK_Ewm0, kCLOCK_ClockNotNeeded);    // TODO not sure
            CLOCK_ControlGate(kCLOCK_Wdog1, kCLOCK_ClockNotNeeded);   // TODO not sure
            CLOCK_ControlGate(kCLOCK_FlexRam, kCLOCK_ClockNeededRun); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Acmp1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Acmp2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Acmp3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Acmp4, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Ocram, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_IomuxcSnvsGpr, kCLOCK_ClockNeededRun);

            /* CCM->CCGR4 */
            CLOCK_ControlGate(kCLOCK_Sim_m7_clk_r, kCLOCK_ClockNeededRun); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Iomuxc, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_IomuxcGpr, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Bee, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_SimM7, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Tsc, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_SimM, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_SimEms, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Pwm1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Pwm2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Pwm3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Pwm4, kCLOCK_ClockNeededRun); // Only PWM4 is used in Harmony
            CLOCK_ControlGate(kCLOCK_Enc1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Enc2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Enc3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Enc4, kCLOCK_ClockNotNeeded);

            /* CCM->CCGR5 */
            CLOCK_ControlGate(kCLOCK_Rom, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Flexio1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Wdog3, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Dma, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Kpp, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Wdog2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Aips_tz4, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Spdif, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_SimMain, kCLOCK_ClockNeededRun); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Sai1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Sai2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Sai3, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart7, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_SnvsHp, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_SnvsLp, kCLOCK_ClockNeededRunWait);

            /* CCM->CCGR6 */
            CLOCK_ControlGate(kCLOCK_UsbOh3, kCLOCK_ClockNeededRun); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Usdhc1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Usdhc2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Dcdc, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Ipmux4, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_FlexSpi, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Trng, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Lpuart8, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Timer4, kCLOCK_ClockNotNeeded); // TODO not sure
            CLOCK_ControlGate(kCLOCK_Aips_tz3, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_SimPer, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Anadig, kCLOCK_ClockNeededRunWait);
            CLOCK_ControlGate(kCLOCK_Lpi2c4, kCLOCK_ClockNeededRun);
            CLOCK_ControlGate(kCLOCK_Timer1, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Timer2, kCLOCK_ClockNotNeeded);
            CLOCK_ControlGate(kCLOCK_Timer3, kCLOCK_ClockNeededRunWait);
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

        bsp::watchdog::refresh();
        checkAndClearPendingIrq();

        setLowPowerClockGate(); // In case something changed it
        setWaitModeConfig();
        peripheralEnterDozeMode();

        enterSleepMode();

        peripheralExitDozeMode();
        setRunModeConfig();
        bsp::watchdog::refresh();

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
