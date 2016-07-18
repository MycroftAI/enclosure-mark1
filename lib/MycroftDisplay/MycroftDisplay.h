#pragma once

#include "MycroftHT1632.h"

#include "font_5x4.h"
#include "font_8x4.h"

class MycroftDisplay {
public:
	MycroftDisplay(int pinCS1, int pinWR, int pinDATA);
	void drawFrame(byte index, const char (*IMG)[16]);
	void drawIcon(byte pos, byte index, const char (*ICONS)[16]);
	void drawText(String text, int8_t pos, bool small);
	void clear();
	void render();

private:
	void draw8x8(byte pos, byte index, const char (*IMG)[16]);
	void readBuffer(byte idx, const char (*IMG)[16]);

	MycroftHT1632 ht1632;
	char currentImage[16];
};
