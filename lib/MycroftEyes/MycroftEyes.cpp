#include "MycroftEyes.h"

MycroftEyes::MycroftEyes(uint16_t length, uint8_t pin, neoPixelType type) :
neoPixel(length, pin, type) { }

void MycroftEyes::setup() {
	neoPixel.begin();
	color = neoPixel.Color(255, 255, 255);
	neoPixel.setBrightness(30);
	this->on();
}

void MycroftEyes::set(uint32_t color) {
	for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
		neoPixel.setPixelColor(i, color);
	}
	neoPixel.show();
}

void MycroftEyes::on() {
	this->set(color);
}

void MycroftEyes::off() {
	this->set(0);
}

void MycroftEyes::blink(Side side, byte pos, byte leftJump, unsigned long wait) {
	this->on();
	byte max = neoPixel.numPixels() / 2;
	byte opJump = max / 2;
	byte steps = max / 4;
	leftJump = max + leftJump;
	uint32_t c = 0;

	for (byte i = 0, j = (steps * 2); j > 0; j--) {
		uint16_t r1 = mod(pos + i, max);
		uint16_t r2 = mod(pos - 1 - i, max);
		uint16_t ro1 = mod(r1 + opJump, max);
		uint16_t ro2 = mod(r2 + opJump, max);

		if (side == RIGHT || side == BOTH) {
			neoPixel.setPixelColor(r1, c);
			neoPixel.setPixelColor(r2, c);
			neoPixel.setPixelColor(ro1, c);
			neoPixel.setPixelColor(ro2, c);
		}
		if (side == LEFT || side == BOTH) {
			neoPixel.setPixelColor(mod(r1 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(r2 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(ro1 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(ro2 + leftJump, max) + max, c);
		}
		neoPixel.show();
		delay(wait);

		if (i == steps - 1) {
			c = color;
		}
		if (c != color) {
			i++;
		} else {
			i = j - 2;
		}
	}
}

void MycroftEyes::blink(unsigned long wait, const char side) {
	if (side == 'r') {
		this->blink(RIGHT, 0, 0, wait);
	} else if (side == 'l') {
		this->blink(LEFT, 0, 0, wait);
	} else {
		this->blink(BOTH, 0, 0, wait);
	}
}

void MycroftEyes::narrow(Side side, byte pos, byte leftJump, unsigned long wait) {
	this->on();
	byte max = neoPixel.numPixels() / 2;
	byte opJump = max / 2;
	byte steps = (max / 4) - 1;
	leftJump = max + leftJump;
	uint32_t c = 0;

	for (byte i = 0; i < steps; i++) {
		uint16_t r1 = mod(pos + i, max);
		uint16_t r2 = mod(pos - 1 - i, max);
		uint16_t ro1 = mod(r1 + opJump, max);
		uint16_t ro2 = mod(r2 + opJump, max);

		if (side == RIGHT || side == BOTH) {
			neoPixel.setPixelColor(r1, c);
			neoPixel.setPixelColor(r2, c);
			neoPixel.setPixelColor(ro1, c);
			neoPixel.setPixelColor(ro2, c);
		}
		if (side == LEFT || side == BOTH) {
			neoPixel.setPixelColor(mod(r1 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(r2 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(ro1 + leftJump, max) + max, c);
			neoPixel.setPixelColor(mod(ro2 + leftJump, max) + max, c);
		}
		neoPixel.show();
		delay(wait);
	}
}

void MycroftEyes::narrow(unsigned long wait, const char side) {
	if (side == 'r') {
		this->narrow(RIGHT, 0, 0, wait);
	} else if (side == 'l') {
		this->narrow(LEFT, 0, 0, wait);
	} else {
		this->narrow(BOTH, 0, 0, wait);
	}
}

void MycroftEyes::look(byte pos, byte leftJump, unsigned long wait) {
	this->on();
	byte max = neoPixel.numPixels() / 2;
	byte steps = (max / 2) - 1;
	leftJump = max + leftJump;

	for (byte i = 0; i < steps; i++) {
		uint16_t r1 = mod(pos + i, max);
		uint16_t r2 = mod(pos - 1 - i, max);
		neoPixel.setPixelColor(r1, 0);
		neoPixel.setPixelColor(r2, 0);
		neoPixel.setPixelColor(mod(r1 + leftJump, max) + max, 0);
		neoPixel.setPixelColor(mod(r2 + leftJump, max) + max, 0);
		neoPixel.show();
		delay(wait);
	}
}

void MycroftEyes::look(unsigned long wait, const char side) {
	if (side == 'r') {
		look(9, 0, wait);
	} else if (side == 'l') {
		look(3, 0, wait);
	} else if (side == 'u') {
		look(6, 0, wait);
	} else if (side == 'd') {
		look(0, 0, wait);
	} else if (side == 'c') {
		look(3, 6, wait);
	}
}

void MycroftEyes::updateColor(uint8_t r, uint8_t g, uint8_t b) {
	color = neoPixel.Color(r, g, b);
	this->on();
}

void MycroftEyes::updateBrightness(uint8_t level) {
	neoPixel.setBrightness(level);
	this->on();
}

uint16_t MycroftEyes::mod(long a, long b) {
	return (uint16_t) ((a % b + b) % b);
}
