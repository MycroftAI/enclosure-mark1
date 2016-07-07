#include "MycroftWeather.h"

MycroftWeather::MycroftWeather(MycroftMouth* mouth, MycroftEyes* eyes) {
    this->mouth = mouth;
    this->eyes = eyes;
}

void MycroftWeather::display(int8_t condition, int8_t current){
    this->mouth->reset();
    this->mouth->drawImage(8, condition, WEATHER_CONDS);
    String currentStr = String(current);
    currentStr += "\\";
    this->mouth->staticText(currentStr, 16, 1);
    this->mouth->ht1632.render();
}
