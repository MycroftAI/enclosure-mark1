#include "MycroftEyes.h"

MycroftEyes::MycroftEyes(uint16_t length, uint8_t pin, neoPixelType type) :
neoPixel(length, pin, type), MAX(neoPixel.numPixels()/2) { }

void MycroftEyes::updateAnimation() {
	switch (currentAnim) {
	case NONE:
		if (currentState == OPEN) {
			this->on();
		}
	default:
		runAnim();
	}
}

void MycroftEyes::setup() {
	neoPixel.begin();
	color = neoPixel.Color(255, 255, 255);
	neoPixel.setBrightness(30);
	currentAnim = NONE;
	this->on();
}

void MycroftEyes::set(Side side, uint32_t color) {
	const byte NUM_PIX = neoPixel.numPixels();
	const byte BEGIN = rightOn(side) ? 0 : NUM_PIX/2;
	const byte END = leftOn(side) ? NUM_PIX : NUM_PIX/2;

	for (uint16_t i = 0; i < BEGIN; i++) {
		neoPixel.setPixelColor(i, 0);
		neoPixel.show();
	}
	for (uint16_t i = BEGIN; i < END; i++) {
		neoPixel.setPixelColor(i, color);
		neoPixel.show();
	}
	for (uint16_t i = END; i < NUM_PIX; i++) {
		neoPixel.setPixelColor(i, 0);
		neoPixel.show();
	}
}

void MycroftEyes::set(uint32_t color) {
	set(BOTH, color);
}

void MycroftEyes::fill(uint8_t pixel) {
	if(pixel == (neoPixel.numPixels() - 1)) {
		this->on();
	}
	else {
		for (uint16_t i = 0; i <= pixel; i++) {
			neoPixel.setPixelColor(i, color);
			neoPixel.show();
		}
	}
}

void MycroftEyes::on() {
	this->set(color);
	currentState = OPEN;
}

void MycroftEyes::off() {
	Serial.println("off");
	currentState = CLOSED;
	this->set(0);
}

bool MycroftEyes::leftOn(Side side) {
	return side == BOTH || side == LEFT;
}

bool MycroftEyes::rightOn(Side side) {
	return side == BOTH || side == RIGHT;
}

void MycroftEyes::insertTransition(Animation transition, Animation anim, Side side) {
	queuedAnim = anim;
	queuedSide = side;
	isQueued = true;
	if(transition == WIDEN) {
		startAnim(WIDEN, BOTH);
	}
	else if (transition == UNLOOK) {
		startAnim(UNLOOK, lookSide);
	}
}

void MycroftEyes::startAnim(Animation anim, Side side) {
	if (currentState == NARROWED && anim != WIDEN) {
		insertTransition(WIDEN,anim, side);
		return;
	}
	if (currentState == LOOKING && anim != UNLOOK) {
		insertTransition(UNLOOK, anim, side);
		return;
	}
	animSetup(anim, side);
	runAnim();
}

void MycroftEyes::animSetup(Animation anim, Side side) {
	currentAnim = anim;
	if(currentAnim == LOOK) {
		lookSide = side;
	}
	currentSide = side;
	if (currentAnim != WIDEN && currentAnim != UNLOOK) {
		this->on();
	}
	currentState = ANIMATING;
	resetVars();
}

void MycroftEyes::runAnim() {
	if (millis() >= nextTime) {
		switch(currentAnim) {
		case LOOK:
			renderLook(false);
			break;
		case UNLOOK:
			renderLook(true);
			break;
		case BLINK:
			renderNarrow(back);
			break;
		case NARROW:
			renderNarrow(false);
			break;
		case WIDEN:
			renderNarrow(true);
			break;
		}
		neoPixel.show();
		updateCounters();
		nextTime = millis() + delayTime;
	}
}

void MycroftEyes::updateCounters() {
	switch(currentAnim) {
		case LOOK:
			updateLook(false);
			break;
		case UNLOOK:
			updateLook(true);
			break;
		case NARROW:
			updateNarrow();
			break;
		case WIDEN:
			updateWiden();
			break;
		case BLINK:
			updateBlink();
			break;
	}
}

