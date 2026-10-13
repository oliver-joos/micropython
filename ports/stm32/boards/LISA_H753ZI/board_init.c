// -------------------------------------------------------------------------------------------------
// @file  ./mpy-boards/LISA_H753ZI/board_init.c
//
// @brief  Board-specific initialisation
//
// @author  OJS  oliver.joos@hispeed.ch
//
// @copyright  Copyright (C) 2021, Feller AG
// -------------------------------------------------------------------------------------------------
//

#include "py/mphal.h"
#include "storage.h"

void board_early_init(void) {
    // Configure EN_3V3 as an output pin, but keep it turned off
    mp_hal_pin_config(pyb_pin_EN_3V3, MP_HAL_PIN_MODE_OUTPUT, MP_HAL_PIN_PULL_NONE, 0);
}

void board_sleep(int value) {
    #if defined(MICROPY_HW_SPIFLASH_SCK) || defined(MICROPY_HW_QSPIFLASH_SCK)
    mp_spiflash_deepsleep(&spi_bdev.spiflash, value);
    #endif
}

#include "storage.h"

int mp_spiflash_detect(mp_spiflash_t *spiflash, int ret, uint32_t devid) {
    return 0;
}
