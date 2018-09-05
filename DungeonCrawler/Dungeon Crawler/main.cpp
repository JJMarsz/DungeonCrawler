
//#include "party.h"
//#include "dungeon.h"

#include "lib.h"
#include "LButton.h"
#include "SDL.h"
#include "dungeon.h"
#include "Encounter.h"
#include "Room.h"
#include "Mob.h"

rwLock gameLock;
gameData_t gameData;

void inputPoller() {
	SDL_Event e;
	int i;
	//Handle events on queue
	while (!gameData.quit) {
		//Grab lock for reading
		//gameLock.readLock();
		//Poll the event
	
		while (SDL_PollEvent(&e) != 0) {
			//User requests quit
			if (e.type == SDL_QUIT) {
				//Write that an exit conditions has been reached
				//gameLock.readUnlock();
				//gameLock.writeLock();
				gameData.quit = true;
				//gameLock.writeUnlock();
				return;
			}
			if (state == DUNGEON || state == DUNGEON_ROOM || state == GAMEOVER || state == GAMEWON || state == CREDITS) {
				switch (e.type) {
				case SDL_MOUSEBUTTONDOWN:
					gameData.MouseDown = true;
					if (e.button.button == SDL_BUTTON_RIGHT)
						gameData.MouseRight = true;
					else
						gameData.MouseRight = false;
					break;
				case SDL_MOUSEBUTTONUP:
					gameData.MouseUp = true;
					if (!(e.button.button == SDL_BUTTON_RIGHT && gameData.MouseRight == true))
						gameData.MouseRight = false;
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						gameData.ab = NOPE;
						if (state == DUNGEON_ROOM) {
							room->clearRange();
							gameData.click_handler = NULL;
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
			case CHOOSE:
				for (i = 0; i < 4; ++i) {
					if (i < 3) {
						if (gParty->getChar(i)->isAlive())
							choiceButtons[i].handleEvent(&e, i);
					}
					else
						choiceButtons[i].handleEvent(&e, i);
				}
				break;
			case DUNGEON:
				hover_info = false;
				dungeonButtons[0].handleEvent(&e, 0);
				dungeonButtons[1].handleEvent(&e, 1);
				dungeonButtons[2].handleEvent(&e, 2);
				gameData.start_x = (SCREEN_WIDTH - current_dungeon.getWidth() * 50) / 2;
				SDL_GetMouseState(&gameData.x, &gameData.y);
				gameData.hover = false;
				//within bounds
				if (gameData.start_x + current_dungeon.getWidth() * 50 > gameData.x && gameData.start_x < gameData.x && 0 < gameData.y && current_dungeon.getHeight() * 50 > gameData.y) {
					if (gameData.ab == SCOUT) {
						if (!(gameData.start_x + current_dungeon.getWidth() * 50 > gameData.x + 50) || !(current_dungeon.getHeight() * 50 > gameData.y + 50))
							break;
					}

					gameData.y = gameData.y / 50;
					gameData.y = gameData.y * 50;
					if (gameData.x % 50 < 25)
						gameData.x -= (gameData.start_x % 50);
					gameData.x = gameData.x / 50;
					gameData.x = gameData.x * 50;
					if (gParty->isAdj(gameData.x / 50 - (gameData.start_x / 50), gameData.y / 50) || gameData.ab == SCOUT)
						gameData.hover = true;
					if (gameData.prev_x != gameData.x || gameData.prev_y != gameData.y) {
						gameData.MouseDown = false;
						gameData.MouseUp = false;
						gameData.MouseRight = false;
					}
					gameData.prev_x = gameData.x;
					gameData.prev_y = gameData.y;
				}


				if (!gameData.msg_queue.empty()) {
					//new message or display current one
					//display new message or end displaying messages

					//first message from message queue
					if (gameData.message == "")
						display_message = true;
					//display message
					gameData.message = gameData.msg_queue.front();
					messageBox.loadFromRenderedText(gameData.message, gameData.color, 800);
					//shrink the message size until it fits
					if (display_message && messageBox.getWidth() >= 750) {
						int text_size = MSG_TEXT_SIZE - 1;
						while (messageBox.getWidth() >= 750) {
							TTF_Font* font = TTF_OpenFont("PT_sans.ttf", text_size);
							messageBox.setFont(font);
							messageBox.loadFromRenderedText(gameData.message, gameData.color, 800);
							TTF_CloseFont(font);
							font = NULL;
							messageBox.setFont(msg_font);
							text_size--;
						}
					}
				}
				else {
					display_message = false;
					gameData.message = "";
				}
				break;
			case DUNGEON_ROOM:
				hover_info = false;
				if (room->getCurrUnit()->getType() == CHARACTER) {
					for (int i = 0; i < TOTAL_BUTTONS; ++i) {
						Buttons[i].handleEvent(&e, i);
					}
					endTurnButton.handleEvent(&e, 0);
					gameData.start_x = (650 - room->getWidth() * 50) / 2;
					gameData.start_y = (600 - room->getHeight() * 50) / 2;
					SDL_GetMouseState(&gameData.x, &gameData.y);
					gameData.hover = false;
					//within bounds
					if (gameData.start_x + room->getWidth() * 50 > gameData.x && gameData.start_x < gameData.x && gameData.start_y < gameData.y && gameData.start_y + room->getHeight() * 50 > gameData.y) {
						if (gameData.y % 50 < 25)
							gameData.y -= (gameData.start_y % 50);
						gameData.y = gameData.y / 50;
						gameData.y = gameData.y * 50;
						if (gameData.x % 50 < 25)
							gameData.x -= (gameData.start_x % 50);
						gameData.x = gameData.x / 50;
						gameData.x = gameData.x * 50;
						if (room->getTile((gameData.x / 50) - (gameData.start_x / 50), (gameData.y / 50) - (gameData.start_y / 50))->color != NORMAL)
							gameData.hover = true;
						if (gameData.prev_x != gameData.x || gameData.prev_y != gameData.y) {
							gameData.MouseDown = false;
							gameData.MouseUp = false;
							gameData.MouseRight = false;
						}
						gameData.prev_x = gameData.x;
						gameData.prev_y = gameData.y;
					}
					else {
						gameData.MouseDown = false;
						gameData.MouseUp = false;
						gameData.MouseRight = false;
					}
				}
				else {
					gameData.MouseDown = false;
					gameData.MouseUp = false;
					gameData.MouseRight = false;
					enemyturn.render(0, 600);
					//pop off all events that occured during the enemys turn
					while (SDL_PollEvent(&e) != 0) {}
				}

				break;
			case REWARD:
				acceptrejectButtons[2].handleEvent(&e, 2);
				break;
			case GAMEOVER:
			case GAMEWON:
			case CREDITS:
				if (gameData.MouseDown && gameData.MouseUp) {
					state = MAIN_MENU;
					gameData.MouseDown = false;
					gameData.MouseUp = false;
				}
			default:
				break;
			}
			//Let go of the lock in case there are writers waiting
			//gameLock.readUnlock();
		}
	}

}

int main(int argc, char* args[])
{
	messageBox.loadFromRenderedText("", gameData.color, 800);
	
	//Start up SDL and create window
	if (!init()){
		printf("Failed to initialize!\n");
	}
	else{
		//Load media
		if (!loadMedia()){
			printf("Failed to load media!\n");
		}
		else{
			//Instantiate threads
			//While application is running
			std::thread inputThread(inputPoller);
			while (!gameData.quit){
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
					gameData.message = gameData.msg_queue.front();
					messageBox.loadFromRenderedText(gameData.message, gameData.color, 800);
					if (messageBox.getWidth() >= 750) {
						int text_size = MSG_TEXT_SIZE - 1;
						while (messageBox.getWidth() >= 750) {
							TTF_Font* font = TTF_OpenFont("PT_sans.ttf", text_size);
							messageBox.setFont(font);
							messageBox.loadFromRenderedText(gameData.message, gameData.color, 800);
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
					gameData.message = "";
					break;
				case DUNGEON:
					if (current_dungeon.isEnd(gParty->getX(), gParty->getY()))
						state = REWARD;
					drawDungeon();
					drawDungeonMenu();
					if (gameData.mod_state == MOD_UP)
						r++;
					else
						r--;
					if (r >= 1275)
						gameData.mod_state = MOD_DOWN;
					if (r <= 900)
						gameData.mod_state = MOD_UP;
					SDL_Delay(10);

					if (!gameData.msg_queue.empty()) {
						//if there are multiple messages in the queue
						//start counting until a message can timeout
						if (gameData.msg_timeout < 150) {
							gameData.msg_timeout++;
						}
						else {
							gameData.msg_timeout = 0;
							gameData.msg_queue.pop();
						}
					}
					if (gameData.hover) {
						if (gameData.MouseDown && gameData.MouseUp && !gameData.MouseRight && gameData.ab == NOPE) {
							gParty->moveParty(gameData.x / 50 - (gameData.start_x / 50), gameData.y / 50);
							if (current_dungeon.isEncounter(gParty->getX(), gParty->getY()) && !current_dungeon.getVisited(gParty->getX() + gParty->getY()*current_dungeon.getWidth()))
								handleEncounter();
							//check to see if party sees something new in area
							//done before LOS is updated to prevent player abuse by constantly rerolling
							//once a tile has been spotted, it gets 1 roll to see what it is, no more
							//current_dungeon.perceptionCheck();
							//update line of sight
							current_dungeon.updateLOS();
							/* prevent double clicking */
							gameData.MouseDown = false;
							gameData.MouseUp = false;
							/* remove hover after clicking */
							gameData.hover = false;
							break;
						}
						switch (gameData.ab) {
						case NOPE:
							tileSST.render(gameData.x + (gameData.start_x % 50), gameData.y, &tileSpriteClips[HOVER]);
							break;
						case PEEK:
							tileSST.setColor(200, 0, 0);
							tileSST.render(gameData.x + (gameData.start_x % 50), gameData.y, &tileSpriteClips[HOVER]);
							tileSST.setColor(255, 255, 255);
							if (gameData.MouseDown && gameData.MouseUp && !gameData.MouseRight) {
								if (gParty->usePeek()) {
									current_dungeon.scoutTile(gameData.x / 50 - (gameData.start_x / 50) + (gameData.y / 50)*current_dungeon.getWidth(), true);
									gameData.msg_queue.push("The party peeks the nearby room.");
									gameData.ab = NOPE;
								}
								else {
									//disable peeking
								}
								gameData.hover = false;
							}
							break;
						case SCOUT:
							tileSST.setColor(0, 200, 0);
							tileSST.render(gameData.x + (gameData.start_x % 50), gameData.y, &tileSpriteClips[MED_HOVER]);
							tileSST.setColor(255, 255, 255);
							if (gameData.MouseDown && gameData.MouseUp && !gameData.MouseRight) {
								if (gParty->useScout()) {
									current_dungeon.scoutTile(gameData.x / 50 - (gameData.start_x / 50) + (gameData.y / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile((gameData.x + 50) / 50 - (gameData.start_x / 50) + (gameData.y / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile(gameData.x / 50 - (gameData.start_x / 50) + ((gameData.y + 50) / 50)*current_dungeon.getWidth(), true);
									current_dungeon.scoutTile((gameData.x + 50) / 50 - (gameData.start_x / 50) + ((gameData.y + 50) / 50)*current_dungeon.getWidth(), true);
									gameData.msg_queue.push("The party scouts a region of the dungeon.");
									gameData.ab = NOPE;
								}
								else {
									//disable scouting

								}
								gameData.hover = false;
							}
							break;
						default:
							break;
						}

						if (gameData.MouseDown && gameData.MouseUp && !gameData.MouseRight) {
							gameData.MouseDown = false;
							gameData.MouseUp = false;
						}
					}
					if (gameData.ab == REST) {
						gameData.MouseDown = false;
						gameData.MouseUp = false;
						gameData.msg_queue.push("The party rests to regain their strength.");
						gameData.ab = NOPE;
					}
					if (display_message == true || hover_info == true) {
						messageBox.render((SCREEN_WIDTH - messageBox.getWidth()) / 2, 614);
					}
					else
						gameData.message = "";

					break;
				case DUNGEON_ROOM:
					if (gameData.mod_state == MOD_UP)
						r++;
					else
						r--;
					if (r >= 12750*2)
						gameData.mod_state = MOD_DOWN;
					if (r <= 18000)
						gameData.mod_state = MOD_UP;
					drawRoom();
					if (room->getCurrUnit()->getType() != CHARACTER) {
						//differentiate between melee and ranged
						room->getCurrUnit()->callHandler();
						gameData.message = "";
						messageBox.loadFromRenderedText(" ", gameData.color, 800);
						endTurnHandler(0);
					}

					if (gameData.hover) {
						switch (gameData.ab) {
						case ATTACK:
							if (room->getTile((gameData.x / 50) - (gameData.start_x / 50), (gameData.y / 50) - (gameData.start_y / 50))->type != ENEMY) {
								roomTilesSST.render(gameData.x + (gameData.start_x % 50), gameData.y + (gameData.start_y % 50), &roomTileClips[INVALID]);
								gameData.MouseDown = false;
								gameData.MouseUp = false;
							}
							tileSST.setColor(255, 0, 0);
							break;
						}
						tileSST.render(gameData.x + (gameData.start_x % 50), gameData.y + (gameData.start_y % 50), &tileSpriteClips[HOVER]);
						tileSST.setColor(255, 255, 255);
						if (gameData.MouseDown && gameData.MouseUp && !gameData.MouseRight && gameData.ab != NOPE) {
							//execute click handler
							if (gameData.click_handler != NULL) {
								gameData.click_handler((gameData.x / 50) - (gameData.start_x / 50) + ((gameData.y / 50) - (gameData.start_y / 50))*room->getWidth());
							}
							gameData.MouseDown = false;
							gameData.MouseUp = false;
							gameData.hover = false;
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
				case CREDITS:
					credit.render(0, 0);
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

