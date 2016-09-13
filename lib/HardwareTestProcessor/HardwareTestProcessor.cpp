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
