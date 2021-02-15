// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#ifndef PUREPHONE_POWERMANAGER_HPP
#define PUREPHONE_POWERMANAGER_HPP

#include <functional>

#include "bsp/lpm/bsp_lpm.hpp"
#include "drivers/semc/DriverSEMC.hpp"
#include "CpuGovernor.hpp"

namespace sys
{
    inline constexpr uint32_t frequencyShiftLowerThreshold{40};
    inline constexpr uint32_t frequencyShiftUpperThreshold{60};
    inline constexpr uint32_t maxBelowThresholdCount{50};
    inline constexpr uint32_t maxAboveThresholdCount{3};

    class PowerManager
    {

      public:
        PowerManager();
        ~PowerManager();

        int32_t PowerOff();
        int32_t Reboot();

        /// called periodically to calculate the CPU requirement
        ///
        /// if for the last 'maxAboveThresholdCount' periods the current CPU consumption has been above the set upper
        /// limit (frequencyShiftUpperThreshold), CPU frequency is increased; if for the last 'maxBelowThresholdCount'
        /// periods the current CPU usage was below the lower limit (frequencyShiftLowerThreshold), CPU frequency is
        /// reduced frequency
        /// @param current cpu load
        void UpdateCpuFrequency(uint32_t cpuLoad);

        /// called when the CPU frequency needs to be increased
        /// @note the frequency is always increased to the maximum value
        void IncreaseCpuFrequency() const;

        /// called when the CPU frequency needs to be reduced
        /// @note the frequency is always reduced by one step
        void DecreaseCpuFrequency() const;

        [[nodiscard]] auto getExternalRamDevice() const noexcept -> std::shared_ptr<devices::Device>;

        void RegisterNewSentinel(std::shared_ptr<CpuSentinel> newSentinel) const;
        void SetCpuFrequencyRequest(std::string sentinelName, bsp::CpuFrequencyHz request) const;

      private:
        void ResetFrequencyShiftCounter();
        void SetCpuFrequency(bsp::CpuFrequencyHz freq) const;

        uint32_t belowThresholdCounter{0};
        uint32_t aboveThresholdCounter{0};

        std::unique_ptr<bsp::LowPowerMode> lowPowerControl;
        std::shared_ptr<drivers::DriverSEMC> driverSEMC;
        std::unique_ptr<CpuGovernor> cpuGovernor;
    };

} // namespace sys

#endif // PUREPHONE_POWERMANAGER_HPP
