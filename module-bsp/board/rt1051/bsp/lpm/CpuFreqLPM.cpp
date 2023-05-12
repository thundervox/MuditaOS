// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "CpuFreqLPM.hpp"
#include "fsl_dcdc.h"

namespace bsp
{
    CpuFreqLPM::CpuFreqLPM()
    {}

    void CpuFreqLPM::SetCpuFrequency(CpuFreqLPM::CpuClock freq)
    {
        SetHighestCoreVoltage();

        switch (freq) {
        case CpuClock::CpuClock_Osc_4_Mhz:
            CLOCK_SetDiv(kCLOCK_PeriphClk2Div, PeriphClk2Div3);
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv2);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourceOsc_24MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_950_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Osc_12_Mhz:
            CLOCK_SetDiv(kCLOCK_PeriphClk2Div, PeriphClk2Div1);
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv2);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourceOsc_24MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_975_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Osc_24_Mhz:
            CLOCK_SetDiv(kCLOCK_PeriphClk2Div, PeriphClk2Div1);
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv1);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourceOsc_24MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_1050_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Pll2_66_Mhz:
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv8);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourcePll2_528MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_1075_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Pll2_132_Mhz:
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv4);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourcePll2_528MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_1100_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Pll2_264_Mhz:
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv2);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourcePll2_528MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_1125_mV, VDDStandby_925_mV);
            break;
        case CpuClock::CpuClock_Pll2_528_Mhz:
            CLOCK_SetDiv(kCLOCK_AhbDiv, AhbDiv1);
            CLOCK_SetMux(kCLOCK_PeriphMux, PeriphMuxSourcePll2_528MHz);
            DCDC_AdjustTargetVoltage(DCDC, VDDRun_1150_mV, VDDStandby_925_mV);
            break;
        }

        /* Set SystemCoreClock variable. */
        SystemCoreClockUpdate();
    }

    void CpuFreqLPM::SetHighestCoreVoltage()
    {
        DCDC_AdjustTargetVoltage(DCDC, VDDRun_1275_mV, VDDStandby_925_mV);
    }
} // namespace bsp
