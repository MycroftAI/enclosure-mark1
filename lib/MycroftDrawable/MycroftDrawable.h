#pragma once

class MycroftDisplay;

/*
 * Represents anything that is drawn each frame.
 * Includes anything that draws to the matrix.
 */
class MycroftDrawable {
public:
	MycroftDrawable();
	void update(MycroftDisplay &display);
	void reset();
	
protected:
	/*
	 * Draws image to display.
	 * Returns milliseconds until the next update or -1 to indicate
	 * no subsequent update is necessary
	 */
	virtual int updateDrawable(MycroftDisplay &display) = 0;
	virtual void resetDrawable();
	
private:
	unsigned long nextUpdate;
};