void MycroftEyes::checkQueued() {
	if(isQueued) {
		isQueued = false;
		startAnim(queuedAnim, queuedSide);
	}
}

void MycroftEyes::updateLook(bool unlook) {
	pos = unlook ? pos -1 : pos + 1;
	int dir = unlook ? -1 : 1;
	Serial.println(dir);
	if (pos == endPos) {
		currentState = unlook ? OPEN : LOOKING;
		currentAnim = NONE;
		checkQueued();
		return;
	}
	r1 = (r1 + dir) <= MAX ? r1 + dir : 0;
	r2 = (r2 - dir) >= 0 ? r2 - dir : MAX;
}

void MycroftEyes::renderLook(bool unlook) {
	c = unlook ? color : 0;
	Serial.println(r1);
	Serial.println(r2);
	Serial.println(mod(r1+leftJump, MAX) + MAX);
	Serial.println(mod(r2+leftJump, MAX) + MAX);
	neoPixel.setPixelColor(r1, c);
	neoPixel.setPixelColor(r2, c);
	neoPixel.setPixelColor(mod(r1+leftJump, MAX) + MAX, c);
	neoPixel.setPixelColor(mod(r2 + leftJump, MAX) + MAX, c);
}

void MycroftEyes::updateNarrow() {
	pos++;
	Serial.println(pos);
	if(pos >= 2) {
		currentState = NARROWED;
		currentAnim = NONE;
		checkQueued();
		return;
	}
}


void MycroftEyes::updateWiden() {
	pos--;
	if(pos < 0) {
		currentState = OPEN;
		currentAnim = NONE;
		checkQueued();
		return;
	}
}

void MycroftEyes::updateBlink() {
	if(!back) {
		pos++;
		if(pos > 2) {
			back = true;
		}
	}
	else {
		pos--;
		if(pos <=0) {
			currentState = OPEN;
			currentAnim = NONE;
			checkQueued();
		}
	}
}

void MycroftEyes::renderNarrow(bool widen) {
	c = widen ? color : 0;
	if (currentSide == RIGHT || currentSide == BOTH) {
		setEyeNarrow(pos, 0);
	}
	if (currentSide == LEFT || currentSide == BOTH) {
		setEyeNarrow(pos, MAX);
	}
}

void MycroftEyes::setEyeNarrow(byte pos, byte offset) {
	neoPixel.setPixelColor(0   + pos + offset, c);
	neoPixel.setPixelColor(MAX - pos - 1 + offset, c);
	neoPixel.setPixelColor(MAX/2 + pos + offset, c);
	neoPixel.setPixelColor(MAX/2 - pos - 1 + offset, c);
}

void MycroftEyes::setLookVars(Side side, bool unlook) {
	pos = unlook ? 6 : 0;
	switch(side) {
		case UP:
			startPos = unlook ? 11 : 5;
			break;
		case DOWN:
			startPos = unlook ? 5 : 11;
			break;
		case LEFT:
			startPos = unlook ? 2 : 8;
			break;
		case RIGHT:
			startPos = unlook ? 8 : 2;
			break;
		case CROSS:
			startPos = unlook ? 8 : 2;
			break;
	}
	endPos = unlook ? 0 : 6;
	r1 = startPos;
	r2 = startPos + 1 > 11 ? 0 : startPos + 1;
}
void MycroftEyes::resetVars() {
	leftJump = 0;
	nextTime = 0;
	switch(currentAnim) {
		case BLINK:
			pos = 0;
			back = false;
			delayTime = 35;
			break;
		case LOOK:
			setLookVars(currentSide, false);
			if(currentSide == CROSS) {
				leftJump = 6;
			}
			delayTime = 70;
			break;
		case UNLOOK:
			setLookVars(currentSide, true);
			if(currentSide == CROSS) {
				leftJump = 6;
			}
			delayTime = 70;
			break;
		case NARROW:
			pos = 0;
			delayTime = 140;
			break;
		case WIDEN:
			pos = 2;
			delayTime = 140;
			break;
	}
	leftJump = MAX + leftJump;
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
