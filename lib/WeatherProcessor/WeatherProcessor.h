#pragma once

#include "BaseProcessor.h"
#include "MycroftWeather.h"

class WeatherProcessor : public BaseProcessor {
public:
	WeatherProcessor();
	void drawAnimation();

private:
	void process(const String& cmd);

	MycroftWeather weather;
};
