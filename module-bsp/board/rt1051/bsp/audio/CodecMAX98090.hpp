// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

/*
 *  @file MAX98090.hpp
 *  @author Mateusz Piesta (mateusz.piesta@mudita.com)
 *  @date 30.07.19
 *  @brief
 *  @copyright Copyright (C) 2019 mudita.com
 *  @details
 */

#ifndef PUREPHONE_CODECMAX98090_HPP
#define PUREPHONE_CODECMAX98090_HPP

#include "Codec.hpp"
#include "drivers/i2c/DriverI2C.hpp"

class CodecParamsMAX98090 : public CodecParams
{
  public:
    enum class Cmd
    {
        SetOutVolume,
        SetInGain,
        SetMute,
        SetOutput,
        SetInput,
        Reset,
        MicBiasCtrl,
        None
    };

    enum class SampleRate
    {
        Rate8KHz   = 8000,
        Rate16KHz  = 16000,
        Rate44K1Hz = 44100,
        Rate48KHz  = 48000,
        Rate32KHz  = 32000,
        Rate96KHz  = 96000,
        Invalid
    };

    enum class OutputPath
    {
        Headphones,
        HeadphonesMono,
        Earspeaker,
        Loudspeaker,
        None
    };

    enum class InputPath
    {
        Headphones,
        Microphone,
        None
    };

    enum class MonoStereo
    {
        Mono,
        Stereoq
    };

    static SampleRate ValToSampleRate(uint32_t rate)
    {
        switch (rate) {
        case 8000:
            return SampleRate ::Rate8KHz;
        case 16000:
            return SampleRate::Rate16KHz;
        case 32000:
            return SampleRate::Rate32KHz;
        case 44100:
            return SampleRate::Rate44K1Hz;
        case 48000:
            return SampleRate::Rate48KHz;
        case 96000:
            return SampleRate::Rate96KHz;
        default:
            return SampleRate ::Invalid;
        }
    }

    uint32_t GetSampleRateVal()
    {
        switch (sampleRate) {
        case SampleRate::Rate8KHz:
            return 8000;
        case SampleRate::Rate16KHz:
            return 16000;
        case SampleRate::Rate32KHz:
            return 32000;
        case SampleRate::Rate44K1Hz:
            return 44100;
        case SampleRate::Rate48KHz:
            return 48000;
        case SampleRate::Rate96KHz:
            return 96000;
        default:
            return 0;
        }
    }

    Cmd opCmd             = Cmd::None;
    float outVolume       = 0;
    float inGain          = 0;
    bool muteEnable       = false;
    bool resetEnable      = false;
    bool micBiasEnable    = false;
    InputPath inputPath   = InputPath ::None;
    OutputPath outputPath = OutputPath ::None;
    SampleRate sampleRate = SampleRate ::Rate44K1Hz;
};

class CodecMAX98090 : public Codec
{
  public:
    CodecMAX98090();
    ~CodecMAX98090();

    std::optional<uint32_t> Probe() override final;

    CodecRetCode Start(const CodecParams &param) override final;

    CodecRetCode Pause() override final;

    CodecRetCode Resume() override final;

    CodecRetCode Stop() override final;

    CodecRetCode Ioctrl(const CodecParams &param) override final;

  private:
    std::shared_ptr<drivers::DriverI2C> i2c;
    drivers::I2CAddress i2cAddr;
    CodecParamsMAX98090 currentParams;

    CodecRetCode SetOutputVolume(const float vol);
    CodecRetCode SetInputGain(const float gain);
    CodecRetCode SetMute(const bool enable);
    CodecRetCode SetInputPath(const CodecParamsMAX98090::InputPath path);
    CodecRetCode SetOutputPath(const CodecParamsMAX98090::OutputPath path);
    CodecRetCode MicBias(const bool enable);
    CodecRetCode WriteFilterCoeff(const float coeff, const uint8_t basereg);
    CodecRetCode Reset();
};

#endif // PUREPHONE_CODECMAX98090_HPP
