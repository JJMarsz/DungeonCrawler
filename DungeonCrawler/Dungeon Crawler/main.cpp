
//#include "party.h"
//#include "dungeon.h"

#include "lib.h"
#include "LButton.h"
#include "SDL.h"
#include "dungeon.h"
#include "Encounter.h"
#include "Room.h"
#include "Mob.h"

bool hover;
//Modulation components
MOD mod_state = MOD_DOWN;
std::string message = "";
SDL_Color color = { 255, 255, 255 };

int main(int argc, char* args[])
{
	int i, x=0, y=0, start_x=0, start_y=0;
	int prev_x = -1, prev_y = -1;
	bool MouseDown = false;
	bool MouseUp = false;
	bool MouseRight = false;
	int w;
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
					if(e.type == SDL_QUIT)
						quit = true;
					if (state == DUNGEON || state == DUNGEON_ROOM || state == GAMEOVER || state == GAMEWON) {
						switch (e.type) {
						case SDL_MOUSEBUTTONDOWN:
							MouseDown = true;
							if (e.button.button == SDL_BUTTON_RIGHT)
								MouseRight = true;
							else
								MouseRight = false;
							break;
						case SDL_MOUSEBUTTONUP:
							MouseUp = true;
							if (e.button.button == SDL_BUTTON_RIGHT && MouseRight == true)
								MouseRight = true;
							else
								MouseRight = false;
							break;
						case SDL_KEYDOWN:
							if (e.key.keysym.sym == SDLK_ESCAPE) {
								ab = NOPE;
								if (state == DUNGEON_ROOM) {
									room->clearRange();
									click_handler = NULL;
								}
							}
							break;
						default:
							break;
						}
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
						for (i = 0; i < 1; ++i) {
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
					case CHOOSE:
						for (i = 0; i < 4; ++i) {
							if (i < 3) {
								if(gParty->getChar(i)->isAlive())
									choiceButtons[i].handleEvent(&e, i);
							}
							else
								choiceButtons[i].handleEvent(&e, i);
						}
						break;
					case DUNGEON:
						hover_info = false;
						if (msg_queue.empty()) {
							dungeonButtons[0].handleEvent(&e, 0);
							dungeonButtons[1].handleEvent(&e, 1);
							dungeonButtons[2].handleEvent(&e, 2);
							start_x = (SCREEN_WIDTH - current_dungeon.getWidth() * 50) / 2;
							SDL_GetMouseState(&x, &y);
							hover = false;
							//within bounds
							if (start_x + current_dungeon.getWidth() * 50 > x && start_x < x && 0 < y && current_dungeon.getHeight() * 50 > y) {
								if (ab == SCOUT) {
									if (!(start_x + current_dungeon.getWidth() * 50 > x + 50) || !(current_dungeon.getHeight() * 50 > y + 50))
										break;
								}
									
								y = y / 50;
								y = y * 50;
								if (x % 50 < 25)
									x -= (start_x % 50);
								x = x / 50;
								x = x * 50;
								if (gParty->isAdj(x / 50 - (start_x / 50), y / 50) || ab == SCOUT)
									hover = true;
								if (prev_x != x || prev_y != y) {
									MouseDown = false;
									MouseUp = false;
									MouseRight = false;
								}
								prev_x = x;
								prev_y = y;
							}
						}
						else {//new message or display current one
							//display new message or end displaying messages
							if (MouseDown == true && MouseUp == true && MouseRight == false) {
								msg_queue.pop();
								if (msg_queue.empty()) {
									display_message = false;
									message = "";

								}
								else {
									message = msg_queue.front();
									messageBox.loadFromRenderedText(message, color, 800);
								}
							}
							else if(message == ""){
								//first message from message queue
								display_message = true;
								message = msg_queue.front();
								messageBox.loadFromRenderedText(message, color, 800);
							}
							if (display_message && messageBox.getWidth() >= 750) {
								int text_size = MSG_TEXT_SIZE - 1;
								while (messageBox.getWidth() >= 750) {
									TTF_Font* font = TTF_OpenFont("PT_sans.ttf", text_size);
									messageBox.setFont(font);
									messageBox.loadFromRenderedText(message, color, 800);
									TTF_CloseFont(font);
									font = NULL;
									messageBox.setFont(msg_font);
									text_size--;
								}
							}
						}
						break;
					case DUNGEON_ROOM:
						hover_info = false;
						for (int i = 0; i < TOTAL_BUTTONS; ++i){
							Buttons[i].handleEvent(&e, i);
						}
						endTurnButton.handleEvent(&e, 0);
						start_x = (650 - room->getWidth() * 50) / 2;
						start_y = (600 - room->getHeight() * 50) / 2;
						SDL_GetMouseState(&x, &y);
						hover = false;
						//within bounds
						if (start_x + room->getWidth() * 50 > x && start_x < x && start_y < y && start_y + room->getHeight() * 50 > y) {
							if (y % 50 < 25)
								y -= (start_y % 50);
							y = y / 50;
							y = y * 50;
							if (x % 50 < 25)
								x -= (start_x % 50);
							x = x / 50;
							x = x * 50;
							if (room->getTile((x / 50) - (start_x / 50), (y / 50) - (start_y / 50))->color != NORMAL)
								hover = true;
							if (prev_x != x || prev_y != y) {
								MouseDown = false;
								MouseUp = false;
								MouseRight = false;
							}
							prev_x = x;
							prev_y = y;
						}
						else {
							MouseDown = false;
							MouseUp = false;
							MouseRight = false;
						}
						break;
					case REWARD:
						acceptrejectButtons[2].handleEvent(&e, 2);
						break;
					case GAMEOVER:
					case GAMEWON:
						if (MouseDown && MouseUp) {
							state = MAIN_MENU;
						}
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
				case CHOOSE:
					message = msg_queue.front();
					messageBox.loadFromRenderedText(message, color, 800);
					if (messageBox.getWidth() >= 750) {
						int text_size = MSG_TEXT_SIZE - 1;
						while (messageBox.getWidth() >= 750) {
							TTF_Font* font = TTF_OpenFont("PT_sans.ttf", text_size);
							messageBox.setFont(font);
							messageBox.loadFromRenderedText(message, color, 800);
							TTF_CloseFont(font);
							font = NULL;
							messageBox.setFont(msg_font);
							text_size--;
						}
					}
					drawDungeon();
					drawDungeonMenu();
					drawChoiceMenu();
					messageBox.render((SCREEN_WIDTH - messageBox.getWidth()) / 2, 614);
					message = "";
					break;
				case DUNGEON:
					if (current_dungeon.isEnd(gParty->getX(), gParty->getY()))
						state = REWARD;
					drawDungeon();
					drawDungeonMenu();
					if (mod_state == MOD_UP)
						r++;
					else
						r--;
					if (r >= 1275)
						mod_state = MOD_DOWN;
					if (r <= 900)
						mod_state = MOD_UP;
					SDL_Delay(10);
					if (hover) {
						if (MouseDown && MouseUp && !MouseRight && ab == NOPE) {
							gParty->moveParty(x / 50 - (start_x / 50), y / 50);
							if (current_dungeon.isEncounter(gParty->getX(), gParty->getY()) && !current_dungeon.getVisited(gParty->getX() + gParty->getY()*current_dungeon.getWidth()))
								handleEncounter();
							//check to see if party sees something new in area
							//done before LOS is updated to prevent player abuse by constantly rerolling
							//once a tile has been spotted, it gets 1 roll to see what it is, no more
							//current_dungeon.perceptionCheck();
							//update line of sight
							current_dungeon.updateLOS();
							/* prevent double clicking */
							MouseDown = false;
							MouseUp = false;
							/* remove hover after clicking */
							hover = false;
							break;
						}
						switch (ab) {
						case NOPE:
							tileSST.render(x + (start_x % 50), y, &tileSpriteClips[HOVER]);
							break;
						case PEEK:
							tileSST.setColor(200, 0, 0);
							tileSST.render(x + (start_x % 50), y, &tileSpriteClips[HOVER]);
							tileSST.setColor(255, 255, 255);
							if (MouseDown && MouseUp && !MouseRight) {
								if (gParty->usePeek()) {
									current_dungeon.scoutTile(x / 50 - (start_x / 50) + (y / 50)*current_dungeon.getWidth(), true);
									msg_queue.push("The party peeks the nearby room.");
									ab = NOPE;
								}
								else {
									//disable peeking
								}
								hover = false;
							}
							break;
						case SCOUT:
							tileSST.setColor(0, 200, 0);
							tileSST.render(x + (start_x % 50), y, &tileSpriteClips[MED_HOVER]);
							tileSST.setColor(255, 255, 255);
							if (MouseDown && MouseUp && !MouseRight) {
								if (gParty->useScout()) {
									current_dungeon.scoutTile(x / 50 - (start_x / 50) + (y / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile((x + 50) / 50 - (start_x / 50) + (y / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile(x / 50 - (start_x / 50) + ((y + 50) / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile((x + 50) / 50 - (start_x / 50) + ((y + 50) / 50)*current_dungeon.getWidth(), true);
									msg_queue.push("The party scouts a region of the dungeon.");
									ab = NOPE;
								}
								else {
									//disable scouting

								}
								hover = false;
							}
							break;
						default:
							break;
						}

						if (MouseDown && MouseUp && !MouseRight) {
							MouseDown = false;
							MouseUp = false;
						}
					}
					if (ab == REST) {
						MouseDown = false;
						MouseUp = false;
						msg_queue.push("The party rests to regain their strength.");
						ab = NOPE;
					}
					if (display_message == true || hover_info == true) {
						messageBox.render((SCREEN_WIDTH - messageBox.getWidth()) / 2, 614);
					}
					else
						message = "";

					break;
				case DUNGEON_ROOM:
					if (mod_state == MOD_UP)
						r++;
					else
						r--;
					if (r >= 12750*2)
						mod_state = MOD_DOWN;
					if (r <= 18000)
						mod_state = MOD_UP;
					drawRoom();
					if (room->getCurrUnit()->getType() != CHARACTER) {
						//differentiate between melee and ranged
						room->getCurrUnit()->callHandler();
					}

					if (hover) {
						switch (ab) {
						case ATTACK:
							if (room->getTile((x / 50) - (start_x / 50), (y / 50) - (start_y / 50))->type != ENEMY && room->getTile((x / 50) - (start_x / 50), (y / 50) - (start_y / 50))->type != BOSS_BOI) {
								roomTilesSST.render(x + (start_x % 50), y + (start_y % 50), &roomTileClips[INVALID]);
								MouseDown = false;
								MouseUp = false;
							}
							tileSST.setColor(255, 0, 0);
							break;
						}
						tileSST.render(x + (start_x % 50), y + (start_y % 50), &tileSpriteClips[HOVER]);
						tileSST.setColor(255, 255, 255);
						if (MouseDown && MouseUp && !MouseRight && ab != NOPE) {
							//execute click handler
							if (click_handler != NULL) {
								click_handler((x / 50) - (start_x / 50) + ((y / 50) - (start_y / 50))*room->getWidth());
							}
							MouseDown = false;
							MouseUp = false;
							hover = false;
						}
					}
					if (hover_info == true) {
						messageBox.render((650 - messageBox.getWidth()) / 2, 614);
					}
					//check victory or gameover state
					room->checkState();
					break;
				case REWARD:
					drawDungeon();
					drawDungeonMenu();
					questinfo.render(262, 150, NULL);
					acceptrejectButtons[2].render();
					questTitle.render(280, 164);
					questGold.render(280, 220);
					questXP.render(280, 240);
					break;
				case GAMEOVER:
					gameover.render(0, 0);
					break;
				case GAMEWON:
					gamewon.render(0, 0);
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

