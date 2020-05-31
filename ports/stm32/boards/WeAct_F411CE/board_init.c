#include "py/mphal.h"

void WeAct_F411CE_board_early_init(void) {
#ifdef MICROPY_HW_SPIFLASH_CS
    // Set SPI flash CS pin to output and high
    mp_hal_pin_output(MICROPY_HW_SPIFLASH_CS);
    mp_hal_pin_write(MICROPY_HW_SPIFLASH_CS, 1);
#endif
}
