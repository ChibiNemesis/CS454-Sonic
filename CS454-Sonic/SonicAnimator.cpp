#include "SonicAnimator.h"

SonicAnimator::SonicAnimator()
{}

void SonicAnimator::Progress(timestamp_t currTime)
{
	if (state == ANIMATOR_RUNNING) {
		if (currTime - lastTime > SONIC_SPRITE_DELAY) {
			lastTime = currTime;
			animation->UpdateCurrentFrame();
		}
	}
}

void SonicAnimator::SetAnimation(SonicAnimation* _anim)
{
	animation = _anim;
}
