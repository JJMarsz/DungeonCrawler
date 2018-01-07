
//#include "party.h"
//#include "dungeon.h"

#include "lib.h"
#include "LButton.h"
#include "SDL.h"
#include "dungeon.h"


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
					//drawTravel();
					break;
				case DUNGEON:
					drawDungeon();
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

