#include "WeatherProcessor.h"
#include <MycroftWeather.h>
#include <MycroftMouth.h>
#include <MycroftEyes.h>

WeatherProcessor::WeatherProcessor(MycroftMouth &mouth, MycroftEyes &eyes) :
BaseProcessor("weather"), weather(&mouth, &eyes) { }

void WeatherProcessor::process(String cmd) {
	if (contains(cmd, "display=")) {
		cmd.replace("display=", "");
		int8_t cond = cmd.charAt(0) - '0';
		cmd.remove(0, 1);
		weather.display(cond, cmd);
	}
}
