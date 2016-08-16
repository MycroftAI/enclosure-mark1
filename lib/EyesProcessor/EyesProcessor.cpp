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
		cmd.replace("color=", "");
		updateEyesColor(cmd.toInt());
	} else if (contains(cmd, "level=")) {
		cmd.replace("level=", "");
		eyes.updateBrightness((uint8_t) cmd.toInt());
	} else if (contains(cmd, "volume=")) {
		cmd.replace("volume=", "");
		eyes.setEyePixels(MycroftEyes::BOTH, (uint8_t)cmd.toInt());
	} else if (contains(cmd, "on")) {
		eyes.on();
	} else if (contains(cmd, "off")) {
		eyes.off();
	} else if (checkEyeAnim(cmd, "blink", MycroftEyes::BLINK)) {
		return;
	} else if (checkEyeAnim(cmd, "narrow", MycroftEyes::NARROW)) {
		return;
	} else if (checkEyeAnim(cmd, "look", MycroftEyes::LOOK)) {
		return;
	} else if (checkEyeAnim(cmd, "widen", MycroftEyes::WIDEN)) {
		return;
	} else if (checkEyeAnim(cmd, "unlook", MycroftEyes::UNLOOK)) {
		return;
	}
}

bool EyesProcessor::checkEyeAnim(String cmd, String term, MycroftEyes::Animation anim){
	if (contains(cmd, term)) {
		term += '=';
		cmd.replace(term, "");
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
