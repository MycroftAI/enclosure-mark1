#pragma once 

#include <BaseProcessor.h>
#include <MycroftArduino.h>

class ArduinoProcessor : public BaseProcessor
{
public:
    ArduinoProcessor(uint8_t speakerPin);
    void setup();

private:
    void process(String cmd);

    MycroftArduino arduino;
};
