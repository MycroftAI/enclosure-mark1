#include "MycroftMenu.h"

MycroftMenu::MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON) :
display(pinCS1, pinWR, pinDATA), encoder(pinENC1,pinENC2,pinBUTTON) {
    optionIndex = 0;	// Illum
    currentState = MAIN;
    entered = false;
    shouldTest = false;
    resetVal = false;
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
        resetVal = true;
    } else {
        optionIndex--;
        resetVal = false;
    }
}

MycroftMenu::menuState MycroftMenu::getCurrentMenu() {
    return currentState;
}

void MycroftMenu::drawOption(bool leftArrow, const String& option, bool rightArrow) {
    display.clear();
    display.drawTextCentered(option, true);
    if(leftArrow)
        display.drawText("<", 0, true);
    if(rightArrow)
        display.drawText(">", 30, true);

    display.render();
}

void MycroftMenu::run() {
    if(currentState == MAIN) {
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::REBOOT:
                drawOption(true, "REBOOT", true);
                break;
            case OptionContainer::WIFI:
                drawOption(true, "WI-FI", true);
                break;
            case OptionContainer::SHUTDOWN:
                drawOption(true, "OFF", true);
                break;
            case OptionContainer::TEST:
                drawOption(true, "TEST", true);
                break;
            case OptionContainer::SSH:
                drawOption(true, "SSH", true);
                break;
            case OptionContainer::RESET:
                drawOption(true, "RESET", true);
                break;
            case OptionContainer::ILLUM:
                drawOption(false, "ILLUM", true);
                break;
            case OptionContainer::DEMO:
                drawOption(true, "DEMO", true);
                break;
            case OptionContainer::EXIT:
                drawOption(true, "EXIT", false);
                break;
        }
    }
    else if (currentState == BRIGHTNESS) {
	String	strBright(MycroftEyes::instance().getBrightness());
	if (MycroftEyes::instance().getBrightness() > 29)
		strBright = "AUTO";

	drawOption(false, strBright, false);
    }
    else if (currentState == RESETMODE){
        if(resetVal){
            drawOption(true, "YES", false);
        }
        else{
            drawOption(false, "NO", true);
        }
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
            case OptionContainer::SSH:
                Serial.println(F("unit.enable-ssh"));
                entered = false;
                break;
            case OptionContainer::RESET:
                resetVal = false;
                currentState = RESETMODE;
                break;
            case OptionContainer::ILLUM:
                currentState = BRIGHTNESS;
		        break;
            case OptionContainer::DEMO:
                entered = false;
                Serial.println(F("mycroft.mark1.demo"));
                break;
            case OptionContainer::EXIT:
                entered = false;
                break;
        }
//        optionIndex = 2;
    } else if (currentState == BRIGHTNESS) {
        currentState = MAIN;
	optionIndex = 0;
	run();
	return;
    }
//          optionIndex = 3
    else if (currentState == RESETMODE){
        if(resetVal){
            Serial.println(F("unit.factory-reset"));
            entered = false;
            return;
        }
        else {
            resetVal = false;
            currentState = MAIN;
            optionIndex = 6;
            run();
            return;
        }
    }
    display.clear();
    display.render();
}

void MycroftMenu::insertOptions() {
    // This defines the order of the menu items
    menuOptions[0].option = OptionContainer::ILLUM;
    menuOptions[1].option = OptionContainer::WIFI;
    menuOptions[2].option = OptionContainer::REBOOT;
    menuOptions[3].option = OptionContainer::SHUTDOWN;
    menuOptions[4].option = OptionContainer::TEST;
    menuOptions[5].option = OptionContainer::SSH;
    menuOptions[6].option = OptionContainer::RESET;
    menuOptions[7].option = OptionContainer::DEMO;
    menuOptions[8].option = OptionContainer::EXIT;
}

void MycroftMenu::syncBrightness() {
    // Make the menu display a similar brightness as the 
    // eyes
    float fBright = float(MycroftEyes::instance().getBrightness())
			/ float(MAX_EYE_BRIGHTNESS);

    int iBrightDisplay = int(fBright*15)+1;
    display.setBrightness(iBrightDisplay);
}
