#pragma once

#include <Arduino.h>

class MycroftWeather {
public:
	MycroftWeather();
	void display(int8_t condition, String temperature);

private:
	char imgPos, tempPos;
	void calculateImagePosition(String temperature);
	void calculateTemperaturePosition(String temperature);
};
