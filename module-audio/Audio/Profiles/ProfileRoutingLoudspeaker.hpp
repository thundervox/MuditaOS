// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md
#pragma once

#include "Profile.hpp"

namespace audio
{

    class ProfileRoutingLoudspeaker : public Profile
    {
      public:
        ProfileRoutingLoudspeaker(Volume volume, Gain gain)
            : Profile("Routing Speakerphone",
                      Type::RoutingLoudspeaker,
                      audio::codec::Configuration{
                          .sampleRate_Hz = 16000,
                          .bitWidth      = 16,
                          .flags         = static_cast<uint32_t>(
                                       audio::codec::Flags::InputLeft) | // microphone use left audio channel
                                   static_cast<uint32_t>(audio::codec::Flags::OutputMono),
                          .outputVolume = static_cast<float>(volume),
                          .inputGain    = static_cast<float>(gain),
                          .inputPath    = audio::codec::InputPath::Microphone,
                          .outputPath   = audio::codec::OutputPath::Loudspeaker},
                      AudioDevice::Type::Audiocodec)
        {}
    };

} // namespace audio
