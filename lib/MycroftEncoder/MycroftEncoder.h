#include "ClickEncoder.h"
#include "TimerOne.h"

class MycroftEncoder {

public:
    enum class Direction {
        LEFT, RIGHT, NONE

    };

    Direction direction;
    
    ClickEncoder* clickEncoder;

    MycroftEncoder(uint8_t pinEncoderOne, uint8_t pinEncoderTwo, uint8_t pinButton);
 
    Direction getDirection();

    void isr();
    
    
private:
 

};
