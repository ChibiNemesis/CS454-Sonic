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
#include"SDL_mixer.h"
#include"Flower.h"
#include"BoundingBox.h"

#define LEFT 0
#define RIGHT 1
#define COINS 20
#define FLOWERS 4

#define DEFAULT_MOVEMENT_SPEED 15

class Game {
private:
	SDL_Rect ViewWindow;
	Mix_Music* music;
	Mix_Chunk* ringSound;
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

	//INPUTS
	bool Inputs[SDL_NUM_SCANCODES]; //512
	bool isMouseDragging = false;
	int lastMouseX = 0, lastMouseY = 0; // Store previous mouse position
	double  scrollMultiplier = 1.0f;
	bool scrollMultiplierapplied = false;


	SDL_Window* win = nullptr;
	SDL_Surface* winsurface = nullptr;
	Display display = Display();
	Tilemap map = Tilemap();
	Tilemap Foregroundmap = Tilemap();
	GridLayer* grid = NULL;
	TerrainNode* terrains = NULL;
	TerrainNode* current_terrain = NULL;

	//Character, Coin and Flower are subclasses of sprite
	Character* character = NULL;
	Coin *Coins[COINS];
	Flower* Flowers[FLOWERS];

	//One AnimationFilm for every possible diretion
	AnimationFilm* LeftMovementFilm = NULL;
	AnimationFilm* RightMovementFilm = NULL;
	AnimationFilm* LeftIdleFilm = NULL;
	AnimationFilm* RightIdleFilm = NULL;
	AnimationFilm* LeftJumpFilm = NULL;
	AnimationFilm* RightJumpFilm = NULL;
	AnimationFilm* WinFilm = NULL;

	Animation* currAnimation = NULL;

	TickAnimation* tickanimation = NULL;
	TickAnimator* tickanimator = NULL;

	std::vector<int> CoinVec;

	void Render();
	void Input();
	void InputHandler();
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

