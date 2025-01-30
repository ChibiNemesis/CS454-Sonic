#pragma once
#include"Animation.h"

class FlowerAnimation : public Animation
{
public:
	FlowerAnimation(std::string _id, int& _curr) : Animation(id), CurrentFrame(_curr), MaxFrames(0) {}

	Animation* Clone(void) const override
	{
		return new FlowerAnimation(id, CurrentFrame);
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

