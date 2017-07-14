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
#include "MycroftEncoder.h"
#include "ClickEncoder.h"

MycroftEncoder::MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton)
  : clickEncoder(new ClickEncoder(pinEncoderOne, pinEncoderTwo, pinButton, 2)),
    PIN_BUTTON(pinButton),
    last(0), value(0),
    framesHeld(0), clicked(false)
{
}

MycroftEncoder::Direction MycroftEncoder::getDirection() {
	direction = Direction::NONE;
	value += clickEncoder->getValue();
	if (value != last) {
		if (value < last) {
			direction = Direction::LEFT;
		} else if (value > last) {
			direction = Direction::RIGHT;
		} else {
			direction = Direction::NONE;
		}
		last = value;
	}
	return direction;
}

int MycroftEncoder::getFramesHeld() {
	return framesHeld;
}

bool MycroftEncoder::isClicked() {
	if (clicked) {
		clicked = false;
		return true;
	}
	return false;
}

void MycroftEncoder::isr() {
	clickEncoder->service();
	if (digitalRead(PIN_BUTTON) == LOW) {
		framesHeld++;
		if (framesHeld == 1) {
			clicked = true;
		}
	} else if (framesHeld > 0) {
		framesHeld = 0;
	}
}
