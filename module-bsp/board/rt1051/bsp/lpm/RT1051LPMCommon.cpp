// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "RT1051LPMCommon.hpp"
#include <log/log.hpp>
#include <fsl_clock.h>
#include <fsl_gpc.h>
#include <bsp/bsp.hpp>
#include "Oscillator.hpp"
#include "drivers/semc/DriverSEMC.hpp"
#include <hal/boot_reason.h>
#include "DCDC.hpp"

namespace bsp
{
    using namespace drivers;

    namespace
    {
        constexpr std::uint32_t CCM_CLPCR_BYPASS_LPM_HS_BITS{CCM_CLPCR_BYPASS_LPM_HS0_MASK |
                                                             CCM_CLPCR_BYPASS_LPM_HS1_MASK};
    } // namespace

    RT1051LPMCommon::RT1051LPMCommon()
    {
        driverSEMC = drivers::DriverSEMC::Create(drivers::name::ExternalRAM);
        CpuFreq    = std::make_unique<CpuFreqLPM>();
    }

    std::int32_t RT1051LPMCommon::PowerOff()
    {
        board_power_off();
        return 0;
    }

    std::int32_t RT1051LPMCommon::Reboot(RebootType reason)
    {
        switch (reason) {
        case RebootType::GoToRecoveryUpdate:
            set_boot_reason(boot_reason_code_update);
            break;
        case RebootType::GoToRecoveryRecovery:
            set_boot_reason(boot_reason_code_recovery);
            break;
        case RebootType::GoToRecoveryFactoryReset:
            set_boot_reason(boot_reason_code_factory);
            break;
        case RebootType::GoToRecoveryBackup:
            set_boot_reason(boot_reason_code_backup);
            break;
        case RebootType::GoToRecoveryRestore:
            set_boot_reason(boot_reason_code_restore);
            break;
        case RebootType::NormalRestart:
            set_boot_reason(boot_reason_code_os);
            break;
        case RebootType::GoToMSC:
            set_boot_reason(boot_reason_code_usb_mc_mode);
            break;
        default:
            set_boot_reason(boot_reason_code_unknown);
        }
        board_restart();
        return 0;
    }

    enum class Change
    {
        Up,
        Down
    };

    void RT1051LPMCommon::onChangeUp(CpuFrequencyMHz freq, CpuFrequencyMHz newFrequency)
    {
        if ((freq <= CpuFrequencyMHz::Level_1) && (newFrequency > CpuFrequencyMHz::Level_1)) {
            /* Block entering WFI mode */
            BlockEnteringWfiMode();

            /* Switch to external crystal oscillator */
            SwitchOscillatorSource(LowPowerMode::OscillatorSource::External);

            /* Switch external RAM clock source to PLL2 */
            if (driverSEMC) {
                driverSEMC->SwitchToPLL2ClockSource();
            }
        }
    }

    void RT1051LPMCommon::onChangeDown(CpuFrequencyMHz newFrequency)
    {
        if (newFrequency <= CpuFrequencyMHz::Level_1) {
            /* Switch to internal RC oscillator */
            SwitchOscillatorSource(LowPowerMode::OscillatorSource::Internal);

            /* Switch external RAM clock source to OSC */
            if (driverSEMC) {
                driverSEMC->SwitchToPeripheralClockSource();
            }
            /* Allow entering WFI mode */
            AllowEnteringWfiMode();
        }
    }

    void RT1051LPMCommon::SetCpuFrequency(CpuFrequencyMHz freq)
    {
        if (currentFrequency == freq) {
            return;
        }

        /* Switch to overdrive config for any frequency change above 12MHz */
        const auto isChangeAbove12MHz =
            (currentFrequency > CpuFrequencyMHz::Level_1) || (freq > CpuFrequencyMHz::Level_1);
        if (isChangeAbove12MHz) {
            dcdc::SwitchToOverdriveConfig();
        }

        const auto changeDirection = (currentFrequency < freq) ? Change::Up : Change::Down;
        if (Change::Up == changeDirection) {
            onChangeUp(currentFrequency, freq);
        }

        switch (freq) {
        case CpuFrequencyMHz::Level_0:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Osc_4_Mhz);
            break;
        case CpuFrequencyMHz::Level_1:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Osc_12_Mhz);
            break;
        case CpuFrequencyMHz::Level_2:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Osc_24_Mhz);
            break;
        case CpuFrequencyMHz::Level_3:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Pll2_66_Mhz);
            break;
        case CpuFrequencyMHz::Level_4:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Pll2_132_Mhz);
            break;
        case CpuFrequencyMHz::Level_5:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Pll2_264_Mhz);
            break;
        case CpuFrequencyMHz::Level_6:
            CpuFreq->SetCpuFrequency(CpuFreqLPM::CpuClock::CpuClock_Pll2_528_Mhz);
            break;
        }

        /* Switch back to regular config after clock change is done */
        if (isChangeAbove12MHz) {
            dcdc::SwitchToRegularConfig();
        }

        if (Change::Down == changeDirection) {
            onChangeDown(freq);
        }

        const std::string log = "CPU frequency changed to " + std::to_string(CLOCK_GetFreq(kCLOCK_CpuClk)) +
                                ((currentFrequency <= CpuFrequencyMHz::Level_1)
                                     ? (" (WFI time: " + std::to_string(GetLastTimeSpentInWfi()) + " ms)")
                                     : "");
        LOG_INFO(log.c_str());
        currentFrequency = freq;
    }

    std::uint32_t RT1051LPMCommon::GetCpuFrequency() const noexcept
    {
        return CLOCK_GetCpuClkFreq();
    }

    void RT1051LPMCommon::SwitchOscillatorSource(LowPowerMode::OscillatorSource source)
    {
        switch (source) {
        case LowPowerMode::OscillatorSource::Internal:
            SwitchToInternalOscillator();
            break;

        case LowPowerMode::OscillatorSource::External:
            SwitchToExternalOscillator();
            break;
        }
    }

    void RT1051LPMCommon::SetRunModeConfig()
    {
        CCM->CLPCR &= ~(CCM_CLPCR_LPM_MASK | CCM_CLPCR_ARM_CLK_DIS_ON_LPM_MASK);
    }

    void RT1051LPMCommon::SetWaitModeConfig()
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
} // namespace bsp
