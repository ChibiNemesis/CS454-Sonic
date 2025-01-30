#include "FlowerAnimation.h"

void FlowerAnimation::SetCurrentFrame(int& curr)
{
	CurrentFrame = curr;
}

void FlowerAnimation::SetMaxFrames(int _frames)
{
	MaxFrames = _frames;
}

void FlowerAnimation::UpdateCurrentFrame()
{
	CurrentFrame++;
	if (CurrentFrame == MaxFrames)
		CurrentFrame = 0;
}

int FlowerAnimation::GetMaxFrames()
{
	return this->MaxFrames;
}

int FlowerAnimation::GetCurrentFrame()
{
	return this->CurrentFrame;
}
