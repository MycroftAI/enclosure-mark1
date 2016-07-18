#include "CustomAnim.h"

void CustomAnim::addLoopFrames(int frame) {
	loopFrames[numLoopFrames] = frame;
	++numLoopFrames;
}

void CustomAnim::resetDrawable() {
	loopId = 0;
}

void CustomAnim::updateId() {
	++loopId;
	if (loopId > numLoopFrames)
		loopId = 0;
}

byte CustomAnim::getId() {
	return loopFrames[loopId];
}
