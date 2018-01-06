#include "SDL.h"
#include "LButton.h"
#include "dungeon.h"

SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
SDL_Window* gWindow = NULL;

std::vector<SDL_Rect> spriteClips;
std::vector<SDL_Rect> buttonSpriteClips;
std::vector<SDL_Rect> tileSpriteClips;
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
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	buttonSpriteClips.resize(4);
	tileSpriteClips.resize(3);
	Buttons.resize(12);
	menuButtons.resize(1);
	menuButtons[0].setHandler(menuClicked);

	for (int i = 0; i<TOTAL_BUTTONS; i++)
		Buttons[i].setConstraints(BUTTON_WIDTH, BUTTON_HEIGHT);
	menuButtons[0].setConstraints(200, BUTTON_HEIGHT);
	//8,6 small
	//16,12 med
	//20, 15 large
	//top 16 bits are width, bottom 16 are height
	room_map[LARGE] = (20 << 16) + 15;
	room_map[MED] = (16 << 16) + 12;
	room_map[SMALL] = (8 << 16) + 6;
	state = MAIN_MENU;

	current_dungeon = Dungeon(HARD);
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("PT_sans.ttf", 18);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else {

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
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			buttonSpriteClips[i].x = 0;
			buttonSpriteClips[i].y = i * 60;
			buttonSpriteClips[i].w = BUTTON_WIDTH;
			buttonSpriteClips[i].h = BUTTON_HEIGHT;
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
	}
	if (!mainMenu.loadFromFile("mainmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!tileSST.loadFromFile("dungeontiles.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		tileSpriteClips[0].x = 0;
		tileSpriteClips[0].y = 0;
		tileSpriteClips[0].w = 50;
		tileSpriteClips[0].h = 50;

		tileSpriteClips[1].x = 0;
		tileSpriteClips[1].y = 50;
		tileSpriteClips[1].w = 50;
		tileSpriteClips[1].h = 50;

		tileSpriteClips[2].x = 0;
		tileSpriteClips[2].y = 100;
		tileSpriteClips[2].w = 50;
		tileSpriteClips[2].h = 50;
	}
	return success;
}

void close() {
	//Free loaded images
	//buttonOneText.free();
	//buttonTwoText.free();
	//buttonThreeText.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Free Textures
	spriteSheetTexture.free();
	buttonSpriteSheetTexture.free();
	tileSST.free();
	mainMenu.free();
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
	menuButtons[0].render();
}

void drawTravel() {

}

void drawTextInBox(int x, int y, int w, int h) {

}

void drawDungeon() {/**/
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
	for (int x = 0; x < current_dungeon.getWidth(); x++) {
		for (int y = 0; y < current_dungeon.getHeight(); y++) {
			if (current_dungeon.getTile(x + y * (current_dungeon.getWidth())).getType() == PATH) {
				tileSST.render(x * 50, y * 50, &tileSpriteClips[1]);


			}
			/*else if (current_dungeon.getTile(x + y * (current_dungeon.getWidth())).getType() == DEADEND)
				;*/
			else
				tileSST.render(x * 50, y * 50, &tileSpriteClips[0]);

		}
	}
}