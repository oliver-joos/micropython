#define MICROPY_HW_BOARD_NAME       "LISA_H753ZI"
#define MICROPY_HW_MCU_NAME         "STM32H753"
#define MICROPY_HW_FLASH_FS_LABEL   MICROPY_HW_BOARD_NAME   // max. 11 chars!

#if 0
#define CYW43_CHIPSET_FIRMWARE_INCLUDE_FILE "lib/cyw43-driver/firmware/w43439A0_7_95_49_00_combined.h"
#define CYW43_WIFI_NVRAM_INCLUDE_FILE   "lib/cyw43-driver/firmware/wifi_nvram_43439.h"
#endif

// MicroPython features
#define MICROPY_PY_STM              (1)                     // needs 8312 bytes flash
#define MICROPY_PY_PYB_LEGACY       (0)
#define MICROPY_PY_SOCKET           (1)                     // needs 2200 bytes flash and MICROPY_PY_NETWORK
#define MICROPY_PY_NETWORK          (1)                     // needs 116 bytes flash

// cyw43-driver config
#define CYW43_DEFAULT_IP_AP_ADDRESS LWIP_MAKEU32(192, 168, 0, 1)
#define CYW43_DEFAULT_IP_AP_GATEWAY LWIP_MAKEU32(192, 168, 0, 1)

#ifdef CYW43_VERBOSE_DEBUG
#if CYW43_VERBOSE_DEBUG
#define CYW43_VDEBUG(...)               mp_printf(MP_PYTHON_PRINTER, "TRACE  " __VA_ARGS__)
#endif
#define CYW43_DEBUG(...)                mp_printf(MP_PYTHON_PRINTER, "DEBUG  " __VA_ARGS__)
#define CYW43_INFO(...)                 mp_printf(MP_PYTHON_PRINTER, "INFO   " __VA_ARGS__)
#define CYW43_WARN(...)                 mp_printf(MP_PYTHON_PRINTER, "WARN   " __VA_ARGS__)
#endif

// LwIP config
#define MICROPY_PY_LWIP_POLL_INTERVAL (1 << 6)              // interval in milliseconds to poll lwIP

// Hardware features
#define MICROPY_HW_ENABLE_RTC       (1)                     // needs 1280 bytes flash, e.g. for time.time()
#define MICROPY_HW_ENABLE_RNG       (1)                     // needs 204 bytes flash, for machine.rng() and os.urandom()
                                                            // (not faster than pyb_rng_yasmarang(), but true random!)
#define MICROPY_HW_ENABLE_ADC       (1)                     // needs 3192 bytes flash, for pyb.ADC and pyb.ADCAll
#define MICROPY_HW_ENABLE_DAC       (0)
#define MICROPY_HW_ENABLE_USB       (1)                     // needs 9832 bytes flash
#define MICROPY_HW_USB_FS           (MICROPY_HW_ENABLE_USB) // needs 4856 bytes flash
#define MICROPY_HW_USB_HID          (0)
#define MICROPY_HW_HAS_SWITCH       (1)
#define MICROPY_HW_ENABLE_RF_SWITCH (1)

#define MICROPY_BOARD_EARLY_INIT    board_early_init
#define MICROPY_BOARD_ENTER_STOP    board_sleep(1);
#define MICROPY_BOARD_LEAVE_STOP    board_sleep(0);
#define MICROPY_BOARD_ENTER_STANDBY board_sleep(1);
#define MICROPY_BOARD_SDCARD_POWER  mp_hal_pin_high(pyb_pin_EN_3V3);
void board_early_init(void);
void board_sleep(int value);

// HSE+LSE is used, HSE is 16MHz, CPU freq set to 240MHz
// !!! See powerctrl.c: above 400MHz needs special handling for overdrive, currently unsupported
#define MICROPY_HW_CLK_USE_HSI      (0)
#define MICROPY_HW_CLK_PLLM         (8)
#define MICROPY_HW_CLK_PLLN         (240)
#define MICROPY_HW_CLK_PLLP         (2)
#define MICROPY_HW_CLK_PLLQ         (3)
#define MICROPY_HW_CLK_PLLR         (2)
#define MICROPY_HW_CLK_PLLVCI       (RCC_PLL1VCIRANGE_1)
#define MICROPY_HW_CLK_PLLVCO       (RCC_PLL1VCOWIDE)
#define MICROPY_HW_CLK_PLLFRAC      (0)

