#include "BaseProcessor.h"

BaseProcessor::BaseProcessor(const char *cmdName) : cmdStr(cmdName) {
	cmdStr += '.';
}

bool BaseProcessor::tryProcess(const String& cmd) {
	if (cmd.startsWith(cmdStr)) {
		// change "abc.def" to just "def"
		process(cmd.substring(cmdStr.length()));
		return true;
	}
	return false;
}
