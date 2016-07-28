#include <Arduino.h>
#include "MycroftEncoder.h"
#include "ClickEncoder.h"

MycroftEncoder::MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton) : 
last(0), value(0), clickEncoder(new ClickEncoder(pinEncoderOne, pinEncoderTwo, pinButton, 2)),
PIN_BUTTON(pinButton), framesHeld(0), clicked(false) { }

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
