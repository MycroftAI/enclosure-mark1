#include "Arduino.h"
#include "MycroftArduino.h"

MycroftArduino::MycroftArduino(uint8_t buttonPin, uint8_t speakerPin) {
    this->buttonPin = buttonPin;
    this->speakerPin = speakerPin;
}

void MycroftArduino::start() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(this->buttonPin, INPUT);
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

bool MycroftArduino::isButtonPress() {
    return digitalRead(this->buttonPin) == HIGH;
}

void MycroftArduino::blink(long times, unsigned long wait) {
    for (int i = 0; i < times; ++i) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(wait);
        digitalWrite(LED_BUILTIN, LOW);
        delay(wait);
    }
}
