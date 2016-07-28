#pragma once 

#include "BaseProcessor.h"

class MycroftArduino;

class ArduinoProcessor : public BaseProcessor
{
public:
    ArduinoProcessor(MycroftArduino &arduino);
    void setup();

private:
    void process(String cmd);

    MycroftArduino &arduino;
};
