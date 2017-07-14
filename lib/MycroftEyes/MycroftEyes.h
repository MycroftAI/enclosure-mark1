/**                                                                         
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
**/
#pragma once

#include "Adafruit_NeoPixel.h"

#define MAX_EYE_BRIGHTNESS	30
#define MIN_EYE_BRIGHTNESS	1

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
	void setPixel(uint8_t pixel, uint32_t color);
	void setEyePixels(Side side, uint8_t pixels);
	void updateColor(uint8_t r, uint8_t g, uint8_t b);
	bool incrementBrightness(bool up);
	void updateBrightness(uint8_t level);
	uint8_t getBrightness() const
		{ return bright; }
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
		OPEN, LOOKING, NARROWED, ANIMATING, CUSTOM
	};
	State currentState;
	Animation queuedAnim;
	bool isQueued, back;
	const byte MAX;
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
