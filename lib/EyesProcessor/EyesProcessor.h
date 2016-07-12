#pragma once

#include "BaseProcessor.h"
#include "MycroftEyes.h"

class EyesProcessor : public BaseProcessor {
public:
	EyesProcessor(MycroftEyes &eyes);
	void setup();

private:
	void updateEyesColor(long code);
	void process(String cmd);

	MycroftEyes &eyes;
};
