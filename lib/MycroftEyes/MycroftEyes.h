#pragma once

#include "Adafruit_NeoPixel.h"

class MycroftEyes {
public:

	enum Side {
		BOTH, LEFT, RIGHT, UP, DOWN, CROSS
    };
	Adafruit_NeoPixel neoPixel;
	MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);
	enum Animation {
		BLINK, NARROW, LOOK, UNLOOK, WIDEN, VOLUME, SPIN, TIMEDSPIN, REFILL, NONE
	};
	Animation currentAnim;
	void setup();
	void on();
	void reset();
	void off();
	void startAnim(Animation anim, Side side);
	void updateAnimation();
	void timedSpin(int length);
	void setEyePixels(Side side, uint8_t pixels);
	void updateColor(uint8_t r, uint8_t g, uint8_t b);
	void incrementBrightness(bool up);
	void updateBrightness(uint8_t level);
	void set(Side side, uint32_t color);
	void set(uint32_t color);
	void fill(uint8_t pixel);

private:
	uint8_t brightness;
	uint32_t color, c;
	uint8_t volumePix;
	int r1, r2, ro1, ro2;
	unsigned long nextTime, endTime;
	Side currentSide, queuedSide, lookSide;
	enum State {
		OPEN, LOOKING, NARROWED, CLOSED, ANIMATING
	};
	State currentState;
	Animation queuedAnim;
	boolean isQueued, back;
	const byte MAX;
	const uint8_t MAX_BRIGHTNESS, MIN_BRIGHTNESS;
	char pos, narrowPos, lastPos, initialPos, endPos, startPos, leftJump;
	unsigned long delayTime;
	static bool leftOn(Side side);
	static bool rightOn(Side side);
	void animSetup(Animation anim, Side side);
	void runAnim();
	void renderNarrow(bool widen);
	void renderLook(bool unlook);
	void renderSpin();
	void renderRefill();
	void setEyeNarrow(char position, byte offset);
	void setEyePixels(uint8_t pixels);
	void insertTransition(Animation transition, Animation anim, Side side);
	void updateCounters();
	void updateLook(bool unlook);
	void updateBlink();
	void updateNarrow();
	void updateWiden();
	void updateSpin();
	void updateRefill();
	void resetVars();
	void setLookVars(Side side, bool unlook);
	void checkQueued();
	uint16_t mod(long a, long b);
};
