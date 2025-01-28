#pragma once
#include "Animator.h"
#include "CoinAnimation.h"

#define COIN_WAITING_TIME 130

class CoinAnimator :
    public Animator
{
protected:
    CoinAnimation* anim = NULL;
public:
    CoinAnimator();
    void Progress(timestamp_t currTime) override;

	void Start(const CoinAnimation& a, timestamp_t t) {
		anim = (CoinAnimation *)&a;
		lastTime = t;
		state = ANIMATOR_RUNNING;
		NotifyStarted();
	}
    void SetAnimation(CoinAnimation* _anim);
	void Finish(bool isForced = false);
};

