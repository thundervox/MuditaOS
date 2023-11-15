// Copyright (c) 2017-2023, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include "EnterWfiMode.h"
#include <MIMXRT1051.h>
#include <fsl_semc.h>

__attribute__((section(".ocramtext"), used, noinline, optimize("O0"))) void EnterWfiMode(void)
{
    //    __DSB();
    //    __ISB();

    __asm__ __volatile__("nop\n"); // These nops are just hooks to put breakpoints on

    while (!(SEMC->STS0 & (1 << 0))) {} // Wait until SEMC idle

    __asm__ __volatile__("nop\n");

    //    IOMUXC_GPR->GPR4 |= (1 << 0); // Request EDMA STOP
    //    while (IOMUXC_GPR->GPR4 & (1 << 16)) {} // Wait until EDMA acknowledges

    //    IOMUXC_GPR->GPR4 |= (1 << 9); // Request SEMC STOP
    //    while (IOMUXC_GPR->GPR4 & (1 << 25)) {} // Wait until SEMC acknowledges

    volatile status_t s = SEMC_SendIPCommand(SEMC, kSEMC_MemType_SDRAM, 0x80000000, kSEMC_SDRAMCM_SelfRefresh, 0, NULL);
    (void)s;

    __asm__ __volatile__("nop\n");

    __DSB();
    __WFI();
    __ISB();

    //    IOMUXC_GPR->GPR4 &= ~(1 << 9); // Wakeup SEMC

    __asm__ __volatile__("nop\n");
}
