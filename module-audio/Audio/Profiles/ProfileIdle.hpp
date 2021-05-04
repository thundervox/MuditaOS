// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Profile.hpp"

namespace audio
{

    class ProfileIdle : public Profile
    {
      public:
        ProfileIdle() : Profile("Idle", Type::Idle, AudioDevice::Configuration{}, AudioDevice::Type::None)
        {}
    };

} // namespace audio
