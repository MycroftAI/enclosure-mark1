#pragma once

#include "BaseProcessor.h"
#include "MycroftEyes.h"

class EyesProcessor : public BaseProcessor {
public:
	EyesProcessor();
	void setup();

private:
	void updateEyesColor(long code);
	void process(const String& cmd);
	MycroftEyes::Side toSide(const char SIDE_CHAR);
	bool checkEyeAnim(const String& cmd, const String& term, MycroftEyes::Animation anim);
};
