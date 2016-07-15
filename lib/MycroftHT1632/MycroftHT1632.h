#pragma once

#include "HT1632.h"
#include <avr/pgmspace.h>
#include <Arduino.h>

class MycroftHT1632 : public HT1632Class {
public:
	void drawImagePgm(const char * img, char width, char height, char x, char y, int offset = 0);
	void drawTextPgm(const char [], int x, int y, const char font [], const char font_width [], char font_height, int font_glyph_step, char gutter_space = 1);
};