// The USB clock is set using PLL3
#define MICROPY_HW_CLK_PLL3M        (8)
#define MICROPY_HW_CLK_PLL3N        (120)
#define MICROPY_HW_CLK_PLL3P        (3)
#define MICROPY_HW_CLK_PLL3Q        (5)
#define MICROPY_HW_CLK_PLL3R        (3)                     // 3 => 80MHz for ADC
#define MICROPY_HW_CLK_PLL3VCI      (RCC_PLL3VCIRANGE_1)
#define MICROPY_HW_CLK_PLL3VCO      (RCC_PLL3VCOWIDE)
#define MICROPY_HW_CLK_PLL3FRAC     (0)

#define MICROPY_HW_FLASH_LATENCY    FLASH_LATENCY_3

// There is an external 32768 Hz oscillator
// #define MICROPY_HW_RCC_RTC_CLKSOURCE (RCC_RTCCLKSOURCE_LSI) // must always be LSI (used before LSE is stable)
#define MICROPY_HW_RTC_USE_LSE      (1)
// #define MICROPY_HW_RTC_USE_BYPASS   (1)                     // default: (0)
// #define MICROPY_HW_RTC_USE_US       (1)                     // default: undefined == (0)
// #define MICROPY_HW_RTC_USE_CALOUT   (1)                     // default: undefined == (0)


// uGatewayV2 board Rev 03 or later works best with default LSEDRIVE and PREDIVs.
// The RTC on Revision 02 is too slow and uses too much power to run 20 mins from Vbat.
// Default power is 750nA for core function + RCC_LSEDRIVE_LOW (2320nA) for crystal.
// With 2 x 6.8pF (Rev 02) the crystal oszillates 150-350ppm too slow (13-30 sec/day) for ~16min.
// With 2 x 22pF (modified) the crystal oszillates 250ppm too fast (~22 sec/day) for ~19min.
// With RCC_LSEDRIVE_HIGH the RTC is  more precise but uses more power (+680nA).
// => for optimal precision and power the two _parallel_ caps should be 7pF _in_total_ = 2 x 14pF
//
// A wrong frequency can be compensated here in software (by changing "#if 0" -> "#if 1"):
// Custom frequency in Hz = (RTC_ASYNCH_PREDIV + 1) * (RTC_SYNCH_PREDIV + 1) with defaults 0, 0x7fff.
#if 1
#define MICROPY_HW_RCC_LSEDRIVE     (RCC_LSEDRIVE_MEDIUMHIGH)          // LOW = default
// A board Rev 02 with 2 x 6.8pF => 190ppm too slow => 32762 Hz => fixed with (0, 0x7ff9)
// #define RTC_ASYNCH_PREDIV           (0)
// #define RTC_SYNCH_PREDIV            (0x7ff9)
// Another Rev 02 with 2 x 6.8pF => 324ppm too slow => 32758 Hz => fixed with (0, 0x7ff5)
// #define RTC_ASYNCH_PREDIV           (0)
// #define RTC_SYNCH_PREDIV            (0x7ff5)
// A modded Rev 02 with 2 x 22pF => 250ppm too fast => 32776 Hz => fixed with (1, 0x4003)
// #define RTC_ASYNCH_PREDIV           (1)
// #define RTC_SYNCH_PREDIV            (0x4003)
#else
// RCC_LSEDRIVE_HIGH (HIGH needs more power, but is more precise)
#define MICROPY_HW_RCC_LSEDRIVE     (RCC_LSEDRIVE_HIGH)
#define RTC_ASYNCH_PREDIV           (0)
#define RTC_SYNCH_PREDIV            (0x7ffe)
#endif


// Whether to enable storage on the internal flash of the MCU
#define MICROPY_HW_ENABLE_SDCARD    (MICROPY_HW_ENABLE_USB) // needed for eMMC and USB
#define MICROPY_HW_BDEV_HAS_MBR     (1)                     // leave room for MBR with partition table
#define MICROPY_HW_FLASH_MOUNT_AT_BOOT (1)                  // mount "/flash" in boot.py
#define MICROPY_HW_SDCARD_MOUNT_AT_BOOT (0)                 // mount "/sd" in boot.py


