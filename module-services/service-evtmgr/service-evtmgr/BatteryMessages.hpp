﻿// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "Message.hpp"

#include <module-bsp/bsp/torch/torch.hpp>

namespace sevm
{
    class BatteryStatusChangeMessage : public Message
    {};

    class BatterySetCriticalLevel : public Message
    {
      public:
        BatterySetCriticalLevel(std::uint8_t level) : criticalLevel(level)
        {}
        const unsigned int criticalLevel = 0;
    };

    class BatteryStateChangeMessage : public Message
    {};

    class BatteryBrownoutMessage : public Message
    {};

} // namespace sevm
