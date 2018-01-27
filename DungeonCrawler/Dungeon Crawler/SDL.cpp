#include "SDL.h"
#include "Quest.h"
#include "Room.h"
#include "Mob.h"

SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
TTF_Font *msg_font = NULL;
SDL_Window* gWindow = NULL;

ScreenState state;
AbilityState ab = NOPE;
int r = 1000;

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
	tileSpriteClips.resize(NUM_TILES);
	texts.reserve(20);
	charClips.resize(NUM_CHAR*4);
	Buttons.resize(TOTAL_BUTTONS);
	charButtons.resize(3);
	menuButtons.resize(4);
	menuButtons[0].setHandler(newGame);
	menuButtons[1].setHandler(loadGame);
	menuButtons[2].setHandler(tutorial);
	menuButtons[3].setHandler(credits);
	townButtons.resize(2);
	townButtons[0].setHandler(gotoShop);
	townButtons[1].setHandler(gotoCharUp);
	questButtons.resize(3);
	acceptrejectButtons.resize(3);
	healthBoxClips.resize(11);
	dungeonButtons.resize(3);
	multiplyClips.resize(5);
	choiceButtons.resize(4);
	mobClips.resize(NUM_MOBS);
	mobNumClips.resize(5);
	roomTileClips.resize(NUM_ROOM_TILES);
	int i;
	for (i = 0; i<TOTAL_BUTTONS; i++)
		Buttons[i].setConstraints(BUTTON_WIDTH, BUTTON_HEIGHT);
	for (i = 0; i < TOTAL_MENU_BUTTONS; i++)
		menuButtons[i].setConstraints(120, 60);
	for (i = 0; i < 3; i++) 
		charButtons[i].setConstraints(CHAR_BUTTON_WIDTH, CHAR_BUTTON_HEIGHT);
	for (i = 0; i < 3; i++)
		questButtons[i].setConstraints(QUEST_PAGE_WIDTH, QUEST_PAGE_HEIGHT);
	for (i = 0; i < 2; i++)
		townButtons[i].setConstraints(TOWN_BUTTON_WIDTH, TOWN_BUTTON_HEIGHT);
	for (i = 0; i < 3; i++)
		acceptrejectButtons[i].setConstraints(70, 30);
	for (i = 0; i < 3; i++)
		dungeonButtons[i].setConstraints(90, 46);
	for (i = 0; i < 4; i++)
		choiceButtons[i].setConstraints(100, 100);
	endTurnButton.setConstraints(126, 96);
	endTurnButton.setHandler(endTurnHandler);
	acceptrejectButtons[2].setHandler(returnToTown);
	dungeonButtons[2].setHandler(peek);
	dungeonButtons[1].setHandler(scout);
	dungeonButtons[0].setHandler(rest);
	state = MAIN_MENU;
	return success;
}

