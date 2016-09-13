#pragma once

#include "BaseProcessor.h"
#include "MycroftMouth.h"

class MouthProcessor : public BaseProcessor {
public:
	MouthProcessor();

private:
	void process(const String& cmd);
};
