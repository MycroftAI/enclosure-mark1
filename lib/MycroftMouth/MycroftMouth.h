#include "HT1632.h"
#include "Arduino.h"
#include "MouthImages.h"
#include "../HT1632/font_5x4.h"

#pragma once

class MycroftMouth {

public:
    HT1632Class ht1632;

    MycroftMouth(int pinCS1, int pinWR, int pinDATA);

    MycroftMouth();

    char width;

    char height;

    char buffer[16];

    void reset();

    void run();

    void talk();

    void think();

    void listen();

    void smile();

    void write(const char *value);

    template <size_t x>
    void readBuffer(byte idx, const char(&anim)[x][16]) {
        byte size = sizeof(buffer);
        for (byte j = 0; j < size; j++) {
            buffer[j] = (char) pgm_read_byte(&(anim[idx][j]));
        }
    }

private:
    enum State {
        NONE, TALK, LISTEN, THINK, SMILE, TEXT
    };

    char text[64];

    int textWd;

    int textIdx;

    State state;

    void updateText();

    void copyText(const char *value);
};
