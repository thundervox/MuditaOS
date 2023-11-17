// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "EnterWfiMode.h"
#include <MIMXRT1051.h>
#include <fsl_semc.h>

__attribute__((section(".ocramtext"))) void EnterWfiMode(void)
{
    __DSB();
    __ISB();

    while (!(SEMC->STS0 & (1 << 0))) {} // Wait until SEMC idle

    SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, 0x80000000, kSEMC_SDRAMCM_SelfRefresh, 0, NULL);

    __DSB();
    __WFI();
    __ISB();
}
