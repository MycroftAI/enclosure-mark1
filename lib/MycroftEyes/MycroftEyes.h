#pragma once

#include "Adafruit_NeoPixel.h"

class MycroftEyes {
public:
	Adafruit_NeoPixel neoPixel;

	MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);

	void setup();

	void on();

	void off();

	void updateAnimation();

	void blink(const char side);

	void narrow(const char side);

	void look(const char side);

	void updateColor(uint8_t r, uint8_t g, uint8_t b);

	void updateBrightness(uint8_t level);

private:
	uint32_t color;

	unsigned long nextTime;

	enum Side {
		BOTH, LEFT, RIGHT
	};

	enum Animation {
		BLINK, NARROW, LOOK, NONE
	};

	Animation anim;

	void set(uint32_t color);

	void blink(Side side, byte pos, byte leftJump);

	void narrow(Side side, byte pos, byte leftJump);

	void look(byte pos, byte leftJump);

	uint16_t mod(long a, long b);
};
