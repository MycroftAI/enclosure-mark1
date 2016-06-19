#!/usr/bin/env bash
sudo /opt/avrdude/bin/avrdude -p atmega328p -C avrdude-gpio.conf -c linuxgpio -v -U flash:w:/opt/enclosure/build/enclosure.ino.hex
