#include "MycroftDrawable.h"
#include "MycroftDisplay.h"

MycroftDrawable::MycroftDrawable() { }

void MycroftDrawable::update(MycroftDisplay &display) {
	if (msTillUpdate < 0)
		return;
	if (--msTillUpdate <= 0)
	{
		msTillUpdate = updateDrawable(display);
		display.render();
	}
}

void MycroftDrawable::reset() {
	msTillUpdate = 0;
	resetDrawable();
}

void MycroftDrawable::resetDrawable() { }
