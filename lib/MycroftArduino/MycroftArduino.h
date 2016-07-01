#ifndef MYCROFT_ARDUINO_H
#define MYCROFT_ARDUINO_H

#include <Arduino.h>

class MycroftArduino {

public:
    MycroftArduino(uint8_t speakerPin);

    void start();

    void reset();

    void mute();

    void unmute();

    void blink(long times, unsigned long wait);

private:
    uint8_t speakerPin;

};

#endif /* MYCROFT_ARDUINO_H */
