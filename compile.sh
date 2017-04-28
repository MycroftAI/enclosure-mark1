#!/usr/bin/env bash
ARDUINO_SDK_PATH=../arduino-1.8.2
mkdir -p build
${ARDUINO_SDK_PATH}/arduino-builder -compile -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=all -prefs=build.warn_data_percentage=75 "${PWD}/src/enclosure.ino"
