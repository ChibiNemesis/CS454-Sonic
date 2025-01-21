#include "Sprite.h"

bool Sprite::CollisionCheck(const Sprite* s) const
{
	return false;
}

void Sprite::Display(SDL_Surface& dest, const SDL_Rect& dpyArea, const Clipper& clipper) const {
	SDL_Rect clippedBox;
	SDL_Rect dpyPos;
	dpyPos.w = NULL;
	dpyPos.h = NULL;
	if (clipper.Clip(GetBox(), dpyArea, &dpyPos, &clippedBox)) {
		SDL_Rect clippedFrame{
		frameBox.x + clippedBox.x,
		frameBox.y + clippedBox.y,
		clippedBox.w,
		clippedBox.h
		};

		SDL_BlitSurface(currFilm->GetBitmap(), &clippedFrame, &dest, &dpyPos);
		/*
		MaskedBlit(
			currFilm->GetBitmap(),
			clippedFrame,
			dest,
			dpyPos
		);*/
	}
}

//create gridlayer

const Sprite::Mover MakeSpriteGridLayerMover(GridLayer* gridLayer, Sprite* sprite) {
	return [gridLayer, sprite](const SDL_Rect& r, int* dx, int* dy) {
		// the r is actually always the sprite->GetBox():
		assert(r.x == sprite->GetBox().x &&
			r.y == sprite->GetBox().y &&
			r.w == sprite->GetBox().w &&
			r.h == sprite->GetBox().h
		);
		gridLayer->FilterGridMotion(r, dx, dy);
		if (*dx || *dy)
			sprite->SetHasDirectMotion(true).Move(*dx, *dy).SetHasDirectMotion(false);
		};
};