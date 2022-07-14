
#include "MIMXRT1021.h"

extern void Reset_Handler(void);
extern unsigned long _flashimagelen;

/* Lookup table related defintions */
#define CMD_INDEX_READ 0
#define CMD_INDEX_READSTATUS 1
#define CMD_INDEX_WRITEENABLE 2
#define CMD_INDEX_WRITE 4

#define CMD_LUT_SEQ_IDX_READ 0
#define CMD_LUT_SEQ_IDX_READSTATUS 1
#define CMD_LUT_SEQ_IDX_WRITEENABLE 3
#define CMD_LUT_SEQ_IDX_WRITE 9

/* SPI Command types */
#define CMD_SDR 0x01
#define CMD_DDR 0x21
#define RADDR_SDR 0x02
#define RADDR_DDR 0x22
#define CADDR_SDR 0x03
#define CADDR_DDR 0x23
#define MODE1_SDR 0x04
#define MODE1_DDR 0x24
#define MODE2_SDR 0x05
#define MODE2_DDR 0x25
#define MODE4_SDR 0x06
#define MODE4_DDR 0x26
#define MODE8_SDR 0x07
#define MODE8_DDR 0x27
#define WRITE_SDR 0x08
#define WRITE_DDR 0x28
#define READ_SDR 0x09
#define READ_DDR 0x29
#define LEARN_SDR 0x0A
#define LEARN_DDR 0x2A
#define DATSZ_SDR 0x0B
#define DATSZ_DDR 0x2B
#define DUMMY_SDR 0x0C
#define DUMMY_DDR 0x2C
#define DUMMY_RWDS_SDR 0x0D
#define DUMMY_RWDS_DDR 0x2D
#define JMP_ON_CS 0x1F
#define STOP 0

#define FLEXSPI_1PAD 0
#define FLEXSPI_2PAD 1
#define FLEXSPI_4PAD 2
#define FLEXSPI_8PAD 3

#define FLEXSPI_LUT_SEQ(cmd0, pad0, op0, cmd1, pad1, op1) \
    (FLEXSPI_LUT_OPERAND0(op0) | FLEXSPI_LUT_NUM_PADS0(pad0) | FLEXSPI_LUT_OPCODE0(cmd0) | \
     FLEXSPI_LUT_OPERAND1(op1) | FLEXSPI_LUT_NUM_PADS1(pad1) | FLEXSPI_LUT_OPCODE1(cmd1))

/* Progam image data structures */

__attribute__ ((section(".bootdata"), used))
const uint32_t BootData[3] = {
	0x60000000, // Flash base -> boot start location
	(uint32_t)&_flashimagelen, // flash size?
	0 // Plugin flag
};


__attribute__ ((section(".csf"), used))
const uint32_t hab_csf[768];	// placeholder for HAB signature

/*
 * Not needed since Reset_handler will now configure the FlexRam
__attribute__ ((section(".dcd"), used))
const uint32_t dcdTable[64] = {
    0x410020D2,         // Version,Length,Tag   (Header)

    // This may not be needed if __core_init() is safe
    // Small gdb testing suggests this might not have an effect
    // But also that __core_init() may be safe
    0xCC001C04,         // Write CMD, Length, Location Byte Size
    // Address,Value
    0x400AC044,0x000057A5,  // IOMUXC_GPR_GPR17 5FA5, 57A5
    0x400AC040,0x00200007,  // IOMUXC_GPR_GPR16
    0x400AC038,0x00760000   // IOMUXC_GPR_GPR14
};
*/

__attribute__ ((section(".ivt"), used))
const uint32_t ImageVectorTable[8] = {
	0x432000D1,		            // header
	(uint32_t)&Reset_Handler,   // program entry
	0,			                // reserved
	0, // (uint32_t)dcdTable,	// dcd
	(uint32_t)BootData,	        // abs address of boot data
	(uint32_t)ImageVectorTable, // self
	(uint32_t)hab_csf,	        // command sequence file
	0			                // reserved
};



