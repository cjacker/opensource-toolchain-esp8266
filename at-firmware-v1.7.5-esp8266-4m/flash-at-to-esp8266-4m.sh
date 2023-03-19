#!/bin/bash
# gen_misc.sh 1 1 3 2 6
# make COMPILE=gcc BOOT=new APP=1 SPI_SPEED=80 SPI_MODE=DIO SPI_SIZE_MAP=6
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
	-e \
	-fm dio --flash_freq 80m --flash_size 4MB \
	0x0000 boot_v1.7.bin \
	0x1000 user1.4096.new.6.bin \
	0x3fc000 esp_init_data_default_v08.bin \
