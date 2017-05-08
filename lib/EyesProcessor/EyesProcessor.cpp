#include "EyesProcessor.h"
#include "MycroftEyes.h"

EyesProcessor::EyesProcessor()
   : BaseProcessor("eyes")
{
}

void EyesProcessor::setup() {
	MycroftEyes::instance().setup();
}

void EyesProcessor::updateEyesColor(long code) {
	long red = (code >> 16) & 0xFF;
	long green = (code >> 8) & 0xFF;
	long blue = code & 0xFF;
	MycroftEyes::instance().updateColor((uint8_t) red, (uint8_t) green, (uint8_t) blue);
}

void EyesProcessor::process(const String& cmd) {
	MycroftEyes&	eyes = MycroftEyes::instance();

	if (cmd.startsWith("color=")) {
		// substring(6) to skip "color="
		updateEyesColor(cmd.substring(6).toInt());
	} else if (cmd.startsWith("level=")) {
		// substring(6) to skip "level="
		eyes.updateBrightness((uint8_t) cmd.substring(6).toInt());
	} else if (cmd.startsWith("fill=")) {
		// substring(6) to skip "fill="
		eyes.fill((uint8_t) cmd.substring(5).toInt());
	} else if (cmd.startsWith("volume=")) {
		// substring(6) to skip "volume="
		eyes.setEyePixels(MycroftEyes::BOTH, (uint8_t)cmd.substring(7).toInt());
	} else if (cmd.startsWith("spin=")) {
		eyes.timedSpin(cmd.substring(5).toInt());
	} else if (cmd.startsWith("on")) {
		eyes.on();
	} else if (cmd.startsWith("off")) {
		eyes.off();
	} else if (cmd.startsWith("set")) {
                uint8_t     pixel = 0;
                uint32_t    color = 0;
                
                // parse the pixel #
                byte        c = 4; // skip "set="
                while (c < cmd.length() && cmd[c] != ',')
                {
                    pixel = pixel*10 + uint8_t(cmd[c]-'0');
                    c++;
                }
                // parse the color
                while (++c < cmd.length() && cmd[c] != ',')
                {
                    color = color*10 + uint32_t(cmd[c]-'0');
                }
                eyes.setPixel(pixel, color);
	} else if (cmd.startsWith("reset")) {
		eyes.reset();
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

bool EyesProcessor::checkEyeAnim(const String& cmd, const String& term, MycroftEyes::Animation anim) {
	if (cmd.startsWith(term)) {
		String str(cmd);
		str.replace(term+"=", "");
		MycroftEyes::Side side = toSide(str.charAt(0));
		MycroftEyes::instance().startAnim(anim, side);
		return true;
	}
	return false;
}

MycroftEyes::Side EyesProcessor::toSide(const char SIDE_CHAR) {
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
	default:
	    return MycroftEyes::BOTH;
	}
}
