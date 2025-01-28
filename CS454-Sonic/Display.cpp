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
	int special_case[16] = { 753,754,755,756,773,774,775,776,793,794,795,796,813,814,815,816 };

	// first find the row of the tile, then add 8 pixels every 4 rows
	int y_modifier = (((index - 1) / 80) * 8) + 8;
	for (auto a = 0; a < 16; a++) {
		if (special_case[a] == index) {
			y_modifier = 16;
		}
	}

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