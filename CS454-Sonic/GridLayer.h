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



	void FilterGridMotion(const SDL_Rect& r, int* dx, int* dy) {
		/*assert(
			abs(*dx) <= GRID_ELEMENT_WIDTH && abs(*dy) <= GRID_ELEMENT_HEIGHT
		);
		// try horizontal move
		if (*dx < 0)
			FilterGridMotionLeft(r, dx);
		else
			if (*dx > 0)
				FilterGridMotionRight(r, dx);
		// try vertical move
		if (*dy < 0)
			FilterGridMotionUp(r, dy);
		else
			if (*dy > 0)
				FilterGridMotionDown(r, dy);*/
	}
	/*
	void FilterGridMotionLeft(const SDL_Rect& r, int* dx) {
		auto x1_next = r.x + *dx;
		if (x1_next < 0)
			*dx = -r.x;
		else {
			auto newCol = DIV_GRID_ELEMENT_WIDTH(x1_next);
			auto currCol = DIV_GRID_ELEMENT_WIDTH(r.x);
			if (newCol != currCol) {
				assert(newCol + 1 == currCol); // we really move left
				auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
				for (auto row = startRow; row <= endRow; ++row)
					if (!CanPassGridTile(newCol, row, GRID_RIGHT_SOLID_MASK)) {
						*dx = MUL_GRID_ELEMENT_WIDTH(currCol) - r.x;
						break;
					}
			}
		}
	}

	void FilterGridMotionRight(const SDL_Rect& r, int* dx) {
		auto x2 = r.x + r.w - 1;
		auto x2_next = x2 + *dx;
		if (x2_next >= MAX_PIXEL_WIDTH)
			*dx = (MAX_PIXEL_WIDTH)-x2; //was (MAX_PIXEL_WIDTH – 1)
		else {
			auto newCol = DIV_GRID_ELEMENT_WIDTH(x2_next);
			auto currCol = DIV_GRID_ELEMENT_WIDTH(x2);
			if (newCol != currCol) {
				assert(newCol - 1 == currCol); // we really move right
				auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
				for (auto row = startRow; row <= endRow; ++row)
					if (!CanPassGridTile(newCol, row, GRID_LEFT_SOLID_MASK)) {
						*dx = (MUL_GRID_ELEMENT_WIDTH(newCol)) - x2; //was (newCol) - 1
						break;
					}
			}
		}
	}

	void FilterGridMotionUp(const SDL_Rect& r, int* dy) {
		auto y1_next = r.y + *dy;
		if (y1_next < 0)
			*dy = -r.y;
		else {

			auto newRow = DIV_GRID_ELEMENT_WIDTH(y1_next);
			auto currRow = DIV_GRID_ELEMENT_WIDTH(r.y);
			if (newRow != currRow) {
				assert(newRow + 1 == currRow);

				auto startCol = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endCol = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1); //

				for (auto col = startCol; col <= endCol; ++col) {
					if (!CanPassGridTile(newRow, col, GRID_TOP_SOLID_MASK)) {
						*dy = MUL_GRID_ELEMENT_WIDTH(currRow) - r.y;
						break;
					}
				}
			}
		}
	}

	//filter down too
	void FilterGridMotionDown(const SDL_Rect& r, int* dy) {
		auto y1_next = r.y + *dy;
		if (y1_next < 0)
			*dy = -r.y;
		else {
			auto newCol = DIV_GRID_ELEMENT_HEIGHT(y1_next); //WIDTH
			auto currCol = DIV_GRID_ELEMENT_HEIGHT(r.y);
			std::cout << "newCol -> " << newCol << "\n";
			std::cout << "currCol ->" << currCol << "\n";
			if (newCol != currCol) {
				assert(newCol == currCol + 1); // we really move down (was: newCol + 1) (was currCol)
				auto startRow = DIV_GRID_ELEMENT_HEIGHT(r.y);
				auto endRow = DIV_GRID_ELEMENT_HEIGHT(r.y + r.h - 1);
				for (auto row = startRow; row <= endRow; ++row)
					if (!CanPassGridTile(newCol, row, GRID_BOTTOM_SOLID_MASK)) {
						*dy = MUL_GRID_ELEMENT_WIDTH(currCol) - r.y;
						break;
					}
			}
		}
	}
	*/

	bool FilterMotionDown(const BoundingBox& r, int& dy);

	bool FilterMotionUp(const SDL_Rect& r, int* dy);

	bool FilterMotionLeft(const SDL_Rect& r, int* dx);

	bool FilterMotionRight(const SDL_Rect& r, int* dx);

	bool IsOnSolidGround(const BoundingBox& r);

	bool CanGoUp(const SDL_Rect& r);
};

//bool IsTileIndexAssumedEmpty(Index index) {
//	return true;
//}

