#pragma once
#include"Sprite.h"
#include"FlowerAnimation.h"
#include"FlowerAnimator.h"
#include"SystemClock.h"

class Flower: public Sprite
{
public:
	Flower();
	Flower(int _x, int _y, AnimationFilm* film, const std::string& _typeId = "");
	void Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const;
	void Progress(timestamp_t _time);

private:
	FlowerAnimation* animation = NULL;
	FlowerAnimator* animator = NULL;
};

