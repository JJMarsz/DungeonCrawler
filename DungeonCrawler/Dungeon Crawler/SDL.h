#ifndef SDL_H
#define SDL_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "lib.h"

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//helper to draw text within constraints
int loadText(int w, std::string text);

//Used to draw background for a room
void drawRoom();

//draws units ont the field
void drawUnits();

//Used to draw menu
void drawMenu();

//used to draw the main menu
void drawMainMenu();

//used to draw the dungeon
void drawDungeon();

//draws menu used in dungeon
void drawDungeonMenu();

//draws menu for choice tile within dungeon
void drawChoiceMenu();

//used to draw and pick the characters
void drawCharScreen();

//draw quests and quest board
void drawQuestBoard();

//draws quest info
void drawQuestInfo();

//draws shop
void drawShop();

//draws character upgrade/training place
void drawCharUp();

//draw party upgrade
void drawPartyUp();

//draws the bottom menu for town activities
void drawTownMenu();

//The window renderer
extern SDL_Renderer* gRenderer;
//Globally used font
extern TTF_Font *gFont;
extern TTF_Font* msg_font;
//The window we'll be rendering to
extern SDL_Window* gWindow;
extern ScreenState state;
extern AbilityState ab;
extern int r;

#endif