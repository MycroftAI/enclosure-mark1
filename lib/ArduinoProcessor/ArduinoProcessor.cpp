#include "ArduinoProcessor.h"
#include "MycroftArduino.h"

ArduinoProcessor::ArduinoProcessor(MycroftArduino &arduino) :
BaseProcessor("system"), arduino(arduino) { }

void ArduinoProcessor::setup() {
	arduino.setup();
}

void ArduinoProcessor::process(String cmd) {
	if (contains(cmd, "reset")) {
		arduino.reset();
	} else if (contains(cmd, "mute")) {
		arduino.mute();
	} else if (contains(cmd, "unmute")) {
		arduino.unmute();
	} else if (contains(cmd, "blink=")) {
		cmd.replace("blink=", "");
		arduino.blink(cmd.toInt(), 500);
	}
}
