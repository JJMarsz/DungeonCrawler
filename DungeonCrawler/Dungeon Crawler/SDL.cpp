#include "SDL.h"
#include "Quest.h"

SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
SDL_Window* gWindow = NULL;

std::unordered_map<RoomSize, int> room_map;
ScreenState state;

/* SDL functions */
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("Party Saves Town (TM)", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
			}
		}
	}

	//set up relevant data structures
	spriteClips.resize(6);
	buttonSpriteClips.resize(12);
	tileSpriteClips.resize(29);
	texts.reserve(20);
	charClips.resize(NUM_CHAR*3);
	Buttons.resize(12);
	charButtons.resize(3);
	menuButtons.resize(1);
	menuButtons[0].setHandler(menuClicked);
	townButtons.resize(2);
	townButtons[0].setHandler(gotoShop);
	townButtons[1].setHandler(gotoCharUp);
	townButtonClips.resize(8);
	questButtons.resize(3);
	questPageClips.resize(4);
	acceptrejectClips.resize(12);
	acceptrejectButtons.resize(3);
	int i;
	for (i = 0; i<TOTAL_BUTTONS; i++)
		Buttons[i].setConstraints(BUTTON_WIDTH, BUTTON_HEIGHT);
	menuButtons[0].setConstraints(2*BUTTON_WIDTH, BUTTON_HEIGHT);
	for (i = 0; i < 3; i++) 
		charButtons[i].setConstraints(CHAR_BUTTON_WIDTH, CHAR_BUTTON_HEIGHT);
	for (i = 0; i < 3; i++)
		questButtons[i].setConstraints(QUEST_PAGE_WIDTH, QUEST_PAGE_HEIGHT);
	for (i = 0; i < 2; i++)
		townButtons[i].setConstraints(TOWN_BUTTON_WIDTH, TOWN_BUTTON_HEIGHT);
	for (i = 0; i < 3; i++)
		acceptrejectButtons[i].setConstraints(70, 30);
	acceptrejectButtons[2].setHandler(returnToTown);
	
	//8,6 small
	//16,12 med
	//20, 15 large
	//top 16 bits are width, bottom 16 are height
	room_map[LARGE] = (20 << 16) + 15;
	room_map[MED] = (16 << 16) + 12;
	room_map[SMALL] = (8 << 16) + 6;
	state = MAIN_MENU;
	chars = CharList();
	return success;
}

