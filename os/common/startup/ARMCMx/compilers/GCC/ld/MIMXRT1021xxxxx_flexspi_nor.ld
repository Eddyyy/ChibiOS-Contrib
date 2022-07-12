/*
    -----------EXAMPLE HEADER PLS IGNORE----------------------------------
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
    -----------------------------------------------------------------------
*/

/*
 * MIMXRT1021xxxx FlexSPI nor setup
 *
 * RAM0 - BSS, Data, Heap?
 * RAM1 Main Stack, Process Stack, NOCACHE, ETH?
 *
 * FLASH0 - text?
 * FLASH1 - interrupts
 * FLASH2 - flash_config
 * FLASH3 - ivt
 *
 * Static FlexRAM Config (see application note AN12077)
 * Fuse Config Val (0x6D0) -> 0b0110
 * IOMUXC_GPR_GPR17 (FLEXRAM_BANK_CFG) -> 0b0101011110100101; (0x000057A5)
 * IOMUXC_GPR_GPR16 -> 0b0b1000000000000000000111; (0x00200007)
 * IOMUXC_GPR_GPR16_INIT_ITCM_EN_MASK and IOMUXC_GPR_GPR16_INIT_DTCM_EN_MASK not documented in reference manual
 * (^ IOMUXC_GPR_GPR16 && 0b11)
 * IOMUXC_GPR_GPR14 -> CM7_CFGITCMSZ, 0110; CM7_CFGDTCMSZ, 0111; (0x00760000)
 * ITCM and DTCM config for CM7
 * BSS in OCRAM since multiple master access (e.g. DMA) is more efficient
 *
 * flash, flexSPI config
 * 4k+4k+2k+8182k=8MBytes (amount of qspiflash on MIMXRT1021EVK)
 */

/* 
 * TODO Change to work against IMXRT1020 rather than STM32F746xG 
 * TODO Using page 241 of IMXRT1020RM, find out how to add IVT, boot_data and flash_config
 * TODO Need to define __core_init, should also include above Todo
 */
MEMORY
{
    flash0 (RX) : org = 0x60002400, len = 8182k     /* text 8M-10k */
    flash1 (RX) : org = 0x60002000, len = 2k        /* interrupts */
    flash2 (RX) : org = 0x60000000, len = 8k        /* ivt+flash_config */
    flash3 (RX) : org = 0x60000000, len = 0
    flash4 (RX) : org = 0x00000000, len = 0
    flash5 (RX) : org = 0x00000000, len = 0
    flash6 (RX) : org = 0x00000000, len = 0
    flash7 (RX) : org = 0x00000000, len = 0
    ram0   (wx) : org = 0x20200000, len = 160k      /* OCRAM */
    ram1   (wx) : org = 0x20000000, len = 64k       /* DTCM-RAM */
    ram2   (wx) : org = 0x00000000, len = 32k       /* ITCM-RAM */
    ram3   (wx) : org = 0x00000000, len = 0
    ram4   (wx) : org = 0x00000000, len = 0
    ram5   (wx) : org = 0x00000000, len = 0
    ram6   (wx) : org = 0x00000000, len = 0
    ram7   (wx) : org = 0x00000000, len = 0
}

/* For each data/text section two region are defined, a virtual region
   and a load region (_LMA suffix).*/

/* Flash region to be used for exception vectors.(interrupts) */
REGION_ALIAS("VECTORS_FLASH", flash0);
REGION_ALIAS("VECTORS_FLASH_LMA", flash1);

/* Flash region to be used for constructors and destructors.*/
REGION_ALIAS("XTORS_FLASH", flash0);
REGION_ALIAS("XTORS_FLASH_LMA", flash0);

/* Flash region to be used for code text.*/
REGION_ALIAS("TEXT_FLASH", flash0);
REGION_ALIAS("TEXT_FLASH_LMA", flash0);

/* Flash region to be used for read only data.*/
REGION_ALIAS("RODATA_FLASH", flash0);
REGION_ALIAS("RODATA_FLASH_LMA", flash0);

/* Flash region to be used for various.*/
REGION_ALIAS("VARIOUS_FLASH", flash0);
REGION_ALIAS("VARIOUS_FLASH_LMA", flash0);

/* Flash region to be used for RAM(n) initialization data.*/
REGION_ALIAS("RAM_INIT_FLASH_LMA", flash0);

/* RAM region to be used for Main stack. This stack accommodates the processing
   of all exceptions and interrupts.*/
REGION_ALIAS("MAIN_STACK_RAM", ram1);

/* RAM region to be used for the process stack. This is the stack used by
   the main() function.*/
REGION_ALIAS("PROCESS_STACK_RAM", ram1);

/* RAM region to be used for data segment.*/
REGION_ALIAS("DATA_RAM", ram0);
REGION_ALIAS("DATA_RAM_LMA", flash0);

/* RAM region to be used for BSS segment.*/
REGION_ALIAS("BSS_RAM", ram0);

/* RAM region to be used for the default heap.*/
REGION_ALIAS("HEAP_RAM", ram0);

/* Stack rules inclusion.*/
INCLUDE rules_stacks.ld

/*===========================================================================*/
/* Custom sections for IMXRT1020.                                            */
/*===========================================================================*/

/* Idea for boot data definition */
REGION_ALIAS("BOOT_IMAGE", flash2);

SECTIONS 
{
  	.text.headers : {
		KEEP(*(.flashconfig))
		FILL(0xFF)
		. = ORIGIN(flash2) + 0x1000;
		KEEP(*(.ivt))
		KEEP(*(.bootdata))
		. = ALIGN(1024);
	} > BOOT_IMAGE
}


/* RAM region to be used for nocache segment.*/
REGION_ALIAS("NOCACHE_RAM", ram1);

/* RAM region to be used for eth segment.*/
REGION_ALIAS("ETH_RAM", ram1);

SECTIONS
{
    /* Special section for non cache-able areas.*/
    .nocache (NOLOAD) : ALIGN(4)
    {
        __nocache_base__ = .;
        *(.nocache)
        *(.nocache.*)
        *(.bss.__nocache_*)
        . = ALIGN(4);
        __nocache_end__ = .;
    } > NOCACHE_RAM

    /* Special section for Ethernet DMA non cache-able areas.*/
    .eth (NOLOAD) : ALIGN(4)
    {
        __eth_base__ = .;
        *(.eth)
        *(.eth.*)
        *(.bss.__eth_*)
        . = ALIGN(4);
        __eth_end__ = .;
    } > ETH_RAM
}

/* Code rules inclusion.*/
INCLUDE rules_code.ld

/* Data rules inclusion.*/
INCLUDE rules_data.ld

/* Memory rules inclusion.*/
INCLUDE rules_memory.ld

/* Flash size in bytes                                   */
/* _flashimagelen = end flash address - ORIGIN(FLASH);   */
/* the last item placed in flash is from rules_memory.ld */
_flashimagelen = __flash0_free__ - ORIGIN(flash2);

