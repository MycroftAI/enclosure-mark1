#include "EyesProcessor.h"
#include "MycroftEyes.h"

EyesProcessor::EyesProcessor(MycroftEyes &eyes) :
BaseProcessor("eyes"), eyes(eyes) { }

void EyesProcessor::setup() {
	eyes.setup();
}

void EyesProcessor::updateAnimation() {
	eyes.updateAnimation();
}

void EyesProcessor::updateEyesColor(long code) {
	long red = (code >> 16) & 0xFF;
	long green = (code >> 8) & 0xFF;
	long blue = code & 0xFF;
	eyes.updateColor((uint8_t) red, (uint8_t) green, (uint8_t) blue);
}

void EyesProcessor::process(String cmd) {
	if (contains(cmd, "color=")) {
		Serial.println("1");
		cmd.replace("color=", "");
		updateEyesColor(cmd.toInt());
	} else if (contains(cmd, "level=")) {
		Serial.println("2");
		cmd.replace("level=", "");
		eyes.updateBrightness((uint8_t) cmd.toInt());
	} else if (contains(cmd, "on")) {
		Serial.println("3");
		eyes.on();
	} else if (contains(cmd, "off")) {
		Serial.println("4");
		eyes.off();
	} else if (checkEyeAnim(cmd, "blink", MycroftEyes::BLINK)) {
		Serial.println("5");
		return;
	} else if (checkEyeAnim(cmd, "narrow", MycroftEyes::NARROW)) {
		Serial.println("6");
		return;
	} else if (checkEyeAnim(cmd, "look", MycroftEyes::LOOK)) {
		Serial.println("7");
		return;
	} else if (checkEyeAnim(cmd, "widen", MycroftEyes::WIDEN)) {
		Serial.println("8");
		return;
	} else if (checkEyeAnim(cmd, "unlook", MycroftEyes::UNLOOK)) {
		Serial.println("9");
		return;
	}
}

bool EyesProcessor::checkEyeAnim(String cmd, String term, MycroftEyes::Animation anim){
	Serial.println(cmd);
	Serial.println(term);
	if (contains(cmd, term)) {
		Serial.println("contains");
		term += '=';
		cmd.replace(term, "");
		Serial.println(cmd);
		MycroftEyes::Side side = toSide(cmd.charAt(0));
		eyes.startAnim(anim, side);
		return true;
	}
	return false;
}

MycroftEyes::Side EyesProcessor::toSide(const char SIDE_CHAR){
	switch(SIDE_CHAR) {
	case 'l':
		return MycroftEyes::LEFT;
	case 'r':
		return MycroftEyes::RIGHT;
	case 'b':
		return MycroftEyes::BOTH;
	case 'u':
		return MycroftEyes::UP;
	case 'd':
		return MycroftEyes::DOWN;
	case 'c':
		return MycroftEyes::CROSS;
	}
}
