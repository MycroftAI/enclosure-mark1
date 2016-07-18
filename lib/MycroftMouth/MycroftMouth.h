#ifndef MYCROFT_MOUTH_H
#define MYCROFT_MOUTH_H

#include "MycroftHT1632.h"
#include <Arduino.h>
#include "MouthImages.h"
#include "../HT1632/font_5x4.h"
#include "font_8x4.h"

class MycroftMouth {
public:
	MycroftHT1632 ht1632;

	MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates);

	MycroftMouth();

	template <size_t y>
	void drawImage(int8_t pos, int8_t index, const char(&imgs)[y][16]) {
		readBuffer(index, imgs);
		ht1632.drawImage(buffer, width, height, pos, 0);
	}

	void staticText(String text, int8_t pos, int8_t fontIndex);

	void reset();

	void drawAnimation();

	void talk();

	void think();

	void listen();

	void smile();

	void write(const char *value);

private:

	enum State {
		NONE, TALK, LISTEN, THINK, SMILE, TEXT
	};

	byte i, total, size;

	char text[64];

	char width, height;

	char buffer[16];

	int textWd, textIdx, plates;

	unsigned long nextTime;

	boolean back;

	State state, lastState;

	void updateText();

	void copyText(const char *value);

	void resetCounters(State state);

	void drawFrame(byte i, State anim);

	    template <size_t x>
    void readBuffer(byte idx, const char(&anim)[x][16]) {
        byte size = sizeof(buffer);
        for (byte j = 0; j < size; j++) {
            buffer[j] = (char) pgm_read_byte(&(anim[idx][j]));
        }
   }

   void readBufferState(byte idx, State anim);

};

#endif /* MYCROFT_MOUTH_H */
