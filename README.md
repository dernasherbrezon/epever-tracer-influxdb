## About

Read metrics from Epever Tracer via RS485 and output to InfluxDB.

<img src="https://github.com/dernasherbrezon/epever-tracer-influxdb/raw/master/docs/screen1.png">

## Install

1. Make sure Epever Tracer connected via RS485. If you're using [standard](https://www.amazon.com/gp/product/B016RU8JUY/ref=oh_aui_search_detailpage?ie=UTF8&psc=1) USB->RS485 cable, then you have to install custom drivers:

```bash
cd xr_usb_serial_common
sudo ./build_and_install.sh
```

2. Configure external apt repo:

```bash
sudo add-apt-repository ppa:rodionovamp/epever-tracer-influxdb
sudo apt-get update
```

3. Install application:

```bash
sudo apt-get install epever-tracer-influxdb
```

4. Configure:

```bash
sudo vi /etc/epeverTracer.conf
```

## Build

1. Install dependencies

```bash
sudo apt-get install libmodbus-dev check cmake
```

2. Build

```bash
mkdir build
cd build
cmake ..
make
```