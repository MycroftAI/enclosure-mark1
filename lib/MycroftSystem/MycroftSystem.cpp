#include <Arduino.h>
#include "MycroftSystem.h"

MycroftSystem::MycroftSystem(uint8_t speakerPin) : speakerPin(speakerPin) { }

void MycroftSystem::start() {
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(this->speakerPin, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	digitalWrite(this->speakerPin, HIGH);
}

void MycroftSystem::reset() {
	blink(1, 1000);
	asm volatile ("  jmp 0");
}

void MycroftSystem::mute() {
	digitalWrite(this->speakerPin, LOW);
}

void MycroftSystem::unmute() {
	digitalWrite(this->speakerPin, HIGH);
}

void MycroftSystem::blink(long times, unsigned long wait) {
	for (int i = 0; i < times; ++i) {
		digitalWrite(LED_BUILTIN, HIGH);
		delay(wait);
		digitalWrite(LED_BUILTIN, LOW);
		delay(wait);
	}
}
