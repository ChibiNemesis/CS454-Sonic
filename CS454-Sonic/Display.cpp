#include"Display.h"

Dim TileX4(Index index) { return ((index % 11) - 1) * TILE_WIDTH; } // %10
Dim TileY4(Index index) { return (index / 11) * TILE_HEIGHT; }      // %10

Dim TileX4_256(Index index) { return ((index % 10) - 1) * TILE_WIDTH256; } //take a look at these too
Dim TileY4_256(Index index) { return (index / 10) * TILE_HEIGHT256; }  //take a look at these too

void PutTile(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile) {

	SDL_Rect srcrect = { TileX4(tile),
						TileY4(tile),
						TILE_WIDTH,
						TILE_HEIGHT
	};
	SDL_Rect dstrect = { x, y, NULL, NULL };

	SDL_BlitSurface(&tiles, &srcrect, &dest, &dstrect);
}

void PutTile256(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile) {

	SDL_Rect srcrect = { TileX4_256(tile),
						TileY4_256(tile),
						TILE_WIDTH256,
						TILE_HEIGHT256
	};
	SDL_Rect dstrect = { x, y, NULL, NULL };

	SDL_BlitSurface(&tiles, &srcrect, &dest, &dstrect);
}