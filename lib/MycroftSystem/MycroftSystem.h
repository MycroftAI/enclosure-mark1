#ifndef MYCROFT_SYSTEM_H
#define MYCROFT_SYSTEM_H

#include <Arduino.h>

class MycroftSystem {
public:
	MycroftSystem(uint8_t speakerPin);

	void start();

	void reset();

	void mute();

	void unmute();

	void blink(long times, unsigned long wait);

private:
	uint8_t speakerPin;

};

#endif /* MYCROFT_SYSTEM_H */
