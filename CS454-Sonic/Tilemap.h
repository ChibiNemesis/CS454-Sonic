#pragma once
#include"simpleclasses.h"
#include <cstdio>
#include<string>
#include<cassert>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <iomanip>
#include <vector>

#define MAX 2000
#define TILE_WIDTH 64 //16
#define TILE_HEIGHT 64 //16

#define TILE_WIDTH256 256
#define TILE_HEIGHT256 256

#define TILESET_WIDTH 16 // row = 16 tiles
#define TILESET_HEIGHT 16 // col = 16 tiles
#define ROW_MASK 0x0F
#define COL_MASK 0xF0
#define COL_SHIFT 4

#define TILEX_MASK 0xFF00
#define TILEX_SHIFT 8
#define TILEY_MASK 0x00FF

//shift 6 bits for 64 x 64 tilemaps
#define MUL_TILE_WIDTH(i) ((i)<<6)
#define MUL_TILE_HEIGHT(i)((i)<<6)
#define DIV_TILE_WIDTH(i) ((i)>>6)
#define DIV_TILE_HEIGHT(i)((i)>>6)
#define MOD_TILE_WIDTH(i) ((i)&63)
#define MOD_TILE_HEIGHT(i)((i)&63)


//shift 8 bits for 256 x 256 tilemaps
#define MUL_TILE_WIDTH256(i) ((i)<<8)
#define MUL_TILE_HEIGHT256(i)((i)<<8)
#define DIV_TILE_WIDTH256(i) ((i)>>8)
#define DIV_TILE_HEIGHT256(i)((i)>>8)
#define MOD_TILE_WIDTH256(i) ((i)&255)
#define MOD_TILE_HEIGHT256(i)((i)&255)

class Tilemap
{
private:
	int width;
	int height;
	Index map[MAX][MAX];
public:
	Tilemap();
	Tilemap(int w, int h);
	void SetDimensions(int _width, int _height);
	int getWidth();
	int getHeight();
	void SetTile(Dim row, Dim col, Index index);
	Index GetTile(Dim row, Dim col);
	bool ReadTextMap(std::string filename);
};

