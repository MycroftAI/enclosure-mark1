#include "MouthProcessor.h"
#include <MycroftMouth.h>

MouthProcessor::MouthProcessor(int pinCS1, int pinWR, int pinDATA) :
BaseProcessor("mouth."), mouth(pinCS1, pinWR, pinDATA) { }

void MouthProcessor::drawAnimation() {
    mouth.drawAnimation();
}

void MouthProcessor::process(String cmd) {
    if (contains(cmd, "reset")) {
        mouth.reset();
    }
    else if (contains(cmd, "talk")) {
        mouth.talk();
    }
    else if (contains(cmd, "listen")) {
        mouth.listen();
    }
    else if (contains(cmd, "think")) {
        mouth.think();
    }
    else if (contains(cmd, "smile")) {
        mouth.smile();
    }
    else if (contains(cmd, "text=")) {
        cmd.replace("text=", "");
        mouth.write(cmd.c_str());
    }
}
