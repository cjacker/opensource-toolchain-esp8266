#!/bin/bash
esptool.py -p /dev/ttyUSB0 -b 115200 write_flash -e @download.config
