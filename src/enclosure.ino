#include "ClickEncoder.h"
#include "TimerOne.h"

#include "MycroftArduino.h"
#include "MycroftMouth.h"
#include "MycroftEyes.h"
#include "MycroftEncoder.h"

#include "MouthProcessor.h"
#include "EyesProcessor.h"
#include "ArduinoProcessor.h"
#include "WeatherProcessor.h"

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

// Must be initialized first
MycroftArduino arduino(SPEAKER_PIN);
MycroftEncoder encoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN);
MycroftEyes eyes(EYES_SIZE, EYES_PIN, EYES_TYPE);
MycroftMouth mouth(MOUTH_CS1, MOUTH_WR, MOUTH_DATA, MOUTH_PLATES);

MouthProcessor mouthProcessor(mouth);
EyesProcessor eyesProcessor(eyes);
ArduinoProcessor arduinoProcessor(arduino);
WeatherProcessor weatherProcessor(mouth, eyes);
BaseProcessor *processors[] = {
	&mouthProcessor,
	&eyesProcessor,
	&arduinoProcessor,
	&weatherProcessor
};

void timerIsr() {
	encoder.isr();
}

void initSerial() {
	Serial.begin(9600);
	while (!Serial);
	Serial.flush();
	Serial.println(F("Mycroft Hardware v0.1.3 - Connected"));
}

void setup() {
	initSerial();
	eyesProcessor.setup();
	arduinoProcessor.setup();
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);
}

void processVolume() {
	MycroftEncoder::Direction d = encoder.getDirection();
	if (d == MycroftEncoder::Direction::RIGHT) {
		Serial.println("volume.up");
	} else if (d == MycroftEncoder::Direction::LEFT) {
		Serial.println("volume.down");
	}
}

void processButton() {
	ClickEncoder::Button b = encoder.clickEncoder->getButton();
	if (b != ClickEncoder::Open) {
		switch (b) {
			case ClickEncoder::Pressed:
				break;
			case ClickEncoder::Held:
				break;
			case ClickEncoder::Released:
				break;
			case ClickEncoder::Clicked:
				Serial.println("mycroft.stop");
				break;
			case ClickEncoder::DoubleClicked:
				break;
		}
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
		processVolume();
		processButton();
		mouth.update();
	}
}
