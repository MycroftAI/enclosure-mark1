#include "MycroftDisplay.h"
#include "MycroftEncoder.h"
#include "MycroftEyes.h"

class MycroftMenu {
public:
    MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON);
    enum menuState {
        MAIN, BRIGHTNESS, RESET_UNIT, ALLOW_SSH
    };
    menuState getCurrentMenu();
    void drawText();
    void enter();
    void exitMenu();
    void finishTest();
    void updateOptionIndex(bool up);
    bool isEntered();
    bool checkTest();
    void pressButton();
    void syncBrightness();

private:
    MycroftDisplay display;
    MycroftEncoder encoder;
    bool entered, shouldTest, choice;
    
    // This defines the order of menu items
    struct OptionContainer {
        enum Option{
            ILLUM, WIFI, REBOOT, SHUTDOWN, TEST, SSH, RESET, DEMO, EXIT, _COUNT
        };
        Option option;
    };
    menuState currentState;
    OptionContainer menuOptions[OptionContainer::_COUNT];
    uint8_t optionIndex;
    void drawOption(bool leftArrow, const String& option, bool rightArrow);
};
