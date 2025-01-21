#pragma once

#include <cstddef>
#include <bitset>
#include"simpleclasses.h"
#include"Tilemap.h"
#include"SDL.h"
#include<SDL_image.h>
#include <cassert>


Dim TileX4(Index index);
Dim TileY4(Index index);
Dim TileX4_256(Index index);
Dim TileY4_256(Index index);

void PutTile(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile);
void PutTile256(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile);

class Display {
private:
	SDL_Surface* dpyBuffer;
	SDL_Surface* tiles;
	SDL_Surface* BackgroundTiles;
	Dim dpyX;
	Dim	dpyY;
	Point viewPosCached;

public:
	Display() {
		dpyBuffer = SDL_CreateRGBSurface(0, MAX, MAX, 32, 0, 0, 0, 0);
		tiles = NULL;
		dpyX = 0;
		dpyY = 0;
		viewPosCached.x = -1;
		viewPosCached.y = -1;
	}

	~Display() {
		SDL_FreeSurface(dpyBuffer);
		SDL_FreeSurface(tiles);
	}

	void SetBackgroundTileSet(std::string path) {
		BackgroundTiles = IMG_Load(path.c_str());
		assert(BackgroundTiles);
	}

	void SetTileset(std::string path) {
		tiles = IMG_Load(path.c_str());
		assert(tiles);
	}

	void TileTerrainDisplay(Tilemap* BackgroundMap, Tilemap* ForegroundMap, SDL_Surface& dest, const SDL_Rect& viewWin, const SDL_Rect& displayArea) {
		assert(tiles);
		assert(BackgroundTiles);
		if (viewPosCached.x != viewWin.x || viewPosCached.y != viewWin.y) {
			SDL_FreeSurface(dpyBuffer);
			dpyBuffer = SDL_CreateRGBSurface(0, MAX, MAX, 32, 0, 0, 0, 0);


			auto startCol = DIV_TILE_WIDTH(viewWin.x);
			auto startRow = DIV_TILE_HEIGHT(viewWin.y);
			auto endCol = DIV_TILE_WIDTH(viewWin.x + viewWin.w - 1);
			auto endRow = DIV_TILE_WIDTH(viewWin.y + viewWin.h - 1);

			dpyX = MOD_TILE_WIDTH(viewWin.x);
			dpyY = MOD_TILE_HEIGHT(viewWin.y);
			viewPosCached.x = viewWin.x;
			viewPosCached.y = viewWin.y;

			//This renders the Background layer
			for (Dim row = startRow; row <= endRow; ++row)
				for (Dim col = startCol; col <= endCol; ++col) {
					PutTile(
						*dpyBuffer,
						MUL_TILE_WIDTH(col - startCol),
						MUL_TILE_HEIGHT(row - startRow),
						*BackgroundTiles, //tiles
						BackgroundMap->GetTile(row, col)
					);

				}

			//This renders the foreground layer
			/*for (Dim row = startRow; row <= endRow; ++row)
				for (Dim col = startCol; col <= endCol; ++col) {
					PutTile256(
						*dpyBuffer,
						MUL_TILE_WIDTH256(col - startCol),
						MUL_TILE_HEIGHT256(row - startRow),
						*tiles, //tiles
						ForegroundMap->GetTile(row, col)
					);
				}*/
		}

		SDL_Rect srcrect = { dpyX, dpyY, viewWin.w, viewWin.h };
		SDL_Rect dstrect = { displayArea.x, displayArea.y, NULL, NULL };
		SDL_BlitSurface(
			dpyBuffer,
			&srcrect,
			&dest,
			&dstrect
		);
	}
};