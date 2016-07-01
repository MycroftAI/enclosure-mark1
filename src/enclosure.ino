#include "MycroftEyes.h"
#include "MycroftMouth.h"
#include "MycroftArduino.h"
#include "MycroftEncoder.h"

#include "MouthProcessor.h"
#include "EyesProcessor.h"
#include "SystemProcessor.h"

#define BUTTON_PIN 2
#define SPEAKER_PIN 4
#define ENC1_PIN 14
#define ENC2_PIN 15

#define EYES_PIN 3
#define EYES_SIZE 24
#define EYES_TYPE NEO_GRB + NEO_KHZ800

#define MOUTH_CS1 7
#define MOUTH_WR 8
#define MOUTH_DATA 9

MycroftArduino arduino = MycroftArduino(SPEAKER_PIN);
MycroftEyes eyes = MycroftEyes(EYES_SIZE, EYES_PIN, EYES_TYPE);
MycroftMouth mouth = MycroftMouth(MOUTH_CS1, MOUTH_WR, MOUTH_DATA);
MycroftEncoder encoder = MycroftEncoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN);

MouthProcessor mouthProcessor(mouth);
EyesProcessor eyesProcessor(eyes);
SystemProcessor systemProcessor(arduino);

int16_t time = 1000;

void timerIsr(){
    encoder.isr();
}

void initSerial() {
    Serial.begin(9600);
    while (!Serial);
    Serial.flush();
    Serial.println(F("Mycroft Hardware v0.1.3 - Connected"));
}

void setup() {
    initSerial();
    eyes.start();
    arduino.start();
    Timer1.initialize(time);
    Timer1.attachInterrupt(timerIsr);
}

bool contains(String value, String term) {
    return value.indexOf(term) > -1;
}

void processVolume(){
    MycroftEncoder::Direction d = encoder.getDirection();
    if (d == MycroftEncoder::Direction::RIGHT) {
        Serial.println("volume.up");
    }
    else if (d == MycroftEncoder::Direction::LEFT) {
        Serial.println("volume.down");
    }
}

void processButton(){
    ClickEncoder::Button b = encoder.clickEncoder->getButton();
    if (b != ClickEncoder::Open) {
        switch (b) {
            case ClickEncoder::Pressed:
                break;
            case ClickEncoder::Held:
                break;
            case ClickEncoder::Released:
                break;
            case ClickEncoder::Clicked:
                Serial.println("mycroft.stop");
	        arduino.blink(1, 500);
                break;
            case ClickEncoder::DoubleClicked:
            break;
        }
    }
}

void loop() {
    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        Serial.flush();
        Serial.print(F("Command: "));
        Serial.println(cmd);

        if (systemProcessor.tryProcess(cmd));
        else if (eyesProcessor.tryProcess(cmd));
        else mouthProcessor.tryProcess(cmd);
    }
    while (Serial.available() <= 0) {
        processVolume();
        processButton();
        mouth.run();
    }
}
