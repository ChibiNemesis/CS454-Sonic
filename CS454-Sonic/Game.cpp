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
	if (TTF_Init() < 0)
		std::cout << "TTF error: " << TTF_GetError() << std::endl;
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	music = Mix_LoadMUS("Audio\\ambience.mp3");
	ringSound = Mix_LoadWAV("Audio\\ring.mp3");
	skidSound = Mix_LoadWAV("Audio\\skidSound.mp3");
	jumpSound = Mix_LoadWAV("Audio\\jumpSound.mp3");
	spinSound = Mix_LoadWAV("Audio\\spinSound.mp3");
	ringLossSound = Mix_LoadWAV("Audio\\ringLossSound.mp3");
	spikeDeathSound = Mix_LoadWAV("Audio\\spikeDeathSound.mp3");
	gameOverSound = Mix_LoadWAV("Audio\\gameOverSound.mp3");

	if (!music) {
		std::cout << "Music Error: " << Mix_GetError() << "\n";
	}
	if (!ringSound) {
		std::cout << "Ring Sound Error: " << Mix_GetError() << "\n";
	}
	if (!skidSound) {
		std::cout << "Skid Sound Error: " << Mix_GetError() << "\n";
	}
	if (!ringLossSound) {
		std::cout << "Ring Loss Sound Error: " << Mix_GetError() << "\n";
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
	std::string LeftWalkPath = "tilesets\\SonicWalkingLeft.png";
	std::string RightWalkPath = "tilesets\\SonicWalkingRight.png";
	std::string LeftRunningPath = "tilesets\\SonicRunLeft.png";
	std::string RightRunningPath = "tilesets\\SonicRunRight.png";
	std::string LeftSkidPath = "tilesets\\SonicSkidLeft.png";
	std::string RightSkidPath = "tilesets\\SonicSkidRight.png";
	std::string LeftJumpPath = "tilesets\\SonicJumpLeft.png";
	std::string RightJumpPath = "tilesets\\SonicJumpRight.png";
	std::string LeftIdlePath = "tilesets\\SonicIdleLeft.png";
	std::string RightIdlePath = "tilesets\\SonicIdleRight.png";
	std::string LeftRollJump = "tilesets\\SonicRollJumpLeft.png";
	std::string RightRollJump = "tilesets\\SonicRollJumpRight.png";
	std::string LeftCurlUp = "tilesets\\SonicCurlingUpLeft.png";
	std::string RightCurlUp = "tilesets\\SonicCurlingUpRight.png";
	std::string LeftLookUp = "tilesets\\SonicLookUpLeft.png";
	std::string RightLookUp = "tilesets\\SonicLookUpRight.png";
	std::string DeathPath = "tilesets\\SonicDeath.png";
	std::string WinPath = "tilesets\\SonicWin.png";
	std::string GameOverPath = "tilesets\\GameOver.png";

	SDL_Surface* LeftWalkSurface = IMG_Load(LeftWalkPath.c_str());
	SDL_Surface* RightWalkSurface = IMG_Load(RightWalkPath.c_str());
	SDL_Surface* LeftRunningSurface = IMG_Load(LeftRunningPath.c_str());
	SDL_Surface* RightRunningSurface = IMG_Load(RightRunningPath.c_str());
	SDL_Surface* LeftSkidSurface = IMG_Load(LeftSkidPath.c_str());
	SDL_Surface* RightSkidSurface = IMG_Load(RightSkidPath.c_str());
	SDL_Surface* LeftJumpSurface = IMG_Load(LeftJumpPath.c_str());
	SDL_Surface* RightJumpSurface = IMG_Load(RightJumpPath.c_str());
	SDL_Surface* LeftIdleSurface = IMG_Load(LeftIdlePath.c_str());
	SDL_Surface* RightIdleSurface = IMG_Load(RightIdlePath.c_str());
	SDL_Surface* LeftRollJumpSurface = IMG_Load(LeftRollJump.c_str());
	SDL_Surface* RightRollJumpSurface = IMG_Load(RightRollJump.c_str());
	SDL_Surface* LeftCurlUpSurface = IMG_Load(LeftCurlUp.c_str());
	SDL_Surface* RightCurlUpSurface = IMG_Load(RightCurlUp.c_str());
	SDL_Surface* LeftLookUpSurface = IMG_Load(LeftLookUp.c_str());
	SDL_Surface* RightLookUpSurface = IMG_Load(RightLookUp.c_str());
	SDL_Surface* DeathSurface = IMG_Load(DeathPath.c_str());
	SDL_Surface* WinSurface = IMG_Load(WinPath.c_str());
	SDL_Surface* GameOverSurface = IMG_Load(GameOverPath.c_str());

	//Rect paths
	std::string LeftWalkRectPath = "Animation\\Sonic\\SonicWalkLeft.txt";
	std::string RightWalkRectPath = "Animation\\Sonic\\SonicWalkRight.txt";
	std::string LeftRunningRectPath = "Animation\\Sonic\\SonicRunLeft.txt";
	std::string RightRunningRectPath = "Animation\\Sonic\\SonicRunRight.txt";
	std::string LeftSkidRectPath = "Animation\\Sonic\\SonicSkidLeft.txt";
	std::string RightSkidRectPath = "Animation\\Sonic\\SonicSkidRight.txt";
	std::string LeftJumpRectPath = "Animation\\Sonic\\SonicJumpLeft.txt";
	std::string RightJumpRectPath = "Animation\\Sonic\\SonicJumpRight.txt";
	std::string LeftIdleRectPath = "Animation\\Sonic\\SonicIdleLeft.txt";
	std::string RightIdleRectPath = "Animation\\Sonic\\SonicIdleRight.txt";
	std::string LeftRollJumpRectPath = "Animation\\Sonic\\SonicRollJumpLeft.txt";
	std::string RightRollJumpRectPath = "Animation\\Sonic\\SonicRollJumpRight.txt";
	std::string LeftCurlUpRectPath = "Animation\\Sonic\\SonicCurlingUpLeft.txt";
	std::string RightCurlUpRectPath = "Animation\\Sonic\\SonicCurlingUpRight.txt";
	std::string LeftLookUpRectPath = "Animation\\Sonic\\SonicLookUpLeft.txt";
	std::string RightLookUpRectPath = "Animation\\Sonic\\SonicLookUpRight.txt";
	std::string DeathRectPath = "Animation\\Sonic\\SonicDeath.txt";
	std::string WinRectPath = "Animation\\Sonic\\SonicWin.txt";
	std::string GameOverRectPath = "Animation\\Sonic\\GameOver.txt";

	//Now, initialize all necessary films
	LeftWalkFilm = new AnimationFilm(LeftWalkSurface, LeftWalkRectPath, "Sonic-Left-Walk");
	RightWalkFilm = new AnimationFilm(RightWalkSurface, RightWalkRectPath, "Sonic-Right-Walk");
	LeftRunningFilm = new AnimationFilm(LeftRunningSurface, LeftRunningRectPath, "Sonic-Left-Run");
	RightRunningFilm = new AnimationFilm(RightRunningSurface, RightRunningRectPath, "Sonic-Right-Run");
	LeftSkidFilm = new AnimationFilm(LeftSkidSurface, LeftSkidRectPath, "Sonic-Left-Skid");
	RightSkidFilm = new AnimationFilm(RightSkidSurface, RightSkidRectPath, "Sonic-Right-Skid");
	LeftIdleFilm = new AnimationFilm(LeftIdleSurface, LeftIdleRectPath, "Sonic-Left-Idle");
	RightIdleFilm = new AnimationFilm(RightIdleSurface, RightIdleRectPath, "Sonic-Right-Idle");
	LeftJumpFilm = new AnimationFilm(LeftJumpSurface, LeftJumpRectPath, "Sonic-Left-Jump");
	RightJumpFilm = new AnimationFilm(RightJumpSurface, RightJumpRectPath, "Sonic-Right-Jump");
	LeftRollJumpFilm = new AnimationFilm(LeftRollJumpSurface, LeftRollJumpRectPath, "Sonic-Left-RollJump");
	RightRollJumpFilm = new AnimationFilm(RightRollJumpSurface, RightRollJumpRectPath, "Sonic-Right-RollJump");
	LeftCurlUpFilm = new AnimationFilm(LeftCurlUpSurface, LeftCurlUpRectPath, "Sonic-Left-CurlUp");
	RightCurlUpFilm = new AnimationFilm(RightCurlUpSurface, RightCurlUpRectPath, "Sonic-Right-CurlUp");
	LeftLookUpFilm = new AnimationFilm(LeftLookUpSurface, LeftLookUpRectPath, "Sonic-Left-LookUp");
	RightLookUpFilm = new AnimationFilm(RightLookUpSurface, RightLookUpRectPath, "Sonic-Right-LookUp");
	DeathFilm = new AnimationFilm(DeathSurface, DeathRectPath, "Sonic-death");
	WinFilm = new AnimationFilm(WinSurface, WinRectPath, "Sonic-win");
	GameOverFilm = new AnimationFilm(GameOverSurface, GameOverRectPath, "Game-Over");

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

	//Spikes Setup
	for (auto sp = 0; sp < SPIKES; sp++) {
		SpikeVec.push_back(sp);
	}
	std::string Spike_Surface_path = "tilesets\\spikes.png";
	std::string Spike_Rects_Path = "Animation\\Spikes\\SpikeBitmapPos.txt";
	SDL_Surface* Spike_Surface = IMG_Load(Spike_Surface_path.c_str());
	AnimationFilm* spike_Film = new AnimationFilm(Spike_Surface, Spike_Rects_Path, "Spike-Film");

	for (auto sp = 0; sp < SPIKES; sp++) {
		std::string Spike_Pos_Path = "Animation\\Spikes\\Spikes" + std::to_string(sp) + ".txt";

		std::ifstream input{ Spike_Pos_Path };
		std::string line, x, y;
		std::getline(input, line);
		std::istringstream ss(std::move(line));
		std::getline(ss, x, ' ');
		std::getline(ss, y, ' ');

		Spikes[sp] = new Spike(stoi(x), stoi(y), spike_Film, "Spike-" + std::to_string(sp));
	}

	//Font Init
	TextFont = TTF_OpenFont("Fonts\\second.ttf", 20);
	StartTime = GetSystemTime();
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
float acceleration = 0.15f;
float maxSpeed = DEFAULT_MOVEMENT_SPEED;     // The maximum speed the character can reach
float friction = 0.98f;      // How much the velocity decays when no input is given

float jumpInitialVelocity = DEFAULT_MOVEMENT_SPEED-1;
float gravityAcceleration = 0.3f;  // Gravity added per fixed update
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
		// zero small velocities
		if (fabs(velX) < 0.1f)
			velX = 0;
	}

	if (dy!=0) {
		velY = -jumpInitialVelocity;
	}
	else {
		if (!isOnSolidGround || isDead) {
			velY += gravityAcceleration;
			gravityAttached = true;
		}
		else {
			// When on the ground
			velY = 0;
		}
	}

	character->Move(static_cast<int>(velX), static_cast<int>(velY));
	if (!isDead) {
		ScrollWithBoundsCheck(&map, &ViewWindow, static_cast<int>(velX), static_cast<int>(velY));
	}
}

