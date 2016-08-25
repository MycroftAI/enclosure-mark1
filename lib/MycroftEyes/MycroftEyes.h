#pragma once

#include "Adafruit_NeoPixel.h"
#include "HT1632.h"

class MycroftEyes {
public:
	static MycroftEyes& instance() { return *m_instance; }

	uint8_t bright;
	enum Side {
		BOTH, LEFT, RIGHT, UP, DOWN, CROSS
	};
	Adafruit_NeoPixel neoPixel;
	MycroftEyes(uint16_t length, uint8_t pin, neoPixelType type);
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
	bool incrementBrightness(bool up);
	void updateBrightness(uint8_t level);
	uint8_t getBrightness();
	void set(Side side, uint32_t color);
	void set(uint32_t color);
	void fill(uint8_t pixel);

private:
	static MycroftEyes* m_instance;

	uint8_t r1, r2;
	uint8_t delayTime;
	uint32_t color, c;
	unsigned long nextTime, endTime;
	Side currentSide, queuedSide, lookSide;
	enum State {
		OPEN, LOOKING, NARROWED, ANIMATING
	};
	State currentState;
	Animation queuedAnim;
	bool isQueued, back;
	const byte MAX, MAX_BRIGHTNESS, MIN_BRIGHTNESS;
	char pos, narrowPos, lastPos, initialPos, endPos, startPos, leftJump;
	static bool leftOn(Side side);
	static bool rightOn(Side side);
	void animSetup(Animation anim, Side side);
	void runAnim();
	void endAnim(State endState);
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
	void setDelayTime(uint8_t delay);
	void checkQueued();
	uint16_t mod(long a, long b);
};
