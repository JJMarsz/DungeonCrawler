#include "SDL.h"
//#include "party.h"
//#include "dungeon.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>





//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 720;

//Textbox start
const int TEXT_X = 0;
const int TEXT_Y = 600;

//button constants
const int BUTTON_WIDTH = 60;
const int BUTTON_HEIGHT = 60;
const int TOTAL_BUTTONS = 12;
const int BUTTON_Y = SCREEN_HEIGHT - BUTTON_HEIGHT - 5;
const int BUTTON_ONE_X = SCREEN_WIDTH - BUTTON_WIDTH - 18;
const int BUTTON_TWO_X = SCREEN_WIDTH - 2 * BUTTON_WIDTH - 22;
const int BUTTON_THREE_X = SCREEN_WIDTH - 3 * BUTTON_WIDTH - 26;
const int BUTTON_FOUR_X = SCREEN_WIDTH - 4 * BUTTON_WIDTH - 30;
const int BUTTON_FIVE_X = SCREEN_WIDTH - 5 * BUTTON_WIDTH - 34;
const int BUTTON_SIX_X = SCREEN_WIDTH - 6 * BUTTON_WIDTH - 38;
const int BUTTON_SEVEN_X = SCREEN_WIDTH - 7 * BUTTON_WIDTH - 42;
const int BUTTON_EIGHT_X = SCREEN_WIDTH - 8 * BUTTON_WIDTH - 46;
const int BUTTON_NINE_X = SCREEN_WIDTH - 9 * BUTTON_WIDTH - 50;
const int BUTTON_TEN_X = SCREEN_WIDTH - 10 * BUTTON_WIDTH - 54;
const int BUTTON_ELEVEN_X = SCREEN_WIDTH - 11 * BUTTON_WIDTH - 58;
const int BUTTON_TWELVE_X = SCREEN_WIDTH - 12 * BUTTON_WIDTH - 62;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};
enum ScreenState
{
	MAIN_MENU,
	LOAD_GAME,
	PICK_CHAR1,
	PICK_CHAR2,
	PICK_CHAR3,
	NEW_GAME,
	TRAVEL,
	ROOM_MAIN,
	ROOM_MAP
};
enum RoomSize
{
	SMALL,
	MED,
	LARGE
};

#define TOTAL_MENU_BUTTONS	1
//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//Used to draw background for a room
bool drawRoom(RoomSize size);

//Used to draw menu
void drawMenu();

//used to draw the main menu
void drawMainMenu();


class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Deallocates texture
	void free();

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip = NULL);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture * mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	//Allows you to set button sprite
	void setSprite(LButtonSprite newsprite);

	//Shows button sprite
	void render();

	//set button clicked handler
	void setHandler(void(*new_handler)(void));

	//set size
	void setConstraints(int w, int h);

	//getters
	int getWidth();
	int getHeight();

private:
	//Top left position
	SDL_Point mPosition;

	//contraints
	int width, height;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;

	//Button Clicked
	bool MouseDown, MouseUp;

	//Button Clicked function handler
	void(*handler)(void);

	//disabled
	bool disabled;

	//what to display on button
	std::string buttonText;
	LTexture buttonImage;
};

void menuClicked(void);

void emptyHandler(void);
//The window renderer
SDL_Renderer* gRenderer = NULL;

//Character spritesheet along with clips
LTexture spriteSheetTexture;
std::vector<SDL_Rect> spriteClips;

//button spritesheet along with clips
LTexture buttonSpriteSheetTexture;
LTexture mainMenu;
std::vector<SDL_Rect> buttonSpriteClips;
std::vector<LButton> Buttons;
std::vector<LButton> menuButtons;
//Globally used font
TTF_Font *gFont = NULL;

//Rendered texts
LTexture buttonOneText;
LTexture buttonTwoText;
LTexture buttonThreeText;
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

std::unordered_map<RoomSize, int> room_map;
ScreenState state;




/* Texture class defenitions */
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

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
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 255, 150, 200));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor){
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL){
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}





