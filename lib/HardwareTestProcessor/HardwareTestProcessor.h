#pragma once

#include "BaseProcessor.h"

class HardwareTester;
class MycroftEncoder;
class MycroftEyes;
class MycroftMouth;
class MycroftArduino;

class HardwareTestProcessor : public BaseProcessor {
public:
	HardwareTestProcessor(HardwareTester &tester);

	void process(const String& cmd);

private:
	HardwareTester &tester;
};
