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
#include <queue>
#include <time.h>
#include <chrono>
#include <cmath>
#include <Windows.h>

typedef void(*func)();

#define TOTAL_MENU_BUTTONS	4

//Screen dimension constants
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	720

//Textbox start
#define TEXT_X			0
#define TEXT_Y 			600

//crucial
#define MAGIC_NUM		123456789

#define TEXT_SIZE		15
#define MSG_TEXT_SIZE	18

//button constants
#define BUTTON_WIDTH	50
#define BUTTON_HEIGHT	50
#define TOTAL_BUTTONS	10
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
#define MAX_QUESTS			3
#define NUM_QUEST			11
#define END_GAME			5


#define NUM_TILES				33
//Dead ends
#define	TILE_LEFT_X				0
#define	TILE_LEFT_Y				0

#define	TILE_DOWN_X				50
#define	TILE_DOWN_Y				0

#define	TILE_RIGHT_X			100
#define	TILE_RIGHT_Y			0

#define	TILE_UP_X				150
#define	TILE_UP_Y				0

//Corners
#define	TILE_DOWN_LEFT_X		0
#define	TILE_DOWN_LEFT_Y		50

#define	TILE_LEFT_UP_X			50
#define	TILE_LEFT_UP_Y			50

#define	TILE_UP_RIGHT_X			100
#define	TILE_UP_RIGHT_Y			50

#define	TILE_RIGHT_DOWN_X		150
#define	TILE_RIGHT_DOWN_Y		50

//Hallways
#define	TILE_HORIZ_X			0
#define	TILE_HORIZ_Y			100

#define	TILE_VERT_X				50
#define	TILE_VERT_Y				100

//3 way junctions
#define	TILE_DOWN_LEFT_UP_X		0
#define	TILE_DOWN_LEFT_UP_Y		150

#define	TILE_LEFT_UP_RIGHT_X	50
#define	TILE_LEFT_UP_RIGHT_Y	150

#define	TILE_UP_RIGHT_DOWN_X	100
#define	TILE_UP_RIGHT_DOWN_Y	150

#define	TILE_RIGHT_DOWN_LEFT_X	150
#define	TILE_RIGHT_DOWN_LEFT_Y	150

//the rare but real, 4 way crossroads

#define TILE_FOUR_WAY_X			50
#define TILE_FOUR_WAY_Y			200

//special tiles
#define TILE_HOVER_X			150
#define TILE_HOVER_Y			100

#define TILE_DEAD_X				100
#define TILE_DEAD_Y				100

#define TILE_EMPTY_X			200
#define TILE_EMPTY_Y			0

#define TILE_SEEN_X				200
#define TILE_SEEN_Y				50

#define TILE_PARTY_X			200
#define TILE_PARTY_Y			100

#define TILE_MOB_X				200
#define TILE_MOB_Y				150

#define TILE_INFO_X				200
#define TILE_INFO_Y				200

#define TILE_LOOT_X				150
#define TILE_LOOT_Y				200

#define TILE_BOSS_X				100
#define TILE_BOSS_Y				200

#define TILE_CHOICE_X			0
#define TILE_CHOICE_Y			200

#define TILE_TRAP_X				0
#define TILE_TRAP_Y				250

#define TILE_MED_HOVER_X		0
#define TILE_MED_HOVER_Y		300

#define TILE_BIG_HOVER_X		100
#define TILE_BIG_HOVER_Y		250

//ROOM TILES
#define NUM_ROOM_TILES			12

#define	ROOM_NONE_X				0
#define	ROOM_NONE_Y				0

#define	ROOM_LEFT_X				50
#define	ROOM_LEFT_Y				0

#define	ROOM_UP_X				100
#define	ROOM_UP_Y				0

#define	ROOM_RIGHT_X			150
#define	ROOM_RIGHT_Y			0

#define	ROOM_DOWN_X				200
#define	ROOM_DOWN_Y				0

#define	ROOM_DL_X				50
#define	ROOM_DL_Y				50

#define	ROOM_LU_X				100
#define	ROOM_LU_Y				50

#define	ROOM_UR_X				150
#define	ROOM_UR_Y				50

#define	ROOM_RD_X				200
#define	ROOM_RD_Y				50

#define SELECTED_X				0
#define SELECTED_Y				100

#define SELECTED_INIT_X			0
#define SELECTED_INIT_Y			250

#define INVALID_X				0
#define INVALID_Y				50


//Initiative stuff

#define INIT_X					SCREEN_WIDTH - 62
#define INIT_Y					12



#define TILE_WIDTH				50
#define TILE_HEIGHT				50

#define BUTTON_Y				659
#define BUTTON_ONE_X			11
#define BUTTON_TWO_X			71
#define BUTTON_THREE_X			131
#define BUTTON_FOUR_X			191
#define BUTTON_FIVE_X			251
#define BUTTON_SIX_X			311
#define BUTTON_SEVEN_X			371
#define BUTTON_EIGHT_X			461
#define BUTTON_NINE_X			521
#define BUTTON_TEN_X			581

/* Mob numbering */
#define MOB_Y					0
#define MOBONE_X				0
#define MOBTWO_X				15
#define MOBTHREE_X				30
#define MOBFOUR_X				45
#define MOBFIVE_X				60
/* MOB STUFF */

#define NUM_MOBS				1

#define SKELETON_X				0
#define SKELETON_Y				0

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
	DUNGEON,
	DUNGEON_ROOM,
	CHOOSE,
	TOWN_QUEST_BOARD,
	SELECTED_QUEST,
	SELECTED_QUEST_ACCEPT,
	SELECTED_QUEST_REJECT,
	TOWN_SHOP,
	TOWN_CHAR_UP,
	TOWN_PARTY_UP,
	TOWN_BUTTON_LEFT,
	TOWN_BUTTON_RIGHT,
	REWARD,
	END
};

/* TILE CONSTANTS */
enum {
	LEFT_,
	DOWN_,
	RIGHT_,
	UP_,
	DOWN_LEFT,
	LEFT_UP,
	UP_RIGHT,
	RIGHT_DOWN,
	HORIZ,
	VERT,
	DOWN_LEFT_UP,
	LEFT_UP_RIGHT,
	UP_RIGHT_DOWN,
	RIGHT_DOWN_LEFT,
	FOUR_WAY,
	HOVER,
	DEAD_,
	EMPTY_,
	PARTY,
	BAR,
	BOSSS,
	MOBS,
	LOOTS,
	INFOS,
	CHOICES,
	SEEN,
	TRAP_,
	MED_HOVER,
	BIG_HOVER
};

enum RoomTileIndex {
	NOWALL,
	LEFTWALL,
	UPWALL,
	RIGHTWALL,
	DOWNWALL,
	DLCORNER,
	LUCORNER,
	URCORNER,
	RDCORNER,
	SELECTED,
	SELECTED_INIT,
	INVALID
};

enum AbilityState {
	PEEK,
	SCOUT,
	REST,
	NOPE,
	MOVE,
	ATTACK
};

enum MOD{
	MOD_UP,
	MOD_DOWN
};

enum UpgradeState {
	ONE,
	TWO,
	THREE,
	FOUR
};

enum UnitType {
	CHARACTER,
	ENEMY,
	BOSS_BOI,
	NOTHING,
	RANGE
};

enum MobName {
	SKELETON
};

enum MobNum {
	MOBONE,
	MOBTWO,
	MOBTHREE,
	MOBFOUR,
	MOBFIVE
};

enum CharName {
	FIGHTER,
	BARB,
	PALADIN,
	ROGUE,
	RANGER
};
#endif