#include "MycroftDisplay.h"
#include "MycroftEncoder.h"
#include "MycroftEyes.h"

class MycroftMenu {
public:
    MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON);
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
    bool entered, shouldTest;
    struct OptionContainer {
        enum Option{
            WIFI, RESET, REBOOT, SHUTDOWN, TEST, EXIT, ILLUM
        };
        Option option;
    };
    menuState currentState;
    OptionContainer menuOptions[7];
    uint8_t optionIndex;
    const uint8_t maxIndex = 6;
    void insertOptions();
    void drawOption(bool leftArrow, String option, bool rightArrow);
};