void Game::DisplayUI()
{
	auto TotalScore = coins * 100;
	auto CurrentTime = GetSystemTime();
	int minutes = (CurrentTime - StartTime)/60000; //60000 ms = 1 minute
	int Seconds = ((CurrentTime - StartTime) % 60000)/1000;

	std::string timeText;
	std::string ScoreText;
	std::string RingsText;
	if (Seconds < 10) {
		timeText = std::to_string(minutes)+":0" + std::to_string(Seconds);
	}
	else {
		timeText = std::to_string(minutes) + ":" + std::to_string(Seconds);
	}
	ScoreText = "Score " + std::to_string(100 * coins);
	RingsText = "Rings: " + std::to_string(coins);

	SDL_Color color = { 228, 15, 243, 255 };
	SDL_Surface* ScoreSurface = TTF_RenderText_Solid(TextFont, ScoreText.c_str(), color);
	SDL_Surface* TimeSurface = TTF_RenderText_Solid(TextFont, timeText.c_str(), color);
	SDL_Surface* RingsSurface = TTF_RenderText_Solid(TextFont, RingsText.c_str(), color);
	//blit
	SDL_Rect score_Rect = {10, 10, NULL, NULL};
	SDL_Rect Time_Rect = { 10, 40, NULL, NULL };
	SDL_Rect Rings_Rect = { 10, 70, NULL, NULL };
	SDL_BlitSurface(ScoreSurface, NULL, winsurface, &score_Rect);
	SDL_BlitSurface(TimeSurface, NULL, winsurface, &Time_Rect);
	SDL_BlitSurface(RingsSurface, NULL, winsurface, &Rings_Rect);
	SDL_FreeSurface(ScoreSurface);
	SDL_FreeSurface(TimeSurface);
	SDL_FreeSurface(RingsSurface);
}

