#include "cmparams.h"
#include "MIMXRT1021.h"


        .text

        .align      2
        .thumb_func
        .globl       Reset_Handler
Reset_Handler:
        // IOMUXC_GPR_GPR17 = 0x000057A5
        // IOMUXC_GPR_GPR16 = 0x00200007
        // IOMUXC_GPR_GPR14 = 0x00760000
         b          _crt0_entry
