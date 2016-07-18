#include "PingPongAnim.h"

void PingPongAnim::updateId() {
	id += dir;
	if (id <= 0) {
		dir = 1;
	} else if (id >= NUM_FRAMES / 4 - 1) {
		dir = -1;
	}
}

byte PingPongAnim::getId() {
	return id;
}
