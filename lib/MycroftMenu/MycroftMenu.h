/**                                                                         
#Copyright 2017, Mycroft AI, Inc.
#
#Licensed under the Apache License, Version 2.0 (the "License");
#you may not use this file except in compliance with the License.
#You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
#Unless required by applicable law or agreed to in writing, software
#distributed under the License is distributed on an "AS IS" BASIS,
#WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#See the License for the specific language governing permissions and
#limitations under the License.
**/
#include "MycroftDisplay.h"
#include "MycroftEncoder.h"
#include "MycroftEyes.h"

class MycroftMenu {
public:
    MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON);
    enum menuState {
        MAIN, BRIGHTNESS, RESET_UNIT, ALLOW_SSH, LEARN
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
            ILLUM, WIFI, REBOOT, SHUTDOWN, TEST, SSH, LEARN, RESET, DEMO, EXIT, _COUNT
        };
        Option option;
    };
    menuState currentState;
    OptionContainer menuOptions[OptionContainer::_COUNT];
    uint8_t optionIndex;
    void drawOption(bool leftArrow, const String& option, bool rightArrow);
};
