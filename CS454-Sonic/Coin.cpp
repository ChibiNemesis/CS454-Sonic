#include "Coin.h"

Coin::Coin(){
	IsCollected = false;

	animation = new CoinAnimation(this->typeId+"-anim", (int&)this->frameNo);
	animation->SetMaxFrames(this->currFilm->GetTotalFrames());
	animator = new CoinAnimator();
	animator->Start(*animation, GetSystemTime());
}

Coin::Coin(int _x, int _y, AnimationFilm* film, const std::string& _typeId)
{
	x = _x;
	y = _y;
	std::string copy = _typeId;
	typeId = copy;
	SetAnimationFilm(film);
	IsCollected = false;

	animation = new CoinAnimation(this->typeId + "-anim", (int&)this->frameNo);
	animation->SetMaxFrames(this->currFilm->GetTotalFrames());
	animator = new CoinAnimator();
	animator->Start(*animation, GetSystemTime());
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

void Coin::Progress(timestamp_t _time)
{
	animator->Progress(_time);
}

void Coin::DestroyCoin()
{
	animator->Finish(true);
}

void Coin::OnCollected()
{
	
}
