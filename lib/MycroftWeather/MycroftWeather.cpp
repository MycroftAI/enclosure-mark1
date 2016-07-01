#include "MycroftWeather.h"

MycroftWeather::MycroftWeather(MycroftMouth* mouth, MycroftEyes* eyes) {
    this->mouth = mouth;
    this->eyes = eyes;
}

void MycroftWeather::display(int8_t condition, int8_t current){
    byte x = 0;
    this->mouth->readBuffer(0, FULL_PLATES);
    this->mouth->ht1632.clear();
    this->mouth->ht1632.drawImage(mouth->buffer, mouth->width, mouth->height, x, 0);
    x += 8;
    this->mouth->readBuffer(condition, WEATHER_CONDS);
    this->mouth->ht1632.drawImage(mouth->buffer, mouth->width, mouth->height, x, 0);
    x += 8;
    String currentStr = String(current);
    currentStr += "\\";
    this->mouth->ht1632.drawText(currentStr.c_str(), x, 0, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_NUMBER_HEIGHT, FONT_5X4_STEP_GLYPH);
    this->mouth->ht1632.render();
    //delay(5000);
}