bool canJump = true;
bool isRolling = false;
bool downButtonPressed = false;
bool ignoreDownButton = false;
bool lookUp = false;

void Game::HandleCharacterMovements()
{
	// Keyboard input handling for character movement and scrolling:
	int x = 0, y = 0;

	character->directMotion = true;

	ignoreDownButton = true;

	if (Inputs[SDL_SCANCODE_A]) {
		x = -1;
		if (velX > 0.6) //Addition to Stop Faster like Original Game!
			x -= velX;

		if (isRolling && isOnSolidGround && velX<0) { // To ignore the input if in rolling mode but still be able to stop the rolling if oposite direction input of current momentum is received.
			x = 0;
		}

		direction = LEFT;
	}
	else if (Inputs[SDL_SCANCODE_D]) {
		x = 1;
		if (velX < -0.6) //Addition to Stop Faster like Original Game!
			x -= velX;

		if (isRolling && isOnSolidGround && velX> 0) {
			x = 0;
		}

		direction = RIGHT;
	}
	else {
		ignoreDownButton = false; //Stop Ignoring when Move Left/Right aren't pressed.
	}

	if (isOnSolidGround) {
		if (Inputs[SDL_SCANCODE_W] && canJump) {
			y = -1;
			canJump = false;
			Mix_PlayChannel(-1, jumpSound, 0);
		}else if (Inputs[SDL_SCANCODE_S] && !ignoreDownButton) {
				downButtonPressed = true;
		}
		else if (Inputs[SDL_SCANCODE_SPACE]) {
			lookUp = true;
		}
	}
	else
	{
		lookUp = false;
	}

	PhysicsMoveCharacter(x, y); // TODO: This should be run by our Physics Loop preferably

	//Sonic Horizontal/Vertical/Rolling Animations/Movements Start Here
	std::string film_id = character->GetCurrentFilm()->GetId();

	if (isOnSolidGround) {
		if (velX < -0.6) // ---Almost Zero Velocity--- Value: 0.6 == AccelerationRate(0.15) x 4
		{
			if (downButtonPressed) {
				if (direction == LEFT) {
					if (film_id != "Sonic-Left-RollJump") {
						character->SetAnimationFilm(LeftRollJumpFilm);
					}
				}
				else if (direction == RIGHT) {
					if (film_id != "Sonic-Right-RollJump") {
						character->SetAnimationFilm(RightRollJumpFilm);
					}
				}
				if (isRolling == false) {
					isRolling = true;
					character->SetStaticHeight(20);
					character->Move(0, 20);
					Mix_PlayChannel(-1, spinSound, 0);
				}
			}
			else {
				if (velX == -maxSpeed)
				{
					if (film_id != "Sonic-Left-Run")
					{
						character->SetAnimationFilm(LeftRunningFilm);
					}
				}
				else {
					if (direction == LEFT && film_id != "Sonic-Left-Walk")
					{
						character->SetAnimationFilm(LeftWalkFilm);
					}
					else if (x >0 && film_id != "Sonic-Left-Skid")
					{
						character->SetAnimationFilm(LeftSkidFilm);
						Mix_PlayChannel(-1, skidSound, 0);
					}
				}
				if (isRolling) {
					isRolling = false;
					character->Move(0, -20);
					character->SetStaticHeight(40);
				}
			}
		}
		else if (velX > 0.6) //Almost 0
		{
			if (downButtonPressed) {
				if (direction == LEFT) {
					if (film_id != "Sonic-Left-RollJump") {
						character->SetAnimationFilm(LeftRollJumpFilm);
					}
				}
				else if (direction == RIGHT) {
					if (film_id != "Sonic-Right-RollJump") {
						character->SetAnimationFilm(RightRollJumpFilm);
					}
				}
				if (isRolling == false) {
					isRolling = true;
					character->SetStaticHeight(20);
					character->Move(0, 20);
					Mix_PlayChannel(-1, spinSound, 0);
				}
				//downButtonPressed = false;
			}
			else {
				if (velX == maxSpeed)
				{
					if (film_id != "Sonic-Right-Run")
					{
						character->SetAnimationFilm(RightRunningFilm);
					}
				}
				else {
					if (direction==RIGHT && film_id != "Sonic-Right-Walk")
						character->SetAnimationFilm(RightWalkFilm);
					else if (x < 0 && film_id != "Sonic-Right-Skid") {
						character->SetAnimationFilm(RightSkidFilm);
						Mix_PlayChannel(-1, skidSound, 0);
					}
				}
				if (isRolling) {
					isRolling = false;
					character->Move(0, -20);
					character->SetStaticHeight(40);
				}
			}
		}
		else {
			if ( /*x == 0 &&*/ isRolling) {
				isRolling = false;
				character->Move(0, -20);
				character->SetStaticHeight(40);
				downButtonPressed = false;
			}
			if (film_id == "Sonic-Right-Skid" || film_id == "Sonic-Left-Skid") { //For Edge Case to mimic the real game's movement of pressing stop but still keep on slipping. 
				if (direction == LEFT)
				{
					direction = RIGHT;
					character->SetAnimationFilm(RightIdleFilm);
				}
				else
				{
					direction = LEFT;
					character->SetAnimationFilm(LeftIdleFilm);
				}
			} 
			else {		//This runs most times. (Default)
				if (downButtonPressed && !isRolling) {
					if (direction == LEFT)
					{
						if (film_id != "Sonic-Left-CurlUp") {
							character->SetAnimationFilm(LeftCurlUpFilm);
							character->SetStaticHeight(20);
							character->Move(0, 20);
						}
					}
					else
					{
						if (film_id != "Sonic-Right-CurlUp") {
							character->SetAnimationFilm(RightCurlUpFilm);
							character->SetStaticHeight(20);
							character->Move(0, 20);
						}
					}
					
					downButtonPressed = false;
				}
				else {
					if (film_id == "Sonic-Right-CurlUp" || film_id=="Sonic-Left-CurlUp" || isRolling) {
						character->Move(0, -20);
						character->SetStaticHeight(40);
					}

					if (direction == LEFT)
					{
						if (lookUp) {
							character->SetAnimationFilm(LeftLookUpFilm);
						}else
							character->SetAnimationFilm(LeftIdleFilm);
					}
					else
					{
						if (lookUp) {
							character->SetAnimationFilm(RightLookUpFilm);
						}else
							character->SetAnimationFilm(RightIdleFilm);
					}
				}
			}
		}
	}
	else {
		if (canJump == false) { //It's jumping via player Input, thus is in rolling air state.
			if (direction == LEFT) {
				if (film_id != "Sonic-Left-RollJump") {
					character->SetAnimationFilm(LeftRollJumpFilm);
				}
			}
			else if (direction == RIGHT) {
				if (film_id != "Sonic-Right-RollJump") {
					character->SetAnimationFilm(RightRollJumpFilm);
				}
			}
			if (isRolling == false) {
				isRolling = true;
				character->Move(0, 20);
				character->SetStaticHeight(20);
			}
		}
		else {
			if (isRolling) {
				character->Move(0, -20);
				character->SetStaticHeight(40);
				isRolling = false;
			}
		}
	}

	//Sonic Horizontal Movement Based Animations End

	if (velX != 0 || velY != 0) {
		FixCameraPos(ViewWindow); // Camera follows when Character Moves.
	}
	else if (lookUp) {
		ScrollWithBoundsCheck(&map, &ViewWindow, 0, -1);
		//TODO: use a global counter to limit the ammount of scrolling looking up does and reset to normal Y coordinates when not pressing this.
	}
	else {
	}

}

