#include "ArduinoProcessor.h"
#include "MycroftArduino.h"

ArduinoProcessor::ArduinoProcessor(MycroftArduino &arduino)
  : BaseProcessor("system"), arduino(arduino)
{
}

void ArduinoProcessor::setup() {
	arduino.setup();
}

void ArduinoProcessor::process(const String& cmd) {
	if (cmd.startsWith("reset")) {
		arduino.reset();
	} else if (cmd.startsWith("mute")) {
		arduino.mute();
	} else if (cmd.startsWith("unmute")) {
		arduino.unmute();
	} else if (cmd.startsWith("blink=")) {
		arduino.blink(cmd.substring(6).toInt(), 500);
	} else if (cmd.startsWith("ping")) {
		Serial.println(F("Version: "));
		Serial.println(F(ENCLOSURE_VERSION_STRING));
	}
}
