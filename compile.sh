#!/usr/bin/env bash
ARDUINO_SDK_PATH=/path/to/arduino
MYCROFT_HOSTNAME=pi@10.42.0.22
PROJECT_HOME=/home/pi/git/enclosure

mkdir -p build
${ARDUINO_SDK_PATH}/arduino-builder -dump-prefs -logger=machine -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=none -prefs=build.warn_data_percentage=75 -verbose "${PWD}/src/enclosure.ino"
${ARDUINO_SDK_PATH}/arduino-builder -compile -logger=machine -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=none -prefs=build.warn_data_percentage=75 -verbose "${PWD}/src/enclosure.ino"
sshpass -p 'mycroft' scp ${PWD}/build/enclosure.ino.hex ${MYCROFT_HOSTNAME}:${PROJECT_HOME}/enclosure.hex
