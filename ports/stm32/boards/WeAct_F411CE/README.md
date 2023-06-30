
# MicroPython on WeAct F411CE "Blackpill"

## Setup Build Environment
Install a Debian Linux, Ubuntu or Linux Mint and the necessary tools.

```shell
sudo apt update
sudo apt install -y build-essential pkg-config autoconf libtool
sudo apt install -y python3-usb python3-serial libffi-dev libltdl-dev libreadline-dev
sudo apt install -y git dfu-util binutils-arm-none-eabi gcc-arm-none-eabi

# Stop automatic modem commands on ports /dev/ttyACMx:
sudo systemctl stop ModemManager ; sudo systemctl disable ModemManager

# Give the user permission to access USB ports:
sudo adduser $USER dialout

# Now login again: (or reboot your Linux)
su - $USER
```

## Build and Deploy
First download (or later update) MicroPython and build the firmware for WeAct boards.

**IMPORTANT:** To enter STM32 DFU (Device Firmware Updater) hold down BOOT0 while pressing RST and *do not touch pins*!

```shell
# Clone the repository: (ONLY NEEDED ONCE!)
git clone --branch board/weact_f411ce --recurse-submodules https://github.com/oliver-joos/micropython.git
cd micropython/

# Pull new updates from the repository:
git pull

# Build the firmware binary:
make -C ports/stm32/ BOARD=WeAct_F411CE clean all

# Now connect your board, enter the STM32 DFU and deploy firmware:
make -C ports/stm32/ BOARD=WeAct_F411CE deploy

# Invoke the command line "REPL" of the board:
tools/mpremote/mpremote.py soft-reset repl
```

## Custom Build Parameters
WeAct boards come in different versions like "V2.1" or "V3.0" which is written on the bottom of the board.
To build MicroPython for boards other than V3.x you must specify this version.
If you can't find a version number, you probably have a V1.x.

WeAct boards are sold without or with SPI flash of different sizes.
To use the SPI flash as a file storage you have to specify its size in MByte.
Search the Internet for the imprint on the flash chip or guess it (e.g. W25Q128 means 128MBit = 8MByte) or just try "1".

**IMPORTANT:** Always use the same parameters to build *and* deploy!

**IMPORTANT:** Changing the SPI flash size may *delete files* previously stored!

Parameter       | Default | Purpose
--------------- | ------- | -------
`WEACT_VERSION` |   3     | Major digit of board version, like "2" for V2.1
`SPIFLASH_MBYTE`|   -     | Size of SPI flash chip, like "4" for Winbond W25Q32
`USE_MBOOT`     |   0     | Enable MicroPython to be deployed by a bootloader

**Example:**
```shell
make -C ports/stm32/ BOARD=WeAct_F411CE WEACT_VERSION=2 SPIFLASH_MBYTE=2 clean all
make -C ports/stm32/ BOARD=WeAct_F411CE WEACT_VERSION=2 SPIFLASH_MBYTE=2 deploy
```

## Faster build
1. Call ``make`` with ``-j`` and without ``clean``:

  **IMPORTANT:** This may fail if build parameters or MicroPython itself have changed since last build!

  ```shell
  make -j -C ports/stm32/ BOARD=WeAct_F411CE WEACT_VERSION=2 SPIFLASH_MBYTE=2 all
  ```

2. Setup the compiler cache ``ccache``:

  ```shell
  # Install ccache
  sudo apt install -y ccache

  # Add ccache permanently to your $PATH using a text editor like "nano":
  nano ~/.profile

  # Append these 3 lines: (in "nano" press Ctrl+O to save and Ctrl+X to exit)
  if [ -d "/usr/lib/ccache" ] ; then
      PATH="/usr/lib/ccache:$PATH"
  fi

  # Now login again or execute:
  source ~/.profile
  ```
