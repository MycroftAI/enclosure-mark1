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
#include "MycroftMenu.h"

MycroftMenu::MycroftMenu(int pinCS1, int pinWR, int pinDATA, int pinENC1, int pinENC2, int pinBUTTON) :
display(pinCS1, pinWR, pinDATA), encoder(pinENC1,pinENC2,pinBUTTON) {
    optionIndex = OptionContainer::ILLUM;
    currentState = MAIN;
    entered = false;
    shouldTest = false;
    choice = false;
    
    for (int i=0; i < OptionContainer::_COUNT; i++)
        menuOptions[i].option = (OptionContainer::Option)i;
}

void MycroftMenu::enter() {
    entered = true;
    display.clear();
    drawText();
    display.render();
}

bool MycroftMenu::isEntered() {
    return entered;
}

void MycroftMenu::exitMenu() {
    currentState = MAIN;    // when user re-enters, place them in the main menu
    entered = false;
    display.clear();
    display.render();
}

bool MycroftMenu::checkTest() {
    return shouldTest;
}

void MycroftMenu::finishTest() {
    shouldTest = false;
}

void MycroftMenu::updateOptionIndex(bool up) {
    if (up) {
        if (optionIndex+1 < OptionContainer::_COUNT)
            optionIndex++;
        choice = true;
    } else {
        if (optionIndex > 0)
            optionIndex--;
        choice = false;
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

void MycroftMenu::drawText() {
    if(currentState == MAIN) {
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::ILLUM:
                drawOption(false, "ILLUM", true);
                break;
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
            case OptionContainer::LEARN:
                drawOption(true, "LEARN", true);
                break;
            case OptionContainer::RESET:
                drawOption(true, "RESET", true);
                break;
            case OptionContainer::DEMO:
                drawOption(true, "DEMO", true);
                break;
            case OptionContainer::EXIT:
            default:
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
    else if (currentState == RESET_UNIT){
        if (choice){
            drawOption(true, "WIPE", false);
        }
        else{
            drawOption(false, "NO", true);
        }
    }
    else if (currentState == ALLOW_SSH){
        if (choice){
            drawOption(true, "ALLOW", false);
        }
        else{
            drawOption(false, "BLOCK", true);
        }
    }
    else if (currentState == LEARN){
        if (choice){
            drawOption(true, "ALLOW", false);
        }
        else{
            drawOption(false, "OFF", true);
        }
    }
}

void MycroftMenu::pressButton() {
    if (currentState == MAIN){
        switch(menuOptions[optionIndex].option) {
            case OptionContainer::REBOOT:
                Serial.println(F("unit.reboot"));
                exitMenu();
                return;
            case OptionContainer::WIFI:
                Serial.println(F("unit.setwifi"));
                exitMenu();
                return;
            case OptionContainer::SHUTDOWN:
                Serial.println(F("unit.shutdown"));
                exitMenu();
                return;
            case OptionContainer::TEST:
                shouldTest = true;
                exitMenu();
                return;
            case OptionContainer::SSH:
                currentState = ALLOW_SSH;
                choice = false;
                drawText();
                break;
            case OptionContainer::LEARN:
                currentState = LEARN;
                choice = false;
                drawText();
                break;
            case OptionContainer::RESET:
                currentState = RESET_UNIT;
                choice = false;
                drawText();
                break;
            case OptionContainer::ILLUM:
                currentState = BRIGHTNESS;
                drawText();
	        break;
            case OptionContainer::DEMO:
                Serial.println(F("mycroft.mark1.demo"));
                exitMenu();
                return;
            case OptionContainer::EXIT:
            default:
                exitMenu();
                return;
        }
    } else if (currentState == BRIGHTNESS) {
	currentState = MAIN;
	optionIndex = OptionContainer::ILLUM;
	drawText();
	return;
    }
    else if (currentState == ALLOW_SSH){
        if (choice){
            Serial.println(F("unit.enable-ssh"));
        }
        else {
            Serial.println(F("unit.disable-ssh"));
        }
        optionIndex = OptionContainer::SSH;
        exitMenu();
        return;
    }
    else if (currentState == LEARN){
        if (choice){
            Serial.println(F("unit.enable-learning"));
        }
        else {
            Serial.println(F("unit.disable-learning"));
        }
        optionIndex = OptionContainer::LEARN;
        exitMenu();
        return;
    }
    else if (currentState == RESET_UNIT){
        currentState = MAIN;
        optionIndex = OptionContainer::RESET;
        if (choice){
            Serial.println(F("unit.factory-reset"));
            exitMenu();
            return;
        }
        else {
            drawText();
        }
    }
}

void MycroftMenu::syncBrightness() {
    // Make the menu display a similar brightness as the 
    // eyes
    float fBright = float(MycroftEyes::instance().getBrightness())
			/ float(MAX_EYE_BRIGHTNESS);

    int iBrightDisplay = int(fBright*15)+1;
    display.setBrightness(iBrightDisplay);
}
