

# Purpose
This C++ application will read weather information from Davis Weather stations via the USB serial, using a Linux device such as a Raspberry Pi, or an X86 intel powered computer. It will then store it on disk and/or send it to the cloud.

## Connecting the Davis to the Linux Device
Currently, the stations that are known to work with this application are:
* Vantage Vue
* Vantage Pro2

The Davis Weather station can be connected to the Raspberry Pi or Linux device via the USB serial connection. The Davis needs to be connected to the Raspberry Pi using this module -> http://www.davisnet.com/product/weatherlink-windows-usb/ . Once connected to the Pi or Linux device, the Ardexa application will determine which serial/USB device it is connected to, and collect weather data. 

## Building the Ardexa software
To use this application, do the following. It will install the `ardexa-davis` binary in the directory `/usr/local/bin`
```
sudo apt-get update
sudo apt-get install -y libudev-dev cmake
cd
git clone https://github.com/ardexa/davis-weather-station.git
cd davis-weather-station
mkdir build
cd build
cmake ..
make
sudo make install
```

## How does it work
This application is written in C++. Once built, the application will query a Davis weather station using the USB/serial link. Each time this application is run, data will be written to log files on disk in a directory specified via the command line. Usage and command line parameters are as follows. Note that the applications should be run as root only since it has access to a device in the `/dev` directory. 

Usage: sudo ardexa-davis [-t device] [-d directory] [-e] [-w] [-b barometer calibration]
```
-t <device> (optional) This is the name of the device (eg; '/dev/ttyUSB0'). If not specified, the application will find the device for you.
-d <directory> (optional) This is the name of the logging directory. Defaults to: `/opt/ardexa/davis/`
-e (optional) if specified, debug will be turned on
-w (optional) if specified, wind speed is in km/h, not m/s
-b (optional) if specified, will calibrate the barometer using this number as a multiplication to the raw value
```

## Collecting to the Ardexa cloud
Collecting to the Ardexa cloud is free for up to 3 Raspberry Pis (or equivalent). Ardexa provides free agents for ARM, Intel x86 and MIPS based processors. To collect the data to the Ardexa cloud do the following:
a. Create a `RUN` scenario to schedule the Ardexa Davis program to run at regular intervals (say every 60 seconds).
b. Then use a `CAPTURE` scenario to collect the csv (comma separated) data from the filename `/opt/ardexa/davis/latest.csv`. This file contains a header entry (as the first line) that describes the CSV elements of the file.

## Help
Contact Ardexa, and we'll do our best efforts to help.


