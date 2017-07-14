/**
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
**/
#include "ArduinoProcessor.h"
#include "MycroftArduino.h"

ArduinoProcessor::ArduinoProcessor(MycroftArduino &arduino)
  : BaseProcessor("system"), arduino(arduino)
{
}

void ArduinoProcessor::setup() {
	arduino.setup();
}

void ArduinoProcessor::process(const String& cmd) {
	if (cmd.startsWith("reset")) {
		arduino.reset();
	} else if (cmd.startsWith("mute")) {
		arduino.mute();
	} else if (cmd.startsWith("unmute")) {
		arduino.unmute();
	} else if (cmd.startsWith("blink=")) {
		arduino.blink(cmd.substring(6).toInt(), 500);
	} else if (cmd.startsWith("version")) {
		Serial.println(F("Mycroft Mark 1 v" ENCLOSURE_VERSION_STRING));
	}
}
