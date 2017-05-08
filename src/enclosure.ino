#include "ClickEncoder.h"
#include "TimerOne.h"

#include "MycroftArduino.h"
#include "MycroftMouth.h"
#include "MycroftEyes.h"
#include "MycroftEncoder.h"
#include "MycroftMenu.h"
#include "HardwareTester.h"

#include "MouthProcessor.h"
#include "EyesProcessor.h"
#include "ArduinoProcessor.h"
#include "WeatherProcessor.h"
#include "HardwareTestProcessor.h"

#define BUTTON_PIN 2
#define SPEAKER_PIN 4
#define ENC1_PIN 14
#define ENC2_PIN 15

#define EYES_PIN 3
#define EYES_SIZE 24
#define EYES_TYPE NEO_GRB + NEO_KHZ800

#define MOUTH_CS1 7
#define MOUTH_WR 8
#define MOUTH_DATA 9
#define MOUTH_PLATES 4

#define SEC                             1000

#define MENU_HOLD_TIME			(2*SEC)
#define MENU_TIME_OUT		        (30*SEC)


// Uncomment any of these defines when debugging
// #define DEBUG_BUTTON_STATE


// Must be initialized first
MycroftArduino	arduino(SPEAKER_PIN);
MycroftArduino*	MycroftArduino::m_instance = &arduino;

MycroftEncoder	encoder(ENC1_PIN, ENC2_PIN, BUTTON_PIN);
MycroftEncoder*	MycroftEncoder::m_instance = &encoder;

MycroftEyes	eyes(EYES_SIZE, EYES_PIN, EYES_TYPE);
MycroftEyes*	MycroftEyes::m_instance = &eyes;

MycroftMouth	mouth(MOUTH_CS1, MOUTH_WR, MOUTH_DATA, MOUTH_PLATES);
MycroftMouth*	MycroftMouth::m_instance = &mouth;

MycroftMenu	menu(MOUTH_CS1, MOUTH_WR, MOUTH_DATA, ENC1_PIN, ENC2_PIN, BUTTON_PIN);
HardwareTester	hardwareTester;

MouthProcessor		mouthProcessor;
EyesProcessor		eyesProcessor;
ArduinoProcessor	arduinoProcessor(arduino);
WeatherProcessor	weatherProcessor;
HardwareTestProcessor	hardwareTestProcessor(hardwareTester);
BaseProcessor*		processors[] = {
				&mouthProcessor,
				&eyesProcessor,
				&arduinoProcessor,
				&weatherProcessor,
				&hardwareTestProcessor
			};

static void timerIsr() {
	encoder.isr();
}

void initSerial() {
	Serial.begin(9600);
	while (!Serial)
            ;
	Serial.flush();
	Serial.println(F("Mycroft Mark 1 v" ENCLOSURE_VERSION_STRING " - Connected"));
}

void setup() {
	initSerial();
	eyesProcessor.setup();
	arduinoProcessor.setup();
	Timer1.initialize(1000);
	Timer1.attachInterrupt(timerIsr);
}

static void scrollVolume(MycroftEncoder::Direction d) {
	if (d == MycroftEncoder::Direction::RIGHT) {
		Serial.println(F("volume.up"));
	} else if (d == MycroftEncoder::Direction::LEFT) {
		Serial.println(F("volume.down"));
	}
}

static void scrollMenu(MycroftEncoder::Direction d) {
	if (d == MycroftEncoder::Direction::RIGHT) {
		menu.updateOptionIndex(false);
	} else if (d == MycroftEncoder::Direction::LEFT) {
		menu.updateOptionIndex(true);
	}
	menu.drawText();
}

static void scrollBrightness(MycroftEncoder::Direction d) {
	if (d == MycroftEncoder::Direction::RIGHT) {
		eyes.incrementBrightness(true);
		menu.syncBrightness();
		menu.drawText();
	} else if (d == MycroftEncoder::Direction::LEFT) {
		eyes.incrementBrightness(false);
		menu.syncBrightness();
		menu.drawText();
	}
}

static bool		bWasClicked = false;
static bool		bHasTriggered = false;
static bool		bButtonPressed = false;
static unsigned long	timeReleased = 0;
static unsigned long    timeLastInteract = 0;

