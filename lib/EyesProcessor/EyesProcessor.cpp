#include "EyesProcessor.h"
#include <MycroftEyes.h>

EyesProcessor::EyesProcessor(MycroftEyes &eyes) :
BaseProcessor("eyes"), eyes(eyes) { }

void EyesProcessor::setup() {
	eyes.setup();
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
	} else if (contains(cmd, "on")) {
		eyes.on();
	} else if (contains(cmd, "off")) {
		eyes.off();
	} else if (contains(cmd, "blink=")) {
		cmd.replace("blink=", "");
		eyes.blink(35, cmd.charAt(0));
	} else if (contains(cmd, "narrow")) {
		cmd.replace("narrow=", "");
		eyes.narrow(140, cmd.charAt(0));
	} else if (contains(cmd, "look=")) {
		cmd.replace("look=", "");
		eyes.look(70, cmd.charAt(0));
	}
}
