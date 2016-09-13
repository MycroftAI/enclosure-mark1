#pragma once

#include <Arduino.h>

class BaseProcessor {
public:
	// Returns true if successfully processed command
	bool tryProcess(const String& cmd);

protected:
	BaseProcessor(const char *cmdName);
	virtual void process(const String& cmd) = 0;

private:
	String cmdStr;
};
