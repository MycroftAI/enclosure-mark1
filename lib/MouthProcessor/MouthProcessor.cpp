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
