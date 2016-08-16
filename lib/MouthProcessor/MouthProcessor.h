#pragma once

#include "BaseProcessor.h"
#include "MycroftMouth.h"

class MouthProcessor : public BaseProcessor {
public:
	MouthProcessor(MycroftMouth &mouth);

private:
	void process(String cmd);
	MycroftMouth &mouth;
};
