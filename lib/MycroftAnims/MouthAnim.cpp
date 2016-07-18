#include "MouthAnim.h"

int MouthAnim::updateDrawable(MycroftDisplay &display) {
	display.drawFramePgm(getId(), ANIM);
	updateId();
	return MS_PER_FRAME;
}
