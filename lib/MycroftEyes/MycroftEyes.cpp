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
		case WIDEN:
			runAnim();
			break;
		case UNLOOK:
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
	currentAnim = NONE;
	MAX = neoPixel.numPixels() / 2;
	this->on();
}

void MycroftEyes::set(Side side, uint32_t color) {
	const byte NUM_PIX = neoPixel.numPixels();
	const byte BEGIN = rightOn(side) ? 0 : NUM_PIX/2;
	const byte END = leftOn(side) ? NUM_PIX : NUM_PIX/2;

	for (uint16_t i = 0; i < BEGIN; i++) {
		neoPixel.setPixelColor(i, 0);
	}
	for (uint16_t i = BEGIN; i < END; i++) {
		neoPixel.setPixelColor(i, color);
	}
	for (uint16_t i = END; i < NUM_PIX; i++) {
		neoPixel.setPixelColor(i, 0);
	}
	neoPixel.show();
}

void MycroftEyes::set(uint32_t color) {
	set(BOTH, color);
}

void MycroftEyes::on() {
	this->set(color);
}

void MycroftEyes::off() {
	this->set(0);
}

void MycroftEyes::startTransition(Animation transition, Animation anim, Side side){
	queuedAnim = anim;
	queuedSide = side;
	isQueued = true;
	if(transition == WIDEN){
	    startAnim(WIDEN, BOTH);
    }
	else if (transition == UNLOOK){
		startAnim(UNLOOK, lookSide);
	}
}

void MycroftEyes::startAnim(Animation anim, Side side) {
	if (currentState == NARROWED && anim != WIDEN){
		startTransition(WIDEN,anim, side);
		return;
	}
	if (currentState == LOOKING && anim != UNLOOK){
		startTransition(UNLOOK, anim, side);
		return;
	}
	animSetup(anim, side);
	runAnim();
}

void MycroftEyes::animSetup(Animation anim, Side side) {
	currentAnim = anim;
	if(currentAnim == LOOK){
		lookSide = side;
	}
	currentSide = side;
	if (currentAnim != WIDEN && currentAnim != UNLOOK){
	    this->on();
    }
	currentState = ANIMATING;
	setVars();
	resetCounters();
}
bool MycroftEyes::leftOn(Side side) {
	return side == BOTH || side == LEFT;
}

bool MycroftEyes::rightOn(Side side) {
	return side == BOTH || side == RIGHT;
}

void MycroftEyes::runAnim() {
	if (millis() >= nextTime) {
		switch(currentAnim){
		case LOOK:
			r1 = mod(pos + i, MAX);
			r2 = mod(pos - 1 - i, MAX);
			neoPixel.setPixelColor(r1, 0);
			neoPixel.setPixelColor(r2, 0);
			neoPixel.setPixelColor(mod(r1 + leftJump, MAX) + MAX, 0);
			neoPixel.setPixelColor(mod(r2 + leftJump, MAX) + MAX, 0);
		    break;
		case UNLOOK:
			r1 = mod(pos + i, MAX);
			r2 = mod(pos - 1 - i, MAX);
			neoPixel.setPixelColor(r1, color);
			neoPixel.setPixelColor(r2, color);
			neoPixel.setPixelColor(mod(r1 + leftJump, MAX) + MAX, color);
			neoPixel.setPixelColor(mod(r2 + leftJump, MAX) + MAX, color);
		    break;
		case BLINK:
			r1 = mod(pos + i, MAX);
			r2 = mod(pos - 1 - i, MAX);
			ro1 = mod(r1 + opJump, MAX);
			ro2 = mod(r2 + opJump, MAX);
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
		    break;
		case NARROW:
		    for (byte index = 0; index < 4; index++){
		 	    if(currentSide == LEFT || currentSide == BOTH){
					neoPixel.setPixelColor(neoPixelSection[i+1][index], 0);
				}
		        if (currentSide == RIGHT || currentSide == BOTH){
					neoPixel.setPixelColor(neoPixelSection[i][index], 0);
			    }
		    }
		    break;
		case WIDEN:
		    for (byte index = 0; index < 4; index++){
		 	    if(currentSide == LEFT || currentSide == BOTH){
					neoPixel.setPixelColor(neoPixelSection[i+1][index], color);
				}
		        if (currentSide == RIGHT || currentSide == BOTH){
					neoPixel.setPixelColor(neoPixelSection[i][index], color);
			    }
		    }
		    break;

		}
		neoPixel.show();
		updateCounters();
		nextTime = millis() + delayTime;
	}
}

void MycroftEyes::updateCounters() {
	switch(currentAnim){
	case LOOK:
		i+= update;
		if(i >= steps) {
			currentState = LOOKING;
			currentAnim = NONE;
			if(isQueued){
				isQueued = false;
				startAnim(queuedAnim, queuedSide);
			}
		}
		break;
	case NARROW:
		i+= update;
		if (i > steps){
			currentState = NARROWED;
			currentAnim = NONE;
			if(isQueued){
				isQueued = false;
				startAnim(queuedAnim, queuedSide);
			}
		}
		break;
	case UNLOOK:
		if (i <= steps){
			currentState = OPEN;
			currentAnim = NONE;
			if(isQueued){
				isQueued = false;
				startAnim(queuedAnim, queuedSide);
				return;
			}
		}
		i--;
		break;
	case WIDEN:
		if (i <= steps){
			currentState = OPEN;
			currentAnim = NONE;
			if(isQueued){
				isQueued = false;
				startAnim(queuedAnim, queuedSide);
				return;
			}
		}
		i -= update;
		break;
	case BLINK:
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
			if(isQueued){
				isQueued = false;
				startAnim(queuedAnim, queuedSide);
			}
		}
	break;
	}
}

void MycroftEyes::setVars() {
	if(currentAnim == LOOK || currentAnim == BLINK || currentAnim == UNLOOK){
		update = 1;
	}
	else if (currentAnim == NARROW || currentAnim == WIDEN){
		update = 2;
	}
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
	leftJump = MAX + leftJump;
	switch(currentAnim){
	case BLINK:
		opJump = MAX / 2;
		c = 0;
		steps = MAX / 4;
		delayTime = 35;
		break;
	case LOOK:
		steps = (MAX / 4) + 2;
		delayTime = 70;
	    break;
	case UNLOOK:
		steps = 0;
		delayTime = 70;
	    break;
	case NARROW:
		steps = 2;
		delayTime = 140;
		break;
	case WIDEN:
		steps = 0;
		delayTime = 140;
		break;
	}
}

void MycroftEyes::resetCounters() {
    nextTime = 0;
	switch(currentAnim){
	case LOOK:
		i = 0;
		break;
	case NARROW:
		i = 0;
		break;
	case UNLOOK:
		i = (MAX/4) + 2;
	    break;
	case WIDEN:
		i = 2;
		break;
	case BLINK:
		i = 0;
		j = steps*2;
	    break;
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
