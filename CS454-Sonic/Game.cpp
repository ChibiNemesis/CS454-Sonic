#include "Game.h"

Game::Game(std::string name, int height, int width)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	loopCounter = 0;
	change_iter = 0;

	direction = RIGHT;

	//viewwindow on tilemap
	ViewWindow.x = 48;
	ViewWindow.y = 0;
	ViewWindow.w = width;
	ViewWindow.h = height;
	win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	winsurface = SDL_GetWindowSurface(win);
	movement_offset = 0;

	//set correct tileset for background and front tiles
	//remember, background tiles are 64 x 64 while foreground are 256 x 256. from the foreground tiles render 8 pixels lower/to the right
	display.SetTileset("tilesets\\tiles_first_map_fixed.png");
	display.SetBackgroundTileSet("tilesets\\background_foreground64.png");

	//this might not be needed
	TerrainNode* root = new TerrainNode("terrain");

	assert(map.ReadTextMap("terrain\\Background"));
	assert(Foregroundmap.ReadTextMap("terrain\\Foreground"));


	//set gridmap too
	grid = new GridLayer("terrain\\SolidTiles.txt"); //we need to create that first
	grid->SetRowsCols(map.getWidth(), map.getHeight());
	grid->SetGridMap(&map);


	//use this type of animation to update the elapsed time
	tickanimation = new TickAnimation("5",50,1,false);
	tickanimator = new TickAnimator();
	tickanimator->Start(*tickanimation, GetSystemTime());
	
	//Use the correct constructor here
	character = new Character();
}

Game::~Game()
{
	SDL_DestroyWindow(win);
	IMG_Quit();
	SDL_Quit();
}

void Game::stoprunning()
{
	this->running = false;
}

bool Game::getrunning()
{
	return running;
}

void Game::setfps(int _fps)
{
	this->fps = _fps;
}

void Game::setmovementspeed(int speed)
{
	this->movement_offset = speed;
}

int Game::getfps()
{
	return this->fps;
}

void Game::Input()
{
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			ismoving = false;
			if (event.key.keysym.sym == SDLK_RIGHT) {
				ScrollWithBoundsCheck(&map, &ViewWindow, movement_offset, 0);
				//ScrollWithBoundsCheck(&Foregroundmap, &ViewWindow, movement_offset, 0);
				direction = RIGHT;
				ismoving = true;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				ScrollWithBoundsCheck(&map, &ViewWindow, -movement_offset, 0);
				//ScrollWithBoundsCheck(&Foregroundmap, &ViewWindow, -movement_offset, 0);
				direction = LEFT;
				ismoving = true;
			}

			if (event.key.keysym.sym == SDLK_UP) {
				//ScrollWithBoundsCheck(&Foregroundmap, &ViewWindow, 0, -movement_offset);
				ScrollWithBoundsCheck(&map, &ViewWindow, 0, -movement_offset);
				direction = RIGHT;
				ismoving = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				ScrollWithBoundsCheck(&map, &ViewWindow, 0, movement_offset);
				//ScrollWithBoundsCheck(&Foregroundmap, &ViewWindow, 0, movement_offset);
				direction = LEFT;
				ismoving = true;
			}

			if (event.key.keysym.sym == SDLK_ESCAPE) {//pause
				this->stoprunning();
			}
		}
		else if (event.type == SDL_QUIT) {
			stoprunning();
		}
	}
}

void Game::SetTilemap(std::string path)
{
	assert(map.ReadTextMap(path));
}

void Game::change_Tilemap() {
	this->SetTilemap(current_terrain->path + "\\Tilemap.txt");
}

void Game::mainloop()
{
	
	//Uint32 start = SDL_GetTicks();
	
	loopCounter++;
	if (loopCounter == fps)
		loopCounter = 1;
	Input();
	Render();
	Physics();
	//Animate();

	//if (1000 / fps > SDL_GetTicks() - start)
	//	SDL_Delay(1000/fps-(SDL_GetTicks() - start));
}

void Game::Physics()
{

}

void Game::Animate()
{
	tickanimator->Progress(GetSystemTime());
}

void Game::PrepareSpriteGravityHandler(GridLayer* gridLayer, Sprite* sprite)
{
	character->GetGravityHandler().SetOnSolidGround(
		[gridLayer](const SDL_Rect& r)
		{ return gridLayer->IsOnSolidGround(r); }
	);
}

void Game::Render()
{
	//int w,h;
	//SDL_GetWindowSize(win, &w, &h);
	SDL_Rect displayArea = { 0, 0, NULL, NULL };
	display.TileTerrainDisplay(&map, &Foregroundmap ,*winsurface, ViewWindow, displayArea);

	assert(!SDL_UpdateWindowSurface(win));
}
