#include "SystemProcessor.h"
#include "MycroftSystem.h"

SystemProcessor::SystemProcessor(MycroftSystem &system) : BaseProcessor("system."), system(system) { }

void SystemProcessor::process(String cmd) {
    if (contains(cmd, "reset")) {
        system.reset();
    }
    else if (contains(cmd, "mute")) {
        system.mute();
    }
    else if (contains(cmd, "unmute")) {
        system.unmute();
    }
    else if (contains(cmd, "blink=")) {
        cmd.replace("blink=", "");
        system.blink(cmd.toInt(), 500);
    }
}