#ifndef SPIFLASH_MBYTE
// Use a part of internal flash for filesystem --------
#define MICROPY_HW_HAS_FLASH                        (1)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (1)
#define MICROPY_HW_ENABLE_MMCARD                    (0)

#elif SPIFLASH_MBYTE <= 0
// Use only external EMMC flash for filesystems ------------
#define MICROPY_HW_HAS_FLASH                        (0)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (0)
#define MICROPY_HW_ENABLE_MMCARD                    (1)
#define MICROPY_HW_USB_MSC                          (0)

#else  // SPIFLASH_MBYTE > 0
// Use external EMMC flash for main filesystem and external QSPI flash for firmware updates --------
#define MICROPY_HW_HAS_FLASH                        (1)
#define MICROPY_HW_ENABLE_INTERNAL_FLASH_STORAGE    (0)
#define MICROPY_HW_ENABLE_MMCARD                    (1)
#define MICROPY_HW_SPIFLASH_SIZE_BITS (SPIFLASH_MBYTE * 8 * 1024 * 1024)
#define MICROPY_HW_SPIFLASH_DETECT_DEVICE           (1)

// Use external SPI flash for R/W storage
#define MICROPY_HW_QSPI_PRESCALER                   (2)     // PLL1_Q / PRESCALER = QSPI (in MHz)
#define MICROPY_HW_QSPIFLASH_SIZE_BITS_LOG2         (28)
#define MICROPY_HW_QSPIFLASH_CS     (pyb_pin_QSPI2_CS)
#define MICROPY_HW_QSPIFLASH_SCK    (pyb_pin_QSPI2_CLK)
#define MICROPY_HW_QSPIFLASH_IO0    (pyb_pin_QSPI2_D0)
#define MICROPY_HW_QSPIFLASH_IO1    (pyb_pin_QSPI2_D1)
#define MICROPY_HW_QSPIFLASH_IO2    (pyb_pin_QSPI2_D2)
#define MICROPY_HW_QSPIFLASH_IO3    (pyb_pin_QSPI2_D3)

// SPI flash block device config
extern const struct _mp_spiflash_config_t spiflash_config;
extern struct _spi_bdev_t spi_bdev;
#define MICROPY_HW_SPIFLASH_ENABLE_CACHE (1)
#define MICROPY_HW_BDEV_SPIFLASH    (&spi_bdev)
#define MICROPY_HW_BDEV_SPIFLASH_CONFIG (&spiflash_config)
#define MICROPY_HW_BDEV_SPIFLASH_SIZE_BYTES (MICROPY_HW_SPIFLASH_SIZE_BITS / 8)
#define MICROPY_HW_BDEV_SPIFLASH_EXTENDED (&spi_bdev) // for extended block protocol

#endif  // SPIFLASH_MBYTE

// eMMC / SD card
#define MICROPY_HW_SDCARD_SDMMC             (2)
#define MICROPY_HW_SDCARD_BUS_WIDTH         (4)
#define MICROPY_HW_SDCARD_CK                (pyb_pin_SD_CK)
#define MICROPY_HW_SDCARD_CMD               (pyb_pin_SD_CMD)
#define MICROPY_HW_SDCARD_D0                (pyb_pin_SD_D0)
#define MICROPY_HW_SDCARD_D1                (pyb_pin_SD_D1)
#define MICROPY_HW_SDCARD_D2                (pyb_pin_SD_D2)
#define MICROPY_HW_SDCARD_D3                (pyb_pin_SD_D3)

// USB config
// #define MICROPY_HW_USB_VBUS_DETECT_PIN (pin_A9)
// #define MICROPY_HW_USB_OTG_ID_PIN      (pin_A10)

