########################################################################
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
########################################################################
#!/usr/bin/env bash
ARDUINO_SDK_PATH=/opt/arduino
mkdir -p build
${ARDUINO_SDK_PATH}/arduino-builder -compile -hardware "${ARDUINO_SDK_PATH}/hardware" -tools "${ARDUINO_SDK_PATH}/tools-builder" -tools "${ARDUINO_SDK_PATH}/hardware/tools/avr" -built-in-libraries "${ARDUINO_SDK_PATH}/libraries" -libraries "./lib" -fqbn=arduino:avr:pro:cpu=16MHzatmega328 -ide-version=10608 -build-path "${PWD}/build" -warnings=all -prefs=build.warn_data_percentage=75 "${PWD}/src/enclosure.ino"
