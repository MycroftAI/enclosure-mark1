#!/usr/bin/env bash
TOP=${1:-$(pwd)}
sudo avrdude -p atmega328p -C ${TOP}/avrdude-gpio.conf -c linuxgpio -v -U flash:w:/opt/enclosure/build/enclosure.ino.hex
