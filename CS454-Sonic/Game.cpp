#include "Game.h"

Game::Game(std::string name, int height, int width)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	IMG_Init(IMG_INIT_PNG);

	loopCounter = 0;
	change_iter = 0;

	direction = RIGHT;

	//viewwindow on tilemap
	ViewWindow.x = 0;
	ViewWindow.y = 150; 
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
	tickanimation = new TickAnimation("Anim-0",50,1,false);
	tickanimator = new TickAnimator();
	tickanimator->Start(*tickanimation, GetSystemTime());
	
	//Use the correct constructor here
	character = new Character();




	//Rings Setup
	for (auto r = 0; r < COINS; r++) {
		CoinVec.push_back(r);
	}
	std::string Ring_Surface_path = "tilesets\\misc_fixed.png";
	std::string Ring_Rects_Path = "Animation\\Coins\\coinBitmapPos.txt";
	SDL_Surface *Ring_Surface = IMG_Load(Ring_Surface_path.c_str());
	AnimationFilm *coin_Film = new AnimationFilm(Ring_Surface, Ring_Rects_Path, "Coin-Film-0");
	//Coins[0].SetAnimationFilm(coin_Film);
	//Coins[0] = new Coin(100, 295, coin_Film, "Coin-0"); //256, 320
	//Coins[1] = new Coin(296, 320, coin_Film, "Coin-1");
	for (auto c = 0; c < COINS; c++) {
		std::string Ring_Pos_Path = "Animation\\Coins\\Coin"+std::to_string(c)+ ".txt";
		//Initialize coin to correct position
		std::ifstream input{ Ring_Pos_Path };
		std::string line, x, y;
		std::getline(input, line);
		std::istringstream ss(std::move(line));
		std::getline(ss, x, ' ');
		std::getline(ss, y, ' ');

		Coins[c] = new Coin(stoi(x), stoi(y), coin_Film, "Coin-" + std::to_string(c));
	}
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
				direction = RIGHT;
				ismoving = true;
			}
			else if (event.key.keysym.sym == SDLK_LEFT) {
				ScrollWithBoundsCheck(&map, &ViewWindow, -movement_offset, 0);
				direction = LEFT;
				ismoving = true;
			}

			if (event.key.keysym.sym == SDLK_UP) {
				ScrollWithBoundsCheck(&map, &ViewWindow, 0, -movement_offset);
				direction = RIGHT;
				ismoving = true;
			}
			else if (event.key.keysym.sym == SDLK_DOWN) {
				ScrollWithBoundsCheck(&map, &ViewWindow, 0, movement_offset);
				direction = LEFT;
				ismoving = true;
			}

			if (event.key.keysym.sym == SDLK_ESCAPE) {
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
	Animate();


	//
	//if (1000 / fps > SDL_GetTicks() - start)
	//	SDL_Delay(1000/fps-(SDL_GetTicks() - start));
}

void Game::Physics()
{
	//Here check if player box has collided with any coin
	//in that case, set the rendering parameter on that coin to false and play a sound

}

void Game::Animate()
{
	auto time = GetSystemTime();
	tickanimator->Progress(time);

	//Coins[0]->Progress(time);
	//Coins[1]->Progress(time);
	for (auto val : CoinVec) {
		Coins[val]->Progress(time);
	}
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
	SDL_Rect displayArea = { 0, 0, NULL, NULL };
	display.TileTerrainDisplay(&map, &Foregroundmap ,*winsurface, ViewWindow, displayArea);

	//Test coins rendering
	//SDL_Rect Coin_Rect{256, 320, 0, 0};
	//SDL_Rect Coin_Rect{ Coins[0]->GetBox().x - ViewWindow.x, Coins[0]->GetBox().y - ViewWindow.y, 16, 16};
	//Coins[0]->Display(*winsurface, Coin_Rect);
	//SDL_Rect Coin_Rect2{ 150 - ViewWindow.x, 295 - ViewWindow.y, 16, 16 };
	//Coins[1]->Display(*winsurface, Coin_Rect2);

	//Render all available coins
	for (auto val : CoinVec) {
		SDL_Rect Coin_Rect{ Coins[val]->GetBox().x - ViewWindow.x, Coins[val]->GetBox().y - ViewWindow.y, 16, 16 };
		Coins[val]->Display(*winsurface, Coin_Rect);
	}

	assert(!SDL_UpdateWindowSurface(win));
}
