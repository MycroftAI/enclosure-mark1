#include "MycroftWeather.h"

MycroftWeather::MycroftWeather(MycroftMouth mouth, MycroftEyes eyes) {
    this->mouth = mouth;
    this->eyes = eyes;
}


void MycroftWeather::display(int8_t condition, int8_t current){
    //Serial.println("in weather function");
    byte x = 8;
    byte cond = condition;
    //Serial.println(cond);
    this->mouth.readBuffer(cond, WEATHER_CONDS);
    this->mouth.ht1632.drawImage(mouth.buffer, mouth.width, mouth.height, x, 0);
    x += 8;
    char* buff;
    String currentStr = (itoa(current, buff, 10));
    currentStr += '°';
    //Serial.println(currentStr);
    this->mouth.ht1632.drawText(currentStr.c_str(), x, 2, FONT_5X4, FONT_5X4_WIDTH, FONT_5X4_HEIGHT, FONT_5X4_STEP_GLYPH);
    this->mouth.ht1632.render();
}
