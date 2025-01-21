#include "Scroll.h"

int GetMapPixelWidth(Tilemap* map) {
	return map->getWidth() * TILE_WIDTH;
}
int GetMapPixelHeight(Tilemap* map) {
	return map->getHeight() * TILE_HEIGHT;
}

void Scroll(SDL_Rect* viewWin, int dx, int dy)
{
	viewWin->x += dx; viewWin->y += dy;
}
bool CanScrollHoriz(Tilemap* map, const SDL_Rect& viewWin, int dx) {
	return viewWin.x >= -dx &&
		(viewWin.x + viewWin.w + dx) <= GetMapPixelWidth(map);
}
bool CanScrollVert(Tilemap* map, const SDL_Rect& viewWin, int dy) {
	return viewWin.y >= -dy &&
		(viewWin.y + viewWin.h + dy) <= GetMapPixelHeight(map);
}

static void FilterScrollDistance(
	int viewStartCoord, // x or y
	int viewSize, // w or h
	int* d, // dx or dy
	int maxMapSize // w or h
) {
	auto val = *d + viewStartCoord;
	if (val < 0)
		*d = viewStartCoord; // cross low bound
	else
		if (viewSize >= maxMapSize)// fits entirely
			*d = 0;
		else
			if ((val + viewSize) >= maxMapSize) // cross upper bound
				*d = maxMapSize - (viewStartCoord + viewSize);
}

void FilterScroll(Tilemap* map, const SDL_Rect* viewWin, int* dx, int* dy) {
	FilterScrollDistance(
		viewWin->x, viewWin->w, dx, GetMapPixelWidth(map)
	);
	FilterScrollDistance(
		viewWin->y, viewWin->h, dy, GetMapPixelHeight(map)
	);
}

void ScrollWithBoundsCheck(
	Tilemap* map,
	SDL_Rect* viewWin,
	int dx,
	int dy
) {
	FilterScroll(map, viewWin, &dx, &dy);
	Scroll(viewWin, dx, dy);
}