bool loadMedia()
{
	int i;
	//Loading success flag
	bool success = true;

	SDL_Color textColor = { 255, 255, 255 };
	//Open the font
	gFont = TTF_OpenFont("PT_sans.ttf", TEXT_SIZE);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		leftText.loadFromRenderedText("Shop", textColor, 200);
		rightText.loadFromRenderedText("Training", textColor, 200);
		acceptText.loadFromRenderedText("Accept", textColor, 200);
		rejectText.loadFromRenderedText("Reject", textColor, 200);

	}

	//Load spritesheet texture
	if (!spriteSheetTexture.loadFromFile("sprites.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//Set big enemy sprite
		spriteClips[0].x = 0;
		spriteClips[0].y = 0;
		spriteClips[0].w = 50;
		spriteClips[0].h = 50;

		//Set big player sprite
		spriteClips[1].x = 50;
		spriteClips[1].y = 0;
		spriteClips[1].w = 50;
		spriteClips[1].h = 50;

		//Set small enemy sprite
		spriteClips[2].x = 0;
		spriteClips[2].y = 50;
		spriteClips[2].w = 40;
		spriteClips[2].h = 40;

		//Set small player sprite
		spriteClips[3].x = 50;
		spriteClips[3].y = 50;
		spriteClips[3].w = 40;
		spriteClips[3].h = 40;
	}

	//Load button spritesheet texture
	if (!buttonSpriteSheetTexture.loadFromFile("button.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//Set sprites
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			buttonSpriteClips[i].x = 0;
			buttonSpriteClips[i].y = i * 60;
			buttonSpriteClips[i].w = BUTTON_WIDTH;
			buttonSpriteClips[i].h = BUTTON_HEIGHT;
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			buttonSpriteClips[i+4].x = 0;
			buttonSpriteClips[i+4].y = (i+4) * 60;
			buttonSpriteClips[i+4].w = 2*BUTTON_WIDTH;
			buttonSpriteClips[i+4].h = BUTTON_HEIGHT;
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i) 
		{
			buttonSpriteClips[i+8].x = 2 * BUTTON_WIDTH + (i)*200;
			buttonSpriteClips[i+8].y = 0;
			buttonSpriteClips[i+8].w = CHAR_BUTTON_WIDTH;
			buttonSpriteClips[i+8].h = CHAR_BUTTON_HEIGHT;
		}
		//Set buttons to locations
		Buttons[0].setPosition(BUTTON_ONE_X, BUTTON_Y);
		Buttons[1].setPosition(BUTTON_TWO_X, BUTTON_Y);
		Buttons[2].setPosition(BUTTON_THREE_X, BUTTON_Y);
		Buttons[3].setPosition(BUTTON_FOUR_X, BUTTON_Y);
		Buttons[4].setPosition(BUTTON_FIVE_X, BUTTON_Y);
		Buttons[5].setPosition(BUTTON_SIX_X, BUTTON_Y);
		Buttons[6].setPosition(BUTTON_SEVEN_X, BUTTON_Y);
		Buttons[7].setPosition(BUTTON_EIGHT_X, BUTTON_Y);
		Buttons[8].setPosition(BUTTON_NINE_X, BUTTON_Y);
		Buttons[9].setPosition(BUTTON_TEN_X, BUTTON_Y);
		Buttons[10].setPosition(BUTTON_ELEVEN_X, BUTTON_Y);
		Buttons[11].setPosition(BUTTON_TWELVE_X, BUTTON_Y);

		menuButtons[0].setPosition(SCREEN_WIDTH / 2 - menuButtons[0].getWidth() / 2, SCREEN_HEIGHT / 2 + menuButtons[0].getHeight() / 2);
		charButtons[0].setPosition(50, 50);
		charButtons[1].setPosition(300, 50);
		charButtons[2].setPosition(550, 50);
	}
	if (!mainMenu.loadFromFile("mainmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!tileSST.loadFromFile("tiles.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//way too many tiles
		tileSpriteClips[LEFT_].x = TILE_LEFT_X;
		tileSpriteClips[LEFT_].y = TILE_LEFT_Y;
		tileSpriteClips[LEFT_].w = TILE_WIDTH;
		tileSpriteClips[LEFT_].h = TILE_HEIGHT;

		tileSpriteClips[DOWN_].x = TILE_DOWN_X;
		tileSpriteClips[DOWN_].y = TILE_DOWN_Y;
		tileSpriteClips[DOWN_].w = TILE_WIDTH;
		tileSpriteClips[DOWN_].h = TILE_HEIGHT;

		tileSpriteClips[RIGHT_].x = TILE_RIGHT_X;
		tileSpriteClips[RIGHT_].y = TILE_RIGHT_Y;
		tileSpriteClips[RIGHT_].w = TILE_WIDTH;
		tileSpriteClips[RIGHT_].h = TILE_HEIGHT;

		tileSpriteClips[UP_].x = TILE_UP_X;
		tileSpriteClips[UP_].y = TILE_UP_Y;
		tileSpriteClips[UP_].w = TILE_WIDTH;
		tileSpriteClips[UP_].h = TILE_HEIGHT;

		tileSpriteClips[DOWN_LEFT].x = TILE_DOWN_LEFT_X;
		tileSpriteClips[DOWN_LEFT].y = TILE_DOWN_LEFT_Y;
		tileSpriteClips[DOWN_LEFT].w = TILE_WIDTH;
		tileSpriteClips[DOWN_LEFT].h = TILE_HEIGHT;

		tileSpriteClips[LEFT_UP].x = TILE_LEFT_UP_X;
		tileSpriteClips[LEFT_UP].y = TILE_LEFT_UP_Y;
		tileSpriteClips[LEFT_UP].w = TILE_WIDTH;
		tileSpriteClips[LEFT_UP].h = TILE_HEIGHT;

		tileSpriteClips[UP_RIGHT].x = TILE_UP_RIGHT_X;
		tileSpriteClips[UP_RIGHT].y = TILE_UP_RIGHT_Y;
		tileSpriteClips[UP_RIGHT].w = TILE_WIDTH;
		tileSpriteClips[UP_RIGHT].h = TILE_HEIGHT;

		tileSpriteClips[RIGHT_DOWN].x = TILE_RIGHT_DOWN_X;
		tileSpriteClips[RIGHT_DOWN].y = TILE_RIGHT_DOWN_Y;
		tileSpriteClips[RIGHT_DOWN].w = TILE_WIDTH;
		tileSpriteClips[RIGHT_DOWN].h = TILE_HEIGHT;

		tileSpriteClips[HORIZ].x = TILE_HORIZ_X;
		tileSpriteClips[HORIZ].y = TILE_HORIZ_Y;
		tileSpriteClips[HORIZ].w = TILE_WIDTH;
		tileSpriteClips[HORIZ].h = TILE_HEIGHT;

		tileSpriteClips[VERT].x = TILE_VERT_X;
		tileSpriteClips[VERT].y = TILE_VERT_Y;
		tileSpriteClips[VERT].w = TILE_WIDTH;
		tileSpriteClips[VERT].h = TILE_HEIGHT;

		tileSpriteClips[DOWN_LEFT_UP].x = TILE_DOWN_LEFT_UP_X;
		tileSpriteClips[DOWN_LEFT_UP].y = TILE_DOWN_LEFT_UP_Y;
		tileSpriteClips[DOWN_LEFT_UP].w = TILE_WIDTH;
		tileSpriteClips[DOWN_LEFT_UP].h = TILE_HEIGHT;

		tileSpriteClips[LEFT_UP_RIGHT].x = TILE_LEFT_UP_RIGHT_X;
		tileSpriteClips[LEFT_UP_RIGHT].y = TILE_LEFT_UP_RIGHT_Y;
		tileSpriteClips[LEFT_UP_RIGHT].w = TILE_WIDTH;
		tileSpriteClips[LEFT_UP_RIGHT].h = TILE_HEIGHT;

		tileSpriteClips[UP_RIGHT_DOWN].x = TILE_UP_RIGHT_DOWN_X;
		tileSpriteClips[UP_RIGHT_DOWN].y = TILE_UP_RIGHT_DOWN_Y;
		tileSpriteClips[UP_RIGHT_DOWN].w = TILE_WIDTH;
		tileSpriteClips[UP_RIGHT_DOWN].h = TILE_HEIGHT;

		tileSpriteClips[RIGHT_DOWN_LEFT].x = TILE_RIGHT_DOWN_LEFT_X;
		tileSpriteClips[RIGHT_DOWN_LEFT].y = TILE_RIGHT_DOWN_LEFT_Y;
		tileSpriteClips[RIGHT_DOWN_LEFT].w = TILE_WIDTH;
		tileSpriteClips[RIGHT_DOWN_LEFT].h = TILE_HEIGHT;

		tileSpriteClips[FOUR_WAY].x = TILE_FOUR_WAY_X;
		tileSpriteClips[FOUR_WAY].y = TILE_FOUR_WAY_Y;
		tileSpriteClips[FOUR_WAY].w = TILE_WIDTH;
		tileSpriteClips[FOUR_WAY].h = TILE_HEIGHT;

		tileSpriteClips[HOVER].x = TILE_HOVER_X;
		tileSpriteClips[HOVER].y = TILE_HOVER_Y;
		tileSpriteClips[HOVER].w = TILE_WIDTH;
		tileSpriteClips[HOVER].h = TILE_HEIGHT;

		tileSpriteClips[DEAD_].x = TILE_DEAD_X;
		tileSpriteClips[DEAD_].y = TILE_DEAD_Y;
		tileSpriteClips[DEAD_].w = TILE_WIDTH;
		tileSpriteClips[DEAD_].h = TILE_HEIGHT;

		tileSpriteClips[EMPTY_].x = TILE_EMPTY_X;
		tileSpriteClips[EMPTY_].y = TILE_EMPTY_Y;
		tileSpriteClips[EMPTY_].w = TILE_WIDTH;
		tileSpriteClips[EMPTY_].h = TILE_HEIGHT;

		tileSpriteClips[PARTY].x = TILE_PARTY_X;
		tileSpriteClips[PARTY].y = TILE_PARTY_Y;
		tileSpriteClips[PARTY].w = TILE_WIDTH;
		tileSpriteClips[PARTY].h = TILE_HEIGHT;

		tileSpriteClips[BAR].x = TILE_BARRIER_X;
		tileSpriteClips[BAR].y = TILE_BARRIER_Y;
		tileSpriteClips[BAR].w = TILE_WIDTH;
		tileSpriteClips[BAR].h = TILE_HEIGHT;

		tileSpriteClips[BOSSS].x = TILE_BOSS_X;
		tileSpriteClips[BOSSS].y = TILE_BOSS_Y;
		tileSpriteClips[BOSSS].w = TILE_WIDTH;
		tileSpriteClips[BOSSS].h = TILE_HEIGHT;

		tileSpriteClips[MOBS].x = TILE_MOB_X;
		tileSpriteClips[MOBS].y = TILE_MOB_Y;
		tileSpriteClips[MOBS].w = TILE_WIDTH;
		tileSpriteClips[MOBS].h = TILE_HEIGHT;

		tileSpriteClips[INFOS].x = TILE_INFO_X;
		tileSpriteClips[INFOS].y = TILE_INFO_Y;
		tileSpriteClips[INFOS].w = TILE_WIDTH;
		tileSpriteClips[INFOS].h = TILE_HEIGHT;

		tileSpriteClips[LOOTS].x = TILE_LOOT_X;
		tileSpriteClips[LOOTS].y = TILE_LOOT_Y;
		tileSpriteClips[LOOTS].w = TILE_WIDTH;
		tileSpriteClips[LOOTS].h = TILE_HEIGHT;

	}
	if (!charSST.loadFromFile("charicons.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < NUM_CHAR; i++) {
			/* big 200x200 */
			charClips[i].x = i*200;
			charClips[i].y = 0;
			charClips[i].w = 200;
			charClips[i].h = 200;
			/* small 50x50 */
			charClips[i + NUM_CHAR].x = i * 200;
			charClips[i + NUM_CHAR].y = 200;
			charClips[i + NUM_CHAR].w = 50;
			charClips[i + NUM_CHAR].h = 50;
			/* small 25x25 */
			charClips[i + 2*NUM_CHAR].x = i * 200 + 50;
			charClips[i + 2*NUM_CHAR].y = 200;
			charClips[i + 2*NUM_CHAR].w = 25;
			charClips[i + 2*NUM_CHAR].h = 25;
		}
	}
	chars.loadSprites(); 
	if (!townmenu.loadFromFile("townmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!townButtonSST.loadFromFile("townbuttons.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//left button
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			townButtonClips[i].x = 0;
			townButtonClips[i].y = i * 50;
			townButtonClips[i].w = 100;
			townButtonClips[i].h = 50;
		}
		//right button
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			townButtonClips[i+4].x = 100;
			townButtonClips[i+4].y = i * 50;
			townButtonClips[i+4].w = 100;
			townButtonClips[i+4].h = 50;
		}
		townButtons[0].setPosition(25, SCREEN_HEIGHT - 60);
		townButtons[1].setPosition(SCREEN_WIDTH - 125, SCREEN_HEIGHT - 60);
	}
	if (!questboard.loadFromFile("questboard.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!questSST.loadFromFile("quest.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			questPageClips[i].x = QUEST_PAGE_WIDTH * i;
			questPageClips[i].y = 0;
			questPageClips[i].w = QUEST_PAGE_WIDTH;
			questPageClips[i].h = QUEST_PAGE_HEIGHT;
		}
		questButtons[0].setPosition(QUEST_ONE_X, QUEST_Y);
		questButtons[1].setPosition(QUEST_TWO_X, QUEST_Y);
		questButtons[2].setPosition(QUEST_THREE_X, QUEST_Y);

	}
	if (!acceptrejectSST.loadFromFile("acceptreject.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			acceptrejectClips[i].x = 0;
			acceptrejectClips[i].y = i * 30;
			acceptrejectClips[i].w = 70;
			acceptrejectClips[i].h = 30;
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			acceptrejectClips[i+4].x = 70;
			acceptrejectClips[i+4].y = i * 30;
			acceptrejectClips[i+4].w = 70;
			acceptrejectClips[i+4].h = 30;
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			acceptrejectClips[i + 8].x = 140;
			acceptrejectClips[i + 8].y = i * 30;
			acceptrejectClips[i + 8].w = 70;
			acceptrejectClips[i + 8].h = 30;
		}
		acceptrejectButtons[0].setPosition(275, 377);
		acceptrejectButtons[1].setPosition(455, 377);
		acceptrejectButtons[2].setPosition(365, 377);
	}
	if (!questinfo.loadFromFile("questinfo.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	initQuests();
	quests.getQuests();
	if (!shop.loadFromFile("shop.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!training.loadFromFile("training.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!upgrades.loadFromFile("upgrades.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	return success;
}

void close() {
	//Free loaded images
	//free all dynamically text generated
	int i;
	for (i = 0; i < texts.size(); i++) {
		texts[i]->free();
	}
	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;
	leftText.free();
	rightText.free();
	goldmenu.free();
	xp0.free();
	xp1.free();
	xp2.free();
	completed.free();
	questTitle.free();
	questInfoText.free();
	questGold.free();
	questXP.free();
	questDiff.free();
	acceptText.free();
	rejectText.free();

	//Free Textures
	spriteSheetTexture.free();
	buttonSpriteSheetTexture.free();
	tileSST.free();
	charSST.free();
	mainMenu.free();
	townmenu.free();
	townButtonSST.free();
	questboard.free();
	questSST.free();
	questinfo.free();
	acceptrejectSST.free();
	upgrades.free();
	shop.free();
	training.free();
	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture(std::string path)
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}


int loadText(int w, std::string text) {
	SDL_Color textColor = { 255, 255, 255 };
	LTexture* newText = new LTexture;
	if (!newText->loadFromRenderedText(text, textColor, w)) {
		//failure
	}
	else {
		texts.push_back(newText);
	}
	return (texts.size() - 1);
}

/* drawing helper functions */
bool drawRoom(RoomSize size) {
	int width, height;
	if (room_map.find(size) == room_map.end()) {
		return false;
	}
	width = room_map[size] & 0xFFFF0000;
	width = width >> 16;
	height = room_map[size] & 0xFFFF;
	//std::cout << width << std::endl;
	//std::cout << height << std::endl;
	//top screen
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT - 120;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	/*
	//Render red filled quad
	SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
	SDL_RenderFillRect(gRenderer, &fillRect);

	//Render green outlined quad
	SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &outlineRect);
	*/
	int hor_off = (SCREEN_HEIGHT - 120) / height;
	int vert_off = SCREEN_WIDTH / width;
	//Draw black horizontal line
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawLine(gRenderer, 0, 0, SCREEN_WIDTH, 0);
	for (int i = 1; i< height; i++)
		SDL_RenderDrawLine(gRenderer, 0, i*hor_off, SCREEN_WIDTH, i*hor_off);
	SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT - 121, SCREEN_WIDTH, SCREEN_HEIGHT - 121);

	//draw black vertical line
	SDL_RenderDrawLine(gRenderer, 0, 0, 0, SCREEN_HEIGHT);
	for (int i = 1; i< width; i++)
		SDL_RenderDrawLine(gRenderer, i*vert_off, 0, i*vert_off, SCREEN_HEIGHT);
	SDL_RenderDrawLine(gRenderer, SCREEN_WIDTH - 1, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT);

	/*
	//Draw vertical line of yellow dots
	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
	for (int i = 0; i < SCREEN_HEIGHT; i += 4)
	{
	SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
	}
	*/
	return true;
}

void drawMenu() {
	//Bottom bar
	SDL_Rect botbar;
	botbar.x = 0;
	botbar.y = SCREEN_HEIGHT - 120;
	botbar.w = SCREEN_WIDTH;
	botbar.h = 120;
	SDL_RenderSetViewport(gRenderer, &botbar);

	//Render bar
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 163, 73, 164, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	botbar.y = 0;
	botbar.h = SCREEN_HEIGHT;

	SDL_RenderSetViewport(gRenderer, &botbar);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);

	SDL_RenderDrawLine(gRenderer, 0, BUTTON_Y - 5, SCREEN_WIDTH, BUTTON_Y - 5);
	SDL_RenderDrawLine(gRenderer, 0, BUTTON_Y - 6, SCREEN_WIDTH, BUTTON_Y - 6);
	SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT - 120, SCREEN_WIDTH, SCREEN_HEIGHT - 120);
	SDL_RenderDrawLine(gRenderer, 0, BUTTON_Y - 6, SCREEN_WIDTH, BUTTON_Y - 6);
	//Render buttons
	for (int i = 0; i < TOTAL_BUTTONS; ++i) {
		Buttons[i].render();
	}
	/*
	//Text render
	buttonOneText.render(BUTTON_ONE_X + BUTTON_WIDTH / 2 - buttonOneText.getWidth() / 2, BUTTON_Y + BUTTON_HEIGHT / 2 - buttonOneText.getHeight() / 2);
	buttonTwoText.render(BUTTON_TWO_X + BUTTON_WIDTH / 2 - buttonTwoText.getWidth() / 2, BUTTON_Y + BUTTON_HEIGHT / 2 - buttonTwoText.getHeight() / 2);
	buttonThreeText.render(BUTTON_THREE_X + BUTTON_WIDTH / 2 - buttonThreeText.getWidth() / 2, BUTTON_Y + BUTTON_HEIGHT / 2 - buttonThreeText.getHeight() / 2);
	*/
}

void drawMainMenu() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	mainMenu.render(0, 0, NULL);
	//buttonSpriteSheetTexture.render(SCREEN_WIDTH / 2 - menuButtons[0].getWidth() / 2, SCREEN_HEIGHT / 2 + menuButtons[0].getHeight() / 2);
	menuButtons[0].render();
}

int getTileIndex(int x, int y) {
	if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth())).getType() == NONE)
		return EMPTY_;
	/*if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth())).getType() == DEADEND)
		return DEAD_;*/
	if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth())).getType() == BARRIER)
		return EMPTY_;
	//bitwise storage of adjacency
	//	R	D	L	U
	//
	int adj = 0;
	if (current_dungeon.isStart(x, y))
		adj += 4;
	if (current_dungeon.isEnd(x, y))
		adj += 1;
	//check adjacent
	//check up
	if (y > 0) {
		if (current_dungeon.getTile(x + (y-1) * (current_dungeon.getWidth())).getType() != NONE
			&& current_dungeon.getTile(x + (y - 1) * (current_dungeon.getWidth())).getType() != BARRIER)
			adj += 1;
	}
	//check left
	if (x > 0) {
		if (current_dungeon.getTile(x - 1 + (y) * (current_dungeon.getWidth())).getType() != NONE
			&& current_dungeon.getTile(x - 1 + (y) * (current_dungeon.getWidth())).getType() != BARRIER)
			adj += 2;
	}
	//check down
	if (y < current_dungeon.getHeight()-1) {
		if (current_dungeon.getTile(x + (y + 1) * (current_dungeon.getWidth())).getType() != NONE
			&& current_dungeon.getTile(x + (y + 1) * (current_dungeon.getWidth())).getType() != BARRIER)
			adj += 4;
	}
	//check right
	if (x < current_dungeon.getWidth() - 1) {
		if (current_dungeon.getTile(x + 1 + (y) * (current_dungeon.getWidth())).getType() != NONE
			&& current_dungeon.getTile(x + 1 + (y) * (current_dungeon.getWidth())).getType() != BARRIER)
			adj += 8;
	}
	switch (adj) {
	case 1:
		return UP_;
	case 2:
		return LEFT_;
	case 3:
		return LEFT_UP;
	case 4:
		return DOWN_;
	case 5:
		return VERT;
	case 6:
		return DOWN_LEFT;
	case 7:
		return DOWN_LEFT_UP;
	case 8:
		return RIGHT_;
	case 9:
		return UP_RIGHT;
	case 10:
		return HORIZ;
	case 11:
		return LEFT_UP_RIGHT;
	case 12:
		return RIGHT_DOWN;
	case 13:
		return UP_RIGHT_DOWN;
	case 14:
		return RIGHT_DOWN_LEFT;
	case 15:
		return FOUR_WAY;
	default:
		return EMPTY_;
	}
}
int encounterExists(int x, int y) {
	if (gParty.getX() == x && gParty.getY() == y)
		return -1;
	switch (current_dungeon.getTile(x + y * current_dungeon.getWidth()).getType()) {
	case BOSS:
		return BOSSS;
	case MOB:
		return MOBS;
	case LOOT:
		return LOOTS;
	case INFO:
		return INFOS;
	default:
		return -1;
	}
}
void drawDungeon() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);
	int start_x = (SCREEN_WIDTH - current_dungeon.getWidth()*50) / 2;
	for (int x = 0; x < current_dungeon.getWidth(); x++) {
		for (int y = 0; y < current_dungeon.getHeight(); y++) {
			int index = getTileIndex(x, y);
			tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
			//check if something is at this tile
			index = encounterExists(x, y);
			if (index != -1) 
				tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
		}
	}
	//draw any special tiles
	tileSST.render(gParty.getX() * 50 + start_x, gParty.getY() * 50, &tileSpriteClips[PARTY]);
}

