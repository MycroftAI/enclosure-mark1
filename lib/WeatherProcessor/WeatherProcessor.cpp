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
                // Format:   display=temp,image
                String  temp;
                byte    c = 8;
                while (cmd[c] != ',' && c < cmd.length()) {
                        temp += cmd[c];
                        c++;
                }

                String icon = cmd;
                icon.remove(0,c+1);

                weather.display(temp, icon);
	}
}
