#pragma once

#include "BaseProcessor.h"

class HardwareTester;
class MycroftEncoder;
class MycroftEyes;
class MycroftMouth;
class MycroftArduino;

class HardwareTestProcessor : public BaseProcessor {
public:
	HardwareTestProcessor(HardwareTester &tester, MycroftEncoder &encoder,
			MycroftEyes &eyes, MycroftMouth &mouth, MycroftArduino &arduino);
	
	void process(String cmd);

private:
	HardwareTester &tester;
	MycroftEncoder &encoder;
	MycroftEyes &eyes;
	MycroftMouth &mouth;
	MycroftArduino &arduino;
};
