#pragma once

#include <Arduino.h>
#include "MycroftEyes.h"

class MycroftEncoder;
class MycroftMouth;
class MycroftArduino;

class HardwareTester {
public:
	HardwareTester();
	void run(MycroftEncoder &encoder, MycroftEyes &eyes, MycroftMouth &mouth, MycroftArduino &arduino);

private:
	void drawWhiteScreen(MycroftMouth &mouth);
	void setEyeSide(MycroftEyes &eyes, MycroftEyes::Side side);
	void setEyes(MycroftEyes &eyes, byte r, byte g, byte b);
	void pause(MycroftEncoder &encoder);
	void testKnob(MycroftEncoder &encoder, MycroftEyes &eyes);
	void testMute(MycroftArduino &arduino);

	static const char WHITE_PANEL[16];
	uint32_t color;
	MycroftEyes::Side side;
};
