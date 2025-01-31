#pragma once
#include"Animator.h"
#include"SonicAnimation.h"

class SonicAnimator : public Animator
{
public:
	SonicAnimator();
	void Progress(timestamp_t currTime) override;

	void Start(const SonicAnimation& a, timestamp_t t) {
		animation = (SonicAnimation*)&a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		NotifyStarted();
	}

	void SetAnimation(SonicAnimation* _anim);
private:
	SonicAnimation* animation;
};

