#include "MycroftWeather.h"

MycroftWeather::MycroftWeather(MycroftMouth* mouth, MycroftEyes* eyes) {
    this->mouth = mouth;
    this->eyes = eyes;
}

void MycroftWeather::display(int8_t condition, String temperature){
    char first = temperature.charAt(0);
    if(temperature.length() == 1){
      imgPos = 7;
      tempPos = 18;
    }
    else if (temperature.length() == 2){
      tempPos = 16;
      if (first == '-'){
        imgPos = 6;
      }
      else{
        imgPos = 5;
      }
    }
    else if (temperature.length() == 3){
      imgPos = 3;
      if (first == '-'){
        tempPos = 13;
      }
      else{
        tempPos = 14;
      }
    }
    this->mouth->reset();
    this->mouth->drawImage(imgPos, condition, WEATHER_CONDS);
    temperature += "\\";
    this->mouth->staticText(temperature, tempPos, 1);
    this->mouth->ht1632.render();
}
