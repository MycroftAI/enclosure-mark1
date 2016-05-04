#!/usr/bin/env bash
./install-arduino.sh
ARDUINO_SDK_PATH=/opt/arduino
mkdir -p build
${ARDUINO_SDK_PATH}/arduino-builder -dump-prefs -logger=machine -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=none -prefs=build.warn_data_percentage=75 -verbose "${PWD}/src/enclosure.ino"
${ARDUINO_SDK_PATH}/arduino-builder -compile -logger=machine -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=none -prefs=build.warn_data_percentage=75 -verbose "${PWD}/src/enclosure.ino"
