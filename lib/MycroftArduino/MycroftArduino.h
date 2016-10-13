#pragma once

#include <Arduino.h>

// Version info
#define STR_EXPAND(x)                   #x
#define STR(tok)			STR_EXPAND(tok)
#define ENCLOSURE_VERSION_MAJOR         0
#define ENCLOSURE_VERSION_MINOR         2
#define ENCLOSURE_VERSION_REVISION      1
#define ENCLOSURE_VERSION_STRING        STR(ENCLOSURE_VERSION_MAJOR) \
                                        "." STR(ENCLOSURE_VERSION_MINOR) \
                                        "." STR(ENCLOSURE_VERSION_REVISION)


class MycroftArduino {
public:
	static MycroftArduino& instance() { return *m_instance; }

	MycroftArduino(uint8_t speakerPin);
	void setup();
	void reset();
	void mute();
	void unmute();
	void blink(long times, unsigned long wait);

private:
        static MycroftArduino* m_instance;
	uint8_t speakerPin;
};
