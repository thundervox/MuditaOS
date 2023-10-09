// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "drivers/pwm/DriverPWM.hpp"

#include "fsl_common.h"
#include "fsl_pwm.h"

#include <mutex.hpp>

#include <vector>
#include <map>
#include <array>

namespace drivers
{
    class RT1051DriverPWM final : public DriverPWM
    {
      public:
        RT1051DriverPWM(PWMInstances inst, PWMModules mod, const DriverPWMParams &params);

        ~RT1051DriverPWM();

        void InitNextChannel(const DriverPWMParams &params) final;

        void SetDutyCycle(float dutyCyclePercent, PWMChannel channel) final;

        void Start(PWMChannel channel) final;

        void Stop(PWMChannel channel) final;

        void UpdateClockFrequency(bsp::CpuFrequencyMHz newFrequency) final;

        enum class PwmState
        {
            Off,
            On,
        };

      private:
        struct PwmSignalConfig
        {
          public:
            pwm_channels_t pwmChannel;
            float dutyCyclePercent;
            pwm_level_select_t level;
            std::uint16_t deadtimeValue;
            pwm_fault_state_t faultState;

            pwm_signal_param_t getFslConfig()
            {
                const pwm_signal_param_t config = {.pwmChannel       = pwmChannel,
                                                   .dutyCyclePercent = static_cast<std::uint8_t>(dutyCyclePercent),
                                                   .level            = level,
                                                   .deadtimeValue    = deadtimeValue,
                                                   .faultState       = faultState};
                return config;
            }
        };

        PwmState GetPwmState();

        void SetupPWMChannel(PWMChannel channel);

        void SetupPWMInstance(pwm_signal_param_t *config, unsigned numOfChannels, std::uint32_t clockFrequency);

        void ForceLowOutput(PWMChannel channel);

        void RestorePwmOutput(PWMChannel channel);

        pwm_channels_t getChannelMask(PWMChannel channel);

        bool channelEnabled(PWMChannel channel);

        bool otherChannelRunning(PWMChannel channel);

        void stopAll();

        void startAll();

        void restoreDutyCycle();

        PWM_Type *base = nullptr;

        pwm_submodule_t pwmModule = kPWM_Module_0;

        std::array<PwmSignalConfig, 2> pwmSignalsConfig;

        std::vector<PWMChannel> enabledChannels;
        std::map<PWMChannel, PwmState> pwmChannelState = {
            {PWMChannel::A, PwmState::Off}, {PWMChannel::B, PwmState::Off}, {PWMChannel::X, PwmState::Off}};

        cpp_freertos::MutexStandard frequencyChangeMutex;

        std::uint32_t clockFrequency = 0;
    };
} // namespace drivers
