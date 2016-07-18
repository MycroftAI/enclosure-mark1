#include "MycroftDrawable.h"
#include "MycroftDisplay.h"

MycroftDrawable::MycroftDrawable() {
	reset();
}

void MycroftDrawable::update(MycroftDisplay &display) {
	const unsigned long MILLIS = millis();
	if (nextUpdate == 0)
		return;
	if (nextUpdate < MILLIS)
	{
		nextUpdate = MILLIS + updateDrawable(display);
		display.render();
	}
}

void MycroftDrawable::reset() {
	nextUpdate = 1;
	resetDrawable();
}

void MycroftDrawable::resetDrawable() { }
