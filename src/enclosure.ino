#include "ClickEncoder.h"
#include "TimerOne.h"

#include "MycroftArduino.h"
#include "MycroftMouth.h"
#include "MycroftEyes.h"
#include "MycroftEncoder.h"
#include "MycroftMenu.h"
#include "HardwareTester.h"

#include "MouthProcessor.h"
#include "EyesProcessor.h"
#include "ArduinoProcessor.h"
#include "WeatherProcessor.h"
#include "HardwareTestProcessor.h"

#define BUTTON_PIN 2
#define SPEAKER_PIN 4
#define ENC1_PIN 14
#define ENC2_PIN 15

#define EYES_PIN 3
#define EYES_SIZE 24
#define EYES_TYPE NEO_GRB + NEO_KHZ800

#define MOUTH_CS1 7
#define MOUTH_WR 8
#define MOUTH_DATA 9
#define MOUTH_PLATES 4

#define MENU_HOLD_TIME			2000	// in milliseconds


// Must be initialized first
MycroftArduino	arduino(SPEAKER_PIN);
MycroftArduino*	MycroftArduino::m_instance = &arduino;

MycroftEncoder	encoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN);
MycroftEncoder*	MycroftEncoder::m_instance = &encoder;

MycroftEyes	eyes(EYES_SIZE, EYES_PIN, EYES_TYPE);
MycroftEyes*	MycroftEyes::m_instance = &eyes;

MycroftMouth	mouth(MOUTH_CS1, MOUTH_WR, MOUTH_DATA, MOUTH_PLATES);
MycroftMouth*	MycroftMouth::m_instance = &mouth;

MycroftMenu	menu(MOUTH_CS1, MOUTH_WR, MOUTH_DATA, ENC1_PIN, ENC2_PIN, BUTTON_PIN);
HardwareTester	hardwareTester;

MouthProcessor		mouthProcessor;
EyesProcessor		eyesProcessor;
ArduinoProcessor	arduinoProcessor(arduino);
WeatherProcessor	weatherProcessor;
HardwareTestProcessor	hardwareTestProcessor(hardwareTester);
BaseProcessor*		processors[] = {
				&mouthProcessor,
				&eyesProcessor,
				&arduinoProcessor,
				&weatherProcessor,
				&hardwareTestProcessor
			};

static void timerIsr() {
	encoder.isr();
}

void initSerial() {
	Serial.begin(9600);
	while (!Serial);
	Serial.flush();
	Serial.println(F("Mycroft Hardware v" ENCLOSURE_VERSION_STRING " - Connected"));
}

void setup() {
	initSerial();
	eyesProcessor.setup();
	arduinoProcessor.setup();
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);
}

static void processVolume() {
	MycroftEncoder::Direction d = encoder.getDirection();
	if (d == MycroftEncoder::Direction::RIGHT) {
		Serial.println(F("volume.up"));
	} else if (d == MycroftEncoder::Direction::LEFT) {
		Serial.println(F("volume.down"));
	}
}

static void processMenuEncoder() {
	MycroftEncoder::Direction d = encoder.getDirection();
	if (d == MycroftEncoder::Direction::RIGHT) {
		if (menu.withinUpperBound()) {
			menu.updateOptionIndex(true);
		}
	} else if (d == MycroftEncoder::Direction::LEFT) {
		if (menu.withinLowerBound()) {
			menu.updateOptionIndex(false);
		}
	}
}

static void processBrightnessEncoder() {
	MycroftEncoder::Direction d = encoder.getDirection();
	if (d == MycroftEncoder::Direction::RIGHT) {
		eyes.incrementBrightness(true);
		menu.syncBrightness();
		menu.run();
	} else if (d == MycroftEncoder::Direction::LEFT) {
		eyes.incrementBrightness(false);
		menu.syncBrightness();
		menu.run();
	}
}

static void processButton() {
	if (encoder.isClicked()) {
		if(menu.isEntered()) {
			menu.checkButton();
		}
		else {
			Serial.println(F("mycroft.stop"));
		}
	}
	if (encoder.getFramesHeld() >= MENU_HOLD_TIME) {
		menu.enter();
	}
}

void loop() {
	if (Serial.available() > 0) {
		String cmd = Serial.readStringUntil('\n');
		Serial.flush();
		Serial.print(F("Command: "));
		Serial.println(cmd);

		for (BaseProcessor *i : processors)
			if (i->tryProcess(cmd))
				break;
	}
	while (Serial.available() <= 0) {
		processButton();
		if(menu.checkTest()) {
			hardwareTester.run(encoder, eyes, mouth, arduino);
			menu.finishTest();
		}
		if(menu.isEntered()) {
			menu.run();
			if (menu.getCurrentMenu() == MycroftMenu::MAIN){
				processMenuEncoder();
			} else if (menu.getCurrentMenu() == MycroftMenu::BRIGHTNESS){
				processBrightnessEncoder();
			}
		}
		else {
			processVolume();
			eyes.updateAnimation();
			mouth.update();
			if (mouth.state != MycroftMouth::NONE && eyes.currentAnim == MycroftEyes::SPIN) {
				eyes.reset();
			}
		}
	}
}
