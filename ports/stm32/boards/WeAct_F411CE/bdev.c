#if !BUILDING_MBOOT

#include "storage.h"
#include "spi.h"


#ifdef MICROPY_HW_SPIFLASH_CS

#if MICROPY_HW_SPIFLASH_ENABLE_CACHE
STATIC mp_spiflash_cache_t spi_bdev_cache;
#endif

#if WEACT_VERSION >= 2
// WeAct V2.0 and later: external SPI flash can use hardware SPI interface

STATIC const spi_proto_cfg_t hard_spi_bus = {
    .spi = &spi_obj[0],
    .baudrate = 50000000,
    .polarity = SPI_POLARITY_LOW,
    .phase = SPI_PHASE_1EDGE,
    .bits = 8,
    .firstbit = SPI_FIRSTBIT_MSB
};

const mp_spiflash_config_t spiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_SPI,
    .bus.u_spi.cs = MICROPY_HW_SPIFLASH_CS,
    .bus.u_spi.data = (void*)&hard_spi_bus,
    .bus.u_spi.proto = &spi_proto,
    #if MICROPY_HW_SPIFLASH_ENABLE_CACHE
    .cache = &spi_bdev_cache,
    #endif
};

#else
// WeAct V1.x: external SPI flash must use software SPI interface

STATIC const mp_soft_spi_obj_t soft_spi_bus = {
    .delay_half = MICROPY_HW_SOFTSPI_MIN_DELAY,
    .polarity = 0,
    .phase = 0,
    .sck = MICROPY_HW_SPIFLASH_SCK,
    .mosi = MICROPY_HW_SPIFLASH_MOSI,
    .miso = MICROPY_HW_SPIFLASH_MISO,
};

const mp_spiflash_config_t spiflash_config = {
    .bus_kind = MP_SPIFLASH_BUS_SPI,
    .bus.u_spi.cs = MICROPY_HW_SPIFLASH_CS,
    .bus.u_spi.data = (void*)&soft_spi_bus,
    .bus.u_spi.proto = &mp_soft_spi_proto,
    #if MICROPY_HW_SPIFLASH_ENABLE_CACHE
    .cache = &spi_bdev_cache,
    #endif
};

#endif  // WEACT_VERSION

spi_bdev_t spi_bdev;

#endif  // MICROPY_HW_SPIFLASH_CS

#endif  // !BUILDING_MBOOT
