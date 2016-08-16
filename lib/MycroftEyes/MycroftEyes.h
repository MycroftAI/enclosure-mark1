#pragma once

#include "Adafruit_NeoPixel.h"
#include "EyeFrames.h"

class MycroftEyes {
public:
	enum Side {
		BOTH, LEFT, RIGHT, UP, DOWN, CROSS
	};

	Adafruit_NeoPixel neoPixel;

	MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);

	enum Animation {
		BLINK, NARROW, LOOK, UNLOOK, WIDEN, VOLUME, NONE
	};

	void setup();

	void on();

	void off();

	void startAnim(Animation anim, const char side);

	void setEyePixels(const char side, uint8_t pixels);

	void updateAnimation();

	void updateColor(uint8_t r, uint8_t g, uint8_t b);

	void updateBrightness(uint8_t level);

	void set(Side side, uint32_t color);

	void set(uint32_t color);

private:
	uint32_t color, c;

	uint16_t r1, r2, ro1, ro2;

	uint8_t volumePix;

	unsigned long nextTime;

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

	static bool leftOn(Side side);

	static bool rightOn(Side side);

	void setEyePixels(uint8_t pixels);

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
