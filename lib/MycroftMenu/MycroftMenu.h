#include "MycroftDisplay.h"
#include "MycroftEncoder.h"
#include "MycroftEyes.h"

class MycroftMenu {
public:
    MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON, int eyeLength, int pinEYES, neoPixelType type);
    enum menuState {
        MAIN, BRIGHTNESS
    };
    menuState getCurrentMenu();
    void run();
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
    MycroftEyes eyes;
    bool entered, shouldTest;
    struct OptionContainer {
        enum Option{
            WIFI, RESET, REBOOT, SHUTDOWN, TEST, EXIT, BRIGHTNESS
        };
        Option option;
    };
    menuState currentState;
    OptionContainer menuOptions[7];
    uint8_t optionIndex;
    const uint8_t maxIndex = 6;
    void insertOptions();
    void drawOption(String option, bool arrow);
};
