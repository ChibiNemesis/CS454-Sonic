#include "Coin.h"

Coin::Coin(){
	IsCollected = false;
}

Coin::Coin(int _x, int _y, AnimationFilm* film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(film);
	IsCollected = false;
}

void Coin::Display(SDL_Surface& dest, const SDL_Rect& dpyArea) const
{
	if (!IsCollected) {
		assert(currFilm->GetBitmap() != NULL);
		SDL_Rect Dest_Rect = dpyArea; //this can have NULL width and height

		SDL_BlitSurface(currFilm->GetBitmap(), &currFilm->GetFrameBox(frameNo), &dest, &Dest_Rect);
	}
}

void Coin::SetCollected(bool _collected)
{
	IsCollected = _collected;
	if (_collected) {
		OnCollected();
	}
}

bool Coin::GetCollected()
{
	return IsCollected;
}

void Coin::OnCollected()
{
	
}
