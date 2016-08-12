#include "MycroftMenu.h"

MycroftMenu::MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON) :
display(pinCS1, pinWR, pinDATA), encoder(pinENC1,pinENC2,pinBUTTON) {
    optionIndex = 2;
    entered = false;
    shouldTest = false;
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
    }
    else {
        optionIndex--;
    }
}

void MycroftMenu::drawOption(String option, bool arrow) {
    display.clear();
    display.drawText(option, 0, true);
    if(arrow){
        display.drawText(">", 29, true);
    }
    display.render();
    checkButton();
}

void MycroftMenu::run() {
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
            drawOption("<  EXIT  ", false);
            break;
    }
}

void MycroftMenu::checkButton() {
    //Serial.println("button check");
    if(encoder.isClicked()) {
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::REBOOT:
                Serial.println("unit.reboot");
                entered = false;
                break;
            case OptionContainer::WIFI:
                Serial.println("unit.setwifi");
                entered = false;
                break;
            case OptionContainer::SHUTDOWN:
                Serial.println("unit.shutdown");
                entered = false;
                break;
            case OptionContainer::TEST:
                shouldTest = true;
                entered = false;
                break;
            case OptionContainer::RESET:
                Serial.println("unit.factory-reset");
                entered = false;
                break;
            case OptionContainer::EXIT:
                entered = false;
                break;
        }
    }
}

void MycroftMenu::encoderCheck() {
	MycroftEncoder::Direction d = encoder.getDirection();
	if (d == MycroftEncoder::Direction::RIGHT) {
		if(optionIndex < maxIndex) {
            optionIndex++;
        }
	} else if (d == MycroftEncoder::Direction::LEFT) {
		if(optionIndex > 0) {
            optionIndex--;
        }
	}
}

void MycroftMenu::insertOptions() {
    menuOptions[0].option = OptionContainer::REBOOT;
    menuOptions[1].option = OptionContainer::WIFI;
    menuOptions[2].option = OptionContainer::SHUTDOWN;
    menuOptions[3].option = OptionContainer::TEST;
    menuOptions[4].option = OptionContainer::RESET;
    menuOptions[5].option = OptionContainer::EXIT;
}
