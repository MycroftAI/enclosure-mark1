#pragma once

#include <Arduino.h>

class MycroftMouth;
class MycroftEyes;

class MycroftWeather {
public:
	MycroftWeather(MycroftMouth &mouth, MycroftEyes &eyes);

	void display(int8_t condition, String temperature);

private:
	MycroftMouth &mouth;

	MycroftEyes &eyes;

	char imgPos, tempPos;

	void calculateImagePosition(String temperature);

	void calculateTemperaturePosition(String temperature);
};
