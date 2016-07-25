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
boolean red = false;
boolean green = false;
boolean blue = false;
boolean white = false;
boolean mic = false;
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
  neoPixel.setBrightness(30);
}

// private:
enum class State {
  BOTH, LEFT, RIGHT
};

typedef struct Color {
  byte r, g, b;
};
Color color;

State state;

bool leftOn() {
  return state == State::BOTH || state == State::LEFT;
}

bool rightOn() {
  return state == State::BOTH || state == State::RIGHT;
}

void setEyes(byte r, byte g, byte b) {
  color.r = r;
  color.g = g;
  color.b = b;
  const byte NUM_PIX = neoPixel.numPixels();
  const byte BEGIN = leftOn() ? 0 : NUM_PIX/2;
  const byte END = rightOn() ? NUM_PIX : NUM_PIX/2;
  for (uint16_t i = 0; i < BEGIN; i++) {
    neoPixel.setPixelColor(i, 0, 0, 0);
  }
  for (uint16_t i = BEGIN; i < END; i++) {
    neoPixel.setPixelColor(i, r, g, b);
  }
  for (uint16_t i = END; i < NUM_PIX; i++) {
    neoPixel.setPixelColor(i, 0, 0, 0);
  }
  neoPixel.show();
}

void updateEyes() {
  setEyes(color.r, color.g, color.b);
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

void pause() {
  while (encoder->getButton() != ClickEncoder::Clicked) delay(1);
}

void setState(const char side){
  if(side == 'l'){
  state = State::LEFT;
  updateEyes();
  }
  else if (side == 'r'){
  state = State::RIGHT;
  updateEyes();
  }
  else if (side == 'b'){
    state = State::BOTH;
    updateEyes();
  }
}

void loop() {
  setEyes(255,255,255);
  test_image();
  while (encoder->getButton() != ClickEncoder::DoubleClicked) {
    delay(1);
    int change = encoder->getValue();
    switch(change) {
      case -1:
        setState('r');
        break;
      case 1:
        setState('l');
        break;
      default:
        break;
    }
  }
  setState('b');
  setEyes(255,0,0);
  pause();
  setEyes(0,255,0);
  pause();
  setEyes(0,0,255);
  pause();
  Serial.println("mic.test");
  
}
