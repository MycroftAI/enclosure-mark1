#pragma once

#include "Adafruit_NeoPixel.h"
#include "EyeFrames.h"

class MycroftEyes {
public:
	Adafruit_NeoPixel neoPixel;

	MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);

	enum Animation {
		BLINK, NARROW, LOOK, UNLOOK, WIDEN, NONE
	};

	void setup();

	void on();

	void off();

	void startAnim(Animation anim, const char side);

	void updateAnimation();

	void updateColor(uint8_t r, uint8_t g, uint8_t b);

	void updateBrightness(uint8_t level);


private:
	uint32_t color, c;

	uint16_t r1, r2, ro1, ro2;

	unsigned long nextTime;

	enum Side {
		BOTH, LEFT, RIGHT, UP, DOWN, CROSS
	};

	Side currentSide;

	enum State {
		OPEN, LOOKING, NARROWED, CLOSED, ANIMATING
	};

	State currentState;

	Animation currentAnim, queuedAnim;

	char queuedSide, lookSide;

	boolean isQueued;

	byte MAX, pos, opJump, steps, leftJump, i, j, update;

	unsigned long delayTime;

	void set(uint32_t color);

	void animSetup(Animation anim, const char side);

	void runAnim();

	void startTransition(Animation transition, Animation anim, const char side);

	void updateCounters();

	void setSide(const char side);

	void setVars();

	void resetCounters();

	void eyesNarrow(uint32_t c, int wait);

	uint16_t mod(long a, long b);
};
