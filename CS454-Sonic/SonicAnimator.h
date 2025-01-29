#pragma once
#include"Animator.h"
#include"SonicAnimation.h"

class SonicAnimator : public Animator
{
public:
	SonicAnimator();
	void Progress(timestamp_t currTime) override;

private:
	SonicAnimation* animation;
};

