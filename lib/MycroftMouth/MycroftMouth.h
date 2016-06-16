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

    void read_buffer_listen(byte idx);

    void read_buffer_smile(byte idx);

    void read_buffer_talk(byte idx);

    void read_buffer_think(byte idx);

    void copyText(const char *value);
};
