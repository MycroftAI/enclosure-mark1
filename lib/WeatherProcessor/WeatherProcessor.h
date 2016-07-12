#pragma once

#include "BaseProcessor.h"
#include "MycroftWeather.h"

class MycroftMouth;
class MycroftEyes;

class WeatherProcessor : public BaseProcessor {
public:
	WeatherProcessor(MycroftMouth &mouth, MycroftEyes &eyes);
	void drawAnimation();

private:
	void process(String cmd);

	MycroftWeather weather;
};
