#ifndef MYCROFT_MOUTH_H
#define MYCROFT_MOUTH_H

#include <Arduino.h>
#include "HT1632.h"

class MycroftMouth {

public:
    HT1632Class ht1632;

    MycroftMouth(int pinCS1, int pinWR, int pinDATA);

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

    char width;

    char height;

    char text[64];

    int textWd;

    int textIdx;

    char buffer[16];

    State state;

    void updateText();

    template<size_t x>
    void readBuffer(byte idx, const char (&anim)[x][16]);

    void copyText(const char *value);
};

#endif /* MYCROFT_MOUTH_H */
