#pragma once
#include"Tilemap.h"
#include"SDL.h"

#define TILESET_WIDTH 264 // row = 16 tiles
#define TILESET_HEIGHT 264 // col = 16 tiles

int GetMapPixelWidth(Tilemap* map);
int GetMapPixelHeight(Tilemap* map);

void Scroll(SDL_Rect* viewWin, int dx, int dy);
bool CanScrollHoriz(Tilemap* map, const SDL_Rect& viewWin, int dx);
bool CanScrollVert(Tilemap* map, const SDL_Rect& viewWin, int dy);

static void FilterScrollDistance(
	int viewStartCoord, // x or y
	int viewSize, // w or h
	int* d, // dx or dy
	int maxMapSize // w or h
);

void FilterScroll(Tilemap* map, const SDL_Rect* viewWin, int* dx, int* dy);

void ScrollWithBoundsCheck(
	Tilemap* map,
	SDL_Rect* viewWin,
	int dx,
	int dy
);

