
#define MICROPY_HW_BOARD_NAME       "WeAct_F411CE"
#define MICROPY_HW_MCU_NAME         "STM32F411xE"
#define MICROPY_HW_FLASH_FS_LABEL   "WeAct_F411"    // <= 11 chars, no " ./\ ... (see oofatfs/ff.c)

#define MICROPY_HW_WEACT_MAJOR_VERSION  2   // 1 for V1.3, 2 for V2.0, ...

#define MICROPY_HW_HAS_SWITCH       (1)
#define MICROPY_HW_HAS_FLASH        (1)
#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_USB       (1)
#define MICROPY_HW_ENABLE_SERVO     (1)
#define MICROPY_HW_ENABLE_SDCARD    (0)
#define MICROPY_HW_ENABLE_RNG       (0)     // no hardware random gen. available on F401 and F411


// 1 = Use internal flash (64 of 512 KByte)
// 0 = Use onboard SPI flash (set correct size below!)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE (0)

#if !MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE

// The board ships without SPI flash, but you may add your own!
// (It supports flash like Winbond W25Q16, W25Q32, W25Q64, W25Q128)
// #define MICROPY_HW_SPIFLASH_SIZE_BITS (16 * 1024 * 1024) // W25Q16 - 16 Mbit (2 MByte)
// #define MICROPY_HW_SPIFLASH_SIZE_BITS (32 * 1024 * 1024) // W25Q32 - 32 Mbit (4 MByte)
// #define MICROPY_HW_SPIFLASH_SIZE_BITS (64 * 1024 * 1024) // W25Q64 - 64 Mbit (8 MByte)
#define MICROPY_HW_SPIFLASH_SIZE_BITS (128 * 1024 * 1024) // W25Q128 - 128 Mbit (16 MByte)

#define MICROPY_HW_SPIFLASH_CS      (pin_A4)
#define MICROPY_HW_SPIFLASH_SCK     (pin_A5)
#define MICROPY_HW_SPIFLASH_MOSI    (pin_A7)
#if MICROPY_HW_WEACT_MAJOR_VERSION >= 2
#define MICROPY_HW_SPIFLASH_MISO    (pin_B4)    // WeAct V2.0 uses B4 for SPI flash
#else
#define MICROPY_HW_SPIFLASH_MISO    (pin_A6)    // WeAct V1.3 uses A6 for SPI flash
#endif

#define MICROPY_BOARD_EARLY_INIT    WeAct_F411CE_board_early_init
void WeAct_F411CE_board_early_init(void);

extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t spi_bdev;
#define MICROPY_HW_BDEV_IOCTL(op, arg) ( \
    (op) == BDEV_IOCTL_NUM_BLOCKS ? (MICROPY_HW_SPIFLASH_SIZE_BITS / 8 / FLASH_BLOCK_SIZE) : \
    (op) == BDEV_IOCTL_INIT ? spi_bdev_ioctl(&spi_bdev, (op), (uint32_t)&spiflash_config) : \
    spi_bdev_ioctl(&spi_bdev, (op), (arg)) \
)
#define MICROPY_HW_BDEV_READBLOCKS(dest, bl, n) spi_bdev_readblocks(&spi_bdev, (dest), (bl), (n))
#define MICROPY_HW_BDEV_WRITEBLOCKS(src, bl, n) spi_bdev_writeblocks(&spi_bdev, (src), (bl), (n))

#define MICROPY_HW_BDEV_SPIFLASH_EXTENDED (&spi_bdev)   // extended block protocol (e.g. for LittleFS)

#endif  // !MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE

// HSE is 25MHz, CPU freq set to 96MHz
#define MICROPY_HW_CLK_PLLM (25)
#define MICROPY_HW_CLK_PLLN (192)
#define MICROPY_HW_CLK_PLLP (RCC_PLLP_DIV2)
#define MICROPY_HW_CLK_PLLQ (4)

