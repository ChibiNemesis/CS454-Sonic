#include "Game.h"


Game::Game(std::string name, int width, int height)
{
	// Initialize SDL. SDL_Init will return -1 if it fails.
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return;
	}

	IMG_Init(IMG_INIT_PNG);
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
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
	win = SDL_CreateWindow(name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN); //SDL_WINDOW_SHOWN // SDL_WINDOW_FULLSCREEN //SDL_WINDOW_FULLSCREEN_DESKTOP

	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
	if (!rend)
	{
		printf("error creating renderer: %s\n", SDL_GetError());
		SDL_Quit();
		return;
	}

	winsurface = SDL_GetWindowSurface(win);
	//// Fill the window with a white rectangle
	//SDL_FillRect(winsurface, NULL, SDL_MapRGB(winsurface->format, 255, 255, 255));


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
	grid = new GridLayer("terrain\\SolidTiles.txt");


	//use this type of animation to update the elapsed time
	tickanimation = new TickAnimation("Anim-0", 50, 1, false);
	tickanimator = new TickAnimator();
	tickanimator->Start(*tickanimation, GetSystemTime());

	//Setup sonic animation films
	//Surface paths
	std::string LeftMovementPath = "tilesets\\SonicWalkingLeft.png";
	std::string RightMovementPath = "tilesets\\SonicWalkingRight.png";
	std::string LeftJumpPath = "tilesets\\SonicJumpLeft.png";
	std::string RightJumpPath = "tilesets\\SonicJumpRight.png";
	std::string LeftIdlePath = "tilesets\\SonicIdleLeft.png";
	std::string RightIdlePath = "tilesets\\SonicIdleRight.png";
	std::string WinPath = "tilesets\\SonicWin.png";
	SDL_Surface* LeftMovementSurface = IMG_Load(LeftMovementPath.c_str());
	SDL_Surface* RightMovementSurface = IMG_Load(RightMovementPath.c_str());
	SDL_Surface* LeftJumpSurface = IMG_Load(LeftJumpPath.c_str());
	SDL_Surface* RightJumpSurface = IMG_Load(RightJumpPath.c_str());
	SDL_Surface* LeftIdleSurface = IMG_Load(LeftIdlePath.c_str());
	SDL_Surface* RightIdleSurface = IMG_Load(RightIdlePath.c_str());
	SDL_Surface* WinSurface = IMG_Load(WinPath.c_str());

	//Rect paths
	std::string LeftMovementRectPath = "Animation\\Sonic\\SonicWalkLeft.txt";
	std::string RightMovementRectPath = "Animation\\Sonic\\SonicWalkRight.txt";
	std::string LeftJumpRectPath = "Animation\\Sonic\\SonicJumpLeft.txt";
	std::string RightJumpRectPath = "Animation\\Sonic\\SonicJumpRight.txt";
	std::string LeftIdleRectPath = "Animation\\Sonic\\SonicIdleLeft.txt";
	std::string RightIdleRectPath = "Animation\\Sonic\\SonicIdleRight.txt";
	std::string WinRectPath = "Animation\\Sonic\\SonicWin.txt";

	//Now, initialize all necessary films
	LeftMovementFilm = new AnimationFilm(LeftMovementSurface, LeftMovementRectPath, "Sonic-Left");
	RightMovementFilm = new AnimationFilm(RightMovementSurface, RightMovementRectPath, "Sonic-Right");
	LeftIdleFilm = new AnimationFilm(LeftIdleSurface, LeftIdleRectPath, "Sonic-Left-Idle");
	RightIdleFilm = new AnimationFilm(RightIdleSurface, RightIdleRectPath, "Sonic-Right-Idle");
	LeftJumpFilm = new AnimationFilm(LeftJumpSurface, LeftJumpRectPath, "Sonic-Left-Jump");
	RightJumpFilm = new AnimationFilm(RightJumpSurface, RightJumpRectPath, "Sonic-Right-Jump");
	WinFilm = new AnimationFilm(WinSurface, WinRectPath, "Sonic-win");

	character = new Character(158, 276, RightIdleFilm, "Sonic");
	character->SetStaticHeight(40);
	character->SetStaticWidth(32);

	//Rings Setup
	for (auto r = 0; r < COINS; r++) {
		CoinVec.push_back(r);
	}
	std::string Ring_Surface_path = "tilesets\\misc_fixed.png";
	std::string Ring_Rects_Path = "Animation\\Coins\\coinBitmapPos.txt";
	SDL_Surface* Ring_Surface = IMG_Load(Ring_Surface_path.c_str());
	AnimationFilm* coin_Film = new AnimationFilm(Ring_Surface, Ring_Rects_Path, "Coin-Film-0");

	for (auto c = 0; c < COINS; c++) {
		std::string Ring_Pos_Path = "Animation\\Coins\\Coin" + std::to_string(c) + ".txt";
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
	AnimationFilm* flower_films[3] = { Tulip_Film, Sunflower_Film, Lily_Film };

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

void Game::HandleScrolling()
{
	int x = 0, y = 0;

	if (Inputs[SDL_SCANCODE_LEFT]) {
		x = -movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}
	else if (Inputs[SDL_SCANCODE_RIGHT]) {
		x = movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}

	if (Inputs[SDL_SCANCODE_UP]) {
		y = -movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}
	else if (Inputs[SDL_SCANCODE_DOWN]) { // For debugging purposes
		y = movement_offset;
		ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
	}

	if (Inputs[SDL_SCANCODE_HOME] || Inputs[SDL_SCANCODE_KP_1]) {
		Scroll(&ViewWindow, -ViewWindow.x, -ViewWindow.y);
	}
	else if (Inputs[SDL_SCANCODE_END] || Inputs[SDL_SCANCODE_KP_7]) {
		Scroll(&ViewWindow, ((map.getWidth() * 64) - ViewWindow.x) - ViewWindow.w,
			((map.getHeight() * 64) - ViewWindow.y) - ViewWindow.h);
	}

	// Handle Mouse Dragging
	if (isMouseDragging && (mouseDeltaX != 0 || mouseDeltaY != 0)) {
		// Apply the accumulated mouse movement to scroll the map.
		ScrollWithBoundsCheck(&map, &ViewWindow,
			-mouseDeltaX * scrollMultiplier, -mouseDeltaY * scrollMultiplier);
		mouseDeltaX = 0;
		mouseDeltaY = 0;
	}
}

void Game::HandleScrollingMultiplier()
{
	if (Inputs[SDL_SCANCODE_MINUS] || Inputs[SDL_SCANCODE_KP_MINUS]) {
		if (scrollMultiplierapplied) {
			if (scrollMultiplier >= 1.0f) {
				scrollMultiplier -= 0.5f;
				setmovementspeed(DEFAULT_MOVEMENT_SPEED * scrollMultiplier);
			}
			Inputs[SDL_SCANCODE_MINUS] = false;
			Inputs[SDL_SCANCODE_KP_MINUS] = false;
			scrollMultiplierapplied = false;
		}
	}
	else if (Inputs[SDL_SCANCODE_EQUALS] || Inputs[SDL_SCANCODE_KP_PLUS]) {
		if (scrollMultiplierapplied) {
			if (scrollMultiplier <= 1.5f) {
				scrollMultiplier += 0.5f;
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

}

// TODO: Move these to .h file when done.
float velX = 0.0f, velY = 0.0f;
float acceleration = 0.3f;
float maxSpeed = DEFAULT_MOVEMENT_SPEED;     // The maximum speed the character can reach
float friction = 0.9f;      // How much the velocity decays when no input is given

float jumpInitialVelocity = 15.0f;
float gravityAcceleration = 1.0f;  // Gravity added per fixed update
float maxFallSpeed = 14.0f;        // Maximum falling speed
bool isOnSolidGround = true;
bool gravityAttached = false;

void Game::PhysicsMoveCharacter(int dx, int dy) {
	// Horizontal acceleration
	if (dx != 0) {
		velX += dx * acceleration;
		// Clamp velX to maxSpeed
		if (velX > maxSpeed)
			velX = maxSpeed;
		else if (velX < -maxSpeed)
			velX = -maxSpeed;
	}
	else {
		// No horizontal input: apply friction to decelerate
		velX *= friction;
		// Optionally zero small velocities
		if (fabs(velX) < 0.1f)
			velX = 0;
	}

	if (dy!=0) {
		velY = -jumpInitialVelocity;
	}
	if (!isOnSolidGround) {
		velY += gravityAcceleration;
		if (velY > maxFallSpeed) {
			//velY = maxFallSpeed;
			velY = 0;				//For Debugging, Until Gravity and Ground Check are ready.
			isOnSolidGround = true; //For Debugging, Until Gravity and Ground Check are ready.
		}
	}
	else {
		// When on the ground
		velY = 0;
	}

	character->Move(static_cast<int>(velX), static_cast<int>(velY));
	ScrollWithBoundsCheck(&map, &ViewWindow, static_cast<int>(velX), static_cast<int>(velY));
}


void Game::HandleCharacterMovements()
{
	// Keyboard input handling for character movement and scrolling:
	int x = 0, y = 0;

	std::string film_id = character->GetCurrentFilm()->GetId();

	character->directMotion = true;
	/*if (!Inputs[SDL_SCANCODE_A] && !Inputs[SDL_SCANCODE_D]) {

		if (film_id != "Sonic-Right-Idle" && film_id != "Sonic-Left-Idle")
		{
			if (direction == LEFT)
			{
				character->SetAnimationFilm(LeftIdleFilm);
			}
			else
			{
				character->SetAnimationFilm(RightIdleFilm);
			}
		}
	}*/

	if (Inputs[SDL_SCANCODE_A]) {
		direction = LEFT;
		if (film_id != "Sonic-Left")
			character->SetAnimationFilm(LeftMovementFilm);
		x = -1;
	}
	else if (Inputs[SDL_SCANCODE_D]) {
		//x = movement_offset;
		//ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
		direction = RIGHT;
		if (film_id != "Sonic-Right")
			character->SetAnimationFilm(RightMovementFilm);
		//character->Move(movement_offset, 0);
		x = 1;
	}

	if (Inputs[SDL_SCANCODE_W] && isOnSolidGround) {
		//y = -movement_offset;
		//ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
		y = -1;
		isOnSolidGround = false;
		gravityAttached = false;
	}
	else if (Inputs[SDL_SCANCODE_S]) {
		//y = -movement_offset;
		//ScrollWithBoundsCheck(&map, &ViewWindow, x, y);
		y = 1;
	}
	else {
		if (gravityAttached && isOnSolidGround==false) {
			y = 0;
		}
	}

	PhysicsMoveCharacter(x, y); // TODO: This should be run by our Physics Loop preferably

	if (velX != 0 || velY != 0) {
		FixCameraPos(ViewWindow); // Camera follows when Character Moves.
	}
	else {
		if (direction == LEFT)
		{
			character->SetAnimationFilm(LeftIdleFilm);
		}
		else
		{
			character->SetAnimationFilm(RightIdleFilm);
		}
	}

}

bool debugCoinDestroyedTest = false;

/// <summary>
/// Executing the recorded Inputs.
/// </summary>
void Game::InputHandler()
{
	HandleScrolling();
	HandleScrollingMultiplier();
	HandleCharacterMovements();

	// Test coin collision code:
	if (Inputs[SDL_SCANCODE_1] && !debugCoinDestroyedTest) {
		//Test, code when player collides with a coin
		/*if (Inputs[SDL_SCANCODE_1] && debugCoinDestroyedTest == false) {
			Mix_PlayChannel(-1, ringSound, 0);
			Coins[1]->DestroyCoin();
			int coin_in = 1;
			CoinVec.erase(std::find(CoinVec.begin(), CoinVec.end(), coin_in));
			debugCoinDestroyedTest = true;
		}*/
	}
	if (Inputs[SDL_SCANCODE_ESCAPE]) {
		stoprunning();
	}
}

	/// <summary>
	/// Input Polling & Recording to bool array.
	/// </summary>
	void Game::Input() {
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
				default:  // For continuous button reads.
					Inputs[event.key.keysym.scancode] = false;
					break;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					isMouseDragging = true;
					lastMouseX = event.button.x;
					lastMouseY = event.button.y;
				}
			}
			else if (event.type == SDL_MOUSEBUTTONUP) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					isMouseDragging = false;
					// Optionally reset delta here if you want to avoid a sudden jump
					mouseDeltaX = 0;
					mouseDeltaY = 0;
				}
			}
			else if (event.type == SDL_MOUSEMOTION) {
				if (isMouseDragging) {
					// Calculate movement since last event
					int deltaX = event.motion.x - lastMouseX;
					int deltaY = event.motion.y - lastMouseY;
					// Accumulate the delta
					mouseDeltaX += deltaX;
					mouseDeltaY += deltaY;
					// Update last position
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
		const Uint32 FIXED_FPS = 60; // Used to Decouple Physics/InputHandling from Rendering.
		const double physicsUpdateInterval = 1000.0 / FIXED_FPS; // e.g., ~33.33ms per fixed update

		Uint32 currentTime = SDL_GetTicks();
		Uint32 lastPhysicsUpdateTime = currentTime;
		double deltaTime = 0;

		while (getrunning()) {
			Render();   // Render the scene
			Input();    // Poll and record input events
			Animate();  // Update animations

			// Check if it's time to update physics and input logic
			currentTime = SDL_GetTicks();
			deltaTime = currentTime - lastPhysicsUpdateTime;
			if (deltaTime >= physicsUpdateInterval) {
				InputHandler(); // Process input logic
				Physics();      // Update physics
				lastPhysicsUpdateTime = currentTime;
			}
			//FixCameraPos(ViewWindow);
			// Delay to yield CPU
			SDL_Delay(1);
		}
	}

	void Game::Physics()
	{
		//Here check if player box has collided with any coin
		//in that case, set the rendering parameter on that coin to false and play a sound
		for (auto val : CoinVec) {
			auto cb = character->GetBox();
			auto coinb = Coins[val]->GetBox();
			BoundingBox* CharacterBox = new BoundingBox(cb.x, cb.y, cb.x + cb.w, cb.y + cb.h);
			BoundingBox* RingBox = new BoundingBox(coinb.x, coinb.y, coinb.x + coinb.w, coinb.y + coinb.h);
			if (CharacterBox->Intersects(*RingBox)) {
				Coins[val]->SetCollected(true);
				Coins[val]->DestroyCoin();
				Mix_PlayChannel(-1, ringSound, 0);

				CoinVec.erase(find(CoinVec.begin(), CoinVec.end(), val));
			}
			CharacterBox->~BoundingBox();
			RingBox->~BoundingBox();
		}
		SDL_Rect sr = character->GetBox();

		SDL_Rect sr2{ sr.x, sr.y, character->getStaticWidth(), character->getStaticHeight() };

		BoundingBox* CharacterBox = new BoundingBox(sr.x, sr.y, sr.x + sr2.w, sr.y + sr2.h);
		BoundingBox* Temp = new BoundingBox(0, 316, 1173, 408);
		if (CharacterBox->Intersects(*Temp)) {
			std::cout << "Intersects\n";
		}
		grid->IsOnSolidGround(*CharacterBox);
		CharacterBox->~BoundingBox();
		Temp->~BoundingBox();
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

		character->Progress(time);
	}

	void Game::PrepareSpriteGravityHandler(GridLayer * gridLayer, Sprite * sprite)
	{
		/*
		character->GetGravityHandler().SetOnSolidGround(
			[gridLayer](const SDL_Rect& r)
			{ return gridLayer->IsOnSolidGround(r); }
		);*/
	}

	void Game::FixCameraPos(SDL_Rect & ViewWin)
	{
		FixCameraPosX(ViewWin);
		FixCameraPosY(ViewWin);
	}

	void Game::FixCameraPosX(SDL_Rect & ViewWin)
	{
		auto CharacterPos = character->GetBox();
		auto x_center = (ViewWin.x + (ViewWin.w / 2));
		auto x_modifier = 0;
		if ((CharacterPos.x) != x_center) {
			if (abs(x_modifier - (x_center - CharacterPos.x)) < CAMERASCROLLMODIFIER) {
				if (x_center > (CharacterPos.x)) {
					x_modifier = -abs(x_modifier - (x_center - CharacterPos.x));
				}
				else {
					x_modifier = abs(x_modifier - (x_center - CharacterPos.x));
				}
			}
			else {
				if (x_center > (CharacterPos.x)) {
					x_modifier = -CAMERASCROLLMODIFIER;
				}
				else {
					x_modifier = CAMERASCROLLMODIFIER;
				}
			}
		}

		ScrollWithBoundsCheck(&map, &ViewWin, x_modifier, 0);
	}

	void Game::FixCameraPosY(SDL_Rect & ViewWin)
	{
		auto CharacterPos = character->GetBox();
		auto y_center = (ViewWin.y + (ViewWin.h / 2));
		auto y_modifier = 0;
		if ((CharacterPos.y) != y_center) {
			if (abs(y_modifier - (y_center - CharacterPos.y)) < CAMERASCROLLMODIFIER) {
				if (y_center > (CharacterPos.x)) {
					y_modifier = -abs(y_modifier - (y_center - CharacterPos.y));
				}
				else {
					y_modifier = abs(y_modifier - (y_center - CharacterPos.y));
				}
			}
			else {
				if (y_center > (CharacterPos.y)) {
					y_modifier = -CAMERASCROLLMODIFIER;
				}
				else {
					y_modifier = CAMERASCROLLMODIFIER;
				}
			}
		}

		ScrollWithBoundsCheck(&map, &ViewWin, 0, y_modifier);
	}

	void Game::Render()
	{
		SDL_Rect displayArea = { 0, 0, NULL, NULL };
		display.TileTerrainDisplay(&map, &Foregroundmap, *winsurface, ViewWindow, displayArea);

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

		//Render Character after animation and physics are done
		SDL_Rect Character_Rect = {
			character->GetBox().x - ViewWindow.x,
			character->GetBox().y - ViewWindow.y,
			character->GetBox().w,
			character->GetBox().h
		};
		character->Display(*winsurface, Character_Rect);

		assert(!SDL_UpdateWindowSurface(win));
	}
