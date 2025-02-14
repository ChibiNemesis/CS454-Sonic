#include "Spike.h"

Spike::Spike() {
	IsCollided = false;
}

Spike::Spike(int _x, int _y, AnimationFilm* film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(film);
	IsCollided = false;
}

void Spike::Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const
{
	if (!IsCollided) {
		assert(currFilm->GetBitmap() != NULL);
		SDL_Rect Dest_Rect = dpyArea; //this can have NULL width and height

		SDL_BlitSurface(currFilm->GetBitmap(), &currFilm->GetFrameBox(frameNo), &dest, &Dest_Rect);
	}
}

void Spike::SetCollided(bool _collided)
{
	IsCollided = _collided;
	if (_collided) {
		OnCollided();
	}
}

bool Spike::GetCollided()
{
	return IsCollided;
}

void Spike::OnCollided()
{

}
