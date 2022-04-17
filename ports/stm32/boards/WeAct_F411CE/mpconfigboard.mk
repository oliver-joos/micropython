MCU_SERIES = f4
CMSIS_MCU = STM32F411xE
AF_FILE = boards/stm32f411_af.csv

FROZEN_MANIFEST ?= $(BOARD_DIR)/manifest.py

# Major version of your WeAct board?
# Default: 3 for V3.0
# Set 1 for V1.x or 2 for V2.x!
WEACT_VERSION ?= 3
CFLAGS += -DWEACT_VERSION=$(WEACT_VERSION)

# Bootloader in first 16kiB?
# Default: 0 for "no bootloader"
# Set 1 to use MicroPython with a bootloader like MBoot or TinyUF2!
USE_MBOOT ?= 0

# SPI flash filesystem size in MiB?
# Default: unset to use 64kiB of internal flash
# Set e.g. 2 for 2MiB, 16 for 16MiB or 0 for no filesystem at all!
ifdef SPIFLASH_MBYTE
# Put filesystem into external SPI flash (like Winbond W25Q16 ... W25Q128)
CFLAGS += -DSPIFLASH_MBYTE=$(SPIFLASH_MBYTE)
ifeq ($(USE_MBOOT),1)
LD_FILES = $(BOARD_DIR)/stm32f411_noifs.ld boards/common_bl.ld
TEXT0_ADDR = 0x08004000
else
LD_FILES = $(BOARD_DIR)/stm32f411_noifs.ld boards/common_basic.ld
TEXT0_ADDR = 0x08000000
endif

else
# Put a small filesystem into 64kiB of internal flash
ifeq ($(USE_MBOOT),1)
LD_FILES = boards/stm32f411.ld boards/common_blifs.ld
TEXT0_ADDR = 0x08020000
else
LD_FILES = boards/stm32f411.ld boards/common_ifs.ld
TEXT0_ADDR = 0x08000000
TEXT1_ADDR = 0x08020000
endif

endif

# Generating UF2
UF2CONV ?= $(TOP)/tools/uf2conv.py

define GENERATE_UF2
	$(ECHO) "GEN $(1)"
	$(Q)$(PYTHON) $(UF2CONV) -c -f STM32F4 -o $(1) $(2)
endef

$(BUILD)/firmware.uf2: $(BUILD)/firmware.hex
	$(call GENERATE_UF2,$@,$^)

uf2: $(BUILD)/firmware.uf2
