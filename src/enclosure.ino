#include "MycroftEyes.h"
#include "MycroftMouth.h"
#include "MycroftArduino.h"
#include "ClickEncoder.h"
#include "TimerOne.h"

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

MycroftArduino arduino = MycroftArduino(BUTTON_PIN, SPEAKER_PIN);
MycroftEyes eyes = MycroftEyes(EYES_SIZE, EYES_PIN, EYES_TYPE);
MycroftMouth mouth = MycroftMouth(MOUTH_CS1, MOUTH_WR, MOUTH_DATA);

int16_t last, value;
ClickEncoder *encoder;

void timerIsr() {//time-based interrupt that checks the encoder's status
  encoder->service();
}

void initSerial() {
    Serial.begin(9600);
    while (!Serial);
    Serial.flush();
    Serial.println(F("Mycroft Hardware v0.1.2 - Connected"));
}

void setup() {
    initSerial();
    eyes.start();
    arduino.start();
    encoder = new ClickEncoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN); //creating a new ClickEncoder object with the three pins used by the encoder
    Timer1.initialize(1000);
    Timer1.attachInterrupt(timerIsr);//these two lines use the TimerOne library to attach a time-based interrupt to one of the Arduino's internal timers
}

bool contains(String value, String term) {
    return value.indexOf(term) > -1;
}

void processSystem(String cmd) {
    if (contains(cmd, "reset")) {
        arduino.reset();
    }
    else if (contains(cmd, "mute")) {
        arduino.mute();
    }
    else if (contains(cmd, "unmute")) {
        arduino.unmute();
    }
    else if (contains(cmd, "blink=")) {
        cmd.replace("blink=", "");
        arduino.blink(cmd.toInt(), 500);
    }
}

void updateEyesColor(long code) {
    long red = (code >> 16) & 0xFF;
    long green = (code >> 8) & 0xFF;
    long blue = code & 0xFF;
    eyes.updateColor((uint8_t) red, (uint8_t) green, (uint8_t) blue);
}

void processEyes(String cmd) {
    if (contains(cmd, "color=")) {
        cmd.replace("color=", "");
        updateEyesColor(cmd.toInt());
    }
    else if (contains(cmd, "level=")) {
        cmd.replace("level=", "");
        eyes.updateBrightness((uint8_t) cmd.toInt());
    }
    else if (contains(cmd, "on")) {
        eyes.on();
    }
    else if (contains(cmd, "off")) {
        eyes.off();
    }
    else if (contains(cmd, "blink=")) {
        cmd.replace("blink=", "");
        eyes.blink(35, cmd.charAt(0));
    }
    else if (contains(cmd, "narrow")) {
        cmd.replace("narrow=", "");
        eyes.narrow(140, cmd.charAt(0));
    }
    else if (contains(cmd, "look=")) {
        cmd.replace("look=", "");
        eyes.look(70, cmd.charAt(0));
    }
}

void processMouth(String cmd) {
    if (contains(cmd, "reset")) {
        mouth.reset();
    }
    else if (contains(cmd, "talk")) {
        mouth.talk();
    }
    else if (contains(cmd, "listen")) {
        mouth.listen();
    }
    else if (contains(cmd, "think")) {
        mouth.think();
    }
    else if (contains(cmd, "smile")) {
        mouth.smile();
    }
    else if (contains(cmd, "text=")) {
        cmd.replace("text=", "");
        mouth.write(cmd.c_str());
    }
}

void checkReset() {
    if (arduino.isButtonPress()) {
        Serial.println("mycroft.stop");
        arduino.blink(1, 500);
    }
}

void loop() {
//volume control: checks the current position of the encoder dial using the ClickEncoder library, and if it's different from the last one, communicates with mycroft core to change the volume
    value += encoder->getValue();

    if (value != last) {
        if (value > last) {
            Serial.println("volume.up");
        }
        else {
            Serial.println("volume.down");
        }
    last = value;
    }

   ClickEncoder::Button b = encoder->getButton();
   if (b != ClickEncoder::Open) { //this switch case is ClickEncoder syntax to handle the different things that can be done with the encoder's button: most of the cases are empty, but they can be filled with different functionality later
    switch (b) { // covers the possible states of the button: "pressed" just means that the button is down, while "clicked" specifically refers to a press followed quickly by a release
      case ClickEncoder::Pressed:
        break;
      case ClickEncoder::Held:
        break;
      case ClickEncoder::Released:  
        break;
      case ClickEncoder::Clicked:
        Serial.println("mycroft.stop"); //replaces checkReset() and arduino.isButtonPress()
	arduino.blink(1, 500);
        break;
      case ClickEncoder::DoubleClicked:
        break;
    }
  }

    if (Serial.available() > 0) {
        String cmd = Serial.readStringUntil('\n');
        Serial.flush();
        Serial.print(F("Command: "));
        Serial.println(cmd);

        if (contains(cmd, "system.")) {
            cmd.replace("system.", "");
            processSystem(cmd);
        }
        else if (contains(cmd, "eyes.")) {
            cmd.replace("eyes.", "");
            processEyes(cmd);
        }
        else if (contains(cmd, "mouth.")) {
            cmd.replace("mouth.", "");
            processMouth(cmd);
        }
    }
    while (Serial.available() <= 0) {
        mouth.run();
    }
}
