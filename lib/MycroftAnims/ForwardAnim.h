#pragma once

#include "MouthAnim.h"

class ForwardAnim : public MouthAnim {
public:
	
	using MouthAnim::MouthAnim;
	
private:
	void resetDrawable() override;
	void updateId() override;
	byte getId() override;
	
	byte id = 0;

};
