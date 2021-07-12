USE_MBOOT ?= 0

# MCU settings
MCU_SERIES = h7
CMSIS_MCU = STM32H753xx
MICROPY_FLOAT_IMPL = double
AF_FILE = boards/stm32h753_af.csv

ifeq ($(USE_MBOOT),1)
# When using Mboot everything goes after the bootloader
LD_FILES = boards/stm32h753zi.ld boards/common_bl.ld
TEXT0_ADDR = 0x08020000
else
# When not using Mboot everything goes at the start of flash
LD_FILES = boards/stm32h753zi.ld boards/common_basic.ld
TEXT0_ADDR = 0x08000000
endif

# MicroPython settings
MICROPY_PY_LWIP = 1
MICROPY_PY_USSL = 1
MICROPY_SSL_MBEDTLS = 1
MICROPY_VFS_LFS2 = 1

# Flash tool configuration
OPENOCD_CONFIG = boards/openocd_stm32h7_dual_bank.cfg
