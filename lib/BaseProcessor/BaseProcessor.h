#pragma once

#include <Arduino.h>

class BaseProcessor {
public:
	// Returns true if successfully processed command
	bool tryProcess(String cmd);

protected:
	BaseProcessor(const char *cmdName);
	bool contains(String value, String term);
	virtual void process(String cmd) = 0;

private:
	String cmdStr;
};
