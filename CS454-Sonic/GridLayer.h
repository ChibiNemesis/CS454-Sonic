#pragma once
#include"Tilemap.h"
#include"simpleclasses.h"
#include<string>
#include"SDL.h"
#include<cassert>
#include<set>
#include <fstream>
#include <sstream>
#include <filesystem>
#include"BoundingBox.h"

#define MAX_WIDTH 2000
#define MAX_HEIGHT 2000


#define GRID_ELEMENT_WIDTH 8 
#define GRID_ELEMENT_HEIGHT 8
#if TILE_WIDTH % GRID_ELEMENT_WIDTH != 0
#error "TILE_WIDTH % GRID_ELEMENT_WIDTH must be zero!"
#endif
#if TILE_HEIGHT % GRID_ELEMENT_HEIGHT != 0
#error "TILE_HEIGHT % GRID_ELEMENT_HEIGHT must be zero!"
#endif
#define GRID_BLOCK_COLUMNS (TILE_WIDTH / GRID_ELEMENT_WIDTH)
#define GRID_BLOCK_ROWS (TILE_HEIGHT / GRID_ELEMENT_HEIGHT)
#define GRID_ELEMENTS_PER_TILE (GRID_BLOCK_ROWS * GRID_BLOCK_COLUMNS)
#define GRID_MAX_HEIGHT (MAX_HEIGHT * GRID_BLOCK_ROWS)
#define GRID_MAX_WIDTH (MAX_WIDTH * GRID_BLOCK_COLUMNS)

#define GRID_THIN_AIR_MASK 0x0000 // element is ignored
#define GRID_LEFT_SOLID_MASK 0x0001 // bit 0
#define GRID_RIGHT_SOLID_MASK 0x0002 // bit 1
#define GRID_TOP_SOLID_MASK 0x0004 // bit 2
#define GRID_BOTTOM_SOLID_MASK 0x0008 // bit 3
#define GRID_GROUND_MASK 0x0010 // bit 4, keep objects top / bottom (gravity)
#define GRID_FLOATING_MASK 0x0020 // bit 5, keep objects anywhere inside (gravity)

#define MAX_PIXEL_WIDTH MUL_TILE_WIDTH(MAX_WIDTH)
#define MAX_PIXEL_HEIGHT MUL_TILE_HEIGHT(MAX_HEIGHT)
#define DIV_GRID_ELEMENT_WIDTH(i) ((i)>>3) //2
#define DIV_GRID_ELEMENT_HEIGHT(i) ((i)>>3)
#define MUL_GRID_ELEMENT_WIDTH(i) ((i)<<3)
#define MUL_GRID_ELEMENT_HEIGHT(i) ((i)<<3)

#define GRID_EMPTY_TILE GRID_THIN_AIR_MASK
#define GRID_SOLID_TILE \
(GRID_LEFT_SOLID_MASK | GRID_RIGHT_SOLID_MASK | GRID_TOP_SOLID_MASK | GRID_BOTTOM_SOLID_MASK)


using GridIndex = byte;

class GridLayer {
private:
	std::set<int> solidtiles;
	GridIndex map[MAX][MAX];
	Dim totalRows, totalColumns;
	std::vector<BoundingBox> Colliders;
public:
	GridLayer(std::string path);

	bool FilterMotionDown(const BoundingBox& r, int& dy);

	bool FilterMotionUp(const BoundingBox& r, int* dy);

	bool FilterMotionLeft(const BoundingBox& r, int* dx);

	bool FilterMotionRight(const BoundingBox& r, int* dx);

	bool IsOnSolidGround(const BoundingBox& r);

	bool CanGoUp(const  BoundingBox& r);
};