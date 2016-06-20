#include "Arduino.h"
#include "MycroftArduino.h"


MycroftArduino::MycroftArduino(uint8_t speakerPin, void (*delay)(int)) {
    this->speakerPin = speakerPin;
    this->delay = delay;
}

void MycroftArduino::start() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(this->speakerPin, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(this->speakerPin, HIGH);
}

void MycroftArduino::reset() {
    blink(1, 1000);
    asm volatile ("  jmp 0");
}

void MycroftArduino::mute() {
    digitalWrite(this->speakerPin, LOW);
}

void MycroftArduino::unmute() {
    digitalWrite(this->speakerPin, HIGH);
}

void MycroftArduino::blink(long times, unsigned long wait) {
    for (int i = 0; i < times; ++i) {
        digitalWrite(LED_BUILTIN, HIGH);
        this->delay(wait);
        digitalWrite(LED_BUILTIN, LOW);
        this->delay(wait);
    }
}
