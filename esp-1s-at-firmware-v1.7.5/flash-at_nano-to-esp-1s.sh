#!/bin/bash
# cd ESP8266-NONOS-SDK-3.0.5
# cp -r example/at_nano . && cd at_nano
# make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=2

esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
    -e \
    -fm dio --flash_freq 80m --flash_size 1MB \
	0x00000 boot_v1.7.bin \
	0x01000 user1.1024.new.2.bin \
	0xfc000 esp_init_data_default_v08.bin
