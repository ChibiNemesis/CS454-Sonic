#include "Clipper.h"

bool clip_rect(const SDL_Rect& r, const SDL_Rect& area, SDL_Rect* result) {
	return _clip_rect(
		r.x,
		r.y,
		r.w,
		r.h,
		area.x,
		area.y,
		area.w,
		area.h,
		&result->x,
		&result->y,
		&result->w,
		&result->h
	);
}