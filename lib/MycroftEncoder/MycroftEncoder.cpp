#include "Arduino.h"
#include "MycroftArduino.h"
#include "MycroftEncoder.h"

int16_t last, value;

void MycroftEncoder::isr(){
    clickEncoder->service();
}

MycroftEncoder::MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton) {
    clickEncoder = new ClickEncoder(pinEncoderOne, pinEncoderTwo, pinButton); 
}

MycroftEncoder::Direction MycroftEncoder::getDirection() {
    MycroftEncoder::Direction ret;
    value += clickEncoder->getValue();
    if (value != last) {
        if (value > last) {
            ret = Direction::LEFT;
        }
        else if (value < last) {
            ret = Direction::RIGHT;
        }
        else{
	    ret = Direction::NONE;
        }
    last = value;
    }
	return ret;
}