bool debugCoinDestroyedTest = false;

bool deathAnimationActive = false;
bool lastMusic = false;
int frameCountAfterDeath = 0;

/// <summary>
/// Executing the recorded Inputs.
/// </summary>
void Game::InputHandler()
{
	HandleScrolling();
	HandleScrollingMultiplier();

	if (!isDead) {
		HandleCharacterMovements();
	}
	else {
		if (!deathAnimationActive) {
			//Sonic DEath ANimation plays and dies!
			Mix_PlayChannel(-1, spikeDeathSound, 0);
			Mix_HaltMusic();
			deathAnimationActive = true;
		}
		else {
			if (!lastMusic) {
				Mix_PlayChannel(-1, gameOverSound, 0);
				lastMusic = true;
				character->SetAnimationFilm(DeathFilm);
				gravityAcceleration = 0.03f;
			}
			
			if (frameCountAfterDeath < 20) {
				velY = -1;
				character->Move(0, static_cast<int>(velY));;
			}
			else {
				PhysicsMoveCharacter(0, 0);
			}

			frameCountAfterDeath++;
			// Game Over
			//PhysicsMoveCharacter(); // Up and Down Motion.
		}
	}

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
				case SDL_SCANCODE_W: //JUMP Button
					Inputs[SDL_SCANCODE_W] = false;
					canJump = true;
					break;
				case SDL_SCANCODE_SPACE: //JUMP Button
					Inputs[SDL_SCANCODE_SPACE] = false;
					lookUp = false;
					break;
				//case SDL_SCANCODE_S:
				//	//downButtonPressed = true;
				//	Inputs[SDL_SCANCODE_S] = false;
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
		const Uint32 FIXED_FPS = 60; // Used to Decouple Physics/InputHandling from Rendering. 16.67ms
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
			//SDL_Delay(1);
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
				coins++;
			}
			CharacterBox->~BoundingBox();
			RingBox->~BoundingBox();
		}

		for (auto val : SpikeVec) {
			auto cb = character->GetBox();
			auto spikeb = Spikes[val]->GetBox();
			BoundingBox* CharacterBox = new BoundingBox(cb.x, cb.y, cb.x + cb.w, cb.y + cb.h);
			BoundingBox* SpikeBox = new BoundingBox(spikeb.x, spikeb.y, spikeb.x + spikeb.w, spikeb.y + spikeb.h);
			if (CharacterBox->Intersects(*SpikeBox)) {
				//Spikes[val]->SetCollided(true);
				//Spikes[val]->DestroyCoin();
				if (coins > 0) {
					//Sonic gets Hit!
					Mix_PlayChannel(-1, ringLossSound, 0);
					coins -= 10;
					if (coins < 0) {
						coins = 0;
					}
				}
				else {
					//Sonic Dies from Spikes!
					if (!isDead) {
						//Mix_PlayChannel(-1, spikeDeathSound, 0);
						isDead = true;
					}
				}

				//SpikeVec.erase(find(SpikeVec.begin(), SpikeVec.end(), val));
				/*coins++;*/ //Damaged++
			}
			CharacterBox->~BoundingBox();
			SpikeBox->~BoundingBox();
		}

		SDL_Rect sr = character->GetBox();

		SDL_Rect sr2{ sr.x, sr.y, character->getStaticWidth(), character->getStaticHeight() };

		BoundingBox* CharacterBox = new BoundingBox(sr.x, sr.y, sr.x + sr2.w, sr.y + sr2.h);
		BoundingBox* Temp = new BoundingBox(0, 316, 1173, 408);
		/*if (CharacterBox->Intersects(*Temp)) {
			std::cout << "Intersects\n";
		}*/
		isOnSolidGround = grid->IsOnSolidGround(*CharacterBox);
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

		//Render all available spikes
		for (auto val : SpikeVec) {
			SDL_Rect Spike_Rect{
				Spikes[val]->GetBox().x - ViewWindow.x,
				Spikes[val]->GetBox().y - ViewWindow.y,
				Spikes[val]->GetBox().w,
				Spikes[val]->GetBox().h
			};
			Spikes[val]->Display(*winsurface, Spike_Rect);
		}

		//Render Character after animation and physics are done
		SDL_Rect Character_Rect = {
			character->GetBox().x - ViewWindow.x,
			character->GetBox().y - ViewWindow.y,
			character->GetBox().w,
			character->GetBox().h
		};
		character->Display(*winsurface, Character_Rect);

		DisplayUI();

		assert(!SDL_UpdateWindowSurface(win));
	}
