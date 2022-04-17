
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define MICROPY_HW_BOARD_NAME       "WeAct_F411CE-V" STR(WEACT_VERSION)
#define MICROPY_HW_MCU_NAME         "STM32F411xE"
#define MICROPY_HW_FLASH_FS_LABEL   "WeAct_F411"    // <= 11 chars, no " ./\ ... (see oofatfs/ff.c)

// MicroPython features
#define MICROPY_PY_STM              (1)         // Set to 0 to save 6kiB flash
#define MICROPY_PY_PYB_LEGACY       (0)
#define MICROPY_PY_UASYNCIO         (1)
#define MICROPY_PY_FRAMEBUF         (1)         // Set to 0 to save 4kiB flash
#define MICROPY_PY_ONEWIRE          (1)
#define MICROPY_PY_MACHINE_BITSTREAM (1)

// Hardware features
#define MICROPY_HW_HAS_SWITCH       (1)
#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_ADC       (1)         // IN1 at PA1, IN8 at PB0, IN9 at PB1
#define MICROPY_HW_ENABLE_USB       (1)
#define MICROPY_HW_USB_FS           (MICROPY_HW_ENABLE_USB)
#define MICROPY_HW_ENABLE_SERVO     (1)
#define MICROPY_HW_ENABLE_SDCARD    (0)
#define MICROPY_HW_ENABLE_RNG       (0)         // STM32F4x1 have no hardware random generator


#ifndef SPIFLASH_MBYTE
// Use 64 kiB of internal flash for filesystem --------
#define MICROPY_HW_HAS_FLASH                        (1)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (1)

#elif SPIFLASH_MBYTE <= 0
// Use without filesystem -----------------------------
#define MICROPY_HW_HAS_FLASH                        (0)
#define MICROPY_HW_USB_MSC                          (0)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (0)

#else  // SPIFLASH_MBYTE > 0
// Use external SPI flash for filesystem --------------
#define MICROPY_HW_HAS_FLASH                        (1)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (0)

#define MICROPY_HW_SPIFLASH_SIZE_BITS (SPIFLASH_MBYTE * 8 * 1024 * 1024)

#define MICROPY_HW_SPIFLASH_CS      (pin_A4)
#define MICROPY_HW_SPIFLASH_SCK     (pin_A5)
#if WEACT_VERSION == 2
#define MICROPY_HW_SPIFLASH_MISO    (pin_B4)    // WeAct V2.0 uses B4 for SPI flash
#else
#define MICROPY_HW_SPIFLASH_MISO    (pin_A6)    // WeAct V1.3 and V3.0 use A6 for SPI flash
#endif
#define MICROPY_HW_SPIFLASH_MOSI    (pin_A7)

extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t spi_bdev;
#define MICROPY_HW_SPIFLASH_ENABLE_CACHE (1)
#define MICROPY_HW_BDEV_IOCTL(op, arg) ( \
    (op) == BDEV_IOCTL_NUM_BLOCKS ? (MICROPY_HW_SPIFLASH_SIZE_BITS / 8 / FLASH_BLOCK_SIZE) : \
    (op) == BDEV_IOCTL_INIT ? spi_bdev_ioctl(&spi_bdev, (op), (uint32_t)&spiflash_config) : \
    spi_bdev_ioctl(&spi_bdev, (op), (arg)) \
)
#define MICROPY_HW_BDEV_READBLOCKS(dest, bl, n) spi_bdev_readblocks(&spi_bdev, (dest), (bl), (n))
#define MICROPY_HW_BDEV_WRITEBLOCKS(src, bl, n) spi_bdev_writeblocks(&spi_bdev, (src), (bl), (n))
#define MICROPY_HW_BDEV_SPIFLASH_EXTENDED (&spi_bdev)   // extended block protocol for LittleFS

#endif  // SPIFLASH_MBYTE


// HSE is 25MHz, CPU freq set to 96MHz
#define MICROPY_HW_CLK_PLLM         (25)
#define MICROPY_HW_CLK_PLLN         (192)
#define MICROPY_HW_CLK_PLLP         (RCC_PLLP_DIV2)
#define MICROPY_HW_CLK_PLLQ         (4)

// This board has a 32kHz crystal for the RTC
#define MICROPY_HW_RTC_USE_LSE      (1)
#define MICROPY_HW_RTC_USE_US       (0)
#define MICROPY_HW_RTC_USE_CALOUT   (1)         // turn on/off PC13 512Hz output (= LED1!)

#define MICROPY_HW_FLASH_LATENCY    FLASH_LATENCY_3


