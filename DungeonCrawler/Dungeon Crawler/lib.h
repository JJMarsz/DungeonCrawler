#ifndef LIB_H
#define LIB_H

#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <sstream>

#define TOTAL_MENU_BUTTONS	1

//Screen dimension constants
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	720

//Textbox start
#define TEXT_X			0
#define TEXT_Y 			600

//crucial
#define MAGIC_NUM		123456789

#define TEXT_SIZE		15

//button constants
#define BUTTON_WIDTH	60
#define BUTTON_HEIGHT	60
#define TOTAL_BUTTONS	12
#define CHAR_BUTTON_WIDTH	200
#define CHAR_BUTTON_HEIGHT	520

//town menu and button stuff
#define TOWN_BUTTON_WIDTH	100
#define TOWN_BUTTON_HEIGHT	50
#define LEFT_X				45
#define LEFTRIGHT_Y			SCREEN_HEIGHT - 45
#define RIGHT_X				SCREEN_WIDTH - TOWN_BUTTON_WIDTH - 10

//quest stuff
#define QUEST_Y				280
#define QUEST_ONE_X			200
#define QUEST_TWO_X			360
#define QUEST_THREE_X		520
#define QUEST_PAGE_WIDTH	80
#define QUEST_PAGE_HEIGHT	115

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
	ROOM_MAIN,
	ROOM_MAP,
	DUNGEON,
	TOWN_QUEST_BOARD,
	SELECTED_QUEST,
	SELECTED_QUEST_ACCEPT,
	SELECTED_QUEST_REJECT,
	TOWN_SHOP,
	TOWN_CHAR_UP,
	TOWN_PARTY_UP,
	TOWN_BUTTON_LEFT,
	TOWN_BUTTON_RIGHT
};
enum RoomSize
{
	SMALL,
	MED,
	LARGE
};

#endif