#include "SystemProcessor.h"
#include <MycroftArduino.h>

SystemProcessor::SystemProcessor(MycroftArduino &arduino) : BaseProcessor("arduino."), arduino(arduino) { }

void SystemProcessor::process(String cmd) {
    if (contains(cmd, "reset")) {
        arduino.reset();
    }
    else if (contains(cmd, "mute")) {
        arduino.mute();
    }
    else if (contains(cmd, "unmute")) {
        arduino.unmute();
    }
    else if (contains(cmd, "blink=")) {
        cmd.replace("blink=", "");
        arduino.blink(cmd.toInt(), 500);
    }
}
