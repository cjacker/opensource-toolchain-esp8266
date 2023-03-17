#!/bin/bash
esptool.py --port /dev/ttyUSB0 -b 115200 write_flash \
	-fm dio --flash_freq 80m --flash_size 1MB \
	-e \
	0x0 ESP8266-AT_MQTT-1M.bin
