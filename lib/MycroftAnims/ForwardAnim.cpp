#include "ForwardAnim.h"

void ForwardAnim::resetDrawable() {
	id = 0;
}

void ForwardAnim::updateId() {
	++id;
	if (id >= NUM_FRAMES / 4) {
		id = 0;
	}
}

byte ForwardAnim::getId() {
	return id;
}
