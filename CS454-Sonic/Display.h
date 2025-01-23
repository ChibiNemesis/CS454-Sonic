#pragma once

#include <cstddef>
#include <bitset>
#include"simpleclasses.h"
#include"Tilemap.h"
#include"SDL.h"
#include<SDL_image.h>
#include <cassert>

#define FOREGROUND_TILEMAP_ERROR_WIDTH 8
#define FOREGROUND_TILEMAP_ERROR_HEIGHT 8

#define BACKGROUND_TILEMAP_HEIGHT 10
#define BACKGROUND_TILEMAP_WIDTH 11

#define FOREGROUND_TILEMAP_HEIGHT 45
#define FOREGROUND_TILEMAP_WIDTH 20

Dim TileX4(Index index);
Dim TileY4(Index index);
Dim TileX4_V2(Index index);
Dim TileY4_V2(Index index, int modifier);

void PutTile(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile);
void PutTile_V2(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile, int YModifier);

class Display {
private:
	SDL_Surface* dpyBuffer;
	SDL_Surface* tiles;
	SDL_Surface* BackgroundTiles;
	Dim dpyX;
	Dim	dpyY;
	Point viewPosCached;

	int IndexTileAdditions[FOREGROUND_TILEMAP_HEIGHT][FOREGROUND_TILEMAP_WIDTH];

public:
	Display() {
		dpyBuffer = SDL_CreateRGBSurface(0, MAX, MAX, 32, 0, 0, 0, 0);
		tiles = NULL;
		dpyX = 0;
		dpyY = 0;
		viewPosCached.x = -1;
		viewPosCached.y = -1;

		int rows = 45;
		int columns = 20;
		int width = 64;
		int height = 64;
		int heights[45][20];

		for (auto r = 0; r < rows; r++) {
			for (auto c = 0; c < columns; c++) {
				heights[r][c] = r * height;
			}
		}

		for (auto r = 0; r < rows; r++) {
			for (auto c = 0; c < columns; c++) {
				int index = (columns * r) + (c + 1);
				//std::cout << index << ", ";
				IndexTileAdditions[r][c] = (heights[r][c] / width) - (index / height);
			}
			//std::cout << "\n";
		}

		/*
		for (auto i = 0; i < rows; i++) {
			for (auto col = 0; col < columns; col++) {
				std::cout << IndexTileAdditions[i][col] << ", ";
			}
			std::cout << "\n";
		}*/
	}

	~Display() {
		SDL_FreeSurface(dpyBuffer);
		SDL_FreeSurface(tiles);
	}

	int GetIndexYModifier(int Index) {
		int r = (Index-1) / FOREGROUND_TILEMAP_WIDTH;
		int c = Index - FOREGROUND_TILEMAP_WIDTH * (Index / FOREGROUND_TILEMAP_WIDTH);
		return IndexTileAdditions[r][c];
	}

	int IndexAdditor(int index) {

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
						BackgroundMap->GetTile(row, col) // BackgroundMap->GetTile(row, col)
					);
				}

			//This renders the foreground layer
			for (Dim row = startRow; row <= endRow; ++row)
				for (Dim col = startCol; col <= endCol; ++col) {
					//Stuff to do in case of gaps
					int x_modifier = 0;

					//Check if it is first column, in that case move 8 pixels to the right
					if (col == 0) {
						x_modifier = 8;
					}
					else {
						// for each 4 tiles on x axis add 8 to x modifier
						x_modifier = (col / 4) * 8;
					}

					PutTile_V2(
						*dpyBuffer,
						MUL_TILE_WIDTH(col - startCol),
						MUL_TILE_HEIGHT(row - startRow),
						*tiles, //tiles
						ForegroundMap->GetTile(row, col),
						GetIndexYModifier(ForegroundMap->GetTile(row, col))
					);
				}
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