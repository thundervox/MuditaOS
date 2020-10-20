// Copyright (c) 2017-2020, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include "../Events.hpp"

namespace harness
{

    struct SysStart : public Event<harness::Events::SysStart>
    {
        // encoded json event
        auto encode() -> std::string final;
    };

}; // namespace harness
