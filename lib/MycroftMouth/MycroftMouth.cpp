#include "MycroftMouth.h"
#include "MouthImages.h"
#include "font_5x4.h"
#include "font_8x4.h"

MycroftMouth::MycroftMouth(int pinCS1, int pinWR, int pinDATA, int plates) {
	ht1632 = MycroftHT1632();
	ht1632.begin(pinCS1, pinWR, pinDATA);
	reset();
	this->plates = plates;
	lastState = state = NONE;
}

MycroftMouth::MycroftMouth() { }

void MycroftMouth::setPanel(int8_t pos, const char (&IMG)[16]) {
	ht1632.drawImage(IMG, width, height, pos, 0);
}

void MycroftMouth::render() {
	ht1632.render();
}

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
		total = (size * 2) - 2;
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
		size = 11;
		total = (size * 2) - 1;
	}
	if (millis() > nextTime) {
		drawFrame(i, state);
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
	else if (anim == SMILE){
		this->readBuffer(idx, SMILE_IMAGE);
	}
}

void MycroftMouth::smile() {
	state = SMILE;
	drawFrame(0, SMILE);
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
	i = 0;
	nextTime = 0;
}
