#include "MouthProcessor.h"
#include "MycroftMouth.h"

MouthProcessor::MouthProcessor(MycroftMouth &mouth) :
BaseProcessor("mouth"), mouth(mouth) { }

void MouthProcessor::process(String cmd) {
	if (contains(cmd, "reset")) {
		mouth.reset();
	} else if (contains(cmd, "faketalk")) {
		mouth.fakeTalk();
	} else if (contains(cmd, "talk")) {
		mouth.talk();
	} else if (contains(cmd, "listen")) {
		mouth.listen();
	} else if (contains(cmd, "think")) {
		mouth.think();
	} else if (contains(cmd, "smile")) {
		mouth.smile();
	} else if (contains(cmd, "text=")) {
		cmd.replace("text=", "");
		mouth.write(cmd.c_str());
	} else if (contains(cmd, "icon")) {
		cmd.replace("icon=", "");
		mouth.showIcon(cmd.c_str());
	} else if (contains(cmd, "viseme=")) {
		cmd.replace("viseme=", "");
		mouth.viseme(cmd);
	}

}
