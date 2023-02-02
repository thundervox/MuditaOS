// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "RT1051DriverPLL2.hpp"
#include "board/rt1051/bsp/lpm/ClockState.hpp"

#include "bsp/pwr/lpm.h"

namespace drivers
{
    namespace constants
    {
        inline constexpr auto REFTOP_LOWPOWER_FLAG{0x00000004};
    } // namespace constants

    RT1051DriverPLL2::RT1051DriverPLL2() noexcept
    {
        if (!IsPLL2Enabled()) {
            /* Restore bandgap */
            /* Turn on regular bandgap and wait for stable */
            CCM_ANALOG->MISC0_CLR = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;
            while ((CCM_ANALOG->MISC0 & CCM_ANALOG_MISC0_REFTOP_VBGUP_MASK) == 0) {}
            /* Low power band gap disable */
            XTALOSC24M->LOWPWR_CTRL_CLR = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
            PMU->MISC0_CLR              = 0x00000004;

            /* Wait CCM operation finishes */
            CLOCK_CCM_HANDSHAKE_WAIT();
            /* Take some delay */
            LPM_DELAY(40);

            clkPLL2setup(CLK_ENABLE);
        }
    }

    RT1051DriverPLL2::~RT1051DriverPLL2()
    {
        if ((CLOCK_GetMux(kCLOCK_SemcMux) == SemcMuxPeripheralClock) && !bsp::IsClockEnabled(kCLOCK_Lpspi1) &&
            !bsp::IsClockEnabled(kCLOCK_Lpspi2) && !bsp::IsClockEnabled(kCLOCK_Lpspi3) &&
            !bsp::IsClockEnabled(kCLOCK_Lpspi4) && !bsp::IsClockEnabled(kCLOCK_Usdhc1) &&
            !bsp::IsClockEnabled(kCLOCK_Usdhc2)) {

            clkPLL2setup(CLK_DISABLE); //    <---- ???

            /* Switch bandgap */
            PMU->MISC0_SET              = 0x00000004;
            XTALOSC24M->LOWPWR_CTRL_SET = XTALOSC24M_LOWPWR_CTRL_LPBG_SEL_MASK;
            PMU->MISC0_SET              = CCM_ANALOG_MISC0_REFTOP_PWD_MASK;

            /* Wait CCM operation finishes */
            CLOCK_CCM_HANDSHAKE_WAIT();
            /* Take some delay */
            LPM_DELAY(40);
        }
    }

    bool RT1051DriverPLL2::IsPLL2Enabled() const noexcept
    {
        return !(CCM_ANALOG->PLL_SYS & CCM_ANALOG_PLL_SYS_POWERDOWN_MASK);
    }

} // namespace drivers
