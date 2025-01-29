#pragma once
#include "Animation.h"

class SonicAnimation : public Animation
{
public:
	SonicAnimation(std::string _id);
	Animation* Clone(void) const override
	{
		return new SonicAnimation(id);
	}
};

