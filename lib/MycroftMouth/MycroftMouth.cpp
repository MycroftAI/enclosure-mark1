#include "MycroftMouth.h"
#include "MouthImages.h"

MycroftMouth::MycroftMouth(int pinCS1, int pinWR, int pinDATA) :
display(pinCS1, pinWR, pinDATA) {
	reset();
	lastState = state = NONE;
}

void MycroftMouth::staticText(String text, int8_t pos, int8_t fontIndex){
	display.drawText(text, pos, fontIndex == 0);
}

void MycroftMouth::setPanel(int8_t pos, const char (&IMG)[16]) {
	display.drawIconPgm(pos, 0, &IMG);
}

void MycroftMouth::render() {
	display.render();
}

void MycroftMouth::reset() {
	state = NONE;
	textWd = 0;
	textIdx = 0;
	display.clear();
	display.render();
}

void MycroftMouth::update() {
	switch (state) {
		case TALK:
			this->talk();
			break;
		case LISTEN:
			this->listen();
			break;
		case THINK:
			this->think();
			break;
		case SMILE:
			this->smile();
			break;
		case TEXT:
			this->updateText();
			break;
		default:
			if (lastState != NONE) {
				this->reset();
			}
	}
	lastState = state;
}

void MycroftMouth::talk() {
	if (state != TALK) {
		resetCounters(TALK);
		size = 4;
		total = (size * 2) - 2;
	}
	if (millis() > nextTime) {
		display.drawFramePgm(i, TALK_ANIMATION);
		display.render();
		if (i < size - 1) {
			i++;
		} else {
			i--;
		}
		nextTime = millis() + 70;
		total--;
	}
	if (total == 0) {
		resetCounters(TALK);
		total = (size * 2) - 2;
	}
}

void MycroftMouth::listen() {
	size = 6;
	if (state != LISTEN) {
		resetCounters(LISTEN);
	}
	if (millis() > nextTime) {
		display.drawFramePgm(i, LISTEN_ANIMATION);
		display.render();
		if (i < (size - 1)) {
			i++;
		} else {
			i = 0;
		}
		nextTime = millis() + 70;
	}
}

void MycroftMouth::think() {
	if (state != THINK) {
		resetCounters(THINK);
		size = 11;
		total = (size * 2) - 1;
	}
	if (millis() > nextTime) {
		display.drawFramePgm(i, THINK_ANIMATION);
		display.render();
		i++;
		if (i == size) {
			i = 0;
		}
		nextTime = millis() + 120;
		total--;
	}
	if (total == 0) {
		resetCounters(THINK);
		total = (size*2) -1;
	}
}

void MycroftMouth::smile() {
	state = SMILE;
	display.drawFramePgm(0, SMILE_IMAGE);
	display.render();
}

void MycroftMouth::write(const char *value) {
	state = TEXT;
	copyText(value);
	textWd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
	textIdx = 0;
	resetCounters(TEXT);
	this->updateText();
}

void MycroftMouth::copyText(const char *value) {
	for (byte i = 0; i < sizeof (text); i++) {
		text[i] = ' ';
	}
	strcpy(text, value);
	Serial.println(text);
}

void MycroftMouth::updateText() {
	if (millis() > nextTime) {
		display.slideText();
		display.clear();
		display.drawText(text, OUT_SIZE - textIdx, true);
		display.render();
		textIdx = (textIdx + 1) % (textWd + OUT_SIZE);
		nextTime = millis() + 150;
	}
}

void MycroftMouth::resetCounters(State anim) {
	state = anim;
	i = 0;
	nextTime = 0;
}
