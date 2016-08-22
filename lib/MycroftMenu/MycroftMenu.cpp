#include "MycroftMenu.h"

MycroftMenu::MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON) :
display(pinCS1, pinWR, pinDATA), encoder(pinENC1,pinENC2,pinBUTTON) {
    optionIndex = 2;
    currentState = MAIN;
    entered = false;
    shouldTest = false;
    breathe = false;
    insertOptions();
}

void MycroftMenu::enter() {
    entered = true;
}

bool MycroftMenu::isEntered() {
    return entered;
}

bool MycroftMenu::checkTest() {
    return shouldTest;
}

bool MycroftMenu::checkBreathe() {
    return breathe;
}

void MycroftMenu::breatheStarted() {
    breathe = false;
}

void MycroftMenu::finishTest() {
    shouldTest = false;
}

bool MycroftMenu::withinUpperBound() {
    return (optionIndex < maxIndex);
}

bool MycroftMenu::withinLowerBound() {
    return (optionIndex != 0);
}

void MycroftMenu::updateOptionIndex(bool up) {
    if(up) {
        optionIndex++;
    } else {
        optionIndex--;
    }
}

MycroftMenu::menuState MycroftMenu::getCurrentMenu() {
    return currentState;
}

void MycroftMenu::drawOption(String option, bool arrow) {
    display.clear();
    display.drawText(option, 0, true);
    if(arrow) {
        display.drawText(">", 29, true);
    }
    display.render();
}

void MycroftMenu::run() {
    if(currentState == MAIN) {
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::REBOOT:
                drawOption(" REBOOT", true);
                break;
            case OptionContainer::WIFI:
                drawOption("<WI-FI ", true);
                break;
            case OptionContainer::SHUTDOWN:
                drawOption("<  OFF  ", true);
                break;
            case OptionContainer::TEST:
                drawOption("<  TEST  >", false);
                break;
            case OptionContainer::RESET:
                drawOption("< RESET >", false);
                break;
            case OptionContainer::EXIT:
                drawOption("<  EXIT  >", false);
                break;
            case OptionContainer::BRIGHTNESS:
                drawOption("< BRIGHT >", false);
                break;
            case OptionContainer::BREATHE:
                drawOption("<BREATHE", false);
                break;
        }
    }
    else if (currentState == BRIGHTNESS) {
        drawOption("<DN UP>", false);
    }
}

void MycroftMenu::checkButton() {
    if (currentState == MAIN){
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::REBOOT:
                Serial.println(F("unit.reboot"));
                entered = false;
                break;
            case OptionContainer::WIFI:
                Serial.println(F("unit.setwifi"));
                entered = false;
                break;
            case OptionContainer::SHUTDOWN:
                Serial.println(F("unit.shutdown"));
                entered = false;
                break;
            case OptionContainer::TEST:
                shouldTest = true;
                entered = false;
                break;
            case OptionContainer::RESET:
                Serial.println(F("unit.factory-reset"));
                entered = false;
                break;
            case OptionContainer::BRIGHTNESS:
                currentState = BRIGHTNESS;
                break;
            case OptionContainer::BREATHE:
                breathe = true;
                entered = false;
                break;
            case OptionContainer::EXIT:
                entered = false;
                break;
        }
        optionIndex = 2;
    } else if (currentState == BRIGHTNESS) {
        entered = false;
        currentState = MAIN;
    }
    display.clear();
    display.render();
}

void MycroftMenu::insertOptions() {
    menuOptions[0].option = OptionContainer::REBOOT;
    menuOptions[1].option = OptionContainer::WIFI;
    menuOptions[2].option = OptionContainer::SHUTDOWN;
    menuOptions[3].option = OptionContainer::TEST;
    menuOptions[4].option = OptionContainer::RESET;
    menuOptions[5].option = OptionContainer::BRIGHTNESS;
    menuOptions[6].option = OptionContainer::EXIT;
    menuOptions[7].option = OptionContainer::BREATHE;
}
