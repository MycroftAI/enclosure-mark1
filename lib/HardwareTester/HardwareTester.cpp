#include "HT1632.h"
#include "HardwareTester.h"
#include "MycroftEncoder.h"
#include "MycroftMouth.h"
#include "MycroftArduino.h"

HardwareTester::HardwareTester() : color(Adafruit_NeoPixel::Color(255,255,255)),
side(MycroftEyes::BOTH) { }

const char HardwareTester::WHITE_PANEL[16] = {
	0xF, 0xF, 0xF, 0xF,
	0xF, 0xF, 0xF, 0xF,
	0xF, 0xF, 0xF, 0xF,
	0xF, 0xF, 0xF, 0xF,
};

void HardwareTester::drawWhiteScreen(MycroftMouth &mouth) {
	const byte NUM_PANELS = 4;
	const byte PANEL_PX = 8;
	for (byte j = 0; j < NUM_PANELS; j++) {
		mouth.setPanel(j * PANEL_PX, WHITE_PANEL);
	}
	mouth.render();
}

void HardwareTester::setEyeSide(MycroftEyes &eyes, MycroftEyes::Side side){
	this->side = side;
	eyes.set(side, color);
}

void HardwareTester::HardwareTester::setEyes(MycroftEyes &eyes, byte r, byte g, byte b) {
	color = Adafruit_NeoPixel::Color(r, g, b);
	eyes.set(side, color);
}

void HardwareTester::pause(MycroftEncoder &encoder) {
	while (!encoder.isClicked()) {
		delay(1);
	}
}

void HardwareTester::testKnob(MycroftEncoder &encoder, MycroftEyes &eyes) {
	while (!encoder.isClicked()) {
		delay(1);
		switch(encoder.getDirection()) {
			case MycroftEncoder::Direction::RIGHT:
				setEyeSide(eyes, MycroftEyes::Side::RIGHT);
				break;
			case MycroftEncoder::Direction::LEFT:
				setEyeSide(eyes, MycroftEyes::Side::LEFT);
				break;
			default:
				break;
		}
	}
}

void HardwareTester::testMute(MycroftArduino &arduino) {
	// 7/2 seconds == 3.5 seconds
	const byte SEC_NUM = 7;
	const byte SEC_DEN = 2;
	const byte TIMES_PER_SEC = 16;
	for (int i = 0; i < (SEC_NUM * TIMES_PER_SEC) / SEC_DEN; ++i) {
		// 1/3 Mute and 2/3 Unmute
		arduino.mute();
		delay(1000 / (3 * TIMES_PER_SEC));
		arduino.unmute();
		delay(2000 / (3 * TIMES_PER_SEC));
	}
}

void HardwareTester::run(MycroftEncoder &encoder, MycroftEyes &eyes, MycroftMouth &mouth, MycroftArduino &arduino) {
	setEyes(eyes, 255,255,255);
	drawWhiteScreen(mouth);
	Serial.println("system.test.begin");
	testKnob(encoder, eyes);

	setEyeSide(eyes, MycroftEyes::Side::BOTH);
	setEyes(eyes, 255,0,0);
	pause(encoder);

	setEyes(eyes, 0,255,0);
	pause(encoder);

	setEyes(eyes, 0,0,255);
	pause(encoder);

	setEyes(eyes, 255,255,255);
	Serial.println("mic.test");
	testMute(arduino);
	Serial.println("system.test.end");
	mouth.reset();
}
