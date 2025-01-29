#pragma once
#include "Sprite.h"
#include "SonicAnimation.h"
#include"SonicAnimator.h"

class Character : public Sprite
{
public:
	Character(int _x, int _y, AnimationFilm* _film,const std::string& _typeId = "");
	Character();
	void ChangeFilm(AnimationFilm* _film);
private:
	SonicAnimation* animation = NULL;
	SonicAnimator* animator = NULL;
};

