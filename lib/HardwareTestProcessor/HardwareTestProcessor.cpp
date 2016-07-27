#include "HardwareTestProcessor.h"
#include "HardwareTester.h"

HardwareTestProcessor::HardwareTestProcessor(HardwareTester &tester,
		MycroftEncoder &encoder, MycroftEyes &eyes, MycroftMouth &mouth,
		MycroftArduino &arduino) :
BaseProcessor("test"), tester(tester), encoder(encoder), eyes(eyes), mouth(mouth), arduino(arduino) { }

void HardwareTestProcessor::process(String cmd) {
	if (contains(cmd, "begin")) {
		tester.run(encoder, eyes, mouth, arduino);
	}
}