bool loadMedia()
{

	std::vector<SDL_Rect> clips;
	clips.resize(4);
	int i;
	//Loading success flag
	bool success = true;

	SDL_Color textColor = { 255, 255, 255 };
	//Open the font
	gFont = TTF_OpenFont("PT_sans.ttf", TEXT_SIZE);
	msg_font = TTF_OpenFont("PT_sans.ttf", MSG_TEXT_SIZE);
	messageBox.setFont(msg_font);
	hp0.setFont(msg_font);
	hp1.setFont(msg_font);
	hp2.setFont(msg_font);
	endTurnText.setFont(msg_font);
	endTurnText.loadFromRenderedText(" End Turn", textColor, 40);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = false;
	}
	else {
		leftText.loadFromRenderedText("Shop", textColor, 200);
		rightText.loadFromRenderedText("Training", textColor, 200);
		acceptText.loadFromRenderedText("Accept", textColor, 200);
		rejectText.loadFromRenderedText("Reject", textColor, 200);
		newgametext.loadFromRenderedText("New Game", textColor, 200);
		loadgametext.loadFromRenderedText("Load Game", textColor, 200);
		tutorialtext.loadFromRenderedText("Tutorial", textColor, 200);
		creditstext.loadFromRenderedText("Credits", textColor, 200);

	}


	//Load spritesheet texture
	if (!spriteSheetTexture.loadFromFile("textures/sprites.png")) {
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
	if (!buttonSpriteSheetTexture.loadFromFile("textures/button.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//Set sprites
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			clips[i].x = 0;
			clips[i].y = i * 50;
			clips[i].w = BUTTON_WIDTH;
			clips[i].h = BUTTON_HEIGHT;
		}
		for (i = 0; i < TOTAL_BUTTONS; i++) {
			Buttons[i].setClips(clips);
			Buttons[i].setSST(&buttonSpriteSheetTexture);
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			clips[i].x = 0;
			clips[i].y = (i+4) * 60;
			clips[i].w = 120;
			clips[i].h = 60;
		}
		for (i = 0; i < 4; i++) {
			menuButtons[i].setClips(clips);
			menuButtons[i].setSST(&buttonSpriteSheetTexture);
		}
		for (i = 0; i < BUTTON_SPRITE_TOTAL; ++i) 
		{
			clips[i].x = 120 + (i)*200;
			clips[i].y = 0;
			clips[i].w = CHAR_BUTTON_WIDTH;
			clips[i].h = CHAR_BUTTON_HEIGHT;
		}
		for (i = 0; i < 3; i++) {
			charButtons[i].setClips(clips);
			charButtons[i].setSST(&buttonSpriteSheetTexture);
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

		menuButtons[0].setPosition(145, 610);
		menuButtons[1].setPosition(275, 610);
		menuButtons[2].setPosition(405, 610);
		menuButtons[3].setPosition(535, 610);

		charButtons[0].setPosition(50, 50);
		charButtons[1].setPosition(300, 50);
		charButtons[2].setPosition(550, 50);
	}
	if (!mainMenu.loadFromFile("textures/mainmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!tileSST.loadFromFile("textures/tiles.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		tileSST.setBlendMode(SDL_BLENDMODE_BLEND);
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

		tileSpriteClips[SEEN].x = TILE_SEEN_X;
		tileSpriteClips[SEEN].y = TILE_SEEN_Y;
		tileSpriteClips[SEEN].w = TILE_WIDTH;
		tileSpriteClips[SEEN].h = TILE_HEIGHT;

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

		tileSpriteClips[CHOICES].x = TILE_CHOICE_X;
		tileSpriteClips[CHOICES].y = TILE_CHOICE_Y;
		tileSpriteClips[CHOICES].w = TILE_WIDTH;
		tileSpriteClips[CHOICES].h = TILE_HEIGHT;

		tileSpriteClips[TRAP_].x = TILE_TRAP_X;
		tileSpriteClips[TRAP_].y = TILE_TRAP_Y;
		tileSpriteClips[TRAP_].w = TILE_WIDTH;
		tileSpriteClips[TRAP_].h = TILE_HEIGHT;

		tileSpriteClips[MED_HOVER].x = TILE_MED_HOVER_X;
		tileSpriteClips[MED_HOVER].y = TILE_MED_HOVER_Y;
		tileSpriteClips[MED_HOVER].w = TILE_WIDTH * 2;
		tileSpriteClips[MED_HOVER].h = TILE_HEIGHT * 2;

		tileSpriteClips[BIG_HOVER].x = TILE_BIG_HOVER_X;
		tileSpriteClips[BIG_HOVER].y = TILE_BIG_HOVER_Y;
		tileSpriteClips[BIG_HOVER].w = TILE_WIDTH * 3;
		tileSpriteClips[BIG_HOVER].h = TILE_HEIGHT * 3;

	}
	if (!charSST.loadFromFile("textures/charicons.png")) {
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
			/* med 100x100 */
			charClips[i + NUM_CHAR].x = i * 200;
			charClips[i + NUM_CHAR].y = 200;
			charClips[i + NUM_CHAR].w = 100;
			charClips[i + NUM_CHAR].h = 100;
			/* small 50x50 */
			charClips[i + 2*NUM_CHAR].x = i * 200 + 100;
			charClips[i + 2*NUM_CHAR].y = 200;
			charClips[i + 2*NUM_CHAR].w = 50;
			charClips[i + 2*NUM_CHAR].h = 50;
			/* small 25x25 */
			charClips[i + 3*NUM_CHAR].x = i * 200 + 150;
			charClips[i + 3*NUM_CHAR].y = 200;
			charClips[i + 3*NUM_CHAR].w = 25;
			charClips[i + 3*NUM_CHAR].h = 25;
		}
	}
	chars.loadSprites(); 
	if (!townmenu.loadFromFile("textures/townmenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!townButtonSST.loadFromFile("textures/townbuttons.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		//left button
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = 0;
			clips[i].y = i * 50;
			clips[i].w = 100;
			clips[i].h = 50;
		}
		townButtons[0].setSST(&townButtonSST);
		townButtons[0].setClips(clips);
		//right button
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = 100;
			clips[i].y = i * 50;
			clips[i].w = 100;
			clips[i].h = 50;
		}
		townButtons[1].setSST(&townButtonSST);
		townButtons[1].setClips(clips);

		townButtons[0].setPosition(25, SCREEN_HEIGHT - 60);
		townButtons[1].setPosition(SCREEN_WIDTH - 125, SCREEN_HEIGHT - 60);
	}
	if (!questboard.loadFromFile("textures/questboard.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!questSST.loadFromFile("textures/quest.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = QUEST_PAGE_WIDTH * i;
			clips[i].y = 0;
			clips[i].w = QUEST_PAGE_WIDTH;
			clips[i].h = QUEST_PAGE_HEIGHT;
		}
		for (i = 0; i < 3; i++) {
			questButtons[i].setSST(&questSST);
			questButtons[i].setClips(clips);
		}
		questButtons[0].setPosition(QUEST_ONE_X, QUEST_Y);
		questButtons[1].setPosition(QUEST_TWO_X, QUEST_Y);
		questButtons[2].setPosition(QUEST_THREE_X, QUEST_Y);

	}
	if (!acceptrejectSST.loadFromFile("textures/acceptreject.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = 0;
			clips[i].y = i * 30;
			clips[i].w = 70;
			clips[i].h = 30;
		}
		acceptrejectButtons[0].setSST(&acceptrejectSST);
		acceptrejectButtons[0].setClips(clips);
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = 70;
			clips[i].y = i * 30;
			clips[i].w = 70;
			clips[i].h = 30;
		}
		acceptrejectButtons[1].setSST(&acceptrejectSST);
		acceptrejectButtons[1].setClips(clips);
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].x = 140;
			clips[i].y = i * 30;
			clips[i].w = 70;
			clips[i].h = 30;
		}
		acceptrejectButtons[2].setSST(&acceptrejectSST);
		acceptrejectButtons[2].setClips(clips);

		acceptrejectButtons[0].setPosition(275, 377);
		acceptrejectButtons[1].setPosition(455, 377);
		acceptrejectButtons[2].setPosition(365, 377);
	}
	if (!questinfo.loadFromFile("textures/questinfo.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	initQuests();
	quests.getQuests();
	if (!shop.loadFromFile("textures/shop.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!training.loadFromFile("textures/training.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!upgrades.loadFromFile("textures/upgrades.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if(!dungeonmenu.loadFromFile("textures/dungeonmenu.png")){
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!healthboxSST.loadFromFile("textures/healthbox.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < 11; i++) {
			healthBoxClips[i].x = i*50;
			healthBoxClips[i].y = 0;
			healthBoxClips[i].w = 50;
			healthBoxClips[i].h = 50;
		}
	}
	if (!dungeonButtonSST.loadFromFile("textures/dungeonbutton.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].y = i * 46;
			clips[i].x = 0;
			clips[i].w = 90;
			clips[i].h = 46;
		}
		for (i = 0; i < 3; i++) {
			dungeonButtons[i].setSST(&dungeonButtonSST);
			dungeonButtons[i].setClips(clips);
		}
		dungeonButtons[0].setPosition(498, SCREEN_HEIGHT - 58);
		dungeonButtons[1].setPosition(598, SCREEN_HEIGHT - 58);
		dungeonButtons[2].setPosition(698, SCREEN_HEIGHT - 58);
	}
	if (!multiplierSST.loadFromFile("textures/multiply.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < 5; i++) {
			multiplyClips[i].x = i * 22;
			multiplyClips[i].y = 0;
			multiplyClips[i].w = 22;
			multiplyClips[i].h = 22;
		}
	}
	if (!choicemenu.loadFromFile("textures/choicemenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!choiceButtonSST.loadFromFile("textures/choicebutton.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].y = i * 100;
			clips[i].x = 0;
			clips[i].w = 100;
			clips[i].h = 100;
		}
		for (i = 0; i < 4; i++) {
			choiceButtons[i].setSST(&choiceButtonSST);
			choiceButtons[i].setClips(clips);
			choiceButtons[i].setPosition((SCREEN_WIDTH - CHOICE_MENU_WIDTH) / 2 + 12 + 110 * i, 212);
		}
	}
	if (!no.loadFromFile("textures/no.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!dungeonroom.loadFromFile("textures/dungeonroommenu.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!roomTilesSST.loadFromFile("textures/roomtiles.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {

		roomTilesSST.setBlendMode(SDL_BLENDMODE_BLEND);
		roomTileClips[NOWALL].x = ROOM_NONE_X;
		roomTileClips[NOWALL].y = ROOM_NONE_Y;
		roomTileClips[NOWALL].w = TILE_WIDTH;
		roomTileClips[NOWALL].h = TILE_HEIGHT;

		roomTileClips[LEFTWALL].x = ROOM_LEFT_X;
		roomTileClips[LEFTWALL].y = ROOM_LEFT_Y;
		roomTileClips[LEFTWALL].w = TILE_WIDTH;
		roomTileClips[LEFTWALL].h = TILE_HEIGHT;

		roomTileClips[UPWALL].x = ROOM_UP_X;
		roomTileClips[UPWALL].y = ROOM_UP_Y;
		roomTileClips[UPWALL].w = TILE_WIDTH;
		roomTileClips[UPWALL].h = TILE_HEIGHT;

		roomTileClips[RIGHTWALL].x = ROOM_RIGHT_X;
		roomTileClips[RIGHTWALL].y = ROOM_RIGHT_Y;
		roomTileClips[RIGHTWALL].w = TILE_WIDTH;
		roomTileClips[RIGHTWALL].h = TILE_HEIGHT;

		roomTileClips[DOWNWALL].x = ROOM_DOWN_X;
		roomTileClips[DOWNWALL].y = ROOM_DOWN_Y;
		roomTileClips[DOWNWALL].w = TILE_WIDTH;
		roomTileClips[DOWNWALL].h = TILE_HEIGHT;

		roomTileClips[DLCORNER].x = ROOM_DL_X;
		roomTileClips[DLCORNER].y = ROOM_DL_Y;
		roomTileClips[DLCORNER].w = TILE_WIDTH;
		roomTileClips[DLCORNER].h = TILE_HEIGHT;

		roomTileClips[LUCORNER].x = ROOM_LU_X;
		roomTileClips[LUCORNER].y = ROOM_LU_Y;
		roomTileClips[LUCORNER].w = TILE_WIDTH;
		roomTileClips[LUCORNER].h = TILE_HEIGHT;

		roomTileClips[URCORNER].x = ROOM_UR_X;
		roomTileClips[URCORNER].y = ROOM_UR_Y;
		roomTileClips[URCORNER].w = TILE_WIDTH;
		roomTileClips[URCORNER].h = TILE_HEIGHT;

		roomTileClips[RDCORNER].x = ROOM_RD_X;
		roomTileClips[RDCORNER].y = ROOM_RD_Y;
		roomTileClips[RDCORNER].w = TILE_WIDTH;
		roomTileClips[RDCORNER].h = TILE_HEIGHT;

		roomTileClips[SELECTED_INIT].x = SELECTED_INIT_X;
		roomTileClips[SELECTED_INIT].y = SELECTED_INIT_Y;
		roomTileClips[SELECTED_INIT].w = 146;
		roomTileClips[SELECTED_INIT].h = 62;

		roomTileClips[SELECTED].x = SELECTED_X;
		roomTileClips[SELECTED].y = SELECTED_Y;
		roomTileClips[SELECTED].w = TILE_WIDTH;
		roomTileClips[SELECTED].h = TILE_HEIGHT;

		roomTileClips[INVALID].x = INVALID_X;
		roomTileClips[INVALID].y = INVALID_Y;
		roomTileClips[INVALID].w = TILE_WIDTH;
		roomTileClips[INVALID].h = TILE_HEIGHT;
	}
	if (!background.loadFromFile("textures/background.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	if (!endTurn.loadFromFile("textures/endturn.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		for (i = 0; i < BUTTON_SPRITE_TOTAL; i++) {
			clips[i].y = i * 96;
			clips[i].x = 0;
			clips[i].w = 126;
			clips[i].h = 96;
		}
		endTurnButton.setSST(&endTurn);
		endTurnButton.setClips(clips);
		endTurnButton.setPosition(662, 612);
	}
	if (!mobSST.loadFromFile("textures/mobicons.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		mobClips[SKELETON].x = SKELETON_X;
		mobClips[SKELETON].y = SKELETON_Y;
		mobClips[SKELETON].w = 50;
		mobClips[SKELETON].h = 50;

	}
	if (!mobNum.loadFromFile("textures/mobnum.png")) {
		printf("Failed to load texture image!\n");
		success = false;
	}
	else {
		mobNumClips[MOBONE].x = MOBONE_X;
		mobNumClips[MOBONE].y = MOB_Y;
		mobNumClips[MOBONE].w = 15;
		mobNumClips[MOBONE].h = 15;

		mobNumClips[MOBTWO].x = MOBTWO_X;
		mobNumClips[MOBTWO].y = MOB_Y;
		mobNumClips[MOBTWO].w = 15;
		mobNumClips[MOBTWO].h = 15;

		mobNumClips[MOBTHREE].x = MOBTHREE_X;
		mobNumClips[MOBTHREE].y = MOB_Y;
		mobNumClips[MOBTHREE].w = 15;
		mobNumClips[MOBTHREE].h = 15;

		mobNumClips[MOBFOUR].x = MOBFOUR_X;
		mobNumClips[MOBFOUR].y = MOB_Y;
		mobNumClips[MOBFOUR].w = 15;
		mobNumClips[MOBFOUR].h = 15;

		mobNumClips[MOBFIVE].x = MOBFIVE_X;
		mobNumClips[MOBFIVE].y = MOB_Y;
		mobNumClips[MOBFIVE].w = 15;
		mobNumClips[MOBFIVE].h = 15;
	}
	loadMobs();
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
	TTF_CloseFont(msg_font);
	msg_font = NULL;
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
	dungText1.free();
	dungText2.free();
	dungText3.free();
	newgametext.free();
	loadgametext.free();
	tutorialtext.free();
	creditstext.free();
	messageBox.free();
	hp0.free();
	hp1.free();
	hp2.free();
	multiplierSST.free();
	no.free();
	dungeonroom.free();
	roomTilesSST.free();
	background.free();
	endTurn.free();
	endTurnText.free();
	mobSST.free();
	mobNum.free();

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
	dungeonmenu.free();
	healthboxSST.free();
	dungeonButtonSST.free();
	choicemenu.free();
	choiceButtonSST.free();

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

 RoomTileIndex getRoomTileIndex(int x, int y) {
	RoomTile* tile = room->getTile(x, y);
	//bitwise
	//	L	U	R	D
	int index = 0;
	if (tile->down)
		index += 1;
	if (tile->right)
		index += 2;
	if (tile->up)
		index += 4;
	if (tile->left)
		index += 8;

	switch (index) {
	case 1:
		return DOWNWALL;
	case 2:
		return RIGHTWALL;
	case 3:
		return RDCORNER;
	case 4:
		return UPWALL;
	case 6:
		return URCORNER;
	case 8:
		return LEFTWALL;
	case 9:
		return DLCORNER;
	case 12:
		return LUCORNER;
	case 15:
	default:
		return NOWALL;
	}
}		

/* drawing helper functions */
void drawRoom() {
	int x, y;
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
	dungeonroom.render(0, 0);
	background.setColor(r / 100, r / 100, r / 100);
	background.render(0, 0);
	background.setColor(255, 255, 255);
	endTurnButton.render();
	endTurnText.render(662 + (126 - endTurnText.getWidth())/2, 612 + (96 - endTurnText.getHeight())/2);
	//find x and y offset to center map
	int start_x = (650 - 50 * room->getWidth()) / 2;
	int start_y = (600 - 50 * room->getHeight()) / 2;
	//draw tiles
	for (x = 0; x < room->getWidth(); x++) {
		for (y = 0; y < room->getHeight(); y++) {
			switch (room->getTile(x, y)->color) {
			case RED:
				roomTilesSST.setColor(255, 100, 100);
				break;
			case YELLOW:
				roomTilesSST.setColor(239, 228, 176);
				break;
			}
			roomTilesSST.render(x * 50 + start_x, y * 50 + start_y, &roomTileClips[getRoomTileIndex(x, y)]);
			roomTilesSST.setColor(255, 255, 255);
		}
	}
	int mob_count = 0;
	//draw units onto field
	std::vector<Unit*>* order = room->getInititiveOrder();
	for (x = 0; x < order->size(); x++) {
		switch (order->at(x)->getType()) {
		case CHARACTER:
			charSST.render(start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &order->at(x)->getIcon50());
			break;
		case ENEMY:
			mobSST.render(start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &order->at(x)->getIcon50());
			switch (mob_count) {
			case 0:
				mobNum.render(33 + start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), 33 + start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &mobNumClips[MOBONE]);
				break;
			case 1:
				mobNum.render(33 + start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), 33 + start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &mobNumClips[MOBTWO]);
				break;
			case 2:
				mobNum.render(33 + start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), 33 + start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &mobNumClips[MOBTHREE]);
				break;
			case 3:
				mobNum.render(33 + start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), 33 + start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &mobNumClips[MOBFOUR]);
				break;
			case 4:
				mobNum.render(33 + start_x + 50 * (order->at(x)->getRMO() % room->getWidth()), 33 + start_y + 50 * (order->at(x)->getRMO() / room->getWidth()), &mobNumClips[MOBFIVE]);
				break;
			}
			mob_count++;
			break;
		case BOSS_BOI:

			break;
		}
	}
	mob_count = 0;
	//highlight current char
	Unit* currUnit = room->getCurrUnit();
	roomTilesSST.render(start_x + (currUnit->getRMO() % room->getWidth())*50, start_y + 50*(currUnit->getRMO() / room->getWidth()), &roomTileClips[SELECTED]);
	roomTilesSST.render(SCREEN_WIDTH - 148, INIT_Y - 6 + 60*room->getInitIndex(), &roomTileClips[SELECTED_INIT]);
	//draw the initiative order
	double health_ratio;
	int char_count = 0;
	double slice, diff, ratio;
	SDL_Color textColor = { 255, 255, 255 };
	for (x = 0; x < order->size(); x++){
		slice = (double)(order->at(x)->getMaxHP()) / 11;
		diff = (order->at(x)->getMaxHP() - order->at(x)->getHP());
		ratio = diff / slice;
		ratio += .5;
		health_ratio = (int)ratio;
		healthBoxClips[health_ratio].w = 20;
		healthboxSST.render(INIT_X - 25, INIT_Y + 60 * x, &healthBoxClips[health_ratio]);
		healthBoxClips[health_ratio].w = 50;
		switch (order->at(x)->getType()) {
		case CHARACTER:
			charSST.render(INIT_X, INIT_Y + 60 * x, &order->at(x)->getIcon50());
			switch (char_count) {
			case 0:
				hp0.loadFromRenderedText(std::to_string(order->at(x)->getHP()) + "/" + std::to_string(order->at(x)->getMaxHP()), textColor, 200);
				hp0.render(INIT_X - 82, INIT_Y + 12 + x*60);
				char_count++;
				break;
			case 1:
				hp1.loadFromRenderedText(std::to_string(order->at(x)->getHP()) + "/" + std::to_string(order->at(x)->getMaxHP()), textColor, 200);
				hp1.render(INIT_X - 82, INIT_Y + 12 + x * 60);
				char_count++;
				break;
			case 2:
				hp2.loadFromRenderedText(std::to_string(order->at(x)->getHP()) + "/" + std::to_string(order->at(x)->getMaxHP()), textColor, 200);
				hp2.render(INIT_X - 82, INIT_Y + 12 + x * 60);
				char_count++;
				break;
			}
			break;
		case ENEMY:

			mobSST.render(INIT_X, INIT_Y + 60 * x, &order->at(x)->getIcon50());
			switch (mob_count) {
			case 0:
				mobNum.render(33 + INIT_X, 33 + INIT_Y + 60*x, &mobNumClips[MOBONE]);
				break;
			case 1:
				mobNum.render(33 + INIT_X, 33 + INIT_Y + 60 * x, &mobNumClips[MOBTWO]);
				break;
			case 2:
				mobNum.render(33 + INIT_X, 33 + INIT_Y + 60 * x, &mobNumClips[MOBTHREE]);
				break;
			case 3:
				mobNum.render(33 + INIT_X, 33 + INIT_Y + 60 * x, &mobNumClips[MOBFOUR]);
				break;
			case 4:
				mobNum.render(33 + INIT_X, 33 + INIT_Y + 60 * x, &mobNumClips[MOBFIVE]);
				break;
			}
			mob_count++;
			break;
		case BOSS_BOI:

			break;
		}
	}

	//setup abilities
	std::vector<Ability*> abList = currUnit->getAbilities();
	for (x = 0; x < abList.size(); x++) {
		Buttons[x].setHandler(abList[x]->getButtonHandler());
		Buttons[x].render();
	}
}

void drawUnits() {

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
	for(int i=0;i<TOTAL_MENU_BUTTONS;i++)
		menuButtons[i].render();
	newgametext.render(164, 630);
	loadgametext.render(294, 630);
	tutorialtext.render(434, 630);
	creditstext.render(568, 630);
}

int getTileIndex(int x, int y) {
	if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth()))->getType() == NONE)
		return EMPTY_;
	/*if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth())).getType() == DEADEND)
		return DEAD_;*/
	if (current_dungeon.getTile(x + (y) * (current_dungeon.getWidth()))->getType() == BARRIER)
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
		if (current_dungeon.getTile(x + (y-1) * (current_dungeon.getWidth()))->getType() != NONE
			&& current_dungeon.getTile(x + (y - 1) * (current_dungeon.getWidth()))->getType() != BARRIER)
			adj += 1;
	}
	//check left
	if (x > 0) {
		if (current_dungeon.getTile(x - 1 + (y) * (current_dungeon.getWidth()))->getType() != NONE
			&& current_dungeon.getTile(x - 1 + (y) * (current_dungeon.getWidth()))->getType() != BARRIER)
			adj += 2;
	}
	//check down
	if (y < current_dungeon.getHeight()-1) {
		if (current_dungeon.getTile(x + (y + 1) * (current_dungeon.getWidth()))->getType() != NONE
			&& current_dungeon.getTile(x + (y + 1) * (current_dungeon.getWidth()))->getType() != BARRIER)
			adj += 4;
	}
	//check right
	if (x < current_dungeon.getWidth() - 1) {
		if (current_dungeon.getTile(x + 1 + (y) * (current_dungeon.getWidth()))->getType() != NONE
			&& current_dungeon.getTile(x + 1 + (y) * (current_dungeon.getWidth()))->getType() != BARRIER)
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
int encounterExists(int x, int y, bool alt) {
	if (gParty.getX() == x && gParty.getY() == y)
		return -1;
	EncounterType target;
	if (!alt) 
		target = current_dungeon.getTile(x + y * current_dungeon.getWidth())->getType();
	else
		target = current_dungeon.getTile(x + y * current_dungeon.getWidth())->getAlt();

	switch (target) {
	case BOSS:
		return BOSSS;
	case MOB:
		return MOBS;
	case LOOT:
		return LOOTS;
	case INFO:
		return INFOS;
	case CHOICE:
		return CHOICES;
	case TRAP:
		return TRAP_;
	default:
		return -1;
	}
}

bool charAdj(int x, int y) {
	int RMO = gParty.getX() + gParty.getY()*current_dungeon.getWidth();
	int trap_RMO = x + y * current_dungeon.getWidth();
	if (RMO + 1 == trap_RMO || RMO - 1 == trap_RMO || RMO - current_dungeon.getWidth() == trap_RMO || RMO + current_dungeon.getWidth() == trap_RMO)
		return true;
	return false;
}

void drawChoiceMenu() {

	choicemenu.render((SCREEN_WIDTH - CHOICE_MENU_WIDTH) / 2, 200);
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].render();
		if(i < 3)
			charSST.render((SCREEN_WIDTH - CHOICE_MENU_WIDTH) / 2 + 12 + 110 * i, 212, &gParty.getChar(i)->getIcon100());
	}
	no.render((SCREEN_WIDTH - CHOICE_MENU_WIDTH) / 2 + 342, 212);
	
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
			if (current_dungeon.getSightStatus(x + y * current_dungeon.getWidth())){
				int index = getTileIndex(x, y);
				if (index == EMPTY_) {
					tileSST.setColor(r / 5, r / 5, r / 5);
					tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[SEEN]);
					tileSST.setColor(255, 255, 255);
				}
				else if(!current_dungeon.getVisited(x + y * current_dungeon.getWidth())){
					tileSST.setColor(200, 200, 200);
					tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
					tileSST.setColor(255, 255, 255);
				}
				else
					tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
				//do scout check

				//check if something is at this tile
				index = encounterExists(x, y, false);
				if (index != -1 && current_dungeon.getScouted(x + y * current_dungeon.getWidth()) && !current_dungeon.getVisited(x + y * current_dungeon.getWidth())) {
					tileSST.setAlpha(100);
					std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
						std::chrono::system_clock::now().time_since_epoch()
						);
					srand(ms.count());//random seed
					int alt_index = encounterExists(x, y, true);
					if (rand() % 2 == 1) {
						tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
						tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[alt_index]);
					}
					else {
						tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[alt_index]);
						tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[index]);
					}
					tileSST.setAlpha(255);
				}
			}
			else {
				tileSST.setColor(r / 5, r / 5, r / 5);
				tileSST.render(x * 50 + start_x, y * 50, &tileSpriteClips[EMPTY_]);
				tileSST.setColor(255,255, 255);
			}
		}
	}
	//draw any special tiles
	tileSST.render(gParty.getX() * 50 + start_x, gParty.getY() * 50, &tileSpriteClips[PARTY]);
}

