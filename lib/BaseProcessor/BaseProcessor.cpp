#include "BaseProcessor.h"

bool BaseProcessor::contains(String value, String term) {
    return value.indexOf(term) > -1;
}

BaseProcessor::BaseProcessor(const char *cmdName) : cmdName(cmdName) { }

bool BaseProcessor::tryProcess(String cmd) {
    if (contains(cmd, cmdName)) {
        cmd.replace(cmdName, "");
        process(cmd);
        return true;
    }
    return false;
}
