#include"Display.h"

//Used for background
Dim TileX4(Index index) { return ((index % BACKGROUND_TILEMAP_WIDTH) - 1) * TILE_WIDTH; }
Dim TileY4(Index index) { return (index / (BACKGROUND_TILEMAP_HEIGHT + 1)) * TILE_HEIGHT; }

//Used for Foreground
Dim TileX4_V2(Index index) { 
	//The tilemap has gaps, so we have to move the pivot to the right
	int x_modifier = ((((index % FOREGROUND_TILEMAP_WIDTH) - 1) / 4) + 1) * 8;
	return (((index % FOREGROUND_TILEMAP_WIDTH) - 1) * TILE_WIDTH) + x_modifier; 
}
Dim TileY4_V2(Index index) { 

	//The tilemap has gaps, so we have to move the pivot down
	int y_modifier = ((((index % FOREGROUND_TILEMAP_HEIGHT) - 1) / 80) + 2) * 8;

	return ((((index-1) / (FOREGROUND_TILEMAP_WIDTH))) * TILE_HEIGHT) + y_modifier; 
}

void PutTile(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile) {

	SDL_Rect srcrect = { TileX4(tile),
						TileY4(tile),
						TILE_WIDTH,
						TILE_HEIGHT
	};
	SDL_Rect dstrect = { x, y, NULL, NULL };

	SDL_BlitSurface(&tiles, &srcrect, &dest, &dstrect);
}


void PutTile_V2(SDL_Surface& dest, Dim x, Dim y, SDL_Surface& tiles, Index tile) 
{

	SDL_Rect srcrect = { TileX4_V2(tile),
						TileY4_V2(tile),
						TILE_WIDTH,
						TILE_HEIGHT
	};

	SDL_Rect dstrect = { x, y, NULL, NULL };

	SDL_BlitSurface(&tiles, &srcrect, &dest, &dstrect);
}