#include "MycroftWeather.h"
#include "MycroftMouth.h"
#include "MouthImages.h"

MycroftWeather::MycroftWeather(MycroftMouth &mouth, MycroftEyes &eyes) :
mouth(mouth), eyes(eyes) { }

void MycroftWeather::calculateImagePosition(String temperature) {
	char first = temperature.charAt(0);
	if (temperature.length() == 1) {
		imgPos = 7;
	} else if (temperature.length() == 2) {
		if (first == '-') {
			imgPos = 6;
		} else {
			imgPos = 5;
		}
	} else if (temperature.length() == 3) {
		imgPos = 3;
	}
}

void MycroftWeather::calculateTemperaturePosition(String temperature) {
	char first = temperature.charAt(0);
	if (temperature.length() == 1) {
		tempPos = 18;
	} else if (temperature.length() == 2) {
		tempPos = 16;
	} else if (temperature.length() == 3) {
		if (first == '-') {
			tempPos = 13;
		} else {
			tempPos = 14;
		}
	}
}

void MycroftWeather::display(int8_t condition, String temperature) {
	calculateImagePosition(temperature);
	calculateTemperaturePosition(temperature);
	mouth.reset();
	mouth.drawImage(imgPos, condition, WEATHER_CONDS);
	temperature += "\\";
	mouth.staticText(temperature, tempPos, 1);
	mouth.ht1632.render();
}
