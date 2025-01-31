#include "Character.h"

Character::Character(int _x, int _y, AnimationFilm* _film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(_film);

	animation = new SonicAnimation("Sonic-Anim", (int&)frameNo);
	animation->SetMaxFrames(currFilm->GetTotalFrames());
	animator = new SonicAnimator();

	animator->Start(*animation,GetSystemTime());
}

Character::Character()
{
	x = 0;
	y = 0;
	animation = new SonicAnimation("Sonic-Anim", (int&)frameNo);
	animator = new SonicAnimator();
}

void Character::ChangeFilm(AnimationFilm* _film)
{
	SetAnimationFilm(_film);
}

AnimationFilm* Character::GetCurrentFilm()
{
	return this->currFilm;
}

void Character::Progress(timestamp_t _time)
{
	animator->Progress(_time);
}
