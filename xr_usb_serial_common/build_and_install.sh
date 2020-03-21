#!/bin/bash

set -e

apt-get install raspberrypi-kernel-headers
make
rmmod cdc-acm
modprobe -r usbserial
modprobe usbserial
insmod ./xr_usb_serial_common.ko