// UART config
#define MICROPY_HW_UART1_TX         (pin_A9)
#define MICROPY_HW_UART1_RX         (pin_A10)

#define MICROPY_HW_UART2_TX         (pin_A2)
#define MICROPY_HW_UART2_RX         (pin_A3)
// #define MICROPY_HW_UART2_CTS        (pin_A0)    // conflicts with USRSW if pressed
// #define MICROPY_HW_UART2_RTS        (pin_A1)

// NOTE: REPL is already accessible via USB VCP.
// Enable following lines to duplicate REPL to UART1, but
// then print will be 10x slower than with USB only!
#if 0
#define MICROPY_HW_UART_IS_RESERVED(id) (id == 1)
#define MICROPY_HW_UART_REPL        PYB_UART_1
#define MICROPY_HW_UART_REPL_BAUD   115200
#endif

// I2C busses
#define MICROPY_HW_I2C1_SCL         (pin_B6)
#define MICROPY_HW_I2C1_SDA         (pin_B7)
#define MICROPY_HW_I2C2_SCL         (pin_B10)
#define MICROPY_HW_I2C2_SDA         (pin_B9)
#define MICROPY_HW_I2C3_SCL         (pin_A8)
#define MICROPY_HW_I2C3_SDA         (pin_B8)

// I2S bus
#define MICROPY_HW_I2S2             (1)         // Remove this line to save 5kiB flash
//  I2S2_CK     = PB13
//  I2S2_WS     = PB12
//  I2S2_SD     = PB15
//  (I2S2ext_SD  = PB14)
//  (I2S2_MCK conflicts with UART2_RX or SPI1_MISO)

// SPI1 is reserved if SPI flash is mounted
#ifdef MICROPY_HW_SPIFLASH_CS
#define MICROPY_HW_SPI_IS_RESERVED(id) (id == 1)
#endif
#if WEACT_VERSION >= 2
// WeAct V2.0 and later: external SPI flash can use hardware SPI
#define MICROPY_HW_SPI1_NSS         (pin_A4)
#define MICROPY_HW_SPI1_SCK         (pin_A5)
#if WEACT_VERSION == 2
#define MICROPY_HW_SPI1_MISO        (pin_B4)    // WeAct V2.0 uses B4 for SPI flash
#else
#define MICROPY_HW_SPI1_MISO        (pin_A6)    // WeAct V1.3 and V3.0 use A6 for SPI flash
#endif
#define MICROPY_HW_SPI1_MOSI        (pin_A7)
#endif

// SPI2 may be used if SDCARD and I2S2 are disabled
#if !MICROPY_HW_ENABLE_SDCARD && !MICROPY_HW_I2S2
#define MICROPY_HW_SPI2_NSS         (pin_B12)
#define MICROPY_HW_SPI2_SCK         (pin_B13)
#define MICROPY_HW_SPI2_MISO        (pin_B14)
#define MICROPY_HW_SPI2_MOSI        (pin_B15)
#endif

// SPI3 MISO conflicts with JTAG and with SPI1 on WeAct V2.0
#if WEACT_VERSION != 2
#define MICROPY_HW_SPI3_NSS         (pin_A15)
#define MICROPY_HW_SPI3_SCK         (pin_B3)
#define MICROPY_HW_SPI3_MISO        (pin_B4)
#define MICROPY_HW_SPI3_MOSI        (pin_B5)
#endif

// SPI4 conflicts with USB Device Mode
// #define MICROPY_HW_SPI4_NSS         (pin_B12)
// #define MICROPY_HW_SPI4_SCK         (pin_B13)
// #define MICROPY_HW_SPI4_MISO        (pin_A1)
// #define MICROPY_HW_SPI4_MOSI        (pin_A11)

// SPI5 conflicts with USB Device Mode
// #define MICROPY_HW_SPI5_NSS         (pin_B1)
// #define MICROPY_HW_SPI5_SCK         (pin_A10)
// #define MICROPY_HW_SPI5_MISO        (pin_A12)
// #define MICROPY_HW_SPI5_MOSI        (pin_B0)

// USRSW is pulled up. Pressing the button makes the input go low.
#define MICROPY_HW_USRSW_PIN        (pin_A0)
#define MICROPY_HW_USRSW_PULL       (GPIO_PULLUP)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_FALLING)
#define MICROPY_HW_USRSW_PRESSED    (0)

// LEDs
#define MICROPY_HW_LED1             (pin_C13)   // Blue C13 LED on board
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_low(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_high(pin))


// MicroPython Bootloader configuration
#define MBOOT_USB_AUTODETECT_PORT   (1)
#define MBOOT_FSLOAD                (0)         // would need QSPI flash
