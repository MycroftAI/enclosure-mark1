#include "MycroftMouth.h"
#include "MouthImages.h"
#include "font_5x4.h"
#include "font_8x4.h"

MycroftMouth::MycroftMouth(int pinCS1, int pinWR, int pinDATA) {
    ht1632 = MycroftHT1632();
    ht1632.begin(pinCS1, pinWR, pinDATA);
    reset();
    lastState = state = NONE;
}

MycroftMouth::MycroftMouth(){

}

void MycroftMouth::staticText(String text, int8_t pos, int8_t fontIndex){
      if (fontIndex == 0){
          ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
      }
      else if (fontIndex == 1){
          ht1632.drawTextPgm(text.c_str(), pos, 0, FONT_8X4, FONT_8X4_WIDTH, FONT_8X4_HEIGHT, FONT_8X4_STEP_GLYPH);
      }
}

void MycroftMouth::reset() {
    state = NONE;
    width = 8;
    height = 8;
    textWd = 0;
    textIdx = 0;
    ht1632.clear();
    ht1632.render();
}

void MycroftMouth::drawAnimation() {
    switch (state) {
        case TALK:
            this->talk();
            break;
        case LISTEN:
            this->listen();
            break;
        case THINK:
            this->think();
            break;
        case SMILE:
            this->smile();
            break;
        case TEXT:
            this->updateText();
            break;
        default:
            if (lastState != NONE){
                this->reset();
            }
    }
    lastState = state;
}

void MycroftMouth::talk() {
    state = TALK;
    byte size = 4;
    byte total = (size * 2) - 2;

    for (byte i = 0, count = 0; count < total; count++) {
        ht1632.clear();
        for (byte j = 0; j < size; j++) {
            byte idx = (i * size) + j;
            byte x = j * 8;
            this->readBuffer(idx,TALK_ANIMATION);
            ht1632.drawImage(buffer, width, height, x, 0);
        }
        ht1632.render();
        delay(70);

        if (i < size - 1) {
            i++;
        } else {
            i--;
        }
    }
}



void MycroftMouth::listen() {
    state = LISTEN;
    byte size = 6;
    byte plates = 4;
    byte total = size * 2;

    for (byte i = 0, count = 0; count < total; count++) {
        ht1632.clear();
        for (byte j = 0; j < plates; j++) {
            byte idx = (i * plates) + j;
            byte x = j * 8;
            this->readBuffer(idx, LISTEN_ANIMATION);
            ht1632.drawImage(buffer, width, height, x, 0);
        }
        ht1632.render();
        delay(70);

        if (i < size - 1) {
            i++;
        } else {
            i = 0;
        }
    }
}


void MycroftMouth::think() {
    state = THINK;
    byte size = 8;
    byte plates = 4;
    byte total = (size * 2)-1;
    boolean back = false;

    for (byte i = 0, count = 0; count < total; count++) {
        ht1632.clear();
        for (byte j = 0; j < plates; j++) {
            byte idx = (i * plates) + j;
            byte x = j * 8;
            this->readBuffer(idx, THINK_ANIMATION);
            ht1632.drawImage(buffer, width, height, x, 0);
        }
        ht1632.render();
        delay(200);

        if (i < size - 1 && !back) {
            i++;
        } else {
            back = true;
            i--;
        }
    }
}


void MycroftMouth::smile() {
    state = SMILE;
    byte size = 4;
        for (byte j = 0; j < size; j++) {
            byte idx = j;
            byte x = j * 8;
            this->readBuffer(idx, SMILE_IMAGE);
            ht1632.drawImage(buffer, width, height, x, 0);
        }
        ht1632.render();
        delay(70);
}

void MycroftMouth::write(const char *value) {
    state = TEXT;
    copyText(value);
    textWd = HT1632.getTextWidth(text, FONT_5X4_WIDTH, FONT_5X4_HEIGHT);
    textIdx = 0;
    this->updateText();
}

void MycroftMouth::copyText(const char *value) {
    for (byte i = 0; i < sizeof(text); i++) {
        text[i] = ' ';
    }
    strcpy(text, value);
    Serial.println(text);
}

void MycroftMouth::updateText() {
    ht1632.transition(TRANSITION_BUFFER_SWAP);
    ht1632.clear();
    ht1632.drawTextPgm(text, OUT_SIZE - textIdx, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
    ht1632.render();
    textIdx = (textIdx + 1) % (textWd + OUT_SIZE);
    delay(150);
}
