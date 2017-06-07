#include "MycroftWeather.h"
#include "MycroftMouth.h"
#include "MouthImages.h"

MycroftWeather::MycroftWeather()
{
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

void MycroftWeather::display(const String& temperature, const String& icon) {
	MycroftMouth&	mouth = MycroftMouth::instance();

	calculateTemperaturePosition(temperature);
	mouth.reset();
        mouth.showIcon(icon.c_str());
        // "\\" is mapped as the degree symbol
	mouth.staticText(temperature+"\\", tempPos, 1);
	mouth.ht1632.render();
}
