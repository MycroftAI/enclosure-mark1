#pragma once

#include "MouthAnim.h"

class PingPongAnim : public MouthAnim {
public:
	using MouthAnim::MouthAnim;

private:
	void updateId() override;
	byte getId() override;

	byte id = 0;
	int8_t dir = 1;
};