// UART config
#define MICROPY_HW_UART_IS_RESERVED(id) (id == 3)
#define MICROPY_HW_UART2_NAME       "WLI"
#define MICROPY_HW_UART2_TX         (pin_D5)        // WLI_TX --> Nina-W156 UART_RXD
#define MICROPY_HW_UART2_RX         (pin_D6)        // WLI_RX <-- Nina-W156 UART_TXD
#define MICROPY_HW_UART2_RTS        (pin_D4)        // WLI_RTS --> Nina-W156 UART_CTS
#define MICROPY_HW_UART2_CTS        (pin_D3)        // WLI_CTS <-- Nina-W156 UART_RTS
#define MICROPY_HW_UART3_NAME       "REPL"
#define MICROPY_HW_UART3_TX         (pin_D8)        // ST_DEB_TX --> PC RXD (FTDI Pin 5, yel)
#define MICROPY_HW_UART3_RX         (pin_D9)        // ST_DEB_RX <-- PC TXD (FTDI Pin 4, org)
#define MICROPY_HW_UART6_NAME       "BLIF"
#define MICROPY_HW_UART6_TX         (pin_C6)        // BLIF_TX --> A-Block RXD
#define MICROPY_HW_UART6_RX         (pin_C7)        // BLIF_RX <-- A-Block TXD

#define MICROPY_HW_UART_REPL        PYB_UART_3
#define MICROPY_HW_UART_REPL_BAUD   115200

// I2C buses
#define MICROPY_HW_I2C1_NAME        "SHT21"
#define MICROPY_HW_I2C1_SCL         (pin_B6)        // SHT21_SCL
#define MICROPY_HW_I2C1_SDA         (pin_B7)        // SHT21_SDA
#define MICROPY_HW_ENABLE_HW_I2C_TARGET (0)

// SPI buses
#define MICROPY_HW_SPI_IS_RESERVED(id) (id == 3)
#define MICROPY_HW_SPI1_NAME        "NEOPIXEL"
#define MICROPY_HW_SPI1_SCK         (pin_A5)
#define MICROPY_HW_SPI1_MOSI        (pin_D7)
#define MICROPY_HW_SPI3_NAME        "FLASH"
#define MICROPY_HW_SPI3_NSS         (pin_A4)
#define MICROPY_HW_SPI3_SCK         (pin_B3)
#define MICROPY_HW_SPI3_MISO        (pin_B4)
#define MICROPY_HW_SPI3_MOSI        (pin_B5)
#define MICROPY_HW_SPI4_NAME        "TLC5947"
// #define MICROPY_HW_SPI4_NSS         (pin_E11)    // TLC5947_BLANK (used by software to blank LEDs)
#define MICROPY_HW_SPI4_SCK         (pin_E12)       // TLC5947_SCLK
// #define MICROPY_HW_SPI4_MISO        (pin_E13)    // TLC5947_XLAT (used by software to update LEDs)
#define MICROPY_HW_SPI4_MOSI        (pin_E14)       // TLC5947_SIN

// USRSW is pulled high externally. Pressing the button makes the input go low.
#define MICROPY_HW_USRSW_PIN        (pyb_pin_S_PRG)
#define MICROPY_HW_USRSW_PULL       (GPIO_NOPULL)
#define MICROPY_HW_USRSW_EXTI_MODE  (GPIO_MODE_IT_FALLING)
#define MICROPY_HW_USRSW_PRESSED    (0)

// LEDs
#define MICROPY_HW_LED1             (pin_A8)        // STATUS_RED
#define MICROPY_HW_LED2             (pin_A9)        // STATUS_GREEN
#define MICROPY_HW_LED3             (pin_A10)       // STATUS_BLUE
#define MICROPY_HW_LED1_PWM         { TIM1, 1, TIM_CHANNEL_1, GPIO_AF1_TIM1 }
#define MICROPY_HW_LED2_PWM         { TIM1, 1, TIM_CHANNEL_2, GPIO_AF1_TIM1 }
#define MICROPY_HW_LED3_PWM         { TIM1, 1, TIM_CHANNEL_3, GPIO_AF1_TIM1 }
#define MICROPY_HW_LED_ON(pin)      (mp_hal_pin_low(pin))
#define MICROPY_HW_LED_OFF(pin)     (mp_hal_pin_high(pin))
#define MICROPY_HW_LED_INVERTED (1)
