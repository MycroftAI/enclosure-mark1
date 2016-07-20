#include "MycroftEyes.h"

MycroftEyes::MycroftEyes(uint16_t length, uint8_t pin, neoPixelType type) :
neoPixel(length, pin, type) { }

void MycroftEyes::updateAnimation() {
	switch (currentAnim) {
		case BLINK:
            runAnim();
            break;
		case NARROW:
            runAnim();
            break;
		case LOOK:
			runAnim();
			break;
		default:
			if (currentState == OPEN) {
			    this->on();
		}
	}
}

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

void MycroftEyes::startAnim(Animation anim, const char side) {
	animSetup(anim, side);
	runAnim();
}

void MycroftEyes::animSetup(Animation anim, const char side) {
	currentAnim = anim;
	setSide(side);
	this->on();
	currentState = ANIMATING;
	setVars();
	resetCounters();
}

void MycroftEyes::runAnim() {
	if (millis() >= nextTime) {
		if (currentAnim == LOOK) {
			uint16_t r1 = mod(pos + i, MAX);
			uint16_t r2 = mod(pos - 1 - i, MAX);
			neoPixel.setPixelColor(r1, 0);
			neoPixel.setPixelColor(r2, 0);
			neoPixel.setPixelColor(mod(r1 + leftJump, MAX) + MAX, 0);
			neoPixel.setPixelColor(mod(r2 + leftJump, MAX) + MAX, 0);
			neoPixel.show();
		}
		else if (currentAnim == BLINK || currentAnim == NARROW) {
			uint16_t r1 = mod(pos + i, MAX);
			uint16_t r2 = mod(pos - 1 - i, MAX);
			uint16_t ro1 = mod(r1 + opJump, MAX);
			uint16_t ro2 = mod(r2 + opJump, MAX);
			if (currentSide == RIGHT || currentSide == BOTH) {
				neoPixel.setPixelColor(r1, c);
				neoPixel.setPixelColor(r2, c);
				neoPixel.setPixelColor(ro1, c);
				neoPixel.setPixelColor(ro2, c);
			}
			if (currentSide == LEFT || currentSide == BOTH) {
				neoPixel.setPixelColor(mod(r1 + leftJump, MAX) + MAX, c);
				neoPixel.setPixelColor(mod(r2 + leftJump, MAX) + MAX, c);
				neoPixel.setPixelColor(mod(ro1 + leftJump, MAX) + MAX, c);
				neoPixel.setPixelColor(mod(ro2 + leftJump, MAX) + MAX, c);
			}
			neoPixel.show();
		}
		updateCounters();
		nextTime = millis() + delayTime;
	}
}

void MycroftEyes::updateCounters() {
	if (currentAnim == NARROW || currentAnim == LOOK) {
		i++;
		Serial.println("i:");
		Serial.println(i);
		if(i >= steps) {
			if(currentAnim == NARROW) {
				currentState = NARROWED;
			}
			else {
				currentState = LOOKING;
			}
			currentAnim = NONE;
		}
	}
	else if (currentAnim == BLINK) {
		if (i == steps - 1) {
			c = color;
		}
		if (c != color) {
			i++;
		} else {
			i = j - 2;
		}
		j--;
		if (j <= 0) {
			currentState = OPEN;
			currentAnim = NONE;
		}
	}
}

void MycroftEyes::setSide(const char side) {
	if (side == 'l') {
		currentSide = LEFT;
	} else if (side == 'r') {
		currentSide = RIGHT;
	} else if (side == 'b') {
		currentSide = BOTH;
	} else if (side == 'u') {
		currentSide = UP;
	} else if (side == 'd') {
		currentSide = DOWN;
	} else if (side == 'c') {
		currentSide = CROSS;
	}
}

void MycroftEyes::setVars() {
	if(currentAnim == LOOK && currentSide == CROSS) {
		leftJump = 6;
	}
	else {
		leftJump = 0;
	}
	if(currentAnim == BLINK || currentAnim == NARROW) {
		pos = 0;
	}
	else if (currentAnim == LOOK) {
		if (currentSide == LEFT || currentSide == CROSS) {
			pos = 3;
		}
		else if (currentSide == DOWN) {
			pos = 0;
		}
		else if (currentSide == UP) {
			pos = 6;
		}
		else if (currentSide == RIGHT) {
			pos = 9;
		}
	}
	MAX = neoPixel.numPixels() / 2;
	leftJump = MAX + leftJump;
	if(currentAnim == BLINK || currentAnim == NARROW) {
		opJump = MAX / 2;
		c = 0;
	}
	if (currentAnim == BLINK) {
		steps = MAX / 4;
		delayTime = 35;
	}
	else if (currentAnim == LOOK) {
		steps = (MAX / 4) + 2;
		delayTime = 70;
	}
	else if (currentAnim == NARROW) {
		steps = (MAX / 4) + 1;
		delayTime = 140;
	}
}

void MycroftEyes::resetCounters() {
    nextTime = 0;
	if (currentAnim == LOOK) {
		i = 0;
	}
	else if (currentAnim == BLINK) {
		i = 0;
		j = steps*2;
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
