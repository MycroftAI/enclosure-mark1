#pragma once

#include "Adafruit_NeoPixel.h"

class MycroftEyes {
public:
	Adafruit_NeoPixel neoPixel;

	MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);

	enum Animation {
		BLINK, NARROW, LOOK, NONE
	};
	
	void setup();

	void on();

	void off();

	void startAnim(Animation anim, const char side);

	void updateAnimation();

	void updateColor(uint8_t r, uint8_t g, uint8_t b);

	void updateBrightness(uint8_t level);


private:
	uint32_t color;

	unsigned long nextTime;

	enum Side {
		BOTH, LEFT, RIGHT, UP, DOWN, CROSS
	};

	Side currentSide;

	enum State {
		OPEN, LOOKING, CLOSED
	};

	Animation currentAnim;

	boolean firstFrame;

	byte max, pos, opJump, steps, leftJump, delayTime, i, j;

	uint32_t c;

	void set(uint32_t color);

	void animSetup(Animation anim, const char side);

	void runAnim();

	void updateCounters();

	void setSide(const char side);

	void setVars();

	void resetCounters();

	uint16_t mod(long a, long b);
};
