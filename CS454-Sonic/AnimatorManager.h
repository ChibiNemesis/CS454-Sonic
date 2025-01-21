#pragma once
#include<set>
#include<cassert>
#include"Animator.h"

class AnimatorManager {
private:
	std::set<Animator*> running, suspended;
	//static AnimatorManager singleton;
	AnimatorManager(void) = default;
	AnimatorManager(const AnimatorManager&) = delete;
	AnimatorManager(AnimatorManager&&) = delete;
public:
	void Register(Animator* a);
	void Cancel(Animator* a);
	void MarkAsRunning(Animator* a);
	void MarkAsSuspended(Animator* a);
	void Progress(timestamp_t currTime);
	//static auto GetSingleton(void) -> AnimatorManager&;
	//static auto GetSingletonConst(void) -> const AnimatorManager&;
};

