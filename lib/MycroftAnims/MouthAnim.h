#pragma once

#include "MycroftDisplay.h"
#include "MycroftDrawable.h"

class MouthAnim : public MycroftDrawable {
public:
	template<size_t N>
	MouthAnim(const char (&ANIM)[N][16], const int MS_PER_FRAME = 70);

protected:
	const char (*ANIM)[16];
	const size_t NUM_FRAMES;

private:
	int updateDrawable(MycroftDisplay &display) override;
	virtual void updateId() = 0;
	virtual byte getId() = 0;

	const int MS_PER_FRAME;
};

#include "MouthAnim.inl"
