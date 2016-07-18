#include "MycroftMouth.h"

MycroftMouth::MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates) {
	ht1632 = MycroftHT1632();
	ht1632.begin(pinCS1, pinWR, pinDATA);
	reset();
	this->plates = plates;
	lastState = state = NONE;
}

MycroftMouth::MycroftMouth() { }

void MycroftMouth::staticText(String text, int8_t pos, int8_t fontIndex) {
	if (fontIndex == 0) {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
	} else if (fontIndex == 1) {
		ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_8X4, FONT_8X4_WIDTH, FONT_8X4_HEIGHT, FONT_8X4_STEP_GLYPH);
	}
}

void MycroftMouth::reset() {
	state = NONE;
	width = 8;
	height = 8;
	textWd = 0;
	textIdx = 0;
	ht1632.clear();
	ht1632.render();
}

void MycroftMouth::drawAnimation() {
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
		drawFrame(i, state);
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
	}
}

void MycroftMouth::listen() {
	size = 6;
	if (state != LISTEN) {
		resetCounters(LISTEN);
	}
	if (millis() > nextTime) {
		drawFrame(i, state);
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
		size = 8;
		total = (size * 2) - 1;
	}
	if (millis() > nextTime) {
		drawFrame(i, state);
		if (i < (size - 1) && !back) {
			i++;
		} else {
			back = true;
			i--;
		}
		nextTime = millis() + 200;
		total--;
	}
	if (total == 0) {
		resetCounters(THINK);
	}
}

void MycroftMouth::drawFrame(byte i, State anim) {
	ht1632.clear();
	for (byte j = 0; j < this->plates; j++) {
		byte idx = (i * this->plates) + j;
		byte x = j * 8;
		readBufferState(idx, anim);
		ht1632.drawImage(buffer, width, height, x, 0);
	}
	ht1632.render();
}

void MycroftMouth::readBufferState(byte idx, State anim){
	if (anim == THINK){
		this->readBuffer(idx, THINK_ANIMATION);
	}
	else if (anim == LISTEN){
		this->readBuffer(idx, LISTEN_ANIMATION);
	}
	else if (anim == TALK){
		this->readBuffer(idx, TALK_ANIMATION);
	}
}

void MycroftMouth::smile() {
	state = SMILE;
	for (byte j = 0; j < this->plates; j++) {
		byte idx = j;
		byte x = j * 8;
		this->readBuffer(idx, SMILE_IMAGE);
		ht1632.drawImage(buffer, width, height, x, 0);
	}
	ht1632.render();
	delay(70);
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
		ht1632.transition(TRANSITION_BUFFER_SWAP);
		ht1632.clear();
		ht1632.drawTextPgm(text, OUT_SIZE - textIdx, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
		ht1632.render();
		textIdx = (textIdx + 1) % (textWd + OUT_SIZE);
		nextTime = millis() + 150;
	}
}

void MycroftMouth::resetCounters(State anim) {
	state = anim;
	back = false;
	i = 0;
	nextTime = 0;
}
