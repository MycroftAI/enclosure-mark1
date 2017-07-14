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
#include "HardwareTestProcessor.h"
#include "HardwareTester.h"
#include "MycroftMouth.h"
#include "MycroftEyes.h"
#include "MycroftArduino.h"
#include "MycroftEncoder.h"


HardwareTestProcessor::HardwareTestProcessor(HardwareTester &tester)
   : BaseProcessor("test"), tester(tester)
{
}

void HardwareTestProcessor::process(const String& cmd) {
	if (cmd.startsWith("begin")) {
		tester.run(MycroftEncoder::instance(), MycroftEyes::instance(),
			MycroftMouth::instance(), MycroftArduino::instance());
	}
}
