# Opensource toolchain tutorial for Espressif ESP8266

For ESP32 series, please refer to https://github.com/cjacker/opensource-toolchain-esp32, and ESP32-C2/C3 is recommended to use instead to replace ESP8266.

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

# Table of contents
- [Hardware prerequiest](#hardware-prerequiest)
- [Toolchain overview](#toolchain-overview)
- [Compiler](#compiler)
- [SDK](#sdk)
  + [ESP8266_RTOS_SDK](#esp8266_rtos_sdk)
  + [ESP8266_NONOS_SDK](#esp8266_nonos_sdk)
- [Programming](#programming)
  + [ESP8266_RTOS_SDK](#esp8266_rtos_sdk-1)
  + [ESP8266_NONOS_SDK](#esp8266_nonos_sdk-1)
- [Debugging](#debugging)
- [Classic AT firmware for ESP-1S](#classic-at-firmware-for-esp8266)
- [AT-MQTT firmware for ESP-1S](#at-mqtt-firmware-for-esp-1s)
 [AT simple usage](#at-simple-usage)

# Hardware prerequiest
- ESP8266 devboard
  + I use D1 mini (4M Flash) and An-Thinker ESP-1S in this tutorial

# Toolchain overview
- Compiler: Xtensa L106 GNU Toolchain
- SDK: ESP8266_RTOS_SDK / ESP8266_NONOS_SDK (**deprecated**)
- Programming Tool: esptool integrated in SDK
- Debugging: UART log

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
## ESP8266_RTOS_SDK

Espressif released two versions of the SDK — one is [ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK) based FreeRTOS and the other is ESP8266_NONOS_SDK which already deprecated. This tutorial will focus on [ESP8266_RTOS_SDK](https://github.com/espressif/ESP8266_RTOS_SDK), since it is supported and still maintained by upstream.

### setup the SDK

I put the sdk in `~/esp` dir.
```
mkdir ~/esp && cd ~/esp
git clone https://github.com/espressif/ESP8266_RTOS_SDK.git
cd ESP8266_RTOS_SDK
git submodule update --init --recursive --progress
```
You can also use the release version as you like, as this tutorial written, the latest stable release is v3.4.

An env var need exported to find the SDK:
```
export IDF_PATH=$HOME/esp/ESP8266_RTOS_SDK
```
And you also need install some python modules the SDK required:
```
python -m pip install --user -r $IDF_PATH/requirements.txt
```

### build hello_world demo
There is some examples provided by ESP8266_RTOS_SDK, you can build the hello_world example as:
```
cd ~/esp/ESP8266_RTOS_SDK/examples/get-started/hello_world
make
```

When build hello_world first time, it will invoke `make menuconfig` to generate `sdkconfig` file, you can tune some options as your need. The `menuconfig` can be invoke by `make menuconfig`, you can use `make help` to find more usages.

After build successfully, the 'hello_world.bin/elf` will be generated at `build` dir.


## ESP8266_NONOS_SDK

Non-OS SDK is not based on an operating system. And deprecated from December 2019, but there are still a lot of users stay with Non-OS SDK

### Setup the SDK

I put the sdk in `~/esp` dir.
```
mkdir -p ~/esp && cd ~/esp
git clone https://github.com/espressif/ESP8266_NONOS_SDK.git
cd ESP8266_NONOS_SDK
git submodule update --init --recursive --progress
```

You can also use the release version as you like, as this tutorial written, the latest stable release is v3.0.5

And since all python script in Non-OS SDK is python2, you should fix the python version issue as:
```
sed -i "s/@python /@python2 /g" ~/esp/ESP8266_NONOS_SDK/Makefile
```

Not like ESP8266_RTOS_SDK, it doesn't support 'out-of-sdk' projects, you have to put your project at SDK dir, for example, use `blink-nonos` demo in this repo as example, you should put it to `~/esp/ESP8266_NONOS_SDK` dir.
```
cp -r blink-nonos ~/esp/ESP8266_NONOS_SDK
```

### Build blink-nonos demo

#### with gen_misc.sh

You can use `gen_misc.sh` to build nonos project as:

```
cd ~/esp/ESP8266_NONOS_SDK/blink-nonos
./gen_misc.sh
```
There are some questions you need answer, here I used D1 mini with 4M Flash:
```
gen_misc.sh version 20150511

Please follow below steps(1-5) to generate specific bin(s):
STEP 1: choose boot version(0=boot_v1.1, 1=boot_v1.2+, 2=none)
enter(0/1/2, default 2):
1
boot mode: new

STEP 2: choose bin generate(0=eagle.flash.bin+eagle.irom0text.bin, 1=user1.bin, 2=user2.bin)
enter (0/1/2, default 0):
1
generate bin: user2.bin

STEP 3: choose spi speed(0=20MHz, 1=26.7MHz, 2=40MHz, 3=80MHz)
enter (0/1/2/3, default 2):
3
spi speed: 80 MHz

STEP 4: choose spi mode(0=QIO, 1=QOUT, 2=DIO, 3=DOUT)
enter (0/1/2/3, default 0):
2
spi mode: DIO

STEP 5: choose spi size and map
    0= 512KB( 256KB+ 256KB)
    2=1024KB( 512KB+ 512KB)
    3=2048KB( 512KB+ 512KB)
    4=4096KB( 512KB+ 512KB)
    5=2048KB(1024KB+1024KB)
    6=4096KB(1024KB+1024KB)
    7=4096KB(2048KB+2048KB) not support ,just for compatible with nodeMCU board
    8=8192KB(1024KB+1024KB)
    9=16384KB(1024KB+1024KB)
enter (0/2/3/4/5/6/7/8/9, default 0):
4
spi size: 4096KB
spi ota map:  512KB + 512KB


start...
```

And it will start build automatically, the output looks like:
```
Support boot_v1.2 and +
Generate user2.4096.new.4.bin successully in folder bin/upgrade.
boot.bin------------>0x00000
user1.4096.new.4.bin--->0x1000
```

You can also choose `2` when 
```
STEP 2: choose bin generate(0=eagle.flash.bin+eagle.irom0text.bin, 1=user1.bin, 2=user2.bin)
```
And the final user firmware will be `user2.4096.new.4.bin` and should program to `0x81000`.

The `user1.bin` and `user2.bin` is something related to FOTA (firmware over the air), it is a update mechanism, you can treat it as AB system for now, if A running, then update B and switch to B. For this simple blink demo, use `user1.bin` or `user2.bin` or both as you like.

#### with make

```
make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=4
```
Or use `APP=2` 
```
make COMPILE=gcc BOOT=new APP=2 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=4
```
You may be confused which one should choose when running `gen_misc.sh` and which option should used with `make`, I will explain it next section.

**NOTE :** Partition table 

Partition table was introduced from NonOS SDK v3.0, you have to add user_pre_init() in your project, which will be called before user_init(). And you MUST call system_partition_table_regist() in user_pre_init to register your project partition table. Otherwise you may encounter rebooting loop issue with serial output `rf_cal[0] !=0x05,is 0xFF`.


# Programming

Connect the USB typec or micro port of ESP8266 devboard to USB PC port, usually there is a UART chip integrated on board and you should find `/dev/ttyACM0` or `/dev/ttyUSB0` created after devboard plugged in.

## ESP8266_RTOS_SDK
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

## ESP8266_NONOS_SDK

After blink-nonos demo built successfully, enter `~/esp/ESP8266_NONOS_SDK/bin` dir.

You will find these files:
- boot_v1.2.bin / boot_v1.6.bin / boot_v1.7.bin, boot firmware provided by SDK.
- esp_init_data_default_v05.bin / esp_init_data_default_v08.bin, the default init data provided by SDK.
- upgrade/user1.4096.new.4.bin, user1 firmware.
- upgrade/user2.4096.new.4.bin, user2 firmware if you built it again with `APP=2`.

For first time programming:
```
esptool.py --port /dev/ttyUSB0 -b 115200 erase_flash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0 boot_v1.7.bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x1000 upgrade/user1.4096.new.4.bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x81000 upgrade/user2.4096.new.4.bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x3fc000 esp_init_data_default_v08.bin
```

You can also combine these together:
```
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
-e -fm dio --flash_freq 80m --flash_size 4MB \
0x00000 boot_v1.7.bin \
0x1000 upgrade/user1.4096.new.4.bin \
0x81000 upgrade/user2.4096.new.4.bin \
0x3fc000 esp_init_data_default_v08.bin
```

If you already have these firmware programmed, you can only update the user firmware as:

```
esptool.py --port /dev/ttyUSB0 -b 115200  write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x1000 upgrade/user1.4096.new.4.bin
```
Or
```
esptool.py --port /dev/ttyUSB0 -b 115200  write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x81000 upgrade/user2.4096.new.4.bin
```

**NOTE**, the `-fm dio --flash_freq 80m --flash_size 4MB` should match exactly what you speficied with `make` or what you input when running `gen_misc.sh` to build the project.

And let's explain some inputs to `gen_misc.sh` when building this project:

### why program user2 firmware to 0x81000 ?

We know user1 should always program to `0x1000`. why program user2 firmware to 0x81000 ?

Actually, the target address depend on flash size, you can program `boot_v1.7.bin` to target device as:

```
esptool.py --port /dev/ttyUSB0 -b 115200 erase_flash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0 boot_v1.7.bin
```

Then use tio to open the serial port:

```
tio -b 74880 /dev/ttyUSB0
```

Then reset the target device, the output should look like:
```
2nd boot version : 1.7
  SPI Speed      : 80MHz
  SPI Mode       : DIO
  SPI Flash Size & Map: 32Mbit(512KB+512KB)
no GPIO select!
jump to run user2 @ 81000
```
The line `jump to run user2 @ 81000` tell us user2 firmware should program to 0x81000. Since the start addr of user2 depends on flash size, so you need to be very careful to provide correct args for `esptool.py` when programming.

### why program `esp_init_data_default_v08.bin` to `0x3fc000` ?

It is defined by partition table introduced from NONOS SDK v3.0, refer to [blink-nonos/user/partitions.c](./blink-nonos/user/partitions.c) for more information, usually:

- 0x7c000 for 512K flash
- 0xfc000 for 1M flash
- 0x1fc000 for 2M flash
- 0x3fc000 for 4M flash

# Debugging

There is no JTAG debugging support for ESP8266, although there is [some work to openocd](https://www.esp8266.com/viewtopic.php?f=9&t=1871) already done, but as I verified, they all don't work.

You have to use UART log to debug ESP8266 as mentioned above, such as `make monitor` or `tio -b 74880 /dev/ttyUSB0`.


# Classic AT firmware for ESP8266

The latest classic AT firmware is **v1.7.5** provided by ESP8266_NONOS_SDK. Since The most common devices that use ESP8266 is ESP-1S module, I will introduce how to build and flash at firmware of ESP-1S in the first section.

For usage of classic AT firmware, please refer to official document [ESP8266 Non-OS AT Instruction Set](https://www.espressif.com/sites/default/files/documentation/4a-esp8266_at_instruction_set_en.pdf).

## ESP-1S

Since I don't have ESP-1 module, I will use ESP-1S as example, ESP-1S has 1M flash and ESP-1 has only 512K flash, it will affect building options, you should change it if you have a ESP-1 module.

### Check AT firmware version

Since ESP-1S module has no UART chip integrated on board, you have to use a external USB2TTL adapter and wire it up as:

| USB2TTL | ESP-1S |
|---------|--------|
| 3v3     | 3v3    |
| GND     | GND    |
| TX      | RX     |
| RX      | TX     |

NOTE, **DO NOT use 5V power supply, ESP-1S is 3V3 tolerance.**

And use tio as:
```
tio -b 115200 -m ONLCRNL /dev/ttyUSB0 
```
or picocom
```
picocom -b 115200 --omap crcrlf /dev/ttyUSB0
```
And input:
```
AT+GMR
```
The output look like :
```
AT version:0.25.0.0(Jun  5 2015 16:27:16)
SDK version:1.1.1
Ai-Thinker Technology Co. Ltd.
Jun 23 2015 23:23:50

OK
```

Because AT commands must be terminated with both the CR and LF ASCII control codes, I use `-m ONLCRNL` with tio or `--omap crcrlf` with picocom, otherwise, you have to use `Ctrl-M Ctrl-J` instead of `Enter` key.


### build and flash at_nano firmware

For ESP-1S, since it has 1M flash, it can only use `at_nano` firmware which supports an SSL library with fewer ciphers but fits on 1 MB of flash memory.

You can use some ESP-1S Dock to flash ESP-1S. if you do not have one, wire it up with a USB2TTL adapter as:

| USB2TTL | ESP-1[S] |
|---------|----------|
| 3v3     | 3v3      |
| 3v3     | RST      |
| 3v3     | EN       |
| 3v3     | GPIO2    |
| GND     | GND      |
| GND     | GPIO0    |
| RX      | TX       |
| TX      | RX       |

Note again, **DO NOT use 5V power supply, ESP-1S is 3V3 tolerance.**

#### To build at_nano firmware for ESP-1S
```
cd ESP8266_NONOS_SDK
cp -r examples/at_nano . && cd at_nano
make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=2
```

The output looks like:
```
Support boot_v1.2 and +
Generate user1.1024.new.2.bin successully in folder bin/upgrade.
boot.bin------------>0x00000
user1.1024.new.2.bin--->0x01000
```

#### to flash to ESP-1S
After `user1.1024.new.2.bin` generated, program it to ESP-1S as:
```
cd <where you put>/ESP8266_NONOS_SDK/bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
    -e \
    -fm dio --flash_freq 80m --flash_size 1MB \
    0x00000 boot_v1.7.bin \
    0x01000 upgrade/user1.1024.new.2.bin \
    0xfc000 esp_init_data_default_v08.bin
```

Actually, it can be used to program to 2M or 4M flash size too.

After firemware programmed, please refer to above "Check AT firmware version" section to verify it.

I also put a copy at [at-firmware-v1.7.5-esp8266-1m](./at-firmware-v1.7.5-esp8266-1m) dir in this repo, you can use `flash-at_nano-to-esp-1s.sh` to program it to ESP-1S.

## Other 8266 devboards

It's simpler to program at firmware for various ESP8266 devboards. Most of them have a UART chip integrated, it's not necessary to care about how to wire up, and most of them has 2M or 4M flash, `at` firmware can be used instead of `at_nano`.

### build at firmware
Use d1 mini with 4M flash as example.

```
cd ESP8266_NONOS_SDK
cp -r examples/at . && cd at
make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=6
```

The output looks like:
```
Support boot_v1.4 and +
Generate user1.4096.new.6.bin successully in folder bin/upgrade.
boot.bin------------>0x00000
user1.4096.new.6.bin--->0x01000
```

#### to flash to D1 mini
After `user1.4096.new.6.bin` generated, program it to ESP-1S as:
```
cd <where you put>/ESP8266_NONOS_SDK/bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
    -e \
    -fm dio --flash_freq 80m --flash_size 4MB \
    0x0000 boot_v1.7.bin \
    0x1000 upgrade/user1.4096.new.6.bin \
    0x3fc000 esp_init_data_default_v08.bin \
```

After programming finished, you can verify it by `tio -b 115200 /dev/ttyUSB0 -m ONLCRNL`:
```
$ tio -b 115200 /dev/ttyUSB0 -m ONLCRNL
[14:59:57.369] tio v2.5
[14:59:57.369] Press ctrl-t q to quit
[14:59:57.370] Connected
AT

OK

AT+GMR
AT version:1.7.5.0(Oct  9 2021 09:26:04)
SDK version:3.0.5(b29dcd3)
compile time:Mar 16 2023 13:14:07
OK
```

I also put a copy at [at-firmware-v1.7.5-esp8266-4m](./at-firmware-v1.7.5-esp8266-4m) dir in this repo, you can use `flash-at-to-esp8266-4m.sh` to program it to ESP8266 device with 4M flash.


# AT-MQTT firmware for ESP8266

[ESP-AT](https://github.com/espressif/esp-at) firmware for ESP8266 is newer than classic AT firmware, sometimes it also refers to `AT-MQTT` firmware. it is based on ESP8266_RTOS_SDK and the latest version is v2.2.1.0.

> v2.2.1.0_esp8266 is the last version of ESP-AT for ESP8266, corresponding to branch release/v2.2.0.0_esp8266, corresponding to documentation https://docs.espressif.com/projects/esp-at/en/release-v2.2.0.0_esp8266.


## ESP-1S
<strike>
The building process should be:
```
git clone https://github.com/espressif/esp-at.git
cd esp-at
git checkout release/v2.2.0.0_esp8266
./build.py menuconfig
```
Then you will be prompted to config some options as:
```
Platform name:
1. PLATFORM_ESP32
2. PLATFORM_ESP8266
3. PLATFORM_ESP32S2
4. PLATFORM_ESP32C3
choose(range[1,4]):2

Module name:
1. WROOM-02 (description: TX:15 RX:13)
2. WROOM-5V2L (description: 5V UART level)
3. ESP8266_1MB (description: No OTA)
4. WROOM-02-N (description: TX:1 RX:3)
5. WROOM-S2
6. ESP8266_QCLOUD (description: QCLOUD TX:15 RX:13)
choose(range[1,6]):3

Enable silence mode to remove some logs and reduce the firmware size?
0. No
1. Yes
choose(range[0,1]):0
```

After menu show up, go to `Component config -> Common ESP-related -> UART for console output` and choose `UART0 (TX1 RX3)`.

Then built it as:
```
./build.py build
```
</strike>

As I tried, I failed to build a workable firmware for ESP8266 with 1M flash, I think the support of upstream may broken due to some unknown reason.

The latest workable firmware can be downloaded from ai-thinker official website with below version information:

```
AT version:2.3.0.0-dev(s-bcd64d2 - ESP8266 - Jun 23 2021 11:42:05)
SDK version:v3.4-22-g967752e2
compile time(b498b58):Jun 30 2021 11:28:20
Bin version:2.2.0(ESP8266_1MB)
```

I put a copy at [at-mqtt-firmware-v2.3dev-esp8266-1m](./at-mqtt-firmware-v2.3dev-esp8266-1m) dir in this repo, you can use `flash-at_mqtt-2.3dev-to-esp-1s.sh` to program it to ESP-1S.

After programming finished, you can verify it by `tio -b 115200 /dev/ttyUSB0 -m ONLCRNL`:
```
$ tio -b 115200 /dev/ttyUSB0 -m ONLCRNL
```
And input
```
AT+GMR

AT version:2.3.0.0-dev(s-bcd64d2 - ESP8266 - Jun 23 2021 11:42:05)
SDK version:v3.4-22-g967752e2
compile time(b498b58):Jun 30 2021 11:28:20
Bin version:2.2.0(ESP8266_1MB)

OK
```

## Other ESP8266 devboards
```
git clone https://github.com/espressif/esp-at.git
cd esp-at
git checkout release/v2.2.0.0_esp8266
./build.py menuconfig
```
Then you will be prompted to config some options:
```
Platform name:
1. PLATFORM_ESP32
2. PLATFORM_ESP8266
3. PLATFORM_ESP32S2
4. PLATFORM_ESP32C3
choose(range[1,4]):2

Module name:
1. WROOM-02 (description: TX:15 RX:13)
2. WROOM-5V2L (description: 5V UART level)
3. ESP8266_1MB (description: No OTA)
4. WROOM-02-N (description: TX:1 RX:3)
5. WROOM-S2
6. ESP8266_QCLOUD (description: QCLOUD TX:15 RX:13)
choose(range[1,6]):4

Enable silence mode to remove some logs and reduce the firmware size?
0. No
1. Yes
choose(range[0,1]):0
```

After menu show up:
- go to `Serial flash config`, choose `Flash size` according to your devboard.
- go to `Component config -> Common ESP-related -> UART for console output` and choose `UART0 (TX1 RX3)`.

Then built it as:
```
./build.py build
```

After built successfully, program the target device as:
```
./build.py flash
```

I put a copy at [at-mqtt-firmware-v2.2.2.0dev-esp8266-4m](./at-mqtt-firmware-v2.2.2.0dev-esp8266-4m) dir in this repo, you can use `flash.sh` to program it to ESP8266 with 4m flash.

After programming finished, you can verify it by `tio -b 115200 /dev/ttyUSB0 -m ONLCRNL`:
```
$ tio -b 115200 /dev/ttyUSB0 -m ONLCRNL
```
And input
```
AT+GMR

AT version:2.2.2.0-dev(952f658 - ESP8266 - Nov 23 2022 06:37:28)
SDK version:v3.4-63-ge3348ac7
compile time(64dbe87):Mar 19 2023 21:25:48
Bin version:2.2.1(WROOM-02-N)

OK
```


# AT simple usage
Please refer to official documents for detail usage, here is just a brief intro to connect to WiFi:
```
# should return 'OK'
AT
# show firmware version information
AT+GMR
# current mode
AT+CWMODE?
# change to station mode
AT+CWMODE=1
# scan WiFi
AT+CWLAP
# connect
AT+CWJAP="SSID","password"
# show IP
AT+CIPSTA?
# ping
AT+PING="A IP"
```
