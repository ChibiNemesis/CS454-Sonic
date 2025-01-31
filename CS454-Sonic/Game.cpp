#include "Game.h"


Game::Game(std::string name, int width, int height)
{
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return ;
	}

	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(4410, MIX_DEFAULT_FORMAT, 2, 1024);
	music = Mix_LoadMUS("Audio\\ambience.mp3");
	ringSound = Mix_LoadWAV("Audio\\ring.mp3");
	if (!music) {
		std::cout << "Music Error: " << Mix_GetError() << "\n";
	}
	if (!ringSound) {
		std::cout << "Ring Sound Error: " << Mix_GetError() << "\n";
	}
	if (music != NULL) {
		Mix_PlayMusic(music, -1);
	}

	loopCounter = 0;
	change_iter = 0;

	direction = RIGHT;

	//viewwindow on tilemap
	ViewWindow.x = 0;
	ViewWindow.y = 150; 
	ViewWindow.w = width;
	ViewWindow.h = height;
	win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN //SDL_WINDOW_FULLSCREEN_DESKTOP
	winsurface = SDL_GetWindowSurface(win);
	// Fill the window with a white rectangle
	SDL_FillRect(winsurface, NULL, SDL_MapRGB(winsurface->format, 255, 255, 255));


	// Update the window display
	SDL_UpdateWindowSurface(win);
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

	//Flowers Setup
	std::string Flowers_Surface_Path = "tilesets\\flowers.png";
	std::string Tulip_Rects_Path = "Animation\\Flowers\\Frames\\Tulip.txt";
	std::string Lily_Rects_Path = "Animation\\Flowers\\Frames\\Lily.txt";
	std::string Sunflower_Rects_Path = "Animation\\Flowers\\Frames\\Sunflower.txt";
	SDL_Surface* Flower_Surface = IMG_Load(Flowers_Surface_Path.c_str());
	AnimationFilm* Tulip_Film = new AnimationFilm(Flower_Surface, Tulip_Rects_Path, "Tulip-Film");
	AnimationFilm* Sunflower_Film = new AnimationFilm(Flower_Surface, Sunflower_Rects_Path, "Sunflower-Film");
	AnimationFilm* Lily_Film = new AnimationFilm(Flower_Surface, Lily_Rects_Path, "Lily-Film");
	AnimationFilm* flower_films[3] = {Tulip_Film, Sunflower_Film, Lily_Film};
	
	int Flower_Type[FLOWERS] = { 0,1,1,1 };
	for (auto f = 0; f < FLOWERS; f++) {
		std::string Flower_Pos_Path = "Animation\\Flowers\\Positions\\Flower" + std::to_string(f) + ".txt";

		std::ifstream input{ Flower_Pos_Path };
		std::string line, x, y;
		std::getline(input, line);
		std::istringstream ss(std::move(line));
		std::getline(ss, x, ' ');
		std::getline(ss, y, ' ');

		Flowers[f] = new Flower(stoi(x), stoi(y), flower_films[Flower_Type[f]], "Flower-" + std::to_string(f));
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

bool debugCoinDestroyedTest = false;

/// <summary>
/// Executing the recorded Inputs.
/// </summary>
void Game::InputHandler() {
	int x=0, y=0;

	if (Inputs[SDL_SCANCODE_LEFT]) {
		x = -movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}else if (Inputs[SDL_SCANCODE_RIGHT]) {
		x = movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}
	
	if (Inputs[SDL_SCANCODE_UP]) {
		y = -movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}else if (Inputs[SDL_SCANCODE_DOWN]) { //This doesn't exist, needs to be removed. For Debugging purposes only.
		y = movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}

	if (Inputs[SDL_SCANCODE_HOME] || Inputs[SDL_SCANCODE_KP_1]) {
		Scroll(&ViewWindow, -ViewWindow.x, -ViewWindow.y);
	}
	else if (Inputs[SDL_SCANCODE_END] || Inputs[SDL_SCANCODE_KP_7]) {
		Scroll(&ViewWindow, ((map.getWidth() * 64) - ViewWindow.x) -ViewWindow.w, ((map.getHeight() * 64) - ViewWindow.y) - ViewWindow.h);
	}

	if (Inputs[SDL_SCANCODE_MINUS] || Inputs[SDL_SCANCODE_KP_MINUS]) {
		if (scrollMultiplierapplied) {
			if (scrollMultiplier >= 1.0f) {
				scrollMultiplier = scrollMultiplier - 0.5f;
				setmovementspeed(DEFAULT_MOVEMENT_SPEED * scrollMultiplier);
			}
			Inputs[SDL_SCANCODE_MINUS] = false;
			Inputs[SDL_SCANCODE_KP_MINUS] = false;

			scrollMultiplierapplied = false; //To apply it only once the keyup is recorded, instead of continuously.
		}
	}
	else if (Inputs[SDL_SCANCODE_EQUALS] || Inputs[SDL_SCANCODE_KP_PLUS]) {
		if (scrollMultiplierapplied) {
			if (scrollMultiplier <= 1.5f) {
				scrollMultiplier = scrollMultiplier + 0.5f;
				setmovementspeed(DEFAULT_MOVEMENT_SPEED * scrollMultiplier);
			}
			Inputs[SDL_SCANCODE_EQUALS] = false;
			Inputs[SDL_SCANCODE_KP_PLUS] = false;

			scrollMultiplierapplied = false;
		}
	}
	else if (Inputs[SDL_SCANCODE_0]) {
		scrollMultiplier = 1.0f;
		setmovementspeed(DEFAULT_MOVEMENT_SPEED * scrollMultiplier);
	}

	//Test, code when player collides with a coin
	if (Inputs[SDL_SCANCODE_1] && debugCoinDestroyedTest==false) {
		Mix_PlayChannel(-1, ringSound, 0);
		Coins[1]->DestroyCoin();
		int coin_in = 1;
		CoinVec.erase(find(CoinVec.begin(), CoinVec.end(), coin_in));
		//int index;
		//for (auto c = 0; c < CoinVec.size(); c++) {
		//	if (c == CoinVec.at(c)) {
		//
		//	}
		//}
		debugCoinDestroyedTest = true;
	}

	if (Inputs[SDL_SCANCODE_ESCAPE]) {
		this->stoprunning();
	}
}

/// <summary>
/// Input Polling & Recording to bool array.
/// </summary>
void Game::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			Inputs[event.key.keysym.scancode] = true;
		}
		else if (event.type == SDL_KEYUP) {
			switch (event.key.keysym.scancode) { // For button presses that we want to read once.
				case SDL_SCANCODE_MINUS:
					scrollMultiplierapplied = true;
					break;
				case SDL_SCANCODE_EQUALS:
					scrollMultiplierapplied = true;
					break;
				case SDL_SCANCODE_KP_PLUS:
					scrollMultiplierapplied = true;
					break;
				case SDL_SCANCODE_KP_MINUS:
					scrollMultiplierapplied = true;
					break;
				default:						// For continuous button reads.
					Inputs[event.key.keysym.scancode] = false;
					break;
			}
		}else if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                isMouseDragging = true;
                lastMouseX = event.button.x;
                lastMouseY = event.button.y;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                isMouseDragging = false;
            }
        }
        else if (event.type == SDL_MOUSEMOTION) {
            if (isMouseDragging) {
                int deltaX = event.motion.x - lastMouseX;
                int deltaY = event.motion.y - lastMouseY;
                
                // Move the map view
                ScrollWithBoundsCheck(&map, &ViewWindow, -deltaX * scrollMultiplier, -deltaY * scrollMultiplier);
                
                // Update last mouse position
                lastMouseX = event.motion.x;
                lastMouseY = event.motion.y;
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
	const int targetFPS = 30;                  // Desired FPS
	const double frameDelay = 1000.0 / targetFPS; // Milliseconds per frame (~16.67ms)
	Uint32 lastTime = SDL_GetTicks();          // Time at the start of the frame
	double lag = 0.0;
	Uint32 currentTime = lastTime;
	double elapsed = 0;

	while (getrunning()) {
		currentTime = SDL_GetTicks();   // Current time
		elapsed = currentTime - lastTime; // Time since last frame
		lastTime = currentTime;
		lag += elapsed;

		Render();
		Input();	     // Poll and record the inputs
		Animate();
		while (lag >= frameDelay) { // Anything in here runs at a fixed rate, not as soon as it can.
			InputHandler(); // Handle logic for inputs
			Physics();
			lag -= frameDelay;
		}
	}
	//Uint32 start = SDL_GetTicks();
	
	/*loopCounter++;
	if (loopCounter == fps)
		loopCounter = 1;


	Input();
	InputHandler();
	Render();
	Physics();
	Animate();*/


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

	for (auto f = 0; f < FLOWERS; f++) {
		Flowers[f]->Progress(time);
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

	//Render Flowers After Background and Foreground
	for (auto f = 0; f < FLOWERS; f++) {
		SDL_Rect Flower_Rect{ 
			Flowers[f]->GetBox().x - ViewWindow.x, 
			Flowers[f]->GetBox().y - ViewWindow.y, 
			Flowers[f]->GetBox().w, 
			Flowers[f]->GetBox().h 
		};
		Flowers[f]->Display(*winsurface, Flower_Rect);
	}

	//Render all available coins
	for (auto val : CoinVec) {
		SDL_Rect Coin_Rect{ 
			Coins[val]->GetBox().x - ViewWindow.x, 
			Coins[val]->GetBox().y - ViewWindow.y, 
			16, 
			16 
		};
		Coins[val]->Display(*winsurface, Coin_Rect);
	}

	assert(!SDL_UpdateWindowSurface(win));
}
