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
		
		//std::cout << "frameNo -> " << anim->GetCurrentFrame() <<"\n";
		//td::cout << "Max Frames -> " << anim->GetMaxFrames() << "\n";
	}
}

void CoinAnimator::SetAnimation(CoinAnimation * _anim)
{
	this->anim = _anim;
}