static void handleButton() {

	// Button is a little finicky.  Require a state be held for over
        // 20ms before it sticks.
	//
	if (encoder.getFramesHeld() > 5)
	{
#ifdef DEBUG_BUTTON_STATE
		Serial.println(F("button: held > 5"));
#endif
		if (!bButtonPressed)
		{
			bButtonPressed = true;
			bHasTriggered = false;
		}
		timeReleased = 0;	// reset for next usage
	}
	else if (!encoder.isClicked() && bButtonPressed)
	{
#ifdef DEBUG_BUTTON_STATE
		Serial.println(F("button: not clicked"));
#endif
		if (timeReleased == 0)
		{
#ifdef DEBUG_BUTTON_STATE
			Serial.println(F("button: start debounce timer"));
#endif
			timeReleased = millis();
		}
		else if (millis()-timeReleased > 5)
		{
#ifdef DEBUG_BUTTON_STATE
			Serial.println(F("button: released"));
#endif
			bButtonPressed = false;
			timeReleased = 0;	// reset for next usage
		}
	}

	if (bButtonPressed) {
#ifdef DEBUG_BUTTON_STATE
		Serial.println(F("Button: pressed"));
#endif
		if (encoder.getFramesHeld() > 0 && encoder.getFramesHeld() < 10) {
#ifdef DEBUG_BUTTON_STATE
			Serial.println(F("Button: triggered"));
#endif
			bHasTriggered = false;
		}
		else if (encoder.getFramesHeld() >= 10 && !bHasTriggered) {
#ifdef DEBUG_BUTTON_STATE
			Serial.println(F("Button: triggered"));
#endif
			bHasTriggered = true;
			if (menu.isEntered()) {
#ifdef DEBUG_BUTTON_STATE
				Serial.println(F("Button: menu click"));
#endif
				bWasClicked = false;	// cancel generating message on button up
				menu.pressButton();
			}
			else
				bWasClicked = true;
		}
		if (encoder.getFramesHeld() >= MENU_HOLD_TIME) {
			bWasClicked = false;	// cancel generating message on button up
			timeLastInteract = millis();
                        eyes.on();
			menu.enter();
		}
	}
	else if (bWasClicked)
	{
#ifdef DEBUG_BUTTON_STATE
		Serial.println(F("Button: wasClicked"));
#endif
		bWasClicked = false;
		bHasTriggered = false;		// reset for next time
		Serial.println(F("mycroft.stop"));
	}
}

void loop() {

	// Read the command string off the serial line.
	// Our convention is: class.command=param
	// Ex:
	//	mouth.reset
	//	mouth.text=abc
	//	eyes.blink=l
	//
	if (Serial.available() > 0) {
		String cmd = Serial.readStringUntil('\n');
		Serial.flush();
		Serial.print(F("Command: "));
		Serial.println(cmd);

		for (BaseProcessor *i : processors) {
                        if ((i == &mouthProcessor || i == &weatherProcessor)
                            && menu.isEntered())
                            continue;   // no mouth movements while MENU is showing
                        
			if (i->tryProcess(cmd))
				break;
                }
	}

	// Until more serial data becomes available, just
	// loop and do and processing for animation or
	// the encoder
	//
	while (Serial.available() <= 0) {
		handleButton();
		if (menu.checkTest()) {
			hardwareTester.run(encoder, eyes, mouth, arduino);
			menu.finishTest();
		}
		
                MycroftEncoder::Direction dir = encoder.getDirection();
		if (menu.isEntered()) {
                        if (dir == MycroftEncoder::Direction::NONE) {
                            // check for timeout
                            if (millis()-timeLastInteract > MENU_TIME_OUT) {
                                // Auto-exit the menu
                                menu.exitMenu();
                            }
                        } else {
                            timeLastInteract = millis();
                            
                            if (menu.getCurrentMenu() == MycroftMenu::MAIN
                                || menu.getCurrentMenu() == MycroftMenu::RESET_UNIT
                                || menu.getCurrentMenu() == MycroftMenu::ALLOW_SSH) {
                                    scrollMenu(dir);
                            } else if (menu.getCurrentMenu() == MycroftMenu::BRIGHTNESS){
                                    scrollBrightness(dir);
                            }
                        }
		}
		else {
			scrollVolume(dir);
			eyes.updateAnimation();
			mouth.update();
			if (mouth.getState() != MycroftMouth::NONE && eyes.currentAnim == MycroftEyes::SPIN) {
				eyes.reset();
			}
		}
	}
}
