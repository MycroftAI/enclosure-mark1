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

#include "MycroftHT1632.h"

#include "font_5x4.h"
#include "font_8x4.h"

class MycroftDisplay {
public:
	static const byte PANEL_SX = 8, PANEL_SY = 8;
	static const byte NUM_PANELS = 4;
	static const byte SX = NUM_PANELS * PANEL_SX, SY = 1 * PANEL_SY;
	MycroftDisplay(int pinCS1, int pinWR, int pinDATA);
	void drawFrame(const char (&IMG)[4][16]);
	void drawFramePgm(byte index, const char (*IMG)[16]);
	void drawIconPgm(byte pos, byte index, const char (*ICONS)[16]);
	void drawTextCentered(const String& text, bool small);
	void drawText(const String& text, int8_t pos, bool small);
	void setBrightness(int iLevel);	// 1-15
	void slideText();
	void clear();
	void render();

private:
	void drawPanelPgm(byte pos, byte index, const char (*IMG)[16]);
	void readBuffer(byte idx, const char (*IMG)[16]);

	MycroftHT1632 ht1632;
	char currentImage[16];
};
