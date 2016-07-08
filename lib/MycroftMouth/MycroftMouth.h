#include "MycroftHT1632.h"
#include "Arduino.h"
#include "MouthImages.h"
#include "../HT1632/font_5x4.h"
#include "font_8x4.h"

#pragma once

class MycroftMouth {

public:
    MycroftHT1632 ht1632;

    MycroftMouth(int pinCS1, int pinWR, int pinDATA);

    MycroftMouth();

    template <size_t y>
    void drawImage(int8_t pos, int8_t index, const char(&imgs)[y][16]){
        readBuffer(index, imgs);
        ht1632.drawImage(buffer, width, height, pos, 0);
    }

    void staticText(String text, int8_t pos, int8_t fontIndex);

    void reset();

    void run();

    void talk();

    void think();

    void listen();

    void smile();

    void write(const char *value);

private:
    enum State {
        NONE, TALK, LISTEN, THINK, SMILE, TEXT
    };

    char text[64];

    char width;

    char height;

    char buffer[16];

    int textWd;

    int textIdx;

    State state, lastState;

    void updateText();

    void copyText(const char *value);

    template <size_t x>
    void readBuffer(byte idx, const char(&anim)[x][16]) {
        byte size = sizeof(buffer);
        for (byte j = 0; j < size; j++) {
            buffer[j] = (char) pgm_read_byte(&(anim[idx][j]));
        }
    }
};
