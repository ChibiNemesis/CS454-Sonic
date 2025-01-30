#pragma once
#include"FlowerAnimation.h"
#include"Animator.h"

#define FLOWER_WAITING_TIME 300

class FlowerAnimator: public Animator
{
public:
	FlowerAnimator();

	void Progress(timestamp_t currTime) override;

	void Start(const FlowerAnimation& a, timestamp_t t) {
		anim = (FlowerAnimation*)&a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		NotifyStarted();
	}
	void SetAnimation(FlowerAnimation* _anim);
private:
	FlowerAnimation* anim = NULL;
};