void drawDungeonMenu() {
	//dungeon menu stuff
	int health_ratio;
	SDL_Color textColor = { 255, 255, 255 };
	dungeonmenu.render(0, SCREEN_HEIGHT - 120);
	goldmenu.loadFromRenderedText(std::to_string(gParty.getGold()), textColor, 200);
	goldmenu.render(400, SCREEN_HEIGHT - 45);
	//all char stuff
	for (int i = 0; i < 3; i++) {
		charSST.render(10 + i*120, SCREEN_HEIGHT - 60, &gParty.getChar(i)->getIcon50());
		double slice = (double)(gParty.getChar(i)->getMaxHP()) / 11;
		double diff = (gParty.getChar(i)->getMaxHP() - gParty.getChar(i)->getHP());
		double ratio = diff / slice;
		ratio += .5;
		health_ratio =  (int)ratio;
		healthboxSST.render(70 + 120*i, SCREEN_HEIGHT - 60, &healthBoxClips[health_ratio]);
		dungeonButtons[i].render();
		
	}
	multiplierSST.render(SCREEN_WIDTH - 225, SCREEN_HEIGHT - 65, &multiplyClips[gParty.getRest()]);
	multiplierSST.render(SCREEN_WIDTH - 125, SCREEN_HEIGHT - 65, &multiplyClips[gParty.getScout()]);
	multiplierSST.render(SCREEN_WIDTH - 25, SCREEN_HEIGHT - 65, &multiplyClips[gParty.getPeek()]);

	hp0.loadFromRenderedText(std::to_string(gParty.getChar(0)->getHP()) + "/" + std::to_string(gParty.getChar(0)->getMaxHP()), textColor, 200);
	hp1.loadFromRenderedText(std::to_string(gParty.getChar(1)->getHP()) + "/" + std::to_string(gParty.getChar(1)->getMaxHP()), textColor, 200);
	hp2.loadFromRenderedText(std::to_string(gParty.getChar(2)->getHP()) + "/" + std::to_string(gParty.getChar(2)->getMaxHP()), textColor, 200);

	hp0.render(70, SCREEN_HEIGHT - 48);
	hp1.render(190, SCREEN_HEIGHT - 48);
	hp2.render(310, SCREEN_HEIGHT - 48);

	dungText1.render(526, SCREEN_HEIGHT - 46);
	dungText2.render(622, SCREEN_HEIGHT - 46);
	dungText3.render(725, SCREEN_HEIGHT - 46);
}

void drawCharScreen() {
	int i;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
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
	charSST.render(246, SCREEN_HEIGHT - 47, &gParty.getChar(0)->getIcon25());
	charSST.render(332, SCREEN_HEIGHT - 47, &gParty.getChar(1)->getIcon25());
	charSST.render(418, SCREEN_HEIGHT - 47, &gParty.getChar(2)->getIcon25());

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