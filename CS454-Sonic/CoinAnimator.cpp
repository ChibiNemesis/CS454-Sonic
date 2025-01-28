#include "CoinAnimator.h"

void CoinAnimator::Finish(bool isForced)
{
	state = ANIMATOR_FINISHED;
}

CoinAnimator::CoinAnimator()
{}

void CoinAnimator::Progress(timestamp_t currTime)
{
	if (state == ANIMATOR_RUNNING) {
		if((currTime - lastTime) > COIN_WAITING_TIME){
			lastTime = currTime;
			anim->UpdateCurrentFrame();
		}
	}
}

void CoinAnimator::SetAnimation(CoinAnimation * _anim)
{
	this->anim = _anim;
}
