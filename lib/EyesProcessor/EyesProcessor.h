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
	MycroftEyes::Side toSide(const char SIDE_CHAR);
	bool checkEyeAnim(String cmd, String term, MycroftEyes::Animation anim);

	MycroftEyes &eyes;
};
