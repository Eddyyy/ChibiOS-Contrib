
# List of the ChibiOS IMXRT1021 startup and CMSIS files.
STARTUPSRC = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt1.c \
			 $(CHIBIOS_CONTRIB)/os/common/ext/NXP/MIMXRT102x/system_MIMXRT1021.c \
			 $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/devices/MIMXRT102x/bootImage.c
          
STARTUPASM = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/vectors.S \
			 $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/devices/MIMXRT102x/boot.S

STARTUPINC = $(CHIBIOS)/os/common/portability/GCC \
             $(CHIBIOS)/os/common/ext/ARM/CMSIS/Core/Include \
             $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC \
             $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/devices/MIMXRT102x \
             $(CHIBIOS_CONTRIB)/os/common/ext/NXP/MIMXRT102x

STARTUPLD  = $(CHIBIOS)/os/common/startup/ARMCMx/compilers/GCC/ld
STARTUPLD_CONTRIB  = $(CHIBIOS_CONTRIB)/os/common/startup/ARMCMx/compilers/GCC/ld

# Shared variables
ALLXASMSRC += $(STARTUPASM)
ALLCSRC    += $(STARTUPSRC)
ALLINC     += $(STARTUPINC)
