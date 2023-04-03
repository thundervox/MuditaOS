// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#pragma once

namespace bsp
{
    void allowEnteringWfiMode();
    void blockEnteringWfiMode();
    bool isWfiModeAllowed();
    void enterWfiModeIfAllowed();
} // namespace bsp
