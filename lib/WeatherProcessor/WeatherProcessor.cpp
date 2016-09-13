#include "WeatherProcessor.h"
#include "MycroftWeather.h"
#include "MycroftMouth.h"
#include "MycroftEyes.h"

WeatherProcessor::WeatherProcessor()
  : BaseProcessor("weather")
{
}

void WeatherProcessor::process(const String& cmd) {
	if (cmd.startsWith("display=")) {
		String str(cmd);
		str.replace("display=", "");
		int8_t cond = str.charAt(0) - '0';
		str.remove(0, 1);
		weather.display(cond, str);
	}
}
