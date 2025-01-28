#include "CoinAnimation.h"

void CoinAnimation::SetCurrentFrame(int& curr)
{
	CurrentFrame = curr;
}

void CoinAnimation::SetMaxFrames(int _frames)
{
	MaxFrames = _frames;
}

int CoinAnimation::GetMaxFrames()
{
	return this->MaxFrames;
}

int CoinAnimation::GetCurrentFrame()
{
	return this->CurrentFrame;
}
