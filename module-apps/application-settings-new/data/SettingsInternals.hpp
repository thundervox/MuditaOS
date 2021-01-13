// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

#include <cstdint>

namespace settingsInternals
{
    enum class ListItemName
    {
        Name,
        APN,
        Proxy,
        Port,
        Username,
        Password,

        Server,
        MMSC,
        MmsProxy,
        MmsPort,
        MCC,
        MNC,
        AuthType,
        ApnType,
        ApnProtocol
    };

} // namespace settingsInternals
