#ifndef WEATHER_PROCESSOR_H
#define WEATHER_PROCESSOR_H

#include <BaseProcessor.h>
#include <MycroftWeather.h>

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

#endif /* WEATHER_PROCESSOR_H */
