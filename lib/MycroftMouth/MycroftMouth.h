#pragma once

#include <Arduino.h>
#include "MycroftHT1632.h"
#include "MycroftComponent.h"
#include "MycroftDisplay.h"

class MycroftMouth : public MycroftComponent {
public:
	MycroftDisplay display;

	MycroftMouth(int pinCS1, int pinWR, int pinDATA);

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

	char buffer[16];

	int textWd, textIdx;

	unsigned long nextTime;

	boolean back;

	State state, lastState;

	void updateText();

	void copyText(const char *value);

	void resetCounters(State state);
};
