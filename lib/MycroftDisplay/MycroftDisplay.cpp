#include "MycroftDisplay.h"

MycroftDisplay::MycroftDisplay(int pinCS1, int pinWR, int pinDATA) {
	ht1632.begin(pinCS1, pinWR, pinDATA);
	ht1632.clear();
	ht1632.render();
}

void MycroftDisplay::drawFrame(const char (&IMG)[4][16]) {
	for (byte panel = 0; panel < 4; ++panel) {
		ht1632.drawImage(IMG[panel], 8, 8, 8 * panel, 0);
	}
	render();
}

void MycroftDisplay::drawFramePgm(byte index, const char (*IMG)[16]) {
	index *= 4;
	for (byte panel = 0; panel < 4; ++panel) {
		draw8x8Pgm(8 * panel, index + panel, IMG);
	}
	render();
}

void MycroftDisplay::drawIconPgm(byte pos, byte index, const char (*ICONS)[16]) {
	draw8x8Pgm(pos, index, ICONS);
}

void MycroftDisplay::drawText(String text, int8_t pos, bool small) {
	if (small) {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
	} else {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_8X4, FONT_8X4_WIDTH, FONT_8X4_HEIGHT, FONT_8X4_STEP_GLYPH);
	}
}

void MycroftDisplay::slideText() {
	ht1632.transition(TRANSITION_BUFFER_SWAP);
}

void MycroftDisplay::clear() {
	ht1632.clear();
}

void MycroftDisplay::render() {
	ht1632.render();
}

void MycroftDisplay::draw8x8Pgm(byte pos, byte index, const char (*IMG)[16]) {
	readBuffer(index, IMG);
	ht1632.drawImage(currentImage, 8, 8, pos, 0);
}

void MycroftDisplay::readBuffer(byte idx, const char (*IMG)[16]) {
	byte size = sizeof (currentImage);
	for (byte j = 0; j < size; j++) {
		currentImage[j] = (char) pgm_read_byte(&(IMG[idx][j]));
	}
}
