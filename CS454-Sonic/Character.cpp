#include "Character.h"

Character::Character(int _x, int _y, AnimationFilm* _film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(_film);

	animation = new SonicAnimation("Sonic-Anim");
	animator = new SonicAnimator();
}

Character::Character()
{
	x = 0;
	y = 0;
	animation = new SonicAnimation("Sonic-Anim");
	animator = new SonicAnimator();
}

void Character::ChangeFilm(AnimationFilm* _film)
{
	SetAnimationFilm(_film);
}
