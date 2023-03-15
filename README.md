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

And since all python script in Non-OS SDK is python2, you should fix the python version issue as:
```
sed -i "s/@python /@python2 /g" ~/esp/ESP8266_NONOS_SDK/Makefile
```


Not like ESP8266_RTOS_SDK, it doesn't support 'out-of-sdk' projects, you have to put your project at SDK dir, for example, use `blink-nonos` demo in this repo as example, you should put it to `~/esp/ESP8266_NONOS_SDK` dir.
```
cp -r blink-nonos ~/esp/ESP8266_NONOS_SDK
```

### Build blink-nonos demo

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
2
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
user2.4096.new.4.bin--->0x81000
```

You may be confused which one should choose when running `gen_misc.sh`, I will explain it next section.

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

After build successfully, enter `~/esp/ESP8266_NONOS_SDK/bin` dir.

You will find these files:
- boot_v1.2.bin / boot_v1.6.bin / boot_v1.7.bin, boot firmware provided by SDK.
- esp_init_data_default_v05.bin / esp_init_data_default_v08.bin, the default init data provided by SDK.
- upgrade/user2.4096.new.4.bin, user firmware.

For first time programming:
```
esptool.py --port /dev/ttyUSB0 -b 115200 erase_flash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0 boot_v1.7.bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x81000 upgrade/user2.4096.new.4.bin
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x3fc000 esp_init_data_default_v08.bin
```

If you already have these firmware programmed, you can only update the user firmware as:
```
esptool.py --port /dev/ttyUSB0 -b 115200  write_flash -fm dio --flash_freq 80m --flash_size 4MB 0x81000 upgrade/user2.4096.new.4.bin
```

**NOTE**, the `-fm dio --flash_freq 80m --flash_size 4MB` should match exactly what you input when running `gen_misc.sh` to build the project.

And let's explain some inputs to `gen_misc.sh` when building this project:

### why build user2 firmware ?

```
./gen_misc.sh
...
STEP 2: choose bin generate(0=eagle.flash.bin+eagle.irom0text.bin, 1=user1.bin, 2=user2.bin)
enter (0/1/2, default 0):
2
generate bin: user2.bin
```
The reason choose `2` here is required by `boot_v1.7.bin`, you can program `boot_v1.7.bin` to target device as:

```
esptool.py --port /dev/ttyUSB0 -b 115200 erase_flash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -fm dio --flash_freq 80m --flash_size 4MB 0 boot_v1.7.bin
```

Then use tio to open the serial port:

```
tio -b 74880 /dev/ttyUSB0
```

And reset the target device, the output should look like:
```
2nd boot version : 1.7
  SPI Speed      : 80MHz
  SPI Mode       : DIO
  SPI Flash Size & Map: 32Mbit(512KB+512KB)
no GPIO select!
jump to run user2 @ 81000
```
The line `jump to run user2 @ 81000` tell us user2 firmware should be built and program to 0x81000. The start addr depends on flash size, so you need to be very careful to provide correct args for `esptool.py` to get the correct information.

### why program `esp_init_data_default_v08.bin` to `0x3fc000` ?

It is defined by partition table introduced from NONOS SDK v3.0, refer to [blink-nonos/user/partitions.c](./blink-nonos/user/partitions.c) for more information, usually:

- 0x7c000 for 512K flash
- 0xfc000 for 1M flash
- 0x1fc000 for 2M flash
- 0x3fc000 for 4M flash

# Debugging

There is no JTAG debugging support for ESP8266, although there is [some work to openocd](https://www.esp8266.com/viewtopic.php?f=9&t=1871) already done, but as I verified, they all don't work.

You have to use UART log to debug ESP8266 as mentioned above, such as `make monitor` or `tio -b 74880 /dev/ttyUSB0`.
