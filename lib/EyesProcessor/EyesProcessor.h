#pragma once

#include "BaseProcessor.h"
#include "MycroftEyes.h"

class EyesProcessor : public BaseProcessor {
public:
	EyesProcessor(MycroftEyes &eyes);
	void setup();
	void updateAnimation();

private:
	void updateEyesColor(long code);
	void process(String cmd);

	MycroftEyes &eyes;
};
