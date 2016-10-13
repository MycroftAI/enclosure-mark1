#include "MouthProcessor.h"
#include "MycroftMouth.h"

MouthProcessor::MouthProcessor()
  : BaseProcessor("mouth")
{
}

void MouthProcessor::process(const String& cmd) {
	MycroftMouth&	mouth = MycroftMouth::instance();

	if (cmd.startsWith("reset")) {
		mouth.reset();
	} else if (cmd.startsWith("faketalk")) {
		mouth.fakeTalk();
	} else if (cmd.startsWith("talk")) {
		mouth.talk();
	} else if (cmd.startsWith("listen")) {
		mouth.listen();
	} else if (cmd.startsWith("think")) {
		mouth.think();
	} else if (cmd.startsWith("text=")) {
		// substring(5) to skip "text="
		mouth.write(cmd.substring(5).c_str());
	} else if (cmd.startsWith("icon")) {
		// substring(5) to skip "icon="
		mouth.showIcon(cmd.substring(5).c_str());
	} else if (cmd.startsWith("viseme=")) {
		// substring(5) to skip "viseme="
		mouth.viseme(cmd.substring(7));
	}

}