void drawCharScreen() {
	int i;
	srand(time(NULL));
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);
	for (i = 0; i < 3; i++) {
		charButtons[i].render();
		displayList[i].render(50 + 250 * i, 50);
	}
}

void drawQuestBoard() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	questboard.render(0, 0, NULL);
	for (int i = 0; i < 3; i++)
		questButtons[i].render();
}

void drawShop() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	shop.render(0, 0, NULL);
}

void drawCharUp() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	training.render(0, 0, NULL);
}

void drawPartyUp() {
	SDL_Rect topViewport;
	topViewport.x = 0;
	topViewport.y = 0;
	topViewport.w = SCREEN_WIDTH;
	topViewport.h = SCREEN_HEIGHT;
	SDL_RenderSetViewport(gRenderer, &topViewport);
	//make it gray
	SDL_Rect barRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_SetRenderDrawColor(gRenderer, 0xA0, 0xA0, 0xA0, 0xFF);
	SDL_RenderFillRect(gRenderer, &barRect);

	upgrades.render(0, 0, NULL);
}

void drawTownMenu() {
	int i;
	townmenu.render(0, SCREEN_HEIGHT - 60, NULL);
	state = TOWN_BUTTON_LEFT;
	townButtons[0].render();
	state = TOWN_BUTTON_RIGHT;
	townButtons[1].render();
	leftText.render(LEFT_X, LEFTRIGHT_Y);
	rightText.render(RIGHT_X, LEFTRIGHT_Y);
	//render text gold amount, xp amount, and completed dungeons
	goldmenu.render(542, SCREEN_HEIGHT - 44);
	xp2.render(464, SCREEN_HEIGHT - 44);
	xp1.render(378, SCREEN_HEIGHT - 44);
	xp0.render(292, SCREEN_HEIGHT - 44);
	completed.render(210, SCREEN_HEIGHT - 44);

	//render character icons for selected characters
	charSST.render(246, SCREEN_HEIGHT - 47, &gParty.getChar(0).getIcon25());
	charSST.render(332, SCREEN_HEIGHT - 47, &gParty.getChar(1).getIcon25());
	charSST.render(418, SCREEN_HEIGHT - 47, &gParty.getChar(2).getIcon25());

}

void drawQuestInfo() {
	

	questinfo.render(262, 150, NULL);
	state = SELECTED_QUEST_ACCEPT;
	acceptrejectButtons[0].render();
	state = SELECTED_QUEST_REJECT;
	acceptrejectButtons[1].render();
	questTitle.render(280, 164);
	questDiff.render(280, 200);
	questGold.render(280, 220);
	questXP.render(280, 240);
	questInfoText.render(280, 260);
	acceptText.render(286, 381);
	rejectText.render(466, 381);
}