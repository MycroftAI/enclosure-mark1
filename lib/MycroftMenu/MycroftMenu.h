#include "MycroftDisplay.h"
#include "MycroftEncoder.h"

class MycroftMenu {
public:
    MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON);
    void run();
    void encoderCheck();
    void enter();
    void finishTest();
    void updateOptionIndex(bool up);
    bool isEntered();
    bool checkTest();
    bool withinUpperBound();
    bool withinLowerBound();
    void checkButton();

private:
    MycroftDisplay display;
    MycroftEncoder encoder;
    bool entered, shouldTest;
    typedef struct OptionContainer {
        enum Option{
            WIFI, RESET, REBOOT, SHUTDOWN, TEST, EXIT
        };
        Option option;
    };
    OptionContainer menuOptions[6];
    uint8_t optionIndex;
    const uint8_t maxIndex = 5;
    void insertOptions();
    void drawOption(String option, bool arrow);
};