// The pyboard has a 32kHz crystal for the RTC
#define MICROPY_HW_RTC_USE_LSE      (1)
#define MICROPY_HW_RTC_USE_US       (0)
#define MICROPY_HW_RTC_USE_CALOUT   (1)

#define MICROPY_HW_FLASH_LATENCY    FLASH_LATENCY_3

// USB config
#define MICROPY_HW_USB_FS              (1)
// #define MICROPY_HW_USB_VBUS_DETECT_PIN (pin_A9)
// #define MICROPY_HW_USB_OTG_ID_PIN      (pin_A10)

// UART config
#define MICROPY_HW_UART1_TX     (pin_A9)
#define MICROPY_HW_UART1_RX     (pin_A10)

#define MICROPY_HW_UART2_TX     (pin_A2)
#define MICROPY_HW_UART2_RX     (pin_A3)
// #define MICROPY_HW_UART2_CTS    (pin_A0)     // CTS conflicts with USRSW
// #define MICROPY_HW_UART2_RTS    (pin_A1)

// UART 1 connects to the STM32F411 on the WeAct board
// and this is exposed as a USB Serial port.
#define MICROPY_HW_UART_REPL        PYB_UART_1
#define MICROPY_HW_UART_REPL_BAUD   115200

// I2C busses
#define MICROPY_HW_I2C1_SCL (pin_B6)
#define MICROPY_HW_I2C1_SDA (pin_B9)
#define MICROPY_HW_I2C2_SCL (pin_B10)
#define MICROPY_HW_I2C2_SDA (pin_B3)
#define MICROPY_HW_I2C3_SCL (pin_A8)
#define MICROPY_HW_I2C3_SDA (pin_B8)

// SPI1 may be used if no SPI flash is mounted
#define MICROPY_HW_SPI1_NSS     (pin_A4)
#define MICROPY_HW_SPI1_SCK     (pin_A5)
#define MICROPY_HW_SPI1_MOSI    (pin_A7)
#define MICROPY_HW_SPI1_MISO    (pin_B4)

// SPI2 may be used if SDCard interface is disabled
#if !MICROPY_HW_ENABLE_SDCARD
#define MICROPY_HW_SPI2_NSS     (pin_B12)
#define MICROPY_HW_SPI2_SCK     (pin_B13)
#define MICROPY_HW_SPI2_MISO    (pin_B14)
#define MICROPY_HW_SPI2_MOSI    (pin_B15)
#endif

// SPI3 MISO conflicts with SPI1 and SPI2
// #define MICROPY_HW_SPI3_NSS     (pin_A15)
// #define MICROPY_HW_SPI3_SCK     (pin_B12)
// #define MICROPY_HW_SPI3_MISO    (pin_B4)
// #define MICROPY_HW_SPI3_MOSI    (pin_B5)

// SPI4 conflicts with USB Device Mode
// #define MICROPY_HW_SPI4_NSS     (pin_B12)
// #define MICROPY_HW_SPI4_SCK     (pin_B13)
// #define MICROPY_HW_SPI4_MISO    (pin_A1)
// #define MICROPY_HW_SPI4_MOSI    (pin_A11)

// SPI5 conflicts with USB Device Mode
// #define MICROPY_HW_SPI5_NSS     (pin_B1)
// #define MICROPY_HW_SPI5_SCK     (pin_A10)
// #define MICROPY_HW_SPI5_MISO    (pin_A12)
// #define MICROPY_HW_SPI5_MOSI    (pin_B0)

// USRSW is pulled up. Pressing the button makes the input go low.
#define MICROPY_HW_USRSW_PIN        (pin_A0)
#define MICROPY_HW_USRSW_PULL       (GPIO_PULLUP)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_FALLING)
#define MICROPY_HW_USRSW_PRESSED    (0)

// LEDs
#define MICROPY_HW_LED1             (pin_C13)   // Blue C13 LED on board
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_low(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_high(pin))
