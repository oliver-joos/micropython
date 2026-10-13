# MCU settings
MCU_SERIES = h7
CMSIS_MCU = STM32H753xx
MICROPY_FLOAT_IMPL = single
AF_FILE = boards/stm32h743_af.csv

# Bootloader in first flash page?
USE_MBOOT ?= 0

# SPI flash filesystem size in MiB?
# Default: unset to use a part of internal flash
SPIFLASH_MBYTE ?= 32

ifdef SPIFLASH_MBYTE
# Put filesystem into external SPI flash (like Winbond W25Q16 ... W25Q128)
CFLAGS += -DSPIFLASH_MBYTE=$(SPIFLASH_MBYTE)
LD_FILES = $(BOARD_DIR)/stm32h753.ld boards/common_basic.ld
TEXT0_ADDR = 0x08000000
else
# Put a small filesystem into a part of internal flash
LD_FILES = $(BOARD_DIR)/stm32h753_ifs.ld boards/common_ifs.ld
TEXT0_ADDR = 0x08000000
TEXT1_ADDR = 0x08020000
endif

# Network configuration
MICROPY_PY_LWIP = 1
MICROPY_PY_LWIP_LOOPBACK = 1
MICROPY_PY_NETWORK_CYW43 = $(MICROPY_PY_LWIP)
MICROPY_PY_NETWORK_CYW43_FW = 1
MICROPY_PY_NETWORK_CYW43_FW_LOADER = 1

# CYW43 configuration
CFLAGS += -DCYW43_CONFIG_FILE='<cyw43_configport_lisa.h>'

# CYW43 debug mode: unset=muted, 0=normal, 1=verbose
ifdef CYW43_VERBOSE_DEBUG
CFLAGS += -DCYW43_VERBOSE_DEBUG=$(CYW43_VERBOSE_DEBUG)
endif

# VFS support
MICROPY_VFS_LFS2 = 1
MICROPY_VFS_LFS1 = 1
MICROPY_VFS_FAT = 1

MICROPY_HW_ENABLE_ISR_UART_FLASH_FUNCS_IN_RAM = 1

# Flash tool configuration
STFLASH = st-flash --connect-under-reset
OPENOCD_CONFIG = boards/openocd_stm32h7_dual_bank.cfg

# Board-specific modules and header files
CFLAGS += -I$(BOARD_DIR)/..
CFLAGS += -I$(BOARD_DIR)/lwip_inc
CFLAGS += -DMICROPY_PY_ASYNCIO=1
