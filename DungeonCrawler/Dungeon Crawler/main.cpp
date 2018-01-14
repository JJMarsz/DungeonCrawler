
//#include "party.h"
//#include "dungeon.h"

#include "lib.h"
#include "LButton.h"
#include "SDL.h"
#include "dungeon.h"

bool hover;

int main(int argc, char* args[])
{
	int i, x=0, y=0, start_x=0;
	int prev_x = -1, prev_y = -1;
	bool MouseDown = false;
	bool MouseUp = false;
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
					switch (e.type) {
					case SDL_QUIT:
						quit = true;
						break;
					case SDL_MOUSEBUTTONDOWN:
						MouseDown = true;
						break;
					case SDL_MOUSEBUTTONUP:
						MouseUp = true;
						break;
					default:
						break;
					}
					//cicking state machine
					switch (state) {
					case MAIN_MENU:
						//Handle button events
						for (i = 0; i < TOTAL_MENU_BUTTONS; ++i)
						{
							menuButtons[i].handleEvent(&e, i);
						}
						break;
					case PICK_CHAR1:
						for (i = 0; i < 3; ++i) {
							charButtons[i].handleEvent(&e, i);
						}
						break;
					case PICK_CHAR2:
						for (i = 0; i < 3; ++i) {
							charButtons[i].handleEvent(&e, i);
						}
						break;
					case PICK_CHAR3:
						for (i = 0; i < 3; ++i) {
							charButtons[i].handleEvent(&e, i);
						}
						break;
					case TOWN_QUEST_BOARD:
						for (i = 0; i < 3; ++i) {
							questButtons[i].handleEvent(&e, i);
						}
						townButtons[0].handleEvent(&e, 0);
						townButtons[1].handleEvent(&e, 1);
						break;
					case SELECTED_QUEST:
						acceptrejectButtons[0].handleEvent(&e, 0);
						acceptrejectButtons[1].handleEvent(&e, 1);
						break;
					case TOWN_CHAR_UP:

						townButtons[0].handleEvent(&e, 0);
						townButtons[1].handleEvent(&e, 1);
						break;
					case TOWN_PARTY_UP:

						townButtons[0].handleEvent(&e, 0);
						townButtons[1].handleEvent(&e, 1);
						break;
					case TOWN_SHOP:

						townButtons[0].handleEvent(&e, 0);
						townButtons[1].handleEvent(&e, 1);
						break;
					case DUNGEON:
						start_x = (SCREEN_WIDTH - current_dungeon.getWidth() * 50) / 2;
						SDL_GetMouseState(&x, &y);
						hover = false;
						//within bounds
						if (start_x + current_dungeon.getWidth() * 50 > x && start_x < x && 0 < y && current_dungeon.getHeight() * 50 > y) {
							y = y / 50;
							y = y * 50;
							if (x % 50 < 25)
								x -= (start_x % 50);
							x = x / 50;
							x = x * 50;
							if (gParty.isAdj(x/50 - (start_x/50), y/50))
								hover = true;
							if (prev_x != x || prev_y != y) {
								MouseDown = false;
								MouseUp = false;
							}
							prev_x = x;
							prev_y = y;
						}
						break;
					case REWARD:
						acceptrejectButtons[2].handleEvent(&e, 2);
						break;
					case ROOM_MAIN:
						for (int i = 0; i < TOTAL_BUTTONS; ++i)
						{
							Buttons[i].handleEvent(&e, i);
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
				//drawing state machine
				switch (state) {
				case MAIN_MENU:
					drawMainMenu();
					break;
				case PICK_CHAR1:
					drawCharScreen();
					break;
				case PICK_CHAR2:
					drawCharScreen();
					break;
				case PICK_CHAR3:
					drawCharScreen();
					break;
				case NEW_GAME:

					break;
				case TOWN_QUEST_BOARD:
					drawQuestBoard();
					drawTownMenu();
					state = TOWN_QUEST_BOARD;
					break;
				case SELECTED_QUEST:
					drawQuestBoard();
					drawTownMenu();
					drawQuestInfo();
					state = SELECTED_QUEST;
					break;
				case TOWN_CHAR_UP:
					drawCharUp();
					drawTownMenu();
					state = TOWN_CHAR_UP;

					break;
				case TOWN_SHOP:
					drawShop();
					drawTownMenu();
					state = TOWN_SHOP;

					break;
				case TOWN_PARTY_UP:
					drawPartyUp();
					drawTownMenu();
					state = TOWN_PARTY_UP;

					break;
				case DUNGEON:
					if (current_dungeon.isEnd(gParty.getX(), gParty.getY()))
						state = REWARD;
					drawDungeon();
					if (hover) {
						if (MouseDown && MouseUp) {
							gParty.moveParty(x / 50 - (start_x / 50), y / 50);
							/* prevent double clicking */
							MouseDown = false;
							MouseUp = false;
							/* remove hover after clicking */
							hover = false;
							break;
						}
						tileSST.render(x + (start_x % 50), y, &tileSpriteClips[HOVER]);
					}
					break;
				case REWARD:
					drawDungeon();
					questinfo.render(262, 150, NULL);
					acceptrejectButtons[2].render();
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

