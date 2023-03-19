#!/bin/bash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash -e -fm dio --flash_freq 80m --flash_size 4MB 0x0 at-mqtt-esp8266-4m-tx1-rx3.bin
