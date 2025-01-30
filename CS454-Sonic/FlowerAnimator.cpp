#include "FlowerAnimator.h"

FlowerAnimator::FlowerAnimator()
{
}

void FlowerAnimator::Progress(timestamp_t currTime)
{
	if (state == ANIMATOR_RUNNING) {
		if ((currTime - lastTime) > FLOWER_WAITING_TIME) {
			lastTime = currTime;
			anim->UpdateCurrentFrame();
		}
	}
}

void FlowerAnimator::SetAnimation(FlowerAnimation* _anim)
{
	this->anim = _anim;
}
