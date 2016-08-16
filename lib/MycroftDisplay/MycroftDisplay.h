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
	void drawText(String text, int8_t pos, bool small);
	void slideText();
	void clear();
	void render();

private:
	void drawPanelPgm(byte pos, byte index, const char (*IMG)[16]);
	void readBuffer(byte idx, const char (*IMG)[16]);

	MycroftHT1632 ht1632;
	char currentImage[16];
};
