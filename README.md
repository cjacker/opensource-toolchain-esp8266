# Opensource toolchain tutorial for Espressif ESP8266

The ESP8266 is a low-cost Wi-Fi microchip, with built-in TCP/IP networking software, and microcontroller capability, produced by Espressif Systems in Shanghai, China. The chip was popularized in the English-speaking maker community in August 2014 via the ESP-01 module, made by a third-party manufacturer Ai-Thinker. This small module allows microcontrollers to connect to a Wi-Fi network and make simple TCP/IP connections using Hayes-style commands. 

**Features**

- Processor: L106 32-bit RISC microprocessor core based on the Tensilica Diamond Standard 106Micro running at 80 or 160 MHz
- Memory:
  + 32 KiB instruction RAM
  + 32 KiB instruction cache RAM
  + 80 KiB user-data RAM
  + 16 KiB ETS system-data RAM
- External QSPI flash: up to 16 MiB is supported (512 KiB to 4 MiB typically included)
- IEEE 802.11 b/g/n Wi-Fi
  + Integrated TR switch, balun, LNA, power amplifier and matching network
  + WEP or WPA/WPA2 authentication, or open networks
- 17 GPIO pins
- Serial Peripheral Interface Bus (SPI)
- I²C (software implementation)
- I²S interfaces with DMA (sharing pins with GPIO)
- UART on dedicated pins, plus a transmit-only UART can be enabled on GPIO2
- 10-bit ADC (successive approximation ADC)

# Hardware prerequiest
- ESP8266 devboard
  + I use D1 mini in this tutorial
- Any JTAG debugger

# Toolchain overview
- Compiler: Xtensa L106 GNU Toolchain
- SDK: ESP8266_RTOS_SDK / ESP8266_NONOS_SDK (**deprecated**)
- Programming Tool: esptool integrated in SDK
- Debugging: esp8266-openocd / gdb

# Compiler

Espressif provide pre-built Xtensa L106 GNU toolchain, you can download it from:
- for 64bit Linux: https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
- for 32bit Linux: https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-i686.tar.gz

After download finished, extract it as:
```
sudo tar xf xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz -C /opt
```

It has well-formed dir name `xtensa-lx106-elf` and it is also the triplet of Xtensa L106 GNU Toolchain.

Do not forget to add `/opt/xtensa-lx106-elf/bin` to PATH env of your shell.


# SDK
Espressif released two versions of the SDK — one is [ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) based FreeRTOS and the other is ESP8266_NONOS_SDK which already deprecated. This tutorial will focus on [ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK), since it is supported and still maintained by upstream.

## setup the SDK

I put the sdk in `~/esp` dir.
```
mkdir ~/esp && cd ~/esp
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git
cd ESP8266_RTOS_SDK
git submodule update --init --recursive --progress
```

A env var need exported to find the SDK:
```
export IDF_PATH=$HOME/esp/ESP8266_RTOS_SDK
```
And you also need install some python modules the SDK required:
```
python -m pip install --user -r $IDF_PATH/requirements.txt
```

## build hello_world demo
There is some examples provided by ESP8266_RTOS_SDK, you can build the hello_world example as:
```
cd ~/esp/ESP8266_RTOS_SDK/examples/get-started/hello_world
make
```

When build hello_world first time, it will invoke `make menuconfig` to generate `sdkconfig` file, you can tune some options as your need. The `menuconfig` can be invoke by `make menuconfig`, you can use `make help` to find more usages.

After build successfully, the 'hello_world.bin/elf` will be generated at `build` dir.

# Programming

Connect the USB typec or micro port of ESP8266 devboard to USB PC port, usually there is a UART chip integrated on board and you should find `/dev/ttyACM0` or `/dev/ttyUSB0` created after devboard plugged in.

Then you can program the target device as:
```
make flash
```
Or flash just the app when you flashed this project one time already
```
make app-flash
```

`hello_world` demo is an UART demo, you can use serial terminal to monitor it, SDK provided a python tool named idf_monitor.py, you can use it as:
```
make monitor
```

The output of `hello_world` demo looks like:
```
--- idf_monitor on /dev/ttyUSB0 74880 ---
--- Quit: Ctrl+] | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---
load 0x40100000, len 7544, room 16
tail 8
chksum 0xee
load 0x3ffe8408, len 24, room 0
tail 8
chksum 0x67
load 0x3ffe8420, len 3472, room 0
tail 0
chksum 0x51
csum 0x51
I (46) boot: ESP-IDF v3.4-75-g7973d71b-dirty 2nd stage bootloader
I (46) boot: compile time 21:17:52
I (47) qio_mode: Enabling default flash chip QIO
I (55) boot: SPI Speed      : 40MHz
I (61) boot: SPI Mode       : QIO
I (67) boot: SPI Flash Size : 2MB
I (73) boot: Partition Table:
...
```

You can also use tio as:
```
tio -b 74880 /dev/ttyUSB0
```

The baudrate can be setup and found in `sdkconfig`.



