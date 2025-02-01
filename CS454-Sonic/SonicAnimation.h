#pragma once
#include "Animation.h"

#define SONIC_SPRITE_DELAY 400

class SonicAnimation : public Animation
{
public:
	SonicAnimation(std::string _id, int& _curr);
	Animation* Clone(void) const override
	{
		return new SonicAnimation(id, CurrentFrame);
	}

	void SetCurrentFrame(int& curr);
	void SetMaxFrames(int _frames);

	void UpdateCurrentFrame();

	int GetMaxFrames();
	int GetCurrentFrame();

private:
	int& CurrentFrame;
	int MaxFrames;
};

