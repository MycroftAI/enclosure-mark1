#ifndef MYCROFT_WEATHER_H
#define MYCROFT_WEATHER_H

#include "MycroftEyes.h"
#include "MycroftMouth.h"

class MycroftWeather {
public:
	MycroftWeather(MycroftMouth *mouth, MycroftEyes *eyes);

	void display(int8_t condition, String temperature);

private:
	MycroftMouth *mouth;

	MycroftEyes *eyes;

	char imgPos, tempPos;

	void calculateImagePosition(String temperature);

	void calculateTemperaturePosition(String temperature);
};

#endif /* MYCROFT_WEATHER_H */
