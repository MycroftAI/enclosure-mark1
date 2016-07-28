#pragma once

#include <Arduino.h>
#include "MycroftHT1632.h"
#include "MycroftComponent.h"

class MycroftMouth : public MycroftComponent {
public:
	MycroftHT1632 ht1632;

	MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates);

	MycroftMouth();

	template <size_t y>
	void drawImage(int8_t pos, int8_t index, const char(&imgs)[y][16]) {
		readBuffer(index, imgs);
		setPanel(pos, buffer);
	}
	
	void setPanel(int8_t pos, const char (&IMG)[16]);

	void render();

	void staticText(String text, int8_t pos, int8_t fontIndex);

	void reset();
	
	void update();

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
