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
#include <Arduino.h>
#include "MycroftSystem.h"

MycroftSystem::MycroftSystem(uint8_t speakerPin) : speakerPin(speakerPin) { }

void MycroftSystem::start() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(this->speakerPin, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(this->speakerPin, HIGH);
}

void MycroftSystem::reset() {
	blink(1, 1000);
	asm volatile ("  jmp 0");
}

void MycroftSystem::mute() {
	digitalWrite(this->speakerPin, LOW);
}

void MycroftSystem::unmute() {
	digitalWrite(this->speakerPin, HIGH);
}

void MycroftSystem::blink(long times, unsigned long wait) {
	for (int i = 0; i < times; ++i) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(wait);
		digitalWrite(LED_BUILTIN, LOW);
		delay(wait);
	}
}
