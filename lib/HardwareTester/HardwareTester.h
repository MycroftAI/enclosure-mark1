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
	void testAnimation(MycroftMouth &mouth);

	static const char WHITE_PANEL[16];
	uint32_t color;
	MycroftEyes::Side side;
};