// Since uint32_t is 32bits the 1 value has 4 bytes (as in reference manual)
__attribute__ ((section(".flashconfig"), used))
uint32_t FlexSPI_NOR_Config[128] = {
	// 448 byte common FlexSPI configuration block, 8.6.3.1 page 223 (RT1060 rev 0)
	// 448 byte common FlexSPI configuration block, 9.6.3.1 page 207 (RT1020 rev ?)
	// MCU_Flashloader_Reference_Manual.pdf, 8.2.1, Table 8-2, page 72-75
    // Value,       // Name             (offset)
	0x42464346,		// Tag				0x000
	0x56010000,		// Version          0x004
	0,			    // reserved
	0x00030301,		// columnAdressWidth,dataSetupTime,dataHoldTime,readSampleClkSrc
                    // ->               0x00C (note: data == cs)

	0x00000000,		// waitTimeCfgCommands,-,deviceModeCfgEnable
                    // ->               0x010

	0,			    // deviceModeSeq    0x014
	0, 			    // deviceModeArg    0x018
	0x00000000,		// -,-,-,configCmdEnable
                    // ->               0x01C

	0,			    // configCmdSeqs[3:0]	0x020
	0,			    // configCmdSeqs[7:4]
	0,			    // configCmdSeqs[11:8]
	0,              // reserved

	0,			    // cfgCmdArgs[0:3]		0x030
	0,			    // cfgCmdArgs[7:4]
	0,			    // cfgCmdArgs[11:8]
	0,              // reserved

	0x00000000,		// controllerMiscOption
                    // ->               0x040
#if defined(MIMXRT1021EVK_EVAL_BRD)
	0x00060401,		// lutCustomSeqEnable,serialClkFreq,sflashPadType,deviceType
                    // ->               0x044 (100Mhz serialClkFreq)
#else
	0x00080401,		// lutCustomSeqEnable,serialClkFreq,sflashPadType,deviceType
                    // ->               0x044 (30Mhz serialClkFreq)
#endif
	0,			    // reserved
	0,			    // reserved

#if defined(ARDUINO_TEENSY40)
	0x00200000,		// sflashA1Size			0x050
#elif defined(ARDUINO_TEENSY41)
	0x00800000,		// sflashA1Size			0x050
#elif defined(ARDUINO_TEENSY_MICROMOD)
	0x01000000,		// sflashA1Size			0x050
#elif defined(MIMXRT1021EVK_EVAL_BRD)
	0x00800000,		// sflashA1Size			0x050
#else
#error "Unknow flash chip size";
#endif
	0,			    // sflashA2Size     0x054
	0,			    // sflashB1Size     0x058
	0,			    // sflashB2Size     0x05C

	0,			    // csPadSettingOverride		0x060
	0,			    // sclkPadSettingOverride   0x064
	0,			    // dataPadSettingOverride   0x068
	0,			    // dqsPadSettingOverride    0x06C

	0,			    // timeoutInMs	    0x070
	0,			    // commandInterval  0x074
	0,			    // dataValidTime    0x078
	0x00000000,		// busyBitPolarity,busyOffset
                    // ->               0x07C

    // See IMXRT1020RM FlexSPI section 26.7.7 Look Up Table page 1487
    // and FlexSPI section 26.7.8 Programmable Sequence Engine page 1498

    // LUT Index 0 Read
	0x0A1804EB,		// lookupTable[0]		0x080
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_2PAD, 0xEB, RADDR_SDR, FLEXSPI_4PAD, 0x18)
	0x26043206,		// lookupTable[1]
    // FLEXSPI_LUT_SEQ(DUMMY_SDR, FLEXSPI_4PAD, 0x06, READ_SDR, FLEXSPI_4PAD, 0x04)
	0,			    // lookupTable[2]
	0,			    // lookupTable[3]

    // LUT Index 1 ReadStatus
	0x24040405,		// lookupTable[4]		0x090
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x05, READ_SDR, FLEXSPI_4PAD, 0x04)
	0,			    // lookupTable[5]
	0,			    // lookupTable[6]
	0,			    // lookupTable[7]

    // LUT Index 2 Reserved
	0,			    // lookupTable[8]		0x0A0
	0,			    // lookupTable[9]
	0,			    // lookupTable[10]
	0,			    // lookupTable[11]

    // LUT Index 3 WriteEnable
	0x00000406,		// lookupTable[12]		0x0B0
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x06, STOP, FLEXSPI_1PAD, 0x0)
	0,			    // lookupTable[13]
	0,			    // lookupTable[14]
	0,			    // lookupTable[15]

    // LUT Index 4 Reserved
	0,			    // lookupTable[16]		0x0C0
	0,			    // lookupTable[17]
	0,			    // lookupTable[18]
	0,			    // lookupTable[19]

    // LUT Index 5 EraseSector
	0x08180420,		// lookupTable[20]		0x0D0
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x20, RADDR_SDR, FLEXSPI_1PAD, 0x18)
	0,			    // lookupTable[21]
	0,			    // lookupTable[22]
	0,			    // lookupTable[23]

    // LUT Index 6 Reserved
	0,			    // lookupTable[24]		0x0E0
	0,			    // lookupTable[25]
	0,			    // lookupTable[26]
	0,			    // lookupTable[27]

    // LUT Index 7 Reserved
	0,			    // lookupTable[28]		0x0F0
	0,			    // lookupTable[29]
	0,			    // lookupTable[30]
	0,			    // lookupTable[31]

    // LUT Index 8 Reserved
	0x081804D8,		// lookupTable[32]		0x100
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0xD8, RADDR_SDR, FLEXSPI_1PAD, 0x018)
	0,			    // lookupTable[33]
	0,			    // lookupTable[34]
	0,			    // lookupTable[35]

    // LUT Index 9 PageProgram
	0x08180402,		// lookupTable[36]		0x110
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x20, RADDR_SDR, FLEXSPI_1PAD, 0x18)
	0x00002004,		// lookupTable[37]
    // FLEXSPI_LUT_SEQ(WRITE_SDR, FLEXSPI_1PAD, 0x04, STOP, FLEXSPI_1PAD, 0x0)
	0,			    // lookupTable[38]
	0,			    // lookupTable[39]

    // LUT Index 10 Reserved
	0,			    // lookupTable[40]		0x120
	0,			    // lookupTable[41]
	0,			    // lookupTable[42]
	0,			    // lookupTable[43]

    // LUT Index 11 ChipErase
	0x00000460,		// lookupTable[44]		0x130
    // FLEXSPI_LUT_SEQ(CMD_SDR, FLEXSPI_1PAD, 0x60, STOP, FLEXSPI_1PAD, 0x0)
	0,			    // lookupTable[45]
	0,			    // lookupTable[46]
	0,			    // lookupTable[47]

    // LUT Index 12 Reserved
	0,			    // lookupTable[48]		0x140
	0,			    // lookupTable[49]
	0,			    // lookupTable[50]
	0,			    // lookupTable[51]

    // LUT Index 13 Reserved (NOR_CMD_LUT_SE Q_IDX_READ_SFDP)
	0,			    // lookupTable[52]		0x150
	0,			    // lookupTable[53]
	0,			    // lookupTable[54]
	0,			    // lookupTable[55]

    // LUT Index 14 Reserved (NOR_CMD_LUT_SE Q_IDX_RESTORE_N OCMD)
	0,			    // lookupTable[56]		0x160
	0,			    // lookupTable[57]
	0,			    // lookupTable[58]
	0,			    // lookupTable[59]

    // LUT Index 15 Dummy
	0,			    // lookupTable[60]		0x170
	0,			    // lookupTable[61]
	0,			    // lookupTable[62]
	0,			    // lookupTable[63]

	0,			    // LUT 0: Read			0x180
	0,			    // LUT 1: ReadStatus
	0,			    // LUT 3: WriteEnable
	0,			    // LUT 5: EraseSector

	0,			    // LUT 9: PageProgram		0x190
	0,			    // LUT 11: ChipErase
	0,			    // LUT 15: Dummy
	0,			    // LUT unused?

	0,			    // LUT unused?			0x1A0
	0,			    // LUT unused?
	0,			    // LUT unused?
	0,			    // LUT unused?

	0,			    // reserved			0x1B0
	0,			    // reserved
	0,			    // reserved
	0,			    // reserved

	// 64 byte Serial NOR configuration block, 8.6.3.2, page 346 (IMXRT1060)
	// 64 byte Serial NOR configuration block, 9.6.3.2, page 211 (IMXRT1020)

	256,			// pageSize			0x1C0
	4096,			// sectorSize
	1,			    // ipCmdSerialClkFreq
	0,			    // reserved

	0x00010000,		// block size		0x1D0
	0,			    // reserved
	0,			    // reserved
	0,			    // reserved

	0,			    // reserved			0x1E0
	0,			    // reserved
	0,			    // reserved
	0,			    // reserved

	0,			    // reserved			0x1F0
	0,			    // reserved
	0,			    // reserved
	0			    // reserved
};
