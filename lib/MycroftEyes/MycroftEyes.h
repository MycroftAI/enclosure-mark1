#ifndef MYCROFT_EYES_H
#define MYCROFT_EYES_H

#include "Adafruit_NeoPixel.h"

class MycroftEyes {

public:
    Adafruit_NeoPixel neoPixel;

    MycroftEyes(uint16_t size, uint8_t pin, uint16_t type);

    void start();

    void on();

    void off();

    void blink(unsigned long wait, const char side);

    void narrow(unsigned long wait, const char side);

    void look(unsigned long wait, const char side);

    void updateColor(uint8_t r, uint8_t g, uint8_t b);

    void updateBrightness(uint8_t level);

private:
    uint32_t color;

    enum Side {
        BOTH, LEFT, RIGHT
    };

    void set(uint32_t color);

    void blink(Side side, byte pos, byte leftJump, unsigned long wait);

    void narrow(Side side, byte pos, byte leftJump, unsigned long wait);

    void look(byte pos, byte leftJump, unsigned long wait);

    uint16_t mod(long a, long b);
};

#endif /* MYCROFT_EYES_H */
