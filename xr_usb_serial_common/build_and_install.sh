#!/bin/bash

set -e

apt-get install raspberrypi-kernel-headers dkms

cp -a ../xr_usb_serial_common /usr/src/xr_usb_serial_common-1a
dkms add -m xr_usb_serial_common -v 1a
dkms build -m xr_usb_serial_common -v 1a
dkms install -m xr_usb_serial_common -v 1a

if lsmod | grep "cdc-acm" &> /dev/null ; then
  rmmod cdc-acm
fi

modprobe -r usbserial
modprobe usbserial

make
insmod ./xr_usb_serial_common.ko

echo "blacklist cdc-acm" > /etc/modprobe.d/xrusb-blacklist.conf
