#pragma once
#include"SDL.h"
#include"Display.h"
#include"Tilemap.h"
#include<cassert>
#include"Scroll.h"
#include"TerrainNode.h"
#include"Sprite.h"
#include"AnimationFilm.h"
#include"AnimationFilmHolder.h"
#include"TickAnimation.h"
#include"TickAnimator.h"
#include"SystemClock.h"
#include<vector>
#include<iostream>
#include "Character.h"
#include "Coin.h"

#define LEFT 0
#define RIGHT 1
#define COINS 20

class Game {
private:
	SDL_Rect ViewWindow;
	bool running = true;
	bool ismoving = false;
	int movement_offset;
	int fps;
	int loopCounter;
	int change_iter;
	int direction;

	//may be here, we'll see...
	int score = 0;
	int coins = 0;
	int time_minutes = 0;
	int time_seconds = 0;

	SDL_Window* win = nullptr;
	SDL_Surface* winsurface = nullptr;
	Display display = Display();
	Tilemap map = Tilemap();
	Tilemap Foregroundmap = Tilemap();
	GridLayer* grid = NULL;
	TerrainNode* terrains = NULL;
	TerrainNode* current_terrain = NULL;

	//Character and Coin are subclasses of sprite
	Character* character = NULL;
	Coin *Coins[COINS];

	//Not sure if we need 1 or more films
	AnimationFilm* LeftMovementFilm = NULL;
	AnimationFilm* RightMovementFilm = NULL;
	AnimationFilm* LeftIdleMovementFilm = NULL;
	AnimationFilm* RightIdleMovementFilm = NULL;

	Animation* currAnimation = NULL;

	TickAnimation* tickanimation = NULL;
	TickAnimator* tickanimator = NULL;

	std::vector<int> CoinVec;

	void Render();
	void Input();
	void Animate();
	void Physics();
public:
	Game(std::string name, int height, int width);
	~Game();

	void stoprunning();

	bool getrunning();

	void setfps(int _fps);

	void setmovementspeed(int speed);

	int getfps();

	void SetTilemap(std::string path);

	void change_Tilemap();

	void mainloop();

	void PrepareSpriteGravityHandler(GridLayer* gridLayer, Sprite* sprite);
};

