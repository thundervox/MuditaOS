// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <cstdint>
#include <fsl_common.h>
#include <fsl_pmu.h>
#include <log/log.hpp>

namespace bsp
{
    namespace
    {
        constexpr auto OutputVoltage2P5 = 0x1BU; // 2.775V
        constexpr auto OffsetVoltage2P5 = 0x03U; // 3*25mV

        constexpr auto mVPerStep = 25U;

        std::uint32_t ldoValueToVoltage(std::uint32_t value)
        {
            constexpr auto mVMin = 2100U;
            return mVMin + mVPerStep * value;
        }

        std::uint32_t brownoutOffsetValueToVoltage(std::uint32_t value)
        {
            return mVPerStep * value;
        }
    } // namespace

    void Brownout_init()
    {
        PMU_2P5nableBrownout(PMU, true);
        PMU_2P5SetRegulatorOutputVoltage(PMU, OutputVoltage2P5);
        PMU_2P5SetBrownoutOffsetVoltage(PMU, OffsetVoltage2P5);
        PMU_2P5EnableOutput(PMU, true);

        LOG_PRINTF("[Brownout]: 2P5 LDO output voltage: %" PRIu32 "mV", ldoValueToVoltage(OutputVoltage2P5));
        LOG_PRINTF("[Brownout]: 2P5 brownout offset voltage: %" PRIu32 "mV",
                   brownoutOffsetValueToVoltage(OffsetVoltage2P5));
    }
} // namespace bsp
