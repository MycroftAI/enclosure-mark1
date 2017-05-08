#pragma once

#include <Arduino.h>

class MycroftWeather {
public:
	MycroftWeather();
	void display(const String& temperature, const String& icon);

private:
	char imgPos, tempPos;
	void calculateTemperaturePosition(String temperature);
};
