#pragma once

#include "MycroftHT1632.h"

#include "font_5x4.h"
#include "font_8x4.h"

class MycroftDisplay {
public:
	MycroftDisplay(int pinCS1, int pinWR, int pinDATA);
	void drawFrame(const char (&IMG)[4][16]);
	void drawFramePgm(byte index, const char (*IMG)[16]);
	void drawIconPgm(byte pos, byte index, const char (*ICONS)[16]);
	void drawText(String text, int8_t pos, bool small);
	void slideText();
	void clear();
	void render();

private:
	void draw8x8Pgm(byte pos, byte index, const char (*IMG)[16]);
	void readBuffer(byte idx, const char (*IMG)[16]);

	MycroftHT1632 ht1632;
	char currentImage[16];
};
