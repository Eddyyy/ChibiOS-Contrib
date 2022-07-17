/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

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
            cpsid   i
            nop

            ldr     r3, iomuxc_gpr_base // IOMUXC_GPR_BASE

            // IOMUXC_GPR->GPR17 = 0x000057A5
            ldr     r2, gpr17_init
            str     r2, [r3, #68]       // GPR17, 0x44

            // IOMUXC_GPR->GPR16 = 0x00200007
            ldr     r2, gpr16_init
            str     r2, [r3, #64]       // GPR16, 0x40

            // IOMUXC_GPR->GPR14 = 0x00760000
            ldr     r2, gpr14_init
            str     r2, [r3, #56]       // GPR14, 0x38

            dsb
            isb

            b          _crt0_entry

            iomuxc_gpr_base: .word 0x400AC000
#if defined(MIMXRT1021EVK_EVAL_BRD)
            gpr17_init: .word 0x000057A5
            gpr16_init: .word 0x00200007
            gpr14_init: .word 0x00760000
#endif
