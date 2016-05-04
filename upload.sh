#!/usr/bin/env bash
./install-avrdude.sh
sudo /opt/avrdude/bin/avrdude -p atmega328p -C avrdude-gpio.conf -c linuxgpio -v -U flash:w:enclosure.hex
