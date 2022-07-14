
#include "boot.h"
#include "cmparams.h"
#include "MIMXRT1021.h"

/**
 * @file    boot.S
 * @brief   IMXRT1021 specific startup file for ChibiOS.
 *
 * Configures FlexRam before any startup code is used.
 */


            .text

            .align      2
            .thumb_func
            .globl       Reset_Handler
Reset_Handler:
            movw    r3, IOMUXC_GPR_BASE

            // IOMUXC_GPR->GPR17 = 0x000057A5
            movw    r2, 0x000057A5
            str     r2, [r3, 0x44]

            // IOMUXC_GPR->GPR16 = 0x00200007
            movw    r2, 0x00200007
            str     r2, [r3, 0x40]

            // IOMUXC_GPR->GPR14 = 0x00760000
            movw    r2, 0x00760000
            str     r2, [r3, 0x38]

            dsb
            isb

            b          _crt0_entry
