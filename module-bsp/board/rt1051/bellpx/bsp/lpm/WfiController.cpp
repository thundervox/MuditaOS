// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "WfiController.hpp"
#include <fsl_common.h>

namespace bsp
{
    void enterWfiMode()
    {
        __DSB();
        __WFI();
        __ISB();
    }
} // namespace bsp
