#include "BaseProcessor.h"

bool BaseProcessor::contains(String value, String term) {
	return value.indexOf(term) > -1;
}

BaseProcessor::BaseProcessor(const char *cmdName) : cmdStr(cmdName) {
	cmdStr += '.';
}

bool BaseProcessor::tryProcess(String cmd) {
	if (contains(cmd, cmdStr.c_str())) {
		cmd.replace(cmdStr.c_str(), "");
		process(cmd);
		return true;
	}
	return false;
}
