#include "Flower.h"

Flower::Flower()
{
		x = 0;
		y = 0;
		typeId = "Default-Id";
}

Flower::Flower(int _x, int _y, AnimationFilm* film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(film);

	animation = new FlowerAnimation(this->typeId + "-anim", (int&)this->frameNo);
	animation->SetMaxFrames(this->currFilm->GetTotalFrames());
	animator = new FlowerAnimator();
	animator->Start(*animation, GetSystemTime());
}

void Flower::Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const
{
	assert(currFilm->GetBitmap() != NULL);
	SDL_Rect Dest_Rect = dpyArea;

	SDL_BlitSurface(currFilm->GetBitmap(), &currFilm->GetFrameBox(frameNo), &dest, &Dest_Rect);
}

void Flower::Progress(timestamp_t _time)
{
	animator->Progress(_time);
}