/* Button class defenitions */
LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	MouseDown = false;
	MouseUp = false;
	disabled = false;
	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
	handler = emptyHandler;
	buttonText = "";
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e){
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		//Check if mouse is in button
		bool inside = true;

		//Mouse is outside of the button
		if (x < mPosition.x || x > mPosition.x + width || y < mPosition.y || y > mPosition.y + height){
			inside = false;
		}//Mouse is outside button
		if (!inside){
			mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
			MouseUp = false;
			MouseDown = false;
		}
		//Mouse is inside button
		else{
			//Set mouse over sprite
			switch (e->type){
			case SDL_MOUSEMOTION:
				if (mCurrentSprite != BUTTON_SPRITE_MOUSE_DOWN)
					mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				MouseDown = true;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				MouseUp = true;
				break;
			}
		}
		//button was clicked
		if (MouseDown == true && MouseUp == true) {
			handler();
		}
	}
}

void LButton::setSprite(LButtonSprite newsprite) {
	mCurrentSprite = newsprite;
}

void LButton::render(){
	//Show current button sprite
	buttonSpriteSheetTexture.render(mPosition.x, mPosition.y, &buttonSpriteClips[mCurrentSprite]);
}

void LButton::setHandler(void(*new_handler)(void)) {
	handler = new_handler;
}

void LButton::setConstraints(int w, int h) {
	width = w;
	height = h;
}

int LButton::getWidth() {
	return width;
}

int LButton::getHeight() {
	return height;
}

//different button clicked function handlers
void menuClicked(void) {
	//generateDungeon();
	state = TRAVEL;
}

void emptyHandler(void) {
	return;
}




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
	Buttons.resize(12);
	menuButtons.resize(1);
	menuButtons[0].setHandler(menuClicked);

	for(int i=0;i<TOTAL_BUTTONS;i++)
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
	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont("PT_sans.ttf", 18);
	if (gFont == NULL){
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else{
		
	}

	//Load spritesheet texture
	if (!spriteSheetTexture.loadFromFile("sprites.png")){
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

		menuButtons[0].setPosition(SCREEN_WIDTH/2 - menuButtons[0].getWidth()/2, SCREEN_HEIGHT/2 + menuButtons[0].getHeight() / 2);
	}
	if (!mainMenu.loadFromFile("mainmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	return success;
}

void close(){
	//Free loaded images
	buttonOneText.free();
	buttonTwoText.free();
	buttonThreeText.free();

	//Free global font
	TTF_CloseFont(gFont);
	gFont = NULL;

	//Free Textures
	spriteSheetTexture.free();
	buttonSpriteSheetTexture.free();
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
	for(int i=1;i< height;i++)
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
	SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT-120, SCREEN_WIDTH, SCREEN_HEIGHT-120);
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

/* dungeon helper functions */
void generateDungeon() {
	//this creates a dungeon stored in a global variable

}


int main(int argc, char* args[])
{
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					switch (state) {
					case MAIN_MENU:
						//Handle button events
						for (int i = 0; i < TOTAL_MENU_BUTTONS; ++i)
						{
							menuButtons[i].handleEvent(&e);
						}
						break;
					case ROOM_MAIN:
						//Handle button events
						for (int i = 0; i < TOTAL_BUTTONS; ++i)
						{
							Buttons[i].handleEvent(&e);
						}
						break;
					default:
						break;
					}


				}

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Poll keyboard state
				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
				switch (state) {
				case MAIN_MENU:
					drawMainMenu();
					break;
				case PICK_CHAR1:

					break;
				case PICK_CHAR2:

					break;
				case PICK_CHAR3:

					break;
				case NEW_GAME:

					break;
				case TRAVEL:
					drawTravel();
					break;
				case ROOM_MAIN:
					//Draw menu bar
					drawMenu();
					//for testing
					RoomSize size = LARGE;
					drawRoom(size);
					switch (size) {
					case LARGE:
						//render sprite
						spriteSheetTexture.render(40, 40, &(spriteClips[2]));
						spriteSheetTexture.render(80, 80, &(spriteClips[3]));

						break;
					case MED:
						//render sprite
						spriteSheetTexture.render(50, 50, &(spriteClips[0]));
						spriteSheetTexture.render(100, 100, &(spriteClips[1]));
						break;

					case SMALL:
						break;
					}
					break;
				}
				
				//Update screen
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

