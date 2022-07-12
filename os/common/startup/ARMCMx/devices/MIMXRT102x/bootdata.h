
#include "MIMXRT1021.h"

/* Lookup table related defintions */
#define CMD_INDEX_READ 0
#define CMD_INDEX_READSTATUS 1
#define CMD_INDEX_WRITEENABLE 2
#define CMD_INDEX_WRITE 4

#define CMD_LUT_SEQ_IDX_READ 0
#define CMD_LUT_SEQ_IDX_READSTATUS 1
#define CMD_LUT_SEQ_IDX_WRITEENABLE 3
#define CMD_LUT_SEQ_IDX_WRITE 9

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

