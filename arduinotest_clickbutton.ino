#include "Adafruit_NeoPixel.h"
#include "HT1632.h"
#include "testimages.h"
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
boolean hasTested = false;
boolean micTested = false;
int16_t last, value;
char buffer[16];

Adafruit_NeoPixel neoPixel = Adafruit_NeoPixel(EYES_SIZE, EYES_PIN, EYES_TYPE);
HT1632Class ht1632 = HT1632Class();
ClickEncoder *encoder;

void timerIsr() {
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
  ht1632.begin(MOUTH_CS1, MOUTH_WR, MOUTH_DATA);
  encoder = new ClickEncoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN);
  neoPixel.begin();
  neoPixel.show();
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}



void testEyes() {
  neoPixel.setBrightness(30);
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 255, 0, 0);
  }
  neoPixel.show();
  delay(1000);
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 0, 255, 0);
  }
  neoPixel.show();
  delay(1000);
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 0, 0, 255);
  }
  neoPixel.show();
  delay(1000);
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
  }
  neoPixel.show();
  delay(1000);
}

void eyesOn() {
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
  }
  neoPixel.show();
}

void eyesOff() {
  for (uint16_t i = 0; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 0, 0, 0);
  }
  neoPixel.show();
}

void rightOn() {
  eyesOff();
  for (uint16_t i = 0; i < neoPixel.numPixels() / 2; i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
  }
  neoPixel.show();
}

void leftOn() {
  eyesOff();
  for (uint16_t i = neoPixel.numPixels() / 2; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
  }
  neoPixel.show();
}

void read_buffer_anim(byte idx) {
  byte size = sizeof(buffer);
  for (byte j = 0; j < size; j++) {
    buffer[j] = (char) pgm_read_byte(&(TEST_ANIMATION[idx][j]));
  }
}

void read_buffer_image(byte idx) {
  byte size = sizeof(buffer);
  for (byte j = 0; j < size; j++) {
    buffer[j] = (char) pgm_read_byte(&(TEST_IMAGE[idx][j]));
  }
}

void test_image() {
  byte size = 4;
  for (byte j = 0; j < size; j++) {
    byte idx = j;
    byte x = j * 8;
    read_buffer_image(idx);
    ht1632.drawImage(buffer, 8, 8, x, 0);
  }
  ht1632.render();
  delay(70);
}

void test_anim() {
  byte size = 32;
  byte plates = 4;
  byte total = (size * 2) - 1;
  boolean back = false;
  for (byte i = 0, count = 0; count < total; count++) {
    ht1632.clear();
    for (byte j = 0; j < plates; j++) {
      byte idx = (i * plates) + j;
      byte x = j * 8;
      read_buffer_anim(idx);
      ht1632.drawImage(buffer, 8, 8, x, 0);
    }
    ht1632.render();
    delay(50);

    if (i < size - 1 && !back) {
      i++;
    } else {
      back = true;
      i--;
    }
  }
}
void testMouth() {
  test_image();
  delay(300);
  test_anim();
}

void rollEyes(){
  eyesOff();
  for (uint16_t i = 0; i < neoPixel.numPixels() / 2; i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
    neoPixel.show();
    delay(50);
  }
  for (uint16_t i = neoPixel.numPixels()/2; i < neoPixel.numPixels(); i++) {
    neoPixel.setPixelColor(i, 255, 255, 255);
    neoPixel.show();
    delay(50);
  }
  
}

int framesHeld = 0, framesOff = 0, prevFramesHeld = 0;

void loop() {
  value += encoder->getValue();

  if (value != last) {
    if (hasTested) {
      eyesOff();
      if (value > last) {
        rightOn();
      }
      else {
        leftOn();
      }
    }
    last = value;
  }

 /* if (digitalRead(2) == LOW) {
      ++framesHeld;
      if (framesHeld == 500) {
          Serial.println("mic.test");
      }
      if (prevFramesHeld > 0) {
          prevFramesHeld = 0;
          // Double clicked
          Serial.println("Double clicked!");
      }
      framesOff = 0;
      
  } else {
      if (framesHeld > 0)
      {
        prevFramesHeld = framesHeld;
        framesHeld = 0;
      }
      if (++framesOff > 500)
          prevFramesHeld = 0;
  }
  delay(1);*/

  ClickEncoder::Button b = encoder->getButton();
  if (b != ClickEncoder::Open) {
    switch (b) {
      case ClickEncoder::Pressed:
        break;
      case ClickEncoder::Held:
        /*test_image();
        Serial.println("mic.test");*/
        break;
      case ClickEncoder::Released:
        break;
      case ClickEncoder::Clicked:
        if(hasTested){
          test_image();
          Serial.println("mic.test");
        }
        break;
      case ClickEncoder::DoubleClicked: 
        rollEyes();
        testEyes();
        testMouth();
        hasTested = true;
        break;
    }
  }
}
