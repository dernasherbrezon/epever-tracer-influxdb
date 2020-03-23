#!/bin/bash

set -e

apt-get install raspberrypi-kernel-headers
make
if lsmod | grep "cdc-acm" &> /dev/null ; then
  rmmod cdc-acm
fi
modprobe -r usbserial
modprobe usbserial
insmod ./xr_usb_serial_common.ko

# fixme blacklist cdc-acm and auto load xr on startup