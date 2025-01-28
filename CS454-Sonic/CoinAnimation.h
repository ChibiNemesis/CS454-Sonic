#pragma once
#include "Animation.h"
class CoinAnimation :
    public Animation
{
public:
	CoinAnimation(std::string _id, int& _curr) : Animation(id), CurrentFrame(_curr), MaxFrames(0){}
	Animation* Clone(void) const override
	{
		return new CoinAnimation(id, CurrentFrame);
	}

	void SetCurrentFrame(int& curr);
	void SetMaxFrames(int _frames);

	void UpdateCurrentFrame() {
		CurrentFrame++;
		if (CurrentFrame == MaxFrames)
			CurrentFrame = 0;
	}

	int GetMaxFrames();
	int GetCurrentFrame();
private:
	int &CurrentFrame;
	int MaxFrames;
};

