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

//Used to draw background for a room
bool drawRoom(RoomSize size);

//Used to draw menu
void drawMenu();

//used to draw the main menu
void drawMainMenu();

//used to draw the dungeon
void drawDungeon();

//The window renderer
extern SDL_Renderer* gRenderer;
extern std::vector<SDL_Rect> spriteClips;
extern std::vector<SDL_Rect> buttonSpriteClips;
extern std::vector<SDL_Rect> tileSpriteClips;
//Globally used font
extern TTF_Font *gFont;
//The window we'll be rendering to
extern SDL_Window* gWindow;
extern std::unordered_map<RoomSize, int> room_map;
extern ScreenState state;

#endif