#pragma once

#include <Arduino.h>
#include "MycroftHT1632.h"

class MycroftMouth {
public:
	static MycroftMouth& instance() { return *m_instance; }

	enum State {
		NONE, TALK, LISTEN, THINK, TEXT, VISEME, ICON
	};

	MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates);
	template <size_t y>
	void drawImage(int8_t pos, int8_t index, const char(&imgs)[y][16]) {
		readBuffer(index, imgs);
		setPanel(pos, buffer);
	}
	void setPanel(int8_t pos, const char (&IMG)[16]);
	void render();
	void staticText(const String& text, int8_t pos, int8_t fontIndex);
	void reset();
	void update();
	void talk();
	void fakeTalk();	// for TTS with no Viseme support
	void think();
	void listen();
	void viseme(const String& str);
	void showIcon(const char strIcon[61]);
	void write(const char *value);
	State getState() const { return state; }

	MycroftHT1632 ht1632;

private:
        static MycroftMouth* 	m_instance;
	State			state;
	byte 			i, total;
	String 			textBuf;
	char 			buffer[16];
	int 			textWd, textIdx, plates;
	unsigned long 		nextTime;
	State 			lastState;

	void updateText();
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
