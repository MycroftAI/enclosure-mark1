#pragma once

#include <Arduino.h>
#include "MouthAnim.h"

class CustomAnim : public MouthAnim {
public:
	using MouthAnim::MouthAnim;

	template<typename... Args>
	void addLoopFrames(int frame, Args... args);
	void addLoopFrames(int frame);

private:
	void resetDrawable() override;
	void updateId();
	byte getId();

	static const byte MAX_LOOP_FRAMES = 10;
	byte loopFrames[MAX_LOOP_FRAMES];
	byte numLoopFrames = 0;
	byte loopId = 0;
};

template<typename... Args>
void CustomAnim::addLoopFrames(int frame, Args... args)
{
	addLoopFrames(frame);
	addLoopFrames(args...);
}
