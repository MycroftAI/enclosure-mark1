#include "Arduino.h"
#include "MycroftArduino.h"
#include "MycroftEncoder.h"

void MycroftEncoder::isr(){
    clickEncoder->service();
}

MycroftEncoder::MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton) {
    pinMode(pinButton, INPUT_PULLUP);
    clickEncoder = new ClickEncoder(pinEncoderOne, pinEncoderTwo, pinButton);
    value = 0;
    last = 0;
}

MycroftEncoder::Direction MycroftEncoder::getDirection() {
    direction = Direction::NONE;
    value += clickEncoder->getValue();
    if (value != last) {
        if (value < last) {
            direction = Direction::LEFT;
        }
        else if (value > last) {
            direction = Direction::RIGHT;
        }
        else{
	    direction = Direction::NONE;
        }
    last = value;
    }
    return direction;
}
